/// @file
/// Scene code of overlay 0x682, in ROM order, from s0188's offset up to the next object the overlay links.

#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02002a44();
s32 Func_02002b3c();
void Func_02002a4c(void);
extern u8 MsgChampa49[];

void Func_02000188(void) {
  s32 t24;
  s32 t26;
  s32 t29;

  Event_SetFlag(0x973);
  Func_02002a44();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgChampa49);
  Actor_FaceDirection(32, 0x3000, 0);
  FieldActor_Jump(32, 6, 20);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  FieldCamera_MoveTo(PIXELS(176), -1, PIXELS(256), 1);
  Func_02002b3c();
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(32, 3);
  Actor_SetAttachedEffect(0x20, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(33, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x21, 0x101, 0x23);
  Actor_FaceDirectionAndWait(33, 0x3000);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x21, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x21, 0, 5);
  FieldActor_Jump(32, 6, 0);
  FieldActor_Jump(33, 6, 0);
  Actor_SetSpeed(32, 0x1cccc, 0xe666);
  Actor_SetSpeed(33, 0x1cccc, 0xe666);
  t24 = ((s32)Actor_Get(32));
  FIELD_AT((t24 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t24 + 0x5a), u8, 0));
  Actor_WalkBy(32, 0, -16);
  t26 = ((s32)Actor_Get(33));
  FIELD_AT((t26 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t26 + 0x5a), u8, 0));
  Actor_WalkByAndWait(33, 0, -16);
  FieldEvent_Wait(1);
  t29 = ((s32)Actor_Get(33));
  FIELD_AT((t29 + 0x5a), u8, 0) = (1 | FIELD_AT((t29 + 0x5a), u8, 0));
  FieldEvent_Wait(15);
  Actor_ShowEmote(0x20, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  Actor_FaceDirection(32, 0xb000, 0);
  Actor_FaceDirectionAndWait(33, 0xd000);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x8021, 0, 5);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_WatchAndSetScript(32, 34);
  FieldActor_WatchAndSetScript(33, 34);
  Actor_SetSpeed(34, 0x13333, 0x9999);
  Actor_WalkByAndWait(34, 0, 32);
  Actor_FaceDirectionAndWait(34, 0x3000);
  FieldCamera_MoveTo(PIXELS(208), -1, PIXELS(296), 1);
  Func_02002b3c();
  FieldEvent_Wait(15);
  FieldActor_Jump(34, 6, 15);
  FieldActor_Jump(34, 6, 23);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_Jump(34, 6, 20);
  Actor_SetSpeed(34, 0x20000, 0x10000);
  Actor_WalkByAndWait(34, 0, -48);
  Actor_FaceDirection(32, 0xb000, 0);
  Actor_FaceDirection(33, 0xd000, 0);
  Actor_SetPosition(34, 0, 0);
  Actor_SetSpeed(32, 0x20000, 0x10000);
  Actor_SetSpeed(33, 0x20000, 0x10000);
  Actor_WalkByAndWait(32, -16, -16);
  Actor_WalkBy(33, 16, -16);
  Actor_WalkByAndWait(32, 0, -16);
  Actor_SetPosition(32, 0, 0);
  Actor_WalkByAndWait(33, 0, -16);
  Actor_SetPosition(33, 0, 0);
  Func_02002a4c();
}

void Actor_SetActionCallback(s32 *object, s32 action);

void Func_02000430(s32 actor) {
  s32 *object = ((s32 *)Actor_Get(actor));

  object[7] = 0x10000;
  object[6] = 0x10000;
  FieldEvent_SetMessageId((s32)MsgChampa07);
  Event_ShowMessage(actor, 0);
  Actor_FaceDirectionAndWait(actor, FACING_EAST);
  Actor_SetActionCallback(object, 8);
}

void Func_02000468(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

extern u8 MsgChampa30[];
extern u8 MsgChampa05[];

void Func_020004b4(s32 actor) {
  if (Event_GetFlag(0x97f)) {
    FieldEvent_SetMessageId((s32)MsgChampa30);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 2);
    }
    Event_ShowMessage(actor, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa05);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 2);
    }
    Event_ShowMessage(actor, 0);
  }
}

#include "event_inline.h"
extern u8 MsgChampa35[];
extern u8 MsgChampa36[];

void Func_02000554(s32 a0) {
  s32 t0;

  t0 = Event_GetFlag(0x972);
  if (t0 == 0) {
    Event_SetFlag(0x972);
    FieldEvent_SetMessageId((s32)MsgChampa35);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa36);
  }
  Event_ShowMessage(a0, 0);
}

void Func_02000590(s32 actor) {
  s32 msg = (s32)&MsgChampa11;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}
