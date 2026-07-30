typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 Func_02003166(s32, s32, s32);
void Func_0200319a(void);

void Func_02001574(void) {
    if (Func_02003166(9, 31, 9) != 0) {
        Func_0200319a();
    }
}
