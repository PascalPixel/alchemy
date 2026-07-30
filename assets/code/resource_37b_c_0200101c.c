typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 Func_02003456(s32);
void Func_0200347e(void);
void Func_0200349c(s32);
void Func_020034ae(s32, s32, s32);
void Func_020034da(s32, s32, s32);
void Func_020034a6(void);

void Func_0200101c(void) {
    if (Func_02003456(0x80b) == 0) {
        Func_0200347e();
        Func_0200349c(9);
        Func_020034ae(9, 0x3333, 0x1999);
        Func_020034da(9, 504, 152);
        Func_020034a6();
    }
}
