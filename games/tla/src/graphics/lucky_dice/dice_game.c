/// @file
/// The casino's dice game (Func_081b83c4, not C yet). Its one table, after
/// its code, is gLuckyDice_FaceTiles again: the die face tile at each step
/// of its roll.

#include "types.h"

/// Scores four dice for the dice game: 5 when all four match, otherwise by
/// the number of matching pairs: 2 for one, 3 for two, 4 for three, 0 for
/// none. The matched values are collected in a local array the score never
/// reads.
// The native's frame and stores keep that array.
s32 Func_081b834c(s32 *dice) {
  s32 matches[2];
  s32 score;
  s32 pairs;
  s32 i;
  s32 j;

  score = 0;
  if (dice[0] == dice[1] && dice[0] == dice[2] && dice[0] == dice[3]) {
    score = 5;
  } else {
    pairs = 0;
    for (i = 0; i != 3; i++) {
      for (j = i + 1; j != 4; j++) {
        if (dice[i] == dice[j]) {
          matches[pairs] = dice[i];
          /* FAKEMATCH(barrier): the store issued before the pointer step's
           * constant; as plain C, 2 of 60 instructions differ in tla-en. */
          asm("");
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

const u8 gData_081ba30c[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                             3, 9,  13, 4, 7,  6, 5, 17, 11};
