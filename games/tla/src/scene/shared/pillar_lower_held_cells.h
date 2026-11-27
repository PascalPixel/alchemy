/* Lowers the cell under the pillar ahead of the leader, or the one Move holds,
 */
/* when it is one of the listed pillars: scPillarRaise undone, the cell 32 */
/* pixels down and neither it nor the cell south of it climbable; the */
/* field state keeps the pillar as its moved block. */
void scPillarLowerHeldCells(u16 *list) {
	ACTOR *held = fdLeaderFindActorAhead();
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	SHAPE_CELL *grid;

	if(held == NULL) {
		s32 id =
			fdFindNearbyPsynergyTarget(MainRoutineStateWork.leaderActorId, 1);

		if(id != -1)
			held = fdActorGetById(id);
	}
	grid = (SHAPE_CELL *)Ram_MapShapeGrid;
	state->movedBlock = NULL;
	for(; *list != ACTOR_LIST_END; list++) {
		if(held == fdActorGetById(*list)) {
			u8 *cell = (u8 *)((MAP_STATE *)Ram_HeapSlots->mapGrid)
						   ->layers[held->layer]
						   .cells;
			SHAPE_CELL *slot =
				grid + ((MAP_CELL *)cell - (MAP_CELL *)Ram_MapCellBuffer);
			s32 kind = fdMapGridGetTerrainKind(held->layer, held->x.fixed,
											   held->z.fixed);
			s32 height = fdMapGetTerrainHeight(held->layer, held->x.fixed,
											   held->z.fixed);
			s32 index = fdActorGetCellIndex(held);
			s32 result = fdMapShapeFindOrAdd(
				held->layer, kind,
				CELL_HEIGHT_STEPS(height) - CELL_HEIGHT_STEPS(toFIXED(32)));

			slot += index;
			/* The index goes on as the cell offset, four bytes a cell. */
			index <<= 2;
			slot->shape = result;
			cell += index;
			((MAP_CELL *)cell)->attributeB &=
				~(MAP_CELL_JUMP | MAP_CELL_CLIMBABLE);
			cell += MAPGRID_SIZE * sizeof(MAP_CELL);
			((MAP_CELL *)cell)->attributeB &= ~MAP_CELL_CLIMBABLE;
			state->movedBlock = held;
		}
	}
}
