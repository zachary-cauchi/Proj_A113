#include "tile.h"

Tile * Tile_InitTileMatrix(u16 width, u16 height) {

	// Begin by allocating enough memory to store an array of the desired amount of tiles
	Tile * tiles = malloc(sizeof(Tile) * (width * height));

	// Initialise the array, setting the default values.
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Tile * tile = &tiles[COORDS_2D(x, height, y)];
			tile->x = x;
			tile->y = y;
			tile->state = EMPTY;
		}
	}

	return tiles;
}

bool Tile_TileHasEmptyNeighbours(Tile * map, int mapWidth, int mapHeight, Tile * selectTile) {
	int coordX, coordY;
	TILE_STATE state;

	// Iterate over all tiles adjacent to the selected block.
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			// Get the coordinates of the tile to be tested.
			coordX = selectTile->x + i;
			coordY = selectTile->y + j;

			// If the coordinates are out of range, skip to the next tile
			if (coordX < 0 || coordX >= mapWidth) continue;
			if (coordY < 0 || coordY >= mapHeight) continue;

			// If the state of this tile is EMPTY, return true
			state = map[COORDS_2D(coordX, mapHeight, coordY)].state;
			dprintf("Tile_TileHasEmptyNeighbours->Checking state of tile %hu, %hu (%d)\n", coordX, coordY, state);
			if (state == EMPTY) return true;
		}
	}
	return false;
}
