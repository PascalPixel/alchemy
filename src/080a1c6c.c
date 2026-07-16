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
s32 Func_080022fc(s32, s32);
s32 Func_080a17c4(void *);

void Func_080a1c6c(void **arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    register s32 first_arg4 asm("r1");
    s32 var_r6;
    void *temp_r5;

    var_r6 = arg1;
    if (var_r6 > 0xF) {
        var_r6 = 0;
    }
    first_arg4 = arg4;
    asm volatile("" : "+r" (first_arg4));
    temp_r5 = *arg0;
    M2C_FIELD(temp_r5, s16 *, 8) = (s16) ((Func_080022ec(var_r6, first_arg4) * 0x10) + arg3);
    M2C_FIELD(temp_r5, s16 *, 6) = (s16) ((Func_080022fc(var_r6, arg4) * 0x18) + arg2);
    Func_080a17c4(temp_r5);
}
