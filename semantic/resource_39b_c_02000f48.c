/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-call-arg0-between-pool-pair.
 * Two residuals closed: the horizontal stop is 0x80000 + (page << 19), not
 * 0x40000; and at 0x02000f56 the reference writes `movs r0, #0' between the r2
 * and r1 pool loads instead of after both.
 */

#include "types.h"











/* Slide actor 0 to a page-indexed horizontal stop. */
extern void Func_0200342a();
extern void Func_02003568();
extern void Func_02003452();
extern void Func_020034d2();
extern void Func_0200348e();
extern void * Func_02003464();
extern void Func_0200340a();
extern void Func_02003458();
extern void Func_020034c2();
extern void Func_02003470();
void Func_02000f48(s32 page)
{
    void *actor;

    Func_0200342a();
    Func_02003568(228);
    Func_02003452(0, 0x6666, 0x3333);
    Func_020034d2(0, 2);
    Func_0200348e(0, 0, -8);
    actor = Func_02003464(0);
    Func_0200340a(actor, 0);
    Func_02003458(8);
    Func_020034c2(0, 0x80000 + (page << 19), 0);
    Func_02003470(30);
}
