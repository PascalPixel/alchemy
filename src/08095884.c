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

s32 Func_0809b804(s32);

void Func_08095884(void) {
    s32 var_r5;
    s32 var_r6;

    var_r5 = *(s32 *)0x03001F30 + 0x58;
    var_r6 = 0x17;
    do {
        var_r6 -= 1;
        Func_0809b804(var_r5);
        var_r5 += 0x48;
    } while (var_r6 >= 0);
}
