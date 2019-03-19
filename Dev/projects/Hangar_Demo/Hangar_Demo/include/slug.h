#ifndef _SLUG_H
#define _SLUG_H

#include <gba_types.h>

#include "tile.h"

/**
 * A slug struct representing a slug instance.
 * 
 */
typedef struct Slug {
	u16 size;			//< Contains the size of the coords array and hence the slug.
	bool isCreated;		//< Determines whether the slug has been created or not.
	bool isDead;		//< Determines whether the slug is dead or not.
	Tile ** coords;		//< Array of pointers to the tiles the slug occupies.
} Slug;

/**
 * Initialises a slug based on the given Tile matrix of the given size.
 * @param map The tilemap to base the slug on.
 * @param maxWidth The maximum width of the slug, equal to or less than the width of the provided tilemap.
 * @param maxHeight The maximum height of the slug, equal to or less than the height of the provided tilemap.
 * @return The newly created slug.
 */
Slug * Slug_CreateSlug(Tile * map, u16 maxWidth, u16 maxHeight, u16 size);

/**
 * Destroys the given slug, freeing its memory.
 * @param slug The slug to free up.
 */
void Slug_DestroySlug(Slug * slug);

#endif //< _SLUG_H
