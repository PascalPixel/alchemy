/// @file
/// The scene code of overlay 0x69c, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/effects.h"
#include "shared/jupiter_lighthouse_room_3.h"

extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern const u16 gPaletteCycleColors[];
extern const u16 gPaletteCycleUnlitPalette[];
extern const u16 gPaletteCycleUnlitColors[];
#include "event_work.h"
#include "field.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(struct Actor *object) {
  if (object->x >> 20 == 14 && object->z >> 20 == 12)
    Actor_SetSpritePriorityDirect(object, 2);
}

void Func_02000070(void) {
  struct Actor *object = Actor_Get(10);
  struct PushedBlock block;
  s32 i;
  s32 x;

  object->update = (void (*)(void))Func_02000054;
  if (PushTarget_FindSlide(&block))
    PushTarget_PushFill(block);
  if (object->x >> 20 == 16 && object->z >> 20 == 12 &&
      Event_GetFlag(0x200) == 0) {
    Event_SetFlag(0x200);
    FieldActor_SetSpritePriority(10, 3);
    object->motionFlags = 3;
    System_Wait(5);
    Event_PlaySound(134);
    object->motionFlags = 2;
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460(0x1000000, x, 0, 128);
    }
  }
  object->update = 0;
}

void Data_02000130(void) {}

/// Sets flag 0xa04 or 0xa05 when the object stands on cell (20, 36) or
/// (32, 36), then waits up to 30 frames for it to land and lifts it there.
void Func_02000134(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 placed = 0;
  s32 i;
  if (object->x >> 20 == 20 && object->z >> 20 == 36) {
    Event_SetFlag(0xa04);
    placed = 1;
  }
  if (object->x >> 20 == 32 && object->z >> 20 == 36) {
    Event_SetFlag(0xa05);
    placed = 1;
  }
  if (placed) {
    System_Wait(10);
    Event_PlaySound(134);
    for (i = 0; i < 30; i++) {
      if (object->y <= object->terrainHeight && object->velocityY == 0)
        break;
      System_Wait(1);
    }
    object->motionFlags = 0;
    Func_02002368(object->x, object->z, 0, 0);
  }
}

void Func_020001c0(void) { Import_EventSetExitRequest(12); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02002440(12, 1);
  Func_02002440(13, 1);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 12)
    Func_02002468(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 13)
    Func_02002468(gMainRoutineWork.currentOwner);
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 i;
  s32 x;

  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0xa04)) {
    object = Actor_Get(14);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(14, PIXELS(328), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  if (Event_GetFlag(0xa05)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(15, PIXELS(520), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  Actor_Get(8)->priorityFlags |= 32;
  Actor_Get(9)->priorityFlags |= 32;
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  FieldActor_SetSpritePriority(8, 3);
  FieldActor_SetSpritePriority(10, 3);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(11, 3);
  Actor_Get(10)->priorityFlags |= 2;
  Func_020023d0(8);
  Func_020023d0(9);
  Func_020023d0(10);
  Func_020023d0(11);
  Func_02000c78(8);
  Func_02000c78(9);
  Func_02000c78(10);
  Func_02000c78(11);
  if (Event_GetFlag(0x200)) {
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460(0x1000000, x, 0, 128);
    }
  }
  return 0;
}

void Func_020003c4(void) { Field_TryJumpForward(); }

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "event_inline.h"

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES_ZERO(
    JupiterLighthouse_LoadPalettesZero, gPaletteCyclePausesInEvents,
    gPaletteCycleColors, gPaletteCycleUnlitPalette, gPaletteCycleFrame,
    PaletteCycle_StepAt, gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02002368, Func_02002460)

#include "actor.h"
#include "main_routine_work.h"
#include "frame.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

s32 Position_FlatDistance(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;
  s32 dxsq = dx * dx;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dzsq);
}
