/* Sets stone head `id` out of the wall at the hold under (x, y, z), in 16.16 */
/* pixels, and makes that hold solid rock. */
void syStoneHeadPlace(s32 id, s32 x, s32 y, s32 z) {
	ACTOR *actor = fdActorGet(id);
	MAP_CELL *cell = ((MAP_STATE *)Ram_HeapSlots->mapGrid)
						 ->layers[MAPGRID_LAYER_OVERHEAD]
						 .cells;
	s32 cellX = x / toFIXED(16);
	s32 cellZ = (z - y + toFIXED(1) / 2 - 1) / toFIXED(16);

	cell += cellX + cellZ * MAPGRID_SIZE;
	cell->collisionCode = CELL_BEHAVIOR_WALL;
	actor->x.fixed = (x & FIELD_CELL_MASK) + toFIXED(8);
	actor->z.fixed = (z & FIELD_CELL_MASK) + toFIXED(8);
	actor->y.fixed = actor->z.fixed - FIELD_CELL_ORIGIN(cellZ);
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprFlags(actor, 0);
}
