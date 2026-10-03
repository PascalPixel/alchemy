#include "EDITION.H"
#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"
#include "TBS_EDITION.H"
void UiWindow_Commit(s32 window);


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
#include "BATTLE_TYPES.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"

extern u8 MsgAbilityDescription;
extern u8 MsgUsableInBattle;
extern u8 MsgUsableInField;
extern u8 MsgUsableAnywhere;
#if !EDITION_INTERNATIONAL
extern u8 MsgCanBeUsed;
#endif

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 icon, s32 x, s32 y, s32 palette);
struct BattleAction *BattleAction_Get(s32 action);
void Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

#define ACTION_ID_MASK 0x3fff

/* The Psynergy page with the element and range icons: names the selected
   entry and its targeting class (0xb13-0xb15), then redraws the page rows
   with their range icons and highlights. The info window is held as a
   pointer, which keeps its load independent of the page-state store. */
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    struct BattleAction *ability;
    s32 row;
    s32 base;

    menu = (struct PsynergyMenuState *)gMenuWork;
    state->selected_index = state->page * PAGE_ROWS + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    /* Spanish and Italian clear the info window's top row instead. */
    UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
    RenderOutput_ClearListFar((s32)menu->info_window);
#endif
    WaitFrames(1);
    if (menu->psynergies[state->selected_index] != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(
#else
        UiText_DrawMessageAt(
#endif
            (menu->psynergies[state->selected_index] & ACTION_ID_MASK)
                + (s32)&MsgAbilityDescription,
            (s32)menu->info_window, 0, 0);
        ability = BattleAction_Get(menu->psynergies[state->selected_index] & ACTION_ID_MASK);
#if EDITION_INTERNATIONAL
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
#else
        /* Japanese lists each usable location before the closing phrase. */
        UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
        row = 0;
        if (ability->type_0c != 0 || (ability->target_flags & 0x40) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgUsableInField, window, 0, 72);
            row = 1;
        }
        if ((ability->target_flags & 0x80) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)&MsgUsableInBattle, window, 0, row * 8 + 72);
            row++;
        }
        UiText_DrawCharacterAtOffsetFar((s32)&MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
    }

    base = state->page * PAGE_ROWS;
    for (row = 0; row <= PAGE_ROWS - 1; row++) {
        if (row == state->row) {
            ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 0);
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 14);
                Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 14);
            }
        } else {
            ability = BattleAction_Get(menu->psynergies[base + row] & ACTION_ID_MASK);
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

#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"
#include "UI.H"

extern u8 MsgPsynergyPp;
extern u8 MsgNoPsynergy;
extern u8 MsgAbilityName;
extern u8 MsgClassName;
extern u8 Menu_LvString;


#define PSY_LIST_OFS 0x1c8
#define ACT_ID_MASK 0x3fff
#define OWNER_LEVEL_OFS 15
#define OWNER_CLASS_MSG_OFS 0x129


void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_DrawPageIndicator(
    s32 window, s32 item_count, s32 page_size, s32 selected_page, s32 right_edge);
void UiText_DrawStringAtOffsetFar(u8 *, void *, s32, s32);
void UiText_DrawStringInWindowFar(u8 *, s32, s32, s32);
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);
void PsynergyMenu_DrawRange(s32, s32, s32, s32, s32);
u8 *Owner_GetStateFar(s32 owner);
struct BattleAction *Ability_GetData(s32 action);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = gMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Owner_GetStateFar(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * PAGE_ROWS;
    rows = (u8)(res->entry_count - first);
    if (rows > PAGE_ROWS) {
        rows = PAGE_ROWS;
    }

    Menu_SetPageIcons(PAGE_ROWS, first, window, LIST_ICONS_X, LIST_ICONS_Y);
    Menu_DrawPageIndicator(window, res->entry_count, PAGE_ROWS, res->page, 28);

    UiText_DrawAt((s32)&MsgPsynergyPp, window, LIST_PP_X, 0);

    row = 0;
    if (rows > row) {
        ofs = (s32)(first * 2) + PSY_LIST_OFS;
        do {
            struct BattleAction *act;
            s32 msg;
            s32 y;
            s32 range;
            act = Ability_GetData(
                ACT_ID_MASK & *(u16 *)(ofs + (s32)menu));
            msg = (*(u16 *)(ofs + (s32)menu) & ACT_ID_MASK) +
                (s32)&MsgAbilityName;
            y = row * 16 + LIST_FIRST_ROW * 8;

            UiText_DrawAt(msg, window, LIST_NAME_X, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, LIST_PP_NUMBER_X, y);

            range = act->range;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

            row++;
            ofs += 2;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawAt((s32)&MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClassName, window, 0, 32);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
    /* The Spanish and French level label is drawn at a pixel offset. */
    UiText_DrawStringAtOffsetFar(&Menu_LvString, (void *)window, 0, 48);
#else
    UiText_DrawStringInWindowFar(&Menu_LvString, window, 0, 48);
#endif
    UiText_DrawNumberInWindowFar(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}
