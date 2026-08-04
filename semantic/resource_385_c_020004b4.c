#include "types.h"

/* Complete actor-mode scene wrapper through its return and alignment. */
extern void Func_0808a018(void);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a020(void);

void Func_020004b4(s32 actor)
{
    Func_0808a018();
    Func_0808a100(actor, 1);
    Func_0808a180(actor, 0);
    Func_0808a020();
}
