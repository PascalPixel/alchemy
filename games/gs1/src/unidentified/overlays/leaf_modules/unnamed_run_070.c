/* Contiguous unnamed leaf-owner run for resource_376. */

#include "types.h"
extern s32 Func_020012e6(s32 flagId);

/* Complete 80-byte three-flag table selector through its seven-word pool. */
void *Func_02000140(void)
{
    if (Func_020012ca(0x834) != 0)
        return (void *)0x02009ac8;
    if (Func_020012d8(0x87a) != 0)
        return (void *)0x02009ffc;
    if (Func_020012e6(0x815) != 0)
        return (void *)0x02009da4;
    return (void *)0x02009c00;
}

/*
 * BYTE-EXACT and adopted 2026-08-07 with
 * -fthumb-call-literal-arg1-first-after-call: the last residual was a
 * two-literal argument sheet that opens right after a call, where the
 * reference writes r1 before r0 -- the same function writes later pairs in
 * register order, so the returning call is the discriminator.
 */

#include "types.h"






/* Complete 44-byte actor-15 scene owner through its one-word pool. */
extern void Func_02001330(void);
extern void Func_020013de(s32 message);
extern void Func_020013d8(s32 actor, s32 target, s32 frames);
extern s32 Func_02001410(s32 actor, s32 mode);
extern void Func_02001354(void);
void Func_02000190(void)
{
    Func_02001330();
    Func_020013de(0xf6d);
    Func_020013d8(0, 15, 6);
    Func_02001410(15, 0);
    Func_02001354();
}

#include "types.h"






/* Complete 44-byte actor-19 scene owner through its one-word pool. */
extern void Func_0200135c(void);
extern void Func_0200140a(s32 message);
extern void Func_02001404(s32 actor, s32 target, s32 frames);
/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */
extern s32 Func_0200143c(s32 actor, s32 mode);
extern void Func_02001380(void);
void Func_020001bc(void)
{
    Func_0200135c();
    Func_0200140a(0xf73);
    Func_02001404(0, 19, 6);
    Func_0200143c(19, 0);
    Func_02001380();
}

