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

s32 Func_080dbb98();

void Func_080d655c(s32 arg0) {
    s32 var_r6;

    var_r6 = 0;
    if (arg0 != 0) {
        do {
            var_r6 += 1;
            Func_080dbb98();
        } while (var_r6 != arg0);
    }
}
