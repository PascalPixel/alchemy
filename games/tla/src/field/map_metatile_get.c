/* A map metatile's collision code and attributes, read on a layer or from the
   metatile buffer. */

#include "field/map_grid.h"

/* Returns a metatile's collision code and attributes as a halfword. */
Sint32 fdMapMetatileGetAttributes(Uint32 layer, Sint32 x, Sint32 z)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Uint8	  *metatile;

	if(map == NULL)
		return 0;
	if(layer <= 2)
		metatile = (Uint8 *)map->layers[layer].metatiles;
	else
		metatile = Ram_MapMetatileBuffer;
	metatile += (x + (z << 7)) << 2;
	return (metatile[2] << 8) | metatile[3];
}
