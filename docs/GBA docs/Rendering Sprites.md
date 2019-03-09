# Rendering of Sprites

There are three main steps to rendering sprites:
- Load the sprite's image data (graphics and palette) into Object VRAM and the sprite palette.
- Configure the OAM attributes for that sprite to use the right tiles and the right size.
- Enable objects in REG_DISPCNT and set the correct mapping mode.

## Sprite tiles

Sprites are composed of an arrangement of 4bpp tiles or 8bpp tiles stored in **Object VRAM**, or **OVRAM**. OVRAM is 32KB in size and encompasses the last 2 charblocks (located at `0601:0000h` `0601:4000h`). Tiles are indexed in 32-byte offsets, making the first tile located at `0601:0020h`. There is enough room for 1024 tiles, but that number drops to 512 when running in modes 3-5. <br>
Tile addressing should ideally be done through indexing (mapping OVRAM to a matrix. TONC utilises a 2D charblock-tile matrix where the first dimension is the charblock and the 2nd is the tile).

Here's a reference to the aforementioned tile-indexing dilemma:
<style type="text/css">@media screen and (max-width: 767px) {.tg {width: auto !important;}.tg col {width: auto !important;}.tg-wrap {overflow-x: auto;-webkit-overflow-scrolling: touch;}}</style><div class="tg-wrap"><table style="border-collapse:collapse;border-spacing:0" class="tg"><tr><td style="font-weight: bold"><span style="font-weight:bold">Memory</span>&nbsp;&nbsp;0601:</td><td style="font-weight: bold">0000<br></td><td style="font-weight: bold">0020</td><td style="font-weight: bold">0040</td><td style="font-weight: bold">0060</td><td style="font-weight: bold">0080</td><td style="font-weight: bold">0100</td><td style="font-weight: bold">...</td></tr><tr><td style="font-weight: bold">4bpp tile:</td><td >0</td><td >1</td><td >2</td><td >3</td><td >4<br></td><td >5<br></td><td >...</td></tr><tr><td style="font-weight: bold">8bpp tile:</td><td  colspan="2">0</td><td  colspan="2">2</td><td  colspan="2">4</td><td >...</td></tr></table></div>

### Mapping modes

1D and 2D mapping are ways for how tiles are stored in OVRAM. In 1D mode, the GBA expects all tiles for a sprite to be stored sequentially where the tile of a new row of the sprite follows the final tile of the previous row, effectively forming a long string of tiles for a single sprite. 2D mapping treats OVRAM like a table, storing the sprite tiles in a manner which visually matches how they will be rendered (if the tiles in OVRAM were layed out on a table, the first tile of the next row of the sprite will be directly beneath the first tile of the previous row.) This makes 2D mapping easier to debug faulty tile-mapping.

#### Foot note:
1D and 2D mapping do not have any significant difference on the game (beyond needing to remember adding an offset to tile indices in 2D mapping, which is something most GBA graphics editors account for)

## Object Attribute Memory

**Object Attribute Memory** (`0700:0000h`) is a 1KB long memory space holding attributes and data pertaining to the sprite/objects. These store two structure types: **OBJ_ATTR** for the regular sprites, and additionally, **OBJ_AFF** for affine sprites/transformations. Below are **aligned** structs defining the layout of these structs:

```C
typedef struct tagOBJ_ATTR
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 fill; // Padding for the OBJ_AFFINE values to stay in
} ALIGN4 OBJ_ATTR;

typedef struct OBJ_AFFINE
{
    u16 fill0[3]; // Padding for the OBJ_ATTR structs to stay in
    s16 pa;
    u16 fill1[3];
    s16 pb;
    u16 fill2[3];
    s16 pc;
    u16 fill3[3];
    s16 pd;
} ALIGN4 OBJ_AFFINE;
```

OAM is a _special_ and unique memory space in that it expects the OBJ_ATTR and OBJ_AFF structs to be interweaved together (hence the filler/padding in the above structs). The below table illustrates this weaving of values:

<div class="tg-wrap"><table><tr><td>mem (u16)</td><td>0</td><td>3</td><td>4</td><td>7<br></td><td>8</td><td>b</td><td>c</td><td>f</td></tr><tr><td>OBJ_ATTR</td><td>0 1 2</td><td></td><td>0 1 2</td><td></td><td>0 1 2</td><td></td><td>0 1 2</td><td></td></tr><tr><td>OBJ_AFF</td><td></td><td>pa</td><td></td><td>pb</td><td></td><td>pc</td><td></td><td>pd</td></tr></table></div>

## OBJ_ATTR

### Attribute 0
The most important object properties represented by attribute 0 are the Y coordinate, shape of the sprite, enabling affine transformations and setting 4-bit or 8-bit colour depth.

F E | D | C | B A | 9 8 | 7 6 5 4 3 2 1 0
:---: | :---: | :---: | :---: | :---: | :---:
Sh | CM | Mos | GM | OM | Y

