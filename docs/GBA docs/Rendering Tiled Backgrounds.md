# Introduction

In video modes 0-2 (tiled mode), one can have up to four backgrounds which display **tilemaps** (references to a unique tile in a legend of tiles known as a **tileset**). Tilemaps and tilesets are both stored in VRAM, where they are stored in memory spaces known as **charblocks** and **screenblocks**. Tilesets go in charblocks and tilemaps in the screenblocks.\
VRAM can be divided into a mix of either 32, 2KB (0800h) screenblocks or 4, 16KB (4000h) charblocks. When choosing which screenblocks and tileblocks to use be aware -- **they overlap eachother** -- so verify that the chosen blocks occupy the same space.

Memory | 0600:0000 | 0600:4000 | 0600:8000 | 0600:C000
--- | :---: | :---: | :---: | :---:
charblock | 0 | 1 | 2 | 3
screenblock | 0 … 7 | 8 … 15 | 16 … 23 | 24 … 31

Following is further delving into the rendering of tiled backgrounds, which may have not been (or glossed over) in the **Rendering Backgrounds and Sprites** page. If this page has not been read yet, it is encouraged to do so before continuing this page.

# Control

## Background types
There are two types of backgrounds supported by the GBA: text/regular, and rotation/affine. Which backgrounds are what is determined by the video mode set. Due to the divergence in control between the two, they will each be tackled in their own documents; this document covers regular backgrounds.

mode | BG0 | BG1 | BG2 | BG3
--- | --- | --- | --- | ---
0 | reg | reg | reg | reg
1 | reg | reg | aff | -
2 | - | - | aff | aff

## Control registers
Each background has 3 main control registers. The primary register is _REG_BGxCNT_, where _x_ indicates the background it refers to (0, 1, 2, or 3). The other 2 secondary registers are scrolling registers known as _REG_BGxHOFS_ and _REG_BGxVOFS_.

Register | length | address
--- | :---: | ---
REG_BGxCNT | 2 | 0400:0008h + 2·x
REG_BGxHOFS | 2 | 0400:0010h + 4·x
REG_BGxVOFS | 2 | 0400:0012h + 4·x

An important about the scrolling registers: they're **write-only**; scrolling position must be stored separately and the registers updated accordingly.\
Another important note: Scrolling position is relative to the map not the screen. This means the screen must be offset from the map's origin (0, 0), so scrolling defines the offset of map's origin from the screen's origin. Assuming the map's origin as **p** and the screen's position as **q**, the following describe how the offset can be obtained:\
    **q + dx = p**\
    **p - dx = q**

**Increasing the offset will move the map left and up. Decreasing it moves the map right and down.**

## Useful macros and setups

```C
// Screenblock entry
typedef u16 SCR_ENTRY;

// Affine parameter struct for backgrounds
typedef struct BG_AFFINE
{
    s16 pa, pb;
    s16 pc, pd;
    s32 dx, dy;
} ALIGN4 BG_AFFINE;

// Regular map offset coordinates
typedef struct BG_POINT
{
    s16 x, y;
} ALIGN4 BG_POINT;

// Screenblock struct
typedef SCR_ENTRY   SCREENBLOCK[1024];

// pointer mapping VRAM to screenblocks and screenblock entries. index 0 is the screenblock index and index 1 is the screenblock entry
// i.e: se_mem[y][x] is SBB y, entry x
#define se_mem          ((SCREENBLOCK*)MEM_VRAM)

// BG control register array: REG_BGCNT[x] is REG_BGxCNT
#define REG_BGCNT      ((vu16*)(REG_BASE+0x0008))

// BG offset array: REG_BG_OFS[n].x/.y is REG_BGnHOFS / REG_BGnVOFS
#define REG_BG_OFS      ((BG_POINT*)(REG_BASE+0x0010))

// BG affine params array
#define REG_BG_AFFINE   ((BG_AFFINE*)(REG_BASE+0x0000))
```

## Regular background tile-maps
Each screenblock entry in a screenblock is composed of either a 16-bit value (for regular Backgrounds) broken down into bitfields, or a 8-bit tile index (for affine backgrounds). Here is the layout for a regular background SE:

