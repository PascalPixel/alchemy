// Near miss, differ 263 of 209 (tla-en, 2026-10-31): what remains is GCSE
// code hoisting, which alchemy-gcc runs only under -Os. The native loads the
// four halfwords both arms' first copy reads (unk00[0], [1], [4], [5]) with
// ldrh ahead of the odd-frame test and sign-extends each by shifts inside
// both arms, and reads the slot's frame a second time as ldrh at the slot
// loop's top beside the row's ldrsh. This C compiled with -Os in place of
// -O2 (analysis only) does exactly that; at -O2 each arm loads its own
// ldrsh and the allocation follows. Also left: the native steps two
// pointers (the gate in r10, the gate + 2 in r8, n at sp+16) and reaches
// the slots through a per-pass copy of r8 spilled to sp+12 plus reduced
// offsets (20 + 2i in r9, 24 + 2i at sp+8); tried without reaching it: one
// stepped pointer (267), a second pointer to unk00[1] stepped or set per
// pass (281, 288), one pointer at unk00[1] (283), indexing by n (337), the
// hoisted halfwords as s16 or u16 locals (262 to 281; the locals load
// sign-extended, as PROMOTE_MODE holds every halfword local signed).
// Aqua Rock's five water gates (Data_02003450, 32 bytes each): each follows
// its flag (the low 12 bits of unk00[9], inverted by bit 12); a change
// queues the new state in one of its two animation slots, and each running
// slot copies the gate's next row of cells, open or closed by turns, into
// the map and its attributes (Func_02002968, Func_02002958) until the
// gate's height is drawn.
#include "types.h"

struct Unk02003450 {
  s16 unk00[16];
};
extern struct Unk02003450 Data_02003450[];
s32 GameFlag_Test(s32 flag);
void Func_02002968(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002958(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02000580(void) {
  struct Unk02003450 *gate;
  s32 n, i, flag, row, open, closed;

  for (n = 0; n <= 4; n++) {
    gate = &Data_02003450[n];
    flag = GameFlag_Test(gate->unk00[9] & 0xfff);
    if (gate->unk00[9] & 0x1000)
      flag ^= 1;
    if (gate->unk00[10] != flag) {
      gate->unk00[11 + gate->unk00[15]] = 0;
      gate->unk00[13 + gate->unk00[15]] = flag;
      if (++gate->unk00[15] > 1)
        gate->unk00[15] = 0;
      gate->unk00[10] = flag;
    }
    for (i = 0; i < 2; i++) {
      row = gate->unk00[11 + i];
      if (row < gate->unk00[8] * 4) {
        row /= 4;
        open = gate->unk00[13 + i];
        closed = open ^ 1;
        if (gate->unk00[11 + i] & 1) {
          Func_02002968(open * 3 + gate->unk00[0], gate->unk00[1] + row, 3, 1,
                        gate->unk00[4], gate->unk00[5] + row);
          Func_02002958(open * 3 + gate->unk00[2], gate->unk00[3] + row, 3, 1,
                        gate->unk00[6], gate->unk00[7] + row);
        } else {
          Func_02002968(closed * 3 + gate->unk00[0], gate->unk00[1] + row, 3,
                        1, gate->unk00[4], gate->unk00[5] + row);
          Func_02002958(closed * 3 + gate->unk00[2], gate->unk00[3] + row, 3,
                        1, gate->unk00[6], gate->unk00[7] + row);
        }
        gate->unk00[11 + i]++;
      }
    }
  }
}
