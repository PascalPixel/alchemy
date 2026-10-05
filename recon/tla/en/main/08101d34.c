/* Matches (differ 0): UiPanel_SetRectHighlight's twin for the palette
   rectangle. Waits on Ui_SetRectPalette (08101c7c, a draft at 6) and
   UiPanel_HighlightGridCell (08101c40, a draft at 3), which sit between it
   and PANEL_HIGHLIGHT.C. */
#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};

void Ui_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);

/* Recolours a rectangle given inside the panel's frame. */
void UiPanel_SetRectPalette(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    Ui_SetRectPalette(panel->x + x + 1, panel->y + y + 1, width, height, palette);
}
