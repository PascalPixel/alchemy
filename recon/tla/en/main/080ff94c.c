#include "TYPES.H"
#include "BATTLE_TYPES.H"
void UiWindow_Commit(s32 window);

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state)
{
    struct PsynergyListWork *menu;
    struct BattleAction *ability;
    s32 row;
    s32 base;

    menu = (struct PsynergyListWork *)gMenuWork;
    state->selected_index = state->page * 5 + state->row;
    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
    WaitFrames(1);
    if (menu->psynergies[state->selected_index] != 0) {
        UiText_DrawCharacterAtOffsetFar((menu->psynergies[state->selected_index] & ACTION_ID_MASK)
                + (s32)&MsgAbilityDescription,
            (s32)menu->info_window, 0, 0);
        ability = BattleAction_Get(menu->psynergies[state->selected_index] & ACTION_ID_MASK);
        UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
        row = 0;
        if (ability->type_0c != 0 || (ability->target_flags & 0x40) != 0) {
            row = 2;
        }
        if ((ability->target_flags & 0x80) != 0) {
            row |= 1;
        }
        if (row == 3) {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgUsableAnywhere, window, 0, 96);
        } else if (row == 2) {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgUsableInField, window, 0, 96);
        } else if (row == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgUsableInBattle, window, 0, 96);
        }
    }

    base = state->page * 5;
    for (row = 0; row <= 4; row++) {
        if (row == state->row) {
            ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + 2, 0);
                Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 14);
                Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 14);
            }
        } else {
            ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + 2, 4);
                Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 15, 1, 15);
                Render_SetTilemapFlagRect(window, 25, row * 2 + 2, 3, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + 2, 19, 1, 15);
            }
        }
    }
    WaitFrames(1);
    return 1;
}
