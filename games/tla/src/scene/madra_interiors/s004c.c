/// @file
/// Scene code of overlay 0x659, in ROM order, from s004c's offset up to the next object the overlay links.

#include "madra_interiors.h"
#include "version.h"
#include "scene_tables.h"
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

const u8 *Func_0200004c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002828;
  if (gMainRoutineWork.entrance == 20)
    return gData_02002750;
  if (Event_GetFlag(0x8ff))
    return gData_02002480;
  return gData_02002240;
}

void Func_020000a4(void) {
  s32 msg;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)MsgMadraInteriors06;
  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(9, 0);
  Func_02001d58();
}

void Func_020000fc(void) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001f08(1, 22);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors155);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
    Event_ShowMessage(22, 0);
#else
    if (Event_GetFlag(0x8ff)) {
      s32 msg = (s32)MsgMadraInteriors155;

      FieldEvent_SetMessageId(msg);
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
        FieldEvent_Wait(10);
        FieldEvent_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        FieldEvent_SetMessageId(msg + 2);
      }
      Event_ShowMessage(22, 0);
    } else {
      if (Event_GetFlag(0x8ab))
        FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
      else
        FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
      Event_ShowMessage(22, 0);
    }
#endif
  }
}

void Func_020001b8(s32 actor) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001f00(actor);
  } else {
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors149);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors81);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors33);
    Event_ShowMessage(actor, 0);
  }
}

void Func_0200023c(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(14, 4, 0);
  FieldActor_Jump(14, 6, 20);
  Actor_FaceActor(4, 14, 0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors18);
  Event_ShowMessage(14, 0);
  ActorTurn(14, FACING_SOUTHEAST, 0);
  Func_02001d58();
}

void Func_0200028c(void) {
  s32 msg;

  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310))
      msg = (s32)MsgMadraInteriors111;
    else
      msg = (s32)MsgMadraInteriors118;
    FieldEvent_SetMessageId(msg);
    Event_ShowMessage(10, 0);
  } else {
    if (Event_GetFlag(0x210)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors116);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors106);
      Event_ShowMessage(10, 0);
    }
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  }
}

void Func_02000308(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors112);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors119);
      Event_ShowMessage(11, 0);
    }
  } else if (Event_GetFlag(0x210)) {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors117);
    Event_ShowMessage(11, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors107);
    Event_ShowMessage(11, 0);
  }
}

void Func_02000368(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors113);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors120);
      Event_ShowMessage(10, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors108;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(10, 0);
    }
  }
}

void Func_020003c8(void) {
  if (Event_GetFlag(0x98a)) {
    if (Event_GetFlag(0x310)) {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors114);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId((s32)MsgMadraInteriors121);
      Event_ShowMessage(11, 0);
    }
  } else {
    s32 flag = Event_GetFlag(0x210);
    s32 msg = (s32)MsgMadraInteriors109;

    if (flag) {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    } else {
      FieldEvent_SetMessageId(msg);
      Event_ShowMessage(11, 0);
    }
  }
}

void Func_02000428(void) {
  if (!FlagCheck(0x211)) {
    Event_SetFlag(0x211);
    FieldEvent_SetMessageId((s32)MsgUhhhnnnUhhhnnn2);
    Actor_WalkByAndWait(14, -12, 0);
    FieldEvent_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    FieldEvent_Wait(20);
    Event_ShowMessage(14, 0);
    FieldActor_ShowEmote(14, 265, 30);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(14, 12, 0);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMadraInteriors97);
    Actor_FaceActor(14, 4, 0);
    Event_ShowMessage(14, 0);
  }
}

const u8 *Func_020004c4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002c9c;
  if (gMainRoutineWork.entrance == 20)
    return gData_02002bb8;
  if (Event_GetFlag(0x8ff))
    return gData_0200308c;
  if (Event_GetFlag(0x8ab))
    return gData_02002da4;
  return gData_020028a0;
}

