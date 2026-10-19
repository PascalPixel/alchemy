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

// A grid cell's highlight is a tile narrower in Japanese.
#if LANGUAGE == LANGUAGE_JAPANESE
#define GRID_CELL_WIDTH 5
#else
#define GRID_CELL_WIDTH 6
#endif

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

/// Highlights the cell of a four-column grid at index's column and row's line,
/// with style 14 when selected and 15 otherwise.
void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 row,
                               s32 selected) {
  s32 style = ((0u - selected) | selected) >> 31;

  style = 15 - style;
  UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, GRID_CELL_WIDTH,
                           1, style);
}
