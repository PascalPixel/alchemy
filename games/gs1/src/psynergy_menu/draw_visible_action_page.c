#include "battle_types.h"
#include "psynergy_menu.h"

void UiWindow_Commit(s32 window);
void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
void Func_080a21b0(s32 window, s32 count, s32 page_size, s32 page, s32 style);
s32 UiPalette_SetColor(s32 color);
void Func_080150a8(s32 value, s32 digits, s32 layer, s32 x, s32 y);
struct BattleUnit *Func_08077008(s32 owner);
struct BattleAction *Ability_GetData(s32 action);
extern u8 Value_00000333;

s32 Func_080a6b64(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 ofs;
    struct BattleUnit *owner;
    struct BattleAction *ability;
    struct PsynergyMenuState *menu = Data_03001f2c;

    (void)unused;

    UiWindow_Commit(window);
    Func_08015070(window, 0, 11, 16, 11);

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

    Func_080a2324(5, first_entry, window, 0x70, 0x22);
    Func_080a21b0(window, state->entry_count, 5, state->page, 15);
    UiText_DrawAt(0xaed, window, 0x60, 0);

    row = 0;
    if (visible_count > row) {
        ofs = (s32)(first_entry * 2) + 0x1c8;
        do {
            owner = Func_08077008(menu->owner_ids[0]);
            ability = Ability_GetData(0x3fff & *(u16 *)(ofs + (s32)menu));

            if (ability->pp_cost > owner->pp) {
                UiPalette_SetColor(2);
            } else if (Func_080a735c(0x3fff & *(u16 *)(ofs + (s32)menu)) != 0) {
                UiPalette_SetColor(4);
            } else {
                UiPalette_SetColor(15);
            }

            UiText_DrawAt(
                (0x3fff & *(u16 *)(ofs + (s32)menu)) + (s32)&Value_00000333,
                window, 16, row * 16 + 8);
            Func_080150a8(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiPalette_SetColor(15);

            row++;
            ofs += 2;
        } while (visible_count > row);
    }

    return 1;
}
