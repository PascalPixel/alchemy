/* When the leader has fallen through a cracked tile, sets him in the middle of
   the metatile the two flag bytes saved, counted from metatile (x, z), clears
   them, centres the view on him and drops him in with the screen transition
   effect. */
void scCrackedTilePlaceLanding(Sint32 x, Sint32 z, Sint32 effect)
{
	ACTOR		*leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 dx = scFieldGameFlagGetByte(CrackedTiles.flgByte);
	Sint32		 dz = scFieldGameFlagGetByte(CrackedTiles.flgByte + 8);

	if(dx != 0 || dz != 0) {
		ACTOR *marker;

		syGameFlagSetByte(CrackedTiles.flgByte, 0);
		syGameFlagSetByte(CrackedTiles.flgByte + 8, 0);
		leader->x.fixed = FIELD_METATILE_CENTER(dx + x);
		leader->z.fixed = FIELD_METATILE_CENTER(dz + z);
		marker = state->camera;
		marker->x.fixed = leader->x.fixed;
		marker->z.fixed = leader->z.fixed;
		fdMapGridCenterOnFollowedPoint();
		scCrackedTileDropIn();
		FieldStateWork->transition = effect;
	}
}
