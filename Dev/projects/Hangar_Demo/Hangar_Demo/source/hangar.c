#include "hangar.h"
#include "mappy.h"

static int base_sb;						//< The base screenblock to be used for mapping the hangar.
static HANGAR_TILE_SIZE hangar_size;	//< The size of the hangar's tiles.

static int hangar_select_curr_tile_index = 0;
static int hangar_select_x;
static int hangar_select_y;
static int prevX;			//< Previous selected slug grid tile on X-axis (offset not absolute).
static int prevY;			//< Previous selected slug grid tile on Y-axis (offset not absolute).

static int coordBuffer;
static Tile * tiles;
static Slug * slug;

void Hangar_Init(int sb, int xCoord, int yCoord, int width, int height, HANGAR_TILE_SIZE size, int slugSize) {
	
	hangar_slug_size = slugSize;
	base_sb = sb;
	hangar_size = size;
	hangar_x_coord = xCoord;
	hangar_y_coord = yCoord;
	hangar_width = width;
	hangar_height = height;

	tiles = Tile_InitTileMatrix(hangar_width, hangar_height);
	slug = Slug_CreateSlug(tiles, hangar_width, hangar_height, hangar_slug_size);

	Hangar_DrawHangar();

}

void Hangar_Update() {
	Hangar_HandleInputs();

	if (input_key_pressed(KEY_SELECT)) {

		// Create a new slug
		Slug_DestroySlug(slug);
		slug = Slug_CreateSlug(tiles, hangar_width, hangar_height, hangar_slug_size);

		Hangar_DrawHangar();
	}

	// Redraws the slug
	if (input_key_pressed(KEY_START)) {
		Hangar_DrawSlug(slug);
	}
}

void Hangar_DrawHangar() {
	// Fill the screenblock with the 
	clearScreenblock(base_sb, 13);

	// Draw the slug ground map
	Hangar_DrawGroundMap(base_sb, hangar_size, hangar_x_coord, hangar_y_coord, hangar_width, hangar_height);

	// Draw the selector tile
	Hangar_DrawHangarTile(hangar_select_x, hangar_select_y, 1, 0);

	Hangar_DrawSlug(slug);
}

void clearScreenblock(int sb, int filltile) {
	// Map each tile in the screenblock to the fill tile
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 32; y++) {
			MAP[base_sb][y][x] = filltile;
		}
	}
}

void Hangar_HandleInputs() {

	int selectX = 0;
	int selectY = 0;

	// Input handling for selected tile x-coordinate movement.
	if (input_key_pressed(KEY_LEFT)) {
		selectX--;
	}
	else if (input_key_pressed(KEY_RIGHT)) {
		selectX++;
	}

	// Input handling for selected tile y-coordinate movement
	if (input_key_pressed(KEY_UP)) {
		selectY--;
	}
	else if (input_key_pressed(KEY_DOWN)) {
		selectY++;
	}

	// Check if the user wishes to increase or decrease the hangar tile size and do so.
	if (input_key_pressed(KEY_L) && hangar_size > HANGAR_SMALL) {
		hangar_size--;
		Hangar_DrawHangar();
		Hangar_InitSelector();
		Hangar_DrawSlug(slug);
	}

	if (input_key_pressed(KEY_R) && hangar_size < HANGAR_LARGE) {
		hangar_size++;
		Hangar_DrawHangar();
		Hangar_InitSelector();
		Hangar_DrawSlug(slug);
	}

	// If either of the values are not 0, update the selector
	if (selectX || selectY) {
		Hangar_MoveSelector(selectX, selectY);
	}

}

void Hangar_MoveSelector(int xOffset, int yOffset) {

	// Validate the coordinates and update the selector

	coordBuffer = hangar_select_x + xOffset;
	if (coordBuffer > -1 && coordBuffer < hangar_width) {
		prevX = hangar_select_x;
		hangar_select_x = coordBuffer;
	}

	coordBuffer = hangar_select_y + yOffset;
	if (coordBuffer > -1 && coordBuffer < hangar_height) {
		prevY = hangar_select_y;
		hangar_select_y = coordBuffer;
	}

	Hangar_UpdateSelector();
}

