/* Opens again the overhead cells a marked pillar walled off */
/* (scBlocksMarkCells) and clears its mark. */
void scMagmaRockInsideClearBlockCells(ACTOR *actor) {
	MAP_STATE *map;
	MAP_CELL **cells;
	MAP_CELL *cell;
	s32 x;
	s32 z;

	if(actor->sceneByte == 0)
		return;
	map = Ram_HeapSlots->mapGrid;
	if(map == NULL)
		return;
	cells = &map->layers[MAPGRID_LAYER_OVERHEAD].cells;
	cell = *cells;
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
	cell += x + ((z + 1) << 7);
	cell->collisionCode = CELL_BEHAVIOR_NORMAL;
	cell = *cells;
	cell += x + ((z - 1) << 7);
	cell->collisionCode = CELL_BEHAVIOR_NORMAL;
	actor->sceneByte = 0;
}
