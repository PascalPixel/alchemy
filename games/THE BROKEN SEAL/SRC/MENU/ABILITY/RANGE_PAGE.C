#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"

void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + 0xf281;
    /* Keep the empty zero path; GCC emits the reference branch shape. */
    if (!n) {
    }
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + 0xf280, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"

struct MenuEntryIcon {
    u8 unknown_00[5];
    u8 state;                    /* 0x05 */
    u8 unknown_06[6];
    u16 field_0c;                /* 0x0c */
    u8 unknown_0e;
    u8 field_0f;                 /* 0x0f */
};

struct PsynergyListWork {
    u8 unknown_000[8];
    s32 field_008;                            /* 0x008 */
    u8 unknown_00c[8];
    struct MenuEntryIcon *pane_icon[2];       /* 0x014 */
    s8 tab_index[2];                          /* 0x01c */
    u8 unknown_01e[6];
    s32 field_024;                            /* 0x024 */
    u8 unknown_028[4];
    void *info_window;                        /* 0x02c */
    u8 unknown_030[4];
    s32 list_window;                          /* 0x034 */
    u8 unknown_038[0x0c];
    struct MenuEntryIcon *entry_grid_cursor;  /* 0x044 */
    struct MenuEntryIcon *entry_icons[32];    /* 0x048 */
    u8 unknown_0c8[0x4c];
    s32 tab_objects[4];                       /* 0x114 */
    u8 unknown_124[0x20];
    u16 tab_colors[4];                        /* 0x144 */
    u8 unknown_14c[0x28];
    u16 pane_row[2];                          /* 0x174 */
    u16 pane_action[2];                       /* 0x178 */
    u8 unknown_17c[0x4c];
    u16 psynergies[32];                       /* 0x1c8 */
    u16 owner_table[8];                       /* 0x208 */
    u8 psynergy_count;                        /* 0x218 */
    u8 owner_count;                           /* 0x219 */
    u8 owner_ids[2];                          /* 0x21a */
    struct MenuEntryIcon *cursor_icon;        /* 0x21c */
    u16 flags;                                /* 0x220 */
    u8 unknown_222[0x3e];
    s8 selected_index_by_owner[8];            /* 0x260 */
    u8 mode;                                  /* 0x268 */
};

extern u8 Value_0000053a;
extern u8 Value_00000b13;
extern u8 Value_00000b14;
extern u8 Value_00000b15;

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiWindow_SetTilemapEntryFar(s32 window, s32 icon, s32 x, s32 y, s32 palette);
struct BattleAction *BattleAction_Get(s32 action);
void Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);

#define ACTION_ID_MASK 0x3fff

/* The Psynergy page with the element and range icons: names the selected
   entry and its targeting class (0xb13-0xb15), then redraws the five rows
   with their range icons and highlights. The info window is held as a
   pointer, which keeps its load independent of the page-state store. */
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
                + (s32)&Value_0000053a,
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
            UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b15, window, 0, 96);
        } else if (row == 2) {
            UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b14, window, 0, 96);
        } else if (row == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b13, window, 0, 96);
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

#include "BATTLE_TYPES.H"
#include "PSYNERGY_MENU.H"
#include "UI.H"

extern u8 Value_00000aed;
extern u8 Value_00000aef;
extern u8 Value_00000333;
extern u8 Value_00000741;
extern u8 Menu_LvString;

#define MsgPP Value_00000aed
#define MsgNoPsy Value_00000aef
#define MsgAction Value_00000333
#define MsgClass Value_00000741
#define StrLv Menu_LvString
#define PsyMenuWork gMenuWork

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
u8 *Runtime_GetObject(s32 owner);
struct BattleAction *Ability_GetData(s32 action);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
    struct PsynergyMenuState *menu = PsyMenuWork;
    u8 *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = Runtime_GetObject(menu->owner_ids[0]);

    UiWindow_Commit(window);

    first = res->page * 5;
    rows = (u8)(res->entry_count - first);
    if (rows > 5) {
        rows = 5;
    }

    Menu_SetPageIcons(5, first, window, 80, 58);
    Menu_DrawPageIndicator(window, res->entry_count, 5, res->page, 28);

    UiText_DrawAt((s32)&MsgPP, window, 176, 0);

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
                (s32)&MsgAction;
            y = row * 16 + 16;

            UiText_DrawAt(msg, window, 88, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, 176, y);

            range = act->range;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + 2, range, 0);

            row++;
            ofs += 2;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawAt((s32)&MsgNoPsy, window, 96, 17);
    }

    UiText_DrawStringAtOffsetFar(owner, (void *)window, 40, 0);
    UiText_DrawAt(
        owner[OWNER_CLASS_MSG_OFS] + (s32)&MsgClass, window, 0, 32);
    UiText_DrawStringInWindowFar(&StrLv, window, 0, 48);
    UiNumber_DrawAt(owner[OWNER_LEVEL_OFS], 2, window, 24, 48);

    return 1;
}
