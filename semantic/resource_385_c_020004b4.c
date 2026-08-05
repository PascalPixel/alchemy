#include "types.h"

/* Complete actor-mode scene wrapper through its return and alignment. */





extern void Func_020015a2(void);
extern void Func_020015fa(s32 actor, s32 mode);
extern void Func_02001642(s32 actor, s32 mode);
extern void Func_020015be(void);
void Func_020004b4(s32 actor)
{
    Func_020015a2();
    Func_020015fa(actor, 1);
    Func_02001642(actor, 0);
    Func_020015be();
}
