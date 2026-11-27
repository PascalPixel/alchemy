// Gaia Rock's climbing walls, which its outside and inside overlays keep a copy of.

// Each tick while the leader climbs a wall (fdClimbUp and fdClimbDown leave the keys alone moving
// him): the metatile behavior of the overhead layer's metatile where he is drawn, if one that
// raises a step cue, is his step cue.
void scGaiaRockCueWallMetatile(void)
{
	if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS)
	{
		FIELD_STATE  *state = RamHeapSlots->fieldState;
		MAP_STATE    *map   = RamHeapSlots->mapGrid;
		ACTOR        *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		Sint32        x     = actor->x.fixed / toFIXED(16);
		Sint32        z     = (actor->z.fixed - actor->y.fixed + (toFIXED(0.5) - 1)) / toFIXED(16);
		MAP_METATILE *metatiles = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
		MAP_METATILE *metatile  = &metatiles[x + z * MAPGRID_SIZE];
		Sint32        behavior  = metatile->collisionCode;

		if (behavior >= METATILE_BEHAVIOR_CUE_FIRST && behavior <= METATILE_BEHAVIOR_CUE_LAST)
			state->stepCue = behavior;
	}
}
