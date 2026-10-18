// Draft, differ 2 of 60: scores four slot reels: 5 when all four match,
// otherwise by the number of matching pairs: 2 for one, 3 for two, 4 for
// three, 0 for none. The matched values are collected in a local array the
// score never reads (the native's frame and stores keep it). Left:
// instruction order only: the native stores the matched value (stmia) before
// building the pointer step 4, the draft after.
#include "types.h"

s32 Func_081b834c(s32 *reels) {
  s32 matches[2];
  s32 score;
  s32 pairs;
  s32 i;
  s32 j;

  score = 0;
  if (reels[0] == reels[1] && reels[0] == reels[2] && reels[0] == reels[3]) {
    score = 5;
  } else {
    pairs = 0;
    for (i = 0; i != 3; i++) {
      for (j = i + 1; j != 4; j++) {
        if (reels[i] == reels[j]) {
          matches[pairs] = reels[i];
          pairs++;
        }
      }
    }
    if (pairs == 1)
      score = 2;
    if (pairs == 2)
      score = 3;
    if (pairs == 3)
      score = 4;
  }
  return score;
}
