/// @file
/// Clock texts: a frame count written as hours, minutes, seconds and
/// hundredths, and the save files' play time.
///
/// Each field comes from UiText_FormatNumber; a two-digit field is formatted
/// with 100 added, which gives it a leading zero in its last two digits.

#include "types.h"

u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width);

// XXX: what shows this clock; the overlays that call Func_08038400 would
// settle it.

/// Writes frames (60 a second) as "HHH:MM:SS:CC" into out and returns out.
/// Reached only through the far call Func_08038400.
u8 *Func_080430c4(u32 frames, u8 *out) {
  u8 digits[16];
  u8 *digitsCursor;
  u8 *textCursor;
  u32 fraction;
  u32 seconds;
  u32 minutes;
  u32 hours;
  u32 hundredths;

  fraction = frames % 60;
  frames /= 60;
  hundredths = fraction * 100 / 60;
  seconds = frames % 60;
  frames /= 60;
  minutes = frames % 60;
  hours = frames / 60;

  digitsCursor = UiText_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  /* FAKEMATCH(barrier): the hours' first store comes before the cursor's
   * step; as plain statements, the six barriers of this function make 20 of
   * 106 instructions differ in tla-en. */
  __asm__ volatile("");
  textCursor = out + 1;
  /* FAKEMATCH(barrier): the load waits for the cursor's step. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = *digitsCursor;
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, minutes + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, seconds + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, hundredths + 100, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}

/// Writes a save file's play time (in frames) as "HHH:MM" into out and
/// returns out; the clock stops at 999:59.
void *Text_FormatPlayTime(s32 frames, u8 *out) {
  u8 digits[16];
  u32 totalMinutes;
  u32 hours;
  s32 minutes;
  u8 *digitsCursor;
  u8 *textCursor;

  totalMinutes = (u32)frames / (60 * 60);
  if (totalMinutes > 999 * 60 + 59)
    totalMinutes = 999 * 60 + 59;

  hours = totalMinutes / 60;
  minutes = totalMinutes % 60;

  digitsCursor = UiText_FormatNumber(digits, hours, 3);
  *out = *digitsCursor;
  digitsCursor++;
  /* FAKEMATCH(barrier): the hours' first store comes before the cursor's
   * step; as plain statements, the four barriers of this function make 12 of
   * 58 instructions differ in tla-en. */
  __asm__("" : "+l"(out) : "m"(*out));
  textCursor = out + 1;
  /* FAKEMATCH(barrier): the load waits for the cursor's step. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = *digitsCursor;
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  minutes += 100;
  textCursor++;
  *textCursor = ':';

  digitsCursor = UiText_FormatNumber(digits, minutes, 2);
  textCursor++;
  *textCursor = digitsCursor[0];
  textCursor++;
  /* FAKEMATCH(barrier): as above. */
  __asm__("" : "+l"(digitsCursor) : "l"(textCursor));
  *textCursor = digitsCursor[1];
  textCursor[1] = 0;

  return out;
}
