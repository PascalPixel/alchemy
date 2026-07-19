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

void Func_080a1c2c(s32 *, s32, s32, s32, s32);

void Func_080a1bdc(s32 arg0, s32 arg1, s32 arg2) {
    s32 _v1d16;
    s32 _v1t36;
    s32 _v1d75;
    s32 *var_r7;
    s32 *var_r5;
    s32 temp_r3;
    s32 var_r6;

    var_r6 = 0;
    _v1t36 = (*(s32 *)0x03001F2C); var_r5 = _v1t36 + 0x48;
    var_r7 = var_r5;
    do {
        temp_r3 = *var_r7++;
        if (temp_r3 != 0) {
            Func_080a1c2c(var_r5, var_r6, arg0, arg1, arg2);
        }
        _v1d75 = 0; var_r6 += 1;
        _v1d16 = 0; (s32) var_r5 += 4;
    } while (var_r6 <= 0x1F);
}
