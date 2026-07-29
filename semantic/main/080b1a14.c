#include "layout_guard.h"
#include "types.h"

typedef struct Cursor_080b1a14 {
    u8 padding00[5];
    u8 kind;
} Cursor_080b1a14;

typedef struct BattleMenuState_080b1a14 {
    u8 padding000[0x0c];
    s32 base_window;
    u8 padding010[0x10];
    s32 detail_window;
    u8 padding024[0x34a];
    s16 actors[9];
    Cursor_080b1a14 *cursor;
    u8 padding384[0x23];
    s8 actor_count;
    u8 mode;
    u8 padding3a9;
    s8 dispatcher_variant;
} BattleMenuState_080b1a14;

LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_BaseWindow,
    BattleMenuState_080b1a14,
    base_window,
    0x0c);
LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_DetailWindow,
    BattleMenuState_080b1a14,
    detail_window,
    0x20);
LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_Actors,
    BattleMenuState_080b1a14,
    actors,
    0x36e);
LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_Cursor,
    BattleMenuState_080b1a14,
    cursor,
    0x380);
LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_ActorCount,
    BattleMenuState_080b1a14,
    actor_count,
    0x3a7);
LAYOUT_OFFSET_GUARD(
    BattleMenuState080b1a14_Variant,
    BattleMenuState_080b1a14,
    dispatcher_variant,
    0x3aa);
LAYOUT_OFFSET_GUARD(
    Cursor080b1a14_Kind,
    Cursor_080b1a14,
    kind,
    5);

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
s32 Func_08077248(s32);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
void Func_080b0a6c(s32, s32, s32);
void Func_080b10cc(void);
void Func_080b11c4(s32, s32, s32);
void Func_080b1bd0(s32);
void Func_080b1dec(s32, s32);
void Func_080b211c(s32);
void Func_080f9010(s32);

/*
 * Keep an actor-selection menu active while dispatching the chosen actor into
 * one of two action screens.  The menu itself closes only when cancelled.
 */
s32 Func_080b1a14(void)
{
    BattleMenuState_080b1a14 *state =
        *(BattleMenuState_080b1a14 **)0x03001f2c;
    s32 list_window;
    s32 selection = 0;
    s32 actor = 0;
    s32 redraw = 1;

    state->base_window = Func_08015010(0, 9, 12, 4, 2);
    Func_080b10cc();
    state->detail_window = Func_08015010(16, 12, 14, 8, 2);
    list_window = Func_08015010(0, 14, 13, 3, 2);
    state->cursor->kind = 4;
    state->mode = 12;
    Func_080a1028(list_window, 2, 0, 8, 0);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = Func_080022fc(
                selection + state->actor_count,
                state->actor_count);
            actor = state->actors[selection];
            Func_080b0a6c(list_window, selection * 24 - 12, 0);
            state->mode = 3;
            Func_080b11c4(list_window, selection, 0);
            Func_080b1dec(state->detail_window, actor);
        }

        if ((*(volatile u32 *)0x03001c94 & 1) != 0) {
            Func_080030f8(1);
            if (Func_08077248(actor) == 0) {
                Func_080f9010(0x71);
            } else {
                Func_080f9010(0x70);
                if (state->dispatcher_variant == 1)
                    Func_080b1bd0(actor);
                else
                    Func_080b211c(actor);
                state->cursor->kind = 4;
                state->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((*(volatile u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            Func_080a1030();
            Func_08015018(list_window, 2);
            Func_08015018(state->detail_window, 2);
            Func_08015018(state->base_window, 2);
            Func_080030f8(1);
            return 0;
        }

        if ((*(volatile u32 *)0x03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            selection--;
            redraw = 1;
        }
        if ((*(volatile u32 *)0x03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            selection++;
            redraw = 1;
        }
        Func_080030f8(1);
    }
}
