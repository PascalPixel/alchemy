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

s32 Func_0808ec14(s32 arg0) {
    s32 var_r1;
    s32 var_r4;
    void *var_r2;

    var_r2 = *(s32 *)0x03001EBC + 0x11C;
    var_r4 = -1;
    var_r1 = 0;
    if (M2C_FIELD(var_r2, u8 *, 4) == arg0) {
        var_r4 = 0;
    } else {
loop_2:
        var_r1 += 1;
        var_r2 += 8;
        if (var_r1 <= 9) {
            if (M2C_FIELD(var_r2, u8 *, 4) == arg0) {
                var_r4 = var_r1;
            } else {
                goto loop_2;
            }
        }
    }
    return var_r4;
}
