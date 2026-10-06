#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"

struct UiPanel {
    u8 unknown_00[0xc];
    s16 x;
    s16 y;
};


void DjinnMenu_DrawFirstPage(struct UiPanel *panel)
{
    DjinnMenu_DrawPage(panel, 0);
}

void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width, s32 height, s32 style)
{
    Ui_SetRectHighlightFar(panel->x + x + 1, panel->y + y + 1, width, height, style);
}
