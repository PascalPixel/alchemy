#include "RUNTIME_MEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "ITEM.H"
#include "BATTLE_RUNTIME.H"
#include "INVENTORY_MENU.H"
#include "GLOBAL_CELLS.H"
/* Where the shorter modal lines start: the Japanese ones a column or three
   further right. */
#if EDITION_INTERNATIONAL
#define MODAL_SHORT_X   14
#define MODAL_REMOVED_X 14
#else
#define MODAL_SHORT_X   15
#define MODAL_REMOVED_X 17
#endif

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

/* The resident word copier, addressed from the start of the IWRAM runtime
   bank, whose first routine is the interrupt handler. A symbol plus an
   offset keeps the call in a register as the pool entry's schedule shows;
   named alone, GCC emits a Thumb bl that cannot reach IWRAM. */
#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls share the 332-byte size in r5 and move the copy routine to r6. */
    return copy(dst, src, size);
}

extern char MsgWhoseItem;
extern char MsgWhichItem;
extern char MsgUseOnWhom;
extern char MsgGiveToWhom;
extern char MsgSwapForWhat;
extern char MsgEquippedIt;
extern char MsgDroppedIt;
extern char MsgGiven;
extern char MsgRemoved;
extern char MsgTraded;
extern char MsgCannotRemoveItem;
extern char MsgCannotTrade;
extern char MsgCannotHoldMore;
extern char MsgItemUseResult;

void WaitFrames(s32 frames);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 unused, s32 x, s32 y, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void Audio_PlayCue(s32 cue);
void Owner_RecalculateStatsFar(s32 owner);
s32 Inventory_AddItemFar(s32 owner, s32 item);
s32 Inventory_EquipFar(s32 owner, s32 slot);
s32 Inventory_RemoveFar(s32 owner, s32 slot);
s32 GameFlag_TestFar(s32 flag);
void Func_08077240(s32 item, s32 mode);
void Func_080772c0(s32 owner);
s32 BattleFx_HasTriggerFar(s32 item);
void Event_ClearInvalidPackedValuesFar(void);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 unused, s32 style);
void InventoryMenu_ShowModalMessage(s32 message, s32 acknowledgement_mode, s32 window_mode);
void UiText_DrawWorkValueWithLabel(s32 window);
s32 Func_080a414c(void);
void UiIcon_PrepareObject(struct RenderOutput *icon);

enum ItemMenuStep {
    ITEM_STEP_OWNER = 0,
    ITEM_STEP_ITEM = 1,
    ITEM_STEP_USE = 2,
    ITEM_STEP_EQUIP = 3,
    ITEM_STEP_GIVE = 4,
    ITEM_STEP_DROP = 5,
    ITEM_STEP_TARGET = 6,
    ITEM_STEP_TRADE = 7,
    ITEM_STEP_IDLE = 8,
    ITEM_STEP_COMMAND = 9,
    ITEM_STEP_INSPECT = 10,
    ITEM_STEP_REMOVE = 11,
    ITEM_STEP_QUANTITY = 12
};

/*
 * The item menu command loop.  Each pass of the state machine runs one
 * screen: 0 picks the owner, 1 the item, 9 the command, and the handlers
 * use (2), equip (3), give (6, then 4 or the trade in 7), drop (5), unequip
 * (11) and inspect (10).  The loop ends when a handler sets done or flag
 * 0x150 is raised.  Using an item outside the menu fills the three out
 * parameters and returns 1; cancelling the owner returns -1, as does flag
 * 0x150; anything else returns 0.
 *
 * Every handler sets the next state inside the branch that leaves it, and
 * the trade backs both inventories up through the resident word copier so a
 * failed swap can restore them.
 */
