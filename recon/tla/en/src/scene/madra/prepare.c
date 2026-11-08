#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02002ae8(const void *data, s32 a, s32 b);
void Func_02002af0(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002af8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002b28(void);
void Func_02002b30(void);
void Func_02002b88(s32 id, s32 a, s32 b);
void Func_02002b90(s32 id, s32 x, s32 y, s32 z);
void Func_02002bd0(s32 message);
void Func_02002be0(s32 id, s32 a, s32 b);
void Func_02002be8(s32 a, s32 b);
void Func_02002bf8(s32 id, s32 a);
void Func_02002c08(s32 id, s32 a, s32 b);
void Func_02002c30(s32 a);
void Func_02002c58(s32 a);
s32 Func_02002c80(void);
void Func_02002c88(s32 a, s32 b);
void Func_02002c90(s32 a);
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
  Func_02002c58(0);
  msg = (s32)&MsgWasIJust;
  Func_02002bd0(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
    FieldEvent_Wait(10);
    Func_02002bd0(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Func_02002bd0(msg + 2);
  }
  Func_02002be8(actor, 0);
  Func_02002b30();
}

void Func_02000894(void) {
  s32 msg;

  Func_02002b28();
  Func_02002c58(0);
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x800, 171)))) {
    msg = (s32)&MsgHeyMisterAre;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02002bd0(msg + 2);
    }
    Func_02002be8(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0)
      Actor_SetAngleToward(14, 4, 0);
    msg = (s32)&MsgDidYouCome;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02002bd0(msg + 2);
    }
    Func_02002be8(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Func_02002b30();
}

void Func_02000968(void) {
  s32 msg, x, z;

  Func_02002b28();
  Func_02002c58(0);
  if (GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x800, 171)))) {
    msg = (s32)&MsgHeyMisterAre;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02002bd0(msg + 2);
    }
    Func_02002be8(8, 0);
  } else {
    if (GameFlag_Test(0x300) == 0) {
      x = 0x100;
      z = 30;
      /* FAKEMATCH: the native loads both distances before the actor id. */
      __asm__ volatile("" : "+r"(x), "+r"(z));
      Func_02002c08(14, x, z);
      Actor_SetAngleToward(14, 4, 0);
    }
    msg = (s32)&MsgDidYouCome;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      FieldEvent_Wait(10);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Func_02002bd0(msg + 2);
    }
    Func_02002be8(8, 0);
  }
  Actor_ArmCallback(14, FACING_SOUTH, 0);
  GameFlag_SetBit(0x300);
  Func_02002b30();
}

void Func_02000a48(void) {
  s32 flags;

  Func_02002b28();
  Func_02002c58(0);
  Actor_SetAngleToward(11, 4, 0);
  Func_02002bd0((s32)&MsgIsThereBy);
  flags = 0;
  /* FAKEMATCH: the native loads the flags before the actor id. */
  __asm__ volatile("" : "+r"(flags));
  FieldMessage_ShowAtActor(11, flags);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x300, 1)));
    Func_02002be8(11, 0);
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02002be8(11, 0);
  }
  Func_02002b30();
}
