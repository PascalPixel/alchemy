#include "types.h"
extern u8 *Func_0808a080(s32);
void Func_02000468(void)
{
    u8 *leader = Func_0808a080(0);
    if (*(s32 *)(leader + 12) >= 0x100000)
        Func_0808a080(8)[0x23] |= 2;
    else
        Func_0808a080(8)[0x23] = 1;
}
