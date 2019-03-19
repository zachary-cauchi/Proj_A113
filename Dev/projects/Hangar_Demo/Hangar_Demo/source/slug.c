#include "slug.h"
#include "randlib.h"
#include "mappy.h"

Slug * Slug_CreateSlug(Tile * map, u16 maxWidth, u16 maxHeight, u16 size) {

	int randX;					//< Random x-coordinate buffer
	int randY;					//< Random y-coordinate buffer
	u32 selectedTile = 0;		//< The currently active tile
	bool coordsValid = true;	//< Used for checking if the generated coordinates are within range or not

	Slug * slug = malloc(sizeof * slug);				//< The slug to create
	Tile ** coords = malloc(sizeof(Tile *) * size);		//< The coordinates of the slug

	// Getting the coordinates for the first tile
	randX = qran_range(0, maxWidth);
	randY = qran_range(0, maxHeight);

	// Assigning the first tile to the tile array
	coords[0] = &map[COORDS_2D(randX, maxHeight, randY)];
	coords[0]->state = SLUG_HERE;

	/**
	 * 1. Check if the selected starting tile has empty neighbours.
	 * 1.1. If not, select a different starting tile (at random).
	 * 2. Select a random set of coordinates.
	 * 2.1. Validate those coordinates, making sure they are in range.
	 * 2.2. If the tile is not empty, reiterate until an empty tile is chosen.
	 * 3. Add the tile to the coordinates list.
	 * 4. Mark the newly selected tile as full.
	 * 5. Set the selected starting tile to the new tile.
	 */

	for (int tile = 1; tile < size; tile++) {

		// 1. Checking if tile has empty neighbours.
		while (!Tile_TileHasEmptyNeighbours(map, maxWidth, maxHeight, coords[selectedTile])) {
			// 1.1. Reassigning selected tile to a previously completed tile.
			selectedTile = qran_range(0, tile);
		}

		do {

			// Reset the coordinates 
			coordsValid = true;

			// 2. Select random coords.
			// Pick a new adjacent tile around the previous tile's coordinates
			randX = ((qran() % 3) - 1);
			randY = ((qran() % 3) - 1);

			randX += coords[selectedTile]->x;
			randY += coords[selectedTile]->y;

			dprintf("Slug_CreateSlug->Chose new coordinates %d, %d\n", randX, randY);

			// 2.1. Validate the coordinates are in-range
			if (randX < 0 || randX >= maxWidth || randY < 0 || randY >= maxHeight) {
				coordsValid = false;
			}

			// 2.2. Check if chosen tile is not empty and coords are valid.
		} while (map[COORDS_2D(randX, maxHeight, randY)].state != EMPTY || !coordsValid);

		dprintf("Slug_CreateSlug->Assigning new tile %d, %d\n", randX, randY);

		// 3. Add the tile to the list of slug tiles.
		coords[tile] = &map[COORDS_2D(randX, maxHeight, randY)];
		// 4. Mark the added tile as full.
		coords[tile]->state = SLUG_HERE;

		// 5. Set the new selected tile.
		selectedTile = tile;

	}

	slug->coords = coords;	//< Assign the new coords array to the slug.
	slug->size = size;		//< Assign the size of the slug.
	slug->isCreated = true;	//< Mark the slug as craeted.

	// Testing of the assigned coords tiles
	for (int tile = 0; tile < size; tile++) {
		dprintf("Slug_CreateSlug->Testing newly assigned Tile %d %d at location %p\n", slug->coords[tile]->x, slug->coords[tile]->y, slug->coords[tile]);
	}

	return slug;

}

void Slug_DestroySlug(Slug * slug) {

	// Iterate over all occupied tiles and set them to EMPTY.
	for (int i = 0; i < slug->size; i++) {
		slug->coords[i]->state = EMPTY;
	}

	// Free the coordinates pointer array and then the slug.
	free(slug->coords);
	free(slug);
}
