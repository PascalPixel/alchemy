// A character's HP and PP, drawn as two rows of current and maximum figures.

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
