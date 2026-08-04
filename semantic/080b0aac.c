#include "layout_guard.h"
#include "types.h"

typedef struct ItemMetadata_080b0aac {
    s16 value;
} ItemMetadata_080b0aac;

typedef struct Cursor_080b0aac {
    u8 padding00[5];
    u8 kind;
} Cursor_080b0aac;

typedef struct MenuState_080b0aac {
    u8 padding000[0x0c];
    s32 base_window;
    u8 padding010[0x10];
    s32 detail_window;
    u8 padding024[0x248];
    s16 items[129];
    s16 actors[24];
    u16 current_item;
    u8 padding3a0[6];
    s8 item_count;
    s8 actor_count;
    u8 display_mode;
    u8 padding3a9;
    s8 transaction_mode;
} MenuState_080b0aac;

typedef struct CursorOwner_080b0aac {
    u8 padding000[0x380];
    Cursor_080b0aac *cursor;
} CursorOwner_080b0aac;

typedef struct EconomyState_080b0aac {
    u8 padding00[0x10];
    u32 available;
} EconomyState_080b0aac;

LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_BaseWindow,
    MenuState_080b0aac,
    base_window,
    0x0c);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_DetailWindow,
    MenuState_080b0aac,
    detail_window,
    0x20);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_Items,
    MenuState_080b0aac,
    items,
    0x26c);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_Actors,
    MenuState_080b0aac,
    actors,
    0x36e);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_CurrentItem,
    MenuState_080b0aac,
    current_item,
    0x39e);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_ItemCount,
    MenuState_080b0aac,
    item_count,
    0x3a6);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_ActorCount,
    MenuState_080b0aac,
    actor_count,
    0x3a7);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_DisplayMode,
    MenuState_080b0aac,
    display_mode,
    0x3a8);
LAYOUT_OFFSET_GUARD(
    MenuState080b0aac_TransactionMode,
    MenuState_080b0aac,
    transaction_mode,
    0x3aa);
LAYOUT_OFFSET_GUARD(
    CursorOwner080b0aac_Cursor,
    CursorOwner_080b0aac,
    cursor,
    0x380);
LAYOUT_OFFSET_GUARD(
    Cursor080b0aac_Kind,
    Cursor_080b0aac,
    kind,
    5);
LAYOUT_OFFSET_GUARD(
    EconomyState080b0aac_Available,
    EconomyState_080b0aac,
    available,
    0x10);

extern MenuState_080b0aac *Data_03001f2c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
void Func_08015120(s32, s32);
void Func_08015270(s32);
ItemMetadata_080b0aac *Func_08077018(s32);
s32 Func_08077028(s32, u16);
void Func_08077058(s32);
s32 Func_08077220(s32, u16);
s32 Func_08077238(u16);
void Func_08077240(u16, s32);
s32 Func_08077248(s32);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
s32 Func_080b0070(void);
void Func_080b04dc(s32);
void Func_080b0574(s32);
s32 Func_080b0634(s32);
void Func_080b0a6c(s32, s32, s32);
void Func_080b0fa4(s32, s32);
void Func_080b10cc(void);
void Func_080b110c(s32, s32, s32, s32);
void Func_080b11a4(s32, s32);
void Func_080b11c4(s32, s32, u16);
void Func_080b1260(s32, s32, u16);
void Func_080b1470(s32, s32, u16);
s32 Func_080b153c(s32, u16);
void Func_080b17e4(s32, u16, s32);
void Func_080b24e4(s32, s32);
void Func_080f9010(s32);

/*
 * Drive the two-stage item transaction screen.  The first grid chooses an
 * item; the second list chooses an actor and validates/applies the transfer.
 * A completed transfer can return to the item grid for another transaction,
 * while cancellation from the item grid closes the whole screen.
 */
