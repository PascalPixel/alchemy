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

u32 Func_0801f704(void) {
    u32 var_r0;
    void *var_r2;

    var_r2 = *(s32 *)0x03001F1C + 0x1040;
    var_r0 = 0;
loop_1:
    if (M2C_FIELD(var_r2, u8 *, 0x1C) != 0) {
        var_r0 += 1;
        var_r2 += 0x40;
        if (var_r0 > 2U) {
            var_r0 = 0x3E7;
        } else {
            goto loop_1;
        }
    }
    return var_r0;
}
