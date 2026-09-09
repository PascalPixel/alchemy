#include "types.h"
#include "inventory_menu.h"
#include "item.h"
#include "owner_state.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define InventoryMenu_RunItemCommand Func_080a2680

/* Message-table cells reached through the shared Value_<id> address symbols. */
extern char Value_00000ad8;
extern char Value_00000ad9;
extern char Value_00000adb;
extern char Value_00000adc;
extern char Value_00000add;
extern char Value_00000b7c;
extern char Value_00000b7d;
extern char Value_00000b7f;
extern char Value_00000b80;
extern char Value_00000b81;
extern char Value_00000b82;
extern char Value_00000b84;
extern char Value_00000b85;
extern char Value_00000bef;

void Func_080030f8(s32 frames);
void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);
void Func_080072f8(void *dst, const void *src, s32 size);
void Func_080072fc(void *dst, const void *src, s32 size);
void Func_08015068(s32 window, s32 unused, s32 x, s32 y, s32 height);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_08015270(s32 window);
void Func_08015278(s32 window);
void Func_080f9010(s32 cue);
void Func_08077010(s32 owner);
s32 Func_08077028(s32 owner, s32 item);
s32 Func_08077050(s32 owner, s32 slot);
s32 Func_08077058(s32 owner, s32 slot);
s32 Func_080770c0(s32 flag);
void Func_08077240(s32 item, s32 mode);
void Func_080772c0(s32 owner);
s32 Func_0808a490(s32 item);
void Func_0808a548(void);
void Func_080a112c(s32 window, s32 owner, s32 unused, s32 style);
void Func_080a1d08(s32 message, s32 acknowledgement_mode, s32 window_mode);
void Func_080a23c0(s32 window);
s32 Func_080a38d0(s32 mode);
void Func_080a3ef0(s32 owner, s32 slot, s32 unused);
s32 Func_080a414c(void);
s32 Func_080a46b4(s32 owner, s32 item);
void Func_080a4800(s32 item);
s32 Func_080a4f08(s32 unused, s32 limit, s32 mode);
s32 Func_080a524c(s32 index);
s32 Func_080a5388(s32 mode);
s32 Func_080a5788(s32 mode);

#define InventoryMenu_GetCommandChoice  Func_080a414c
#define InventoryMenu_SelectPartyMember Func_080a5788
#define Item_ClassifyUseMode            Func_080a46b4
#define UiText_DrawWorkValueWithLabel   Func_080a23c0

/*
 * Item command driver for the field inventory menu.
 *
 * One frame-driven state machine: `state` selects the current screen (item
 * list, command list, target picker, give/trade/drop/remove handlers) and the
 * loop runs until a handler sets `done` or the abort flag 0x150 is raised.
 * The three out parameters carry a "use this item outside the menu" request
 * back to the caller, and the return value is 1 when such a request was made,
 * -1 when flag 0x150 aborted the menu, 0 otherwise.
 *
 * Uncertain: the roles of the two menu cells at 0x25c and 0x25d (written but
 * never read here) and of the two window handles at 0x24 and 0x34; both are
 * still spelled as raw offsets. The named state numbers are the reference
 * jump-table order, not recovered constants.
 */
