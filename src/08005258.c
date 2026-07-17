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

void Func_08005258(u32 arg0, u32 arg1, u32 arg2) {
    u32 *store = (u32 *)0x03001ce0;
    store[0] = arg0;
    store[1] = arg1;
    store[2] = arg2;
}
