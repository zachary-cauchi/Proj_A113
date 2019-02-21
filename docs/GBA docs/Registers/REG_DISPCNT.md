# Register name: REG_DISPCNT
## Location: `0400:0000h`
## Main purpose:
- Set video mode to 0, 1, 2, 3, 4, or 5
- Enabling Object Mapping Mode(OAM)
- Activating backgrounds and sprites

F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3̅ | 2 1 0
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---
OW | W1 | W0 | Obj | BG3 | BG2 | BG1 | BG0 | FB | OM | HB | PS | GB | Mode

bits | name | define | description
:---: | --- | --- | ---
0-2 | Mode | DCNT_MODEx. DCNT_MODE# | Sets video mode. 0, 1, 2 are tiled modes; 3, 4, 5 are bitmap modes.
3 | GB | DCNT_GB | Is set if cartridge is a GBC game. Read-only.
4 | PS | DCNT_PAGE | Page select. Modes 4 and 5 can use page flipping for smoother animation. This bit selects the displayed page (and allowing the other one to be drawn on without artifacts).
5 | HB | DCNT_OAM_HBL | Allows access to OAM in an HBlank. OAM is normally locked in VDraw. Will reduce the amount of sprite pixels rendered per line.
6 | OM | DCNT_OBJ_1D | Object mapping mode. Tile memory can be seen as a 32x32 matrix of tiles. When sprites are composed of multiple tiles high, this bit tells whether the next row of tiles lies beneath the previous, in correspondence with the matrix structure (2D mapping, OM=0), or right next to it, so that memory is arranged as an array of sprites (1D mapping OM=1). More on this in the sprite chapter.
7 | FB | DCNT_BLANK | Force a screen blank.
8-B | BG0-BG3, Obj | DCNT_BGx, DCNT_OBJ. DCNT_LAYER# | Enables rendering of the corresponding background and sprites.
D-F | W0-OW | DCNT_WINx, DCNT_WINOBJ | Enables the use of windows 0, 1 and Object window, respectively. Windows can be used to mask out certain areas (like the lamp did in Zelda:LTTP). 