#include "types.h"
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a1e0(s32 actorId, s32 mode);
void Func_0200094c(void)
{
    u8 *leader = Func_0808a080(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Func_0808a1e0(20, 1);
}
