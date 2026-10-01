#include "EDITION.H"
#include "INVENTORY_MENU.H"
/* Item menu: ask whether to drop the item and return the chosen row (1 when cancelled). */
#include "TYPES.H"

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
