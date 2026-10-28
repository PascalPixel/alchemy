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

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void ArmCallback(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Walk(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002c08(id, a, b);
}

static __inline__ void OpenAt(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(id, flags);
}

static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002af0(a, b, c, d, e, f);
}

static __inline__ void CopyCells2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002af8(a, b, c, d, e, f);
}

static __inline__ void Emote(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002be0(id, a, b);
}

static __inline__ void Face(s32 id, s32 target, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetAngleToward(id, target, flags);
}

static __inline__ void Anim(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Show(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002b88(id, a, b);
}

static __inline__ void Place(s32 id, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002b90(id, x, y, z);
}

static __inline__ void Speed(s32 id, s32 s, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, s, a);
}

static __inline__ void SetPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void Turn(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002bf8(id, a);
}

void Func_02000108(s32 arg) {
  s32 angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  /* FAKEMATCH: the native loads the angle before building the quarter turn. */
  __asm__ volatile("" : "+r"(angle));
  if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
    Func_02002c88(4, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgItLooksLikeThe);
  else if (GameFlag_Test(0x8ab))
    Func_02002bd0((s32)&MsgTheArmorerAnd);
  else
    Func_02002bd0((s32)&MsgAfterTheChampa);
  Func_02002be8(29, 0);
}

void Func_0200018c(s32 arg) {
  s32 angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  /* FAKEMATCH: the native loads the angle before building the quarter turn. */
  __asm__ volatile("" : "+r"(angle));
  if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
    Func_02002c88(5, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgPeopleAreSaying);
  else if (GameFlag_Test(0x8ab))
    Func_02002bd0((s32)&MsgIKnewThere);
  else
    Func_02002bd0((s32)&MsgOwOwOw);
  Func_02002be8(30, 0);
}

void Func_02000210(s32 arg) {
  s32 angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  /* FAKEMATCH: the native loads the angle before building the quarter turn. */
  __asm__ volatile("" : "+r"(angle));
  if ((u16)((angle + 0x2000) & 0xc000) == 0xc000) {
    Func_02002c88(6, arg);
    return;
  }
  if (GameFlag_Test(0x8ff))
    Func_02002bd0((s32)&MsgMyBoyfriendShin);
  else if (GameFlag_Test(0x8ab))
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
