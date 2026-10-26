/// @file
/// Colosseum Final 1 and 3: setting up a race against the opponent, with
/// the race gauge at the top of the screen and the opponent running a recorded
/// path (colosso_path_opponent.h).
#include "types.h"
#include "actor.h"
#include "script_layer.h"
#include "flags.h"

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
  s32 handle;
  s32 pal;

  work = AllocHeapSlotEwram(HEAP_SLOT_COLOSSO_RACE,
                            sizeof(struct ColossoRaceWork));
  handle = (s32)AllocEwram(512);

  work->gauge.playerActorId = playerActorId;
  work->gauge.opponentActorId = opponentActorId;
  work->gauge.finishItemActorIdA = finishItemActorIdA;
  work->gauge.finishItemActorIdB = finishItemActorIdB;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  player = Actor_GetById(playerActorId);
  opponent = Actor_GetById(opponentActorId);

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - player->x.fixed;
    opponent->z.fixed = player->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  FieldDecompress_TaggedLz(gColosso_GaugeGraphics, (void *)handle);

  pal = VramCache_FindFreeEntry();
  work->gauge.vramBlock = pal;
  VramCache_Load((s16)pal, 512, (const void *)handle);

  // The native object stores this local Thumb symbol with an even value and
  // adds the state bit here. Modern GCC already includes that bit in a
  // function pointer; adding one would turn it into an ARM-state address.
#ifdef UBFIX
  FieldScheduler_AddCallback(Colosso_DrawGauge, TASK_PRIORITY_MAP - 10);
#else
  FieldScheduler_AddCallback((TaskFunc)((s32)Colosso_DrawGauge + 1),
                             TASK_PRIORITY_MAP - 10);
#endif

  Free((void *)handle);
}

/// Decodes the opponent's recorded path (resource) after the gauge's work and,
/// unless the party returns to where it was, has the opponent run it from the
/// start, mirrored across the finish; then starts the opponent's callback.
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
  FieldScheduler_AddCallback(Colosso_UpdatePathOpponent, TASK_PRIORITY_MAP + 5);
}
