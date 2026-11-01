// Draft, differ 264 of 209, skeleton gap 76 (tla-en, 2026-11-01)
// The native's ldrh ahead of the odd-frame test and the shifts in both arms
// are s32 locals read through (u16) and used through (s16): tick (its ldrsh
// and ldrh pair) and a, b, c, d, which took the skeleton from 125 to 76 and
// the draft to 209 instructions. dbac is the best load order of the 48 tried.
// What is left is one missing pseudo, and it sets the frame (16, native 20):
// the native holds a copy of slots in sp+12, set each pass at the inner top
// (mov r0, r8; str r0, [sp, #12]) and read only by the tick increment. Here
// GCSE's copy propagation replaces that copy (insns 410, 415 in .07.gcse) with
// slots, however it is spelled, so the reload rotation differs throughout, and
// flag lands in r0 where the native has r4. gcse.c's hash_scan_set drops a copy
// whose source or destination is set later in its block, which may be the way
// to look. The slot pointer's spill order puts it after n and before the
// loop's offset registers.
// Tried without gain: `s` set at both tops (same code), slots = s in an arm or
// before the increment (76 to 88), the slot step as a static
// inline (identical), Data_02003450[n] indexing (141), only gate stepped with
// outer and inner pointers, or a macro recomputing gate + 2 at each use (93 to
// 106: loop.c reduces it, but folds the gate reads onto that pointer, where the
// native keeps them on r10), flag shared with the slot's queued state (78).
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