void Hangar_UpdateSelector() {
	// If the selected tile has changed, mark it as selected and clear the previous tile.
	if (prevX != hangar_select_x | prevY != hangar_select_y) {

		// Clear the previous tile
		Hangar_DrawHangarTile(hangar_select_x, hangar_select_y, 1, 0);

		// Draw the new tile
		Hangar_DrawHangarTile(prevX, prevY, 2, 0);

		prevX = hangar_select_x;
		prevY = hangar_select_y;
	}
}

void Hangar_DrawSlug(Slug * s) {
	for (int i = 0; i < s->size; i++) {
		Hangar_DrawHangarTile(s->coords[i]->x, s->coords[i]->y, 8, 9);
	}
}

void Hangar_InitSelector() {
	hangar_select_x = 0;
	hangar_select_y = 0;
	prevX = 0;
	prevY = 0;
	// Draw initial starting tile
	Hangar_DrawHangarTile(0, 0, 1, 0);
}

void Hangar_DrawGroundMap(int sb, HANGAR_TILE_SIZE tileSize, int startX, int startY, int sizeX, int sizeY) {

	// Initialise the hangar variables
	hangar_size = tileSize;
	base_sb = sb;

	hangar_x_coord = startX;
	hangar_y_coord = startY;
	
	// Calculate the width and height of the hangar in GBA tiles
	hangar_total_width = sizeX * hangar_size;
	hangar_total_height = sizeY * hangar_size;

	// Save the width and height of the hangar in hangar tiles
	hangar_width = sizeX;
	hangar_height = sizeY;

	// From the given start till the given end, set each tile to the hangar ground tile
	for (int x = 0; x < sizeX; x++) {
		for (int y = 0; y < sizeY; y++) {
			Hangar_DrawHangarTile(x, y, 2, 0);
		}
	}

}

void Hangar_DrawHangarTile(int coordX, int coordY, int tileI, int altTileI) {

	int actualCoordX = hangar_x_coord + coordX * hangar_size;
	int actualCoordY = hangar_y_coord + coordY * hangar_size;

	switch (hangar_size) {

	case HANGAR_SMALL:
		MAP[base_sb][actualCoordY][actualCoordX] = tileI;
		break;

	case HANGAR_MEDIUM:
		// Draw the tiles, flipping each one so they 'face inwards' to the center.
		MAP[base_sb][actualCoordY][actualCoordX] = tileI;
		MAP[base_sb][actualCoordY][actualCoordX + 1] = tileI | BIT(10);
		MAP[base_sb][actualCoordY + 1][actualCoordX] = tileI | BIT(11);
		MAP[base_sb][actualCoordY + 1][actualCoordX + 1] = tileI | BIT(10) | BIT(11);
		break;

	case HANGAR_LARGE:

		// Draw a 3x3 tile composed of 4 corner tiles and 5 central tiles.
		// Row 1
		MAP[base_sb][actualCoordY][actualCoordX] = tileI;
		MAP[base_sb][actualCoordY][actualCoordX + 1] = altTileI;
		MAP[base_sb][actualCoordY][actualCoordX + 2] = tileI | BIT(10);
		// Row 2
		MAP[base_sb][actualCoordY + 1][actualCoordX] = altTileI;
		MAP[base_sb][actualCoordY + 1][actualCoordX + 1] = altTileI;
		MAP[base_sb][actualCoordY + 1][actualCoordX + 2] = altTileI | BIT(10);
		// Row 3
		MAP[base_sb][actualCoordY + 2][actualCoordX] = tileI | BIT(11);
		MAP[base_sb][actualCoordY + 2][actualCoordX + 1] = altTileI | BIT(11);
		MAP[base_sb][actualCoordY + 2][actualCoordX + 2] = tileI | BIT(11) | BIT(10);
		break;

	default:
		break;

	}

}