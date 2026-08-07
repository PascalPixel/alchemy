/*
 * BYTE-EXACT and adopted 2026-08-07 with
 * -fthumb-call-literal-arg1-first-after-call: the last residual was a
 * two-literal argument sheet that opens right after a call, where the
 * reference writes r1 before r0 -- the same function writes later pairs in
 * register order, so the returning call is the discriminator.
 */

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