s32 ItemMenu_RunCommands(s32 *owner_out, s32 *target_out, s32 *item_out)
{
    s32 done;
#if !EDITION_INTERNATIONAL
    s32 owner;
    s32 chosen;
#endif
    s32 sel;
    s32 ret;
    struct BattleUnit *source;
    struct BattleUnit *target;
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;
    struct BattleUnit *source_copy;
    struct BattleUnit *target_copy;
    u32 state;
    s32 command;
    s32 result;
    s32 qty;
    s32 amount;
    s32 aborted;
    s32 work;
    s32 n;
    u8 moved_source;
    u8 moved_target;
    s8 stack;

    done = 0;
#if !EDITION_INTERNATIONAL
    owner = 0;
    chosen = 0;
#endif
    sel = 0;
    ret = 0;
    state = ITEM_STEP_OWNER;
    menu = gMenuWork;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case ITEM_STEP_OWNER:
            menu->selected_slots[0] = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->active = 13;
            ItemMenu_DrawMsg(0, (s32)&MsgWhoseItem);
#if EDITION_INTERNATIONAL
            RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
            UiText_DrawWorkValueWithLabel((s32)menu->info_window);
            command = ItemMenu_PrepOwner(0);
            if (command == -1) {
                sel = command = 0;
                ret = -1;
                done = 1;
            }
#else
            /* The Japanese loop keeps the owner, item and target it chose
               and hands those out. */
            UiText_DrawWorkValueWithLabel((s32)menu->info_window);
            owner = ItemMenu_PrepOwner(0);
            if (owner == -1) {
                chosen = sel = owner = 0;
                ret = -1;
                done = 1;
            }
#endif
            RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
            ItemMenu_HideAllIcons();
            state = ITEM_STEP_ITEM;
            break;

        case ITEM_STEP_ITEM:
            if (ItemMenu_Count(menu->pane_owner[0]) == 0) {
                state = ITEM_STEP_OWNER;
                break;
            }
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->active = 13;
            menu->pane_icons[0]->active = 1;
            ItemMenu_DrawMsg(0, (s32)&MsgWhichItem);
            sel = ItemMenu_RunList(0);
            state = ITEM_STEP_OWNER;
            if (sel == -1) {
                break;
            }
            menu->list_mode = 0xff;
            state = ITEM_STEP_COMMAND;
            break;

        case ITEM_STEP_COMMAND:
            command = Func_080a414c();
            if (command == -1) {
                state = ITEM_STEP_ITEM;
                menu->completion_flag = 1;
            }
            if (command == ITEM_COMMAND_USE) {
                if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0) {
                    done = 1;
#if EDITION_INTERNATIONAL
                    *owner_out = menu->pane_owner[0];
                    *target_out = command;
                    *item_out = menu->selected_items[0] & ITEM_ID_MASK;
#else
                    *owner_out = owner;
                    *target_out = chosen;
                    *item_out = sel;
#endif
                    ret = 1;
                    break;
                }
                result = Item_ClassifyUseMode(
                    menu->pane_owner[0], menu->selected_items[0]);
                if (result == 1) {
                    state = ITEM_STEP_USE;
                }
                if (result == 2) {
                    ItemMenu_Use();
                    RenderOutput_ClearListFar((s32)menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#endif
                    menu->pane_icons[0]->active = 13;
                    menu->item_count = ItemMenu_Collect(
                        Owner_GetStateFar(menu->pane_owner[0]), menu->items, 0);
                    ItemMenu_DrawIcons(menu->items, 0);
                    state = ITEM_STEP_OWNER;
                }
                if (result == -1 || result == 0) {
                    done = 1;
                    *owner_out = menu->pane_owner[0];
                    *target_out = menu->pane_owner[1];
                    *item_out = menu->selected_items[0] & ITEM_ID_MASK;
                    ret = 1;
                }
            }
            if (command == ITEM_COMMAND_EQUIP) {
                state = ITEM_STEP_EQUIP;
            }
            if (command == ITEM_COMMAND_GIVE) {
                state = ITEM_STEP_TARGET;
            }
            if (command == ITEM_COMMAND_DROP) {
                state = ITEM_STEP_DROP;
            }
            if (command == ITEM_COMMAND_REMOVE) {
                state = ITEM_STEP_REMOVE;
            }
            if (command == ITEM_COMMAND_INSPECT) {
                state = ITEM_STEP_INSPECT;
            }
            break;

        case ITEM_STEP_USE:
            ItemMenu_HideAllIcons();
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            RenderOutput_RedrawSavedRectFar((s32)menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)&MsgUseOnWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
            if (ItemMenu_SelectTarget(0) != -1) {
#else
            chosen = ItemMenu_SelectTarget(0);
            if (chosen != -1) {
#endif
                command = 0;
                if (ItemMenu_IsSpecial(
                        menu->selected_items[0] & ITEM_ID_MASK) != 0) {
                    command = 8;
                }
                result = ItemMenu_Use();
                Menu_DrawOwnerStatusPanel(
                    (s32)menu->status_window, menu->pane_owner[1], 0, command);
                if (result != -1) {
                    RenderOutput_ClearListFar((s32)menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#endif
                    menu->pane_icons[0]->active = 13;
                    ItemMenu_TryBreak();
                    state = ITEM_STEP_ITEM;
                }
                menu->item_count = ItemMenu_Collect(
                    Owner_GetStateFar(menu->pane_owner[0]), menu->items, 0);
                ItemMenu_DrawIcons(menu->items, 0);
                menu->completion_flag = 1;
            } else {
                state = ITEM_STEP_COMMAND;
            }
            break;

        case ITEM_STEP_TARGET:
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            RenderOutput_RedrawSavedRectFar((s32)menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)&MsgGiveToWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
            n = ItemMenu_SelectTarget(1);
            state = ITEM_STEP_GIVE;
            if (n == -1) {
#else
            chosen = ItemMenu_SelectTarget(1);
            state = ITEM_STEP_GIVE;
            if (chosen == -1) {
#endif
                ItemMenu_DrawEquipPreview(
                    menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
                state = ITEM_STEP_COMMAND;
            }
            break;

        case ITEM_STEP_DROP:
            ItemMenu_HideAllIcons();
            item = Item_Get(menu->selected_items[0] & ITEM_ID_MASK);
            result = 0;
            if ((item->flags & ITEM_STACKABLE) != 0) {
                qty = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
                if (qty > 1) {
                    ItemMenu_DrawItemHead();
                    result = ItemMenu_SelectGiveQuantity(0, qty, 1);
                }
            }
            if (result == -1) {
                state = ITEM_STEP_COMMAND;
                break;
            }
            menu->pane_owner[1] = 0;
            Resource_LoadByModeIntoSlotFar(
                2,
                (menu->selected_items[0] & ITEM_ID_MASK) | (result << INVENTORY_QUANTITY_SHIFT),
                (u8)menu->selected_item_icon->index,
                0);
            menu->selected_item_icon->active = 1;
            menu->selected_item_icon->x = 120;
            menu->selected_item_icon->y = 28;
            UiIcon_PrepareObject(menu->selected_item_icon);
            UiWindow_ClearInteriorTilesFar((s32)menu->list_window, 0, 72, 120, 96);
            RenderOutput_RedrawSavedRectFar((s32)menu->message_window);
            if (ItemMenu_ConfirmDrop(sel) == 0) {
#if EDITION_INTERNATIONAL
                command = menu->pane_owner[0];
                Owner_GetStateFar(command);
                for (work = 0; work < result + 1; work++) {
                    Inventory_RemoveFar(command, menu->selected_slots[0]);
                    Func_08077240(menu->selected_items[0] & ITEM_ID_MASK, 1);
                }
                Owner_RecalculateStatsFar(command);
#else
                owner = menu->pane_owner[0];
                Owner_GetStateFar(owner);
                for (work = 0; work < result + 1; work++) {
                    Inventory_RemoveFar(owner, menu->selected_slots[0]);
                    Func_08077240(menu->selected_items[0] & ITEM_ID_MASK, 1);
                }
                Owner_RecalculateStatsFar(owner);
#endif
                ItemMenu_SetItemWin3();
                ItemMenu_RefreshOwner(menu->pane_owner[0], 0);
                menu->selected_item_icon->active = 13;
                menu->pane_icons[0]->active = 13;
                WaitFrames(1);
                RenderOutput_ClearListFar((s32)menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&MsgDroppedIt, MODAL_SHORT_X, 13);
                menu->completion_flag = 1;
                state = ITEM_STEP_ITEM;
            } else {
                state = ITEM_STEP_COMMAND;
            }
            Owner_RecalculateStatsFar(menu->pane_owner[0]);
            menu->selected_item_icon->active = 13;
            Event_ClearInvalidPackedValuesFar();
            break;

        case ITEM_STEP_GIVE:
            aborted = 0;
            item = Item_Get(menu->selected_items[0] & ITEM_ID_MASK);
            if ((item->flags & ITEM_STACKABLE) != 0) {
                qty = InventoryMenu_GetItemQuantity(
                    menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
                if (qty == 30) {
                    aborted = 1;
                }
                if (ItemMenu_Count(menu->pane_owner[1]) == 15 &&
                    qty == 0) {
                    state = ITEM_STEP_TRADE;
                    break;
                }
                stack = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
                if (aborted == 0) {
                    /* FAKEMATCH: the loop only places a label between the
                       count and its sign-extended use. */
                    do {
                        amount = stack;
                    } while (0);
                    if (qty + amount > 30) {
                        amount = 30 - qty;
                    }
                    if (stack > 1) {
                        result = ItemMenu_SelectGiveQuantity(0, amount, 0);
                    } else {
                        result = 0;
                    }
                    if (result == -1) {
                        state = ITEM_STEP_TARGET;
                        break;
                    }
                    for (command = 0; command < result + 1; command++) {
                        qty = Inventory_AddItemFar(
                            menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                        if (qty != -1) {
                            Inventory_RemoveFar(
                                menu->pane_owner[0], menu->selected_slots[0]);
                            menu->selected_slots[1] = qty;
                        } else {
                            aborted = 1;
                        }
                    }
                }
            } else {
                result = Inventory_AddItemFar(
                    menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                if (result == -1) {
                    state = ITEM_STEP_TRADE;
                    break;
                }
                menu->selected_slots[1] = result;
                result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
                if (result == -1) {
                    aborted = 1;
                }
            }
            Owner_RecalculateStatsFar(menu->pane_owner[0]);
            Owner_RecalculateStatsFar(menu->pane_owner[1]);
            Func_080772c0(menu->pane_owner[0]);
            Func_080772c0(menu->pane_owner[1]);
            result = 1;
            if (aborted == 0) {
                menu->pane_owner[0] = menu->pane_owner[1];
                menu->selected_items[0] &= ITEM_ID_MASK;
#if EDITION_INTERNATIONAL
                ItemMenu_SetMsgWin6();
#endif
                RenderOutput_RedrawSavedRectFar((s32)menu->message_window);
                ItemMenu_DrawItemHead();
                result = Unnamed_080a5388(0);
            }
            if (GameFlag_TestFar(0x150) != 0) {
                break;
            }
            ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
            menu->pane_icons[0]->active = 13;
            WaitFrames(1);
            if (aborted == 1) {
                RenderOutput_ClearListFar((s32)menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&MsgCannotHoldMore, 15, 14);
            } else {
                RenderOutput_ClearListFar((s32)menu->info_window);
                if (result == 1) {
                    InventoryMenu_ShowModalMessage(
                        (s32)&MsgGiven, 15, 14);
                } else {
                    ItemMenu_DrawEquipPreview(
                        menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
                    InventoryMenu_ShowModalMessage(
                        (s32)&MsgEquippedIt, 15, 14);
                    item = Item_Get(menu->selected_items[0]);
                    if ((item->flags & ITEM_CURSED) != 0) {
                        Audio_PlayCue(0x67);
                        RenderOutput_ClearListFar((s32)menu->info_window);
                        InventoryMenu_ShowModalMessage(
                            (s32)&MsgEquippedIt + 7, MODAL_SHORT_X, 14);
                    }
                }
            }
            Event_ClearInvalidPackedValuesFar();
            state = ITEM_STEP_OWNER;
            break;

        case ITEM_STEP_TRADE:
            aborted = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            ItemMenu_DrawMsg(0, (s32)&MsgSwapForWhat);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            /* In Spanish and Italian the swap list's choice does not
               replace the selection a later drop confirms. */
            if (ItemMenu_RunList(1) == -1) {
#else
            sel = ItemMenu_RunList(1);
            if (sel == -1) {
#endif
                state = ITEM_STEP_TARGET;
                break;
            }
            source = Owner_GetStateFar(menu->pane_owner[0]);
            target = Owner_GetStateFar(menu->pane_owner[1]);
            source_copy = Runtime_BumpAllocate(BATTLE_UNIT_SIZE);
            target_copy = Runtime_BumpAllocate(BATTLE_UNIT_SIZE);
            CopyWords(IWRAM_COPY_WORDS, source_copy, source, BATTLE_UNIT_SIZE);
            CopyWords(IWRAM_COPY_WORDS, target_copy, target, BATTLE_UNIT_SIZE);
            moved_source = 0;
            while (moved_source <= 29) {
                result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
                if (result == 2) {
                    break;
                }
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                moved_source++;
            }
            moved_source++;
            moved_target = 0;
            while (moved_target <= 29) {
                if ((menu->selected_items[1] & INVENTORY_EQUIPPED) != 0) {
                    item = Item_Get(menu->selected_items[1] & ITEM_ID_MASK);
                    if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
                        aborted = 1;
                    }
                }
                result = Inventory_RemoveFar(
                    menu->pane_owner[1], menu->selected_slots[1]);
                if (result == 2) {
                    break;
                }
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                moved_target++;
            }
            moved_target++;
            while (moved_source != 0) {
                result = Inventory_AddItemFar(
                    menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slots[1] = result;
                moved_source--;
            }
            while (moved_target != 0) {
                result = Inventory_AddItemFar(
                    menu->pane_owner[0], menu->selected_items[1] & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slots[0] = result;
                moved_target--;
            }
            WaitFrames(1);
            if (aborted == 1) {
                CopyWords(IWRAM_COPY_WORDS, source, source_copy, BATTLE_UNIT_SIZE);
                CopyWords(IWRAM_COPY_WORDS, target, target_copy, BATTLE_UNIT_SIZE);
                RenderOutput_ClearListFar((s32)menu->info_window);
#if defined(TBS_EDITION_FR)
                /* The longer French line starts further left, and the status
                   window is redrawn after it. */
                InventoryMenu_ShowModalMessage((s32)&MsgCannotTrade, 11, 14);
                RenderOutput_RedrawSavedRectFar((s32)menu->status_window);
#else
                InventoryMenu_ShowModalMessage((s32)&MsgCannotTrade, 15, 14);
#endif
            } else {
                Owner_RecalculateStatsFar(menu->pane_owner[0]);
                Owner_RecalculateStatsFar(menu->pane_owner[1]);
                Func_080772c0(menu->pane_owner[0]);
                Func_080772c0(menu->pane_owner[1]);
                ItemMenu_SetMsgWin5();
#if EDITION_INTERNATIONAL
                ItemMenu_SetMsgWin6();
                ItemMenu_HidePageIcons();
#endif
                RenderOutput_RedrawSavedRectFar((s32)menu->message_window);
                menu->pane_owner[0] = menu->pane_owner[1];
                menu->selected_items[0] &= ITEM_ID_MASK;
                ItemMenu_DrawItemHead();
                result = Unnamed_080a5388(0);
                if (GameFlag_TestFar(0x150) == 0) {
                    RenderOutput_ClearListFar((s32)menu->info_window);
                    ItemMenu_SetItemWin5();
                    ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
                    if (result == 0) {
                        ItemMenu_DrawEquipPreview(
                            menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
                        InventoryMenu_ShowModalMessage(
                            (s32)&MsgEquippedIt, 15, 14);
                        item = Item_Get(menu->selected_items[0]);
                        if ((item->flags & ITEM_CURSED) != 0) {
                            Audio_PlayCue(0x67);
                            RenderOutput_ClearListFar((s32)menu->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&MsgEquippedIt + 7, MODAL_SHORT_X, 14);
                        }
                    } else {
#if defined(TBS_EDITION_IT)
                        /* The longer Italian line starts further left. */
                        InventoryMenu_ShowModalMessage(
                            (s32)&MsgTraded, 13, 14);
#else
                        InventoryMenu_ShowModalMessage(
                            (s32)&MsgTraded, 15, 14);
#endif
                    }
                }
            }
            Runtime_BumpFree(target_copy);
            Runtime_BumpFree(source_copy);
            Event_ClearInvalidPackedValuesFar();
            state = ITEM_STEP_OWNER;
            break;

        case ITEM_STEP_EQUIP:
            result = Inventory_EquipFar(menu->pane_owner[0], menu->selected_slots[0]);
            if (result == -1) {
                state = ITEM_STEP_ITEM;
                break;
            }
            if (result == -2) {
                RenderOutput_ClearListFar((s32)menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&MsgCannotRemoveItem, 0, -1);
#if !EDITION_INTERNATIONAL
                RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#endif
                state = ITEM_STEP_ITEM;
                break;
            }
            Owner_RecalculateStatsFar(menu->pane_owner[0]);
            Func_080772c0(menu->pane_owner[0]);
            menu->pane_icons[0]->active = 13;
            menu->item_count = ItemMenu_Collect(
                Owner_GetStateFar(menu->pane_owner[0]), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            ItemMenu_DrawEquipPreview(
                menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            RenderOutput_ClearListFar((s32)menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&MsgEquippedIt, 15, 8);
            item = Item_Get(menu->selected_items[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
                Audio_PlayCue(0x67);
                RenderOutput_ClearListFar((s32)menu->info_window);
                InventoryMenu_ShowModalMessage(
                    (s32)&MsgEquippedIt + 7, MODAL_SHORT_X, 8);
            }
            state = ITEM_STEP_ITEM;
            break;

        case ITEM_STEP_REMOVE:
            Owner_GetStateFar(menu->pane_owner[0])->inventory[menu->selected_slots[0]]
                &= 0xfdff;
            Owner_RecalculateStatsFar(menu->pane_owner[0]);
            Func_080772c0(menu->pane_owner[0]);
            menu->pane_icons[0]->active = 13;
            menu->item_count = ItemMenu_Collect(
                Owner_GetStateFar(menu->pane_owner[0]), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            menu->equip_preview = 1;
            ItemMenu_DrawEquipPreview(
                menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            menu->equip_preview = 0;
            RenderOutput_ClearListFar((s32)menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&MsgRemoved, MODAL_REMOVED_X, 8);
            Event_ClearInvalidPackedValuesFar();
            state = ITEM_STEP_ITEM;
            break;

        case ITEM_STEP_INSPECT:
            menu->pane_icons[0]->active = 13;
            Menu_SelectQuantity(menu->selected_items[0]);
            RenderOutput_RedrawSavedRectFar((s32)menu->status_window);
            ItemMenu_DrawEquipPreview(
                menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
            menu->pane_icons[0]->active = 1;
            state = ITEM_STEP_COMMAND;
            break;

        case ITEM_STEP_QUANTITY:
            result = ItemMenu_SelectGiveQuantity(0, 30, 0);
            if (result != -1) {
                qty = result;
            }
            state = ITEM_STEP_ITEM;
            break;

        case ITEM_STEP_IDLE:
            break;

        default:
            done = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150) != 0) {
        ret = -1;
    }
    return ret;
}
