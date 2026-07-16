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

s32 Func_0807a0cc(u32 arg0, u32 arg1) {
    s32 var_r3;

    var_r3 = 0;
    if ((arg0 <= 3U) && (arg1 <= 0x13U)) {
        var_r3 = (arg0 * 0x14) + arg1;
    }
    return (var_r3 * 0xC) + 0x0808926C;
}
