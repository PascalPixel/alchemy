#include "TYPES.H"
#include "ITEM.H"
#include "OWNER_STATE.H"
#include "GLOBAL_CELLS.H"

struct ItemMenuIcon {
    u8 unknown_00[5];
    u8 state;                       /* 0x05 */
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[4];
    u8 render_target;               /* 0x0e */
};

struct ItemCommandWork {
    u8 unknown_000[0x14];
    struct ItemMenuIcon *list_icon;  /* 0x014 */
    u8 unknown_018[0x0c];
    s32 status_window;              /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;                /* 0x02c */
    u8 unknown_030[4];
    s32 preview_window;             /* 0x034 */
    u8 unknown_038[0xd4];
    s32 message_window;             /* 0x10c */
    u8 unknown_110[0x64];
    u16 selected_slot;              /* 0x174 */
    u16 target_slot;                /* 0x176 */
    u16 selected_item;              /* 0x178 */
    u16 target_item;                /* 0x17a */
    u8 unknown_17c[0x4c];
    u16 items[32];                  /* 0x1c8 */
    u8 unknown_208[0x10];
    u8 item_count;                  /* 0x218 */
    u8 party_count;                 /* 0x219 */
    u8 item_owner;                  /* 0x21a */
    u8 target_owner;                /* 0x21b */
    struct ItemMenuIcon * volatile selected_item_icon; /* 0x21c */
    u8 unknown_220[2];
    s16 completion_flag;            /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;             /* 0x25a */
    u8 equip_preview;               /* 0x25c */
    u8 list_mode;                   /* 0x25d */
};

#define ItemMenu_RunItemCommand Func_080a2680

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
extern char Value_00000200;

void Func_080030f8(s32 frames);
void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);
typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}
void Func_08015068(s32 window, s32 unused, s32 x, s32 y, s32 height);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_08015270(s32 window);
void Func_08015278(s32 window);
void Func_08015298(s32 kind, s32 item, s32 target, s32 flags);
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
s32 Func_080a3d9c(s32 owner, s32 item);
void Func_080a3ef0(s32 owner, s32 slot, s32 unused, s32 owner2);
s32 Func_080a414c(void);
s32 Func_080a46b4(s32 owner, s32 item);
void Func_080a4800(s32 item);
s32 Func_080a4f08(s32 unused, s32 limit, s32 mode);
s32 Func_080a524c(s32 index);
s32 Func_080a5388(s32 mode);
s32 Func_080a5788(s32 mode);

#define ItemMenu_GetCommandChoice  Func_080a414c
#define ItemMenu_ShowModalMessage  Func_080a1d08
#define ItemMenu_SelectPartyMember Func_080a5788
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
 *
 * DRAFT, not yet C (2026-09-24): 1266 of 1294 listing lines, 126 lines differ
 * (62 once registers are normalised). Proven: the handlers set the next
 * state inside each exit branch (if-conversion hoists it, so cse never sees
 * a redundant set); one result variable (r6) and one command variable (r7)
 * are shared across the handlers; clearing the command on cancel lets the
 * -1 compare register reach ret; the preview call passes the owner again in
 * r3; the 0x03001388 copy routine goes through _call_via with an inline
 * wrapper; the 0x17a item is a short-lived temporary; 0x200 comes from the
 * pool; the selected item icon pointer is volatile. The drop handler loads
 * 0x1ff into the state variable and never reassigns it on success, which
 * ends the menu through the default case (FAKEMATCH candidate). Remaining:
 * the stack count in the give handler is sign-extended at its use, the
 * list-icon stores schedule the menu copy one slot early, and case 12
 * keeps its result in r6.
 */
