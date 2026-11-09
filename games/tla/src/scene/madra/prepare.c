#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002c88(s32 a, s32 b);
void Func_02000f80(void);
void Func_020019d8(void);

extern const u8 gData_02002d38[];
extern const u8 gData_02002ff0[];
extern const u8 gData_02003358[];
extern const u8 gData_0200367c[];
extern const u8 gData_0200394c[];

extern u8 MsgItLooksLikeThe;
extern u8 MsgTheArmorerAnd;
extern u8 MsgAfterTheChampa;
extern u8 MsgPeopleAreSaying;
extern u8 MsgIKnewThere;
extern u8 MsgOwOwOw;
extern u8 MsgMyBoyfriendShin;
extern u8 MsgAllMyInjuries;
extern u8 MsgMySisterWas;
extern u8 MsgIfYouDon;
extern u8 MsgHaveYouBy;
extern u8 MsgUsingPsynergyOn;
extern u8 MsgWhereCouldMenardi;
extern u8 MsgEveryoneHereIn;
extern u8 MsgYeahWellWe;
extern u8 MsgYouCominThrough;
extern u8 MsgYeahUhWe;
extern u8 MsgOhUhYou;
extern u8 MsgWasIJust;
extern u8 MsgHeyMisterAre;
extern u8 MsgDidYouCome;
extern u8 MsgIsThereBy;

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200083c(s32 actor) {
  s32 msg;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgWasIJust;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
  Func_02002b30();
}

void Func_02000894(void) {
  s32 msg;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x8ab)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0)
      Actor_SetAngleToward(14, 4, 0);
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Func_02002b30();
}

void Func_02000968(void) {
  s32 msg, x, z;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x8ab)) {
    msg = (s32)&MsgHeyMisterAre;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0) {
      x = 0x100;
      z = 30;
      /* FAKEMATCH: the native loads both distances before the actor id. */
      __asm__ volatile("" : "+r"(x), "+r"(z));
      Func_080d47b4(14, x, z);
      Actor_SetAngleToward(14, 4, 0);
    }
    msg = (s32)&MsgDidYouCome;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Func_02002b30();
}

void Func_02000a48(void) {
  s32 flags;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(11, 4, 0);
  Event_SetMessageId((s32)&MsgIsThereBy);
  flags = 0;
  /* FAKEMATCH: the native loads the flags before the actor id. */
  __asm__ volatile("" : "+r"(flags));
  FieldMessage_ShowAtActor(11, flags);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    GameFlag_SetBit(0x301);
    Event_ShowMessageAndWait(11, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageAndWait(11, 0);
  }
  Func_02002b30();
}
