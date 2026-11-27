/* Gives the cell of `layer` under an actor the shape of the ground there, */
/* `level` steps of 8 pixels (CELL_HEIGHT_STEPS) higher, or lower when */
/* negative, as a crumbling pillar sinks 4. */
void fdActorSetCellShape(s32 level, s32 layer, s32 id) {
	ACTOR *actor = fdActorGetById(id);
	u8 *shape = Ram_MapShapeGrid +
				(((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[layer].cells -
				 (MAP_CELL *)Ram_MapCellBuffer);
	s32 kind =
		fdMapGridGetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
	s32 height = CELL_HEIGHT_STEPS(
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

	height += level;

	shape += actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
			 actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
	*shape = fdMapShapeFindOrAdd(actor->layer, kind, height);
}
