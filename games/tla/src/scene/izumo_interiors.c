/// @file
/// The scene code of overlay 0x67d, in ROM order.

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

s32 Func_02000038(void) {
  struct Actor *object = Actor_GetById(10);

  if ((u32)(object->x - 0x940000) <= 0x80000) {
    s32 z = object->z;

    if (z >= 0x26c0000) {
      if (z <= 0x2740000)
        return 0;
    }
  }
  return 1;
}

s32 Func_0200006c(void) {
  struct Actor *object = Actor_GetById(9);

  if ((u32)(object->x - 0x840000) <= 0x80000) {
    s32 z = object->z;

    if (z >= 0x26c0000) {
      if (z <= 0x2740000)
        return 0;
    }
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02002b40[];
extern const u8 gData_02002cf0[];
extern const u8 gData_02002d80[];
extern const u8 gData_02002f78[];
extern const u8 gData_02003164[];
extern const u8 gData_0200323c[];

void Func_020021c4(void);
void Func_020021cc(void);
void Func_02002304(s32 slot, s32 actor);
void Func_0200230c(s32 actor);
void Func_02002314(s32 slot, s32 actor);
void Func_0200061c(s32 mode);

static inline void LeaderAnimate(s32 animation, s32 frames) {
  Func_080d47b4(gMainRoutineWork.currentOwner, animation, frames);
}

const u8 *Func_020000b4(void) {
  if (GameFlag_Test(0x927))
    return gData_02002d80;
  if (gMainRoutineWork.entrance == 7)
    return gData_02002cf0;
  return gData_02002b40;
}

const u8 *Func_020000f0(void) {
  if (GameFlag_Test(0x927))
    return gData_0200323c;
  if (gMainRoutineWork.entrance == 7)
    return gData_02003164;
  return gData_02002f78;
}

void Func_0200012c(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_0200230c(actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927)) {
      Event_SetMessageId((s32)MsgNowThatPeace);
      Event_ShowMessageAndWait(12, 0);
    } else {
      Event_SetMessageId((s32)MsgWhyWouldOur);
      Event_ShowMessageAndWait(22, 0);
    }
    Func_020021cc();
  }
}

void Func_020001a8(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02002314(9, actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927))
      Event_SetMessageId((s32)MsgBriggsFromChampa);
    else
      Event_SetMessageId((s32)MsgOfCourseI);
    Event_ShowMessageAndWait(actor, 0);
    Func_020021cc();
  }
}

void Func_02000220(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02002304(24, actor);
  } else {
    Func_020021c4();
    Event_ResolvePendingActions(0);
    if (GameFlag_Test(0x927))
      Event_SetMessageId((s32)MsgYouMustSee);
    else
      Event_SetMessageId((s32)MsgIzumoSLeaders);
    Event_ShowMessageAndWait(actor, 0);
    Func_020021cc();
  }
}

