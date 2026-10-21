/// @file
/// Colosseum Final 1 and 3: setting up a race against the rival, with
/// the race gauge at the top of the screen and the rival running a recorded
/// path (colosso_path_rival.h).
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

/// Starts the race gauge (struct ColossoRaceWork, in HEAP_SLOT_COLOSSO_RACE):
/// Isaac and the rival as playerActor and rivalActor, a bar of segments pieces
/// either side, and the finish at pixel (finishX, finishZ) in 16.16, where the
/// two item actors stand. Unless the party returns to where it was, the rival
/// is put opposite Isaac across x = finishX.
void Colosso_StartRaceGauge(s32 playerActor, s32 rivalActor, s32 segments,
                            s32 finishX, s32 finishZ, s32 finishItemActorA,
                            s32 finishItemActorB) {

  struct ColossoRaceWork *work;
  struct FieldActor *player;
  struct FieldActor *rival;
  s32 handle;
  s32 pal;

  work = Runtime_AllocateBlock(HEAP_SLOT_COLOSSO_RACE,
                               sizeof(struct ColossoRaceWork));
  handle = (s32)Runtime_BumpAllocateAlternatePool(512);

  work->gauge.playerActor = playerActor;
  work->gauge.rivalActor = rivalActor;
  work->gauge.finishItemActorA = finishItemActorA;
  work->gauge.finishItemActorB = finishItemActorB;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  player = Object_GetById(playerActor);
  rival = Object_GetById(rivalActor);

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    rival->x.fixed = (finishX << 1) - player->x.fixed;
    rival->z.fixed = player->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  FieldResource_DecodeType01(gColosso_GaugeGraphics, (void *)handle);

  pal = Resource_FindFreeEntry();
  work->gauge.vramBlock = pal;
  Engine_VramLoad((s16)pal, 512, (const void *)handle);

  // The native object stores this local Thumb symbol with an even value and
  // adds the state bit here. Modern GCC already includes that bit in a
  // function pointer; adding one would turn it into an ARM-state address.
#ifdef UBFIX
  FieldScheduler_AddCallback(Colosso_DrawGauge, 0xc76);
#else
  FieldScheduler_AddCallback((SchedulerCallback)((s32)Colosso_DrawGauge + 1),
                             0xc76);
#endif

  Runtime_BumpFree((void *)handle);
}

/// Decodes the rival's recorded path (resource) after the gauge's work and,
/// unless the party returns to where it was, has the rival run it from the
/// start, mirrored across the finish; then starts the rival's callback.
void Colosso_StartRival(s32 resource) {
  struct ColossoRaceWork *work = (struct ColossoRaceWork *)gColossoWork;
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

  FieldResource_DecodeType01(Resource_GetTableEntry(resource), work->path);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actor = work->gauge.rivalActor;
    recorder->still = 0;
    recorder->pos = 0;
  }
  FieldScheduler_AddCallback(Colosso_UpdatePathRival, 0xc85);
}
