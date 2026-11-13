/// @file
/// Lemuria's dock, where the Lemurian ship ties up: Piers steps off to wait by
/// the ship and asks whether the party is ready to sail, and a house stands on
/// the quay.

#include "battle/battle.h"
#include "flags.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "version.h"
#include "system/io_reg.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

struct SceneWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x44];
  s32 unk1ac;
  u8 unk1b0[0x14];
  u16 unk1c4;
};

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->eventWork)

extern u8 MsgAreWeSetting[];
extern s32 gLemuriaDock_Exits[];
extern s32 gLemuriaDock_ExitsWithFlag960[];
extern struct WarpTarget gLemuriaDock_HouseDoor;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void LemuriaDock_EnterDoor(const struct WarpTarget *warp, s32 door, s32 mode);
void LemuriaDock_CyclePalette(void);
void Event_End(void);

extern s16 gLemuriaDock_HouseDoorOpening[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

s32 *Scene_GetExits(void) {
  if (!Event_GetFlag(0x960))
    return gLemuriaDock_Exits;
  return gLemuriaDock_ExitsWithFlag960;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// Arriving on the dock (entrance 2): where Piers is not already on the dock he
/// steps out of the party and walks to his place by the ship; where he is, he
/// walks back into the leader.
void LemuriaDock_MovePiers(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->entrance == 2) {
    Event_Start();
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

      Actor_WalkByAndWait(4, 0, 16);
      Actor_SetSpeed(8, 0x20000, 0x10000);
      Actor_WalkToAndWait(8, 440, 440);
      Actor_SetSpeed(8, 0x13333, 0x9999);
      Actor_SetAnimation(8, 2);
      leader = Actor_GetById(party->currentOwner);
      if (leader != NULL)
        Actor_SetDestination(8, S16_AT(leader, 10), S16_AT(leader, 18));
      Actor_WaitForMoveById(8);
      Actor_SetPosition(8, 0, 0);
    }
    Event_End();
  }
}

/// Piers asks whether the party is ready to sail; yes takes it aboard the ship
/// (exit 1).
/// "Felix, are we setting off now?"
void LemuriaDock_AskSettingOff(void) {
  struct MainRoutineWork *party;
  s32 id;
  s32 limit;
  s32 z;

  if (SCENE_WORK->unk166 != 0) {
    Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
    Actor_ShowEmote(8, 0x100, 30);
  }
  Event_SetMessageId((s32)MsgAreWeSetting);
  Event_OpenMessage(8, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(15);
    Event_ShowMessage(8, 0);
    Event_Wait(5);
    Actor_Jump(8, 4, 0);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Event_Wait(5);
    party = &gMainRoutineWork;
    z = Actor_GetById(party->currentOwner)->z;
    limit = 0x1880000;
    id = party->currentOwner;
    if (z < limit)
      Actor_WalkToAndWait(id, 472, 376);
    else
      Actor_WalkToAndWait(id, 472, 408);
    id = 4;
    Actor_FaceDirectionAndWait(id, FACING_EAST);
    Event_Wait(10);

    gMainRoutineWork.ownerOverride = 1;
    Import_EventSetExitRequest(1);
  } else {
    Event_Wait(20);
    SCENE_WORK->unk1c4++;
    Event_ShowMessage(8, 0);
  }
}

/// The party leader walks into the door `warp` names, which opens with its cell
/// animation, and leaves by exit `door`; mode 0 lines the leader up with the
/// door first.
void LemuriaDock_EnterDoor(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(warp->scene, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void LemuriaDock_EnterHouse(s32 door) {
  LemuriaDock_EnterDoor(&gLemuriaDock_HouseDoor, door, 0);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// A palette task: every 8 frames it turns colours 4 to 9 and 12 to 15 of
/// background palette 11 round by one.
void LemuriaDock_CyclePalette(void) {
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

s32 Scene_Initialize(void) {
  SCENE_WORK->unk1ac = 256;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Task_Add(LemuriaDock_CyclePalette, 0x480);
  Flag_Clear(FLAG_SHOW_AREA_NAME);
#if TLA_PATCH_1 /* From patch 1. */
  if (Flag_IsSet(0x90a))
    Actor_SetSpritePriority(9, 1);
  else
    Actor_SetPosition(9, 0, 0);
#endif
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (!Flag_IsSet(FLAG_SHIP_FLIES)) {
    MapGrid_CopyCellsTo(57, 52, 64, 15, 1, 1);
    MapGrid_CopyCellsTo(60, 53, 67, 16, 6, 3);
    MapGrid_CopyCellsTo(50, 52, 16, 16, 6, 3);
    MapGrid_CopyCellsTo(50, 61, 16, 28, 10, 3);
    MapGrid_CopyCellsTo(61, 61, 16, 63, 10, 3);
  }
  return 0;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
/// The dock's exits: which of Ancient Lemuria's entrances the way north leads
/// to follows flag 0x960, which Ancient Lemuria sets by the way the party last
/// came.
// XXX: what flag 0x960 records; Ancient Lemuria's entrances 1, 4 and 5
// against 2 and 3 would settle it, read from that scene's map.
s32 gLemuriaDock_Exits[] = {
    200, 0x101030c7, -1, 0x102020c9, -1, 0x10301028,
    -1, 511,
};

s32 gLemuriaDock_ExitsWithFlag960[] = {
    200, 0x101050c7, -1, 0x102050c9, -1, 0x10301028,
    -1, 511,
};

struct ActorPlacement gActorPlacements[] = {
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
#if TLA_PATCH_1
    {420, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(392), FACING_EAST, 2, 0},
#endif
    {SCENE_TABLE_END},
};

s16 gLemuriaDock_HouseDoorOpening[] = {
    51, 38, 1, 1, 6, -1,
};
// clang-format on

struct WarpTarget gLemuriaDock_HouseDoor = {(s32)gLemuriaDock_HouseDoorOpening,
                                            70, 22};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(LemuriaDock_EnterHouse)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(LemuriaDock_MovePiers)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(LemuriaDock_AskSettingOff)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1032, CONDITION_ALWAYS,
     EVENT(LemuriaDock_AskSettingOff)},
    {TRIGGER_TABLE_END},
};
