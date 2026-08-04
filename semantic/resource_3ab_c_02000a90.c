#include "types.h"
extern s32 Func_080770c0(s32); extern void Func_0808a170(s32); extern void Func_0808a180(s32,s32);
void Func_02000a90(void)
{
    if (Func_080770c0(0x941) != 0)
        Func_0808a170(0x24e8);
    else
        Func_0808a170(0x1bc0);
    Func_0808a180(9, 0);
}
