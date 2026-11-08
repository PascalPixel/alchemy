#include "types.h"
#include "iwram_call.h"

struct BarPatternRow {
  u32 word0;
  u32 word1;
};

extern const struct BarPatternRow gMenu_BarPatternRows[];

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

s32 Func_080451bc(void) {
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = (u32 *)(set * 0x180 + n * 0x40 + 0x06006280);
      s32 x;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (x = 1; x <= 7; x++) {
        s32 mi = n;

        if (set == 1 && x <= 1) {
          continue;
        }
        if (set == 0 && n > x - 2) {
          mi = x - 2;
          if (mi < 0) {
            mi = 0;
          }
        }
        tile[x] = XorWord(tile[x], gMenu_BarPatternRows[mi].word0);
        tile[x + 8] = XorWord(tile[x + 8], gMenu_BarPatternRows[mi].word1);
      }
    }
  }
}
