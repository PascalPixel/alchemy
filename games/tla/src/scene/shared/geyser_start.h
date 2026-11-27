/* Starts the geysers: clears their state unless the party keeps its place,
   loads the column's tiles and starts scGeyserUpdate and btGeyserDraw. */
void scGeyserStart(Sint32 unused, Sint32 firstTerrain)
{
	GEYSER_WORK *state = GEYSER_STATE;
	void		*buffer;

	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		syWordsClear((Sint32)state, sizeof(GEYSER_WORK));
	buffer = (void *)Value1((Sint32 (*)())syHeapAllocEWRAM, 2048);
	syDecompressTaggedLZ(Field_GeyserTiles, buffer);
	state->entry = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(state->entry, 2048, buffer);
	syHeapRelease(buffer);
	state->block = syHeapAllocEWRAM(GEYSER_SPRITES * 3 * sizeof(Uint32));
	state->firstTerrain = firstTerrain;
	sySetEvent((Sint32)scGeyserUpdate, EVENT_PASS_FRAME + 13);
	sySetEvent((Sint32)btGeyserDraw, EVENT_PASS_FRAME);
	fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}