s32 InventoryMenu_RunItemCommand(s32 *owner_out, s32 *target_out, s32 *item_out)
{
    s32 done;
    s32 sel;
    s32 ret;
    struct OwnerInventoryState *source;
    struct OwnerInventoryState *target;
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;
    void *source_copy;
    void *target_copy;
    u32 state;
    s32 command;
    s32 mode;
    s32 cnt;
    s32 owner;
    s32 qty;
    s32 amount;
    s32 picked;
    s32 swapped;
    s32 aborted;
    s32 work;
    s32 n;
    u8 moved_source;
    u8 moved_target;
    s8 stack;

    done = 0;
    sel = 0;
    ret = 0;
    state = 0;
    menu = Data_03001f2c;

    while (done == 0 && Func_080770c0(0x150) == 0) {
        switch (state) {
        case 0:
            menu->selected_slot = 0;
            InventoryMenu_SetMessageWindowRow3Bounds();
            InventoryMenu_SetItemWindowThreeRowBounds();
            menu->selected_item_icon->state = 13;
            InventoryMenu_DrawMessage(0, (s32)&Value_00000ad8);
            UiWindow_Commit(menu->info_window);
            UiText_DrawWorkValueWithLabel(menu->info_window);
            command = InventoryMenu_PrepareOwnerItems(0);
            if (command == -1) {
                ret = -1;
                sel = 0;
                done = 1;
            }
            UiWindow_Commit(menu->info_window);
            InventoryMenu_HideAllItemIcons();
            state = 1;
            break;

        case 1:
            if (InventoryMenu_CountItems(menu->item_owner) == 0) {
                state = 0;
                break;
            }
            InventoryMenu_SetMessageWindowRow3Bounds();
            InventoryMenu_SetItemWindowThreeRowBounds();
            menu->selected_item_icon->state = 13;
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 1;
            InventoryMenu_DrawMessage(0, (s32)&Value_00000ad9);
            sel = InventoryMenu_SelectPartyMember(0);
            state = 0;
            if (sel == -1) {
                break;
            }
            M2C_FIELD(menu, u8 *, 0x25d) = 0xff;
            state = 9;
            break;

        case 9:
            command = InventoryMenu_GetCommandChoice();
            if (command == -1) {
                state = 1;
                menu->completion_flag = 1;
            }
            if (command == 0) {
                if (Func_0808a490(menu->selected_item & 0x1ff) != 0) {
                    done = 1;
                    *owner_out = menu->item_owner;
                    *target_out = command;
                    *item_out = menu->selected_item & 0x1ff;
                    ret = 1;
                    break;
                }
                mode = Item_ClassifyUseMode(
                    menu->item_owner, menu->selected_item);
                if (mode == 1) {
                    state = 2;
                }
                if (mode == 2) {
                    InventoryMenu_UseSelectedItem();
                    Func_08015278(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state =
                        13;
                    menu->item_count = InventoryMenu_CollectItems(
                        OwnerState_GetFar(menu->item_owner), menu->items, 0);
                    InventoryMenu_DrawItemIcons(menu->items, 0);
                    state = 0;
                }
                if (mode == -1 || mode == 0) {
                    done = 1;
                    *owner_out = menu->item_owner;
                    *target_out = menu->target_owner;
                    *item_out = menu->selected_item & 0x1ff;
                    ret = 1;
                }
            }
            if (command == 1) {
                state = 3;
            }
            if (command == 3) {
                state = 6;
            }
            if (command == 5) {
                state = 5;
            }
            if (command == 4) {
                state = 11;
            }
            if (command == 2) {
                state = 10;
            }
            break;

        case 2:
            InventoryMenu_HideAllItemIcons();
            InventoryMenu_SetMessageWindowRow5Bounds();
            InventoryMenu_SetItemWindowFiveRowBounds();
            UiWindow_Commit(menu->message_window);
            InventoryMenu_DrawSelectedItemHeader();
            UiText_DrawAt(
                (s32)&Value_00000adb, menu->message_window, 16, 16);
            if (Func_080a38d0(0) != -1) {
                cnt = 0;
                if (InventoryMenu_IsSpecialItemRange(
                        menu->selected_item & 0x1ff) != 0) {
                    cnt = 8;
                }
                mode = InventoryMenu_UseSelectedItem();
                Func_080a112c(
                    M2C_FIELD(menu, s32 *, 0x24), menu->target_owner, 0, cnt);
                if (mode != -1) {
                    Func_08015278(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state =
                        13;
                    InventoryMenu_MaybeBreakSelectedItem();
                    state = 1;
                }
                menu->item_count = InventoryMenu_CollectItems(
                    OwnerState_GetFar(menu->item_owner), menu->items, 0);
                InventoryMenu_DrawItemIcons(menu->items, 0);
                menu->completion_flag = 1;
            } else {
                state = 9;
            }
            break;

        case 6:
            InventoryMenu_SetMessageWindowRow5Bounds();
            InventoryMenu_SetItemWindowFiveRowBounds();
            UiWindow_Commit(menu->message_window);
            InventoryMenu_DrawSelectedItemHeader();
            UiText_DrawAt(
                (s32)&Value_00000adc, menu->message_window, 16, 16);
            n = Func_080a38d0(1);
            state = 4;
            if (n == -1) {
                Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
                state = 9;
            }
            break;

        case 5:
            InventoryMenu_HideAllItemIcons();
            item = Item_Get(menu->selected_item & 0x1ff);
            cnt = 0;
            if ((item->flags & 16) != 0) {
                n = (menu->selected_item >> 11) + 1;
                if (n > 1) {
                    InventoryMenu_DrawSelectedItemHeader();
                    cnt = Func_080a4f08(0, n, 1);
                }
            }
            state = 9;
            if (cnt == -1) {
                break;
            }
            menu->target_owner = 0;
            InventoryMenu_DrawItemIcon(
                2,
                (menu->selected_item & 0x1ff) | (cnt << 11),
                menu->selected_item_icon->render_target,
                0);
            menu->selected_item_icon->state = 1;
            menu->selected_item_icon->x = 120;
            menu->selected_item_icon->y = 28;
            UiIcon_PrepareObject(menu->selected_item_icon);
            Func_08015068(M2C_FIELD(menu, s32 *, 0x34), 0, 72, 120, 96);
            UiWindow_Commit(menu->message_window);
            if (Func_080a524c(sel) == 0) {
                owner = menu->item_owner;
                OwnerState_GetFar(owner);
                for (work = cnt + 1; work > 0; work--) {
                    Func_08077058(owner, menu->selected_slot);
                    Func_08077240(menu->selected_item & 0x1ff, 1);
                }
                Func_08077010(owner);
                InventoryMenu_SetItemWindowThreeRowBounds();
                InventoryMenu_RefreshOwnerItems(menu->item_owner, 0);
                menu->selected_item_icon->state = 13;
                M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 13;
                WaitFrames(1);
                Func_08015278(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b7d, 14, 13);
                menu->completion_flag = 1;
                state = 1;
            } else {
                state = 9;
            }
            Func_08077010(menu->item_owner);
            menu->selected_item_icon->state = 13;
            Func_0808a548();
            break;

        case 4:
            aborted = 0;
            item = Item_Get(menu->selected_item & 0x1ff);
            if ((item->flags & 16) != 0) {
                qty = InventoryMenu_GetItemQuantity(
                    menu->target_owner, menu->selected_item & 0x1ff);
                if (qty == 30) {
                    aborted = 1;
                }
                if (InventoryMenu_CountItems(menu->target_owner) == 15 &&
                    qty == 0) {
                    state = 7;
                    break;
                }
                stack = (menu->selected_item >> 11) + 1;
                if (aborted == 0) {
                    amount = stack;
                    if (qty + amount > 30) {
                        amount = 30 - qty;
                    }
                    if (stack > 1) {
                        picked = Func_080a4f08(0, amount, 0);
                    } else {
                        picked = 0;
                    }
                    if (picked == -1) {
                        state = 6;
                        break;
                    }
                    for (work = 0; work < picked + 1; work++) {
                        n = Inventory_AddForOwner(
                            menu->target_owner, menu->selected_item & 0x5ff);
                        if (n != -1) {
                            Func_08077058(
                                menu->item_owner, menu->selected_slot);
                            M2C_FIELD(menu, u16 *, 0x176) = n;
                        } else {
                            aborted = 1;
                        }
                    }
                }
            } else {
                n = Inventory_AddForOwner(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (n == -1) {
                    state = 7;
                    break;
                }
                M2C_FIELD(menu, u16 *, 0x176) = n;
                if (Func_08077058(menu->item_owner, menu->selected_slot) ==
                    -1) {
                    aborted = 1;
                }
            }
            Func_08077010(menu->item_owner);
            Func_08077010(menu->target_owner);
            Func_080772c0(menu->item_owner);
            Func_080772c0(menu->target_owner);
            swapped = 1;
            if (aborted == 0) {
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                InventoryMenu_SetMessageWindowRow6Bounds();
                UiWindow_Commit(menu->message_window);
                InventoryMenu_DrawSelectedItemHeader();
                swapped = Func_080a5388(0);
            }
            if (Func_080770c0(0x150) != 0) {
                break;
            }
            InventoryMenu_RefreshOwnerItems(menu->target_owner, 1);
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 13;
            WaitFrames(1);
            if (aborted == 1) {
                Func_08015278(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b85, 15, 14);
            } else {
                Func_08015278(menu->info_window);
                if (swapped == 1) {
                    InventoryMenu_ShowModalMessage(
                        (s32)&Value_00000b7f, 15, 14);
                } else {
                    Func_080a3ef0(
                        menu->target_owner,
                        M2C_FIELD(menu, u16 *, 0x176),
                        0);
                    InventoryMenu_ShowModalMessage(
                        (s32)&Value_00000b7c, 15, 14);
                    item = Item_Get(menu->selected_item);
                    if ((item->flags & 1) != 0) {
                        Audio_PlayCue(0x67);
                        Func_08015278(menu->info_window);
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b7c + 7, 14, 14);
                    }
                }
            }
            Func_0808a548();
            state = 0;
            break;

        case 7:
            aborted = 0;
            InventoryMenu_SetMessageWindowRow3Bounds();
            InventoryMenu_SetItemWindowThreeRowBounds();
            InventoryMenu_DrawMessage(0, (s32)&Value_00000add);
            sel = InventoryMenu_SelectPartyMember(1);
            if (sel == -1) {
                state = 6;
                break;
            }
            source = OwnerState_GetFar(menu->item_owner);
            target = OwnerState_GetFar(menu->target_owner);
            source_copy = Runtime_BumpAllocate(0x14c);
            target_copy = Runtime_BumpAllocate(0x14c);
            Func_080072fc(source_copy, source, 0x14c);
            Func_080072fc(target_copy, target, 0x14c);
            moved_source = 0;
            while (moved_source <= 29) {
                n = Func_08077058(menu->item_owner, menu->selected_slot);
                if (n == 2) {
                    break;
                }
                if (n == -1) {
                    aborted = 1;
                    break;
                }
                moved_source++;
            }
            moved_source++;
            moved_target = 0;
            while (moved_target <= 29) {
                work = M2C_FIELD(menu, u16 *, 0x17a);
                if ((work & 0x200) != 0) {
                    item = Item_Get(work & 0x1ff);
                    if ((item->flags & 2) != 0) {
                        aborted = 1;
                    }
                }
                n = Func_08077058(
                    menu->target_owner, M2C_FIELD(menu, u16 *, 0x176));
                if (n == 2) {
                    break;
                }
                if (n == -1) {
                    aborted = 1;
                    break;
                }
                moved_target++;
            }
            moved_target++;
            while (moved_source != 0) {
                n = Inventory_AddForOwner(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (n == -1) {
                    aborted = 1;
                    break;
                }
                M2C_FIELD(menu, u16 *, 0x176) = n;
                moved_source--;
            }
            while (moved_target != 0) {
                n = Inventory_AddForOwner(
                    menu->item_owner, M2C_FIELD(menu, u16 *, 0x17a) & 0x5ff);
                if (n == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slot = n;
                moved_target--;
            }
            WaitFrames(1);
            if (aborted == 1) {
                Func_080072f8(source, source_copy, 0x14c);
                Func_080072f8(target, target_copy, 0x14c);
                Func_08015278(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b84, 15, 14);
            } else {
                Func_08077010(menu->item_owner);
                Func_08077010(menu->target_owner);
                Func_080772c0(menu->item_owner);
                Func_080772c0(menu->target_owner);
                InventoryMenu_SetMessageWindowRow5Bounds();
                InventoryMenu_SetMessageWindowRow6Bounds();
                InventoryMenu_HidePageStartIcons();
                UiWindow_Commit(menu->message_window);
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                InventoryMenu_DrawSelectedItemHeader();
                swapped = Func_080a5388(0);
                if (Func_080770c0(0x150) == 0) {
                    Func_08015278(menu->info_window);
                    InventoryMenu_SetItemWindowFiveRowBounds();
                    InventoryMenu_RefreshOwnerItems(menu->target_owner, 1);
                    if (swapped == 0) {
                        Func_080a3ef0(
                            menu->target_owner,
                            M2C_FIELD(menu, u16 *, 0x176),
                            0);
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b7c, 15, 14);
                        item = Item_Get(menu->selected_item);
                        if ((item->flags & 1) != 0) {
                            Audio_PlayCue(0x67);
                            Func_08015278(menu->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&Value_00000b7c + 7, 14, 14);
                        }
                    } else {
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b81, 15, 14);
                    }
                }
            }
            Func_08002df0(target_copy);
            Func_08002df0(source_copy);
            Func_0808a548();
            state = 0;
            break;

        case 3:
            n = Func_08077050(menu->item_owner, menu->selected_slot);
            state = 1;
            if (n == -1) {
                break;
            }
            if (n == -2) {
                Func_08015278(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b82, 0, -1);
                state = 1;
                break;
            }
            Func_08077010(menu->item_owner);
            Func_080772c0(menu->item_owner);
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 13;
            menu->item_count = InventoryMenu_CollectItems(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            InventoryMenu_DrawItemIcons(menu->items, 0);
            WaitFrames(1);
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
            Func_08015278(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&Value_00000b7c, 15, 8);
            item = Item_Get(menu->selected_item);
            if ((item->flags & 1) != 0) {
                Audio_PlayCue(0x67);
                Func_08015278(menu->info_window);
                InventoryMenu_ShowModalMessage(
                    (s32)&Value_00000b7c + 7, 14, 8);
            }
            state = 1;
            break;

        case 11:
            OwnerState_GetFar(menu->item_owner)->inventory[menu->selected_slot]
                &= 0xfdff;
            Func_08077010(menu->item_owner);
            Func_080772c0(menu->item_owner);
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 13;
            menu->item_count = InventoryMenu_CollectItems(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            InventoryMenu_DrawItemIcons(menu->items, 0);
            WaitFrames(1);
            M2C_FIELD(menu, u8 *, 0x25c) = 1;
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
            M2C_FIELD(menu, u8 *, 0x25c) = 0;
            Func_08015278(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&Value_00000b80, 14, 8);
            Func_0808a548();
            state = 1;
            break;

        case 10:
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 13;
            Func_080a4800(menu->selected_item);
            UiWindow_Commit(M2C_FIELD(menu, s32 *, 0x24));
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0);
            M2C_FIELD(menu, struct InventoryMenuIcon **, 0x14)->state = 1;
            state = 9;
            break;

        case 12:
            cnt = Func_080a4f08(0, 30, 0);
            state = 1;
            break;

        case 8:
            break;

        default:
            done = 1;
            break;
        }
    }

    if (Func_080770c0(0x150) != 0) {
        ret = -1;
    }
    return ret;
}
