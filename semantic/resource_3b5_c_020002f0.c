#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x020002f0 (224 bytes; 196 bytes of
 * code plus two interior literal pools at 0x02000336-0x02000343 and
 * 0x020003b6-0x020003cf).  The pool map comes from a control-flow walk: the
 * first pool is reached only by falling out of the `b.n 0x2000376` at
 * 0x02000334, and execution resumes at 0x02000344 via the `beq.n` above it.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8 save at
 * 0x020002f0-0x020002f4, and the matching
 * `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020003ac-0x020003b4.  r0 holds the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds 0x020082f1
 * in a record's handler word — Func_020002f0 + the Thumb bit under this
 * overlay's proven 0x02008000 link base.  A no-argument script step; the
 * table pairs it with 0x020083d1 (Func_020003d0) at six separate places, and
 * the two rows are twins (see that file).
 *
 * Behaviour: speak one of six lines for actor 16, chosen by the scene counter
 * at workspace + 382 and story flags 0x950 / 0x962, with the actor's heading
 * saved across the exchange and restored afterwards.
 *
 * IMPORTANT: the six selection arms all fall into ONE `bl Func_0808a170` at
 * 0x02000376.  Writing a call per arm would inject five phantom calls into the
 * per-target multiset, so the line id is hoisted into a local and the import
 * is called exactly once.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; twelve sites,
 * matching the inventory's `calls=12`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
u8 *Func_0808a080();
void Func_0808a018();
s32 Func_080770c0();
void Func_0808a170();
void Func_0808a100();
void Func_0808a150();
void Func_0808a188();
void Func_080000c0();
void Func_0808a020();

void Func_020002f0(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u8 *actor;
    u16 *flags;
    s16 heading;
    s32 line;

    actor = Func_0808a080(16);
    heading = *(s16 *)(actor + 6);
    flags = (u16 *)(actor + 100);

    Func_0808a018();

    *flags |= 2;

    if (*(s16 *)(workspace + 382) == 0) {
        if (Func_080770c0(0x950) != 0) {
            line = 0x2365;
        } else if (Func_080770c0(0x962) != 0) {
            line = 0x21e2;
        } else {
            line = 0x1f95;
        }
    } else {
        if (Func_080770c0(0x950) != 0) {
            line = 0x2371;
        } else if (Func_080770c0(0x962) != 0) {
            line = 0x21f5;
        } else {
            line = 0x1faa;
        }
    }

    Func_0808a170(line);

    Func_0808a100(16, 0);
    Func_0808a150(16, 0, 2);
    Func_0808a188(16, 0, 10);

    *(u16 *)(actor + 6) = (u16)heading;

    Func_080000c0(1);

    *flags &= 1;

    Func_0808a020();
}
