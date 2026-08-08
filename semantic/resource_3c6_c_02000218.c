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
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c6 0218` - 23 distinct
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
void Func_0200174c();
void Func_020017ea();
void Func_020017c0();
void Func_0200181c();
void Func_020017dc();
void Func_020017e4();
void Func_02001850();
void Func_0200185c();
void Func_02001782();
void Func_02001882();
void Func_020017f8();
void Func_0200179e();
void Func_02001872();
void Func_020017e8();
void Func_020018c4();
void Func_020017ca();
void Func_02001864();
void Func_0200186e();
void Func_02001878();
void Func_020017ee();
void Func_0200187e();
void Func_020017fc();
void Func_020018b4();
void Func_0200180a();
void Func_020018ce();
void Func_020018cc();
s32 Func_0200184c();
void Func_02001832();
void Func_020018c2();
void Func_02001840();
void Func_020018f8();
void Func_0200184e();
void Func_020018e8();
void Func_02001866();
void Func_0200191e();
void Func_020018e6();
void Func_0200187c();
void Func_0200190c();
void Func_0200188a();
void Func_02001942();
void Func_02001898();
void Func_02001918();
void Func_020018a6();
void Func_0200195e();
void Func_02001944();
void Func_0200197a();
void Func_020018d0();
void Func_0200196a();
void Func_02001974();
void Func_0200195c();
void Func_020018f2();
void Func_020019c6();
void Func_020019b6();
void Func_0200198c();
void Func_0200191a();
void Func_02001920();
void Func_020019e4();
void Func_02001932();
void Func_020019c2();
void Func_02001940();
void Func_020019f8();
void Func_0200194e();
void Func_020019de();
void Func_02001a14();
void Func_02001a3e();
void Func_02001a10();
void Func_02001a38();
void Func_0200198e();
void Func_02001a1e();
void Func_0200199c();
void Func_02001a54();
void Func_020019aa();
void Func_02001a6e();
void Func_020019bc();
void Func_02001a74();
void Func_020019ca();
void Func_02001a4a();
void Func_020019d8();
void Func_02001ab2();
void Func_02001a24();
void Func_02001a08();
void Func_02001aca();
void Func_02001a18();
void Func_02001adc();
void Func_02001a2a();
void Func_02001aee();
void Func_02001a3c();
void Func_02001af4();
void Func_02001ada();
void Func_02001a58();
void Func_02001b10();
void Func_02001a66();
void Func_02001b3a();
void Func_02001b36();
void Func_02001a84();
void Func_02001b3c();
void Func_02001a92();
void Func_02001b12();
void Func_02001aa0();
void Func_02001b58();
void Func_02001aae();
void Func_02001b3e();
void Func_02001abc();
void Func_02001af6();
void Func_02001b1c();
void Func_02001b96();
void Func_02001ae4();
void Func_02001b9c();
void Func_02001af2();
void Func_02001b82();
void Func_02001b00();
void Func_02001bb8();
void Func_02001b0e();
void Func_02001bd2();
void Func_02001b20();
void Func_02001ba0();
void Func_02001b2e();
void Func_02001be6();
void Func_02001b8c();
void Func_02001c66();
void Func_02001b6c();
void Func_02001c24();
void Func_02001b7a();
void Func_02001c4e();
void Func_02001c58();
void Func_02001c2a();
void Func_02001c34();
void Func_02001c68();
void Func_02001bb6();
void Func_02001c46();
void Func_02001bc4();
void Func_02001c7c();
void Func_02001c62();
void Func_02001be0();
void Func_02001c98();
void Func_02001bee();
void Func_02001cb0();
void Func_02001c92();
void Func_02001c08();
void Func_02001c16();
void Func_02001cce();
void Func_02001ce8();
void Func_02001cca();
void Func_02001cfe();
void Func_02001c4c();
void Func_02001d10();
void Func_02001c5e();
void Func_02001cee();
void Func_02001c6c();
void Func_02001d24();
void Func_02001c7a();
void Func_02001cfa();
void Func_02001c88();
void Func_02001d08();
void Func_02001c96();
void Func_02001cd0();
void Func_02001d18();
u8 *Func_02001cd6();
void Func_02001d00();
void Func_02001d26();
void Func_02001d38();
void Func_02001cd6_b();
void Func_02001cea();
                                /* wait n frames */
                                /* begin the scripted sequence */
                                /* end the scripted sequence */
                                /* scene-state query */
                                /* scene record by slot selector, or 0 */
                                /* per-slot reorientation request */
                                /* move a slot to (x, z) */
                                /* timed per-slot adjustment */
                                /* per-slot release */
                                /* place an entity at (x, z) */
                                /* per-record scene request */
                                /* scene-presentation request */
                                /* select an entity presentation */
                                /* per-slot pairing request */
                                /* show a dialogue line by id */
                                /* dialogue-line variant with a mode word */
                                /* wait for the slot's action to finish */
                                /* face the slot along a heading */
                                /* play animation id for n frames */
                                /* scene bookkeeping, no arguments */
                                /* scene bookkeeping, no arguments */
                                /* signed per-slot adjustment */
                                /* signed per-slot adjustment */

void Func_02000218(s32 mode)
{
    s16 *record;

    Func_0200174c(mode);   /* 021a */
    Func_020017ea(0x2694);   /* 0220 */
    Func_020017c0(0, 0xf80000, 0x1a80000);   /* 022e */
    Func_0200181c(0, 0xc000, 0);   /* 023a */
    Func_020017dc(8, 0);   /* 0242 */
    Func_020017e4(9, 0);   /* 024a */
    Func_02001850();   /* 024e */
    Func_0200185c();   /* 0252 */
    Func_02001782(20);   /* 0258 */
    Func_02001882(22, 8, -16, 0xc000);   /* 0268 */
    Func_020017f8(22);   /* 026e */
    Func_0200179e(20);   /* 0274 */
    Func_02001872(22, 258, 60);   /* 0280 */
    Func_020017e8(22, 0x10000, 0x8000);   /* 028e */
    Func_020018c4(22, 0, -16);   /* 029a */
    Func_020017ca(10);   /* 02a0 */
    Func_02001864(22, 8, 40);   /* 02aa */
    Func_0200186e(22, 9, 40);   /* 02b4 */
    Func_02001878(22, 8, 40);   /* 02be */
    Func_020017ee(10);   /* 02c4 */
    Func_0200187e(22, 2);   /* 02cc */
    Func_020017fc(20);   /* 02d2 */
    Func_020018b4(22, 0);   /* 02da */
    Func_0200180a(10);   /* 02e0 */
    Func_020018ce(22, 0x4000, 0);   /* 02ec */
    Func_0200181c(30);   /* 02f2 */
    Func_020018cc(22, 0);   /* 02fa */
    Func_0200184c(0, 0);   /* 0302 */
    Func_02001832(30);   /* 0308 */
    Func_020018c2(8, 2);   /* 0310 */
    Func_02001840(20);   /* 0316 */
    Func_020018f8(8, 0);   /* 031e */
    Func_0200184e(10);   /* 0324 */
    Func_020018e8(22, 8, 30);   /* 032e */
    Func_020018e8(22, 2);   /* 0336 */
    Func_02001866(20);   /* 033c */
    Func_0200191e(22, 0);   /* 0344 */
    Func_020018e6(8, 1);   /* 034c */
    Func_0200187c(20);   /* 0352 */
    Func_0200190c(8, 2);   /* 035a */
    Func_0200188a(30);   /* 0360 */
    Func_02001942(8, 0);   /* 0368 */
    Func_02001898(10);   /* 036e */
    Func_02001918(22, 3);   /* 0376 */
    Func_020018a6(20);   /* 037c */
    Func_0200195e(22, 0);   /* 0384 */
    Func_020018b4(10);   /* 038a */
    Func_02001944(9, 2);   /* 0392 */
    Func_020018c2(20);   /* 0398 */
    Func_0200197a(9, 0);   /* 03a0 */
    Func_020018d0(10);   /* 03a6 */
    Func_0200196a(22, 9, 0);   /* 03b0 */
    Func_02001974(0, 9, 30);   /* 03ba */
    Func_0200195c(9, 1);   /* 03c2 */
    Func_020018f2(10);   /* 03c8 */
    Func_020019c6(22, 264, 40);   /* 03d4 */
    Func_020019b6(22, 0);   /* 03dc */
    Func_0200190c(10);   /* 03e2 */
    Func_0200198c(0, 3);   /* 03ea */
    Func_0200191a(20);   /* 03f0 */
    Func_02001920(10);   /* 03f6 */
    Func_020019e4(22, 0x4000, 0);   /* 0402 */
    Func_02001932(30);   /* 0408 */
    Func_020019c2(22, 2);   /* 0410 */
    Func_02001940(20);   /* 0416 */
    Func_020019f8(22, 0);   /* 041e */
    Func_0200194e(10);   /* 0424 */
    Func_020019de(9, 2);   /* 042c */
    Func_0200195c(30);   /* 0432 */
    Func_02001a14(9, 0);   /* 043a */
    Func_0200196a(10);   /* 0440 */
    Func_02001a3e(22, 258, 50);   /* 044c */
    Func_02001a10(22, 9, 20);   /* 0456 */
    Func_02001a38(22, 0);   /* 045e */
    Func_0200198e(10);   /* 0464 */
    Func_02001a1e(8, 2);   /* 046c */
    Func_0200199c(20);   /* 0472 */
    Func_02001a54(8, 0);   /* 047a */
    Func_020019aa(10);   /* 0480 */
    Func_02001a6e(22, 0x4000, 0);   /* 048c */
    Func_020019bc(30);   /* 0492 */
    Func_02001a74(22, 0);   /* 049a */
    Func_020019ca(10);   /* 04a0 */
    Func_02001a4a(0, 3);   /* 04a8 */
    Func_020019d8(20);   /* 04ae */
    Func_020019de(20);   /* 04b4 */
    Func_02001ab2(22, 258, 50);   /* 04c0 */
    Func_02001a24(22, 0x1cccc, 0xe666);   /* 04ca */
    Func_02001a4a(22, 256, 360);   /* 04d8 */
    Func_02001a08(10);   /* 04de */
    Func_02001aca(22, 0, 0);   /* 04e8 */
    Func_02001a18(30);   /* 04ee */
    Func_02001adc(22, 0x8000, 0);   /* 04fa */
    Func_02001a2a(30);   /* 0500 */
    Func_02001aee(22, 0xc000, 0);   /* 050c */
    Func_02001a3c(30);   /* 0512 */
    Func_02001af4(22, 0);   /* 051a */
    Func_02001a4a(10);   /* 0520 */
    Func_02001ada(9, 2);   /* 0528 */
    Func_02001a58(20);   /* 052e */
    Func_02001b10(9, 0);   /* 0536 */
    Func_02001a66(10);   /* 053c */
    Func_02001b3a(22, 256, 40);   /* 0548 */
    Func_02001b36(22, 0x2000, 0);   /* 0554 */
    Func_02001a84(20);   /* 055a */
    Func_02001b3c(22, 0);   /* 0562 */
    Func_02001a92(10);   /* 0568 */
    Func_02001b12(9, 3);   /* 0570 */
    Func_02001aa0(30);   /* 0576 */
    Func_02001b58(9, 0);   /* 057e */
    Func_02001aae(20);   /* 0584 */
    Func_02001b3e(22, 2);   /* 058c */
    Func_02001abc(20);   /* 0592 */
    Func_02001af6(22, 0x19999, 0xcccc);   /* 059c */
    Func_02001b1c(22, 256, 384);   /* 05aa */
    Func_02001b96(22, 0, 0);   /* 05b4 */
    Func_02001ae4(20);   /* 05ba */
    Func_02001b9c(22, 0);   /* 05c2 */
    Func_02001af2(10);   /* 05c8 */
    Func_02001b82(8, 2);   /* 05d0 */
    Func_02001b00(20);   /* 05d6 */
    Func_02001bb8(8, 0);   /* 05de */
    Func_02001b0e(10);   /* 05e4 */
    Func_02001bd2(22, 0x8000, 0);   /* 05f0 */
    Func_02001b20(40);   /* 05f6 */
    Func_02001ba0(22, 3);   /* 05fe */
    Func_02001b2e(20);   /* 0604 */
    Func_02001be6(22, 0);   /* 060c */
    Func_02001b3c(10);   /* 0612 */
    Func_02001b8c(22, 0x13333, 0x9999);   /* 0632 */
    Func_02001c66(22, 0, 16);   /* 063c */
    Func_02001b6c(10);   /* 0642 */
    Func_02001c24(22, 0);   /* 064a */
    Func_02001b7a(10);   /* 0650 */
    Func_02001c4e(0, 258, 80);   /* 065c */
    Func_02001c58(22, 257, 80);   /* 0666 */
    Func_02001c2a(22, 8, 40);   /* 0670 */
    Func_02001c34(22, 9, 40);   /* 067a */
    Func_02001c68(22, 0x4000, 0);   /* 0686 */
    Func_02001bb6(30);   /* 068c */
    Func_02001c46(22, 2);   /* 0694 */
    Func_02001bc4(20);   /* 069a */
    Func_02001c7c(22, 0);   /* 06a2 */
    Func_02001bd2(10);   /* 06a8 */
    Func_02001c62(9, 2);   /* 06b0 */
    Func_02001be0(20);   /* 06b6 */
    Func_02001c98(9, 0);   /* 06be */
    Func_02001bee(10);   /* 06c4 */
    Func_02001cb0(22, 0, 0);   /* 06ce */
    Func_02001c92(0, 9, 30);   /* 06d8 */
    Func_02001c08(10);   /* 06de */
    Func_02001c98(8, 2);   /* 06e6 */
    Func_02001c16(20);   /* 06ec */
    Func_02001cce(8, 0);   /* 06f4 */
    Func_02001c24(10);   /* 06fa */
    Func_02001ce8(22, 0x8000, 0);   /* 0706 */
    Func_02001cca(0, 8, 70);   /* 0710 */
    Func_02001cfe(22, 0x4000, 0);   /* 071c */
    Func_02001c4c(40);   /* 0722 */
    Func_02001d10(0, 0xc000, 0);   /* 072e */
    Func_02001c5e(40);   /* 0734 */
    Func_02001cee(22, 2);   /* 073c */
    Func_02001c6c(20);   /* 0742 */
    Func_02001d24(22, 0);   /* 074a */
    Func_02001c7a(10);   /* 0750 */
    Func_02001cfa(0, 3);   /* 0758 */
    Func_02001c88(20);   /* 075e */
    Func_02001d08(22, 3);   /* 0766 */
    Func_02001c96(30);   /* 076c */
    Func_02001cd0(22, 0x13333, 0x9999);   /* 0776 */
    Func_02001d18(22, 2);   /* 077e */

    record = (s16 *)Func_02001cd6(0);   /* 0784 */
    if (record != 0) {
        /* record[5] and record[9] are the +0x0a and +0x12 halfwords. */
        Func_02001d00(22, record[5], record[9]);   /* 0796 */
    }

    Func_02001d26(22);   /* 079c */
    Func_02001d38(22, 0, 0);   /* 07a6 */
    Func_02001cd6_b(10);   /* 07ac */
    Func_02001cea();   /* 07b0 */
}
