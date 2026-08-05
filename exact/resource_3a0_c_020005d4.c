#include "types.h"




extern void Func_02001998(void);
extern void Func_02001a3e(s32 messageId);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_02001a5e(s32 actorId, s32 mode);
extern void Func_020019b2(void);
void Func_020005d4(void)
{
    Func_02001998();
    Func_02001a3e(0x17e8);
    Func_02001a5e(9, 0);
    Func_020019b2();
}
