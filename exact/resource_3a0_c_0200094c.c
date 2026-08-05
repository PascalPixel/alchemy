#include "types.h"
extern u8 *Func_02001d22(s32 actorId);
extern void Func_02001dea(s32 actorId, s32 mode);
void Func_0200094c(void)
{
    u8 *leader = Func_02001d22(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Func_02001dea(20, 1);
}
