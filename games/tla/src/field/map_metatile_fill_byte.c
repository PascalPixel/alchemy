/* Filling a rectangle of a map layer's metatiles with one collision code. */

#include "field/map_grid.h"

extern MAP_METATILE MapGridMetatileBuffer[];

/* Sets the collision code of each metatile in a layer rectangle to value.
   Layers 3 and above use the shared buffer. */
Sint32 fdMapFillLayerRectCollision(
	Uint32 layer, Uint32 x, Uint32 y, Uint32 width, Uint32 height, Sint32 value)
{
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	MAP_METATILE *metatiles;

	if(map == NULL)
		return 0;
	if(layer <= 2)
		metatiles = map->layers[layer].metatiles;
	else
		metatiles = MapGridMetatileBuffer;
	metatiles += x + (y << 7);
	/* x and y, no longer needed, count the rows and the columns. */
	for(x = 0; x < height; x++) {
		MAP_METATILE *metatile = metatiles + (x << 7);

		for(y = 0; y < width; y++, metatile++)
			metatile->collisionCode = value;
	}
	return 0;
}
