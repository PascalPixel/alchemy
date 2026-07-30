typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

u16 *Func_02003456(s32);
void Func_0200352c(void);

void Func_02000fe4(void) {
    u16 *p = Func_02003456(0);
    if (p[3] == 0xc000) {
        Func_0200352c();
    }
}
