#include "layout_guard.h"
#include "types.h"

typedef struct EconomyState_080b24e4 {
    u8 padding000[0x118];
    s32 available;
    s8 amount_index;
} EconomyState_080b24e4;

typedef struct BattleState_080b24e4 {
    u8 padding000[0x36e];
    s16 actors[24];
    u16 current_item;
    u8 padding3a0[7];
    s8 actor_count;
    u8 display_mode;
} BattleState_080b24e4;

LAYOUT_OFFSET_GUARD(
    EconomyState080b24e4_Available,
    EconomyState_080b24e4,
    available,
    0x118);
LAYOUT_OFFSET_GUARD(
    EconomyState080b24e4_AmountIndex,
    EconomyState_080b24e4,
    amount_index,
    0x11c);
LAYOUT_OFFSET_GUARD(
    BattleState080b24e4_Actors,
    BattleState_080b24e4,
    actors,
    0x36e);
LAYOUT_OFFSET_GUARD(
    BattleState080b24e4_CurrentItem,
    BattleState_080b24e4,
    current_item,
    0x39e);
LAYOUT_OFFSET_GUARD(
    BattleState080b24e4_ActorCount,
    BattleState_080b24e4,
    actor_count,
    0x3a7);

extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015120(s32, s32);
s32 Func_08077028(s32, u16);
void Func_08077058(s32);
s32 Func_08077248(s32);
void Func_08077298(s32);
void Func_080772a0(s32);
void Func_080b04dc(s32);
void Func_080b0574(s32);
void Func_080b0a6c(void *, s32, s32);
void Func_080b11c4(void *, s32, u16);
void Func_080b1470(void *, s32, u16);
void Func_080f9010(s32);

/*
 * Offer a fixed item to one member of the current actor list.  The list and
 * detail windows are refreshed as the selection moves; the saved item is
 * restored after either a successful transaction or cancellation.
 */
void Func_080b24e4(void *list_window, void *detail_window)
{
    BattleState_080b24e4 *state =
        *(BattleState_080b24e4 **)0x03001f2c;
    EconomyState_080b24e4 *economy =
        (EconomyState_080b24e4 *)0x02000240;
    const s16 *amount_table = (const s16 *)0x080b4146;
    u16 saved_item = state->current_item;
    s32 amount = amount_table[economy->amount_index];
    s32 selection = 0;
    s32 actor = 0;
    s32 redraw = 1;

    if (amount > economy->available)
        return;

    state->current_item = 0x00e4;
    Func_08015120(0x00e4, 2);
    Func_080b0574(0x0cc3);
    Func_08015120(state->current_item, 2);
    Func_080b0574(0x0cc4);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = Func_080022fc(
                selection + state->actor_count,
                state->actor_count);
            actor = state->actors[selection];
            Func_080b0a6c(list_window, selection * 24 - 12, 0);
            state->display_mode = 3;
            Func_080b11c4(
                list_window,
                selection,
                state->current_item);
            Func_080b1470(detail_window, actor, state->current_item);
        }

        if ((Data_03001c94 & 1) != 0) {
            if (Func_08077028(actor, state->current_item) < 0) {
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
            Func_080f9010(0x65);
            Func_080b0574(0x0ca1);
            Func_08077028(actor, state->current_item);
            Func_08077298(-amount);
            Func_080772a0(1);
            break;
        }

        if ((Data_03001c94 & 2) != 0) {
            Func_080b0574(0x0cc5);
            Func_080f9010(0x71);
            break;
        }

        if ((Data_03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            selection--;
            redraw = 1;
        }
        if ((Data_03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            selection++;
            redraw = 1;
        }
        Func_080030f8(1);
    }

    state->current_item = saved_item;
}
