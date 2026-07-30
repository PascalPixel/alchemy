#include "types.h"

typedef void (*Callback_080994d0)(void);

void Func_080030f8(s32 frames);
void Func_08003f3c(s32 value);
void Func_080090d0(void *object);
void Func_08009158(void *object);
void Func_08009240(void *object, s32 mode);
void Func_08096bec(void *object, s32 arg1, s32 arg2);
void *Func_08096c48(void *value, void *previous);
u8 *Func_08096c80(s32 type, s32 x, s32 y, s32 z);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080f9010(s32 sound);

void Func_080994d0(void)
{
    u8 *state = *(u8 **)0x03001f30;
    u8 *source = *(u8 **)(state + 16);
    void *chain = 0;
    u8 *object;
    s32 saved_value;
    s32 index;

    Func_08097384();
    for (index = 0; index < 8; index++) {
        object = Func_08096c80(
            233,
            *(s32 *)(source + 8),
            *(s32 *)(source + 12) + 0x200000,
            *(s32 *)(source + 16));
        if (object != 0) {
            *(s32 *)(object + 24) = 0xb333;
            *(s32 *)(object + 28) = 0xb333;
            *(s32 *)(object + 108) = 0x08099341;
            *(u16 *)(object + 100) = 120;
            *(u16 *)(object + 102) = index << 13;
            object[85] = 4;
            chain = Func_08096c48(*(void **)(object + 80), chain);
        }
        Func_080030f8(1);
    }

    saved_value = *((u8 *)chain + 28);
    Func_080f9010(130);
    Func_080030f8(110);

    object = Func_08096c80(233, 0, 0, 0);
    if (object != 0) {
        *(s32 *)(object + 24) = 0xb333;
        *(s32 *)(object + 28) = 0xb333;
        *(s32 *)(object + 8) = *(s32 *)(state + 4);
        *(s32 *)(object + 12) = *(s32 *)(state + 8) + 0x100000;
        *(s32 *)(object + 16) = *(s32 *)(state + 12);
        object[85] = 4;
        Func_08009240(object, 7);
    }

    Func_080f9010(131);
    Func_080030f8(12);
    if (object != 0) {
        for (index = 0; index < 30; index++) {
            if ((index & 3) != 0)
                Func_08009240(object, 9);
            else
                Func_08009240(object, 10);
            Func_080030f8(2);
        }
    }

    Func_08009240(object, 0);
    Func_080f9010(84);
    if (object != 0) {
        *(s32 *)(object + 108) = 0x080993b1;
        *(u16 *)(object + 100) = 0;
        if ((s8)state[32] != 0)
            Func_080030f8(128);
        else
            Func_080030f8(192);
    }

    if (object != 0) {
        *(u16 *)(object + 100) = 0xffff;
        *(s32 *)(object + 48) = 0x50000;
        *(s32 *)(object + 52) = 0x6666;
        object[90] = 0;
        Func_08096bec(object, 0x00c00000, 0x0000e800);
        Func_08009158(object);
        Func_080090d0(object);
    }

    if (saved_value != 96)
        Func_08003f3c(saved_value);

    if (*(u32 *)(state + 36) != 0) {
        Callback_080994d0 callback =
            (Callback_080994d0)*(u32 *)(state + 36);
        callback();
    }
    Func_0809748c();
}
