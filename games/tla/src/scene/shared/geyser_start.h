/* Starts the geysers: clears their state unless the party keeps its place, */
/* loads the column's tiles and starts btGeyserUpdate and btGeyserDraw. */
void btGeyserStart(s32 unused, s32 firstTerrain) {
	GEYSER_WORK *state = GEYSER_STATE;
	void *buffer;

	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		syWordsClear((s32)state, sizeof(GEYSER_WORK));
	buffer = (void *)Value1((s32 (*)())syHeapAllocEwram, 2048);
	syDecompressTaggedLz(Field_GeyserTiles, buffer);
	state->entry = syVramCacheFindFreeEntry();
	syVramCacheLoad(state->entry, 2048, buffer);
	syHeapRelease(buffer);
	state->block = syHeapAllocEwram(GEYSER_SPRITES * 3 * sizeof(u32));
	state->firstTerrain = firstTerrain;
	sySetEvent((s32)btGeyserUpdate, TASK_PASS_FRAME + 13);
	sySetEvent((s32)btGeyserDraw, TASK_PASS_FRAME);
	fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}