void Func_02000534(void) {
  if (Event_GetFlag(0x8ab)) {
    ActorPlace(26, PIXELS(888), PIXELS(8));
  } else {
    Actor_SetItemIcon(Actor_Get(26), 242);
    Actor_SetSingleChildFlags(Actor_Get(26), 0);
  }
}

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, u16 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Func_02000570(void) {
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionAndAngleWithTerrain
   * forwarded through ActorPlaceFacing; as direct calls to
   * Actor_SetHorizontalPositionAndAngleWithTerrain, 18 of 72 instructions
   * differ in tla-en. */
  if (!Event_GetFlag(0x8ab)) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 0xd4c) = 3;
    if (!Event_GetFlag(0x911)) {
      Func_020007dc();
    } else {
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(10, 0, 0);
    }
  } else if (Event_GetFlag(0x8ff)) {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHEAST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetPosition(8, 0, 0);
  } else {
    ActorPlaceFacing(9, PIXELS(515), PIXELS(841), FACING_NORTHWEST);
    ActorPlaceFacing(10, PIXELS(472), PIXELS(877), FACING_NORTH);
    Actor_SetPosition(8, 0, 0);
  }
}

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void Field_DispatchTypeHandler();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();

void Func_02001e80(s32 value);

/// The rooms' setup: the leader's sprite priority, the two rooms' actors and
/// the sprite of actor 10, by the story flags.
s32 Func_02000624(void) {
  struct FieldSprite *sprite;
  void *event;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001e80(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room1) {
    event = Ram_HeapSlots->eventWork;
    FIELD_AT(event, u16, 0xd4c) = 3;
    if (!FlagCheck(0x821)) {
      ActorPlace(29, 0, 0);
      ActorPlace(30, 0, 0);
    } else {
      Actor_SetSpriteColor(Actor_Get(29), 3);
      Actor_SetSpriteColor(Actor_Get(30), 5);
    }
    if (FlagCheck(0x8ab)) {
      MapGrid_CopyCellsLayered(2, 110, 16, 10, 16, 64);
      MapGrid_CopyCellAttributes(2, 46, 16, 10, 16, 0);
    }
  }
  if (gMainRoutineWork.entrance == 20)
    Func_02000570();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2) {
    if (FlagCheck(0x8ab)) {
      ActorPlace(10, 0, 0);
    } else {
      Actor_SetSpriteRotationById(10, 0xc000);
      Actor_SetSingleChildFlags(Actor_Get(10), 0);
      ((struct FieldSprite *)Actor_Get(10)->animation)->nextFrame |= 8;
      sprite = Actor_Get(10)->animation;
      sprite->unk11 = (sprite->unk11 & 3) | 32;
      ((struct FieldSprite *)Actor_Get(10)->animation)->unk19 = 1;
    }
  }
  Func_02000534();
  if (FlagCheck(0x8ff))
    Actor_SetSpriteColor(Actor_Get(33), 2);
  else
    Actor_SetSpriteColor(Actor_Get(28), 2);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 Func_020007ac(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation -= 0x800;
  return 0;
}

