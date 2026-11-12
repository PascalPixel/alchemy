/// @file
/// Scene code of overlay 0x67b, in ROM order, from s0580's offset up to the next object the overlay links.

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

extern s32 gData_02003204, gData_02003208, gData_0200320c, gData_02003210;

/// Unless flag 0x104 is set, runs four 240-frame timers (gData_02003204 to
/// gData_02003210) that hold flags 0x200 to 0x203 set for their last 90
/// frames; in their first 150 the first two follow flags 0x320 and 0x321 and
/// the last two are clear.
void Func_02000740(void) {
  if (GameFlag_Test(0x104))
    return;
  if (gData_02003204 <= 149) {
    if (GameFlag_Test(0x320))
      GameFlag_SetBit(0x200);
    else
      GameFlag_ClearBit(0x200);
  } else {
    GameFlag_SetBit(0x200);
  }
  if (++gData_02003204 > 239)
    gData_02003204 = 0;
  if (gData_02003208 <= 149) {
    if (GameFlag_Test(0x321))
      GameFlag_SetBit(0x201);
    else
      GameFlag_ClearBit(0x201);
  } else {
    GameFlag_SetBit(0x201);
  }
  if (++gData_02003208 > 239)
    gData_02003208 = 0;
  if (gData_0200320c <= 149)
    GameFlag_ClearBit(0x202);
  else
    GameFlag_SetBit(0x202);
  if (++gData_0200320c > 239)
    gData_0200320c = 0;
  if (gData_02003210 <= 149)
    GameFlag_ClearBit(0x203);
  else
    GameFlag_SetBit(0x203);
  if (++gData_02003210 > 239)
    gData_02003210 = 0;
}
