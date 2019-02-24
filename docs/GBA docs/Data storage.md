TL:DR, there's *no file system* on the GBA, meaning files/resources need to be stored through other means. Such alternatives include:
- C-arrays -- Least ideal use case, due to the array being parsed by the compiler, making for longer compile times, useful in the get-go for basic debugging until switching to a more permanent solution.
- Assembly arrays -- A much better alternative to C-arrays in terms of compile-time and security while having the same benefits. Only caveat being they require knowledge on Assembly.
- Use a file system library which emulates file systems, such as [GBFS](http://www.pineight.com/gba/#gbfs)

## Arrays belong in ROM and EWRAM

ROM(aka EWRAM to multiboot programs) is 256Kb in size, making it an ideal spot for large arrays/similarly behaving data structures. Problem is this isn't the default place the compile will look to for the arrays, with the default being a 32Kb-sized location called IWRAM. To tell the compiler to assign arrays to ROM instead of IWRAM, use the `const` keyword (`.rodata` in Assembly) for read-only data structures such as graphics and audio assets. In the case of read-write data, prepend one of the following macros to your array.

```c
#define EWRAM_DATA __attribute__((section(".ewram")))
#define IWRAM_DATA __attribute__((section(".iwram")))
#define  EWRAM_BSS __attribute__((section(".sbss")))

#define EWRAM_CODE __attribute__((section(".ewram"), long_call))
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
```

## Endianness of data

The GBA prioritises bytes in what's called a little-endian manner. This means bytes are read and stored starting from the smallest byte (right-to-left). This may not be a problem when converters are used on graphics and other binary assets, but it should go without saying that if hands-on manipulation of your binary data is expected, remember the following table:

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
.tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:black;}
.tg .tg-s6z2{text-align:center}
.tg .tg-baqh{text-align:center;vertical-align:top}
@media screen and (max-width: 767px) {.tg {width: auto !important;}.tg col {width: auto !important;}.tg-wrap {overflow-x: auto;-webkit-overflow-scrolling: touch;}}</style>
<div class="tg-wrap"><table class="tg">
  <tr>
    <td class="tg-s6z2">big u32</td>
    <td class="tg-s6z2" colspan="4">0x01020304</td>
  </tr>
  <tr>
    <td class="tg-s6z2">big u16</td>
    <td class="tg-s6z2" colspan="2">0x0102</td>
    <td class="tg-baqh" colspan="2">0x0304</td>
  </tr>
  <tr>
    <td class="tg-baqh">u8</td>
    <td class="tg-baqh">0x01</td>
    <td class="tg-baqh">0x02</td>
    <td class="tg-baqh">0x03</td>
    <td class="tg-baqh">0x04</td>
  </tr>
  <tr>
    <td class="tg-s6z2">little u16</td>
    <td class="tg-s6z2" colspan="2">0x0201</td>
    <td class="tg-baqh" colspan="2">0x0403</td>
  </tr>
  <tr>
    <td class="tg-baqh">little u32</td>
    <td class="tg-baqh" colspan="4">0x04030201</td>
  </tr>
</table></div>

# Data alignment

The GBA is a 32-bit system, meaning it is designed to perform much better with 32-bit transactions. As such, copying large amounts of data should be done in word-sized chunks (32-bit chunks).

```c
#define fooSize ...
const u16 fooData[]= { ... };

// copy data via a u16 array
u16 *dst= (u16*)vid_mem, *src= (u16*)fooData;
for(ii=0; ii<fooSize/2; ii++)
    dst[ii]= src[ii];

// copy the same data via u32 array. This results in approx. 160% faster operation
u32 *dst= (u32*)vid_mem, *src= (u32*)fooData;
for(ii=0; ii<fooSize/4; ii++)
     dst[ii]= src[ii];
```

Manual alignment through prepending `__attribute__(( aligned(4) ))` to variable declarations is helpful, as that addition will force word-alignment

## Struct alignment
There is data-alignment in place which will make data types smaller than 32 bits padded up to a word. What this means is that if a variable is less than 32 bits in size, add 0's to the end until it adds up to 32 bits before allocating data to the next location. This is known as member-alignment and keeps everything aligned and allows for easy array accessing.

The below example is a scenario showing such a problem, leading to a struct which -- would otherwise be 7 bytes in size -- is actually 12 bytes in size when padding is applied.
```c
// one byte, one word, one halfword. 7 bytes in theory
struct FOO
{
     u8 b;
    u32 w;
    u16 h;
};

// Define a FOO array
struct FOO foos[4]= 
{
    { 0x10, 0x14131211, 0x1615 }, 
    { 0x20, 0x24232221, 0x2625 }, 
    { 0x30, 0x34333231, 0x3635 }, 
    { 0x40, 0x44434241, 0x4645 }, 
};

// In memory. 4x12 bytes.
// 10 00 00 00 | 11 12 13 14 | 15 16 00 00
// 20 00 00 00 | 21 22 23 24 | 25 26 00 00
// 30 00 00 00 | 31 32 33 34 | 35 36 00 00
// 40 00 00 00 | 41 42 43 44 | 45 46 00 00
```

Data alignment can be forced manually for structs using the before-mentioned attribute, which will define how much each member should be in size. If that is not enough, `__attribute__((packed))`  will force no padding to be added, resulting in compact structs at the cost of much slower operations.

### Use word-alignment where you can
Word alignment helps the CPU perform faster data transfer of structs by use block-copy versus member-copy instructions. Do this using the above-mentioned instruction (`__attribute__((alligned(4)))`)

# Memory routines

`memcpy()` should be generally avoided for unless bouth the source and destination are word-aligned _and_ more than 16 bytes are to be copied. If these are met, go right ahead, otherwise there will be problems since it only copies byte-by-byte and certain memory locations do not allow this.
`memset()` has a similar story, in that it only works with bytes.
Final note, `sizeof()`. This is a compile-time instruction which gets the variable size. This can be an integer, a string, a pointer(the address it contains, so be careful when using this for arrays). As such, it needs to know the size of the data structure at compile-time to be useful, or else there will be logic trouble that the compiler won't realise.
