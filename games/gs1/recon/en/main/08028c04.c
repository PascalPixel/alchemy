#include "types.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern void *Data_03001f38;

void *Func_080284dc(void);
void Menu_AppendResourceEntry(s32);
void Func_08028808(s32, s32, s32);
s32 Func_080041d8(u32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e74c(s32, s32, s32, s32);
s32 Func_08028574(s32);
void Func_08004278(u32);
void Func_08016478(s32);
void Func_08016418(s32, s32);
void Func_080030f8(s32);
void Func_0802851c(void);

s32 Func_08028c04(s32 mode)
{
    void *state;
    s32 window_a;
    s32 window_b;
    s32 msg_base;
    s32 *win_b_field;
    s32 value;

    Func_080284dc();
    state = Data_03001f38;
    if (mode == 0) {
        Menu_AppendResourceEntry(0x2C);
        Menu_AppendResourceEntry(0x2D);
    } else {
        Menu_AppendResourceEntry(0x2E);
        Menu_AppendResourceEntry(0x2F);
        Menu_AppendResourceEntry(0x30);
    }
    Func_08028808(0x11, 7, 0);
    if (mode != 0) {
        Func_080041d8(0x08028AA9, 0xC76);
        M2C_FIELD(state, u16 *, 0x96) = 0xFFFF;
        window_b = Func_080162d4(7, 0, 0x11, 4, 2);
        msg_base = 0xC77;
        win_b_field = (s32 *)((u8 *)state + 0x80);
        *win_b_field = window_b;
        Func_0801e74c(msg_base, window_b, 0, 4);
        window_a = Func_080162d4(3, 4, 0x19, 0xC, 2);
        M2C_FIELD(state, s32 *, 0x7C) = window_a;
        Func_0801e74c(msg_base + 1, window_a, 8, 0);
        Func_0801e74c(msg_base + 2, M2C_FIELD(state, s32 *, 0x7C), 8, 0xB);
        msg_base += 3;
        Func_0801e74c(msg_base, M2C_FIELD(state, s32 *, 0x7C), 8, 0x16);
    } else {
        Func_080041d8(0x08028B81, 0xC76);
        M2C_FIELD(state, u16 *, 0x96) = 0xFFFF;
        window_b = Func_080162d4(6, 0, 0x12, 4, 2);
        win_b_field = (s32 *)((u8 *)state + 0x80);
        *win_b_field = window_b;
        Func_0801e74c(0xC76, window_b, 2, 4);
        window_a = Func_080162d4(1, 5, 0x1C, 7, 2);
        M2C_FIELD(state, s32 *, 0x7C) = window_a;
    }
    value = Func_08028574(0);
    if (mode != 0) {
        Func_08004278(0x08028AA9);
    } else {
        Func_08004278(0x08028B81);
    }
    Func_08016478(*win_b_field);
    Func_08016478(M2C_FIELD(state, s32 *, 0x7C));
    Func_08016418(*win_b_field, 2);
    Func_08016418(M2C_FIELD(state, s32 *, 0x7C), 2);
    Func_080030f8(1);
    Func_0802851c();
    return value;
}
