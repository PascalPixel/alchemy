/* Filling a rectangle of a map layer's metatiles with one collision code, as a
   pushed actor marks the metatiles it covers (push_target_push.h). Several
   scenes' overlays include a copy. */

#include "field/map_grid.h"

/* Sets the collision code of each metatile in a layer rectangle to value. */
void fdMapGridFillCollisionRectangle(
	Sint32 layer, Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 value)
{
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	MAP_METATILE *metatiles = map->layers[layer].metatiles;
	Uint32		  row;
	Uint32		  column;

	metatiles += x + (y << 7);
	for(row = 0; row < height; row++) {
		MAP_METATILE *metatile = metatiles + (row << 7);

		for(column = 0; column < width; column++) {
			metatile->collisionCode = value;
			metatile++;
		}
	}
}
