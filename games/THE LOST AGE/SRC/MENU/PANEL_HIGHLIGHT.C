#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};

void Ui_SetRectHighlightFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void DjinnMenu_DrawPage(struct UiPanel *panel, s32 first);

void DjinnMenu_DrawFirstPage(struct UiPanel *panel)
{
    DjinnMenu_DrawPage(panel, 0);
}

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 style)
{
    Ui_SetRectHighlightFar(panel->x + x + 1, panel->y + y + 1, width, height, style);
}
