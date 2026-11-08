#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

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
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
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

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

void Func_02000320(void) {
  s32 x, z;

  Func_02002bd0((s32)&MsgUsingPsynergyOn);
  Motion_SetVarCbAndRefresh(33, 2);
  FieldEvent_Wait(20);
  x = 0x108;
  z = 30;
  /* FAKEMATCH: the native loads both distances before the actor id. */
  __asm__ volatile("" : "+r"(x), "+r"(z));
  Func_02002c08(33, x, z);
  Func_02002be0(33, 0, 10);
  Motion_SetModeAndWaitAnimation(33, 4);
  FieldEvent_Wait(10);
  Func_02002be0(33, 0, 10);
}

void Func_0200036c(void) {
  if (Flag_Test(0x20f) == 0) {
    Func_02002b28();
    Func_02002c58(0);
    Func_02002c08(33, 0x105, 30);
    Func_02002bd0((s32)&MsgWhereCouldMenardi);
    Func_02002be0(33, 0, 10);
    Func_02002b30();
    GameFlag_SetBit(SPLIT_CONSTANT(0x110, 255));
  }
}

void Func_020003b8(s32 actor) {
  s32 msg = (s32)&MsgEveryoneHereIn;

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
}

void Func_02000404(void) {
  s32 msg;

  Actor_SetAngleToward(26, 4, 0);
  if (Flag_Test(0x281)) {
    Func_02002bd0((s32)&MsgYeahWellWe);
  } else {
    msg = (s32)&MsgYouCominThrough;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(26, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      GameFlag_SetBit(SPLIT_CONSTANT(0x182, 255));
      FieldEvent_Wait(10);
      Func_02002c08(26, 0x102, 0);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Motion_SetModeAndWaitAnimation(26, 3);
      Func_02002bd0(msg + 2);
    }
  }
  Func_02002be8(26, 0);
}

void Func_02000490(void) {
  s32 msg;

  Actor_SetAngleToward(26, 4, 0);
  if (Flag_Test(0x281)) {
    Func_02002bd0((s32)&MsgYeahUhWe);
  } else {
    msg = (s32)&MsgOhUhYou;
    Func_02002bd0(msg);
    FieldMessage_ShowAtActor(26, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02002c80(), 0) == 0) {
      GameFlag_SetBit(SPLIT_CONSTANT(0x182, 255));
      FieldEvent_Wait(10);
      Func_02002c08(26, 0x102, 0);
      Func_02002bd0(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Motion_SetModeAndWaitAnimation(26, 3);
      Func_02002bd0(msg + 2);
    }
  }
  Func_02002be8(26, 0);
}
