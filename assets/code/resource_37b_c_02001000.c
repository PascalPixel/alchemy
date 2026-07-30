typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

u16 *Func_02003472(s32);
void Func_02003548(void);

void Func_02001000(void) {
    u16 *p = Func_02003472(0);
    if (p[3] == 0x4000) {
        Func_02003548();
    }
}
