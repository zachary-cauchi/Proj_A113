# Register name: REG_BGxCNT
## Location: `0400:0008h + 2*x`
## Main purpose:
- Contains the primary controls for background _x_, including setting the starting screenblock for tile-mapping,size, etc.
- Can also be used for applying additional effects such as the mosaic effect and wrapping of _affine_ backgrounds.

| F E | D | C B A 9 8 | 7 | 6 | 5 4 | 3 2 | 1 0 |
|-----|----|-----------|----|-----|-----|-----|-----|
| Sz | Wr | SBB | CM | Mos | - | CBB | Pr |

| bits | name | define | description |
| --- | --- | --- | --- |
|  0-1 | Pr | BG_PRIO# | **Priority**. Determines drawing order of backgrounds. |
| 2-3 | CBB | BG_CBB# |  **Character Base Block**. Sets the charblock that serves as   the base for character/tile indexing. Values: 0-3. |
| 6 | Mos | BG_MOSAIC | **Mosaic flag**. Enables mosaic effect. |
| 7 | CM | BG_4BPP, BG_8BPP | **Color Mode**. 16 colors (4bpp) if cleared;  256 colors (8bpp) if set. |
| 8-C | SBB | BG_SBB# | **Screen Base Block**. Sets the screenblock that serves as   the base for screen-entry/map indexing. Values: 0-31. |
| D | Wr | BG_WRAP | **Affine Wrapping flag**. If set, affine background wrap  around at their edges. Has no effect on regular backgrounds as  they wrap around by default. |
| E-F | Sz | BG_SIZE# | **Background Size**. Regular and affine backgrounds have  different sizes available to them. The sizes, in tiles and in  pixels, can be found in table 9.5. |

Background sizes for **regular** backgrounds
Sz-flag | define | (tiles) | (pixels)
--- | --- | --- | ---
00 | BG_REG_32x32 | 32x32 | 256x256
01 | BG_REG_64x32 | 64x32 | 512x256
10 | BG_REG_32x64 | 32x64 | 256x512
11 | BG_REG_64x64 | 64x64 | 512x512

Background sizes for **affine** backgrounds
Sz-flag | define | (tiles) | (pixels)
--- | --- | --- | ---
00 | BG_AFF_16x16 | 16x16 | 128x128
01 | BG_AFF_32x32 | 32x32 | 256x256
10 | BG_AFF_64x64 | 64x64 | 512x512
11 | BG_AFF_128x128 | 128x128 | 1024x1024