/// @file
/// The scene code of overlay 0x689, in ROM order.

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
#include "lemuria_dock/room.h"
#include "event_inline.h"
#include "version.h"
#include "io_reg.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u8 *Func_02000044(void) {
  if (!GameFlag_Test(0x960))
    return gData_0200057c;
  return gData_0200059c;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_0200006c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->entrance == 2) {
    Func_0200046c();
    Event_ResolvePendingActions(0);
    if (Actor_GetById(8)->x == 0) {
      Actor_WalkByAndWait(4, 0, -16);
      FieldActor_LaunchFromLeader(8, 0, -16, FACING_SOUTHEAST + FACING_STEP);
      Actor_WaitForMoveById(8);
      Actor_SetSpeed(8, 0x13333, 0x9999);
      Actor_WalkToAndWait(8, 472, 392);
      Actor_FaceDirectionAndWait(8, FACING_SOUTH + FACING_STEP);
    } else {
      struct Actor *leader;

      Actor_CommitPositionAndActivate(4, 0, 16);
      Actor_SetSpeed(8, 0x20000, 0x10000);
      Actor_WalkToAndWait(8, 440, 440);
      Actor_SetSpeed(8, 0x13333, 0x9999);
      Actor_SetModeById(8, 2);
      leader = Actor_GetById(party->currentOwner);
      if (leader != NULL)
        Actor_ResetAndSetPosition(8, S16_AT(leader, 10), S16_AT(leader, 18));
      Actor_WaitForMoveById(8);
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    }
    Func_02000474();
  }
}

void Func_02000158(void) {
  struct MainRoutineWork *party;
  s32 id;
  s32 limit;
  s32 z;

  if (SCENE_WORK->unk166 != 0) {
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    Actor_ShowEmote(8, 0x100, 30);
  }
  Event_SetMessageId((s32)MsgAreWeSetting);
  FieldMessage_ShowAtActor(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(15);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(5);
    Actor_Jump(8, 4, 0);
    Actor_ArmCallbackAndRefresh(8, FACING_EAST);
    Event_Wait(5);
    party = &gMainRoutineWork;
    z = Actor_GetById(party->currentOwner)->z;
    limit = 0x1880000;
    id = party->currentOwner;
    if (z < limit)
      Actor_SetPositionAndReset(id, 472, 376);
    else
      Actor_SetPositionAndReset(id, 472, 408);
    id = 4;
    Actor_ArmCallbackAndRefresh(id, FACING_EAST);
    Event_Wait(10);

    gMainRoutineWork.ownerOverride = 1;
    Import_EventSetExitRequest(1);
  } else {
    Event_Wait(20);
    SCENE_WORK->unk1c4++;
    Event_ShowMessageAndWait(8, 0);
  }
}

void Func_02000254(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Func_0200046c();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000474();
}

void Func_020002dc(s32 door) { Func_02000254(&gData_02000610, door, 0); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_020002f8(void) {
  if ((gFrameCount & 7) == 0) {
    volatile u16 *palette;
    volatile u16 *slot;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)&BG_PLTT_COLOR(11, 9);
    index = 9;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(11, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 4)
      goto first;
    slot = (volatile u16 *)&BG_PLTT_COLOR(11, 4);
    *slot = saved;
    slot += 11;
    saved = *slot;
    index = 15;
  second:
    palette = (volatile u16 *)&BG_PLTT_COLOR(11, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 12)
      goto second;
    *(volatile u16 *)&BG_PLTT_COLOR(11, 12) = saved;
  }
}

s32 Func_02000360(void) {
  SCENE_WORK->unk1ac = 256;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Task_Add(Func_020002f8, 0x480);
  Flag_Clear(0x12f);
#if TLA_PATCH_1 /* From patch 1. */
  if (Flag_IsSet(0x90a))
    Actor_SetSpritePriority(9, 1);
  else
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
#endif
  return 0;
}

s32 Func_020003cc(void) {
  if (!Flag_IsSet(0x8df)) {
    Map_CopyCellPictureIndicesRect(57, 52, 64, 15, 1, 1);
    Map_CopyCellPictureIndicesRect(60, 53, 67, 16, 6, 3);
    Map_CopyCellPictureIndicesRect(50, 52, 16, 16, 6, 3);
    Map_CopyCellPictureIndicesRect(50, 61, 16, 28, 10, 3);
    Map_CopyCellPictureIndicesRect(61, 61, 16, 63, 10, 3);
  }
  return 0;
}
