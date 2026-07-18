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

s16 *Func_08077018(s32);

s32 Func_080b20e8(s32 arg0) {
    s32 result = *Func_08077018(arg0) / 4;
    if ((arg0 & 0x400) == 0) {
        result = 0;
    }
    return result;
}
