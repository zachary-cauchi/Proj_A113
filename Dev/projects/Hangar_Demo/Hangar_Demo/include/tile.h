#ifndef _TILE_H
#define _TILE_H

#include "gba_types.h"

typedef enum TILE_STATE {
	EMPTY = 0,
	SLUG_HERE = 1,
	SLUG_HIT,
	MISS_HIT
} TILE_STATE;

typedef struct TILE {
	u16 x;
	u16 y;
	TILE_STATE state;
} Tile;

/**
 * Initialises a 2-dimensional matrix of tiles by the given width and height.
 * Returns a pointer to the final array.
 * @param width The width of the final matrix.
 * @param height The height of the final matrix.
 * @return The newly created tile matrix.
 */
Tile * Tile_InitTileMatrix(u16 width, u16 height);

#define COORDS_2D(x, xMul, y) (x * xMul) + y

/**
 * Checks if the given tile has any empty tiles directly adjacent to it.
 * @param map A pointer to the tile array representing the map.
 * @param mapWidth The width of the map.
 * @param mapHeight The height of the map.
 * @param selectTile The tile to check.
 * @return true on the first tile found with a state equal to TILE_STATE.EMPTY.
 */
bool Tile_TileHasEmptyNeighbours(Tile * map, int mapWidth, int mapHeight, Tile * selectTile);

#endif //< _TILE_H