F E D C | B | A | 9 8 7 6 5 4 3 2 1 0
:---: | --- | --- | :---:
PB | VF | HF | TID

bits | name | define | description
--- | --- | --- | ---
0-9 | TID | SE_ID# | **Tile-index** of the SE.
A-B | HF, VF | SE_HFLIP, SE_VFLIP. SE_FLIP# | **Horizontal/vertical flipping flags**.
C-F | PB | SE_PALBANK# | **Palette bank** to use when in 16-color mode. Has no effect for 256-color bgs (REG_BGxCNT{6} is set).

### Screenblock layout
Each of the 32 screenblocks can store 32x32 screenblock entries (SEs). Multiple SBs can be chained to make up larger maps, achievable by defining the **screen base block** in REG_BGxCNT. The way these are chained together can be seen below:

32x32 | 64x32 | 32x64 | 64x64
:---: | :---: | :---: | :---:
0 | ____<br>\|0 \| 1\|<br>‾‾‾‾ | ___<br>\| 0 \|<br>‾‾‾<br>\| 1 \|<br>‾‾‾‾ | ___<br>\| 0 \| 1 \|<br>‾‾‾<br>\| 2 \| 3 \|<br>‾‾‾‾

This makes finding the screenblock in which a given tile resides difficult. The way to get the screenblock number is by the following procedure:
- Get the tile coordinates and divide them by the SB width and height to obtain the _Screenblock coordinates_
- Use the 2D matrix formula to find the screenblock by its coordinates (see the above table for info)
- The tile index relative to the screenblock can be obtained by first performing _tx%32_ and _ty%32_, then performing the same array formula from before, resulting in the _relative tile index_.
- Taking the above relative index, multiply it by the number of SBs times the SB size to get the _absolute tile index_ in VRAM.


TL:DR: Paste this in:

```C
/**
* Function: se_index
* ------------------
* Find the absolute SE index of a given tile.
*   \param tx: The x-coordinate of the tile
*   \param ty: The y-coordinate of the tile
*   \param pitch: The size of the Screenblock
*/
u32 se_index(u32 tx, u32 ty, u32 pitch)
{	
    u32 sbb= (ty/32)*(pitch/32) + (tx/32);
    return sbb*1024 + (ty%32)*32 + tx%32;
}
```

Another method -- optimized for specific scenarios -- is available. This works by adding a fixed offset should the tx and/or ty index be greater than 32 (bigger than one screenblock):
```C
/**
* Get the screen entry index for a tile-coord pair, assuming a screenblock
* arrangement of 2x2, 64x64-tile screenblocks.
* This is the fast (and possibly unsafe) way.
*   \param tx: The x-coordinate of the tile
*   \param ty: The y-coordinate of the tile
*   \param bgcnt    Control flags for this background (to find its size)
*/
u32 se_index_2x2_fast(u32 tx, u32 ty, u16 bgcnt)
{
    u32 n= tx + ty*32;
    if(tx >= 32)
        n += 0x03E0;
    if(ty >= 32 && (bgcnt&BG_REG_64x64)==BG_REG_64x64)
        n += 0x0400;
    return n;
}
```

## Notes on Background tiles

Unlike sprites, background tilemaps are indexed by their bit-depth; 8bpp tiles are indexed at their own rate and not at the rate of 4bpp tiles. The below table should illustrate this better:

<div class="tg-wrap"><table><tr><td>memory offset</td><td>000h</td><td>020h</td><td>040h</td><td>060h</td><td>080h</td><td>100h</td><td>...</td></tr><tr><td>4bpp tile</td><td>0</td><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>...</td></tr><tr><td>8bpp tile</td><td colspan="2">0</td><td colspan="2">1<br></td><td colspan="2">2</td><td>...</td></tr></table></div>

Background tilemaps can have access to up to 1024 tiles, despite charblock only being capable of storing 512 s-tiles (256 d-tiles). This is achieved by the **character base block** field in REG_BGxCNT. This tells the GBA where counting starts from, so you can point to a tile residing in the next charblock and it will work without a hitch... unless it's a **spriteblock**. This means that you can only do this trick on charblocks 0-2 -- charblock 3 is right next to the spriteblocks, so you'll be stuck with block 3.