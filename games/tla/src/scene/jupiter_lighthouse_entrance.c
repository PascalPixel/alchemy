/// @file
/// The scene code of overlay 0x698, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_02001b1c();

void Func_02000054(void) { Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x46, 0x13, 1); }

void Func_02000074(void) { Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x45, 0x16, 1); }

void Func_02000094(void) { Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1); }

void Func_020000b4(void) { Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x55, 0x1a, 1); }

void Func_020000d4(void) { Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x5e, 0x1a, 1); }

#include "main_routine_work.h"

void Func_020000f4(void) {
  s32 t0;
  s32 t2;

  t0 = Actor_Get(gMainRoutineWork.currentOwner);
  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  t2 = Event_GetFlag(0xfe2);
  if (t2 == 0) {
    Actor_SetPosition(0x40, 0x1180000, 0x1b80000);
    if ((FIELD_AT(t0, s32, 8) >> 20) == 0x11) {
      if ((FIELD_AT(t0, s32, 16) >> 20) == 0x1b) {
        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
        Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, -16, 0);
        FIELD_AT(t0, s32, 40) = 0x60000;
        FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
        Event_End();
      }
    }
  }
}

#include "ram_buffer.h"
#include "scene_ids.h"
#include "debug.h"

void Func_02000184(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }


s32 Scene_Initialize(void) {
  s32 clear;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  clear = Event_GetFlag(0x109);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Func_02001b3c(gMainRoutineWork.currentOwner);
  }
  gMainRoutineWork.savedScene = (s32)&SceneId_Ov694Room2;
  gMainRoutineWork.savedEntrance = 6;
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

extern s32 gData_02002198;

void Func_0200022c(s32 a0) { gData_02002198 = a0; }

#include "shared/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame, gData_02002198,
    gData_020021c0, 0x05000184)

#include "field_event.h"

void Func_02001a6c();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes, gData_02002198, gData_020021c0,
    gData_020021e0, PaletteCycle_StepAt, gLighthouseUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001a6c, Func_02001b34)

#include "actor.h"
#include "frame.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)
