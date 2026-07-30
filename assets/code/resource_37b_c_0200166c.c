typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 *Func_02003ade(s32);
s32 *Func_02003ae8(s32);
void Func_02003ac0(s32, s32, s32, s32);

void Func_0200166c(void) {
    s32 v = Func_02003ade(0)[2] >> 20;
    if ((Func_02003ae8(0)[4] >> 20) == 7 && (u32)(v - 13) <= 1) {
        Func_02003ac0(2, 0xd00000, 0x700000, 255);
    }
}
