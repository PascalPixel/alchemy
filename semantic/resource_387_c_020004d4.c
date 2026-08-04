#include "types.h"

/* resource_387 three-call story-flag wrapper at 0x020004d4. */
extern void Func_080770c8(s32 flagId);
extern void Func_0808a018(void);
extern void Func_0808a020(void);

void Func_020004d4(void)
{
    Func_0808a018();
    Func_080770c8(0x866);
    Func_0808a020();
}
