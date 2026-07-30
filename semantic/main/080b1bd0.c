typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

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

struct CombatantState *Func_08077008(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32, s32);
s32 Func_08077248(s32);
s32 Func_080022fc(s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080b0a6c(void *, s32, s32);
s32 Func_080b19cc(u16);
void Func_080b110c(s32, s32, s32, s32);
void Func_080b11a4(s32, s32);
void Func_080f9010(s32);
void Func_080030f8(u32);
s32 Func_080b1e80(s32, s32);
void Func_080b1f4c(s32, s32);
void Func_080b04dc(s32);

s32 Func_080b1bd0(s32 combatant)
{
    struct MenuState *menu = Data_03001f2c;
    struct CombatantState *state = Func_08077008(combatant);
    s32 grid_window = Func_08015010(0x0f, 8, 0x0f, 4, 2);
    s32 index = 0;
    s32 result;
    volatile s32 branch_barrier = 0;

    do {
        s32 detail_window = Func_08015010(0, 5, 0x1e, 3, 2);
        s32 count = 1;
        s32 redraw = 1;

        menu->focus_380->state_05 = 0x12;
        menu->mode_3a8 = 0x0c;

        for (;;) {
            if (redraw) {
                s32 raw_effect;
                s32 effect;

                redraw = 0;
                count = Func_08077248(combatant);
                if (index > count - 1)
                    index = count - 1;
                raw_effect = state->effects[index];
                effect = raw_effect & 0x1ff;
                Func_080b0a6c(
                    menu->cursor_020,
                    Func_080022fc(index, 5) * 0x10,
                    Func_080022ec(index, 5) * 0x10 + 8);
                menu->mode_3a8 = 3;
                Func_080b110c(
                    grid_window, effect, Func_080b19cc(raw_effect), 1);
                Func_080b11a4(detail_window, effect + 0x75);
            }

            if (*(volatile u32 *)0x03001c94 & 1) {
                Func_080f9010(0x70);
                result = 0;
                break;
            }
            if (*(volatile u32 *)0x03001c94 & 2) {
                Func_080f9010(0x71);
                branch_barrier = 1;
                result = -1;
                break;
            }

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
            Func_080030f8(1);
        }

        Func_08015018(detail_window, 2);
        Func_080030f8(1);
        if (result != 0)
            break;

        if (Func_080b1e80(combatant, index) != -1)
            Func_080b1f4c(combatant, index);
        Func_080b04dc(0xcaa);
    } while (Func_08077248(combatant) != 0);

    Func_08015018(grid_window, 2);
    (void)branch_barrier;
    return result;
}
