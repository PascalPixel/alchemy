#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct UiObject_080a7850 {
    u8 unknown_00[5];
    u8 state_05;
};

struct UiState_080a7850 {
    u8 unknown_000[0x14];
    struct UiObject_080a7850 *object_a_014;
    u8 unknown_018[0x0c];
    s32 grid_window_024;
    u8 unknown_028[0xe4];
    s32 text_window_10c;
    u8 unknown_110[0x34];
    u16 controls_144[4];
    u8 unknown_14c[0x30];
    struct UiObject_080a7850 *object_b_17c;
    u8 unknown_180[0xb4];
    u16 geometry_a_234[4];
    u16 geometry_b_23c[4];
};

typedef char Assert_080a7850_object_state[
    OFFSET_OF(struct UiObject_080a7850, state_05) == 0x05 ? 1 : -1];
typedef char Assert_080a7850_object_a[
    OFFSET_OF(struct UiState_080a7850, object_a_014) == 0x14 ? 1 : -1];
typedef char Assert_080a7850_grid_window[
    OFFSET_OF(struct UiState_080a7850, grid_window_024) == 0x24 ? 1 : -1];
typedef char Assert_080a7850_text_window[
    OFFSET_OF(struct UiState_080a7850, text_window_10c) == 0x10c ? 1 : -1];
typedef char Assert_080a7850_controls[
    OFFSET_OF(struct UiState_080a7850, controls_144) == 0x144 ? 1 : -1];
typedef char Assert_080a7850_object_b[
    OFFSET_OF(struct UiState_080a7850, object_b_17c) == 0x17c ? 1 : -1];
typedef char Assert_080a7850_geometry_a[
    OFFSET_OF(struct UiState_080a7850, geometry_a_234) == 0x234 ? 1 : -1];
typedef char Assert_080a7850_geometry_b[
    OFFSET_OF(struct UiState_080a7850, geometry_b_23c) == 0x23c ? 1 : -1];

extern struct UiState_080a7850 *Data_03001f2c;

void Func_080030f8(s32 frames);
void Func_08015070(s32 window, s32 x, s32 y, s32 width, s32 height);
void Func_08015078(s32 text, s32 window, s32 x, s32 y);
void Func_08015080(s32 text, s32 window, s32 x, s32 y);
void Func_08015270(s32 window);
void Func_08015278(s32 window);
void Func_08015280(
    s32 window, s32 resource, s32 source_index, s32 target_index, s32 mode);
s32 Func_080770c0(s32 flag);
void Func_080a9d84(void);
void Func_080f9010(s32 sound);

/*
 * Display the modal four-by-seven reference grid and restore the surrounding
 * UI geometry after status or user input dismisses it.
 */
void Func_080a7850(void)
{
    struct UiState_080a7850 *state = Data_03001f2c;
    s32 row;

    Func_080a9d84();
    Func_080f9010(0x70);
    Func_08015270(state->text_window_10c);
    Func_08015080(0x0b17, state->text_window_10c, 0, 0x10);
    state->object_a_014->state_05 = 0x0d;
    state->object_b_17c->state_05 = 0x0d;
    Func_080030f8(1);

    for (row = 0; row < 4; row++) {
        state->controls_144[row] = 0x1e;
        state->geometry_a_234[row] = 0x20 + 0x38 * row;
        state->geometry_b_23c[row] = 0x46;
    }

    Func_08015270(state->grid_window_024);
    Func_08015070(state->grid_window_024, 0, 0x0b, 0x1c, 0x0b);
    Func_08015078(0x0b18, state->text_window_10c, -0x60, 0x84);

    for (row = 0; row < 4; row++) {
        s32 column;

        for (column = 0; column < 7; column++) {
            s32 entry = 0x30 + 0x14 * row + column;

            if (Func_080770c0(entry) != 0) {
                Func_08015280(
                    state->grid_window_024,
                    0x1001 + row,
                    1 + 7 * row,
                    3 + column,
                    0);
                Func_08015080(
                    0x045f + 0x14 * row + column,
                    state->grid_window_024,
                    0x10 + 0x38 * row,
                    0x18 + 8 * column);
            }
        }
    }

    (*(volatile u8 **)0x03001e8c)[0x0ea3] = 1;
    for (;;) {
        if (Func_080770c0(0x150) != 0)
            break;
        Func_080030f8(1);
        if ((*(volatile u32 *)0x03001c94 & 7) != 0)
            break;
    }

    Func_08015270(state->grid_window_024);
    Func_08015278(state->text_window_10c);
    for (row = 0; row < 4; row++) {
        state->geometry_a_234[row] = 0x82 + 0x20 * row;
        state->geometry_b_23c[row] = 0x80;
    }
    state->object_a_014->state_05 = 1;
    state->object_b_17c->state_05 = 1;
    Func_080f9010(0x71);
}
