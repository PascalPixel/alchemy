// Draft, differ 20 of 106: writes a frame count as "HHH:MM:SS:CC" (hours,
// minutes, seconds, hundredths at 60 frames a second) into out, each field
// formatted by UiText_FormatNumber (two-digit fields offset by 100 to keep
// their leading zero). Left: instruction order only: the native schedules
// each field's +100 and its width argument earlier, between the previous
// field's byte copies. Tried: the divisions in other orders (differ 25, 36).
#include "types.h"

u8 *UiText_FormatNumber(u8 *buffer, s32 value, s32 digits);

u8 *Func_080430c4(u32 frames, u8 *out) {
  u8 buffer[16];
  u8 *digits;
  u8 *p;
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

  digits = UiText_FormatNumber(buffer, hours, 3);
  p = out;
  *p = *digits++;
  *++p = *digits;
  *++p = digits[1];
  *++p = ':';
  digits = UiText_FormatNumber(buffer, minutes + 100, 2);
  *++p = digits[0];
  *++p = digits[1];
  *++p = ':';
  digits = UiText_FormatNumber(buffer, seconds + 100, 2);
  *++p = digits[0];
  *++p = digits[1];
  *++p = ':';
  digits = UiText_FormatNumber(buffer, hundredths + 100, 2);
  *++p = digits[0];
  *++p = digits[1];
  p[1] = 0;
  return out;
}
