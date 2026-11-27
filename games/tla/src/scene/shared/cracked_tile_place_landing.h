/* When the leader has fallen through a cracked tile, sets him in the middle of
 */
/* the cell the two flag bytes saved, counted from cell (x, z), clears them, */
/* centres the view on him and drops him in with the screen transition effect.
 */
void scCrackedTilePlaceLanding(s32 x, s32 z, s32 effect) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 dx = scFieldGameFlagGetByte(CrackedTiles.flgByte);
	s32 dz = scFieldGameFlagGetByte(CrackedTiles.flgByte + 8);

	if(dx != 0 || dz != 0) {
		ACTOR *marker;

		syGameFlagSetByte(CrackedTiles.flgByte, 0);
		syGameFlagSetByte(CrackedTiles.flgByte + 8, 0);
		leader->x.fixed = FIELD_CELL_CENTER(dx + x);
		leader->z.fixed = FIELD_CELL_CENTER(dz + z);
		marker = state->camera;
		marker->x.fixed = leader->x.fixed;
		marker->z.fixed = leader->z.fixed;
		fdMapGridCenterOnFollowedPoint();
		scCrackedTileDropIn();
		FieldStateWork->transition = effect;
	}
}
