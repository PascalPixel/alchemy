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

s32 Func_08009158();

void Func_08098184(void *arg0) {
    s32 temp_r3;
    s32 var_r2;

    if (arg0 != NULL) {
        var_r2 = M2C_FIELD(arg0, s32 *, 0x18);
        if (var_r2 <= 0xFFFF) {
            do {
                temp_r3 = var_r2 + 0x1000;
                var_r2 = temp_r3;
            } while (temp_r3 <= 0xFFFF);
            M2C_FIELD(arg0, s32 *, 0x18) = temp_r3;
            M2C_FIELD(arg0, s32 *, 0x1C) = temp_r3;
        }
        Func_08009158();
    }
}
