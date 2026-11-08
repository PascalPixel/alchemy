#include "local.h"

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000cd8(s32 unused, s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 60 instructions
   * differ in tla-en. */

  if (id == 24) {
    s32 column;
    s32 row;

    struct Actor *first = Actor_GetById(24);

    column = first->x >> 20;
    row = first->z >> 20;
    GameFlag_SetByte(0x310, column);
    GameFlag_SetByte(0x318, row);
    GameFlag_ClearBit_(0x320);
    GameFlag_ClearBit(SPLIT_CONSTANT(0x300, 33));
    if (column == 7 && row == 35)
      GameFlag_SetBit(0x320);
    if (column == 15 && row == 35)
      GameFlag_SetBit(SPLIT_CONSTANT(0x300, 33));
  } else if (id == 25) {
    struct Actor *object = Actor_GetById(25);

    if (object->x >> 20 == 32) {
      wait(30);
      object->z += 0x10000;
      GameFlag_SetBit(0x950);
    }
  }
}
