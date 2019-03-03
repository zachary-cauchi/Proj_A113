# Bitfield macros
When dealing with registers, bit-specific locations in larger data types, etc, it often makes sense to use bitfield macros to aleviate the burdens of handling such operations. \
As was mentioned in the conventions outline, register fields making use of these bitfield macros are suffixed with a '#' after their define name. Each field has a `MASK` for the field-specific bits, a `SHIFT` for the starting bit of the field, and a bitwise setter with parameter `_n`.

Here's an example using the tile id field of OBJ_ATTR2:

```C
#define ATTR2_ID_MASK       0x03FF
#define ATTR2_ID_SHIFT      0
#define ATTR2_ID(n)         ((n)<<ATTR2_ID_SHIFT)
```

Another set of macros can be used as getters and setters for specific fields. `BF_GET` retrieves the value from the bitfield and not the rest of the value . `BF_SET` sets the bitfield in a value.

```C
/**
* Bitfield macros for modifying/accessing bitfields from values
* x - The value to be assigned/compared against
* y - A value from which to retrieve the bitfield
* name - The name of the bitfield for calling its specific macros
*/
#define BF_PREP(x, name)         ( ((x)<<name##_SHIFT)& name##_MASK  )
#define BF_GET(x, name)          ( ((x) & name##_MASK)>> name##_SHIFT )
#define BF_SET(y, x, name)       (y = ((y)&~name##_MASK) | BF_PREP(x,name) )

// These don't shift the values, useful when the values themselves are already shifted
#define BF_PREP2(x, name)        ( (x) & name##_MASK )
#define BF_GET2(y, name)         ( (y) & name##_MASK )
#define BF_SET2(y, x, name)      (y = ((y)&~name##_MASK) | BF_PREP2(x, name) )
```

Finally, one can use 'build' macros, which piece together values from individual bitfield values. They may not be as frequently used as individual bitfield setters, but as handy 'constructors', they can be a pretty elegant solution. As an example demo of what these macros would look like, here's an implementation for OBJ_ATTR0 of a standard OBJ_ATTR struct in OAM:

```C
// Attribute 0
#define ATTR0_BUILD(y, shape, bpp, mode, mos, bld, win)             \
(                                                                   \
    ((y)&255) | (((mode)&3)<<8) | (((bld)&1)<<10) | (((win)&1)<<11) \
    | (((mos)&1)<<12) | (((bpp)&8)<<10) | (((shape)&3)<<14)         \
)

// Attribute 1, regular sprites
#define ATTR1_BUILD_R(x, size, hflip, vflip)         \
( ((x)&511) | (((hflip)&1)<<12) | (((vflip)&1)<<13) | (((size)&3)<<14) )

// Attribute 1, affine sprites
#define ATTR1_BUILD_A(x, size, aff_id)               \
( ((x)&511) | (((aff_id)&31)<<9) | (((size)&3)<<14) )

Attribute 2
#define ATTR2_BUILD(id, pbank, prio)                 \
( ((id)&0x3FF) | (((pbank)&15)<<12) | (((prio)&3)<<10) )
```