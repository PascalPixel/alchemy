/* @file */
/* Gaia Rock's climbing walls, which its outside and inside overlays keep a */
/* copy of. */

/* Each frame while the leader climbs a wall (fdClimbUp and fdClimbDown */
/* leave the keys alone moving him): the cell behavior of the overhead layer's
 */
/* cell where he is drawn, if one that raises a step trigger, is his step */
/* trigger. */
void scGaiaRockTriggerWallCell(void) {
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;
		MAP_STATE *map = Ram_HeapSlots->mapGrid;
		ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		s32 x = actor->x.fixed / toFIXED(16);
		s32 z = (actor->z.fixed - actor->y.fixed + (toFIXED(0.5) - 1)) /
				toFIXED(16);
		MAP_CELL *cells = map->layers[MAPGRID_LAYER_OVERHEAD].cells;
		MAP_CELL *cell = &cells[x + z * MAPGRID_SIZE];
		s32 behavior = cell->collisionCode;

		if(behavior >= CELL_BEHAVIOR_EVENT_FIRST &&
		   behavior <= CELL_BEHAVIOR_EVENT_LAST)
			state->stepTrigger = behavior;
	}
}
