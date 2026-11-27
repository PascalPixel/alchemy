/* @file */
/* scMapCellSetAttributes, the field engine's function that replaces a map */
/* cell's collision code and attributes, as the scenes that change their cells
 */
/* keep a copy of it (Treasure Isle's unused): included at its place in each */
/* scene's overlay, after the field engine's map_cell_get.c or before it as the
 */
/* ROM has it. */

#include "types.h"
#include "system.h"
#include "field/map_grid.h"

extern MAP_CELL MapGridCellBuffer[];

/* Replaces a cell's collision code and attributes on a layer, or in the cell */
/* buffer for a layer past the overhead one, and returns the old value. */
s32 scMapCellSetAttributes(u32 layer, s32 x, s32 z, s32 value) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_CELL *cell;
	s32 old;

	if(map == NULL)
		return 0;
	if(layer <= MAPGRID_LAYER_OVERHEAD)
		cell = map->layers[layer].cells;
	else
		cell = MapGridCellBuffer;
	cell += x + z * MAPGRID_SIZE;
	old = (cell->collisionCode << 8) | cell->attributeB;
	cell->collisionCode = value >> 8;
	cell->attributeB = value;
	return old;
}
