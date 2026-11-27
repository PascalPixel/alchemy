/* scMapMetatileSetAttributes, the field engine's function that replaces a map
   metatile's collision code and attributes, as the scenes that change their
   metatiles keep a copy of it (Treasure Isle's unused): included at its place
   in each scene's overlay, after the field engine's map_metatile_get.c or
   before it as the ROM has it. */

#include "types.h"
#include "system.h"
#include "field/map_grid.h"

extern MAP_METATILE MapGridMetatileBuffer[];

/* Replaces a metatile's collision code and attributes on a layer, or in the
   metatile buffer for a layer past the overhead one, and returns the old
   value. */
Sint32 scMapMetatileSetAttributes(
	Uint32 layer, Sint32 x, Sint32 z, Sint32 value)
{
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	MAP_METATILE *metatile;
	Sint32		  old;

	if(map == NULL)
		return 0;
	if(layer <= MAPGRID_LAYER_OVERHEAD)
		metatile = map->layers[layer].metatiles;
	else
		metatile = MapGridMetatileBuffer;
	metatile += x + z * MAPGRID_SIZE;
	old = (metatile->collisionCode << 8) | metatile->attributeB;
	metatile->collisionCode = value >> 8;
	metatile->attributeB = value;
	return old;
}
