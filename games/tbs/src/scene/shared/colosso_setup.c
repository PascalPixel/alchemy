/// @file
/// Colosseum Final 1 and 3: setting up a race against the opponent, with
/// the race gauge at the top of the screen and the opponent running a recorded
/// path (colosso_path_opponent.c).

/// Starts the race gauge (struct ColossoRaceState, in HEAP_SLOT_COLOSSO_RACE):
/// Isaac and the opponent as playerActorId and opponentActorId, a bar
/// of segments pieces either side, and the finish at pixel (finishX, finishZ)
/// in 16.16, where the two item actors stand. Unless the party returns
/// to where it was, the opponent is put opposite Isaac across x = finishX.
void Colosso_StartRaceGauge(s32 playerActorId, s32 opponentActorId,
                            s32 segments, FIXED finishX, FIXED finishZ,
                            s32 finishItemActorIdA, s32 finishItemActorIdB) {
  struct ColossoRaceState *state;
  struct Actor *player;
  struct Actor *opponent;
  void *graphics;
  s32 vramSlot;

  state = HeapSlot_AllocEwram(HEAP_SLOT_COLOSSO_RACE,
                              sizeof(struct ColossoRaceState));
  graphics = Heap_AllocEwram(GAUGE_TILE_COUNT * TILE_SIZE_4BPP);

  state->gauge.playerActorId = playerActorId;
  state->gauge.opponentActorId = opponentActorId;
  state->gauge.finishItemActorIdA = finishItemActorIdA;
  state->gauge.finishItemActorIdB = finishItemActorIdB;
  state->gauge.segments = segments;
  state->gauge.finishX = finishX;
  state->gauge.finishZ = finishZ;

  player = Actor_Get(playerActorId);
  opponent = Actor_Get(opponentActorId);

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - player->x.fixed;
    opponent->z.fixed = player->z.fixed;
  }

  state->gauge.level = 0;
  state->gauge.hold = 0;

  FieldDecompress_TaggedLz(gColosso_GaugeGraphics, graphics);
  vramSlot = VramCache_FindFreeEntry();
  state->gauge.vramBlock = vramSlot;
  VramCache_Load((s16)vramSlot, GAUGE_TILE_COUNT * TILE_SIZE_4BPP, graphics);

  Task_AddFrame(Colosso_DrawGauge, TASK_PASS_FRAME - 10);

  Heap_Release(graphics);
}

/// Decodes the opponent's recorded path (resource) after the gauge's state and,
/// unless the party returns to where it was, has the opponent run it from the
/// start, mirrored across the finish; then starts the opponent's task.
void Colosso_StartOpponent(s32 resource) {
  struct ColossoRaceState *state = (struct ColossoRaceState *)gColossoState;
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  FieldDecompress_TaggedLz(Resource_GetTableEntry(resource), state->path);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actorId = state->gauge.opponentActorId;
    recorder->still = 0;
    recorder->pathIndex = 0;
  }
  Task_AddFrame(Colosso_UpdatePathOpponent, TASK_PASS_FRAME + 5);
}
