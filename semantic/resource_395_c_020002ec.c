#include "types.h"

/*
 * Resource 395 cutscene script at 0x020002ec (412 bytes,
 * 0x020002ec-0x02000487).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x020002ec and the single
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000468.  The
 * popped return address lands in r0, so nothing is returned and the owner is
 * `void`.  The body is straight line apart from the two spin-waits below;
 * there is no early exit.
 *
 * POOL MAP, from a control-flow walk (nothing branches over anything here, so
 * the walk simply runs off the end): code is 0x020002ec-0x0200046d, then a
 * two-byte alignment `nop` at 0x0200046e, then the owner's six-word literal
 * pool at 0x02000470-0x02000487.  The next owner's `push {r5, r6, lr}` is at
 * 0x02000488, which confirms the boundary.
 *
 * Pool words, under the 0x02008000 link base (proven for this overlay by
 * eleven prologue+Thumb-bit words across its converted siblings):
 *   0x02009219 = Func_02001218 + 1 — this overlay's own effect spawner,
 *                already converted as
 *                semantic/overlays/resource_395_c_02001218.c.  It is
 *                installed as a per-frame task here and removed again 0x1c8
 *                instructions later, which is a two-way witness: it proves the
 *                base and it names the task.
 *   0x02009dd0   the working-RAM flag word behind the loaded image (the image
 *                ends at 0x1dbc, so this is loader scratch, not image data).
 *                Func_02001218 reads exactly this word to decide whether to
 *                play cue 200; this owner is its writer — set to 1 before the
 *                task is installed and cleared again a few frames later, so
 *                the cue fires only on the spawner's first ticks.  See the
 *                HANDOVER note that a gate flag's setter is usually in a
 *                different owner.
 *   0x00406218, 0x0000014f, 0x000014cc, 0x0000200e — plain constants (a 16.16
 *                amount and three ids).  Small pooled constants are spelled
 *                inline below rather than as `&Value_...` symbols.
 *
 * All 38 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (--annotate for the listing, site->target from --json, never from the
 * summary histogram); overlay_show.ts's own `bl` annotations are wrong on
 * every overlay because an overlay `bl` stores target_offset - 2.  The tool
 * reports sites=38, distinct_targets=20, veneer=38, unknown=0.
 *
 * PLACED MULTISET, checked PER TARGET against that report rather than by
 * total:
 *   Func_080000c0 6   Func_080000d0 1   Func_080000d8 1   Func_0808a010 5
 *   Func_0808a080 3   Func_0808a090 1   Func_0808a0c0 1   Func_0808a100 3
 *   Func_0808a128 1   Func_0808a138 2   Func_0808a170 1   Func_0808a180 1
 *   Func_0808a188 1   Func_0808a1b8 1   Func_0808a1e0 1   Func_0808a210 1
 *   Func_0808a330 2   Func_0808a348 2   Func_0808a4f0 1   Func_080f9010 3
 * = 38.  The two Func_080000c0 sites at 0x020003a2 and 0x020003ae are the two
 * spin-waits and are DISTINCT sites even though their bodies are identical;
 * they are not merged (merging behaviourally identical arms deflates the
 * multiset).  The inventory's calls=36 is the documented undercount.
 *
 * Behaviour: fade the scene in, wait, cue sound 17, raise the spawner flag and
 * install the spawner task, then aim the camera and drive the subject record
 * (the r6 handle from Func_0808a080(0)) through its beam sequence — set its
 * +0x28 amount and its +0x44/+0x48 pair, start script 0x14f, spin until the
 * +0x28 amount goes negative and then until it goes positive again, cue the
 * impact, drop the spawner task, clear the record's bit 0 at +0x5a, show
 * dialogue 0x14cc / 0x200e, and fade back out.
 *
 * Uncertainties, recorded rather than smoothed over:
 *  - r5 is a long-lived register REASSIGNED mid-owner: it holds the pointer to
 *    the 0x02009dd0 flag word for the first third, and from 0x020003cc it
 *    holds the constant 0x4000 (`movs r5,#128` before the call at 0x020003d0,
 *    `lsls r5,r5,#7` after it).  Tracked per use below, not as one variable.
 *  - the two `Func_0808a080(0)` results at 0x02000352 and 0x020003e0 are used
 *    only as `record + 0x5a`; whether they are the same record as r6 is not
 *    asserted, so the call is repeated exactly as the assembly does.
 *  - the +0x28 word the spin-waits poll is written by the script started at
 *    Func_0808a0c0, not by this owner after 0x02000380; it is therefore read
 *    through a volatile view.
 *  - the argument roles of the 0x0808axxx imports are as far as this overlay's
 *    converted siblings establish them; none is asserted beyond that.  Old
 *    style declarations are used because one import can take different
 *    argument counts at different sites.
 */

