/* Raises the cell under the pillar a push or Move has just set down */
/* (FIELD_STATE.movedBlock), as scPillarRaise does. */
void syHeldActorRaiseCell() {
	ACTOR *held = FieldStateWork->movedBlock;
	u8 *cells;
	SHAPE_CELL *shapes;
	s32 shape;
	s32 level;
	s32 index;
	u8 newShape;

	if(held == NULL)
		return;
	cells =
		(u8 *)((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[held->layer].cells;
	shapes = (SHAPE_CELL *)Ram_MapShapeGrid +
			 (((MAP_CELL *)cells) - (MAP_CELL *)Ram_MapCellBuffer);
	shape = fdMapGridGetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
	level = fdMapGetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
	index = fdActorGetCellIndex(held);
	level = CELL_HEIGHT_STEPS(level);
	level += 4;
	newShape = fdMapShapeFindOrAdd(held->layer, shape, level);
	shapes += index;
	index <<= 2;
	shapes->shape = newShape;
	cells += index;
	((MAP_CELL *)cells)->attributeB |= MAP_CELL_JUMP | MAP_CELL_CLIMBABLE;
	cells += MAPGRID_SIZE * sizeof(MAP_CELL);
	((MAP_CELL *)cells)->attributeB |= MAP_CELL_CLIMBABLE;
}
