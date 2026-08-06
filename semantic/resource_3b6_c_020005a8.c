#include "types.h"

/*
 * Resource 3b6 facing-dependent cutscene step at 0x020005a8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020005a8 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000654.  The
 * return address is popped into r0, so the owner is `void`.  r5 holds the
 * incoming argument throughout; r6 holds a message-id base.
 *
 * Two literal pools lie in the row.  The interior one is SIX bytes only —
 * 0x020005d2-0x020005d7, an alignment halfword plus the 0xffffc000 mask — and
 * it is easy to over-read: 0x020005d8 is code, and it is a live branch target
 * (`bne.n 0x020005d8` at 0x020005c6).  The trailing pool is
 * 0x0200065a-0x0200066b.
 *
 * CALL COUNT.  The row carries sixteen `bl` instructions (0x5ae, 0x5cc, 0x5dc,
 * 0x5e8, 0x5f0, 0x5f8, 0x602, 0x60c, 0x614, 0x61e, 0x626, 0x62e, 0x636, 0x642,
 * 0x648, 0x650), every one of them on a reachable path and every one of them
 * placed below.  The inventory advertises `calls=14`, so the manifest
 * undercounts this row by two; none of the sixteen falls inside either literal
 * pool.  Recorded as an uncertainty in the inventory, not in this
 * reconstruction — an undercount is the safe direction, since the shortfall
 * cannot hide an unplaced call.
 *
 * All sites resolve with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`) and are named for the main-image address in each veneer
 * entry's trailing word.
 *
 * CONTROL FLOW.  Two call sites are shared across arms and `goto` is used to
 * keep them single, as the assembly has them: the `Func_0808a170` at 0x0200061e
 * is reached both by falling out of the delay arm (with r6 + 1) and by the
 * 0x962 match (with 0x2219), and the `Func_0808a180` at 0x02000626 is reached
 * both from there and from the `r6 + 2` arm at 0x0200060c.  Restructuring them
 * into per-arm copies would inflate the per-target call-site count.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
  /* fetch a record; called with 0 for the subject's own */

   /* query a numeric id; nonzero when selected */
  /* present message by id */
  /* act on the subject, variant used before a branch */
   /* branch predicate; nonzero selects the second arm */
  /* wait / delay by count */
  /* act on the subject */


extern u16 * Func_02000fa8();
extern void Func_02001046();
extern s32 Func_02000fa6();
extern void Func_02001022();
extern void Func_02001032();
extern s32 Func_02000fea();
extern void Func_02000fdc();
extern s32 Func_02000fde();
extern void Func_02001058();
extern void Func_02001070();
extern void Func_02001068();
extern void Func_02001080();
extern void Func_0200109c();
extern void Func_0200109a();
void Func_020005a8(s32 subject)
{
    u16 *record;
    s32 facing;
    s32 base;
    s32 message;

    record = Func_02000fa8(0);

    /* Bits 14-15 of the 0x2000-biased halfword at record + 6; see 0x0200066c. */
    facing = ((s32)record[3] + 0x2000) & 0xc000;

    if (facing == 0xc000) {
        Func_02001046(26, subject);
        return;
    }

    if (Func_02000fa6(0x950) != 0) {
        base = 0x2389;
        Func_02001022(base);
        Func_02001032(subject, 0);
        if (Func_02000fea(0, 0) == 0) {
            Func_02000fdc(10);
            message = base + 1;
            goto present;
        }
        Func_02001046(base + 2);
        goto act;
    }

    if (Func_02000fde(0x962) != 0) {
        message = 0x2219;
present:
        Func_02001058(message);
act:
        Func_02001070(subject, 0);
        return;
    }

    Func_02001068(0x1fd2);
    Func_02001080(subject, 0);

    /* r1 = 131 << 1 = 0x106.  Argument registers are set r1, r0, r2. */
    Func_0200109c(subject, 0x106, 0);

    Func_02001022(40);
    Func_0200109a(subject, 0);
}
