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

/* The item menu work block at 0x03001F2C, as this loop sees it. */
struct ItemCommandWork {
    u8 unknown_000[0x14];
    union {
        struct ItemMenuIcon *icon;
        u32 word;
    } list;                         /* 0x014 */
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

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

/* The resident word copier, addressed from the IWRAM runtime base. */
extern u8 Value_03000000[];
#define IWRAM_COPY_WORDS ((WordCopyFn)(Value_03000000 + 0x1388))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}

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

void WaitFrames(s32 frames);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *buffer);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 unused, s32 x, s32 y, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void Resource_LoadByModeIntoSlotFar(s32 kind, s32 item, s32 target, s32 flags);
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
s32 Func_080a38d0(s32 mode);
s32 Func_080a3d9c(s32 owner, s32 item);
/* Takes a fourth argument; this caller passes the owner there as well. */
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 arg3);
s32 Func_080a414c(void);
s32 Item_ClassifyUseMode(s32 owner, s32 item);
void Menu_SelectQuantity(s32 item);
s32 Func_080a4f08(s32 unused, s32 limit, s32 mode);
s32 Func_080a524c(s32 index);
s32 Unnamed_080a5388(s32 mode);
s32 ItemMenu_RunList(s32 pane);
s32 ItemMenu_PrepOwner();
s32 ItemMenu_Count();
s32 ItemMenu_Use();
s32 ItemMenu_IsSpecial();
s32 ItemMenu_Collect();
s32 ItemMenu_SetMsgWin3();
s32 ItemMenu_SetMsgWin5();
s32 ItemMenu_SetMsgWin6();
s32 ItemMenu_SetItemWin3();
s32 ItemMenu_SetItemWin5();
s32 ItemMenu_DrawMsg();
s32 ItemMenu_HideAllIcons();
s32 ItemMenu_HidePageIcons();
s32 ItemMenu_DrawIcons();
s32 ItemMenu_DrawItemHead();
s32 ItemMenu_TryBreak();
s32 ItemMenu_RefreshOwner();
s32 UiIcon_PrepareObject();

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
    s32 n;
    u8 moved_source;
    u8 moved_target;
    s8 stack;

    done = 0;
    sel = 0;
    ret = 0;
    state = 0;
    menu = *(struct ItemCommandWork **)ADDR_03001F2C;

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        switch (state) {
        case 0:
            menu->selected_slot = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            menu->selected_item_icon->state = 13;
            ItemMenu_DrawMsg(0, (s32)&Value_00000ad8);
            RenderOutput_RedrawSavedRectFar(menu->info_window);
            UiText_DrawWorkValueWithLabel(menu->info_window);
            command = ItemMenu_PrepOwner(0);
            if (command == -1) {
                sel = command = 0;
                ret = -1;
                done = 1;
            }
            RenderOutput_RedrawSavedRectFar(menu->info_window);
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
            menu->list.icon->state = 1;
            ItemMenu_DrawMsg(0, (s32)&Value_00000ad9);
            sel = ItemMenu_RunList(0);
            state = 0;
            if (sel == -1) {
                break;
            }
            menu->list_mode = 0xff;
            state = 9;
            break;

        case 9:
            command = Func_080a414c();
            if (command == -1) {
                state = 1;
                menu->completion_flag = 1;
            }
            if (command == 0) {
                if (BattleFx_HasTriggerFar(menu->selected_item & 0x1ff) != 0) {
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
                    RenderOutput_ClearListFar(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    menu->list.icon->state = 13;
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
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)&Value_00000adb, menu->message_window, 16, 16);
            if (Func_080a38d0(0) != -1) {
                command = 0;
                if (ItemMenu_IsSpecial(
                        menu->selected_item & 0x1ff) != 0) {
                    command = 8;
                }
                result = ItemMenu_Use();
                Menu_DrawOwnerStatusPanel(
                    menu->status_window, menu->target_owner, 0, command);
                if (result != -1) {
                    RenderOutput_ClearListFar(menu->info_window);
                    InventoryMenu_ShowModalMessage(
                        menu->message_offset + (s32)&Value_00000bef, 0, -1);
                    menu->list.icon->state = 13;
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
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            ItemMenu_DrawItemHead();
            UiText_DrawCharacterAtOffsetFar(
                (s32)&Value_00000adc, menu->message_window, 16, 16);
            n = Func_080a38d0(1);
            state = 4;
            if (n == -1) {
                ItemMenu_DrawEquipPreview(
                    menu->item_owner, menu->selected_slot, 0, menu->item_owner);
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
            Resource_LoadByModeIntoSlotFar(
                2,
                (menu->selected_item & 0x1ff) | (result << 11),
                menu->selected_item_icon->render_target,
                0);
            menu->selected_item_icon->state = 1;
            menu->selected_item_icon->x = 120;
            menu->selected_item_icon->y = 28;
            UiIcon_PrepareObject(menu->selected_item_icon);
            UiWindow_ClearInteriorTilesFar(menu->preview_window, 0, 72, 120, 96);
            RenderOutput_RedrawSavedRectFar(menu->message_window);
            if (Func_080a524c(sel) == 0) {
                command = menu->item_owner;
                OwnerState_GetFar(command);
                for (work = 0; work < result + 1; work++) {
                    Inventory_RemoveFar(command, menu->selected_slot);
                    Func_08077240(menu->selected_item & 0x1ff, 1);
                }
                Owner_RecalculateStatsFar(command);
                ItemMenu_SetItemWin3();
                ItemMenu_RefreshOwner(menu->item_owner, 0);
                menu->selected_item_icon->state = 13;
                menu->list.icon->state = 13;
                WaitFrames(1);
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b7d, 14, 13);
                menu->completion_flag = 1;
                state = 1;
            } else {
                state = 9;
            }
            Owner_RecalculateStatsFar(menu->item_owner);
            menu->selected_item_icon->state = 13;
            Event_ClearInvalidPackedValuesFar();
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
                    /* FAKEMATCH: the loop only places a label between the
                       count and its sign-extended use. */
                    do {
                        amount = stack;
                    } while (0);
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
                        qty = Inventory_AddItemFar(
                            menu->target_owner, menu->selected_item & 0x5ff);
                        if (qty != -1) {
                            Inventory_RemoveFar(
                                menu->item_owner, menu->selected_slot);
                            menu->target_slot = qty;
                        } else {
                            aborted = 1;
                        }
                    }
                }
            } else {
                result = Inventory_AddItemFar(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (result == -1) {
                    state = 7;
                    break;
                }
                menu->target_slot = result;
                result = Inventory_RemoveFar(menu->item_owner, menu->selected_slot);
                if (result == -1) {
                    aborted = 1;
                }
            }
            Owner_RecalculateStatsFar(menu->item_owner);
            Owner_RecalculateStatsFar(menu->target_owner);
            Func_080772c0(menu->item_owner);
            Func_080772c0(menu->target_owner);
            result = 1;
            if (aborted == 0) {
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                ItemMenu_SetMsgWin6();
                RenderOutput_RedrawSavedRectFar(menu->message_window);
                ItemMenu_DrawItemHead();
                result = Unnamed_080a5388(0);
            }
            if (GameFlag_TestFar(0x150) != 0) {
                break;
            }
            ItemMenu_RefreshOwner(menu->target_owner, 1);
            menu->list.icon->state = 13;
            WaitFrames(1);
            if (aborted == 1) {
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b85, 15, 14);
            } else {
                RenderOutput_ClearListFar(menu->info_window);
                if (result == 1) {
                    InventoryMenu_ShowModalMessage(
                        (s32)&Value_00000b7f, 15, 14);
                } else {
                    ItemMenu_DrawEquipPreview(
                        menu->target_owner, menu->target_slot, 0, menu->target_owner);
                    InventoryMenu_ShowModalMessage(
                        (s32)&Value_00000b7c, 15, 14);
                    item = Item_Get(menu->selected_item);
                    if ((item->flags & 1) != 0) {
                        Audio_PlayCue(0x67);
                        RenderOutput_ClearListFar(menu->info_window);
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b7c + 7, 14, 14);
                    }
                }
            }
            Event_ClearInvalidPackedValuesFar();
            state = 0;
            break;

        case 7:
            aborted = 0;
            ItemMenu_SetMsgWin3();
            ItemMenu_SetItemWin3();
            ItemMenu_DrawMsg(0, (s32)&Value_00000add);
            sel = ItemMenu_RunList(1);
            if (sel == -1) {
                state = 6;
                break;
            }
            source = OwnerState_GetFar(menu->item_owner);
            target = OwnerState_GetFar(menu->target_owner);
            source_copy = Runtime_BumpAllocate(0x14c);
            target_copy = Runtime_BumpAllocate(0x14c);
            CopyWords(IWRAM_COPY_WORDS, source_copy, source, 0x14c);
            CopyWords(IWRAM_COPY_WORDS, target_copy, target, 0x14c);
            moved_source = 0;
            while (moved_source <= 29) {
                result = Inventory_RemoveFar(menu->item_owner, menu->selected_slot);
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
                if ((menu->target_item & 0x200) != 0) {
                    item = Item_Get(menu->target_item & 0x1ff);
                    if ((item->flags & 2) != 0) {
                        aborted = 1;
                    }
                }
                result = Inventory_RemoveFar(
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
                result = Inventory_AddItemFar(
                    menu->target_owner, menu->selected_item & 0x5ff);
                if (result == -1) {
                    aborted = 1;
                    break;
                }
                menu->target_slot = result;
                moved_source--;
            }
            while (moved_target != 0) {
                result = Inventory_AddItemFar(
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
                CopyWords(IWRAM_COPY_WORDS, source, source_copy, 0x14c);
                CopyWords(IWRAM_COPY_WORDS, target, target_copy, 0x14c);
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b84, 15, 14);
            } else {
                Owner_RecalculateStatsFar(menu->item_owner);
                Owner_RecalculateStatsFar(menu->target_owner);
                Func_080772c0(menu->item_owner);
                Func_080772c0(menu->target_owner);
                ItemMenu_SetMsgWin5();
                ItemMenu_SetMsgWin6();
                ItemMenu_HidePageIcons();
                RenderOutput_RedrawSavedRectFar(menu->message_window);
                menu->item_owner = menu->target_owner;
                menu->selected_item &= 0x1ff;
                ItemMenu_DrawItemHead();
                result = Unnamed_080a5388(0);
                if (GameFlag_TestFar(0x150) == 0) {
                    RenderOutput_ClearListFar(menu->info_window);
                    ItemMenu_SetItemWin5();
                    ItemMenu_RefreshOwner(menu->target_owner, 1);
                    if (result == 0) {
                        ItemMenu_DrawEquipPreview(
                            menu->target_owner, menu->target_slot, 0, menu->target_owner);
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b7c, 15, 14);
                        item = Item_Get(menu->selected_item);
                        if ((item->flags & 1) != 0) {
                            Audio_PlayCue(0x67);
                            RenderOutput_ClearListFar(menu->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&Value_00000b7c + 7, 14, 14);
                        }
                    } else {
                        InventoryMenu_ShowModalMessage(
                            (s32)&Value_00000b81, 15, 14);
                    }
                }
            }
            Runtime_BumpFree(target_copy);
            Runtime_BumpFree(source_copy);
            Event_ClearInvalidPackedValuesFar();
            state = 0;
            break;

        case 3:
            result = Inventory_EquipFar(menu->item_owner, menu->selected_slot);
            if (result == -1) {
                state = 1;
                break;
            }
            if (result == -2) {
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage((s32)&Value_00000b82, 0, -1);
                state = 1;
                break;
            }
            Owner_RecalculateStatsFar(menu->item_owner);
            Func_080772c0(menu->item_owner);
            menu->list.icon->state = 13;
            menu->item_count = ItemMenu_Collect(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            ItemMenu_DrawEquipPreview(
                menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            RenderOutput_ClearListFar(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&Value_00000b7c, 15, 8);
            item = Item_Get(menu->selected_item);
            if ((item->flags & 1) != 0) {
                Audio_PlayCue(0x67);
                RenderOutput_ClearListFar(menu->info_window);
                InventoryMenu_ShowModalMessage(
                    (s32)&Value_00000b7c + 7, 14, 8);
            }
            state = 1;
            break;

        case 11:
            OwnerState_GetFar(menu->item_owner)->inventory[menu->selected_slot]
                &= 0xfdff;
            Owner_RecalculateStatsFar(menu->item_owner);
            Func_080772c0(menu->item_owner);
            menu->list.icon->state = 13;
            menu->item_count = ItemMenu_Collect(
                OwnerState_GetFar(menu->item_owner), menu->items, 0);
            ItemMenu_DrawIcons(menu->items, 0);
            WaitFrames(1);
            menu->equip_preview = 1;
            ItemMenu_DrawEquipPreview(
                menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            menu->equip_preview = 0;
            RenderOutput_ClearListFar(menu->info_window);
            InventoryMenu_ShowModalMessage((s32)&Value_00000b80, 14, 8);
            Event_ClearInvalidPackedValuesFar();
            state = 1;
            break;

        case 10:
            menu->list.icon->state = 13;
            Menu_SelectQuantity(menu->selected_item);
            RenderOutput_RedrawSavedRectFar(menu->status_window);
            ItemMenu_DrawEquipPreview(
                menu->item_owner, menu->selected_slot, 0, menu->item_owner);
            menu->list.icon->state = 1;
            state = 9;
            break;

        case 12:
            result = Func_080a4f08(0, 30, 0);
            if (result != -1) {
                qty = result;
            }
            state = 1;
            break;

        case 8:
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
