// Measured under -Os, 5 November 2026: differ 290 (tla-en); notes below may predate it.
// Draft, differ 262 of 209, skeleton gap 66 (tla-en, 2026-11-01)
// The native's ldrh ahead of the odd-frame test and the shifts in both arms
// are s32 locals read through (u16) and used through (s16): tick (its ldrsh
// and ldrh pair) and a, b, c, d, which took the skeleton from 125 to 66 (33
// removed, 33 added) and the draft to 209 instructions. dbac is the best load
// order of the 48 tried.
// What is left is one missing pseudo, and it sets the frame (16, native 20):
// the native holds a copy of slots in sp+12, set each pass at the inner top
// (mov r0, r8; str r0, [sp, #12]) and read only by the tick increment, whose
// block is the first one past cse1's reach. Here that last use is folded back
// onto slots, so the reload rotation differs throughout, and flag lands in r0
// where the native has r4.
// Who folds it, read from the dumps: a copy goes in gcse's first cprop; a
// `s = &gate->unk00[1]` at the inner top is not a copy, but PRE turns it into
// `s = reg` (the header's old `.07.gcse` insns) and the last cprop, gcse.c's
// one_cprop_pass (pass + 1, 1), then folds that. hash_scan_set drops a copy
// only when its source or destination is set later in its block, and only a
// second value in s would do that, which is a reused local. Nothing else in
// the inner loop kills the copy, the increment has no path round it, and a
// do-while does not change it: loop.c leaves the copy in place ("savings 1 not
// desirable"), so hoisting is not the problem. Why the native's cprop left this
// one use is open; if no source form does, it is a compiler question, which is
// out of scope here.
// Tried without gain: `s` set at both tops (same code), slots = s in an arm or
// before the increment (66 to 88), the slot step as a static
// inline (identical), Data_02003450[n] indexing (141), only gate stepped with
// outer and inner pointers, or a macro recomputing gate + 2 at each use (93 to
// 119: loop.c reduces it, but folds the gate reads onto that pointer, where the
// native keeps them on r10, because combine_givs takes every address it can
// express; `slots` must be a real second pointer), flag shared with the slot's
// queued state (78), s from gate + 2 beside a stepped slots with for or
// do-while (94, 217 instructions: PRE's insert on the back edge splits it and
// loop.c drops the outer loop for multiple entry points), the increment alone
// through that s (82), slots declared after n (same code).
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
  s32 n, i, flag;

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
      s32 tick = (u16)s->unk14[i];
      s32 row = (s16)tick;
      if (row < s->unk0E * 4) {
        s32 d = (u16)gate->unk00[5], b = (u16)slots->unk00[0], a = (u16)gate->unk00[0], c = (u16)gate->unk00[4];
        s32 open, closed;
        row /= 4;
        open = slots->unk18[i];
        closed = open ^ 1;
        if (tick & 1) {
          Func_02002968(open * 3 + (s16)a, (s16)b + row, 3, 1,
                        (s16)c, (s16)d + row);
          Func_02002958(open * 3 + gate->unk00[2], gate->unk00[3] + row, 3, 1,
                        gate->unk00[6], gate->unk00[7] + row);
        } else {
          Func_02002968(closed * 3 + (s16)a, (s16)b + row, 3,
                        1, (s16)c, (s16)d + row);
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