s32 Func_020007c8(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

void Func_020007dc(void) {
  s32 i;
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;
  struct Actor *object8;
  struct Actor *object9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  s32 result13;
  struct Actor *object14;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(8, 1);
  Actor_SetPosition(4, PIXELS(400), PIXELS(888));
  FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(848), 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors28);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_WalkToAndWait(4, 396, 869);
  FieldActor_LaunchFromLeader(11, 25, 19, FACING_NORTH);
  FieldActor_RefreshSelectorById(11);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(11, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(820), 1);
  Func_02001e68();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x107, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_WalkByAndWait(7, 0, -16);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x103, 0);
  object0 = Actor_Get(9);
  object0->actionFlags &= 254;
  for (i = 2; i >= 0; i--) {
    Actor_WalkByAndWait(9, 0, -8);
    Event_PlaySound(134);
    Actor_WalkByAndWait(9, 0, 8);
  }
  object1 = Actor_Get(9);
  object1->actionFlags |= 1;
  FieldEvent_Wait(30);
  Actor_ShowEmote(7, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(7, 0x107, 30);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceActor(10, 9, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_FaceActor(9, 10, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x107, 20);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(7, 0, 16);
  Func_02001e90(24, 1);
  Func_02001e98(7, 8);
  Func_02001eb0();
  Actor_ShowEmote(9, 0x101, 30);
  Actor_FaceActor(9, 7, 0);
  Actor_FaceActor(10, 7, 0);
  object2 = Actor_Get(9);
  Actor_SetSingleChildFlags(object2, 0);
  Actor_SetScript(9, gData_0200349c);
  Field_DispatchTypeHandler(1);
  Func_02001ea0();
  Func_02001ea8();
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  FieldActor_Stop(9);
  object3 = Actor_Get(9);
  object3->scaleX = 0x10000;
  object4 = Actor_Get(9);
  object4->scaleY = 0x10000;
  FieldEvent_Wait(4);
  object5 = Actor_Get(9);
  object5->motionFlags = 2;
  object6 = Actor_Get(9);
  object6->actionFlags &= 254;
  Actor_SetDestinationOffset(9, -8, 0);
  Actor_SetScript(9, gData_02003418);
  FieldActor_RefreshSelectorById(9);
  Event_PlaySound(127);
  FieldEvent_Wait(60);
  object7 = Actor_Get(9);
  object7->actionFlags = (object7->actionFlags | 1);
  FieldActor_Jump(9, 6, 0);
  FieldEvent_Wait(3);
  Actor_SetSpriteRotationById(9, 0);
  object8 = Actor_Get(9);
  Actor_SetSingleChildFlags(object8, 1);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(10, 0x100, 30);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  Actor_FaceActor(9, 7, 0);
  FieldEvent_Wait(20);
  object9 = Actor_Get(10);
  object9->actionFlags &= 254;
  object10 = Actor_Get(9);
  object10->actionFlags &= 254;
  Actor_WalkByAndWait(9, -16, 16);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  object11 = Actor_Get(9);
  object11->actionFlags = (object11->actionFlags | 1);
  Actor_SetScript(9, gData_020033c8);
  FieldActor_WatchAndSetScript(4, 9);
  FieldActor_WatchAndSetScript(11, 9);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 7, 0);
  FieldActor_WatchAndSetScript(4, 10);
  FieldActor_WatchAndSetScript(11, 10);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x105, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  Actor_WalkByAndWait(10, -16, 16);
  FieldEvent_ShowMessageThenWait(10, 0, 1);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 8);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  object12 = Actor_Get(10);
  object12->actionFlags |= 1;
  Actor_SetScript(10, gData_020033c8);
  FieldEvent_Wait(40);
  FieldActor_Stop(4);
  FieldActor_Stop(11);
  FieldActor_RefreshSelectorById(10);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetAnimation(8, 6);
  Actor_SetSpriteColorById(8, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_WaitForAnimationChange(8);
  FieldCamera_FollowActor(4, 1);
  Func_02001e68();
  Actor_FaceEachOther(11, 4, 0);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  Event_OpenMessage(11, 0);
  result13 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result13 != 0)
    goto L_02008e40;
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_02008e64;
L_02008e40:
  Actor_SetAnimationAndWait(11, 4);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  FieldEvent_ShowMessageThenWait(11, 0, 10);
L_02008e64:
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(11, 2);
  object14 = Actor_Get(4);
  if (object14 == 0)
    goto L_02008eb2;
  Actor_SetDestination(11, *(s16 *)((u8 *)object14 + 10),
                            *(s16 *)((u8 *)object14 + 18));
L_02008eb2:
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0x911);
  Func_02001d58();
  return;
}

void Func_02000ed8(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors29);
  Actor_FaceDirection(11, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Func_02001d58();
  return;
}

void Func_02000f0c(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Func_02001d58();
  return;
}

void Func_02000fe0(void) {
  Func_02001e90(24, 1);
  Func_02001e98(7, 8);
  Func_02001eb0();
  Field_DispatchTypeHandler(1);
  Func_02001ea0();
  Func_02001ea8();
  return;
}

void Func_02001ef8();
void Func_02001910(void);
void Func_02001a20(void);
void PartyInventory_Remove();
extern u32 gData_02001f18[];
extern u32 gData_02001f4c[];
extern u32 gData_02001f90[];
extern u32 gData_02001fc4[];
extern u32 gData_02002054[];
extern u32 gData_020020e4[];

