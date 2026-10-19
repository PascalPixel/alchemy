/// @file
/// Colosso Log and Colosso River: setting up a race against the rival, with
/// the race gauge at the top of the screen and the rival running a recorded
/// path (colosso_path_rival.h).
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

/// Start the race gauge (struct ColossoGaugeWork, in work slot 59): Isaac and
/// the rival as playerActor and rivalActor, a bar of segments pieces either
/// side, and the finish at pixel (finishX, finishZ) in 16.16, where the two
/// item actors stand. Unless the party returns to where it was, the rival is
/// put opposite Isaac across x = finishX.
void Colosso_StartRaceGauge(s32 playerActor, s32 rivalActor, s32 segments,
                            s32 finishX, s32 finishZ, s32 finishItemActorA,
                            s32 finishItemActorB) {

  u8 *desc;
  struct FieldActor *player;
  struct FieldActor *rival;
  s32 handle;
  s32 pal;

  desc = Runtime_AllocateBlock(59, 0x7170);
  handle = (s32)Runtime_BumpAllocateAlternatePool(512);

  *(u16 *)(desc + 222) = playerActor;
  *(u16 *)(desc + 224) = rivalActor;
  *(u16 *)(desc + 226) = finishItemActorA;
  *(u16 *)(desc + 228) = finishItemActorB;
  *(u16 *)(desc + 230) = segments;
  *(s32 *)(desc + 232) = finishX;
  *(s32 *)(desc + 236) = finishZ;

  player = Object_GetById(playerActor);
  rival = Object_GetById(rivalActor);

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    rival->x.fixed = (finishX << 1) - player->x.fixed;
    rival->z.fixed = player->z.fixed;
  }

  // level and hold
  *(u16 *)(desc + 218) = 0;
  *(u16 *)(desc + 220) = 0;

  FieldResource_DecodeType01(gColosso_GaugeGraphics, (void *)handle);

  // vramBlock
  pal = Resource_FindFreeEntry();
  *(u16 *)(desc + 216) = pal;
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

/// Decode the rival's recorded path (resource) after the gauge's work and,
/// unless the party returns to where it was, have the rival run it from the
/// start, mirrored across the finish; then start the rival's callback.
void Colosso_StartRival(s32 resource) {
  u8 *work = gColossoWork;
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

  FieldResource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = 1;
    recorder->mirror = 1;
    recorder->actor = *(u16 *)(work + 224);
    recorder->still = 0;
    recorder->pos = 0;
  }
  FieldScheduler_AddCallback(Colosso_UpdatePathRival, 0xc85);
}
