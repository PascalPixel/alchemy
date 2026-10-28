// Near misses of overlay 664 (Func_02002bd0 and Func_02002c40 of this draft
// are adopted in s2bd0.c and s2c40.c). Func_02003b08 (47, 4):
// the native loads the flags into r2 and builds the bit 1 in r3 for the second
// flag update; a plain |= and an inline parameter both give the reverse.
// Func_02003cac (62, 30): the native keeps the event work pointer in r5 and
// adds the room offset into it, with the row in r6; the draft allocates them
// the other way. The row passes through an inline so that 54 - (room - 201) * 4
// does not fold.
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
s32 __divsi3(s32 a, s32 b);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02002790(void);
void Func_02003b7c(s32 x, s32 y);

static inline void FlagOn(u8 *flags, s32 bit) {
  /* FAKEMATCH: the bit passes through an inline parameter: the native builds it
   * in the register it ORs into (4 differ as a plain |=). */
  *flags = bit | *flags;
}

void Func_02003b08(void) {
  if (*(s16 *)((u8 *)Actor_GetById(gSequenceParentWork.currentOwner) + 18) <= 653) {
    Func_02005e14();
    Func_02005f2c(0);
    Func_02005f94(186);
    BYTE_AT(Actor_GetById(gSequenceParentWork.currentOwner), 90) &= ~1;
    Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x40000, 0x20000);
    Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 376, 668);
    FlagOn(&BYTE_AT(Actor_GetById(gSequenceParentWork.currentOwner), 90), 1);
    Func_02005e1c();
  }
}

static inline s32 RowTop(s32 row) {
  /* FAKEMATCH: the row passes through an inline parameter: the native subtracts
   * 201 and scales before it takes the result from 54 (written in one
   * expression the constants fold, 29 differ). */
  return 54 - row * 4;
}

void Func_02003cac(void) {
  s16 *room = (s16 *)Ram_HeapSlots->eventWork;
  struct FieldActor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 kind = Func_02000690(2, leader->x >> 20, leader->z >> 20) >> 8;
  s32 row;

  room += 181;
  row = RowTop(*room - 201);

  if (kind != 1 && kind != 7) {
    Func_02002790();
    Func_02003b7c(264, row * 8 + 10);
    if (GameFlag_Test(0x200) == 0)
      GameFlag_SetBit(0x200);
    GameFlag_SetBit(*room + 312);
  } else if (GameFlag_Test(0x200)) {
    Func_02003b7c(264, row * 8 + 10);
  }
}
