#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "system.h"
#include "ui.h"
#include "psynergy_menu.h"
#include "menu_list.h"
#include "btlsys.h"
#include "text/msg_ids.h"

s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

#if EDITION_INTERNATIONAL
#define DETAIL_ROW_X     0
#define DETAIL_ROW_WIDTH 15
#else
#define DETAIL_ROW_X     1
#define DETAIL_ROW_WIDTH 14
#endif

s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    s32 combined;
    s32 row;

    menu = gMenuWork;
    combined = state->page * 5;
    combined += state->row;
    state->selected_index = combined;

    if (GameFlag_TestFar(0x151) == 0) {
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
        RenderOutput_ClearListFar(menu->info_window);
#endif
        WaitFrames(1);

        combined = state->selected_index;
        if (menu->psynergies[combined] != 0) {
            s32 masked = (menu->psynergies[combined] & 0x1ff) + (s32)MsgAbilityDescription;
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
            UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
        }
    } else {
        GameFlag_ClearBitFar(0x2ff);
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect(menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 14);
        } else {
            Render_SetTilemapFlagRect(menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
