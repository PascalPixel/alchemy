/* Draft, not exact (2026-09-26): complete extent [080a77a4, 080a7850),
   172 bytes including the interior pool and final alignment halfword.
   Caller CharacterSelector_Run consumes -1 or the stored chosen member;
   both selector callees return s32; redraw and icon preparation are void.
   H1: exact sibling ItemMenu_PrepOwner keeps byte offsets, rather than
   cursor/owner pointers, and initializes its eventual result to zero.
   Candidate 158/172 bytes, 85 differing halfwords, 45 aligned edits;
   equal topology.  The zero/result pseudo is recovered, but CSE retains
   menu+slot*4 rather than the independent slot*4 offset.  No adoption.
   H2: form complete cursor field offsets in short-lived locals, deriving
   slot offsets before loading the menu.  158/172 bytes, 85 differing
   halfwords, 38 aligned edits.  Indexed cursor loads and menu/result
   register roles are recovered, but CSE retains slot*4+20 in r8 instead
   of slot*4 in sl; owner offset/index use r7/r6 instead of r8/r7.
   H3: the exact sibling's ordinary register declarations generate
   identical bytes to H2 (158/172, 85 halfwords, 38 aligned edits).
   Three bounded hypotheses used; preserve this result and stop.  No
   source-path registration or DONE credit has been added for this owner.
   Earlier baseline (2026-09-24): 162/172 bytes, 68 differing halfwords.
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

extern struct OwnerSelectMenu *gMenuWork;

void RenderOutput_RedrawSavedRectFar(s32 window);
s32 GameFlag_TestFar(s32 flag);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
s32 PsynergyMenu_SelectOwner(void);
s32 CharacterSelector_RunRearrange(void);
void UiIcon_PrepareObject(struct OwnerCursor *cursor);
void WaitFrames(s32 frames);

s32 CharacterMenu_SelectOwner(s32 slot)
{
    register struct OwnerSelectMenu *menu;
    register s32 index;
    s32 result;
    register s32 owner_offset;
    register s32 cursor_offset;
    struct OwnerCursor *cursor;

    owner_offset = slot + 28;
    cursor_offset = slot * 4;
    menu = gMenuWork;
    result = 0;
    {
        s32 off = cursor_offset + 20;
        cursor = *(struct OwnerCursor **)((u8 *)menu + off);
    }
    cursor->state = 1;
    cursor->frame = result;
    index = *(s8 *)((u8 *)menu + owner_offset);
    RenderOutput_RedrawSavedRectFar(menu->window);
    if (GameFlag_TestFar(0x172))
        UiWindow_DrawDividerLineFar(menu->window, 9, 1, 9, 3);
    if (index == -1)
        *(s8 *)((u8 *)menu + owner_offset) = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->mode == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = CharacterSelector_RunRearrange();
    {
        s32 off = cursor_offset + 20;
        UiIcon_PrepareObject(*(struct OwnerCursor **)((u8 *)menu + off));
    }
    WaitFrames(1);
    return result;
}
