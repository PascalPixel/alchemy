#include "types.h"

/*
 * resource_3c6 cutscene script at 0x02000218, 1,452 bytes
 * (0x02000218-0x020007c3).
 *
 * Complete owner: `push {lr}` at 0x02000218 through the single interworking
 * epilogue `pop {r0} / bx r0` at 0x020007b4.  The epilogue pops the return
 * address into r0, so the owner is void.
 *
 * The owner takes one argument: r0 is never written before the very first
 * `bl` at 0x0200021a, so the incoming value is forwarded unchanged to
 * Func_0808a018.  Its sole caller in this overlay, Func_0200120c at
 * 0x020014a4, passes 1.
 *
 * Two literal pools sit inside the span; the pool map was derived by walking
 * the owner's control flow from the prologue, not from a referenced-words
 * listing:
 *   0x02000618-0x0200062b  (0x2694, 0x1cccc, 0xe666, 0x19999, 0xcccc) - hopped
 *                          by the lone unconditional `b.n` at 0x02000616;
 *   0x020007b8-0x020007c3  (0x13333, 0x9999, 0x101) - past the `bx`.
 * Register state is carried across the first pool: nothing is in flight there
 * (the `b.n` follows a completed call), but the walk is what proves it.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * None of the eight pool words falls in 0x0200_8xxx-0x0200_bxxx: they are all
 * plain constants - a dialogue id and seven 16.16 coordinate/heading values.
 *
 * Control flow: one conditional, at 0x02000788.  Everything else is a
 * straight-line script over slots 0, 8, 9 and 22.
 *
 * Call accounting: 177 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3c6 0218` - 23 distinct
 * targets, all import veneers, no prologue and no `call_via` site.  The
 * multiset of `Func_...(` calls below equals that multiset, site for site;
 * each site is written exactly once and no site is reached from two paths.
 * (The inventory's `calls=162` is the usual undercount and is not the check.)
 *
 * UNCERTAINTY 1: at 0x0200078c/0x02000790 the offset register r3 holds 18
 * for the second `ldrsh` and is still live at the `bl` two instructions
 * later.  That is the documented phantom-argument idiom, so Func_0808a0b8 is
 * written with three arguments, not four.  The two halfwords read at record
 * +0x0a and +0x12 are the integer parts of the 16.16 words at +0x08/+0x10.
 * UNCERTAINTY 2: Func_0808a070(0, 0) at 0x02000302 is called purely for its
 * side effect here; the same import's result is tested in
 * resource_3c6_c_020000c4.c.
 * UNCERTAINTY 3: Func_0808a1e8's second argument takes the neighbouring
 * values 256/257/258/264, all built by `movs`+`lsls #1` or a pool word.  They
 * read as adjacent animation ids, which is not proven from this owner.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay. */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* begin the scripted sequence */
void Func_0808a020();           /* end the scripted sequence */
s32 Func_0808a070();            /* scene-state query */
u8 *Func_0808a080();            /* scene record by slot selector, or 0 */
void Func_0808a090();           /* per-slot reorientation request */
void Func_0808a0b8();           /* move a slot to (x, z) */
void Func_0808a0d0();           /* timed per-slot adjustment */
void Func_0808a0e8();           /* per-slot release */
void Func_0808a0f0();           /* place an entity at (x, z) */
void Func_0808a100();           /* per-record scene request */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a148();           /* per-slot pairing request */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dialogue-line variant with a mode word */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face the slot along a heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a360();           /* scene bookkeeping, no arguments */
void Func_0808a370();           /* scene bookkeeping, no arguments */
void Func_0808a570();           /* signed per-slot adjustment */
void Func_0808a580();           /* signed per-slot adjustment */

