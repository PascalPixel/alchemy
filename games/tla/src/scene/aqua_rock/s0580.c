#include "types.h"
#include "event_inline.h"

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
          MapGrid_CopyCellsLayered(flag * 3 + entry->unk00, entry->unk02 + row,
                                   3, 1, entry->unk08, entry->unk0A + row);
          MapGrid_CopyCells(flag * 3 + entry->unk04, entry->unk06 + row, 3, 1,
                            entry->unk0C, entry->unk0E + row);
        } else {
          MapGrid_CopyCellsLayered(inverse * 3 + entry->unk00,
                                   entry->unk02 + row, 3, 1, entry->unk08,
                                   entry->unk0A + row);
          MapGrid_CopyCells(inverse * 3 + entry->unk04, entry->unk06 + row, 3,
                            1, entry->unk0C, entry->unk0E + row);
        }
        entry->unk16[i]++;
      }
    }
  }
}
