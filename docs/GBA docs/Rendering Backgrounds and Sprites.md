# Summary
Three stages in the rendering process are **control**, **mapping**, and **image data**. Each property about sprites and backgrounds can be seen below: Things can be a bit difficult to understand at first (even after several times), but the documentation needed to help comprehend it can be found below or on the TONC website.


**Subject** | **Backgrounds** | **Sprites**
--- | --- | ---
Number | 4 (2 affine) | 128 (32 affine)
Max size | **reg**: 512x512 <br> **aff**: 1024x1024 | 64x64
Control | `REG_BGxCNT` | OAM
Base tile block | 0-3 | 4
Available tiles ids | **reg**: 0-1023 <br> **aff**: 0-255 | **modes** 0-2: 0-1023 <br> **modes** 3-5: 512-1023
Tile memory offsets | Per tile size: <br> **4bpp**: start= base + _id_*32 <br> **8bpp**: start= base + _id_*64 | Always per 4bpp tile size: <br> start= base + _id_*32
Mapping | **reg**: the full map is divided into map-blocks of 32×32 tegels. (banked map) <br> **aff**: one matrix of tegels, just like a normal bitmap (flat map) | If a sprite is m × n tiles in size: <br> **1D mapping**: the _m*n_ successive tiles are used, starting at _id_ <br> **2D mapping**: tile-blocks are 32×32 matrices; the used tiles are the _n_ columns of the _m_ rows of the matrix, starting at _id_.
Flipping | Each tile can be flipped individually | Flips the whole sprite
Palette | `0500:0000h` | `0500:0200h`

## Control stage
At this stage, the control registers are set in preparation for our graphical elements(backgrounds and sprites). The registers used cover the what will be used (such as backgrounds and sprites), effects to be applied (blending, affine transformations, etc) and, most importantly, the modes (the `REG_DISPCNT` main control register). There are 3 main steps to this stage:
- Setting the right bits for the needed setup in `REG_DISPCNT`.
- Setting the controls for the background desired through the `REG_BGxCNT` (where _x_ is the number of the desired background).
- Setting the sprites values in **Object Attribute Memory**

## Mapping stage

This stage involves telling the GBA which tiles should be rendered and where they should be rendered on-screen. An illustration of this can be seen [here](http://www.coranac.com/tonc/text/objbg.htm#sec-map).

### Backgrounds
The background makes use of a **tile-map** instead of the usual bitmaps seen in modes 3-5. This tile-map is a matrix composed of **screenblock entries** (tile-indices which serve as _pointers_ to the actual tiles, located elsewhere) which serve as references to the tiles (image-data/actual graphics). The aforementioned tile-map is known as a screenblock, hence the name 'screenblock entries' for the elements inside. Each SE contains a tile-index, bits for both horizontal and vertical flipping, and an optional palette index (for 16-colour backgrounds only).

### Sprites
Sprites follow the same concept of screenblock entries and tile indices as backgrounds with a few differences: The sprite has only one tile-index for all the sprites tiles -- The GBA then reads in the other tiles for the sprite based on its size, shape, and the **sprite mapping mode**. Depending on `REG_DISPCNT{6}`, the mapping can refer to the sprite tiles being stored in either a 1D _sequential_ array of tiles or a 2D array. Following the tile-index are the same arrangement of bits for horiz/vert flipping and possibly a palette index (all of which also apply to the whole sprite instead of a single tile). If affine transformations are enabled, the affine matrix also forms part of the mapping process for sprites.

## Image data

### Tiles
As mentioned above, **tiles** are what make the scenes. A tile is typically an 8x8 bitmap and come in 4bpp or 8bpp variants, making for 16 colours or 256 colours respectively. These can be called **s-tiles** or **d-tiles** to distinguish between the two. Tiles are 32 bytes and 64 bytes in size, respectively. s-tiles are the default tile, unless specified.When stored in VRAM, tile pixels are stored sequentially rather than in an 8x8 fashion.

### Tile blocks, a.k.a. charblocks
Tile data(graphic data) is stored in **charblocks** (a term synonymous with tile-blocks, but kept due to more 'traditional' uses of the word for the same purpose). Charblocks are 16KB (0400h) in size, making for a total of 6 charblocks for the 96KB of VRAM. The charblocks 0-3 are for backgrounds and 4-5 for sprites. Tiled backgrounds start their count at a **character base block**, which can be seen as the 0th tile in a background's charblock. This is indicated in `REG_BGxCNT{2-3}`.\
For sprites, however, tile-indexing starts from block 4 at address `0601:0000h`. Another caveat sprite tile-indexing faces is that tile-inexing is done in 32-bit increments (in other words, by every s-tile) and applies for s-tiles and d-tiles alike. Backgrounds do not have this limitation and can be indexed based on the tile size chosen.

#### Tricks with background tile-indexing
Tile-indices for both sprites and backgrounds are 10 bits each, allowing for 1024 indices. Since only 512 s-tiles can be present in a charblock, 2 tiles can be referenced in the same index value by concatenating the separate indices. For d-tiles, since there can only be 256 d-tiles in a single charblock, allows for 4 indices in just one! This allows for multiple tiles to be referenced at a time for backgrounds so long as the indices do not point to sprite charblocks.

#### Note on bitmaps and sprites
When in bitmap mode, only charblock 5 can be used for sprites -- the bitmap modes' memory allocation extends beyond the background charblocks and into sprite charblock 4.

### Defining your tiles
It is beneficial to define types for tiles and their memory locations. Using `typedef`, VRAM can be accessed on a per-tile basis versus through address arithmetic.

```C
// tile 8x8@4bpp: 32bytes; 8 ints
typedef struct { u32 data[8];  } TILE, TILE4;
// d-tile: double-sized tile (8bpp)
typedef struct { u32 data[16]; } TILE8;
// tile block: 32x16 tiles, 16x16 d-tiles
typedef TILE  CHARBLOCK[512];
typedef TILE8 CHARBLOCK8[256];

#define tile_mem  ( (CHARBLOCK*)0x06000000)
#define tile8_mem ((CHARBLOCK8*)0x06000000)

//In code somewhere
TILE *ptr= &tile_mem[4][12];   // block 4 (== lower object block), tile 12

// Copy a tile from data to sprite-mem, tile 12
tile_mem[4][12] = *(TILE*)spriteData;
```

## Palettes

Backgrounds and sprites have their own palettes, located at `0500:0000h` and `0500:0200h` respectively. Each stores 256 15-bit colour entries that can be referenced by tile data.\
In 8-bit mode, tiles use palette indices to define their pixel colours. In 4-bit mode, the pixel value is a 4-bit palette index value and a 4-bit index for the **palbank** (found in the sprite attribute or the upper 4 bits of the tile). Transparent pixels can be made by using a pixel value of 0.

# Final word on naming schemes

Both the GBA development community and TONC use different naming schemes for the below subjects. To keep distance between the two and assist in the reading of documentation from both sides, here are the common names and their names:

Subject | Traditional | Tonc
--- | --- | ---
Sprite and bg image data | tiles | tiles
Tile-map entries | tiles (can you feel the confusion?) | screenblock entries / SE
Matrix for transformations | Rot/Scale matrix | affine matrix / **P**
Sprite types | ?? vs Rot/Scale | regular vs affine
Background types | text vs rot | regular vs affine
Depository for sprite tiles (`0601:0000`) | `OAMData` (i.e., not the real OAM, which is at `0700:0000`) | `tile_mem_obj`
OAM (`0700:0000`) | `OAMData` or `OAMMem` | `oam_mem`