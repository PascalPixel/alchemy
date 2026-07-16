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

void Func_080dfddc(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_ip;
    s32 var_r1;
    s32 var_r6;
    s32 var_r7;
    u8 *var_r2;
    u8 *var_r4;

    var_ip = 0;
    if (arg3 != 0) {
        var_r7 = 0;
        var_r6 = arg3;
        do {
            var_r1 = 0;
            if (arg2 != 0) {
                var_r2 = (var_r6 + arg1) - 1;
                var_r4 = var_r7 + arg0;
                do {
                    var_r1 += 1;
                    *var_r2 = *var_r4;
                    var_r4 += 1;
                    var_r2 += arg3;
                } while (var_r1 != arg2);
            }
            var_ip += 1;
            var_r7 += arg2;
            var_r6 -= 1;
        } while (var_ip != arg3);
    }
}
