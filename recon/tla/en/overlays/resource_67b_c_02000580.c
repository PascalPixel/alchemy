// Matches: differ 0 (tla-en, 2026-11-08)
// Plain C through one gate pointer: no slots pointer, no copy, no device. The
// native's second register (gate + 2 bytes, r8), its sp+12 copy of it in the
// inner loop and the separate ldrh for the row's low bit all come from loop
// optimization of these plain reads, which the earlier two-pointer forms
// (slots beside gate, s = slots or gate + 2) were reaching for by hand; they
// stopped at differ 12. The calls go through Call6 (call.h); written as direct
// calls the shared constants take it to 277.
// Aqua Rock's five water gates (Data_02003450, 32 bytes each): each follows
// its flag (the low 12 bits of unk12, inverted by bit 12); a change queues the
// new state in one of its two animation slots (unk16 and unk1A, the next slot
// in unk1E), and each running slot copies the gate's next row of cells, open
// or closed by turns, into the map and its attributes (Func_02002968,
// Func_02002958) until the gate's height (unk10) is drawn.
#include "types.h"
#include "call.h"

struct Unk02003450 {
  s16 unk00;
  s16 unk02;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0A;
  s16 unk0C;
  s16 unk0E;
  s16 unk10;
  s16 unk12;
  s16 unk14;
  s16 unk16[2];
  s16 unk1A[2];
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
  s32 n, i, flag;

  gate = Data_02003450;
  for (n = 0; n <= 4; n++, gate++) {
    flag = GameFlag_Test(gate->unk12 & 0xfff);
    if (gate->unk12 & 0x1000)
      flag ^= 1;
    if (gate->unk14 != flag) {
      gate->unk16[gate->unk1E] = 0;
      gate->unk1A[gate->unk1E] = flag;
      if (++gate->unk1E > 1)
        gate->unk1E = 0;
      gate->unk14 = flag;
    }
    for (i = 0; i < 2; i++) {
      s32 row = gate->unk16[i];
      if (row < gate->unk10 * 4) {
        s32 closed;
        row /= 4;
        flag = gate->unk1A[i];
        closed = flag ^ 1;
        if (gate->unk16[i] & 1) {
          Call6((void (*)())Func_02002968, flag * 3 + gate->unk00,
                gate->unk02 + row, 3, 1, gate->unk08, gate->unk0A + row);
          Call6((void (*)())Func_02002958, flag * 3 + gate->unk04,
                gate->unk06 + row, 3, 1, gate->unk0C, gate->unk0E + row);
        } else {
          Call6((void (*)())Func_02002968, closed * 3 + gate->unk00,
                gate->unk02 + row, 3, 1, gate->unk08, gate->unk0A + row);
          Call6((void (*)())Func_02002958, closed * 3 + gate->unk04,
                gate->unk06 + row, 3, 1, gate->unk0C, gate->unk0E + row);
        }
        gate->unk16[i]++;
      }
    }
  }
}
