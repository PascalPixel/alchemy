#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};

void Ui_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);

void UiPanel_SetRectPalette(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    Ui_SetRectPalette(panel->x + x + 1, panel->y + y + 1, width, height, palette);
}
