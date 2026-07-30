typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 Func_02003196(s32, s32, s32);
void Func_020033c6(void);

void Func_020015a4(void) {
    if (Func_02003196(13, 31, 12) != 0) {
        Func_020033c6();
    }
}
