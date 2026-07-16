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

s32 Func_080030f8(s32);

void Func_08003ce0(void) {
    if (*(u8 *)0x03001C98 != 0) {
        do {
            Func_080030f8(1);
        } while (*(u8 *)0x03001C98 != 0);
    }
}
