// A character's HP and PP, drawn as two rows of current and maximum figures,
// and the right-aligned number writer the international editions draw them
// with.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "battle_unit.h"

#if EDITION_INTERNATIONAL
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

// Draws a character's HP and PP into window as two rows, "HP current/maximum"
// at pixel row 40 and "PP current/maximum" at row 48. Current HP is drawn in
// colour 4 when it is below a quarter of maximum HP and in colour 2 at 0;
// then the colour goes back to 15, the default text colour.
//
// unit:   the character whose figures are drawn
// window: the window to draw in, its struct RenderInput pointer held as an
//         s32
void Ui_DrawValuePairRows(struct BattleUnit *unit, s32 window) {
  // Each figure in turn, as an s16. Read straight from unit, the figures
  // compile differently.
  s16 value;

#if EDITION_INTERNATIONAL
  // The label at the left and a slash at 48. A current figure ends before
  // the slash, at 48, and a maximum at 88, each in a field as wide as its
  // digits.
  UiText_DrawStringAtOffsetFar((s32)gUi_HpString, window, 0, 40);
  UiText_DrawStringInWindowFar((u8 *)gUi_SlashString, window, 48, 40);
  value = unit->maxHp;
  UiText_DrawNumberRightAligned(value, window, 88, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    UiWork_SetParamNibbleFar(4);
  }
  if (value == 0) {
    UiWork_SetParamNibbleFar(2);
  }
  UiText_DrawNumberRightAligned(value, window, 48, 40);
  UiWork_SetParamNibbleFar(15);
  UiText_DrawStringAtOffsetFar((s32)gUi_PpString, window, 0, 48);
  UiText_DrawStringInWindowFar((u8 *)gUi_SlashString, window, 48, 48);
  value = unit->pp;
  UiText_DrawNumberRightAligned(value, window, 48, 48);
  value = unit->maxPp;
  UiText_DrawNumberRightAligned(value, window, 88, 48);
#else
  // Each row string ("HP    /" and "EP    /") holds the label and the slash,
  // one character to an 8-pixel cell, the slash in the cell at 48. Each
  // figure is four characters padded with spaces: the current one drawn
  // from 16 at the font's widths, the maximum from 56 a cell a character,
  // ending at 88.
  UiText_DrawStringInWindowFar((u8 *)gUi_HpRowString, window, 0, 40);
  value = unit->maxHp;
  UiText_DrawNumberInWindowFar(value, 4, window, 56, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    UiWork_SetParamNibbleFar(4);
  }
  if (value == 0) {
    UiWork_SetParamNibbleFar(2);
  }
  UiText_DrawNumberAtOffsetFar(value, 4, window, 16, 40);
  UiWork_SetParamNibbleFar(15);
  UiText_DrawStringInWindowFar((u8 *)gUi_PpRowString, window, 0, 48);
  value = unit->pp;
  UiText_DrawNumberAtOffsetFar(value, 4, window, 16, 48);
  value = unit->maxPp;
  UiText_DrawNumberInWindowFar(value, 4, window, 56, 48);
#endif
}
