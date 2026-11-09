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

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

void Func_020029ac(void) { GameFlag_SetBit(0x910); }

void Func_020029b8(void) { GameFlag_SetBit(0x914); }

void Func_020029c8(void) {
  s32 next;

  next = 16;
  if (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154) > 34)
    next = 15;
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= 0xfe;
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(next);
  Func_02002b30();
}

void Func_02002a58(void) {
  s32 id;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  CommitPos(4, 0, -14);
  id = 4;
  Actor_CommitPositionAndActivate(id, 16, 0);
  Func_020029c8();
}

void Func_02002a80(void) {
  s32 id, x, z;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  CommitPos(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH: the actor id comes between the distances and their negation. */
  __asm__ volatile("" : "+r"(id));
  Actor_CommitPositionAndActivate(id, -x, -z);
  Func_020029c8();
}

void Func_02002ab0(void) {
  s32 id, a;

  Func_02002b28();
  Event_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH: the actor id comes between the angle byte and its shift. */
  __asm__ volatile("" : "+r"(id));
  Actor_ArmCallback(id, a << 8, 0);
  Func_020029c8();
}
