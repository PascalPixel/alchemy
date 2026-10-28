/// @file
/// Kibombo's setup.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "call.h"

extern u16 gData_02007948[4];
struct Actor *Actor_GetById(u32 id);
void Func_02005a9c(void);
void Func_02005aa4(void);
void Func_02005c3c(s32 skip);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
u8 *Unit_Get(s32 unit);
void Party_RemoveOwnerRestored(s32 unit);
void Func_020050ac(void);
void Func_020031f0(s32 id);
void Func_02005b0c(s32 id, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Task_Add(void (*func)(void), s32 priority);
void Func_02002c34(void);

static __inline__ s32 Gauge(s32 value, s32 max) {
  s32 gauge = (value << 14) / max;

  s32 result;

  if (gauge > 0x4000)
    result = 0x4000;
  else {
    result = 0;
    if (gauge >= 0)
      result = gauge;
  }
  return result;
}

/// Unless flag 265 is set, places actor 11 and, arriving by entrance 19 with
/// unit 7 in the party (flag 7), restores unit 7's HP and PP, sets their
/// gauges, clears two of its bytes, sets flag 0x92f, clears 0x92e and removes
/// it from the party; then saves four palette colours, frees actors 30 to 35,
/// sets flags 0x202 and 0x209, places actor 29, places actors 21 and 26 to 28
/// when flag 0x8fd is set, sets flag 0x86b and fades in with Func_02002c34
/// scheduled.
void Func_0200516c(void) {
  struct Actor *actor = Actor_GetById(11);
  u8 *unit;
  s32 done;

  Func_02005a9c();
  Func_02005c3c(0);
  done = GameFlag_Test(265);
  if (!done) {
    actor->motionFlags = done;
    actor->y = 0x2a0000;
    actor->z += -0x2a0000;
    Actor_SetSingleChildFlags(Actor_GetById(11), 0);
    Actor_SetSpritePriority(11, 1);
    if (gSequenceParentWork.entrance == 19 && GameFlag_Test(7)) {
      unit = Unit_Get(7);
      *(u16 *)(unit + 0x38) = *(u16 *)(unit + 0x34);
      *(u16 *)(unit + 0x3a) = *(u16 *)(unit + 0x36);
      /* FAKEMATCH(barrier): both copies stored before the gauge's argument
       * is extended; without it, 2 of 218 instructions differ in tla-en. */
      asm("");
      *(u16 *)(unit + 0x14) =
          Gauge(*(s16 *)(unit + 0x38), *(s16 *)(unit + 0x34));
      if (*(u16 *)(unit + 0x14) == 0 && *(s16 *)(unit + 0x38) != 0) {
        /* FAKEMATCH(temporary): the 1 built with movs as a word; as a halfword
         * constant it loads from the pool and 23 of 218 instructions differ in
         * tla-en. */
        s32 one = 1;

        *(s16 *)(unit + 0x14) = one;
      }
      *(u16 *)(unit + 0x16) =
          Gauge(*(s16 *)(unit + 0x3a), *(s16 *)(unit + 0x36));
      if (*(u16 *)(unit + 0x16) == 0 && *(s16 *)(unit + 0x3a) != 0) {
        /* FAKEMATCH(temporary): as for the HP gauge; as a halfword constant, 19
         * of 218 instructions differ in tla-en. */
        s32 one = 1;

        *(s16 *)(unit + 0x16) = one;
      }
      unit[0x131] = 0;
      unit[0x140] = 0;
      GameFlag_SetBit(0x92f);
      GameFlag_ClearBit(0x92e);
      Party_RemoveOwnerRestored(7);
    }
  }
  GameFlag_SetBit(0x202);
  gData_02007948[0] = *(u16 *)0x05000012;
  gData_02007948[1] = *(u16 *)0x050000ae;
  gData_02007948[2] = *(u16 *)0x050000ce;
  gData_02007948[3] = *(u16 *)0x050000ee;
  Func_020050ac();
  Actor_SetSingleChildFlags(Actor_GetById(30), 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 0);
  Actor_SetSingleChildFlags(Actor_GetById(32), 0);
  Actor_SetSingleChildFlags(Actor_GetById(33), 0);
  Actor_SetSingleChildFlags(Actor_GetById(34), 0);
  Actor_SetSingleChildFlags(Actor_GetById(35), 0);
  GameFlag_SetBit(0x209);
  actor = Actor_GetById(29);
  actor->motionFlags = 0;
  actor->y = 0;
  Func_020031f0(29);
  Actor_SetSpritePriority(30, 3);
  if (GameFlag_Test(0x8fd)) {
    Func_02005b0c(26, 0, 0);
    Func_02005b0c(27, 0, 0);
    Func_02005b0c(28, 0, 0);
    Call3((void (*)())Func_02005b0c, 21, 0x12e0000, 0xac0000);
  }
  GameFlag_SetBit(0x86b);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    /* FAKEMATCH(temporary): the priority built before the task function's address;
     * as a plain argument, 2 of 218 instructions differ in tla-en. */
    s32 priority = 0x480;

    Task_Add(Func_02002c34, priority);
  }
  Func_02005aa4();
}
