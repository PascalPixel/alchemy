#include "types.h"





/* Complete 32-byte actor-16 dialogue owner through its one-word pool. */
extern void Func_020013f8(void);
extern void Func_020014a6(s32 message);
extern void Func_020014ce(s32 actor, s32 mode);
extern void Func_02001412(void);
void Func_02000258(void)
{
    Func_020013f8();
    Func_020014a6(0x11be);
    Func_020014ce(16, 0);
    Func_02001412();
}