s32 Func_080b0aac(void)
{
    MenuState_080b0aac *state = Data_03001f2c;
    CursorOwner_080b0aac *cursor_owner =
        (CursorOwner_080b0aac *)state;
    EconomyState_080b0aac *economy =
        (EconomyState_080b0aac *)0x02000240;
    s32 item_grid;
    s32 item_index = 0;

    state->detail_window = 0;
    state->base_window = Func_08015010(18, 7, 12, 4, 2);
    Func_080b10cc();
    item_grid = Func_08015010(0, 8, 15, 4, 2);

    for (;;) {
        s32 item_detail = Func_08015010(0, 12, 30, 4, 2);
        s32 item_help;
        s32 redraw = 1;
        s32 item = 0;
        ItemMetadata_080b0aac *metadata = 0;
        s32 item_result;

        cursor_owner->cursor->kind = 0x12;
        state->display_mode = 12;
        item_help = Func_08015010(0, 17, 30, 3, 2);

        for (;;) {
            if (redraw != 0) {
                s32 column;

                redraw = 0;
                item = state->items[item_index];
                metadata = Func_08077018(item);
                column = Func_080022fc(item_index, 7);
                Func_080b0a6c(
                    item_detail,
                    column * 32 - 8,
                    8);
                state->display_mode = 4;
                Func_080b0fa4(item_detail, item_index);
                Func_080b11a4(item_help, item + 0x75);
                Func_08015270(item_grid);
                Func_080b110c(item_grid, item, metadata->value, 0);
            }

            if ((Data_03001c94 & 1) != 0) {
                Func_080f9010(0x70);
                item_result = 0;
                break;
            }
            if ((Data_03001c94 & 2) != 0) {
                Func_080f9010(0x71);
                item_result = -1;
                break;
            }

            if ((Data_03001b04 & 0x20) != 0) {
                s32 previous = item_index;

                item_index = Func_080022fc(
                    item_index - 1 + state->item_count,
                    state->item_count);
                if (previous != item_index) {
                    Func_080f9010(0x6f);
                    redraw = 1;
                }
            }
            if ((Data_03001b04 & 0x10) != 0) {
                s32 previous = item_index;

                item_index = Func_080022fc(
                    item_index + 1 + state->item_count,
                    state->item_count);
                if (previous != item_index) {
                    Func_080f9010(0x6f);
                    redraw = 1;
                }
            }
            if ((Data_03001b04 & 0x40) != 0 &&
                item_index >= 7) {
                item_index -= 7;
                redraw = 1;
            }
            if ((Data_03001b04 & 0x80) != 0) {
                s32 row_count =
                    Func_080022ec(state->item_count + 6, 7);
                s32 next = item_index + 7;

                if (next < row_count * 7) {
                    item_index = next;
                    redraw = 1;
                }
                if (item_index > state->item_count - 1)
                    item_index = state->item_count - 1;
            }
            Func_080030f8(1);
        }

        Func_08015018(item_help, 2);
        Func_08015018(item_detail, 2);
        Func_080030f8(1);

        if (item_result != 0)
            break;

        state->current_item = state->items[item_index];
        Func_080b04dc(0x0c9d);
        metadata = Func_08077018(state->current_item);

        {
            s32 actor_list = Func_08015010(0, 14, 13, 3, 2);
            s32 actor_detail;
            s32 actor_index = 0;
            s32 actor = 0;
            s32 quantity = 1;
            s32 redraw_actor = 1;
            s32 refresh_message = 0;
            s32 actor_result;

            cursor_owner->cursor->kind = 4;
            state->display_mode = 12;
            Func_080a1028(actor_list, 2, 0, 8, 0);
            actor_detail = Func_08015010(16, 11, 14, 9, 2);

            for (;;) {
                if (refresh_message != 0) {
                    refresh_message = 0;
                    Func_080b04dc(0x0c9d);
                    redraw_actor = 1;
                }

                if (redraw_actor != 0) {
                    redraw_actor = 0;
                    actor_index = Func_080022fc(
                        actor_index + state->actor_count,
                        state->actor_count);
                    actor = state->actors[actor_index];
                    Func_080b0a6c(
                        actor_list,
                        actor_index * 24 - 12,
                        0);
                    state->display_mode = 3;
                    Func_080b11c4(
                        actor_list,
                        actor_index,
                        state->current_item);
                    if (Func_08077238(state->current_item) == 0)
                        Func_080b1470(
                            actor_detail,
                            actor,
                            state->current_item);
                    else
                        Func_080b1260(
                            actor_detail,
                            actor,
                            state->current_item);
                }

                if ((Data_03001c94 & 1) != 0) {
                    s32 compatibility =
                        Func_08077028(actor, state->current_item);

                    if (compatibility < 0) {
                        Func_080f9010(0x71);
                        Func_08015120(actor, 1);
                        Func_08015120(state->current_item, 2);
                        if (Func_08077248(actor) == 15)
                            Func_080b04dc(0x0c9e);
                        else
                            Func_080b04dc(0x0ca6);
                        continue;
                    }

                    Func_08077058(actor);
                    if ((u32)metadata->value > economy->available) {
                        Func_080f9010(0x71);
                        Func_080b0574(0x0c9c);
                        actor_result = -1;
                        break;
                    }

                    if (Func_08077220(
                            actor,
                            state->current_item) == 0) {
                        Func_08015120(actor, 1);
                        Func_080b04dc(0x0c9f);
                        refresh_message = 1;
                        if (Func_080b0634(0) != 0)
                            continue;
                    }

                    Func_080f9010(0x70);
                    Func_080030f8(1);
                    quantity =
                        Func_080b153c(actor, state->current_item);
                    refresh_message = 1;
                    if (quantity == -1)
                        continue;

                    Func_080b17e4(
                        actor,
                        state->current_item,
                        quantity);
                    Func_080b24e4(actor_list, actor_detail);
                    actor_result = 0;
                    break;
                }

                if ((Data_03001c94 & 2) != 0) {
                    Func_080f9010(0x71);
                    actor_result = -1;
                    break;
                }

                if ((Data_03001b04 & 0x20) != 0) {
                    Func_080f9010(0x6f);
                    actor_index--;
                    redraw_actor = 1;
                }
                if ((Data_03001b04 & 0x10) != 0) {
                    Func_080f9010(0x6f);
                    actor_index++;
                    redraw_actor = 1;
                }
                Func_080030f8(1);
            }

            Func_080a1030();
            Func_08015018(actor_detail, 2);
            Func_08015018(actor_list, 2);
            Func_080030f8(1);

            if (actor_result == 0 &&
                state->transaction_mode == 2) {
                s32 remaining = quantity;

                while (remaining > 0) {
                    Func_08077240(state->current_item, -1);
                    remaining--;
                }

                if (Func_080b0070() == 0)
                    break;

                if (item_index > state->item_count - 1)
                    item_index = state->item_count - 1;
            }
        }

        Func_080b04dc(0x0ca8);
    }

    Func_08015018(item_grid, 2);
    Func_08015018(state->base_window, 2);
    Func_080030f8(1);
    return 0;
}
