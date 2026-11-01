// Draft, differ 269 of 209, skeleton gap 125 (tla-en, 2026-11-01)
// The shape is now the listing's: two pointers stepped by a gate, one at the
// gate (r10) and one at its second halfword (r8), with the slots reached
// through a per-pass copy of the second (r8 to sp+12) and offsets reduced
// from i. Struct Unk02003452 is that second view of the same 32 bytes, so
// its unk1E is the next gate's first halfword and is never read.
// What is left is one gap: the native loads unk00[0], unk00[1], unk00[4] and
// unk00[5] with ldrh ahead of the odd-frame test and sign-extends each by
// shifts inside both arms; at -O2 each arm loads its own ldrsh, and the
// lighter register pressure then loses the high registers, the spills and
// the 20-byte frame, which account for most of the skeleton's movs and strs.
// A read of those four halfwords into unused locals just after the height
// test (analysis only, not kept: that is a `dead` device) makes CSE share
// one HImode register between the arms, and the ldrh and shifts appear:
// skeleton 84 (44 removed, 40 added), differ 272. -Os hoists them too, but
// also loads pool constants with ldrh, which the native never does, so the
// options are not the answer. Tried without gain: the row, height and open
// read through the outer pointer or a per-pass copy, closed computed in the
// arm, the row reread instead of held, the odd test in a local, the locals
// scoped to the slot loop, the steps in the for header.
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
struct Unk02003452 {
  s16 unk00[7];
  s16 unk0E;
  s16 unk10;
  s16 unk12;
  s16 unk14[2];
  s16 unk18[2];
  s16 unk1C;
  s16 unk1E;
};
extern struct Unk02003450 Data_02003450[];
s32 GameFlag_Test(s32 flag);
void Func_02002968(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002958(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02000580(void) {
  struct Unk02003450 *gate;
  struct Unk02003452 *slots;
  s32 n, i, flag, row, open, closed;

  gate = Data_02003450;
  slots = (struct Unk02003452 *)&gate->unk00[1];
  for (n = 0; n <= 4; n++) {
    flag = GameFlag_Test(slots->unk10 & 0xfff);
    if (slots->unk10 & 0x1000)
      flag ^= 1;
    if (slots->unk12 != flag) {
      slots->unk14[slots->unk1C] = 0;
      slots->unk18[slots->unk1C] = flag;
      if (++slots->unk1C > 1)
        slots->unk1C = 0;
      slots->unk12 = flag;
    }
    for (i = 0; i < 2; i++) {
      struct Unk02003452 *s = slots;
      row = s->unk14[i];
      if (row < s->unk0E * 4) {
        row /= 4;
        open = slots->unk18[i];
        closed = open ^ 1;
        if (s->unk14[i] & 1) {
          Func_02002968(open * 3 + gate->unk00[0], slots->unk00[0] + row, 3, 1,
                        gate->unk00[4], gate->unk00[5] + row);
          Func_02002958(open * 3 + gate->unk00[2], gate->unk00[3] + row, 3, 1,
                        gate->unk00[6], gate->unk00[7] + row);
        } else {
          Func_02002968(closed * 3 + gate->unk00[0], slots->unk00[0] + row, 3,
                        1, gate->unk00[4], gate->unk00[5] + row);
          Func_02002958(closed * 3 + gate->unk00[2], gate->unk00[3] + row, 3,
                        1, gate->unk00[6], gate->unk00[7] + row);
        }
        s->unk14[i]++;
      }
    }
    slots++;
    gate++;
  }
}
