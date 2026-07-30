#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080284dc(void);
void Func_080287a8(s32);
void Func_08028808(s32, s32, s32);
void Func_080041d8(const void *, s32);
void Func_08004278(const void *);
void *Func_080162d4(u32, u32, u32, u32, u32);
void Func_0801e74c(s32, void *, s32, s32);
s32 Func_08028574(s32);
void Func_08016478(void *);
void Func_08016418(void *, s32);
void Func_080030f8(s32);
void Func_0802851c(void);

/*
 * Build and run one of the two multi-window selection dialogs, then close
 * both windows and return the selector's result.
 */
s32 Func_08028c04(s32 extended)
{
    u8 *state;
    void *primary;
    void *secondary;
    s32 result;

    Func_080284dc();
    state = *(u8 **)0x03001F38;
    if (extended == 0) {
        Func_080287a8(0x2C);
        Func_080287a8(0x2D);
    } else {
        Func_080287a8(0x2E);
        Func_080287a8(0x2F);
        Func_080287a8(0x30);
    }
    Func_08028808(0x11, 7, 0);

    if (extended != 0) {
        Func_080041d8((const void *)0x08028AA9, 0xC76);
        M2C_FIELD(state, u16 *, 0x96) = 0xFFFF;
        primary = Func_080162d4(7, 0, 0x11, 4, 2);
        M2C_FIELD(state, void **, 0x80) = primary;
        Func_0801e74c(0xC77, primary, 0, 4);

        secondary = Func_080162d4(3, 4, 0x19, 0xC, 2);
        M2C_FIELD(state, void **, 0x7C) = secondary;
        Func_0801e74c(0xC78, secondary, 8, 0);
        Func_0801e74c(0xC79, secondary, 8, 0xB);
        Func_0801e74c(0xC7A, secondary, 8, 0x16);
    } else {
        Func_080041d8((const void *)0x08028B81, 0xC76);
        M2C_FIELD(state, u16 *, 0x96) = 0xFFFF;
        primary = Func_080162d4(6, 0, 0x12, 4, 2);
        M2C_FIELD(state, void **, 0x80) = primary;
        Func_0801e74c(0xC76, primary, 2, 4);

        secondary = Func_080162d4(1, 5, 0x1C, 7, 2);
        M2C_FIELD(state, void **, 0x7C) = secondary;
    }

    result = Func_08028574(0);
    if (extended != 0) {
        Func_08004278((const void *)0x08028AA9);
    } else {
        Func_08004278((const void *)0x08028B81);
    }

    primary = M2C_FIELD(state, void **, 0x80);
    secondary = M2C_FIELD(state, void **, 0x7C);
    Func_08016478(primary);
    Func_08016478(secondary);
    Func_08016418(primary, 2);
    Func_08016418(secondary, 2);
    Func_080030f8(1);
    Func_0802851c();
    return result;
}
