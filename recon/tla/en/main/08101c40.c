/* Near miss, differ 3: highlights grid cell index of four across, dimmed
   when selected is set. The native computes the style straight into r4;
   this draft goes through r2 and a copy. A u32 style, a separate x, an
   unsigned flag, !! and a pointer copy did not move it. Joins
   PANEL_HIGHLIGHT.C on a match. */
#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};


void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 style);

void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 y, s32 selected)
{
    s32 style = 15 - (selected != 0);

    UiPanel_SetRectHighlight(panel, (index % 4) * 7 + 1, y + 2, 6, 1, style);
}
