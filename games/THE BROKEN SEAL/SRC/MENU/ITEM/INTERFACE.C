#include "INVENTORY_MENU.H"
#include "A8_STATE.H"
#include "OBJECT_FACTORY.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"

s32 UiMenu_CreateCursor(void *menu);
void InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void ItemMenu_Init(void)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 index;

    InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        *(u16 *)((u8 *)menu + 324 + index * 2) = 30;

    {
        s32 zero = 0;
        s32 style;
        *(s32 *)((u8 *)menu + 40) = zero;
        *(s32 *)((u8 *)menu + 36) = zero;
        style = 2;
        *(s32 *)((u8 *)menu + 44) =
            UiWindow_CreateFar(0, 17, 30, 3, style);
        *(s32 *)((u8 *)menu + 32) = zero;
        *(u8 *)((u8 *)menu + 272) = zero;
        *(u8 *)((u8 *)menu + 273) = zero;
        *(u8 *)((u8 *)menu + 274) = 8;
        *(u8 *)((u8 *)menu + 275) = style;
    }
}

void Menu_SpawnIconEntries(struct State080a8088 *state, s32 arg1)
{
    void **output0;
    s32 index0;
    s32 fifth0;
    void **output1;
    s32 index1;
    s32 fifth1;
    void **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entries[0];
    do {
        *output0++ = RenderOutput_CreateFromResourceFar(2, index0, arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entries[8];
    do {
        *output1++ = RenderOutput_CreateFromResourceFar(2, index1, arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entries[16];
    do {
        *output2++ = RenderOutput_CreateFromResourceFar(2, index2, arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icons = gMenuWork->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct InventoryMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}

void ItemMenu_HidePageIcons(void)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct InventoryMenuIcon *icon = *icon_slot++;

        if (icon != 0 && Math_Mod(slot, 5) == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}

void UiWindow_CloseIfOpen(void *, s32);
void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    InventoryMenu_HideAllItemIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    UiWindow_CloseIfOpen(menu + 0x10, 1);
    UiWindow_CloseIfOpen(menu + 0x20, 1);
    UiWindow_CloseIfOpen(menu + 0x10C, 1);
    UiWindow_CloseIfOpen(menu + 0x24, 1);
    UiWindow_CloseIfOpen(menu + 0x28, 1);
    UiWindow_CloseIfOpen(menu + 0x2C, 1);
    UiWindow_CloseIfOpen(menu + 0x30, 1);
    UiWindow_CloseIfOpen(menu + 0x34, 1);
    UiWindow_CloseIfOpen(menu + 0x38, 1);
    UiWindow_CloseIfOpen(menu + 0x3C, 1);
    UiWindow_CloseIfOpen(menu + 0x40, 1);
}
