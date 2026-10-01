#include "EDITION.H"
#include "INVENTORY_MENU.H"
/* Item menu: ask whether to drop the item and return the chosen row (1 when cancelled). */
#include "TYPES.H"
#include "IWRAM_CALL.H"

extern u8 MsgItemName;
extern void UiIcon_PrepareObject(void *icon);
extern s32 Owner_GetStateFar(s32);
extern void UiText_DrawStringAtOffsetFar(s32, void *, s32, s32);
extern void UiText_DrawCharacterAtOffsetFar(s32, void *, s32, s32);

extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
extern u8 MsgConfirmDrop[];
extern u8 MsgYes[];
s32 __modsi3(s32, s32);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Item_Get(s32);
s32 GameFlag_IsSet(s32);
void UiMenu_PositionCursor(s32, s32);
void UiMenu_SlideCursor(s32, s32);
void Audio_PlayCue(s32 cue);

void ItemMenu_DrawItemHead(void)
{
    struct InventoryMenuState *menu = gMenuWork;

    Resource_LoadByModeIntoSlotFar(
        2, menu->selected_item, menu->selected_item_icon->render_target, 0);
    menu->selected_item_icon->state = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    UiIcon_PrepareObject(menu->selected_item_icon);
    UiText_DrawStringAtOffsetFar(
        Owner_GetStateFar(menu->item_owner),
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawCharacterAtOffsetFar(
        (menu->selected_item & 0x1FF) +
            (s32)&MsgItemName,
        (void *)menu->message_window,
        16,
        8);
}

s32 ItemMenu_ConfirmDrop(s32 a0)
{
    volatile s32 *pad;
    s32 win;
    s32 slot;
    s32 text;
    s32 label;
    register s32 sel asm("r6"); /* FAKEMATCH: keeps the row in r6 */
    s32 changed;

    win = UiWindow_CreateFar(13, 3, 17, 10, 2);
    slot = a0 & 0x1ff;
    Item_Get(slot);
    UiText_DrawAt(slot + (s32)((u8 *)&MsgItemName), win, 24, 0);
    text = (s32)MsgConfirmDrop;
#if EDITION_INTERNATIONAL
    UiText_DrawAt(text, win, 0, 16);
#else
    UiText_DrawAt(text, win, 8, 16);
#endif
    text++;
#if EDITION_INTERNATIONAL
    UiText_DrawAt(text, win, 0, 24);
#else
    UiText_DrawAt(text, win, 8, 24);
#endif
    label = (s32)MsgYes;
    UiText_DrawAt(label, win, 24, 40);
    label++;
    UiText_DrawAt(label, win, 24, 56);
    sel = 1;
    changed = 1;
    UiMenu_SlideCursor(104, 86);
    for (;;) {
        if (GameFlag_IsSet(0x150) != 0) {
            break;
        }
        {
        register s32 c asm("r2"); /* FAKEMATCH: tests the flag through r2 */
        asm("mov %0, %1" : "=l"(c) : "h"(changed)); /* FAKEMATCH: tests the flag through r2 */
        if (c) {
            changed = 0;
            sel = __modsi3(sel + 2, 2);
        }}
        if (gKeyState & 1) {
            Audio_PlayCue(112);
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            sel = 1;
            break;
        }
        UiMenu_PositionCursor(104, (sel << 4) + 70);
        pad = &gKeysRepeat;
        if (*pad & 64) {
            {
            register s32 one asm("r2") = 1; /* FAKEMATCH: sets the flag through r2 */
            register s32 cue asm("r0") = 111; /* FAKEMATCH: loads the cue before the step */
            sel -= 1;
            asm volatile("mov %0, %1" : "=h"(changed) : "l"(one)); /* FAKEMATCH: sets the flag through r2 */
            Audio_PlayCue(cue);
            }
        }
        if (*pad & 128) {
            sel += 1;
            changed = 1;
            Audio_PlayCue(111);
        }
        WaitFrames(1);
    }
    if (GameFlag_IsSet(0x150) != 0) {
        sel = 1;
    }
    UiWork_FinalizeFar(win, 1);
    return sel;
}

extern u8 MsgEquipThisItem[];
void ItemMenu_DrawEquipPreview(s32 owner, s32 item, s32 mode, s32 target);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
s32 Inventory_EquipFar(s32 owner, s32 item);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void Owner_RecalculateStatsFar(s32 owner);
void Owner_RefreshClassActionsFar(s32 owner);

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: a direct call loads the size before the copy routine and
       the two blocks, where the reference loads it last. */
    return copy(dst, src, size);
}

/* Item menu: equip the item on its target for a preview, ask whether to
   keep it, and put the target's saved state back unless the answer is yes.
   Returns the chosen row, 1 when declined, cancelled or not equippable. */
s32 Unnamed_080a5388(void)
{
    s32 sel = 0;
    s32 changed = 1;
    struct InventoryMenuState *menu = gMenuWork;
    void *state = (void *)Owner_GetStateFar(menu->target_owner);
    void *saved;
    s32 win;
    s32 item;
    s32 owner;

    item = menu->equip_item;
    owner = menu->target_owner;
    ItemMenu_DrawEquipPreview(owner, item, 0, owner);
    saved = Runtime_BumpAllocate(0x14c);
    CopyWords((WordCopyFn)Iwram_CopyWords, saved, state, 0x14c);
    win = menu->message_window;
    if ((u32)(Inventory_EquipFar(menu->target_owner, menu->equip_item) + 2) <= 1) {
        sel = 1;
    } else {
        /* The Japanese menu stacks the two answers at the right; the
           international menus set them side by side under the question. */
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes, (void *)win, 24, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, (void *)win, 72, 24);
        UiWindow_ClearInteriorTilesFar(win, 16, 16, 96, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, (void *)win, 0, 16);
        UiMenu_SlideCursor(110, 32);
#else
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes, (void *)win, 96, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, (void *)win, 96, 16);
        UiWindow_ClearInteriorTilesFar(win, 16, 16, 96, 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, (void *)win, 8, 16);
        UiMenu_SlideCursor(184, 5);
#endif
        for (;;) {
            if (GameFlag_IsSet(0x150))
                break;
            if (changed) {
                changed = 0;
                sel = __modsi3(sel + 2, 2);
            }
            if (gKeyState & 1) {
                Audio_PlayCue(175);
                break;
            }
            if (gKeyState & 2) {
                Audio_PlayCue(113);
                sel = 1;
                break;
            }
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(sel * 48 + 110, 32);
            if (gKeysRepeat & 32) {
#else
            UiMenu_PositionCursor(184, (sel << 4) + 5);
            if (gKeysRepeat & 64) {
#endif
                sel--;
                changed = 1;
                Audio_PlayCue(111);
            }
#if EDITION_INTERNATIONAL
            if (gKeysRepeat & 16) {
#else
            if (gKeysRepeat & 128) {
#endif
                sel++;
                changed = 1;
                Audio_PlayCue(111);
            }
            WaitFrames(1);
        }
    }
    if (GameFlag_IsSet(0x150))
        sel = 1;
    if (sel == 1)
        CopyWords((WordCopyFn)Iwram_CopyWords, state, saved, 0x14c);
    Runtime_BumpFree(saved);
    Owner_RecalculateStatsFar(menu->target_owner);
    Owner_RefreshClassActionsFar(menu->target_owner);
    return sel;
}
