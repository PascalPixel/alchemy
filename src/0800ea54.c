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

void Func_0800ea54(void *arg0, s32 arg1) {
    if (arg0 != NULL) {
        *(s32 *)((u8 *)arg0 + 0x6C) = arg1;
    }
}
