/// @file
/// The scene code of overlay 0x69a, in ROM order.

#include "shared/whirlwind.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
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
#include "event_inline.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "debug.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_room_1.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) { Import_EventSetExitRequest(6); }

void Func_02000060(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

void Func_020000ec(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

DEFINE_JUPITER_LIGHTHOUSE_CHECK_BLOCK_ON_CELL(JupiterLighthouse_CheckBlockOnCell, 10, 0xa01, 17, 33,
    Func_02000060)

DEFINE_SCENE_RUN_ONCE(Scene_RunOnce, 0x200, Func_02000060)

DEFINE_SCENE_RUN_IF_FLAG_CLEARED(Scene_RunIfFlagCleared, 0x200, Func_020000ec, JupiterLighthouse_CheckBlockOnCell)

void Func_020001f4(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (!Event_GetFlag(0x109) && gMainRoutineWork.entrance == 6)
    Func_02001bcc(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0xa01)) {
    Actor_SetPosition(10, PIXELS(280), PIXELS(536));
    Func_02000060(0);
  }
  if (Event_GetFlag(0x200))
    Func_02000060(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002230;

void Func_020002bc(s32 value) { gData_02002230 = value; }

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame, gData_02002230,
    gData_02002258, 0x05000184)

void Func_02001b14();
void Func_02001bc4();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes, gData_02002230, gData_02002258,
    gData_02002278, PaletteCycle_StepAt, gData_0200227c)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001b14, Func_02001bc4)

#include "actor.h"
#include "frame.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)
