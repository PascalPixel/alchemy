#include "types.h"

struct FocusObject {
    u8 unknown_00[5];
    u8 state_05;
};

struct MenuState {
    u8 unknown_000[0x20];
    void *cursor_020;
    u8 unknown_024[0x35c];
    struct FocusObject *focus_380;
    u8 unknown_384[0x24];
    s8 mode_3a8;
};

struct CombatantState {
    u8 unknown_000[0xd8];
    u16 effects[15];
};

extern struct MenuState *Data_03001f2c;

struct CombatantState *Func_08077008(s32 combatant);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_08015140(void);
s32 Func_08077248(s32 combatant);
s32 Func_080772a8(s32 combatant, s32 index);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_080022ec(s32 value, s32 divisor);
void Func_080b0a6c(void *, s32, s32);
void Func_080b386c(s32, s32, s32);
void Func_080b11a4(s32, s32);
void Func_080030f8(u32);
void Func_080f9010(s32);

s32 Func_080b362c(s32 combatant)
{
    struct MenuState *menu = Data_03001f2c;
    struct CombatantState *state = Func_08077008(combatant);
    s32 result = 0;
    s32 count = 1;
    s32 index = 0;
    s32 redraw = 1;
    s32 grid_window = Func_08015010(0x0e, 8, 0x10, 4, 2);
    s32 detail_window = Func_08015010(0, 5, 0x1e, 3, 2);
    volatile s32 branch_barrier = 0;

    menu->focus_380->state_05 = 0x12;
    menu->mode_3a8 = 0x0c;

    for (;;) {
        if (redraw) {
            s32 effect;

            redraw = 0;
            count = Func_08077248(combatant);
            if (index > count - 1)
                index = count - 1;
            effect = state->effects[index] & 0x1ff;
            Func_080b0a6c(
                menu->cursor_020,
                Func_080022fc(index, 5) * 0x10,
                Func_080022ec(index, 5) * 0x10 + 8);
            menu->mode_3a8 = 3;
            Func_080b386c(grid_window, combatant, index);
            Func_080b11a4(detail_window, effect + 0x75);
        }

        Func_080030f8(1);
        if (*(volatile u32 *)0x03001c94 & 1) {
            s32 status = Func_080772a8(combatant, index);

            if (status == 0) {
                Func_080f9010(0x70);
                result = index;
                break;
            }

            if (status == -4) {
                Func_08015038(0xc96, 8, 1, 2);
                branch_barrier = 1;
            } else if (result == -3) {
                Func_08015038(0xc97, 8, 1, 2);
                branch_barrier = 2;
            }
            Func_080f9010(0x71);
            branch_barrier = 3;
            while (Func_08015048() == 0)
                Func_080030f8(1);
            Func_08015140();
            redraw = 1;
        } else if (*(volatile u32 *)0x03001c94 & 2) {
            Func_080f9010(0x71);
            branch_barrier = 4;
            result = -1;
            break;
        } else {
            if (*(volatile u32 *)0x03001b04 & 0x20) {
                Func_080f9010(0x6f);
                index = Func_080022fc(index - 1 + count, count);
                redraw = 1;
            }
            if (*(volatile u32 *)0x03001b04 & 0x10) {
                Func_080f9010(0x6f);
                index = Func_080022fc(index + 1 + count, count);
                redraw = 1;
            }
            if (*(volatile u32 *)0x03001b04 & 0x40) {
                index -= 5;
                if (index < 0)
                    index += 15;
                while (index >= count)
                    index -= 5;
                Func_080f9010(0x6f);
                redraw = 1;
            }
            if (*(volatile u32 *)0x03001b04 & 0x80) {
                index += 5;
                if (index >= count)
                    index -= 15;
                while (index < 0)
                    index += 5;
                Func_080f9010(0x6f);
                redraw = 1;
            }
        }
    }

    Func_08015018(detail_window, 2);
    Func_08015018(grid_window, 2);
    Func_080030f8(1);
    (void)branch_barrier;
    return result;
}
