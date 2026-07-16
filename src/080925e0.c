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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);

void Func_080925e0(void *arg0) {
    register s32 temp_r3 asm("r3");
    s32 temp_r3_2;
    s32 temp_r3_3;
    register s32 temp_r5 asm("r5");
    register s32 temp_r7 asm("r7");
    s32 var_r3;

    temp_r5 = M2C_FIELD(arg0, s32 *, 0x30);
    asm volatile("" : "+r" (temp_r5) : : "memory");
    temp_r3 = M2C_FIELD(arg0, s32 *, 8) + temp_r5;
    M2C_FIELD(arg0, s32 *, 8) = temp_r3;
    M2C_FIELD(arg0, s32 *, 0x38) = temp_r3;
    asm volatile("" : : : "memory");
    temp_r7 = M2C_FIELD(arg0, s32 *, 0x34);
    temp_r3_2 = M2C_FIELD(arg0, s32 *, 0x10) + temp_r7;
    M2C_FIELD(arg0, s32 *, 0x10) = temp_r3_2;
    M2C_FIELD(arg0, s32 *, 0x40) = temp_r3_2;
    temp_r3_3 = M2C_FIELD(arg0, s32 *, 0xC) + 0x400;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_r3_3;
    M2C_FIELD(arg0, s32 *, 0x3C) = temp_r3_3;
    M2C_FIELD(arg0, s32 *, 0x30) = (s32) (temp_r5 - Func_080022ec(temp_r5, 0x12));
    var_r3 = temp_r7;
    if (temp_r7 < 0) {
        var_r3 += 0xF;
    }
    M2C_FIELD(arg0, s32 *, 0x34) = (s32) (temp_r7 - (var_r3 >> 4));
}
