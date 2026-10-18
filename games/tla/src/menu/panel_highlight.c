#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"

struct UiPanel {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

void DjinnMenu_DrawFirstPage(struct UiPanel *panel) {
  DjinnMenu_DrawPage(panel, 0);
}

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width,
                              s32 height, s32 style) {
  Ui_SetRectHighlightFar(panel->x + x + 1, panel->y + y + 1, width, height,
                         style);
}

// Highlights the cell of a four-column grid at index's column and row's line,
// with style 14 when selected and 15 otherwise. Japanese cells are a tile
// narrower.
void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 row,
                               s32 selected) {
  s32 style = ((0u - selected) | selected) >> 31;

  style = 15 - style;
#if LANGUAGE == LANGUAGE_JAPANESE
  UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, 5, 1, style);
#else
  UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, 6, 1, style);
#endif
}
