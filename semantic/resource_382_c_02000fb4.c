#include "types.h"
extern u8 *Func_0808a080(int); extern void Func_080f9010(int); extern void Func_08009178(void *,int,int);
extern void Func_080091c0(int,int,int,int,int,int); extern void Func_02000ef4(int,int,int);
void Func_02000fb4(void)
{
    u8 *leader = Func_0808a080(0); u8 *sprite = *(u8 **)(leader + 80);
    Func_080f9010(158); Func_08009178((void *)0x0200a86a, 54, 13); Func_080091c0(33, 20, 1, 3, 23, 12);
    leader[35] &= (u8)~1; sprite[9] |= 12; Func_02000ef4(376, 224, 8);
}