| Bits 	| Name 	| Define                                                       	| Description                                                                                                                                                                                                                                                                                                              	|
| --- | --- | --- | --- |
| 0-7  	| Y    	| ATTR0_Y#                                                     	| Y coordinate. Marks the top of the sprite.                                                                                                                                                                                                                                                                               	|
| 8-9  	| OM   	| ATTR0_REG, ATTR0_AFF, ATTR0_HIDE, ATTR0_AFF_DBL. ATTR0_MODE# 	| (Affine) object mode. Use to hide the sprite or govern      affine mode.     00. Normal rendering.  01. Sprite is an affine sprite, using affine matrix         specified by attr1{9-D}  10. Disables rendering (hides the sprite)  11. Affine sprite using double rendering area. See          affine sprites for more. 	|
| A-B  	| CM   	| ATTR0_BLEND, ATTR0_WIN. ATTR0_GFX#                           	| Gfx mode. Flags for special effects.     00. Normal rendering.  01. Enables alpha blending.  10. Object is part of the object window. The sprite          itself isn't rendered, but serves as a mask for bgs and other           sprites. (I think, haven't used it yet) 11. Forbidden.                                 	|
| C    	| Mos  	| ATTR0_MOSAIC                                                 	| Enables mosaic effect.                                                                                                                                                                                                                                                                                                   	|
| D    	| CM   	| ATTR0_4BPP, ATTR0_8BPP                                       	| Color mode. 16 colors (4bpp) if cleared;      256 colors (8bpp) if set.                                                                                                                                                                                                                                                  	|
| E-F  	| Sh   	| ATTR0_SQUARE, ATTR0_WIDE, ATTR0_TALL, ATTR0_SHAPE#           	| Sprite shape. This and the sprite's size      (attr1{E-F}) determines the sprite's real size.                                                                                                                                                                                                                            	|

Below is a breakdown of the sprite dimensions achievable. The row bits represent the values for `attr0{E-F}` whereas the column bits represent `attr1{E-F}`
shape\size | 00 | 01 | 10 | 11 | ATTR1_SIZE#
--- | --- | --- | --- | --- | ---
00 | 8x8 | 16x16 | 32x32 | 64x64
01 | 16x8 | 32x8 | 32x16 | 64x32
10 | 8x16 | 8x32 | 16x32 | 32x64
ATTR0_SHAPE# | | | |

### Attribute 1

The main points of this attribute are its X coordinate, The index/reference to an OBJ_AFFINE struct for this sprite - if affine mode is enabled - otherwise, otherwise flags for flipping the sprite are available.

<div class="tg-wrap" style="text-align: center"><table><tr><td>F E</td><td>D</td><td>C</td><td>B A 9</td><td>8 7 6 5 4 3 2 1 0</td></tr><tr><td>Sz</td><td>VF</td><td>HF</td><td>-</td><td>X</td></tr><tr><td>-</td><td colspan="3">AID</td><td>-</td></tr></table></div>

| Bits | Name | Define | Description |
| --- | --- | --- | --- |
| 0-8 | X | ATTR1_X# | X coordinate. Marks left of the sprite. |
| 9-D | AID | ATTR1_AFF# | Affine index. Specifies the OAM_AFF_ENTY this sprite uses. Valid only if the affine flag (attr0{8}) is set. |
| C-D | HF, VF | ATTR1_HFLIP, ATTR1_VFLIP, ATTR1_FLIP# | Horizontal/vertical flipping flags. Used only if the affine flag (attr0) is clear; otherwise they're part of the affine index. |
| E-F | Sz | ATTR1_SIZE# | Sprite size. Kinda. Together with the shape bits, (attr0{E-F}) these determine the sprite's real size,  |

### Attribute 2

This attribute stores the sub-palette to be used if the sprite's using 4bpp, the background priority and the tile index from which to start drawing the sprite.

F E D C | B A | 9 8 7 6 5 4 3 2 1 0
:---: | :---: | :---:
PB | Pr | TID

Bits | Name | Define | Description
--- | --- | --- | ---
0-9 | TID | ATTR2_ID# | Base **tile-index** of sprite. Note that in bitmap modes this must be 512 or higher.
A-B | Pr | ATTR2_PRIO# | **Draw priority**. Higher priorities are drawn first (and therefore can be covered by later sprites and backgrounds). Sprites cover backgrounds of the same priority, and for sprites of the same priority, the higher OBJ_ATTRs are drawn first.
C-F | PB | ATTR2_PALBANK# | **Palette-bank** to use when in 16-color mode. Has no effect if the color mode flag (attr0{C}) is set.

### Double-buffering OAM data
You cannot write to OAM during VDraw, so any changes meant for OAM must be stored then written during VBlank. This can easily be taken care of by double-buffering the OAM