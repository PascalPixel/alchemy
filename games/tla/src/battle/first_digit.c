#include "types.h"
#include "edition.h"

// The scan looks for the first of FIRST_DIGIT_COUNT character codes from
// FIRST_DIGIT_BASE: the digits 1 to 9 outside Japan, and the codes 65 to 90 in
// the Japanese release.
#if LANGUAGE == LANGUAGE_JAPANESE
#define FIRST_DIGIT_BASE 65
#define FIRST_DIGIT_COUNT 26
#else
#define FIRST_DIGIT_BASE 49
#define FIRST_DIGIT_COUNT 9
#endif

s32 Func_08128228(u8 *s) {
  s32 i;

  for (i = 0; i <= 13; i++) {
    if ((u8)(s[i] - FIRST_DIGIT_BASE) <= FIRST_DIGIT_COUNT - 1)
      return s[i] - FIRST_DIGIT_BASE;
  }
  return FIRST_DIGIT_COUNT;
}
