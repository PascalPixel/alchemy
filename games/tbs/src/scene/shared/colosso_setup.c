/* Colosseum Final 1 and 3: setting up a race against the opponent, with the
   race gauge at the top of the screen and the opponent running a recorded path
   (colosso_path_opponent.c). */

/* Starts the race gauge (COLOSSO_RACE_STATE, in HEAP_SLOT_COLOSSO_RACE): Isaac
   and the opponent as playerActorId and opponentActorId, a bar of segments
   pieces either side, and the finish at pixel (finishX, finishZ) in 16.16,
   where the two item actors stand. Unless the party returns to where it was,
   the opponent is put opposite Isaac across x = finishX. */
void scColossoStartRaceGauge(Sint32 playerActorId, Sint32 opponentActorId,
	Sint32 segments, FIXED finishX, FIXED finishZ, Sint32 finishItemActorIdA,
	Sint32 finishItemActorIdB)
{
	COLOSSO_RACE_STATE *state;
	ACTOR			   *player;
	ACTOR			   *opponent;
	void			   *graphics;
	Sint32				vramSlot;

	state = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_COLOSSO_RACE, sizeof(COLOSSO_RACE_STATE));
	graphics = syHeapAllocateEWRAM(GAUGE_TILE_COUNT * TILE_SIZE_4BPP);

	state->gauge.playerActorId = playerActorId;
	state->gauge.opponentActorId = opponentActorId;
	state->gauge.finishItemActorIdA = finishItemActorIdA;
	state->gauge.finishItemActorIdB = finishItemActorIdB;
	state->gauge.segments = segments;
	state->gauge.finishX = finishX;
	state->gauge.finishZ = finishZ;

	player = fdActorGet(playerActorId);
	opponent = fdActorGet(opponentActorId);

	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		opponent->x.fixed = (finishX << 1) - player->x.fixed;
		opponent->z.fixed = player->z.fixed;
	}

	state->gauge.level = 0;
	state->gauge.hold = 0;

	syFieldDecompressTaggedLZ(ColossoGaugeGraphics, graphics);
	vramSlot = syVRAMCacheFindFreeEntry();
	state->gauge.vramBlock = vramSlot;
	syVRAMCacheLoad(
		(Sint16)vramSlot, GAUGE_TILE_COUNT * TILE_SIZE_4BPP, graphics);

	sySetTickEvent(scColossoDrawGauge, EVENT_PASS_TICK - 10);

	syHeapRelease(graphics);
}

/* Decodes the opponent's recorded path (resource) after the gauge's state and,
   unless the party returns to where it was, has the opponent run it from the
   start, mirrored across the finish; then starts the opponent's event. */
void scColossoStartOpponent(Sint32 resource)
{
	COLOSSO_RACE_STATE *state = (COLOSSO_RACE_STATE *)ColossoState;
	PATH_RECORDER	   *recorder = (PATH_RECORDER *)SceneLocalState;

	syFieldDecompressTaggedLZ(syResourceGetTableEntry(resource), state->path);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		recorder->mode = PATH_RECORDER_PLAY;
		recorder->mirror = 1;
		recorder->actorId = state->gauge.opponentActorId;
		recorder->still = 0;
		recorder->pathIndex = 0;
	}
	sySetTickEvent(scColossoUpdatePathOpponent, EVENT_PASS_TICK + 5);
}
