#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "BTLSYS.H"
#include "EDITION.H"
#include "TYPES.H"
#include "BTLTYPE.H"
#include "PSYNERGY_MENU.H"
#include "TBS_EDITION.H"
#include "MENU_LIST.H"

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + RANGE_ICON_TILE + 1;
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

#include "TYPES.H"
#include "BTLTYPE.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"

#if !EDITION_INTERNATIONAL
#endif

void UiWindow_SetTilemapEntryFar(s32 window, s32 icon, s32 x, s32 y, s32 palette);
void Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

#define ACTION_ID_MASK 0x3fff

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    struct BtlAct *ability;
    s32 row;
    s32 base;

    menu = (struct PsynergyMenuState *)gMenuWork;
    state->selected_index = state->page * PAGE_ROWS + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
    RenderOutput_RedrawSavedRectFar(menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
    RenderOutput_ClearListFar(menu->info_window);
#endif
    WaitFrames(1);
    if (menu->psynergies[state->selected_index] != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(
#else
        UiText_DrawResourceFar(
#endif
            (menu->psynergies[state->selected_index] & ACTION_ID_MASK)
                + (s32)MsgAbilityDescription,
            (s32)menu->info_window, 0, 0);
        ability = BtlActGet(menu->psynergies[state->selected_index] & ACTION_ID_MASK);
#if EDITION_INTERNATIONAL
        UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
        row = 0;
        if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0) {
            row = 2;
        }
        if ((ability->tgt_flg & 0x80) != 0) {
            row |= 1;
        }
        if (row == 3) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableAnywhere, window, 0, 96);
        } else if (row == 2) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0, 96);
        } else if (row == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0, 96);
        }
#else
        UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
        row = 0;
        if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0, 72);
            row = 1;
        }
        if ((ability->tgt_flg & 0x80) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0, row * 8 + 72);
            row++;
        }
        UiText_DrawCharacterAtOffsetFar((s32)MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
    }

    base = state->page * PAGE_ROWS;
    for (row = 0; row <= PAGE_ROWS - 1; row++) {
        if (row == state->row) {
            ability = BtlActGet(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 0);
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 14);
                Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 14);
            }
        } else {
            ability = BtlActGet(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 4);
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 15);
                Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 15);
            }
        }
    }
    WaitFrames(1);
    return 1;
}

#include "BTLTYPE.H"
#include "PSYNERGY_MENU.H"
#include "UI.H"
#include "text/MSG_IDS.H"
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
struct BtlAct * BtlActGet(s32 action_id);

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#define UiText_DrawStringInWindowFar ((void (*)(u8*,s32,s32,s32))UiText_DrawStringInWindowFar)

#define Menu_LvString (*(u8 *)&Menu_LvString)


#define ACT_ID_MASK 0x3fff

void UiText_DrawStringAtOffsetFar(u8 *, void *, s32, s32);
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = gMenuWork;
    struct BtlUnit *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = UnitGetFar(menu->owner_ids[0]);

    RenderOutput_RedrawSavedRectFar(window);

    first = res->page * PAGE_ROWS;
    rows = (u8)(res->entry_count - first);
    if (rows > PAGE_ROWS) {
        rows = PAGE_ROWS;
    }

    Menu_SetPageIcons(PAGE_ROWS, first, window, LIST_ICONS_X, LIST_ICONS_Y);
    Menu_DrawPageIndicator(window, res->entry_count, PAGE_ROWS, res->page, 28);

    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, LIST_PP_X, 0);

    row = 0;
    if (rows > row) {
        ofs = first;
        do {
            struct BtlAct *act;
            s32 msg;
            s32 y;
            s32 range;
            act = BtlActGet(
                ACT_ID_MASK & menu->psynergies[ofs]);
            msg = (menu->psynergies[ofs] & ACT_ID_MASK) +
                (s32)MsgAbilityName;
            y = row * 16 + LIST_FIRST_ROW * 8;

            UiText_DrawCharacterAtOffsetFar(msg, window, LIST_NAME_X, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, LIST_PP_NUMBER_X, y);

            range = act->rng;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

            row++;
            ofs++;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
    }

    UiText_DrawStringAtOffsetFar(owner->name, (void *)window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(
        owner->class_index + (s32)MsgClassName, window, 0, 32);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
    UiText_DrawStringAtOffsetFar(&Menu_LvString, (void *)window, 0, 48);
#else
    UiText_DrawStringInWindowFar(&Menu_LvString, window, 0, 48);
#endif
    UiText_DrawNumberInWindowFar(owner->level, 2, window, 24, 48);

    return 1;
}
