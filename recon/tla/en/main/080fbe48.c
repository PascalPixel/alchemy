/*
 * Draft: ItemMenu_SetMsgWin3, ☀️'s MENU/ITEM/WINDOW.C setter; it goes after
 * ItemMenu_SetItemWin3 in ⚓️'s MENU/ITEM/WINDOW.C. Score 2: the listing
 * allocates the outgoing argument (sub sp, #4) straight after loading the
 * menu work, before adding the window's 0xf0 offset; this C allocates it
 * after loading the window. A plain asm barrier after the work load does
 * not move it. It matches with load scheduling (-mtune=arm9tdmi).
 */
#include "M7_INTERFACES.H"
#include "RAM_BUFFER.H"

struct ItemMenuWindows {
    u8 unknown_00[0x24];
    struct RenderInput *item_window;
    u8 unknown_28[0xc8];
    struct RenderInput *message_window;
};

#define ITEM_MENU ((struct ItemMenuWindows *)Ram_HeapSlots->menu_runtime)

void ItemMenu_SetMsgWin3(void)
{
    UiWindow_SetBounds(ITEM_MENU->message_window, 13, 0, 17, 3);
}
