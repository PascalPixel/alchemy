// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. Plain C through
// one entry pointer: no slots pointer, no copy. The native's second register
// (entry + 2 bytes, r8), its sp+12 copy of it in the inner loop and the
// separate ldrh for the row's low bit all come from loop optimization of these
// plain reads, which the earlier two-pointer forms were reaching for by hand;
// they stopped at differ 12. One tagged device: the copies go through Call6
// (call.h), a wrapper; as direct calls the shared constants take it to 277.
// Five records of Data_02003450 (32 bytes each) in overlay 67b (Aqua Rock):
// each follows its flag (the low 12 bits of unk12, inverted by bit 12); a
// change queues the new value in one of two slots (unk16 and unk1A, the next
// slot in unk1E), and each running slot copies the record's next row of cells,
// chosen by the queued value or its inverse on alternate steps, into the map
// (Func_02002968) and its attributes (Func_02002958) until unk10 rows are done.
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
  struct Unk02003450 *entry;
  s32 n, i, flag;
  /* FAKEMATCH(wrapper): the four copies forwarded through Call6; as direct
   * calls, 277 of 209 instructions differ in tla-en. */

  entry = Data_02003450;
  for (n = 0; n <= 4; n++, entry++) {
    flag = GameFlag_Test(entry->unk12 & 0xfff);
    if (entry->unk12 & 0x1000)
      flag ^= 1;
    if (entry->unk14 != flag) {
      entry->unk16[entry->unk1E] = 0;
      entry->unk1A[entry->unk1E] = flag;
      if (++entry->unk1E > 1)
        entry->unk1E = 0;
      entry->unk14 = flag;
    }
    for (i = 0; i < 2; i++) {
      s32 row = entry->unk16[i];
      if (row < entry->unk10 * 4) {
        s32 inverse;
        row /= 4;
        flag = entry->unk1A[i];
        inverse = flag ^ 1;
        if (entry->unk16[i] & 1) {
          Call6(Func_02002968, flag * 3 + entry->unk00, entry->unk02 + row, 3,
                1, entry->unk08, entry->unk0A + row);
          Call6(Func_02002958, flag * 3 + entry->unk04, entry->unk06 + row, 3,
                1, entry->unk0C, entry->unk0E + row);
        } else {
          Call6(Func_02002968, inverse * 3 + entry->unk00, entry->unk02 + row,
                3, 1, entry->unk08, entry->unk0A + row);
          Call6(Func_02002958, inverse * 3 + entry->unk04, entry->unk06 + row,
                3, 1, entry->unk0C, entry->unk0E + row);
        }
        entry->unk16[i]++;
      }
    }
  }
}