u8 *Func_02001ca2();
void Func_02001d40();
void Func_02001d80();
void Func_02001d96();
void Func_02001c1c();
void Func_02001dca();
void Func_02001c3a();
void Func_02001c38();
void Func_02001da6();
u8 *Func_02001d04();
void Func_02001d56();
void Func_02001d2c();
void Func_02001e2a();
void Func_02001d70();
void Func_02001cac();
void Func_02001cb8();
void Func_02001e6c();
void Func_02001db4();
void Func_02001d5a();
void Func_02001cea();
void Func_02001ce2();
u8 *Func_02001d92();
void Func_02001d82();
void Func_02001e18();
void Func_02001e3a();
void Func_02001e1a();
void Func_02001da0();
void Func_02001e48();
void Func_02001ebc();
void Func_02001e9e();
void Func_02001eb4();
void Func_02001d3a();
void Func_02001e58();
void Func_02001dde();
void Func_02001e50();
void Func_02001df6();
                                /* frame wait */
                                /* install a per-frame task (callback, rank) */
                                /* remove a per-frame task (callback) */
                                /* scene beat / wait */
                                /* scene-record accessor */
                                /* 16.16 pair setter */
                                /* start a scripted action (slot, script, arg) */
                                /* per-slot scene request */
                                /* per-slot pairing request with a duration */
                                /* select an entity presentation */
                                /* show a dialogue line by id */
                                /* wait for the slot's action to finish */
                                /* as Func_0808a180, with a duration */
                                /* face the slot along a heading */
                                /* per-slot presentation select */
                                /* camera target */
                                /* scene amount (16.16) with a mode */
                                /* scene-effect request */
                                /* no-argument scene commit */
                                /* play a sound cue */

void Func_020002ec(void)
{
    s32 *spawnerFlag = (s32 *)0x02009dd0;
    u8 *subject;
    u8 *record;

    subject = Func_02001ca2(0);

    Func_02001d40(0, 0xc000, 0);            /* 192 << 8 */
    Func_02001d80(0x00406218, 1);
    Func_02001d96(20);
    Func_02001c1c(40);
    Func_02001dca(17);

    *spawnerFlag = 1;
    /* Func_02001218 + 1 under the proven 0x02008000 base. */
    Func_02001c3a((void *)0x02009219, 3200);        /* 200 << 4 */
    Func_02001c38(30);
    *spawnerFlag = 0;

    /* 164 << 17 = 0x01480000, 235 << 16 = 0x00eb0000. */
    Func_02001da6(0x01480000, -1, 0x00eb0000, 1);
    Func_02001d96(0, 1);

    record = Func_02001d04(0);
    record[0x5a] &= 0xfe;

    Func_02001d56(0, 16);
    Func_02001d2c(0, 0x00020000, 0x00020000);       /* 128 << 10, twice */
    Func_02001e2a(133);

    *(s32 *)(subject + 0x28) = 0x00050000;          /* 160 << 11 */
    *(s32 *)(subject + 0x48) = 0x00004000;          /* 128 << 7  */
    *(s32 *)(subject + 0x44) = 0x0000a000;          /* 160 << 8  */

    Func_02001d70(0, 0x14f, 258);                   /* 129 << 1 */

    /* The script drives +0x28; poll it down through zero, then back up. */
    while (*(volatile s32 *)(subject + 0x28) >= 0) {
        Func_02001cac(1);
    }
    do {
        Func_02001cb8(1);
    } while (*(volatile s32 *)(subject + 0x28) <= 0);

    Func_02001e6c(161);
    Func_02001db4(0, 19);
    Func_02001d5a(120);

    Func_02001cea((void *)0x02009219);
    Func_02001ce2(40);

    *(s32 *)(subject + 0x44) = 0x00004000;          /* r5, reassigned above */

    record = Func_02001d92(0);
    record[0x5a] |= 1;

    Func_02001d82(80);
    Func_02001e18(0x14cc);
    Func_02001e3a(0x200e, 0, 20);
    Func_02001e1a(0, 2);
    Func_02001da0(20);
    Func_02001e48(0x200e, 0);
    Func_02001ebc();

    Func_02001e9e(0x00010000, 1);                   /* 128 << 9 */
    Func_02001eb4(20);
    Func_02001d3a(40);

    *(u16 *)(subject + 6) = 0xc000;                 /* 192 << 8 */
    *(s32 *)(subject + 0x48) = 0x00010000;          /* 128 << 9 */
    *(s32 *)(subject + 0x44) = 0x00004000;

    Func_02001e58(0, 2);
    Func_02001dde(40);
    Func_02001e58(0, 4, 0);
    Func_02001e50(0, 1);
    Func_02001df6(20);
}
