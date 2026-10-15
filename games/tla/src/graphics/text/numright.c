// numright.c: numbers drawn right-aligned, for the HP and PP figures of the
// international editions' status panels.

#include "edition.h"
#if EDITION_INTERNATIONAL
#include "types.h"
#include "graphics.h"
#include "text_font.h"

// Where the digit count gives up. A 32-bit number has at most ten digits, so
// the count always ends first; the limit only guards the loop.
#define DIGIT_COUNT_LIMIT 16

// Draws number in window so that it ends at pixel column x, giving it one
// 8-pixel text cell per digit and at least two cells.
//
// The international editions draw a character's HP and PP with it
// (Ui_DrawValuePairRows). The Japanese editions draw those figures in
// four-character fields padded with spaces, which keeps the digits in line
// because the Japanese font's space is as wide as a digit (7 pixels). The
// Latin font's space is 5 pixels, so here a figure is placed by its digit
// count instead.
//
// number: the value to draw, 0 or more
// window: the text window to draw in
// x:      the pixel column the number's last cell ends at, within the window
// y:      the pixel row of the text, within the window
void UiText_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y) {
  s32 rest = number;
  s32 digits;

  // Count the digits: the units digit, one more for each division by ten
  // that leaves two digits or more, then the leading digit. Below 10 the
  // units digit is the leading digit and is counted twice, so those numbers
  // take two cells: a space, then the digit.
  for (digits = 1; digits < DIGIT_COUNT_LIMIT; digits++) {
    rest /= 10;
    if (rest < 10) {
      break;
    }
  }
  digits++; // the leading digit

  // Step x back from the number's right edge to its left edge; the number
  // writer right-aligns the value in a field of that many characters. x
  // changes in place: a separate left-edge local changes the registers.
  x -= digits * TEXT_CELL_WIDTH;
  UiText_DrawNumberAtOffsetFar(number, digits, window, x, y);
}

#endif
