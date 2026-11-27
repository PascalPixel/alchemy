/* Starts the geysers: clears their state unless the party keeps its place,
   loads the column's tiles and starts scGeyserUpdate and btGeyserDraw. */
void scGeyserStart(Sint32 unused, Sint32 firstTerrain)
{
	GEYSER_WORK *state = GEYSER_STATE;
	void		*buf;

	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		syWordsClear((Sint32)state, sizeof(GEYSER_WORK));
	buf = (void *)Value1((Sint32 (*)())syHeapAllocEWRAM, 2048);
	syDecompressTaggedLZ(FieldGeyserTiles, buf);
	state->entry = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->entry, 2048, buf);
	syHeapRelease(buf);
	state->block = syHeapAllocEWRAM(GEYSER_SPRITES * 3 * sizeof(Uint32));
	state->firstTerrain = firstTerrain;
	sySetEvent((Sint32)scGeyserUpdate, EVENT_PASS_TICK + 13);
	sySetEvent((Sint32)btGeyserDraw, EVENT_PASS_TICK);
	fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}
