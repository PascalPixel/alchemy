// Draft, differ 263 of 209: Aqua Rock's five water gates (Data_02003450, 32
// bytes each): each follows its flag (the low 12 bits of its halfword
// 0x12, inverted by bit 12); a change queues the new state in one of its
// two animation slots, and each running slot copies the gate's next row of
// cells, open or closed, into the map and its attributes (Func_02002968,
// Func_02002958) until the gate's height is drawn. Left: a rough reading: the native writes the two copies once for each of the slot's two states (the draft folds them into one pair of calls), keeps the slot offsets in r9 and on the stack, and its count tests sit at the loop bottoms.
#include "types.h"

extern u8 Data_02003450[];
s32 GameFlag_Test(s32 flag);
void Func_02002968(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002958(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02000580(void) {
  u8 *gate = Data_02003450;
  u8 *state = gate + 2;
  s32 n, slot, flag, row, open, which;

  for (n = 0; n <= 4; n++, gate += 32, state += 32) {
    flag = GameFlag_Test(*(u16 *)(state + 16) & 0xfff);
    if (*(u16 *)(state + 16) & 0x1000)
      flag ^= 1;
    if (*(s16 *)(state + 18) != flag) {
      *(u16 *)(state + 20 + *(s16 *)(state + 28) * 2) = 0;
      *(u16 *)(state + 24 + *(s16 *)(state + 28) * 2) = flag;
      if ((s16)++*(u16 *)(state + 28) > 1)
        *(u16 *)(state + 28) = 0;
      *(s16 *)(state + 18) = flag;
    }
    for (slot = 1; slot >= 0; slot--) {
      row = *(s16 *)(state + 20 + (1 - slot) * 2);
      if (row >= *(s16 *)(state + 14) * 4)
        continue;
      open = *(s16 *)(state + 24 + (1 - slot) * 2);
      which = (*(u16 *)(state + 20 + (1 - slot) * 2) & 1) ? open : open ^ 1;
      row /= 4;
      Func_02002968(which * 3 + *(s16 *)gate, *(s16 *)state + row, 3, 1,
                    *(s16 *)(gate + 8), *(s16 *)(gate + 10) + row);
      Func_02002958(which * 3 + *(s16 *)(gate + 4), *(s16 *)(gate + 6) + row,
                    3, 1, *(s16 *)(gate + 12), *(s16 *)(gate + 14) + row);
      (*(u16 *)(state + 20 + (1 - slot) * 2))++;
    }
  }
}
