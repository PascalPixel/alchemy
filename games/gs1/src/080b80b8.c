#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 Data_03001cd4;

s32 Func_080022ec(s32, s32);
void Func_08009150(void *, s32, s32, s32);
void Func_08009080(s32, s32);

void Func_080b80b8(void **start_slot, void **end_slot, s32 progress) {
    s32 z_step;
    s32 start_x;
    s32 x;
    s32 start_z;
    s32 end_z;
    void *start;
    void *end;

    start = *start_slot;
    end = *end_slot;
    start_x = M2C_FIELD(start, s32, 8);
    x = start_x + Func_080022ec(progress * (M2C_FIELD(end, s32, 8) - start_x), 0x64);
    end_z = M2C_FIELD(end, s32, 0x10);
    start_z = M2C_FIELD(start, s32, 0x10);
    z_step = Func_080022ec(progress * (end_z - start_z), 0x64);
    *(s16 *)0x04000050 = 0;
    M2C_FIELD(start, s32, 0x34) = 0x20000;
    M2C_FIELD(start, s32, 0x30) = 0x80000;
    M2C_FIELD(start, s32, 0x28) = 0x40000;
    M2C_FIELD(start, s32, 0x48) = 0xAB85;
    M2C_FIELD(start, s32, 0x44) = 0;
    M2C_FIELD(start, s8, 0x5A) = 1;
    Func_08009150(start, x, 0, start_z + z_step);
    Func_08009080((s32) start, 2);
}
