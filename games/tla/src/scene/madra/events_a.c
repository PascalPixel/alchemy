#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
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

void Func_02000108(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(4, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgItLooksLikeThe);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgTheArmorerAnd);
  else
    Event_SetMessageId((s32)&MsgAfterTheChampa);
  Event_ShowMessageAndWait(29, 0);
}

void Func_0200018c(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(5, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgPeopleAreSaying);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgIKnewThere);
  else
    Event_SetMessageId((s32)&MsgOwOwOw);
  Event_ShowMessageAndWait(30, 0);
}

void Func_02000210(s32 arg) {
  if ((u16)((Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) &
            0xc000) == 0xc000) {
    Func_02002c88(6, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Event_SetMessageId((s32)&MsgMyBoyfriendShin);
  else if (GameFlag_Test(0x8ab))
    Event_SetMessageId((s32)&MsgAllMyInjuries);
  else
    Event_SetMessageId((s32)&MsgMySisterWas);
  Event_ShowMessageAndWait(arg, 0);
}

void Func_02000294(void) {
  Event_SetMessageId((s32)&MsgIfYouDon);
  Motion_SetModeAndWaitAnimation(33, 4);
  Event_Wait(20);
  Event_ShowMessageThenWait(33, 0, 10);
}
