/* @file */
/* A map cell's collision code and attributes, read on a layer or from the */
/* cell buffer. */

#include "field/map_grid.h"

/* Returns a cell's collision code and attributes as a halfword. */
s32 fdMapCellGetAttributes(u32 layer, s32 x, s32 z) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u8 *cell;

	if(map == NULL)
		return 0;
	if(layer <= 2)
		cell = (u8 *)map->layers[layer].cells;
	else
		cell = Ram_MapCellBuffer;
	cell += (x + (z << 7)) << 2;
	return (cell[2] << 8) | cell[3];
}
