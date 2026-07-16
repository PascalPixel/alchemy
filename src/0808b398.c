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

s32 Func_080770c0(s32);

s32 Func_0808b398(s32 arg0) {
    s32 var_r5;

    var_r5 = arg0;
    if (var_r5 <= 8) {
        if (Func_080770c0(0x20) != 0) {
            if (var_r5 == 0) {
                var_r5 = 0x12;
            }
            if (var_r5 == 1) {
                var_r5 = 0x13;
            }
        } else if ((Func_080770c0(0x21) != 0) && (var_r5 == 0)) {
            var_r5 = 0x11;
        }
    }
    return var_r5;
}
