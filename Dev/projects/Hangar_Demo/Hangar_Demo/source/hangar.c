#include "hangar.h"

static int base_sb;						//< The base screenblock to be used for mapping the hangar.
static HANGAR_TILE_SIZE hangar_size;	//< The size of the hangar's tiles.

void drawGroundMap(int sb, HANGAR_TILE_SIZE tileSize, int startX, int startY, int sizeX, int sizeY) {

	// Initialise the hangar variables
	hangar_size = tileSize;
	base_sb = sb;
	
	// Calculate the width and height of the hangar in GBA tiles
	hangar_total_width = sizeX * hangar_size;
	hangar_total_height = sizeY * hangar_size;

	int endX = startX + hangar_total_width;
	int endY = startY + hangar_total_height;

	// From the given start till the given end, set each tile to the hangar ground tile
	for (int x = startX; x < endX; x += tileSize) {
		for (int y = startY; y < endY; y += tileSize) {
			drawHangarTile(x, y, 2, 0);
		}
	}

}

void drawHangarTile(int coordX, int coordY, int tileI, int altTileI) {

	switch (hangar_size) {

		case HANGAR_SMALL:
			MAP[base_sb][coordY][coordX] = tileI;
			break;

		case HANGAR_MEDIUM:
			// Draw the tiles, flipping each one so they 'face inwards' to the center.
			MAP[base_sb][coordY][coordX] = tileI;
			MAP[base_sb][coordY][coordX + 1] = tileI | BIT(10);
			MAP[base_sb][coordY + 1][coordX] = tileI | BIT(11);
			MAP[base_sb][coordY + 1][coordX + 1] = tileI | BIT(10) | BIT(11);
			break;

		case HANGAR_LARGE:

			// Draw a 3x3 tile composed of 4 corner tiles and 5 central tiles.
			// Row 1
			MAP[base_sb][coordY][coordX] = tileI;
			MAP[base_sb][coordY][coordX + 1] = altTileI;
			MAP[base_sb][coordY][coordX + 2] = tileI | BIT(10);
			// Row 2
			MAP[base_sb][coordY + 1][coordX] = altTileI;
			MAP[base_sb][coordY + 1][coordX + 1] = altTileI;
			MAP[base_sb][coordY + 1][coordX + 2] = altTileI | BIT(10);
			// Row 3
			MAP[base_sb][coordY + 2][coordX] = tileI | BIT(11);
			MAP[base_sb][coordY + 2][coordX + 1] = altTileI | BIT(11);
			MAP[base_sb][coordY + 2][coordX + 2] = tileI | BIT(11) | BIT(10);
			break;

		default:
			break;

	}

}