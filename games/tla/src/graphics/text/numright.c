// Numbers drawn right-aligned, in a field as wide as their digits, for the HP
// and PP figures of the international editions' status panels.

#include "edition.h"
#if EDITION_INTERNATIONAL
#include "types.h"
#include "graphics.h"

// The pixels the field allows each digit. The Latin font's digits are 7
// pixels wide, so the digits themselves take a pixel less each.
#define FIELD_PIXELS_PER_DIGIT 8

// Returns how many decimal digits number has, but at least 2: 0-99 give 2,
// 100-999 give 3, 1000-9999 give 4. The count starts with the units digit,
// adds one for each division by ten that leaves 10 or more, then adds the
// leading digit; a number under 10 has its one digit counted as both, which
// is where the minimum of 2 comes from.
static inline s32 CountDigitsAtLeastTwo(s32 number) {
  s32 count;

  // 16 only bounds the loop: an s32 has at most 10 digits, so the count
  // always stops first.
  for (count = 1; count < 16; count++) {
    number /= 10;
    if (number < 10) {
      break;
    }
  }
  count++; // the leading digit
  return count;
}

// Draws number right-aligned in a field that ends at pixel column x and
// allows 8 pixels for each digit, at least two digits wide.
//
// The international editions draw a character's HP and PP with it
// (Ui_DrawValuePairRows). The Japanese editions draw those figures in
// four-character fields padded with spaces, which keeps the digits in line
// because the Japanese font's space is as wide as a digit (7 pixels). The
// Latin font's space is 5 pixels, so here each figure gets a field as wide
// as its digits instead.
//
// The number writer draws the digits 7 pixels apart, so a number ends about
// a pixel per digit before x: 99 ends 2 pixels before it, 100 ends 3. A
// number under 10 draws as a 5-pixel space and its digit, and ends 4 pixels
// before x.
//
// number: the value to draw, 0 or more
// window: the window to draw in, its struct RenderInput pointer held as an
//         s32, as its callers hold it
// x:      the field's right edge, in pixels within the window
// y:      the pixel row of the text, within the window
void UiText_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters. x changes in place: a separate
  // left-edge local changes the registers.
  x -= width * FIELD_PIXELS_PER_DIGIT;
  UiText_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif
