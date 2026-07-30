typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 *Func_02003f20(s32);
void Func_0200360e(s32, s32, s32, s32, s32);

void Func_02001aac(void) {
    s32 *p = Func_02003f20(10);
    if (p != NULL) {
        s32 v = p[4] >> 20;
        Func_0200360e(10, 13, v - 1, 13, v);
    }
}
