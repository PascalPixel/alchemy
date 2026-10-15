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

struct UiPanel
{
    u8  unk00[0xc];
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
