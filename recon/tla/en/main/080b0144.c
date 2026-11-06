// Near miss, differ 11 of 156 (tla-en, 2026-11-06): the native's `b` past the
// series 14 test after series = 13 (an else). Ported from The Broken Seal's
// function of the same name (games/tbs/src/game/game/classes.c): the class
// series comes from a character's two strongest elements, and the class is
// the last of that series whose element requirements it meets. The Lost Age
// has 244 classes, takes the series from a 4 by 4 table, gives characters 5
// and 7 (Jenna and Piers) their own series, and lets three items (443 to 445)
// choose one.
//
// Indexing gClass_DefinitionTable itself (declared here as the record view)
// gives the native's two induction variables (150 to 77), i declared before
// j its loop registers (77 to 49), and the second element held in its own
// variable rather than in j puts the primary in r0, the second in r1 and the
// item in r8 (49 to 22). Left: with `else if (series == 6 ...)`, as the native's
// branch shows, the character (5 refs over 45 insns) and the levels' address
// (6 over 54) tie in global.c and the character takes r5 where the native
// has the levels (differ 22); the plain `if` kept here lengthens the
// character's life by one insn and gives the native's registers, but drops
// that branch. The prototype follows games/tla/include/game/game.h.
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

s32 Class_Determine(s32 character, u32 *djinn, s32 item) {
  s32 levels[4];
  s32 primary;
  s32 best;
  s32 series;
  s32 result;
  s32 i;
  s32 second;
  s32 j;

  result = -1;
  if (character > 7)
    return 0;
  Unit_GetElementLevels(character, (const u8 *)djinn, levels);
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
  if (series == 6 && character == 7)
    series = 14;
  if (item == 444)
    series = 16;
  else if (item == 443)
    series = 15;
  else if (item == 445)
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
