/* Near miss, differ 6 of 79: the twin of The Broken Seal's CharacterMenu_SelectOwner.
   Only the first block's order differs: the native finishes the heap slots' address
   (lsls r3, r3, #18) before it starts the index's offset and loads the menu before it
   saves slot * 4 (ldr r5, [r3]; mov r10, r0); here both come one step later. Four
   minutes of alchemy permute found nothing nearer. */
#include "types.h"
#include "ram_buffer.h"

struct OwnerCursor {
    u8 unknown_00[5];
    u8 active;
    u8 unknown_06[6];
    u16 anim_step;
};

struct OwnerMenuState {
    u8 unknown_000[16];
    void *owner_window;
    struct OwnerCursor *owner_cursors[2];
    s8 owner_index[2];
    u8 unknown_01e[0x1fe];
    u16 page;
};

void RenderOutput_RedrawSavedRectFar(void *window);
s32 GameFlag_Test(s32 flag);
void UiWindow_DrawDividerLineFar(void *window, u32 x1, u32 y1, u32 x2, u32 y2);
void Func_080f8ab4(s32 x, s32 y);
s32 Func_080feb58(void);
s32 Func_080fe894(void);
void UiIcon_PrepareObject(struct OwnerCursor *cursor);
void WaitFrames(s32 frames);

s32 Func_080fe580(s32 slot)
{
    struct OwnerMenuState *menu;
    struct OwnerCursor *cursor;
    s32 result;
    s32 index;

    menu = (struct OwnerMenuState *)Ram_HeapSlots->menu_runtime;
    index = menu->owner_index[slot];
    result = 0;
    cursor = menu->owner_cursors[slot];
    cursor->active = 1;
    cursor->anim_step = 0;
    RenderOutput_RedrawSavedRectFar(menu->owner_window);
    if (GameFlag_Test(0x172))
        UiWindow_DrawDividerLineFar(menu->owner_window, 9, 1, 9, 3);
    UiWindow_DrawDividerLineFar(menu->owner_window, 0, 3, 12, 3);
    if (index == -1)
        menu->owner_index[slot] = 0;
    else
        Func_080f8ab4(index * 24 - 10, 16);
    if (menu->page == 3)
        result = Func_080feb58();
    else
        result = Func_080fe894();
    cursor = menu->owner_cursors[slot];
    UiIcon_PrepareObject(cursor);
    WaitFrames(1);
    return result;
}
