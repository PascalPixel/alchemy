/* 2026-09-30 (Mercury): 8 differing halfwords, 170 of 170 bytes plus the
   pad (was 85 at 158). Plain C without the permuter's registers: reading
   owner_index[slot] before the cursor gives the reference's offsets (slot +
   28 before slot * 4, both kept whole) and registers; the pooled zero
   stored into owner_index is the halfword zero of cursor->frame, which CSE
   shares and local-alloc moves to its use in the other block. Left: sched2
   order. The reference loads the cursor into r0 right after the slot * 4 +
   20 address and reads the index last (ldrsb r7 after mov r8, r2); here the
   cursor takes r2, so it waits behind the index read and the r8 copy (anti
   dependences on r2). Cursor-first spellings give a different 160-byte
   shape (84). */
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

extern struct OwnerSelectMenu *gMenuWork;

void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_IsSet(s32 flag);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_RunRearrange(void);
void UiIcon_PrepareObject(struct OwnerCursor *cursor);
void WaitFrames(s32 frames);

s32 CharacterMenu_SelectOwner(s32 slot)
{
    struct OwnerSelectMenu *menu;
    struct OwnerCursor *cursor;
    s32 result;
    s32 index;

    menu = gMenuWork;
    index = menu->owner_index[slot];
    result = 0;
    cursor = menu->cursors[slot];
    cursor->state = 1;
    cursor->frame = 0;
    RenderOutput_RedrawSavedRectFar(menu->window);
    if (GameFlag_IsSet(0x172))
        UiWindow_DrawDividerLineFar(menu->window, 9, 1, 9, 3);
    if (index == -1)
        menu->owner_index[slot] = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->mode == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = CharacterSelector_RunRearrange();
    UiIcon_PrepareObject(menu->cursors[slot]);
    WaitFrames(1);
    return result;
}
