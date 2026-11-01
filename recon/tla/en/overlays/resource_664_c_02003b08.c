// Matches: differ 0 (tla-en, 2026-11-01)
// Func_02003b08 and Func_02003cac of overlay 664, measured with --symbol each:
// both differ 0, skeleton gap 0. The same forms are in
// games/tla/src/scene/airs_rock_inside/s3b08.c and s3cac.c; this draft holds
// nothing further.
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"
#include "call.h"

void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02002790(void);
void Func_02003b7c(s32 x, s32 y);

void Func_02003b08(void) {
  u8 *flags;
  u8 bit;

  if (((s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->z)[1] > 653)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90) &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 376, 668);
  flags = &BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90);
  /* FAKEMATCH: the bit as a u8 local is built in the register it ORs into, as
   * the native does; as |= 1 the byte loads there instead (differ 4, also as an
   * s32 local). */
  bit = 1;
  *flags = bit | *flags;
  Func_02005e1c();
}

void Func_02003cac(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct FieldActor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cell = Func_02000690(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_02002790();
    Func_02003b7c(0x108, row * 8 + 10);
    if (!Value1((s32 (*)())GameFlag_Test, 0x200))
      GameFlag_SetBit(0x200);
    GameFlag_SetBit(*(s16 *)work + 312);
  } else if (GameFlag_Test(0x200)) {
    Func_02003b7c(0x108, row * 8 + 10);
  }
}
