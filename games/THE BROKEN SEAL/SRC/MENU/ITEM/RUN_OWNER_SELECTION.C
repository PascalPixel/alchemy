#include "EDITION.H"
#include "BATTLE_RUNTIME.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "INVENTORY_MENU.H"
#include "SYSTEM.H"

extern volatile u32 gKeyState;
extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern char MsgArrangeItemsHelp;

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
struct RenderOutput *RenderOutput_CreateFromResourceFar(
    s32 kind, s32 index, struct RenderInput *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
#if !EDITION_INTERNATIONAL
/* The key names the Japanese help lines follow: "L+A:" and "R:". */
extern u8 ItemMenu_ArrangeKeysString[];
extern u8 ItemMenu_EquipmentKeyString[];
void UiText_DrawStringInWindowFar(const u8 *text, s32 window, s32 x, s32 y);
#endif
void UiMenu_PositionCursor(s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
s32 InventoryMenu_SortByListOrder(u16 *items, s32 mode);
s32 GameFlag_TestFar(s32 flag);
void Audio_PlayCue(s32 cue);

/* Choose whose items to list: left and right step through the party, R held
   shows the member's items by category, L with A cycles the sort order, A
   picks a member who carries something and B returns -1. */
s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items)
{
    struct InventoryMenuState *menu;
    s32 selection;
    s32 count;
    s32 result;
    struct BattleUnit *owner;
    u8 sort_mode;
    s32 by_category;
    s32 pending;
    s32 window;
    s32 i;

    menu = gMenuWork;
    selection = menu->pane_index[0];
    count = menu->pane_count[0];
    pending = 1;
    result = 0;
    sort_mode = 0;
    by_category = 0;
    owner = Owner_GetStateFar(owner_ids[selection]);
    if (UiWindow_UpdateOrCreate((s32 *)&menu->item_window, 13, 3, 17, 10, 2))
        Menu_SpawnIconEntries(menu, (s32)menu->item_window);
    if (UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 13, 13, 17, 4, 2)) {
        menu->selected_item_icon = RenderOutput_CreateFromResourceFar(2, 0, (struct RenderInput *)menu->help_window, 0, result);
        menu->selected_item_icon->active = 13;
    }
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar((s32)&MsgArrangeItemsHelp, (s32)menu->help_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgArrangeItemsHelp + 1, (s32)menu->help_window, 0, 8);
#else
    /* The Japanese help lines follow their key names. */
    UiText_DrawStringInWindowFar(ItemMenu_ArrangeKeysString, (s32)menu->help_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgArrangeItemsHelp, (s32)menu->help_window, 32, 0);
    UiText_DrawStringInWindowFar(ItemMenu_EquipmentKeyString, (s32)menu->help_window, 16, 8);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgArrangeItemsHelp + 1, (s32)menu->help_window, 32, 8);
#endif
    menu->pane_icons[0]->active = pending;
    while (!GameFlag_TestFar(0x150)) {
        selection = (selection + count) % count;
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        if (pending) {
            sort_mode = 0;
            pending = 0;
            window = (s32)menu->status_window;
            owner = Owner_GetStateFar(owner_ids[selection]);
            if (by_category) {
                menu->item_count = ItemMenu_Collect(Owner_GetStateFar(owner_ids[selection]), menu->items, 0);
                ItemMenu_OpenCategory(owner_ids[selection]);
                Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 8);
            } else {
                ItemMenu_RefreshOwner(owner_ids[selection], 0);
                Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            }
            for (i = 3; i >= 0; i--)
                menu->owner_y[i] = 30;
            menu->owner_y[selection] = 26;
        }
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (gKeysHeld & KEY_L) {
                sort_mode = (sort_mode + 4) % 4;
                InventoryMenu_SortByListOrder(owner->inventory, sort_mode);
                sort_mode++;
                ItemMenu_RefreshOwner(owner_ids[selection], 0);
                Audio_PlayCue(112);
            } else if (ItemMenu_Count(owner_ids[selection])) {
                Audio_PlayCue(112);
                result = owner_ids[selection];
                break;
            } else {
                Audio_PlayCue(114);
            }
        }
        if (gKeyState & KEY_B) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (gKeyState & KEY_R) {
            by_category = 1;
            pending = 1;
        }
        if (!(gKeysHeld & KEY_R) && by_category == 1) {
            by_category = 0;
            pending = 1;
        }
        if (gKeysRepeat & KEY_LEFT) {
            Audio_PlayCue(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            Audio_PlayCue(111);
            selection++;
            pending = 1;
        }
    }
    menu->pane_index[0] = selection;
    menu->selected_owner = owner_ids[selection];
    menu->pane_owner[0] = owner_ids[selection];
    return result;
}
