#include "scene.h"
#include "shop.h"
#include "types.h"
#include "global_cells.h"
#include "battle_calc.h"

/* shop/sel/pick_item.c */
#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

s32 Modulo(s32 value, s32 divisor);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_Close(s32 window, s32 style);
struct ShopCursorAnchor *Sys_Run(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);

void Audio_PlayCue(s32 cue);

/* Select a party member and then an item owned by that member. */
s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item)
{
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;
    s32 list_window;
    s32 selected_index = 0;
    s32 redraw = 1;
    s32 unit_id = 0;
    s32 item_slot;
    s32 result = 0;

    Sys_unk2_4();
    shop = SHOP_RUNTIME;
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    cursor_anchor = Sys_Run(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        result);
    cursor_anchor->kind = 4;
    cursor_anchor->unknown_00[4] = result;
    Sys_Place(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    shop->mode = 12;
    Sys_SetRange(list_window, 2, 0, 8, result);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = Modulo(
                selected_index + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selected_index, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        WaitFrames(1);
        if ((INPUT_NEW_KEYS & 1) != 0) {
            if (Sys_Check(unit_id) == 0) {
                Audio_PlayCue(0x71);
                continue;
            }

            Audio_PlayCue(0x70);
            item_slot = Sys_unk2(unit_id);
            if (item_slot == -1) {
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
                continue;
            }
            *selected_unit = unit_id;
            *selected_item = item_slot;
            result = 0;
            goto done;
        }

        if ((INPUT_NEW_KEYS & 2) != 0) {
            Audio_PlayCue(0x71);
            *selected_unit = -1;
            *selected_item = -1;
            result = -1;
            goto done;
        }

        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Audio_PlayCue(0x6f);
            selected_index--;
            redraw = 1;
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Audio_PlayCue(0x6f);
            selected_index++;
            redraw = 1;
        }
    }

done:
    Sys_unk3_4();
    UiWindow_Close(list_window, 2);
    UiWindow_Close(shop->item_window, 2);
    WaitFrames(1);
    Sys_unk4();
    return result;
}

/* shop/sel/use.c */
s32 Ability_GetAvailability(s32);

s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePending(void);

extern u8 Value_00000075;

s32 Shop_SelUse(s32 actor)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    /* win2 declared ahead of win1 (and both ahead of object) to match the
     * reference's sp+8/sp+12/sp+16 spill-slot order for these three
     * call-result locals; declaring them in call order instead misassigns
     * object and win2 to each other's slots under this compiler. */
    s32 win2;
    s32 win1;
    u8 *object;
    s32 selection;
    s32 redraw;
    s32 count;
    s32 result;
    s32 status;
    s32 flags;
    s32 off;
    void *window;
    s32 x;
    s32 y;

    /* count/result/redraw assigned as statements here (not declaration
     * initializers) so the reference's constant-materialization order is
     * reproduced: a shared movs r1,#1 is committed to count's and redraw's
     * home registers (sl, r9) right before r1 is needed again for win1's
     * call args, in this exact count-then-redraw order. */
    object = Runtime_GetObject(actor);
    count = 1;
    result = 0;
    redraw = 1;
    win1 = UiWindow_CreateFar(14, 8, 16, 4, 2);
    win2 = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->kind = 18;
    shop->mode = 12;
    selection = 0;

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            count = Ability_GetAvailability(actor);
            if (selection > count - 1)
                selection = count - 1;

            /* Split into a plain-int offset statement followed by a separate
             * pointer dereference: writing this as one expression (any
             * algebraic grouping of object+216+selection*2) always makes the
             * compiler fold object into the offset register before the load
             * (adds+adds+ldrh[reg,#0]); computing the byte offset in `off`
             * first keeps object untouched in its own register so the load
             * folds to a single reg+reg ldrh, matching the reference. */
            off = selection * 2 + 216;
            flags = *(u16 *)(object + off) & 0x1ff;
            window = (void *)shop->item_window;
            x = Modulo(selection, 5) << 4;
            y = (Math_Div(selection, 5) << 4) + 8;
            Shop_PlaceCursor(window, x, y);
            shop->mode = 3;
            Shop_DrawUseItem(win1, actor, selection);
            Shop_DrawMsg(win2, flags + (s32)&Value_00000075);
        }

        WaitFrames(1);

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            status = Sys_Apply(actor, selection);
            if (status == 0) {
                Audio_PlayCue(112);
                result = selection;
                goto exit_loop;
            } else if (status == -4) {
                UiWork_Create(0xc96, 8, 1, 2);
            } else if (result == -3) {
                UiWork_Create(0xc97, 8, 1, 2);
            }
            Audio_PlayCue(113);
            while (UiWork_IsCompleteFar() == 0)
                WaitFrames(1);
            UiWork_FinalizePending();
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            goto exit_loop;
        }

        /* selection -= 1 / += 1 as its own statement before the Modulo
         * call (matching sibling main:080b211c's idiom) so the compiler
         * commits the +-1 directly into selection's home register (r7)
         * ahead of the add; folding it into one `selection +- 1 + count`
         * expression instead subtracts/adds 1 from the sum register after
         * the add, which is a different (non-matching) instruction order. */
        if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            Audio_PlayCue(111);
            selection -= 1;
            selection = Modulo(selection + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            Audio_PlayCue(111);
            selection += 1;
            selection = Modulo(selection + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
            selection -= 5;
            if (selection < 0)
                selection += 15;
            while (selection >= count)
                selection -= 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
            selection += 5;
            if (selection >= count)
                selection -= 15;
            while (selection < 0)
                selection += 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
    }

exit_loop:
    UiWindow_Close(win2, 2);
    UiWindow_Close(win1, 2);
    WaitFrames(1);
    return result;
}

/* shop/draw/use_item.c */
extern u8 Value_00000182;
extern u8 Value_00000c94;
extern u8 Value_00000c95;
extern u8 Value_00000c8d;

void *Sys_Run(s32);

void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id)
{
    u8 *unit = Sys_Run(unit_id);
    s32 slot_offset = item_id * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    s32 mult = (*(volatile u16 *)(unit + slot_offset) >> 11) + 1;

    if (window != 0) {
        s32 result;

        UiWindow_Commit(window);
        UiText_DrawAt(masked + (s32)&Value_00000182, window, 0, 0);

        result = Sys_Apply(unit_id, item_id);
        if (result == -4) {
            UiText_DrawAt((s32)&Value_00000c94, window, 0, 8);
        } else if (result == -3) {
            UiText_DrawAt((s32)&Value_00000c95, window, 0, 8);
        } else {
            s32 qty;
            s32 total;

            qty = Shop_SalePrice(*(u16 *)(unit + slot_offset));
            total = mult *qty;

            UiText_DrawAt((s32)&Value_00000c8d, window, 8, 8);
            UiNumber_DrawAt(total, 5, window, 40, 8);
            UiText_DrawAt((s32)&Value_00000c8d - 5, window, 80, 8);
        }
    }
}
