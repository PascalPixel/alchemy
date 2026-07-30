typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 *Func_02003f90(s32);
void Func_0200367e(s32, s32, s32, s32, s32);

void Func_02001b1c(void) {
    s32 *p = Func_02003f90(12);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200367e(12, 22, v - 1, 22, v);
    }
}
