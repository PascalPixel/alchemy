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

s32 Func_08017394(void *arg0) {
    if (*(u16 *)((u8 *)arg0 + 0x16) == 0) {
        if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
            return 1;
        }
    }
    return 0;
}
