/// @file
/// The scene code of overlay 0x65b that its scene tables belong to, in ROM order; the rest of the overlay is in madra_drawbridge/.

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
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "actor.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_02000054(void) {
  Func_020002b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraGoingToOsenia);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(8, 3);
    Event_ShowMessageThenWait(8, 0, 2);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageThenWait(8, 0, 2);
  }
  Func_020002c0();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_020000c8(void) {
  if (Event_GetFlag(0x200) == 0) {
    Func_020002b8();
    Event_ResolvePendingActions(0);
    if (Event_GetFlag(0x301) == 0) {
      Camera_SetSpeedParameters(0x18000, 0x3000);
      Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
      Actor_WalkTo(gMainRoutineWork.currentOwner, 158, 316);
    }
    Actor_GetById(8)->actionFlags &= ~1;
    Actor_GetById(9)->actionFlags &= ~1;
    Actor_ResetAndSetPositionInMode2(9, 148, 252);
    Actor_SetPositionAndReset(8, 170, 252);
    Actor_WaitForMoveById(9);
    Actor_GetById(8)->actionFlags |= 1;
    Actor_GetById(9)->actionFlags |= 1;
    Actor_FaceDirection(8, FACING_SOUTH, 0);
    Actor_ArmCallback(9, FACING_SOUTH, 0);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    GameFlag_SetBit(0x200);
    Func_02000338();
    MapGrid_CopyCellAttributes(11, 9, 4, 1, 8, 14);
    if (Event_GetFlag(0x301) == 0) {
      Event_SetMessageId((s32)MsgMadraNeedEldersPermission);
      Event_Wait(10);
      Motion_SetVarCbAndRefresh(8, 1);
      Event_ShowMessageThenWait(8, 0, 5);
      Motion_SetModeAndWaitAnimation(9, 3);
      Event_ShowMessageAndWait(9, 0);
      GameFlag_SetBit(0x301);
      Func_020002c0();
    }
  }
}

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_02000224(void) {
  Func_020002b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraHeardAboutYou);
  Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
  Func_02000338();
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  GameFlag_SetBit(0x916);
  Func_020002c0();
}

s32 Func_02000280(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
