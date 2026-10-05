/* Near miss: differ 3. The listing computes the style's "selected != 0"
   straight into r4 (lsrs r4, r2, #31) and subtracts it from 15 there; this
   build shifts into r2 and copies it to r4 first. The same with !!, a
   ?: choice (18 as a local, a branch inline in the call), a reused
   parameter, the HasTrigger-style (u32)(-s | s) >> 31 (4), and u32, s16
   or u8 parameters (3-4). Goes after UiPanel_SetRectHighlight in
   PANEL_HIGHLIGHT.C. */
#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 style);

/* Highlights cell index of a four-column grid on the given row, in style
   14 when it is selected and 15 otherwise. */
void UiPanel_HighlightGridCell(struct UiPanel *panel, s32 index, s32 row, s32 selected)
{
    s32 style = 15 - (selected != 0);

    UiPanel_SetRectHighlight(panel, index % 4 * 7 + 1, row + 2, 6, 1, style);
}
