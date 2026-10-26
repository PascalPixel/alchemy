// Draft, differ 35 of 218 (was 45): Kibombo's setup: unless flag 265 is set,
// places actor 11 and, arriving by entrance 19 with unit 7 in the party (flag
// 7), restores unit 7's HP and PP, sets their gauges, clears two of its bytes,
// sets flag 0x92f, clears 0x92e and removes it from the party; then saves four
// palette colours, frees actors 30 to 35, sets flags 0x202 and 0x20b, places
// actor 29, places actors 21 and 26 to 28 when flag 0x8fd is set, sets flag
// 0x86b and fades in with Func_02002c34 scheduled. Measured: the gauge clamp
// builds its zero before the sign test when the result starts at 0 and takes
// the gauge only when it is not negative (45 to 35; a ternary in either order
// or an if-else chain does not). Left: the minimum gauge 1 is a halfword
// constant this draft loads from the literal pool where the native has movs
// (u16 or s16 stores alike), and that pool word moves the first literal pool
// inside the guarded block, so the guard needs a long branch and every later
// pc offset shifts.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern u16 gData_02007948[4];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005a9c(void);
void Func_02005aa4(void);
void Func_02005c3c(s32 skip);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
u8 *Unit_Get(s32 unit);
void Party_RemoveOwnerRestored(s32 unit);
void Func_020050ac(void);
void Func_020031f0(s32 id);
void Func_02005b0c(s32 id, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
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

void Func_0200516c(void) {
  struct ObjectRuntime *actor = Object_GetById(11);
  u8 *unit;
  s32 done;

  Func_02005a9c();
  Func_02005c3c(0);
  done = GameFlag_Test(265);
  if (!done) {
    actor->motionFlags = done;
    actor->y = 0x2a0000;
    actor->z += -0x2a0000;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
    ObjectMotion_SetActionVariant(11, 1);
    if (gPartyState.entrance == 19 && GameFlag_Test(7)) {
      unit = Unit_Get(7);
      *(u16 *)(unit + 0x38) = *(u16 *)(unit + 0x34);
      *(u16 *)(unit + 0x3a) = *(u16 *)(unit + 0x36);
      *(u16 *)(unit + 0x14) =
          Gauge(*(s16 *)(unit + 0x38), *(s16 *)(unit + 0x34));
      if (*(u16 *)(unit + 0x14) == 0 && *(s16 *)(unit + 0x38) != 0)
        *(s16 *)(unit + 0x14) = 1;
      *(u16 *)(unit + 0x16) =
          Gauge(*(s16 *)(unit + 0x3a), *(s16 *)(unit + 0x36));
      if (*(u16 *)(unit + 0x16) == 0 && *(s16 *)(unit + 0x3a) != 0)
        *(s16 *)(unit + 0x16) = 1;
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
  ObjectDispatch_SetSingleChildFlags(Object_GetById(30), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(31), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(32), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(33), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(34), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(35), 0);
  GameFlag_SetBit(0x20b);
  actor = Object_GetById(29);
  actor->motionFlags = 0;
  actor->y = 0;
  Func_020031f0(29);
  ObjectMotion_SetActionVariant(30, 3);
  if (GameFlag_Test(0x8fd)) {
    Func_02005b0c(26, 0, 0);
    Func_02005b0c(27, 0, 0);
    Func_02005b0c(28, 0, 0);
    Func_02005b0c(21, 0x12e0000, 0xac0000);
  }
  GameFlag_SetBit(0x86b);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Scheduler_AddOrUpdateCallback(Func_02002c34, 0x480);
  Func_02005aa4();
}
