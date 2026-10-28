#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "event_work.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_SetBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_WatchAndSetScript(s32 actor, s32 target);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
s32 FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_020014b8(void);
void Func_020014c0(void);
void Func_02001538(s32 message);
void Func_02001548(s32 id, s32 a, s32 b);
void Func_02001550(s32 id, s32 flags);
void Func_02001560(s32 id, s32 a, s32 b);
void Func_02001588(s32 scene, s32 entrance);
void Func_020015b8(s32 a);
s32 Func_020015e0(void);

void Func_0200006c(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor02;

  Func_02001538(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020015e0(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02001538(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02001538(msg + 2);
  }
  Func_02001550(actor, 0);
}

void Func_020000b8(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor09;

  Func_020014b8();
  Func_020015b8(0);
  Func_02001538(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020015e0(), 0) == 0) {
    FieldEvent_Wait(20);
    Actor_WatchAndSetScript(actor, gAdventure.currentOwner);
    Actor_SetPositionAndReset(gAdventure.currentOwner, 760, 320);
    Call3((void (*)())Actor_ArmCallback, 14, 0x5000, 0);
    Actor_ArmCallback(gAdventure.currentOwner, 0x4000, 0);
    FieldEvent_Wait(10);
    Func_02001588((s32)&SceneId_WorldMap, 67);
  } else {
    Func_02001538(msg + 2);
    Func_02001550(actor, 0);
  }
  Func_020014c0();
}

void Func_02000150(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor14;

  Func_02001538(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020015e0(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02001538(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02001538(msg + 2);
  }
  Func_02001550(actor, 0);
}

void Func_0200019c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8ba);
  Func_020014b8();
  Func_020015b8(0);
  Func_02001538((s32)&MsgAlhafraManor18);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Call3((void (*)())Func_02001560, 18, 256, 60);
  Actor_SetAngleToward(18, gAdventure.currentOwner, 0);
  Value3((s32 (*)())Func_02001560, 18, 263, 20);
  Func_02001548(18, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  FieldEvent_Wait(10);
  Func_02001548(18, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 4);
  Func_02001548(18, 0, 10);
  Func_020014c0();
}

void Func_02000238(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8bb);
  Func_020014b8();
  Func_020015b8(0);
  Func_02001538((s32)&MsgAlhafraManor23);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Call3((void (*)())Func_02001560, 8, 256, 30);
  Actor_WatchAndSetScript(8, gAdventure.currentOwner);
  Actor_SetPositionAndReset(gAdventure.currentOwner, 304, 152);
  Actor_ArmCallback(8, 0, 0);
  Actor_ArmCallback(gAdventure.currentOwner, 0x8000, 0);
  FieldEvent_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  FieldEvent_Wait(10);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  FieldEvent_Wait(10);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  FieldEvent_Wait(10);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Actor_ArmCallback(8, 0xe000, 0);
  FieldEvent_Wait(50);
  Actor_ArmCallback(8, 0, 0);
  FieldEvent_Wait(10);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  FieldEvent_Wait(10);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  Call3((void (*)())Func_02001548, 0x4008, 0, 10);
  Func_020014c0();
}

void Func_02000370(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)((u8 *)work + 0x166) != 0) {
    Actor_SetAngleToward(9, gAdventure.currentOwner, 0);
    FieldEvent_Wait(10);
  }
  GameFlag_SetBit(0x8bc);
  Func_020014b8();
  Func_020015b8(0);
  Func_02001538((s32)&MsgAlhafraManor24);
  FieldEvent_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  FieldEvent_Wait(10);
  Func_02001548(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  FieldEvent_Wait(10);
  Func_02001548(9, 0, 10);
  Actor_ArmCallback(9, 0xd000, 0);
  FieldEvent_Wait(30);
  Func_02001548(9, 0, 10);
  Actor_SetAngleToward(9, gAdventure.currentOwner, 0);
  FieldEvent_Wait(20);
  Func_02001548(9, 0, 10);
  Value3((s32 (*)())Func_02001560, 9, 258, 40);
  Func_02001548(9, 0, 20);
  Value3((s32 (*)())Func_02001560, 9, 264, 40);
  Func_02001548(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  Func_02001548(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  FieldEvent_Wait(10);
  Func_02001548(9, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  FieldEvent_Wait(10);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Func_02001548(9, 0, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    FieldEvent_Wait(30);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Func_02001548(9, 0, 10);
  }
  Func_020014c0();
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Call3((void (*)())Actor_ArmCallback, 9, 0xd000, 0);
}
