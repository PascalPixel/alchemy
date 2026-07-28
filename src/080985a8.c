#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08098698(void);
void Func_08009080(s32, s32);
void Func_080091e0(void *, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_0809748c(void);

void Func_080985a8(void) {
    u8 *object;
    u8 *flags;
    u8 value;

    object = M2C_FIELD(*(void **)0x03001F30, u8 **, 0x14);
    if (object != 0) {
        Func_08098698();
        Func_08009080((s32)object, 2);
        object[0x59] = 0;
        Func_080091e0(object, 0);
        flags = object + 0x23;
        value = 2;
        value |= *flags;
        *flags = value;
        Func_080030f8(0xAU);
        Func_080f9010(0x7E);
        Func_080030f8(0x28U);
        Func_0809748c();
    }
}
