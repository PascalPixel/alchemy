#include "types.h"

struct UiPanel {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width,
                              s32 height, s32 style);

void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 row,
                               s32 selected) {
  s32 style = 15 - (selected != 0);

  UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, 6, 1, style);
}
