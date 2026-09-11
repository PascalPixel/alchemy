#include "scene.h"
#include "abi/psynergy_menu/draw_action_page.h"
#include "battle_types.h"
#include "psynergy_menu.h"

void UiWindow_Commit(s32 window);

void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);

s32 UiPalette_SetColor(s32 color);

struct BattleUnit *Sys_Run(s32 owner);
struct BattleAction *Ability_GetData(s32 action);
extern u8 Value_00000333;

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BattleUnit *owner;
    struct BattleAction *ability;
    struct PsynergyMenuState *menu = gIw;

    (void)unused;

    UiWindow_Commit(window);
    Sys_SetRange(window, 0, 11, 16, 11);

    if (2 & *(u16 *)((u8 *)menu + 0x220)) {
        UiText_DrawAt(0xae1, window, 0, 88);
    } else {
        UiText_DrawAt(0xb89, window, 0, 88);
    }

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Sys_SetRange2(5, first_entry, window, 0x70, 0x22);
    Sys_SetRange3(window, state->entry_count, 5, state->page, 15);
    UiText_DrawAt(0xaed, window, 0x60, 0);

    row = 0;
    if (visible_count > row) {
        cursor = first_entry * 2 + 0x1c8;
        do {
            owner = Sys_Run(menu->owner_ids[0]);
            ability = Ability_GetData(0x3fff & *(const u16 *)(cursor + (s32)menu));

            if (ability->pp_cost > owner->pp) {
                UiPalette_SetColor(2);
            } else if (Sys_Run2(0x3fff & *(const u16 *)(cursor + (s32)menu)) != 0) {
                UiPalette_SetColor(4);
            } else {
                UiPalette_SetColor(15);
            }

            UiText_DrawAt(
                (0x3fff & *(const u16 *)(cursor + (s32)menu)) + (s32)&Value_00000333,
                window, 16, row * 16 + 8);
            Sys_SetRange4(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiPalette_SetColor(15);

            row++;
            cursor += 2;
        } while (visible_count > row);
    }

    return 1;
}
