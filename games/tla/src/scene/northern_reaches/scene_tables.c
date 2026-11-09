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
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_02000038(s32 arg0) {
  *(u8 *)((u8 *)*(s32 *)((u8 *)arg0 + 80) + 26) = 0;
  *(u8 *)((u8 *)(arg0 + 34) + 0) = 1;
  return 1;
}

void Func_02000a68();
void Func_02000a70();
void Func_02000af0();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000064(void) {
  Func_0802b450(47, 9, 9, 4, 25, 9);
  Func_0802b450(47, 69, 9, 10, 25, 69);
  MapGrid_CopyCellAttributes(111, 8, 9, 5, 89, 8);
  return;
}

void Func_020000a8(void) {
  s32 result0;
  struct Actor *object1;

  result0 = Event_GetFlag(0x200);
  if (result0 != 0)
    goto L_02008100;
  GameFlag_SetBit(0x200);
  Func_02000a68();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShipIceWallFirstApproach);
  object1 = Actor_GetById(8);
  Script_WaitForEventTimeout((s32)object1);
  Actor_ArmCallbackAndRefresh(8, FACING_NORTH);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Event_ShowMessageThenWait(11, 0, 5);
  Func_02000a70();
L_02008100:
  return;
}

void Func_02000108(void) {
  s32 result0;
  struct Actor *object1;

  result0 = Event_GetFlag(0x90c);
  if (result0 != 0)
    goto L_020081d2;
  GameFlag_SetBit(0x90c);
  Func_02000a68();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShipIceWallCrewReposition);
  object1 = Actor_GetById(8);
  Script_WaitForEventTimeout((s32)object1);
  Event_Wait(5);
  Event_ShowMessageThenWait(5, 0, 5);
  Camera_MoveWithinMapBounds(PIXELS(464), -1, PIXELS(176), 1);
  Func_02000af0();
  Event_Wait(5);
  Actor_SetPosition(11, PIXELS(408), PIXELS(288));
  Actor_SetPosition(5, PIXELS(408), PIXELS(288));
  Actor_SetPosition(6, PIXELS(408), PIXELS(288));
  Event_ShowMessageThenWait(11, 0, 5);
  Event_ShowMessageThenWait(6, 0, 5);
  Event_ShowMessageThenWait(11, 0, 5);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Func_02000a70();
L_020081d2:
  return;
}