void Func_02000218(s32 mode)
{
    s16 *record;

    Func_0808a018(mode);   /* 021a */
    Func_0808a170(0x2694);   /* 0220 */
    Func_0808a0f0(0, 0xf80000, 0x1a80000);   /* 022e */
    Func_0808a1b8(0, 0xc000, 0);   /* 023a */
    Func_0808a100(8, 0);   /* 0242 */
    Func_0808a100(9, 0);   /* 024a */
    Func_0808a360();   /* 024e */
    Func_0808a370();   /* 0252 */
    Func_0808a010(20);   /* 0258 */
    Func_0808a570(22, 8, -16, 0xc000);   /* 0268 */
    Func_0808a0e8(22);   /* 026e */
    Func_0808a010(20);   /* 0274 */
    Func_0808a1e8(22, 258, 60);   /* 0280 */
    Func_0808a090(22, 0x10000, 0x8000);   /* 028e */
    Func_0808a580(22, 0, -16);   /* 029a */
    Func_0808a010(10);   /* 02a0 */
    Func_0808a148(22, 8, 40);   /* 02aa */
    Func_0808a148(22, 9, 40);   /* 02b4 */
    Func_0808a148(22, 8, 40);   /* 02be */
    Func_0808a010(10);   /* 02c4 */
    Func_0808a138(22, 2);   /* 02cc */
    Func_0808a010(20);   /* 02d2 */
    Func_0808a180(22, 0);   /* 02da */
    Func_0808a010(10);   /* 02e0 */
    Func_0808a1b8(22, 0x4000, 0);   /* 02ec */
    Func_0808a010(30);   /* 02f2 */
    Func_0808a178(22, 0);   /* 02fa */
    Func_0808a070(0, 0);   /* 0302 */
    Func_0808a010(30);   /* 0308 */
    Func_0808a138(8, 2);   /* 0310 */
    Func_0808a010(20);   /* 0316 */
    Func_0808a180(8, 0);   /* 031e */
    Func_0808a010(10);   /* 0324 */
    Func_0808a148(22, 8, 30);   /* 032e */
    Func_0808a138(22, 2);   /* 0336 */
    Func_0808a010(20);   /* 033c */
    Func_0808a180(22, 0);   /* 0344 */
    Func_0808a100(8, 1);   /* 034c */
    Func_0808a010(20);   /* 0352 */
    Func_0808a138(8, 2);   /* 035a */
    Func_0808a010(30);   /* 0360 */
    Func_0808a180(8, 0);   /* 0368 */
    Func_0808a010(10);   /* 036e */
    Func_0808a110(22, 3);   /* 0376 */
    Func_0808a010(20);   /* 037c */
    Func_0808a180(22, 0);   /* 0384 */
    Func_0808a010(10);   /* 038a */
    Func_0808a138(9, 2);   /* 0392 */
    Func_0808a010(20);   /* 0398 */
    Func_0808a180(9, 0);   /* 03a0 */
    Func_0808a010(10);   /* 03a6 */
    Func_0808a148(22, 9, 0);   /* 03b0 */
    Func_0808a148(0, 9, 30);   /* 03ba */
    Func_0808a100(9, 1);   /* 03c2 */
    Func_0808a010(10);   /* 03c8 */
    Func_0808a1e8(22, 264, 40);   /* 03d4 */
    Func_0808a180(22, 0);   /* 03dc */
    Func_0808a010(10);   /* 03e2 */
    Func_0808a110(0, 3);   /* 03ea */
    Func_0808a010(20);   /* 03f0 */
    Func_0808a010(10);   /* 03f6 */
    Func_0808a1b8(22, 0x4000, 0);   /* 0402 */
    Func_0808a010(30);   /* 0408 */
    Func_0808a138(22, 2);   /* 0410 */
    Func_0808a010(20);   /* 0416 */
    Func_0808a180(22, 0);   /* 041e */
    Func_0808a010(10);   /* 0424 */
    Func_0808a138(9, 2);   /* 042c */
    Func_0808a010(30);   /* 0432 */
    Func_0808a180(9, 0);   /* 043a */
    Func_0808a010(10);   /* 0440 */
    Func_0808a1e8(22, 258, 50);   /* 044c */
    Func_0808a148(22, 9, 20);   /* 0456 */
    Func_0808a180(22, 0);   /* 045e */
    Func_0808a010(10);   /* 0464 */
    Func_0808a138(8, 2);   /* 046c */
    Func_0808a010(20);   /* 0472 */
    Func_0808a180(8, 0);   /* 047a */
    Func_0808a010(10);   /* 0480 */
    Func_0808a1b8(22, 0x4000, 0);   /* 048c */
    Func_0808a010(30);   /* 0492 */
    Func_0808a180(22, 0);   /* 049a */
    Func_0808a010(10);   /* 04a0 */
    Func_0808a110(0, 3);   /* 04a8 */
    Func_0808a010(20);   /* 04ae */
    Func_0808a010(20);   /* 04b4 */
    Func_0808a1e8(22, 258, 50);   /* 04c0 */
    Func_0808a090(22, 0x1cccc, 0xe666);   /* 04ca */
    Func_0808a0d0(22, 256, 360);   /* 04d8 */
    Func_0808a010(10);   /* 04de */
    Func_0808a1b8(22, 0, 0);   /* 04e8 */
    Func_0808a010(30);   /* 04ee */
    Func_0808a1b8(22, 0x8000, 0);   /* 04fa */
    Func_0808a010(30);   /* 0500 */
    Func_0808a1b8(22, 0xc000, 0);   /* 050c */
    Func_0808a010(30);   /* 0512 */
    Func_0808a180(22, 0);   /* 051a */
    Func_0808a010(10);   /* 0520 */
    Func_0808a138(9, 2);   /* 0528 */
    Func_0808a010(20);   /* 052e */
    Func_0808a180(9, 0);   /* 0536 */
    Func_0808a010(10);   /* 053c */
    Func_0808a1e8(22, 256, 40);   /* 0548 */
    Func_0808a1b8(22, 0x2000, 0);   /* 0554 */
    Func_0808a010(20);   /* 055a */
    Func_0808a180(22, 0);   /* 0562 */
    Func_0808a010(10);   /* 0568 */
    Func_0808a110(9, 3);   /* 0570 */
    Func_0808a010(30);   /* 0576 */
    Func_0808a180(9, 0);   /* 057e */
    Func_0808a010(20);   /* 0584 */
    Func_0808a138(22, 2);   /* 058c */
    Func_0808a010(20);   /* 0592 */
    Func_0808a090(22, 0x19999, 0xcccc);   /* 059c */
    Func_0808a0d0(22, 256, 384);   /* 05aa */
    Func_0808a1b8(22, 0, 0);   /* 05b4 */
    Func_0808a010(20);   /* 05ba */
    Func_0808a180(22, 0);   /* 05c2 */
    Func_0808a010(10);   /* 05c8 */
    Func_0808a138(8, 2);   /* 05d0 */
    Func_0808a010(20);   /* 05d6 */
    Func_0808a180(8, 0);   /* 05de */
    Func_0808a010(10);   /* 05e4 */
    Func_0808a1b8(22, 0x8000, 0);   /* 05f0 */
    Func_0808a010(40);   /* 05f6 */
    Func_0808a110(22, 3);   /* 05fe */
    Func_0808a010(20);   /* 0604 */
    Func_0808a180(22, 0);   /* 060c */
    Func_0808a010(10);   /* 0612 */
    Func_0808a090(22, 0x13333, 0x9999);   /* 0632 */
    Func_0808a580(22, 0, 16);   /* 063c */
    Func_0808a010(10);   /* 0642 */
    Func_0808a180(22, 0);   /* 064a */
    Func_0808a010(10);   /* 0650 */
    Func_0808a1e8(0, 258, 80);   /* 065c */
    Func_0808a1e8(22, 257, 80);   /* 0666 */
    Func_0808a148(22, 8, 40);   /* 0670 */
    Func_0808a148(22, 9, 40);   /* 067a */
    Func_0808a1b8(22, 0x4000, 0);   /* 0686 */
    Func_0808a010(30);   /* 068c */
    Func_0808a138(22, 2);   /* 0694 */
    Func_0808a010(20);   /* 069a */
    Func_0808a180(22, 0);   /* 06a2 */
    Func_0808a010(10);   /* 06a8 */
    Func_0808a138(9, 2);   /* 06b0 */
    Func_0808a010(20);   /* 06b6 */
    Func_0808a180(9, 0);   /* 06be */
    Func_0808a010(10);   /* 06c4 */
    Func_0808a1b8(22, 0, 0);   /* 06ce */
    Func_0808a148(0, 9, 30);   /* 06d8 */
    Func_0808a010(10);   /* 06de */
    Func_0808a138(8, 2);   /* 06e6 */
    Func_0808a010(20);   /* 06ec */
    Func_0808a180(8, 0);   /* 06f4 */
    Func_0808a010(10);   /* 06fa */
    Func_0808a1b8(22, 0x8000, 0);   /* 0706 */
    Func_0808a148(0, 8, 70);   /* 0710 */
    Func_0808a1b8(22, 0x4000, 0);   /* 071c */
    Func_0808a010(40);   /* 0722 */
    Func_0808a1b8(0, 0xc000, 0);   /* 072e */
    Func_0808a010(40);   /* 0734 */
    Func_0808a138(22, 2);   /* 073c */
    Func_0808a010(20);   /* 0742 */
    Func_0808a180(22, 0);   /* 074a */
    Func_0808a010(10);   /* 0750 */
    Func_0808a110(0, 3);   /* 0758 */
    Func_0808a010(20);   /* 075e */
    Func_0808a110(22, 3);   /* 0766 */
    Func_0808a010(30);   /* 076c */
    Func_0808a090(22, 0x13333, 0x9999);   /* 0776 */
    Func_0808a100(22, 2);   /* 077e */

    record = (s16 *)Func_0808a080(0);   /* 0784 */
    if (record != 0) {
        /* record[5] and record[9] are the +0x0a and +0x12 halfwords. */
        Func_0808a0b8(22, record[5], record[9]);   /* 0796 */
    }

    Func_0808a0e8(22);   /* 079c */
    Func_0808a0f0(22, 0, 0);   /* 07a6 */
    Func_0808a010(10);   /* 07ac */
    Func_0808a020();   /* 07b0 */
}
