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

s32 Func_08002df0(s16 *);
s16 *Func_08004970(s32);
s32 Func_08017aa4(s16 *, s32, s32, s32);

void Func_0801e858(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 *temp_r0;
    s16 *var_r2;
    u8 *var_r5;

    var_r5 = arg0;
    temp_r0 = Func_08004970(0x200);
    var_r2 = temp_r0;
    if (*var_r5 != 0) {
        do {
            *var_r2 = (s16) *var_r5;
            var_r5 += 1;
            var_r2 += 1;
        } while (*var_r5 != 0);
    }
    *var_r2 = 0;
    Func_08017aa4(temp_r0, arg1, arg2, arg3);
    Func_08002df0(temp_r0);
}
