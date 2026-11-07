/// @file
/// Chooses a character's class.

#include "types.h"

// A class record's start, as Class_Determine reads it.
struct ClassSeriesView {
  s32 series;
  u8 required[4];
  u8 unk08[0x4c];
};

extern struct ClassSeriesView gClass_DefinitionTable[];

extern const s32 Data_080c6604[4][4];

s32 Unit_GetElementLevels(s32 record, const u8 *source, s32 output[4]);

/// Chooses a character's class: the series comes from its two strongest
/// elements (or the item that picks one), and the class is the last of that
/// series whose element requirements it meets.
s32 Class_Determine(s32 character, u32 *djinn, s32 item) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 i;
  s32 second;
  s32 j;
  s32 lastSeriesItem = 445; // One constant for its whole life.

  result = -1;
  if (character > 7)
    return 0;
  /* FAKEMATCH(signature): called as a void function, though it returns 0;
   * as a plain call, 2 of 156 instructions differ in tla-en. */
  ((void (*)(s32, const u8 *, s32 *))Unit_GetElementLevels)(
      character, (const u8 *)djinn, levels);
  best = result;
  primary = result;
  for (i = 0; i < 4; i++) {
    if (best < levels[i]) {
      best = levels[i];
      primary = i;
    }
  }
  second = -1;
  best = -1;
  for (i = 0; i < 4; i++) {
    if (i != primary && best < levels[i]) {
      best = levels[i];
      second = i;
    }
  }
  if (levels[second] <= 9)
    second = primary;
  series = Data_080c6604[primary][second];
  if (series == 2 && character == 5)
    series = 13;
  else if (series == 6 && character == 7)
    series = 14;
  if (item == 444)
    series = 16;
  else if (item == 443)
    series = 15;
  else if (item == lastSeriesItem)
    series = 17;
  for (i = 243; i >= 0; i--) {
    if (gClass_DefinitionTable[i].series != series)
      continue;
    for (j = 0; j < 4; j++) {
      if (levels[j] < gClass_DefinitionTable[i].required[j] * 10)
        break;
    }
    if (j == 4) {
      result = i;
      break;
    }
  }
  if (result == -1)
    result = 0;
  return result;
}
