#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "btlsys.h"
#include "inventory_menu.h"
#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "item.h"
#include "text/msg_ids.h"
s32 GameFlag_TestFar(s32 flag);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

extern void UiText_DrawCharacterAtOffsetFar(s32, void *, s32, s32);

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
	s32 win;
	s32 slot;
	s32 text;
	s32 label;
	s32 rows = 2;
	s32 sel;
	/* FAKEMATCH: the native keeps the flag in r8. Removing this binding
	 * alone exchanges the flag and window registers (48 instructions differ). */
	register s32 changed asm("r8");

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
	/* FAKEMATCH: explicit check/update flow keeps the native loop
	 * rotation. A for, while or do loop with the cancelling flag copy
	 * below moves the cancel-key block (28 instructions differ). */
	goto check_input;

update_cursor:
	UiMenu_PositionCursor(104, (sel << 4) + 70);
	/* FAKEMATCH: needed to match: the key-repeat pointer shares text's variable; as a pointer of its own it ranks below sel and the two swap r5 and r6 (13 of 134 rows differ) */
	text = (s32)&gKeysRepeat;
	if (*(volatile s32 *)text & 64)
	{
		/* FAKEMATCH: the native sets the flag through r2. Removing this
		 * binding chooses r3 instead (two instruction rows differ). */
		register s32 one asm("r2") = 1;
		/* FAKEMATCH: the redundant conditional store ends CSE before
		 * the cursor update, keeping the one load ahead of sel's
		 * decrement. Without it, two rows swap; the conditional store
		 * disappears, preserving all 128 instructions in every edition. */
		if (changed)
		{
			changed = one;
		}
		sel -= 1;
		changed = one;
		AudioCommand_PlayFar(111);
	}
	if (*(volatile s32 *)text & 128)
	{
		sel += 1;
		changed = 1;
		AudioCommand_PlayFar(111);
	}
	WaitFrames(1);
check_input:
	if (GameFlag_TestFar(0x150) != 0)
	{
		goto finish;
	}
	{
		/* FAKEMATCH: the native tests this flag copy through r2. Removing
		 * its binding adds an ip copy and changes the test (19 instructions differ). */
		register s32 c asm("r2") = changed;
		/* FAKEMATCH: the cancelling assignments end the native CSE
		 * block before the flag test. Both arms disappear, retaining
		 * the r2 copy; a bare removal tests r8 through r3 instead
		 * (two instruction rows differ). sel is stable here. */
		if (sel)
			c = 0;
		if (sel)
			c = changed;
		if (c)
		{
			changed = 0;
			sel = (sel + rows) % rows;
		}
	}
	if (gKeyState & KEY_A)
	{
		AudioCommand_PlayFar(112);
		goto finish;
	}
	if (gKeyState & KEY_B)
	{
		AudioCommand_PlayFar(113);
		sel = 1;
		goto finish;
	}
	goto update_cursor;

finish:
	if (GameFlag_TestFar(0x150) != 0)
	{
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
    MemFree(saved);
    UnitCalcFar(menu->pane_owner[1]);
    Owner_RefreshClassActionsFar(menu->pane_owner[1]);
    return sel;
}
