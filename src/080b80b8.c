typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern s8 Data_03001cd4;

s32 Func_080022ec(s32, s32);
void Func_08009150(void *, s32, s32, s32);
void Func_08009080(s32, s32);

void Func_080b80b8(void **arg0, void **arg1, s32 arg2) {
    s32 temp_r0;
    s32 temp_r2;
    s32 temp_r8;
    s32 temp_r6_2;
    s32 temp_diff;
    void *temp_r5;
    void *temp_r6;

    temp_r5 = *arg0;
    temp_r6 = *arg1;
    temp_r2 = M2C_FIELD(temp_r5, s32, 8);
    temp_r8 = temp_r2 + Func_080022ec(arg2 * (M2C_FIELD(temp_r6, s32, 8) - temp_r2), 0x64);
    temp_diff = M2C_FIELD(temp_r6, s32, 0x10);
    temp_r6_2 = M2C_FIELD(temp_r5, s32, 0x10);
    temp_r0 = Func_080022ec(arg2 * (temp_diff - temp_r6_2), 0x64);
    *(s16 *)0x04000050 = 0;
    M2C_FIELD(temp_r5, s32, 0x34) = 0x20000;
    M2C_FIELD(temp_r5, s32, 0x30) = 0x80000;
    M2C_FIELD(temp_r5, s32, 0x28) = 0x40000;
    M2C_FIELD(temp_r5, s32, 0x48) = 0xAB85;
    M2C_FIELD(temp_r5, s32, 0x44) = 0;
    M2C_FIELD(temp_r5, s8, 0x5A) = 1;
    Func_08009150(temp_r5, temp_r8, 0, temp_r6_2 + temp_r0);
    Func_08009080((s32) temp_r5, 2);
}
