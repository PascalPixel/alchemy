#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct FocusObject_080b3444 {
    u8 unknown_00[4];
    u8 value_04;
    u8 state_05;
};

struct MenuState_080b3444 {
    u8 unknown_000[0x20];
    s32 primary_window_020;
    u8 unknown_024[0x34a];
    s16 option_values_36e[9];
    struct FocusObject_080b3444 *focus_380;
    u8 unknown_384[0x0c];
    u16 resource_390;
    u8 unknown_392[0x15];
    s8 option_count_3a7;
    u8 ui_state_3a8;
};

typedef char Assert_080b3444_primary_window[
    OFFSET_OF(struct MenuState_080b3444, primary_window_020) == 0x20
        ? 1 : -1];
typedef char Assert_080b3444_option_values[
    OFFSET_OF(struct MenuState_080b3444, option_values_36e) == 0x36e
        ? 1 : -1];
typedef char Assert_080b3444_focus[
    OFFSET_OF(struct MenuState_080b3444, focus_380) == 0x380
        ? 1 : -1];
typedef char Assert_080b3444_resource[
    OFFSET_OF(struct MenuState_080b3444, resource_390) == 0x390
        ? 1 : -1];
typedef char Assert_080b3444_option_count[
    OFFSET_OF(struct MenuState_080b3444, option_count_3a7) == 0x3a7
        ? 1 : -1];
typedef char Assert_080b3444_ui_state[
    OFFSET_OF(struct MenuState_080b3444, ui_state_3a8) == 0x3a8
        ? 1 : -1];

extern struct MenuState_080b3444 *Data_03001f2c;

s32 Func_080022fc(s32 value, s32 divisor);
void Func_080030f8(s32 frames);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32 window, s32 mode);
struct FocusObject_080b3444 *Func_080150c8(
    u16 resource, u32 flags, s32 window, s32 x, s32 y);
s32 Func_08077248(s32 value);
void Func_080a1028(s32 window, s32, s32, s32, s32);
void Func_080a1030(void);
void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b0a20(void *focus_slot, s32 x, s32 y);
void Func_080b0a6c(s32 window, s32 x, s32 y);
void Func_080b11c4(s32 window, s32 selection, s32 mode);
void Func_080b1dec(s32 window, s16 value);
s32 Func_080b362c(s32 value);
void Func_080f9010(s32 sound);

/*
 * Present a cyclic value picker. Confirmation returns the selected value and
 * its mapped result through the two output pointers; cancellation writes -1
 * to both outputs and returns -1.
 */
s32 Func_080b3444(s32 *out_value, s32 *out_mapped)
{
    struct MenuState_080b3444 *state;
    struct FocusObject_080b3444 *focus;
    s32 secondary_window;
    s32 selection = 0;
    s32 redraw = 1;
    s32 result = 0;
    s16 value = 0;

    Func_080b010c();
    state = Data_03001f2c;

    state->primary_window_020 = Func_08015010(0x10, 0x0c, 0x0e, 8, 2);
    secondary_window = Func_08015010(0, 0x0e, 0x0d, 3, 2);
    focus = Func_080150c8(
        state->resource_390, 0x40000000, secondary_window, 0, 0);
    focus->state_05 = 4;
    focus->value_04 = 0;
    Func_080b0a20(&state->focus_380, -0x20, 0x70);
    state->focus_380 = focus;
    state->ui_state_3a8 = 0x0c;
    Func_080a1028(secondary_window, 2, 0, 8, 0);

    for (;;) {
        if (redraw != 0) {
            s32 count = state->option_count_3a7;

            redraw = 0;
            selection = Func_080022fc(selection + count, count);
            value = state->option_values_36e[selection];
            Func_080b0a6c(secondary_window, selection * 0x18 - 0x0c, 0);
            state->ui_state_3a8 = 3;
            Func_080b11c4(secondary_window, selection, 0);
            Func_080b1dec(state->primary_window_020, value);
        }

        Func_080030f8(1);

        if ((*(volatile u32 *)0x03001c94 & 1) != 0) {
            s32 mapped;

            if (Func_08077248(value) == 0) {
                Func_080f9010(0x71);
                continue;
            }

            Func_080f9010(0x70);
            mapped = Func_080b362c(value);
            if (mapped == -1) {
                state->focus_380->state_05 = 4;
                state->ui_state_3a8 = 0x0c;
                redraw = 1;
                continue;
            }

            *out_value = value;
            *out_mapped = mapped;
            result = 0;
            break;
        }

        if ((*(volatile u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            *out_value = -1;
            *out_mapped = -1;
            result = -1;
            break;
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
    }

    Func_080a1030();
    Func_08015018(secondary_window, 2);
    Func_08015018(state->primary_window_020, 2);
    Func_080030f8(1);
    Func_080b0204();
    return result;
}
