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
void Func_08004278(void *);
void Func_08016418(s32, s32);
void Func_08003f3c(u16);
void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_0802851c(void) {
    s32 temp_r0; s32 var_r6; u16 *var_r5; void *temp_r5;
    temp_r5 = *(void **)0x03001F38;
    Func_08004278((void *)0x08028195);
    temp_r0 = M2C_FIELD(temp_r5, s32, 0x78);
    if (temp_r0 != 0) { Func_08016418(temp_r0, 2); }
    var_r6 = 0;
    while (var_r6 < (s32) M2C_FIELD(temp_r5, s16, 0x8E)) {
        var_r5 = (u16 *)((u8 *)temp_r5 + 0x12) + var_r6 * 10;
        Func_08003f3c(*var_r5);
        var_r6 += 1;
    }
    Func_08002dd8(0x3A);
    Func_080030f8(1U);
}