s32 ItemMenu_RunItemCommand(s32 *owner_out, s32 *target_out, s32 *item_out)
{
    s32 done;
    s32 sel;
    s32 ret;
    struct OwnerInventoryState *source;
    struct OwnerInventoryState *target;
    struct ItemCommandWork *menu;
    struct ItemDefinition *item;
    void *source_copy;
    void *target_copy;
    u32 state;
    s32 command;
    s32 result;
    s32 qty;
    s32 amount;
    s32 aborted;
    s32 work;
    s32 other;
    s32 n;
    u8 moved_source;
    u8 moved_target;
    s8 stack;

    done = 0;
    sel = 0;
    ret = 0;
    state = 0;
    menu = *(struct ItemCommandWork **)ADDR_03001F2C;

    while (done == 0 && Func_080770c0(0x150) == 0) {
        switch (state) {
        case 0:
            menu->selected_slot = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->state = 13;
            ItemMenu_DrawMsg(0, (s32)&Value_00000ad8);
            UiWindow_Commit(menu->info_window);
            UiText_DrawWorkValueWithLabel(menu->info_window);
            command = ItemMenu_PrepOwner(0);
            if (command == -1) {
                sel = command = 0;
                ret = -1;
                done = 1;
            }
            UiWindow_Commit(menu->info_window);
            ItemMenu_HideAllIcons();
            state = 1;
            break;

        case 1:
            if (ItemMenu_Count(menu->item_owner) == 0) {
                state = 0;
                break;
            }
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->state = 13;
            menu->list_icon->state = 1;
            ItemMenu_DrawMsg(0, (s32)&Value_00000ad9);
            sel = ItemMenu_SelectPartyMember(0);
            state = 0;
            if (sel == -1) {
                break;
            }
            menu->list_mode = 0xff;
            state = 9;
            break;

        case 9:
            command = ItemMenu_GetCommandChoice();
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
                result = Item_ClassifyUseMode(
                    menu->item_owner, menu->selected_item);
                if (result == 1) {
                    state = 2;
                }
                if (result == 2) {
                    ItemMenu_Use();
                    Func_08015278(menu->info_window);
                    ItemMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    menu->list_icon->state =
                        13;
                    menu->item_count = ItemMenu_Collect(
                        OwnerState_GetFar(menu->item_owner), menu->items, 0);
                    ItemMenu_DrawIcons(menu->items, 0);
                    state = 0;
                }
                if (result == -1 || result == 0) {
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
            ItemMenu_HideAllIcons();
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            UiWindow_Commit(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawAt(
                (s32)&Value_00000adb, menu->message_window, 16, 16);
            if (Func_080a38d0(0) != -1) {
                command = 0;
                if (ItemMenu_IsSpecial(
                        menu->selected_item & 0x1ff) != 0) {
                    command = 8;
                }
                result = ItemMenu_Use();
                Func_080a112c(
                    menu->status_window, menu->target_owner, 0, command);
                if (result != -1) {
                    Func_08015278(menu->info_window);
                    ItemMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    menu->list_icon->state =
                        13;
                    ItemMenu_TryBreak();
                    state = 1;
                }
                menu->item_count = ItemMenu_Collect(
                    OwnerState_GetFar(menu->item_owner), menu->items, 0);
                ItemMenu_DrawIcons(menu->items, 0);
                menu->completion_flag = 1;
            } else {
                state = 9;
            }
            break;

        case 6:
            ItemMenu_SetMsgWin5();
            ItemMenu_SetItemWin5();
            UiWindow_Commit(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawAt(
                (s32)&Value_00000adc, menu->message_window, 16, 16);
            n = Func_080a38d0(1);
            state = 4;
            if (n == -1) {
                Func_080a3ef0(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
                state = 9;
            }
            break;

        case 5:
            ItemMenu_HideAllIcons();
            item = Item_Get(menu->selected_item & 0x1ff);
            result = 0;
            if ((item->flags & 16) != 0) {
                qty = (menu->selected_item >> 11) + 1;
                if (qty > 1) {
                    ItemMenu_DrawItemHead();
                    result = Func_080a4f08(0, qty, 1);
                }
            }
            if (result == -1) {
                state = 9;
                break;
            }
            menu->target_owner = 0;
            state = 0x1ff;
            Func_08015298(
                2,
                (menu->selected_item & 0x1ff) | (result << 11),
                menu->selected_item_icon->render_target,
                0);
            menu->selected_item_icon->state = 1;
            menu->selected_item_icon->x = 120;
            menu->selected_item_icon->y = 28;
            UiIcon_PrepareObject(menu->selected_item_icon);
            Func_08015068(menu->preview_window, 0, 72, 120, 96);
            UiWindow_Commit(menu->message_window);
            if (Func_080a524c(sel) == 0) {
                command = menu->item_owner;
                OwnerState_GetFar(command);
                for (work = 0; work < result + 1; work++) {
                    Func_08077058(command, menu->selected_slot);
                    Func_08077240(menu->selected_item & 0x1ff, 1);
                }
                Func_08077010(command);
                ItemMenu_SetItemWin3();
                ItemMenu_RefreshOwner(menu->item_owner, 0);
                menu->selected_item_icon->state = 13;
                menu->list_icon->state = 13;
                WaitFrames(1);
                Func_08015278(menu->info_window);
                ItemMenu_ShowModalMessage((s32)&Value_00000b7d, 14, 13);
                menu->completion_flag = 1;
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
                qty = Func_080a3d9c(
                    menu->target_owner, menu->selected_item & 0x1ff);
                if (qty == 30) {
                    aborted = 1;
                }
                if (ItemMenu_Count(menu->target_owner) == 15 &&
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
                        result = Func_080a4f08(0, amount, 0);
                    } else {
                        result = 0;
                    }
                    if (result == -1) {
                        state = 6;
                        break;
                    }
                    for (command = 0; command < result + 1; command++) {
                        qty = Inventory_AddForOwner(
                            menu->target_owner, menu->selected_item & 0x5ff);
                        if (qty != -1) {
                            Func_08077058(
                                menu->item_owner, menu->selected_slot);
                            menu->target_slot = qty;
                        } else {
                            aborted = 1;
                        }
                    }
                }
            } else {
                result = Inventory_AddForOwner(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (result == -1) {
                    state = 7;
                    break;
                }
                menu->target_slot = result;
                result = Func_08077058(menu->item_owner, menu->selected_slot);
                if (result == -1) {
                    aborted = 1;
                }
            }
            Func_08077010(menu->item_owner);
            Func_08077010(menu->target_owner);
            Func_080772c0(menu->item_owner);
            Func_080772c0(menu->target_owner);
            result = 1;
            if (aborted == 0) {
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                ItemMenu_SetMsgWin6();
                UiWindow_Commit(menu->message_window);
                ItemMenu_DrawItemHead();
                result = Func_080a5388(0);
            }
            if (Func_080770c0(0x150) != 0) {
                break;
            }
            ItemMenu_RefreshOwner(menu->target_owner, 1);
            menu->list_icon->state = 13;
            WaitFrames(1);
            if (aborted == 1) {
                Func_08015278(menu->info_window);
                ItemMenu_ShowModalMessage((s32)&Value_00000b85, 15, 14);
            } else {
                Func_08015278(menu->info_window);
                if (result == 1) {
                    ItemMenu_ShowModalMessage(
                        (s32)&Value_00000b7f, 15, 14);
                } else {
                    Func_080a3ef0(menu->target_owner, menu->target_slot, 0, menu->target_owner);
                    ItemMenu_ShowModalMessage(
                        (s32)&Value_00000b7c, 15, 14);
                    item = Item_Get(menu->selected_item);
                    if ((item->flags & 1) != 0) {
                        Audio_PlayCue(0x67);
                        Func_08015278(menu->info_window);
                        ItemMenu_ShowModalMessage(
                            (s32)&Value_00000b7c + 7, 14, 14);
                    }
                }
            }
            Func_0808a548();
            state = 0;
            break;

        case 7:
            aborted = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            ItemMenu_DrawMsg(0, (s32)&Value_00000add);
            sel = ItemMenu_SelectPartyMember(1);
            if (sel == -1) {
                state = 6;
                break;
            }
            source = OwnerState_GetFar(menu->item_owner);
            target = OwnerState_GetFar(menu->target_owner);
            source_copy = Runtime_BumpAllocate(0x14c);
            target_copy = Runtime_BumpAllocate(0x14c);
            CopyWords((WordCopyFn)0x03001388, source_copy, source, 0x14c);
            CopyWords((WordCopyFn)0x03001388, target_copy, target, 0x14c);
            moved_source = 0;
            while (moved_source <= 29) {
                result = Func_08077058(menu->item_owner, menu->selected_slot);
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
                other = menu->target_item;
                if ((other & (s32)&Value_00000200) != 0) {
                    item = Item_Get(other & 0x1ff);
                    if ((item->flags & 2) != 0) {
                        aborted = 1;
                    }
                }
                result = Func_08077058(
                    menu->target_owner, menu->target_slot);
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
                result = Inventory_AddForOwner(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->target_slot = result;
                moved_source--;
            }
            while (moved_target != 0) {
                result = Inventory_AddForOwner(
                    menu->item_owner, menu->target_item & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->selected_slot = result;
                moved_target--;
            }
            WaitFrames(1);
            if (aborted == 1) {
                CopyWords((WordCopyFn)0x03001388, source, source_copy, 0x14c);
                CopyWords((WordCopyFn)0x03001388, target, target_copy, 0x14c);
                Func_08015278(menu->info_window);
                ItemMenu_ShowModalMessage((s32)&Value_00000b84, 15, 14);
            } else {
                Func_08077010(menu->item_owner);
                Func_08077010(menu->target_owner);
                Func_080772c0(menu->item_owner);
                Func_080772c0(menu->target_owner);
                ItemMenu_SetMsgWin5();
                ItemMenu_SetMsgWin6();
                ItemMenu_HidePageIcons();
                UiWindow_Commit(menu->message_window);
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                ItemMenu_DrawItemHead();
                result = Func_080a5388(0);
                if (Func_080770c0(0x150) == 0) {
                    Func_08015278(menu->info_window);
                    ItemMenu_SetItemWin5();
                    ItemMenu_RefreshOwner(menu->target_owner, 1);
                    if (result == 0) {
                        Func_080a3ef0(menu->target_owner, menu->target_slot, 0, menu->target_owner);
                        ItemMenu_ShowModalMessage(
                            (s32)&Value_00000b7c, 15, 14);
                        item = Item_Get(menu->selected_item);
                        if ((item->flags & 1) != 0) {
                            Audio_PlayCue(0x67);
                            Func_08015278(menu->info_window);
                            ItemMenu_ShowModalMessage(
                                (s32)&Value_00000b7c + 7, 14, 14);
                        }
                    } else {
                        ItemMenu_ShowModalMessage(
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
            result = Func_08077050(menu->item_owner, menu->selected_slot);
            if (result == -1) {
                state = 1;
                break;
            }
            if (result == -2) {
                Func_08015278(menu->info_window);
                ItemMenu_ShowModalMessage((s32)&Value_00000b82, 0, -1);
                state = 1;
                break;
            }
            Func_08077010(menu->item_owner);
            Func_080772c0(menu->item_owner);
            menu->list_icon->state = 13;
            menu->item_count = ItemMenu_Collect(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            Func_08015278(menu->info_window);
            ItemMenu_ShowModalMessage((s32)&Value_00000b7c, 15, 8);
            item = Item_Get(menu->selected_item);
            if ((item->flags & 1) != 0) {
                Audio_PlayCue(0x67);
                Func_08015278(menu->info_window);
                ItemMenu_ShowModalMessage(
                    (s32)&Value_00000b7c + 7, 14, 8);
            }
            state = 1;
            break;

        case 11:
            OwnerState_GetFar(menu->item_owner)->inventory[menu->selected_slot]
                &= 0xfdff;
            Func_08077010(menu->item_owner);
            Func_080772c0(menu->item_owner);
            menu->list_icon->state = 13;
            menu->item_count = ItemMenu_Collect(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            menu->equip_preview = 1;
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            menu->equip_preview = 0;
            Func_08015278(menu->info_window);
            ItemMenu_ShowModalMessage((s32)&Value_00000b80, 14, 8);
            Func_0808a548();
            state = 1;
            break;

        case 10:
            menu->list_icon->state = 13;
            Func_080a4800(menu->selected_item);
            UiWindow_Commit(menu->status_window);
            Func_080a3ef0(menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            menu->list_icon->state = 1;
            state = 9;
            break;

        case 12:
            result = Func_080a4f08(0, 30, 0);
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
