typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 Func_020031ae(s32, s32, s32);
void Func_020034da(void);

void Func_020015bc(void) {
    if (Func_020031ae(15, 40, 12) != 0) {
        Func_020034da();
    }
}
