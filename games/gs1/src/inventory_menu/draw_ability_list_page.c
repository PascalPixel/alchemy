#include "inventory_menu.h"
#include "menu_result.h"
#include "owner_state.h"
#include "types.h"

extern u8 Value_00000aed;
extern u8 Value_00000aef;
extern u8 Value_00000333;
extern u8 Value_00000741;
extern u8 Data_080af22c;

void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(
    s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void Func_08015090(s32, void *, s32, s32);
void Func_08015098(u8 *, s32, s32, s32);
void Func_080150a8(s32, s32, s32, s32, s32);
void Func_080a8cc0(s32, s32, s32, s32, s32);

#define InventoryMenu_DrawAbilityListPage Func_080a8f40

s32 InventoryMenu_DrawAbilityListPage(
    s32 window, s32 unused, const struct MenuResult *state)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    struct OwnerInventoryState *owner;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 item_offset;

    (void)unused;

    owner = Runtime_GetObject(menu->item_owner);

    UiWindow_Commit(window);

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Func_080a2324(5, first_entry, window, 80, 58);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 28);

    UiText_DrawAt((s32)&Value_00000aed, window, 176, 0);

    row = 0;
    if (visible_count > row) {
        item_offset = (s32)(first_entry * 2) + 0x1c8;
        do {
            u8 *ability;
            s32 message;
            s32 y;
            s32 count;
            ability = Ability_GetData(
                0x3fff & *(u16 *)(item_offset + (s32)menu));
            message =
                (*(u16 *)(item_offset + (s32)menu) & 0x3fff) +
                (s32)&Value_00000333;
            y = row * 16 + 16;

            UiText_DrawAt(message, window, 88, y);
            Func_080150a8(ability[9], 2, window, 176, y);

            count = ability[8];
            if (count == 0xff) {
                count = 11;
            } else {
                count--;
            }
            Func_080a8cc0(window, 25, row * 2 + 2, count, 0);

            row++;
            item_offset += 2;
        } while (visible_count > row);
    }

    if (menu->item_count == 0) {
        UiText_DrawAt((s32)&Value_00000aef, window, 96, 17);
    }

    Func_08015090((s32)owner, (void *)window, 40, 0);
    UiText_DrawAt(
        *((u8 *)owner + 0x129) + (s32)&Value_00000741, window, 0, 32);
    Func_08015098(&Data_080af22c, window, 0, 48);
    UiNumber_DrawAt(*((u8 *)owner + 15), 2, window, 24, 48);

    return 1;
}
