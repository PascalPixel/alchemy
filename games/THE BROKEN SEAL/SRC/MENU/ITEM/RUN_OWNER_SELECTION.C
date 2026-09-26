#include "TYPES.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"

struct OwnerSelectIcon {
    u8 reserved_00[5];
    u8 state;                         /* 0x05 */
};

/* The item menu work block at 0x03001F2C, as the owner selector sees it. */
struct OwnerSelectMenu {
    u8 reserved_000[8];
    s32 selected_owner;               /* 0x008 */
    u8 reserved_00c[8];
    struct OwnerSelectIcon *cursor;   /* 0x014 */
    u8 reserved_018[4];
    s8 selection;                     /* 0x01c */
    u8 reserved_01d;
    s8 count;                         /* 0x01e */
    u8 reserved_01f;
    s32 item_window;                  /* 0x020 */
    s32 status_window;                /* 0x024 */
    s32 help_window;                  /* 0x028 */
    u8 reserved_02c[0x118];
    u16 row_positions[4];             /* 0x144 */
    u8 reserved_14c[0x7c];
    u16 items[32];                    /* 0x1c8 */
    u8 reserved_208[0x10];
    u8 item_count;                    /* 0x218 */
    u8 party_count;                   /* 0x219 */
    u8 item_owner;                    /* 0x21a */
    u8 target_owner;                  /* 0x21b */
    struct OwnerSelectIcon *help_icon; /* 0x21c */
};

extern struct OwnerSelectMenu *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern char Value_00000b87;

struct OwnerInventoryState *Owner_GetStateFar(s32 owner);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_SpawnIconEntries(struct OwnerSelectMenu *menu, s32 window);
struct OwnerSelectIcon *RenderOutput_CreateFromResourceFar(s32 kind, s32 index, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 Math_Mod(s32 numerator, s32 denominator);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items, s32 mode);
void ItemMenu_OpenCategory(s32 owner);
void ItemMenu_RefreshOwner(s32 owner, s32 mode);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
s32 InventoryMenu_SortByListOrder(u16 *items, s32 mode);
s32 ItemMenu_Count(s32 owner);
s32 GameFlag_TestFar(s32 flag);
void Audio_PlayCue(s32 cue);

/* Choose whose items to list: left and right step through the party, R held
   shows the member's items by category, L with A cycles the sort order, A
   picks a member who carries something and B returns -1. */
s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items)
{
    struct OwnerSelectMenu *menu;
    s32 selection;
    s32 count;
    s32 result;
    struct OwnerInventoryState *owner;
    u8 sort_mode;
    s32 by_category;
    s32 pending;
    s32 window;
    s32 i;

    menu = gMenuWork;
    selection = menu->selection;
    count = menu->count;
    pending = 1;
    result = 0;
    sort_mode = 0;
    by_category = 0;
    owner = Owner_GetStateFar(owner_ids[selection]);
    if (UiWindow_UpdateOrCreate(&menu->item_window, 13, 3, 17, 10, 2))
        Menu_SpawnIconEntries(menu, menu->item_window);
    if (UiWindow_UpdateOrCreate(&menu->help_window, 13, 13, 17, 4, 2)) {
        menu->help_icon = RenderOutput_CreateFromResourceFar(2, 0, menu->help_window, 0, result);
        menu->help_icon->state = 13;
    }
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b87, menu->help_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b87 + 1, menu->help_window, 0, 8);
    menu->cursor->state = pending;
    while (!GameFlag_TestFar(0x150)) {
        selection = Math_Mod(selection + count, count);
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        if (pending) {
            sort_mode = 0;
            pending = 0;
            window = menu->status_window;
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
                menu->row_positions[i] = 30;
            menu->row_positions[selection] = 26;
        }
        WaitFrames(1);
        if (gKeyState & 1) {
            if (gKeysHeld & 0x200) {
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
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (gKeyState & 0x100) {
            by_category = 1;
            pending = 1;
        }
        if (!(gKeysHeld & 0x100) && by_category == 1) {
            by_category = 0;
            pending = 1;
        }
        if (gKeysRepeat & 32) {
            Audio_PlayCue(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & 16) {
            Audio_PlayCue(111);
            selection++;
            pending = 1;
        }
    }
    menu->selection = selection;
    menu->selected_owner = owner_ids[selection];
    menu->item_owner = owner_ids[selection];
    return result;
}
