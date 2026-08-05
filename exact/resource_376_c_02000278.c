#include "types.h"





/* Complete 32-byte actor-10 dialogue owner through its one-word pool. */
extern void Func_02001418(void);
extern void Func_020014c6(s32 message);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_020014ee(s32 actor, s32 mode);
extern void Func_02001432(void);
void Func_02000278(void)
{
    Func_02001418();
    Func_020014c6(0x1c3d);
    Func_020014ee(10, 0);
    Func_02001432();
}
