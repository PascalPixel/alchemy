/// @file
/// Overlay 0x65b's scene: its code, then its tables, in the order the ROM has them.

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

void Event_Start(void);
void Event_End(void);
void Func_02000338(void);

void Func_02000054(void) {
  Event_Start();
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
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020000c8(void) {
  if (Event_GetFlag(0x200) == 0) {
    Event_Start();
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
      Event_End();
    }
  }
}

void Func_02000224(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMadraHeardAboutYou);
  Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
  Func_02000338();
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  GameFlag_SetBit(0x916);
  Event_End();
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(53),
    SCENE_EXIT_IF(1, 2, 10, -1),
    SCENE_EXIT_IF(2, 2, 11, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {73, CONDITION_ALWAYS, 1, PIXELS(188), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {88, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, 0x913, EVENT(Func_020000c8)},
    {TRIGGER_STEP, 10, 0x916, EVENT(Func_02000224)},
    {TRIGGER_TALK, 8, 0x913, EVENT(Func_02000054)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMadraMikasallaVillage)},
    {TRIGGER_TALK, 9, 0x913, MSGNO(MsgMadraCanCrossWithPermission)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMadraAvoidYampiDesert)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x913, MSGNO(MsgMadraForTheirSafety)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgMadraWouldNeedBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x913, MSGNO(MsgMadraElderReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMadraRatherStayHome)},
    {TRIGGERS_END},
};
