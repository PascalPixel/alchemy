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

extern struct PsynergyListWork *gMenuWork;

extern u8 Value_0000053a;
extern u8 Value_00000b13;
extern u8 Value_00000b14;
extern u8 Value_00000b15;

void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
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

    menu = gMenuWork;
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
