// Differ 12 of 209 (tla-en, 2026-11-08, -mtune=gba), the same rows.
// tick = row (not a second (u16) read) takes it from 35 to 12: the row and
// unk0E reads now follow the native order; left are the copy below and the
// native's separate ldrh for tick through it. Permuter, 6 minutes: no gain.
// Before: the native's sp+12 is a plain
// copy of the slots pointer, mov r0, r8; str r0, [sp, #12], set at the inner top;
// the slot's row and unk0E are read through it (r0 + r9) and the increment reloads
// it, while unk18[i] and unk00[0] are read through r8. Every other row follows
// from the rotation that sets. Here it is s = gate + 2 bytes, the same value and
// slot, which takes the frame of 20 and leaves mov r0, sl; adds r0, #2.
// (5 November: 248 under the old form.)
// Written s = slots, gcse's cprop folds it (hash_scan_set records the copy unless
// its source or destination is set later in its block, and neither is).
// Tried for the copy, no gain: the operand-tied empty asm, which ranks s above the
// count (73 to 80); slots and s from Data_02003450[n], never reduced in the inner
// loop (73); slots computed at the outer top, whose givs take gate's reads (240).
// Tried 6 November, no gain: the row and unk0E read through s = gate + 2 (57: the
// copy stays sl + 2, kept in r9, not spilled); through s = slots (95: copied and
// folded); slots as a gate + 2 expression throughout, no slots variable (95:
// classic gcse turns the inner gate + 2 into a copy of the outer one and the second
// cprop folds it, so the inner loop reduces s->unk14[i] to a pointer); the outer
// reads written as gate->unk00[k] (82: no giv combines the inner gate + 2 with them).
// Settled from 248: one local for flag and the slot's state (it is why the flag
// sits in r4), the columns read in each arm so gcse hoists the ldrh and not the
// shifts, and n++ before the pointers.
// Aqua Rock's five water gates (Data_02003450, 32 bytes each): each follows
// its flag (the low 12 bits of unk00[9], inverted by bit 12); a change
// queues the new state in one of its two animation slots, and each running
// slot copies the gate's next row of cells, open or closed by turns, into
// the map and its attributes (Func_02002968, Func_02002958) until the
// gate's height is drawn.
#include "types.h"
#include "call.h"

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
  n = 0;
  slots = (struct Unk02003452 *)&gate->unk00[1];
  for (; n <= 4; n++, slots++, gate++) {
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
      struct Unk02003452 *s = (struct Unk02003452 *)&gate->unk00[1];
      s32 row, tick;
      row = slots->unk14[i];
      tick = row;
      if (row < slots->unk0E * 4) {
        s32 a, b, c, d;
        s32 closed;
        row /= 4;
        flag = slots->unk18[i];
        closed = flag ^ 1;
        if (tick & 1) {
          a = (u16)gate->unk00[0];
          b = (u16)slots->unk00[0];
          c = (u16)gate->unk00[4];
          d = (u16)gate->unk00[5];
          Call6((void (*)())Func_02002968, flag * 3 + (s16)a, (s16)b + row, 3, 1,
                        (s16)c, (s16)d + row);
          Call6((void (*)())Func_02002958, flag * 3 + gate->unk00[2], gate->unk00[3] + row, 3, 1,
                        gate->unk00[6], gate->unk00[7] + row);
        } else {
          a = (u16)gate->unk00[0];
          b = (u16)slots->unk00[0];
          c = (u16)gate->unk00[4];
          d = (u16)gate->unk00[5];
          Call6((void (*)())Func_02002968, closed * 3 + (s16)a, (s16)b + row, 3,
                        1, (s16)c, (s16)d + row);
          Call6((void (*)())Func_02002958, closed * 3 + gate->unk00[2], gate->unk00[3] + row, 3,
                        1, gate->unk00[6], gate->unk00[7] + row);
        }
        s->unk14[i]++;
      }
    }


  }
}
