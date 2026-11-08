#include "types.h"
#include "edition.h"
#include "text_font.h"

// The Japanese name entry screen enters kana, whose voicing marks (dakuten
// and handakuten) are characters of their own that join the kana before them.
// The other editions have no marks: in the first function's place they keep
// one that accepts every letter, and they drop the second.
#if LANGUAGE == LANGUAGE_JAPANESE
extern const u8 gNameEntry_VoicedMarkRanges[];
extern const u8 gNameEntry_SemivoicedMarkRanges[];

/// Returns whether `letter` may follow `previous` in a name: any character but
/// a voicing mark may, and a voicing mark only after a kana in its table of
/// ranges (pairs of first and last kana, ended by 0).
s32 Func_08044488(s32 letter, s32 previous) {
  const u8 *range;

  if (letter < TEXT_MARK_VOICED || letter > TEXT_MARK_SEMIVOICED)
    return 1;
  if (letter == TEXT_MARK_VOICED)
    range = gNameEntry_VoicedMarkRanges;
  else
    range = gNameEntry_SemivoicedMarkRanges;
  for (; *range != 0; range += 2) {
    if (previous >= range[0] && previous <= range[1])
      return 1;
  }
  return 0;
}

/// Returns how many letters `name` has, not counting its voicing marks.
s32 Func_080444c4(const u8 *name) {
  s32 count;

  count = 0;
  for (; *name != 0; name++) {
    if ((u8)(*name - TEXT_MARK_VOICED) >
        TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)
      count++;
  }
  return count;
}
#else
/// Unused: the Japanese edition's mark test (Func_08044488 there), which
/// accepts every letter where no letter takes a mark.
UNUSED s32 Window_ReturnOne(void) { return 1; }
#endif