void Func_02000298(void) {
  Func_020021c4();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x922)) {
    Event_SetMessageId((s32)MsgSusaSNot);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Func_080d47b4(8, 265, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Func_080d47b4(8, 261, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(8, 1);
    Event_Wait(60);
    Event_ShowMessageAndWait(8, 0);
    Event_Wait(30);
    GameFlag_SetBit(0x922);
  }
  Actor_SetModeById(8, 4);
  Event_Wait(60);
  Event_SetMessageId((s32)MsgIfTheSerpent);
  Event_ShowMessageAndWait(8, 0);
  Func_020021cc();
}

void Func_02000358(void) {
  GameFlag_SetBit(0x923);
  Event_SetMessageId((s32)MsgIPrayThat);
  Event_ShowMessageAndWait(8, 0);
}

void Func_02000378(void) {
  void *work = Ram_HeapSlots->eventWork;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  if (GameFlag_Test(0x92c)) {
    Event_SetMessageId((s32)MsgWithThisDancing);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgIfYouHave);
    Event_ShowMessageAndWait(15, 0);
    if (PartyInventory_FindOwner(451) != -1)
      FIELD_AT(work, u16, 346) = 1;
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Func_020021cc();
}

void Func_02000400(void) {
  s32 msg;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgTheyVeBeen;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(16, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(16, 0);
  Func_020021cc();
}

void Func_02000458(void) {
  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  if (Event_GetFlag(0x92b)) {
    Event_SetMessageId((s32)MsgTryingAgainWhat);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgAndWhatWere);
    Event_ShowMessageAndWait(15, 0);
    if (GameFlag_Test(0x923)) {
      LeaderAnimate(256, 30);
      Func_080d47b4(15, 259, 30);
      Event_ShowMessageAndWait(15, 0);
      LeaderAnimate(258, 30);
      Actor_SetModeById(15, 4);
      Event_Wait(30);
      Event_ShowMessageAndWait(15, 0);
    }
    GameFlag_SetBit(0x92b);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Func_020021cc();
}

s32 Func_0200051c(void) {
  Func_020021c4();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x925)) {
    Event_SetMessageId((s32)MsgSusaDonT);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_Wait(15);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(17, 1);
    Event_Wait(30);
    Event_ShowMessageAndWait(17, 0);
    Event_Wait(60);
    Event_SetFlag(0x925);
  } else {
    Event_SetMessageId((s32)MsgSusaPleaseDon);
    Event_ShowMessageAndWait(17, 0);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_OffsetPositionAndReset(gMainRoutineWork.currentOwner, 0, 16);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_020021cc();
  return 0;
}

void Func_02000604(void) { Func_0200061c(0); }

void Func_02000610(void) { Func_0200061c(1); }

void Func_02001d8c(void);

/// The scene that opens with MsgWarriorsAppearedFrom: the warriors confront
/// the party in front of persons 20, 21 and 22 through three yes-or-no
/// answers, then persons 5, 6, 7 and 23 join the leader and flags 0x928 and
/// 0x350 are set. Mode 0 and mode 1 (Func_02000604 and Func_02000610) start
/// the party at different spots.
void Func_0200061c(s32 mode) {
  struct Actor *person, *leader;
  s32 party, owner, answer, message;

  Func_020021c4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(PIXELS(143), -1, PIXELS(560), 1);
  party = (s32)&gMainRoutineWork;
  if (mode == 0) {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 184, 568);
  } else {
    Actor_WalkToAndWait(*(s32 *)(party + 532), 104, 568);
  }
  FieldEvent_Wait(15);
  FieldEvent_SetMessageId((s32)MsgWarriorsAppearedFrom);
  Event_ShowMessage(0x800f, 0);
  Actor_SetAnimation(22, 3);
  Event_ShowMessage(0x8016, 0);
  Actor_ShowEmote(15, 0x100, 30);
  Event_ShowMessage(0x800f, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(0x8016, 0);
  Actor_ShowEmote(15, 0x103, 30);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(22, 0x102);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x8016, 0);
  Actor_FaceDirection(20, FACING_EAST, 30);
  Actor_SetAnimation(20, 4);
  Event_ShowMessage(0x8014, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_WEST, 30);
  Actor_SetAnimation(15, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  Actor_SetAnimation(22, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(0x8016, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(15);
  Event_ShowMessage(0x8014, 0);
  Actor_FaceActor(15, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(30);
  Actor_SetDestinationOffset(15, 0, 16);
  FieldActor_WaitForMoveById(15);
  FieldEvent_Wait(15);
  Actor_ShowEmote(15, 0x100, 30);
  if (mode == 0) {
    Actor_WalkToAndWait(15, 168, 536);
  } else {
    Actor_WalkToAndWait(15, 120, 536);
  }
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_FaceActor(20, *(s32 *)owner, 0);
  Event_ShowMessage(0x800f, 0);
  Actor_ShowEmote(15, 0x107, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  FieldActor_Jump(22, 4, 0);
  Actor_SetAttachedEffect(22, 0x101);
  FieldEvent_Wait(30);
  Actor_FaceDirection(20, FACING_SOUTH, 30);
  Actor_ShowEmote(20, 0x101, 30);
  Event_ShowMessage(0x8014, 0);
  Actor_FaceActor(22, 20, 0);
  Actor_SetAttachedEffect(22, 0x100);
  Event_ShowMessage(22, 0);
  FieldEvent_Wait(30);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  FieldActor_StartRepeatedMotion(20, 2);
  FieldEvent_Wait(30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  Event_ShowMessage(0x8014, 0);
  Actor_ShowEmote(15, 0x108, 30);
  Event_ShowMessage(0x800f, 0);
  Actor_WalkTo(15, 144, 520);
  FieldActor_WaitForMoveById(15);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_FaceDirection(20, FACING_SOUTH, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  if (mode == 0) {
    Actor_WalkTo(22, 104, 568);
  } else {
    Actor_WalkTo(22, 184, 568);
  }
  FieldActor_WaitForMoveById(22);
  if (mode == 0) {
    Actor_FaceDirection(22, FACING_EAST, 0);
  } else {
    Actor_FaceDirection(22, FACING_WEST, 0);
  }
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_WalkTo(*(s32 *)owner, 144, 560);
  FieldActor_WaitForMoveById(*(s32 *)owner);
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(5, 8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, 24, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(7, -8, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(23, -24, 24, FACING_NORTH);
  FieldEvent_Wait(60);
  Event_ShowMessage(0x8014, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, 7, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(5, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Actor_SetAnimation(6, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Actor_FaceActor(5, 23, 0);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x8014, 0);
  Actor_SetAnimation(6, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(15, 0x101, 30);
  Event_ShowMessage(0x800f, 0);
  Actor_FaceEachOther(15, 20, 0);
  FieldEvent_Wait(60);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(20, 4);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x8014, 0);
  Actor_FaceActor(*(s32 *)owner, 23, 0);
  Actor_FaceActor(5, 23, 0);
  Actor_FaceActor(6, 23, 0);
  Actor_FaceActor(7, 23, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(23, 15, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(23, 0);
  Actor_ShowEmote(15, 0x100, 0);
  Actor_ShowEmote(20, 0x100, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  Actor_FaceActor(*(s32 *)owner, 15, 0);
  Actor_FaceActor(5, 15, 0);
  Actor_FaceActor(6, 15, 0);
  Actor_FaceActor(7, 15, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, 0x102, 30);
  Event_ShowMessage(0x800f, 0);
  FieldEvent_Wait(60);
  Actor_FaceDirection(15, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Func_02001d8c();
  FieldEvent_Wait(30);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  Actor_ShowEmote(*(s32 *)owner, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Actor_ShowEmote(23, 0x100, 0);
  FieldEvent_Wait(30);
  FieldActor_StartRepeatedMotion(5, 1);
  FieldEvent_Wait(15);
  Event_ShowMessage(5, 0);
  Actor_FaceActor(15, 20, 0);
  FieldEvent_Wait(30);
  Actor_FaceActor(15, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x800f, 0);
  Actor_ShowEmote(20, 0x101, 30);
  FieldEvent_Wait(30);
  Event_OpenMessage(0x8014, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 1) {
    FieldEvent_SetMessageId((s32)MsgIThoughtSo);
    Actor_ShowEmote(20, 0x100, 30);
    Event_ShowMessage(0x8014, 0);
    Actor_SetAnimation(6, 3);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceActor(7, *(s32 *)owner, 0);
    FieldEvent_Wait(15);
    Actor_SetAnimation(7, 4);
    FieldEvent_Wait(30);
    Event_ShowMessage(7, 0);
  }
  FieldEvent_SetMessageId((s32)MsgSoYouCall);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x8014, 0);
  Actor_SetAnimation(*(s32 *)owner, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(23, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(20, 3);
  FieldEvent_Wait(60);
  Actor_ShowEmote(20, 0x101, 30);
  Actor_FaceActor(20, *(s32 *)owner, 0);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x8014, 0);
  FieldEvent_Wait(90);
  Event_ShowMessage(21, 0);
  Actor_SetPosition(21, PIXELS(144), PIXELS(680));
  Actor_FaceActor(*(s32 *)owner, 21, 0);
  Actor_FaceActor(5, 21, 0);
  Actor_FaceActor(6, 21, 0);
  Actor_FaceActor(7, 21, 0);
  Actor_FaceActor(23, 21, 0);
  Actor_FaceActor(22, 21, 0);
  FieldEvent_Wait(120);
  Event_ShowMessage(0x800f, 0);
  FieldEvent_Wait(30);
  FieldCamera_FollowActor(21, 1);
  FieldEvent_Wait(120);
  Actor_SetSpeed(21, 0x9999, 0x4ccc);
  Actor_SetSpeedParameters(21, 0x9999, 0x4ccc);
  Actor_GetById(21)->priorityFlags &= ~1;
  Actor_WalkTo(21, 144, 648);
  if (mode == 0) {
    Actor_WalkToAndWait(21, 88, 648);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 616);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_WalkToAndWait(21, 56, 584);
    Actor_GetById(22)->actionFlags &= ~1;
    Actor_WalkToAndWait(22, 88, 568);
    Event_Wait(1);
    Actor_GetById(22)->actionFlags |= 1;
    Actor_ArmCallback(22, FACING_EAST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 104, 568);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_SetSpritePriority(23, 1);
    Actor_SetSpritePriority(21, 2);
    Actor_SetPositionAndReset(21, 120, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(23, 3);
    Actor_WalkToAndWait(21, 120, 536);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_SetModeById(22, 3);
    Actor_WalkToAndWait(22, 56, 584);
    Actor_WalkToAndWait(22, 88, 648);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  } else {
    Actor_WalkToAndWait(21, 200, 648);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 616);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_WalkToAndWait(21, 232, 584);
    Actor_GetById(22)->actionFlags &= ~1;
    Actor_WalkToAndWait(22, 200, 568);
    Event_Wait(1);
    Actor_GetById(22)->actionFlags |= 1;
    Actor_FaceDirection(22, FACING_WEST, 0);
    Actor_SetSpritePriority(21, 1);
    Actor_WalkToAndWait(21, 184, 568);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_SetSpritePriority(21, 2);
    Actor_SetSpritePriority(6, 1);
    Actor_SetPositionAndReset(21, 168, 568);
    Actor_SetSpritePriority(21, 3);
    Actor_SetSpritePriority(6, 3);
    Actor_WalkToAndWait(21, 168, 536);
    Actor_SetAngleToward(*(s32 *)owner, 21, 0);
    Actor_SetAngleToward(5, 21, 0);
    Actor_SetAngleToward(6, 21, 0);
    Actor_SetAngleToward(7, 21, 0);
    Actor_SetAngleToward(23, 21, 0);
    Actor_SetAngleToward(15, 21, 0);
    Actor_SetAngleToward(20, 21, 0);
    Actor_SetAngleToward(22, 21, 0);
    Actor_SetModeById(22, 3);
    Actor_WalkToAndWait(22, 232, 584);
    Actor_WalkToAndWait(22, 200, 648);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  }
  person = Actor_GetById(21);
  {
    u8 *priority = &person->priorityFlags;
    s32 flags = *priority | 1;

    owner = (s32)&gMainRoutineWork;
    *priority = flags;
  }
  owner = owner + 532;
  Actor_FaceDirection(*(s32 *)owner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_ArmCallback(23, FACING_NORTH, 0);
  Event_Wait(15);
  Event_ShowMessageAndWait(20, 0);
  Event_Wait(15);
  Actor_ShowEmote(20, 0x108, 30);
  Event_ShowMessageAndWait(20, 0);
  Actor_SetAngleToward(21, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetAngleToward(21, *(s32 *)owner, 0);
  Event_Wait(60);
  Actor_SetAngleToward(21, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_SetAngleToward(15, *(s32 *)owner, 0);
  Actor_SetAngleToward(20, *(s32 *)owner, 0);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, 0x101, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(21, *(s32 *)owner, 0);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetAngleToward(15, *(s32 *)owner, 0);
  Actor_SetAngleToward(20, *(s32 *)owner, 0);
  Event_Wait(30);
  Func_080d47b4(*(s32 *)owner, 0x102, 30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, 0x101, 0);
  Actor_ShowEmote(20, 0x101, 0);
  Actor_ShowEmote(21, 0x101, 0);
  Actor_ShowEmote(22, 0x101, 0);
  Event_Wait(30);
  Actor_ShowEmote(23, 0x100, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(21, 0x101, 30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetAngleToward(23, 21, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Func_080d47b4(20, 0x106, 30);
  Event_ShowMessageAndWait(20, 0);
  Actor_ShowEmote(23, 0x108, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(15, 0x101, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(7, 23, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetAngleToward(23, 7, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Func_080d47b4(15, 0x101, 30);
  Actor_SetAngleToward(23, 15, 0);
  Actor_SetAngleToward(7, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(6, 0x108, 30);
  Event_ShowMessageAndWait(6, 0);
  Func_080d47b4(21, 0x100, 30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetModeById(5, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(90);
  Actor_SetModeById(15, 3);
  Event_OpenMessage(15, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId((s32)MsgRegardlessOfYour);
    Func_080d47b4(15, 0x108, 30);
    Event_ShowMessageAndWait(15, 0);
  } else {
    Event_SetMessageId((s32)MsgYouTookPity);
    Event_ShowMessageAndWait(15, 0);
  }
  message = (s32)MsgYouReNotImplying;
  Event_SetMessageId(message);
  Actor_StartRepeatedMotion(23, 1);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAngleToward(15, 23, 0);
  Func_080d47b4(15, 0x101, 30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAngleToward(23, 21, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(21, 0);
  Actor_SetModeById(7, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_ArmCallback(20, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmote(5, 0x108, 30);
  Event_ShowMessageAndWait(5, 0);
  Func_080d47b4(15, 0x100, 30);
  Event_Wait(15);
  Actor_SetAngleToward(15, 21, 0);
  Event_ShowMessageAndWait(15, 0);
  Func_080d47b4(21, 0x108, 30);
  Event_ShowMessageAndWait(21, 0);
  Func_080d47b4(6, 0x106, 30);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_SetAngleToward(6, *(s32 *)owner, 0);
  Event_OpenMessage(6, 0);
  Actor_SetAngleToward(5, *(s32 *)owner, 0);
  Actor_SetAngleToward(6, *(s32 *)owner, 0);
  Actor_SetAngleToward(7, *(s32 *)owner, 0);
  Actor_SetAngleToward(23, *(s32 *)owner, 0);
  Actor_SetAngleToward(15, *(s32 *)owner, 0);
  Actor_SetAngleToward(20, *(s32 *)owner, 0);
  Actor_SetAngleToward(21, *(s32 *)owner, 0);
  Event_Wait(30);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 10);
    Actor_SetModeById(21, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0);
  } else {
    Event_SetMessageId(message + 11);
    Actor_SetModeById(21, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(21, 0);
  }
  message = (s32)MsgIDisagree2;
  Event_SetMessageId(message);
  Actor_SetAngleToward(23, 21, 0);
  Func_080d47b4(23, 0x101, 30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  owner = (s32)&gMainRoutineWork;
  owner = owner + 532;
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Event_Wait(30);
  Actor_SetAngleToward(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAngleToward(15, *(s32 *)owner, 0);
  Actor_SetAngleToward(23, *(s32 *)owner, 0);
  Event_Wait(30);
  Event_OpenMessage(23, 0);
  answer = Event_AskYesNoWithActorAnimation(*(s32 *)owner, 0);
  if (answer == 0) {
    Event_SetMessageId(message + 4);
    Actor_SetModeById(5, 3);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(7, 3);
    Event_Wait(30);
    Event_ShowMessageAndWait(5, 0);
    Event_ShowMessageAndWait(6, 0);
    Event_ShowMessageAndWait(7, 0);
  } else {
    Event_SetMessageId(message + 7);
    Actor_SetModeById(5, 4);
    Actor_SetModeById(6, 4);
    Actor_SetModeById(7, 4);
    Event_Wait(30);
    Event_ShowMessageAndWait(5, 0);
    Event_ShowMessageAndWait(6, 0);
    Event_ShowMessageAndWait(7, 0);
  }
  Event_SetMessageId((s32)MsgAsYouCanSee);
  Actor_SetAngleToward(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_TurnToFaceEachOther(15, 20, 0);
  Event_Wait(30);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  party = (s32)&gMainRoutineWork;
  owner = party + 532;
  Actor_SetAngleToward(15, *(s32 *)owner, 0);
  Actor_SetAngleToward(20, *(s32 *)owner, 0);
  Actor_StartRepeatedMotion(21, 1);
  Event_Wait(15);
  Actor_ShowEmote(21, 0x105, 30);
  Actor_SetAngleToward(23, *(s32 *)owner, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAngleToward(*(s32 *)owner, 23, 0);
  Event_Wait(15);
  Actor_SetModeById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_SetModeById(23, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetAngleToward(5, 23, 0);
  Actor_SetAngleToward(6, 23, 0);
  Actor_SetAngleToward(7, 23, 0);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(*(s32 *)owner, 3);
  Event_Wait(30);
  Actor_ShowEmote(15, 0x100, 0);
  Actor_ShowEmote(20, 0x100, 0);
  Func_080d47b4(21, 0x100, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Actor_SetAngleToward(*(s32 *)owner, 15, 0);
  Actor_SetAngleToward(5, 15, 0);
  Actor_SetAngleToward(6, 15, 0);
  Actor_SetAngleToward(7, 15, 0);
  Actor_SetAngleToward(23, 15, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(23, 0);
  Func_080d47b4(15, 0x109, 30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(5, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(20, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(20, 0);
  Func_080d47b4(6, 0x105, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_SetAngleToward(6, 5, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(6, 0);
  Actor_StartRepeatedMotion(7, 1);
  Event_Wait(30);
  Event_ShowMessageAndWait(7, 0);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 7, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_TurnToFaceEachOther(5, 6, 0);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(15);
  Actor_TurnToFaceEachOther(*(s32 *)owner, 5, 0);
  Actor_TurnToFaceEachOther(7, 6, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Event_Wait(30);
  Actor_SetAngleToward(*(s32 *)owner, 15, 0);
  Actor_SetAngleToward(5, 15, 0);
  Actor_SetAngleToward(6, 15, 0);
  Actor_SetAngleToward(7, 15, 0);
  Actor_SetAngleToward(23, 15, 0);
  Actor_SetModeById(15, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetModeById(*(s32 *)owner, 3);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(23, 3);
  Event_Wait(15);
  Actor_SetModeById(15, 3);
  Actor_SetModeById(20, 3);
  Actor_SetModeById(21, 3);
  Event_Wait(60);
  Actor_SetModeById(5, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(5, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetModeById(6, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(6, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(7, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Actor_SetModeById(23, 2);
  leader = Actor_GetById(*(s32 *)owner);
  if (leader != 0)
    Actor_ResetAndSetPosition(23, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(23);
  Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  GameFlag_SetBit(0x928);
  GameFlag_SetBit(0x350);
  Func_020021cc();
}

typedef s32 (*HandlerFn)(void);

void Func_02002294(s32, s32);
void Func_0200229c(s32, s32);
void Func_020022a4(void);
void Func_020022ac(void);
void Func_020022b4(void);
void Field_DispatchTypeHandler(s32 type);

s32 Func_02001d84(void) { return -1; }

void Func_02001d8c(void) {
  Func_02002294(140, 1);
  Func_0200229c(15, -1);
  FIELD_AT(Ram_HeapSlots->effectWork, HandlerFn, 36) = Func_02001d84;
  FIELD_AT(Ram_HeapSlots->effectWork, u8, 32) = 1;
  Func_020022b4();
  Event_Wait(10);
  Field_DispatchTypeHandler(0);
  Func_020022a4();
  Func_020022ac();
}

void Func_020021c4();
void Func_020021cc();
void Func_020022e4();
void Func_020022ec();
void Func_020022f4();
void Func_020022fc();
void PartyInventory_Remove();
extern u8 MsgAgainYouHave[];
extern u8 MsgAhCouldIt[];

void Func_02001dd0(void) {
  s32 t9, t21, t31;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAhCouldIt);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 144, 536);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Event_Wait(15);
  Actor_ShowEmote(15, 0x100, 60);
  Event_OpenMessage(15, 0);
  t9 = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t9 != 0)
    goto L_02009e3a;
  Event_SetMessageId(((s32)&MsgAhCouldIt + 1));
  Event_ShowMessageAndWait(15, 0);
  goto L_02009e48;
L_02009e3a:
  Event_SetMessageId(((s32)&MsgAhCouldIt + 2));
  Event_ShowMessageAndWait(15, 0);
L_02009e48:
  Func_080d47b4(15, 0x109, 60);
  Event_SetMessageId((s32)&MsgAgainYouHave);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(15, 0x105, 120);
  Func_080d47b4(15, 0x106, 60);
  Event_ShowMessageAndWait(15, 0);
  Func_020022e4();
  t21 = ((s32)Actor_GetById(15));
  Func_020022fc(t21, 2);
  Sound_Play(201);
  Event_Wait(60);
  Actor_SetPosition(24, PIXELS(144), PIXELS(520));
  Actor_Jump(24, 6, 0);
  Actor_SetDestination(24, 160, 520);
  Actor_WaitForMoveById(24);
  Actor_ArmCallback(24, FACING_SOUTH, 0);
  Event_Wait(60);
  t31 = ((s32)Actor_GetById(15));
  Func_020022fc(t31, 0);
  Func_020022f4();
  Func_020022ec();
  Event_Wait(30);
  Event_Wait(30);
  Actor_SetAngleToward(15, 24, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(30);
  Actor_SetModeById(15, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_TurnToFaceEachOther(15, 24, 0);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  Actor_Jump(24, 4, 20);
  Event_Wait(60);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(24, 2, 20);
  Func_080d7788(24, 2, 11);
  Func_020021cc();
  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(15, 2);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  GameFlag_SetBit(0x92c);
  PartyInventory_Remove(0x1c3);
  Func_020021cc();
}

s32 Func_0200200c(void) {
  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x201;
  if (gMainRoutineWork.entrance != 7)
    Event_ClearFlag(0x350);
  if (GameFlag_Test(0x927) && !GameFlag_Test(0x928) &&
      gMainRoutineWork.entrance == 7) {
    Event_PlaySound(7);
    Actor_SetPosition(22, 0x900000, 0x2380000);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (GameFlag_Test(0x927) && GameFlag_Test(0x928) &&
      gMainRoutineWork.entrance == 7 && GameFlag_Test(0x350)) {
    Event_PlaySound(7);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 5 && !GameFlag_Test(0x927)) {
    Actor_SetAnimation(17, 5);
    Actor_SetAttachedEffect(17, 0x101);
  }
  if (gMainRoutineWork.entrance == 7 && !GameFlag_Test(0x927)) {
    struct Actor *object;

    Actor_SetAnimation(9, 5);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    object = Actor_GetById(11);
    object->scaleX = -Actor_GetById(11)->scaleX;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
