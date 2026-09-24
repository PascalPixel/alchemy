#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "GLOBAL_CELLS.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "UI.H"

struct MenuEntryIcon {
    u8 unknown_00[5];
    u8 state;                    /* 0x05 */
    u8 unknown_06[9];
    u8 field_0f;                 /* 0x0f */
};

struct ItemListWork {
    u8 unknown_000[8];
    s32 field_008;                            /* 0x008 */
    u8 unknown_00c[8];
    struct MenuEntryIcon *pane_icon[2];       /* 0x014 */
    s8 tab_index[2];                          /* 0x01c */
    u8 unknown_01e[6];
    s32 field_024;                            /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;                          /* 0x02c */
    s32 equip_window;                         /* 0x030 */
    u8 unknown_034[0x14];
    struct MenuEntryIcon *entry_icons[32];    /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 icon_list;                            /* 0x10c */
    u8 unknown_110[0xb8];
    u16 items[32];                            /* 0x1c8 */
    u16 owner_table[8];                       /* 0x208 */
    u8 item_count;                            /* 0x218 */
    u8 owner_count;                           /* 0x219 */
    u8 owner_ids[2];                          /* 0x21a */
    u8 unknown_21c[0x44];
    s8 selected_index_by_owner[8];            /* 0x260 */
};

extern struct ItemListWork *gMenuWork;

/* "{L}-{R}:Switch characters", then "{A}:Status". */
extern u8 Value_00000b06;

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

#define KEY_A 1
#define KEY_B 2
#define KEY_R 0x100
#define KEY_L 0x200

struct BattleUnit *Owner_GetStateFar(s32 owner);
s32 GameFlag_TestFar(s32 flag);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void ItemMenu_PosCategory(void);
void Scheduler_RemoveCallback(void (*callback)(void));
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Menu_UpdateEntryObjectTransforms(void);
void Menu_SpawnIconEntries(struct ItemListWork *menu, s32 list);
u8 ItemMenu_Collect(struct BattleUnit *owner, u16 *items, s32 mode);
s32 InventoryMenu_SortByListOrder(u16 *items, s32 mode);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 Unnamed_080a1fd4(s32 mode, s32 count, s32 page_size, s32 *row, s32 *page);
void Audio_PlayCue(s32 cue);
s32 Math_Mod(s32 value, s32 divisor);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void ItemMenu_HideAllIcons(void);

/* The status screen's item page: browse the current character's items, switch
   characters with L and R, and return 1 for A or -1 for B. */
s32 ItemMenu_SelectItem(void)
{
    struct ItemListWork *menu;
    struct BattleUnit *owner;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    u16 saved[15];
    struct MenuResult state;

    menu = gMenuWork;
    {
        s32 id = menu->owner_ids[0];

        result = 0;
        Owner_GetStateFar(menu->owner_table[id]);
    }
    UiWindow_UpdateOrCreate(&menu->equip_window, 0, 10, 15, 10, 2);
    ItemMenu_PosCategory();

    for (i = 0; i < 32; i++) {
        struct MenuEntryIcon *icon = menu->entry_icons[i];

        if (icon != 0)
            icon->field_0f = 240;
    }

    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b06, menu->field_024, 64, -24);
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b06 + 3, menu->field_024, 0, -24);
    ItemMenu_PosCategory();
    WaitFrames(1);
    Menu_SpawnIconEntries(menu, menu->icon_list);

    done = 0;
    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->field_024);
        owner = Owner_GetStateFar(menu->owner_ids[0]);
        for (i = 0; i <= 14; i++)
            saved[i] = owner->inventory[i];
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
        InventoryMenu_SortByListOrder(menu->items, 0);
        Menu_BuildPageResult(&state, 0);
        RenderOutput_RedrawSavedRectFar(menu->equip_window);
        ItemMenu_DrawCategory(menu->equip_window, menu->owner_ids[0], 1);
        WaitFrames(1);
        ItemMenu_DrawIcons(menu->items, 0);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                RenderOutput_ClearListFar(menu->info_window);
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    Shop_DrawItemPage(menu->field_024, 0, &state);
                }
                ItemMenu_DrawEquipPage(menu->field_024, 0, &state);
            }
            menu->pane_icon[0]->state = 1;
            UiMenu_PositionCursor(96, state.row * 16 + 52);
            WaitFrames(1);

            nav = Unnamed_080a1fd4(0, state.entry_count, 5, &state.row, &state.page);
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;

            if (INPUT_NEW_KEYS & KEY_A) {
                Audio_PlayCue(0x70);
                result = 1;
                done = 1;
                break;
            }
            if (INPUT_NEW_KEYS & KEY_B) {
                Audio_PlayCue(0x71);
                result = -1;
                done = 1;
                break;
            }
            if ((INPUT_REPEAT_KEYS & KEY_R) || (INPUT_REPEAT_KEYS & KEY_L)) {
                Audio_PlayCue(0x6f);
                tab = menu->tab_index[0];
                menu->selected_index_by_owner[menu->owner_table[tab]] = state.selected_index;
                if (INPUT_REPEAT_KEYS & KEY_R)
                    tab++;
                else
                    tab--;
                for (i = 0; i <= 14; i++)
                    owner->inventory[i] = saved[i];
                tab = Math_Mod(tab + menu->owner_count, menu->owner_count);
                menu->field_008 = menu->owner_table[tab];
                menu->owner_ids[0] = menu->owner_table[tab];
                menu->tab_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_table[tab]);
                break;
            }
        }
    }

    RenderOutput_ClearListFar(menu->info_window);
    RenderOutput_RedrawSavedRectFar(menu->info_window);
    RenderOutput_ClearListFar(menu->icon_list);
    ItemMenu_HideAllIcons();
    RenderOutput_RedrawSavedRectFar(menu->field_024);
    {
        s32 delay = 200;

        delay <<= 4;
        Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, delay);
    }
    return result;
}
