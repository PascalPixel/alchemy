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

s32 Func_08077008();

s32 Func_080a3d6c(void) {
    s32 temp_r3;
    s32 var_r1;
    s32 var_r5;
    u16 *var_r0;

    var_r5 = 0;
    var_r0 = Func_08077008() + 0xD8;
    var_r1 = 0xE;
    do {
        temp_r3 = 0x1FF & *var_r0;
        var_r0 += 1;
        if (temp_r3 != 0) {
            var_r5 += 1;
        }
        var_r1 -= 1;
    } while (var_r1 >= 0);
    return var_r5;
}
