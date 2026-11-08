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

void Func_02000108(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(4, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgItLooksLikeThe);
  else if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171)))
    Func_02002bd0((s32)&MsgTheArmorerAnd);
  else
    Func_02002bd0((s32)&MsgAfterTheChampa);
  Func_02002be8(29, 0);
}

void Func_0200018c(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(5, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgPeopleAreSaying);
  else if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171)))
    Func_02002bd0((s32)&MsgIKnewThere);
  else
    Func_02002bd0((s32)&MsgOwOwOw);
  Func_02002be8(30, 0);
}

void Func_02000210(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(6, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgMyBoyfriendShin);
  else if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171)))
    Func_02002bd0((s32)&MsgAllMyInjuries);
  else
    Func_02002bd0((s32)&MsgMySisterWas);
  Func_02002be8(arg, 0);
}

void Func_02000294(void) {
  Func_02002bd0((s32)&MsgIfYouDon);
  Motion_SetModeAndWaitAnimation(33, 4);
  FieldEvent_Wait(20);
  Func_02002be0(33, 0, 10);
}
