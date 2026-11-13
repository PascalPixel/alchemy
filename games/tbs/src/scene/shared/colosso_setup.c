/// @file
/// Colosseum Final 1 and 3: setting up a race against the opponent, with
/// the race gauge at the top of the screen and the opponent running a recorded
/// path (colosso_path_opponent.c).

/// Starts the race gauge (struct ColossoRaceWork, in HEAP_SLOT_COLOSSO_RACE):
/// Isaac and the opponent as playerActorId and opponentActorId, a bar
/// of segments pieces either side, and the finish at pixel (finishX, finishZ)
/// in 16.16, where the two item actors stand. Unless the party returns
/// to where it was, the opponent is put opposite Isaac across x = finishX.
void Colosso_StartRaceGauge(s32 playerActorId, s32 opponentActorId,
                            s32 segments, s32 finishX, s32 finishZ,
                            s32 finishItemActorIdA, s32 finishItemActorIdB) {
  struct ColossoRaceWork *work;
  struct Actor *player;
  struct Actor *opponent;
  void *graphics;
  s32 vramSlot;

  work = HeapSlot_AllocEwram(HEAP_SLOT_COLOSSO_RACE,
                             sizeof(struct ColossoRaceWork));
  graphics = Heap_AllocEwram(512);

  work->gauge.playerActorId = playerActorId;
  work->gauge.opponentActorId = opponentActorId;
  work->gauge.finishItemActorIdA = finishItemActorIdA;
  work->gauge.finishItemActorIdB = finishItemActorIdB;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  player = Actor_Get(playerActorId);
  opponent = Actor_Get(opponentActorId);

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - player->x.fixed;
    opponent->z.fixed = player->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  FieldDecompress_TaggedLz(gColosso_GaugeGraphics, graphics);
  vramSlot = VramCache_FindFreeEntry();
  work->gauge.vramBlock = vramSlot;
  VramCache_Load((s16)vramSlot, 512, graphics);

  // UB: the Thumb bit is added to the routine's address by hand; where the
  // address already carries it, this makes an ARM address.
#ifdef UBFIX
  Task_AddFrame(Colosso_DrawGauge, TASK_PASS_FRAME - 10);
#else
  Task_AddFrame((TaskFunc)((s32)Colosso_DrawGauge + 1), TASK_PASS_FRAME - 10);
#endif

  Heap_Release(graphics);
}

/// Decodes the opponent's recorded path (resource) after the gauge's work and,
/// unless the party returns to where it was, has the opponent run it from the
/// start, mirrored across the finish; then starts the opponent's task.
void Colosso_StartOpponent(s32 resource) {
  struct ColossoRaceWork *work = (struct ColossoRaceWork *)gColossoWork;
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  FieldDecompress_TaggedLz(Resource_GetTableEntry(resource), work->path);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actorId = work->gauge.opponentActorId;
    recorder->still = 0;
    recorder->pathIndex = 0;
  }
  Task_AddFrame(Colosso_UpdatePathOpponent, TASK_PASS_FRAME + 5);
}
