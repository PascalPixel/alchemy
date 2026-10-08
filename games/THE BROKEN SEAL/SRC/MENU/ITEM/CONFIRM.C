#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "EDITION.H"
#include "BTLSYS.H"
#include "INVENTORY_MENU.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "ITEM.H"
#include "text/MSG_IDS.H"
s32 GameFlag_TestFar(s32 flag);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

extern void UiText_DrawCharacterAtOffsetFar(s32, void *, s32, s32);

extern volatile s32 gKeysRepeat;
extern volatile s32 gKeyState;
s32 __modsi3(s32, s32);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
void AudioCommand_PlayFar(s32 cue);

void ItemMenu_DrawItemHead(void)
{
    struct InventoryMenuState *menu = gMenuWork;

    Resource_LoadByModeIntoSlotFar(
        2, menu->selected_items[0], (u8)menu->selected_item_icon->index, 0);
    menu->selected_item_icon->active = 1;
    menu->selected_item_icon->x = 112;
    menu->selected_item_icon->y = 8;
    UiIcon_PrepareObject(menu->selected_item_icon);
    UiText_DrawStringAtOffsetFar(
        UnitGetFar(menu->pane_owner[0])->name,
        (void *)menu->message_window,
        16,
        0);
    UiText_DrawCharacterAtOffsetFar(
        (menu->selected_items[0] & ITEM_ID_MASK) +
            (s32)MsgItemName,
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
    register s32 changed asm("r8"); /* FAKEMATCH: pins the flag to r8, where the native keeps it; with the r2 copy below unpinned it moves to r7 (86 of 141 rows differ) */

    win = UiWindow_CreateFar(13, 3, 17, 10, 2);
    slot = a0 & ITEM_ID_MASK;
    Item_GetDirectFar(slot);
    UiText_DrawCharacterAtOffsetFar(slot + (s32)((u8 *)MsgItemName), win, 24, 0);
    text = (s32)MsgConfirmDrop;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(text, win, 0, 16);
#else
    UiText_DrawCharacterAtOffsetFar(text, win, 8, 16);
#endif
    text++;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(text, win, 0, 24);
#else
    UiText_DrawCharacterAtOffsetFar(text, win, 8, 24);
#endif
    label = (s32)MsgYes;
    UiText_DrawCharacterAtOffsetFar(label, win, 24, 40);
    label++;
    UiText_DrawCharacterAtOffsetFar(label, win, 24, 56);
    sel = 1;
    changed = 1;
    UiMenu_SlideCursor(104, 86);
    for (;;) {
        if (GameFlag_TestFar(0x150) != 0) {
            break;
        }
        {
        register s32 c asm("r2") = changed; /* FAKEMATCH: tests the flag through r2 */
        /* FAKEMATCH: empty asm barrier on the r2 copy, so cse does not test r8 directly, whose reload takes r3 (2 of 134 rows differ) */
        asm("" : "+l"(c));
        if (c) {
            changed = 0;
            sel = __modsi3(sel + 2, 2);
        }}
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            sel = 1;
            break;
        }
        UiMenu_PositionCursor(104, (sel << 4) + 70);
        pad = &gKeysRepeat;
        if (*pad & 64) {
            {
            register s32 one asm("r2") = 1; /* FAKEMATCH: sets the flag through r2 */
            /* FAKEMATCH: empty asm barrier on the r2 one, so cse does not set r8 from the constant, whose reload takes r3 (2 of 134 rows differ) */
            asm("" : "+l"(one));
            sel -= 1;
            changed = one;
            AudioCommand_PlayFar(111);
            }
        }
        if (*pad & 128) {
            sel += 1;
            changed = 1;
            AudioCommand_PlayFar(111);
        }
        WaitFrames(1);
    }
    if (GameFlag_TestFar(0x150) != 0) {
        sel = 1;
    }
    UiWork_FinalizeFar(win, 1);
    return sel;
}

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: a direct call loads the size before the copy routine and the two blocks, where the reference loads it last. */
    return copy(dst, src, size);
}

s32 ItemMenu_ConfirmEquip(s32 unused)
{
    s32 sel = 0;
    s32 changed = 1;
    struct InventoryMenuState *menu = gMenuWork;
    struct BtlUnit *state = UnitGetFar(menu->pane_owner[1]);
    struct BtlUnit *saved;
    s32 win;
    s32 item;
    s32 owner;

    item = menu->selected_slots[1];
    owner = menu->pane_owner[1];
    ItemMenu_DrawEquipPreview(owner, item, 0, owner);
    saved = MemAlloc(BATTLE_UNIT_SIZE);
    CopyWords((WordCopyFn)Iwram_CopyWords, saved, state, BATTLE_UNIT_SIZE);
    win = (s32)menu->message_window;
    if ((u32)(Inventory_EquipFar(menu->pane_owner[1], menu->selected_slots[1]) + 2) <= 1) {
        sel = 1;
    } else {
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
            if (GameFlag_TestFar(0x150))
                break;
            if (changed) {
                changed = 0;
                sel = __modsi3(sel + 2, 2);
            }
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(175);
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(113);
                sel = 1;
                break;
            }
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(sel * 48 + 110, 32);
            if (gKeysRepeat & KEY_LEFT) {
#else
            UiMenu_PositionCursor(184, (sel << 4) + 5);
            if (gKeysRepeat & KEY_UP) {
#endif
                sel--;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
#if EDITION_INTERNATIONAL
            if (gKeysRepeat & KEY_RIGHT) {
#else
            if (gKeysRepeat & KEY_DOWN) {
#endif
                sel++;
                changed = 1;
                AudioCommand_PlayFar(111);
            }
            WaitFrames(1);
        }
    }
    if (GameFlag_TestFar(0x150))
        sel = 1;
    if (sel == 1)
        CopyWords((WordCopyFn)Iwram_CopyWords, state, saved, BATTLE_UNIT_SIZE);
    Runtime_BumpFree(saved);
    UnitCalcFar(menu->pane_owner[1]);
    Owner_RefreshClassActionsFar(menu->pane_owner[1]);
    return sel;
}
