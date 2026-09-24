#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"


void RenderOutput_RedrawSavedRectFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
s32 UiWork_SetParamNibbleFar(s32 color);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
struct BattleUnit *Owner_GetStateFar(s32 owner);
struct BattleAction *BattleAction_Get(s32 action);
extern u8 MsgAbilityName;

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
    u32 first_entry;
    u32 visible_count;
    u8 row;
    s32 cursor;
    struct BattleUnit *owner;
    struct BattleAction *ability;
    struct PsynergyMenuState *menu = gMenuWork;

    (void)unused;

    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    if (2 & *(u16 *)((u8 *)menu + 0x220)) {
        UiText_DrawCharacterAtOffsetFar(0xae1, window, 0, 88);
    } else {
        UiText_DrawCharacterAtOffsetFar(0xb89, window, 0, 88);
    }

    first_entry = state->page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, 0x70, 0x22);
    Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);
    UiText_DrawCharacterAtOffsetFar(0xaed, window, 0x60, 0);

    row = 0;
    if (visible_count > row) {
        cursor = first_entry * 2 + 0x1c8;
        do {
            owner = Owner_GetStateFar(menu->owner_ids[0]);
            ability = BattleAction_Get(0x3fff & *(const u16 *)(cursor + (s32)menu));

            if (ability->pp_cost > owner->pp) {
                UiWork_SetParamNibbleFar(2);
            } else if (PsynergyMenu_IsActionRestricted(0x3fff & *(const u16 *)(cursor + (s32)menu)) != 0) {
                UiWork_SetParamNibbleFar(4);
            } else {
                UiWork_SetParamNibbleFar(15);
            }

            UiText_DrawCharacterAtOffsetFar(
                (0x3fff & *(const u16 *)(cursor + (s32)menu)) + (s32)&MsgAbilityName,
                window, 16, row * 16 + 8);
            UiText_DrawNumberAtOffsetFar(ability->pp_cost, 2, window, 104, row * 16 + 8);
            UiWork_SetParamNibbleFar(15);

            row++;
            cursor += 2;
        } while (visible_count > row);
    }

    return 1;
}
