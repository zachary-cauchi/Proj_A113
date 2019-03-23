#ifndef _HANGAR_H
#define _HANGAR_H

#include "gba_video.h"
#include "input_controller.h"

// An enum describing the sizes of hangar tiles.
typedef enum HANGER_TILE_SIZE {
	HANGAR_NONE = 0,
	HANGAR_SMALL,
	HANGAR_MEDIUM,
	HANGAR_LARGE
} HANGAR_TILE_SIZE;

int hangar_total_width;					//< Hangar width measured in GBA tiles.
int hangar_total_height;				//< Hangar height measured in GBA tiles.

/**
 * Draws the groundmap for the hangar.
 * This initialises the hangar, setting the base screenblock and hangar size members in the process,
 * since a reassignment to either requires redrawing the hangar.
 * @param sb The base screenblock to render to.
 * @param tileSize The size of each hangar tile.
 * @param startX The start X-coordinate of the hangar in GBA tiles.
 * @param startY The start Y-coordinate of the hangar in GBA tiles.
 * @param sizeX The width of the hangar, in hangar tiles.
 * @param sizeY The height of the hangar, in hangar tiles.
 */
void drawGroundMap(int sb, HANGAR_TILE_SIZE tileSize, int startX, int startY, int sizeX, int sizeY);

/**
 * Draws a hangar tile on the hangar.
 * Initialises a hangar tile on the hangar using the given tiles and built-in drawing subroutines.
 * @param coordX The X-coordinate of the hangar tile relative to the hangar.
 * @param coordY The Y-coordinate of the hangar tile relative to the hangar.
 * @param tileI The primary tile to use in drawing the hangar tile. Prominent in all tile sizes.
 * @param altTileI The secondary tile to use in drawing the hangar tile. Only used in HANGAR_LARGE.
 */
void drawHangarTile(int coordX, int coordY, int tileI, int altTileI);

#endif