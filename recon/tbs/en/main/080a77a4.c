/* Draft, not exact (2026-09-24): candidate=162 reference=172 differing_halfwords=68.
   Menu: open the owner selector for one party slot. Open: the reference
   computes slot + 28 and slot * 4 before loading the menu cell, keeps the
   cursor offset in sl and the owner offset in r8, and zeroes the cursor
   frame from the register that later holds the result. */
#include "TYPES.H"

struct OwnerCursor {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[6];
    u16 frame;
};

struct OwnerSelectMenu {
    u8 unknown_000[0x10];
    s32 window;
    struct OwnerCursor *cursors[2];
    s8 owner_index[2];
    u8 unknown_01e[0x202];
    u16 mode;
};

extern struct OwnerSelectMenu *Data_03001f2c;

void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_TestFar(s32 flag);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
s32 PsynergyMenu_SelectOwner(void);
s32 Func_080a7a34(void);
void UiIcon_PrepareObject(struct OwnerCursor *cursor);
void WaitFrames(s32 frames);

s32 Func_080a77a4(s32 slot)
{
    struct OwnerSelectMenu *menu;
    s32 index;
    s32 result;
    s8 *owner;
    struct OwnerCursor **cursor;

    menu = Data_03001f2c;
    owner = &menu->owner_index[slot];
    cursor = &menu->cursors[slot];
    (*cursor)->state = 1;
    (*cursor)->frame = 0;
    index = *owner;
    RenderOutput_RedrawSavedRectFar(menu->window);
    if (GameFlag_TestFar(0x172))
        UiWindow_DrawDividerLineFar(menu->window, 9, 1, 9, 3);
    if (index == -1)
        *owner = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->mode == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = Func_080a7a34();
    UiIcon_PrepareObject(*cursor);
    WaitFrames(1);
    return result;
}
