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

s32 Func_080072f0(s32, s32, s32, s32);

void Func_08003d04(void) {
    *(s8 *)0x03001D00 = 0;
    Func_080072f0(0x03001400, 0x400, 0x03001D00, 0x03000164);
}
