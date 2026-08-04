#include "types.h"

struct FocusObject {
    u8 unknown_00[5];
    u8 active_05;
};

struct BattleUiState {
    u8 unknown_000[0x17c];
    struct FocusObject *focus_17c;
    u8 unknown_180[0xa0];
    u16 phase_220;
};

struct CombatantState {
    u8 unknown_000[0x48];
    s16 current_values[4][2];
    u8 unknown_058[0xc0];
    u8 first_digits[4];
    u8 second_digits[4];
};

extern struct BattleUiState *Data_03001f2c;

s32 Func_08077290(s32);
struct CombatantState *Func_08077008(s32);
s32 Func_080771f8(s32 combatant, s32 index);
void Func_080030f8(u32 frames);
void Func_08015068();
void Func_08015080(s32 text, s32 window, s32 x, s32 y);
void Func_08015090(const void *resource, s32 window, s32 x, s32 y);
void Func_08015098(const void *resource, s32 window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 width, s32 window, s32 x, s32 y);
void Func_080150b8(s32 color);
void Func_08015280(s32 window, s32 value, s32 x, s32 y, s32 width);
void Func_080a8914(s32 window, s32 combatant, s32 mode);
s32 Func_080a8b10(u8 output[5], s32 requested, s32 combatant);
void Func_080a9d3c(const u8 flags[5]);
s32 Func_080a9dc4(const u8 enabled[5]);

void Func_080a8604(s32 window, s32 combatant, s32 mode)
{
    struct BattleUiState *battle = Data_03001f2c;
    struct CombatantState *state;
    u8 summary[5];
    s32 show_first_column;
    s32 base_row;
    s32 row;
    s32 i;

    show_first_column = Func_08077290(-1) != 0;
    state = Func_08077008(combatant);
    base_row = ((mode & 0xff) == 1) ? 7 : 10;

    battle->focus_17c->active_05 = 1;
    Func_080a8914(window, combatant, mode);
    Func_080a8b10(summary, 1, combatant);
    Func_080a9dc4(summary);

    if ((mode & 0x100) == 0)
        Func_08015068(window, 0, 0x28, 0x60);

    row = 0;
    if (summary[0]) {
        Func_08015080(0xbd5, window, 0x10, 0x28);
        row++;
    }
    if (summary[1]) {
        Func_08015080(0xbd6, window, 0x10, row * 0x10 + 0x28);
        row++;
    }
    if (summary[2]) {
        Func_08015080(0xbd7, window, 0x10, row * 0x10 + 0x28);
        row++;
    }
    if (summary[3]) {
        Func_08015080(0xbd8, window, 0x10, row * 0x10 + 0x28);
        row++;
    }
    if (summary[4]) {
        Func_08015080(0xbd9, window, 0x10, row * 0x10 + 0x28);
        row++;
    }
    if (row == 0)
        Func_08015080(0xbd4, window, 0, 0x28);

    Func_080a9dc4(summary);
    Func_080a9d3c(summary);
    if (battle->phase_220 == 3)
        return;

    if ((mode & 0x100) == 0) {
        Func_080030f8(1);
        Func_08015068(window, 0x40, 0x38, 0xe0, 0x60);
    }

    Func_080150b8(0xf);
    if (mode == 1 || show_first_column) {
        Func_08015280(window, 1, 0x0f, base_row, 4);
        Func_08015280(window, 2, 0x13, base_row, 4);
        Func_08015280(window, 3, 0x17, base_row, 4);
        Func_08015280(window, 4, 0x1b, base_row, 4);
    }

    if (show_first_column)
        Func_08015080(0xafd, window, 0x40, base_row * 8 + 8);

    if (mode == 1) {
        s32 y;

        if (!show_first_column)
            base_row--;
        y = base_row * 8;
        Func_08015090((const void *)0x080af22c, window, 0x40, y + 0x10);
        Func_08015080(0xafe, window, 0x40, y + 0x18);
        Func_08015080(0xaff, window, 0x40, y + 0x20);
    }

    for (i = 0; i < 4; i++) {
        s32 x_first = 0x68 + i * 0x20;
        s32 x_second = 0x78 + i * 0x20;
        s32 y = base_row * 8 + 8;
        s32 value_row = base_row * 8;

        if (show_first_column)
            Func_080150b0(state->first_digits[i], 1, window, x_second, y);

        if ((mode & 0xff) == 1) {
            if (show_first_column) {
                Func_080150b0(
                    state->second_digits[i], 1, window, x_first, y);
                Func_08015098(
                    (const void *)0x080af230, window, x_second - 8, y);
            }
            Func_080150b0(
                Func_080771f8(combatant, i),
                2, window, x_second - 8, value_row + 0x10);
            Func_080150b0(
                state->current_values[i][0],
                3, window, x_first, value_row + 0x18);
            Func_080150b0(
                state->current_values[i][1],
                3, window, x_first, value_row + 0x20);
        }
    }
}