void Func_02001008(void) {
  struct Actor *object;

  Event_SetFlag(0x98a);
  Event_SetFlag(0x310);
  PartyInventory_Remove(0x1c0);
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(11, 0x6666, 0x3333);
  Actor_WalkTo(11, 410, 109);
  object = Actor_Get(4);
  if (object->z > 0x5f0000)
    Actor_WalkToAndWait(4, object->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_WaitForMoveById(11);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_WalkToAndWait(11, 378, 109);
  Actor_FaceDirection(11, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(11, 0x102, 30);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceActor(10, 11, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(11, 0x106, 30);
  FieldActor_Stop(25);
  Actor_SetSpeed(10, 0x8000, 0x4000);
  Actor_SetSpeed(11, 0x8000, 0x4000);
  Actor_SetSpeed(25, 0x8000, 0x4000);
  Actor_SetScript(11, (s32)gData_02001f18);
  Actor_SetScript(4, (s32)gData_02001f4c);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gData_02001f90);
  FieldActor_RefreshSelectorById(10);
  FieldEvent_Wait(120);
  Actor_SetScript(11, (s32)gData_02001fc4);
  FieldActor_WatchAndSetScript(4, 11);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gData_02002054);
  Actor_SetScript(25, (s32)gData_020020e4);
  FieldActor_RefreshSelectorById(10);
  FieldActor_RefreshSelectorById(11);
  FieldActor_RefreshSelectorById(25);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Djinn_JoinParty(25, 2, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Actor_SetAnimation(4, 1);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Func_02001d58();
}

void Func_02001310(void) {
  struct Actor *object0;

  Event_SetFlag(0x210);
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  object0 = Actor_Get(4);
  if (object0->z > 0x5f0000)
    Actor_WalkToAndWait(4, object0->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors115);
  Actor_ShowEmote(10, 0x100, 30);
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x105, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Func_02001d58();
  return;
}

void Func_02001450(void) {
  s32 result0;
  s32 result1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors126);
  Actor_ShowEmote(28, 0x100, 30);
  Actor_FaceActor(28, 4, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceActor(18, 4, 0);
  Actor_FaceActor(14, 4, 0);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceActor(16, 4, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 701, 93);
  Actor_WalkBy(4, -32, 0);
  FieldActor_LaunchFromLeader(5, -8, 32, FACING_WEST);
  FieldActor_LaunchFromLeader(31, -16, 0, FACING_SOUTH + FACING_STEP);
  FieldActor_LaunchFromLeader(6, -8, 16, FACING_SOUTHWEST);
  FieldActor_LaunchFromLeader(7, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_ShowEmote(27, 0x100, 30);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(27, 0x109, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceEachOther(7, 4, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_Jump(18, 6, 20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(28, 0x102, 30);
  Event_OpenMessage(28, 0);
  result0 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result0 != 0)
    goto L_02009690;
  Actor_ShowEmote(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(15, 0x108, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  goto L_02009700;
L_02009690:
  Actor_ShowEmote(15, 0x102, 30);
  Func_02001ef8();
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
L_02009700:
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  result1 = Event_GetFlag(0x981);
  if (result1 == 0)
    goto L_02009760;
  Func_02001910();
  goto L_02009764;
L_02009760:
  Func_02001a20();
L_02009764:
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(31, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(31, 0x13333, 0x9999);
  Actor_SetAnimation(31, 2);
  object2 = Actor_Get(4);
  if (object2 == 0)
    goto L_02009832;
  Actor_SetDestination(31, *(s16 *)((u8 *)object2 + 10),
                            *(s16 *)((u8 *)object2 + 18));
L_02009832:
  FieldActor_WaitForMoveById(31);
  Actor_SetPosition(31, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object3 = Actor_Get(4);
  if (object3 == 0)
    goto L_02009870;
  Actor_SetDestination(6, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02009870:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_Get(4);
  if (object4 == 0)
    goto L_020098ae;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_020098ae:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_Get(4);
  if (object5 == 0)
    goto L_020098ec;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_020098ec:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Event_SetFlag(0x989);
  Func_02001d58();
  return;
}

void Func_02001910(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors127);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceEachOther(4, 31, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}

void Func_02001a20(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors128);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceActor(6, 5, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(17, 0x101, 30);
  Actor_FaceActor(7, 17, 0);
  Actor_ShowEmote(7, 0x108, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(31, 0x102, 30);
  Actor_FaceActor(31, 7, 0);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(60);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceActor(4, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(5, 7, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceActor(7, 6, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(4, 0x101, 30);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(5, 0x109, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Event_SetFlag(0x981);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}
