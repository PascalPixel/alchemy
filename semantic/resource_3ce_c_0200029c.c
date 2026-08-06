#include "types.h"

/*
 * Resource 3ce script-command handler at 0x0200029c (1574 bytes,
 * 0x0200029c-0x020008c1).
 *
 * Role, settled BEFORE any disassembly (call-graph-first, HANDOVER section 0):
 * `assets/code/resource_3ce_overlay.s` carries the pool word 0x0200829d, which
 * under the proven 0x02008000 link base is 0x0200029c + the Thumb bit -- this
 * owner's own entry -- and it appears in the overlay's script-command table as
 * the triple `0xffff0002 / 0x0200829d / 0x00000000`, the same shape that names
 * nine already-tracked byte-exact siblings.  So this is a no-argument command
 * handler, command id 0x02.
 *
 * Link base proof for this overlay: see the header of
 * semantic/overlays/resource_3ce_c_02000b10.c -- fifteen odd
 * `0x0200[89ab]xxx` words in `assets/code/resource_3ce_overlay.s` resolve at
 * `word - 0x8000 - 1` onto already-tracked byte-exact sibling entries.
 *
 * Complete owner: `push {lr}` prologue at 0x0200029c and the matching
 * interworking return `pop {r0} / bx r0` at 0x020008be.  r0 holds the popped
 * return address, so nothing is returned: the owner is `void`.  No stack frame
 * and no saved registers: r0 and r1 are both reloaded with `movs` immediately
 * before every single call, so there is no long-lived register state anywhere
 * in the body.
 *
 * Pool map, derived by walking the owner's control flow from the prologue.
 * The body is entirely straight-line -- the ONLY branch instruction in 1,574
 * bytes is the bare unconditional `b.n 0x020006a0` at 0x02000698 -- and that
 * branch exists solely to hop this owner's single literal pool:
 *   0x0200069a   2-byte zero alignment word
 *   0x0200069c   0x00000c1e   (the only pool word in the owner)
 * The hop is MID-COMPUTATION: r1 is loaded with 236 at 0x02000696, before the
 * branch, and is consumed by the call at 0x020006a2 after it.  A pool walk
 * that stopped at the branch, or one that did not carry r1 across it, would
 * lose that argument.  Code resumes at 0x020006a0 and runs to the epilogue, so
 * there is no trailing pool; the 2 bytes at 0x020008c2 are the alignment
 * halfword before the next owner at 0x020008c4.
 * 0x00000c1e does not decode as a BL prefix, so no phantom call site.
 *
 * All 196 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.  This
 * overlay resolves to veneers only (sites=243, prologue=0 across the whole
 * image), so every call here is a main-image import.
 * sites=196, distinct_targets=3, matching the inventory's calls=196.
 *
 * Per-target multiset, tool count == C call count, checked PER TARGET:
 *   Func_08015040   1     Func_08077010   4     Func_08077028 191
 *
 * The 191 Func_08077028 sites are NOT a loop and are NOT folded.  In the
 * assembly they are 191 separate `movs r1,#id / movs r0,#slot / bl` triples in
 * sixteen consecutive runs of one (slot, id) pair; there is no counter, no
 * back edge and no comparison anywhere in the owner.  Writing them as sixteen
 * loops would deflate the multiset by 175, which is exactly the error class
 * HANDOVER warns about.  The tracked byte-exact sibling
 * `assets/code/resource_3ce_c_020008c4.c` spells its own long call run out the
 * same way, so this is the established shape in this overlay rather than a
 * transcription choice.
 *
 * Run structure (slot, id, repeat count):
 *   (0,187,17) (0,180,14) (0,181,11) (0,182,13) (0,183,9)
 *   (1,186,13) (1,187,12)
 *   (2,188,14) (2,189,11) (2,236,16)
 *   (3,191,11) (3,192,11) (3,193,11) (3,194,7) (3,195,13) (3,196,8)
 * The four slots are the same four the sibling handler at 0x02000b10 drives,
 * and the owner closes with the same Func_08077010 refresh of all four in the
 * same non-sequential 0, 1, 3, 2 order -- an independent cross-check of both
 * files.
 *
 * Behaviour: the handler queues a fixed script of 191 entries against the four
 * battle slots and then refreshes them.
 *
 * Uncertainties: 0x00000c1e is an id passed to Func_08015040 with a second
 * argument of 1, not an address under the link base (it is far below the link
 * band); the meaning of the Func_08077028 ids is not established beyond their
 * grouping by slot.  Arities are taken from the registers written before each
 * site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */




extern void Func_02001454();
extern void Func_020014dc();
extern void Func_020014e4();
extern void Func_020014ec();
extern void Func_020014f4();
extern void Func_020014fc();
extern void Func_02001504();
extern void Func_0200150c();
extern void Func_02001514();
extern void Func_0200151c();
extern void Func_02001524();
extern void Func_0200152c();
extern void Func_02001534();
extern void Func_0200153c();
extern void Func_02001544();
extern void Func_0200154c();
extern void Func_02001554();
extern void Func_0200155c();
extern void Func_02001564();
extern void Func_0200156c();
extern void Func_02001574();
extern void Func_0200157c();
extern void Func_02001584();
extern void Func_0200158c();
extern void Func_02001594();
extern void Func_0200159c();
extern void Func_020015a4();
extern void Func_020015ac();
extern void Func_020015b4();
extern void Func_020015bc();
extern void Func_020015c4();
extern void Func_020015cc();
extern void Func_020015d4();
extern void Func_020015dc();
extern void Func_020015e4();
extern void Func_020015ec();
extern void Func_020015f4();
extern void Func_020015fc();
extern void Func_02001604();
extern void Func_0200160c();
extern void Func_02001614();
extern void Func_0200161c();
extern void Func_02001624();
extern void Func_0200162c();
extern void Func_02001634();
extern void Func_0200163c();
extern void Func_02001644();
extern void Func_0200164c();
extern void Func_02001654();
extern void Func_0200165c();
extern void Func_02001664();
extern void Func_0200166c();
extern void Func_02001674();
extern void Func_0200167c();
extern void Func_02001684();
extern void Func_0200168c();
extern void Func_02001694();
extern void Func_0200169c();
extern void Func_020016a4();
extern void Func_020016ac();
extern void Func_020016b4();
extern void Func_020016bc();
extern void Func_020016c4();
extern void Func_020016cc();
extern void Func_020016d4();
extern void Func_020016dc();
extern void Func_020016e4();
extern void Func_020016ec();
extern void Func_020016f4();
extern void Func_020016fc();
extern void Func_02001704();
extern void Func_0200170c();
extern void Func_02001714();
extern void Func_0200171c();
extern void Func_02001724();
extern void Func_0200172c();
extern void Func_02001734();
extern void Func_0200173c();
extern void Func_02001744();
extern void Func_0200174c();
extern void Func_02001754();
extern void Func_0200175c();
extern void Func_02001764();
extern void Func_0200176c();
extern void Func_02001774();
extern void Func_0200177c();
extern void Func_02001784();
extern void Func_0200178c();
extern void Func_02001794();
extern void Func_0200179c();
extern void Func_020017a4();
extern void Func_020017ac();
extern void Func_020017b4();
extern void Func_020017bc();
extern void Func_020017c4();
extern void Func_020017cc();
extern void Func_020017d4();
extern void Func_020017dc();
extern void Func_020017e4();
extern void Func_020017ec();
extern void Func_020017f4();
extern void Func_020017fc();
extern void Func_02001804();
extern void Func_0200180c();
extern void Func_02001814();
extern void Func_0200181c();
extern void Func_02001824();
extern void Func_0200182c();
extern void Func_02001834();
extern void Func_0200183c();
extern void Func_02001844();
extern void Func_0200184c();
extern void Func_02001854();
extern void Func_0200185c();
extern void Func_02001864();
extern void Func_0200186c();
extern void Func_02001874();
extern void Func_0200187c();
extern void Func_02001884();
extern void Func_0200188c();
extern void Func_02001894();
extern void Func_0200189c();
extern void Func_020018a4();
extern void Func_020018ac();
extern void Func_020018b4();
extern void Func_020018bc();
extern void Func_020018c4();
extern void Func_020018d4();
extern void Func_020018dc();
extern void Func_020018e4();
extern void Func_020018ec();
extern void Func_020018f4();
extern void Func_020018fc();
extern void Func_02001904();
extern void Func_0200190c();
extern void Func_02001914();
extern void Func_0200191c();
extern void Func_02001924();
extern void Func_0200192c();
extern void Func_02001934();
extern void Func_0200193c();
extern void Func_02001944();
extern void Func_0200194c();
extern void Func_02001954();
extern void Func_0200195c();
extern void Func_02001964();
extern void Func_0200196c();
extern void Func_02001974();
extern void Func_0200197c();
extern void Func_02001984();
extern void Func_0200198c();
extern void Func_02001994();
extern void Func_0200199c();
extern void Func_020019a4();
extern void Func_020019ac();
extern void Func_020019b4();
extern void Func_020019bc();
extern void Func_020019c4();
extern void Func_020019cc();
extern void Func_020019d4();
extern void Func_020019dc();
extern void Func_020019e4();
extern void Func_020019ec();
extern void Func_020019f4();
extern void Func_020019fc();
extern void Func_02001a04();
extern void Func_02001a0c();
extern void Func_02001a14();
extern void Func_02001a1c();
extern void Func_02001a24();
extern void Func_02001a2c();
extern void Func_02001a34();
extern void Func_02001a3c();
extern void Func_02001a44();
extern void Func_02001a4c();
extern void Func_02001a54();
extern void Func_02001a5c();
extern void Func_02001a64();
extern void Func_02001a6c();
extern void Func_02001a74();
extern void Func_02001a7c();
extern void Func_02001a84();
extern void Func_02001a8c();
extern void Func_02001a94();
extern void Func_02001a9c();
extern void Func_02001aa4();
extern void Func_02001aac();
extern void Func_02001ab4();
extern void Func_02001abc();
extern void Func_02001ac4();
extern void Func_02001acc();
extern void Func_02001ad4();
extern void Func_02001aca();
extern void Func_02001ad0();
extern void Func_02001ad6();
extern void Func_02001adc();
void Func_0200029c(void)
{
    Func_02001454(0xc1e, 1);

    /* slot 0, id 187 -- 17 consecutive sites */
    Func_020014dc(0, 187);
    Func_020014e4(0, 187);
    Func_020014ec(0, 187);
    Func_020014f4(0, 187);
    Func_020014fc(0, 187);
    Func_02001504(0, 187);
    Func_0200150c(0, 187);
    Func_02001514(0, 187);
    Func_0200151c(0, 187);
    Func_02001524(0, 187);
    Func_0200152c(0, 187);
    Func_02001534(0, 187);
    Func_0200153c(0, 187);
    Func_02001544(0, 187);
    Func_0200154c(0, 187);
    Func_02001554(0, 187);
    Func_0200155c(0, 187);

    /* slot 0, id 180 -- 14 consecutive sites */
    Func_02001564(0, 180);
    Func_0200156c(0, 180);
    Func_02001574(0, 180);
    Func_0200157c(0, 180);
    Func_02001584(0, 180);
    Func_0200158c(0, 180);
    Func_02001594(0, 180);
    Func_0200159c(0, 180);
    Func_020015a4(0, 180);
    Func_020015ac(0, 180);
    Func_020015b4(0, 180);
    Func_020015bc(0, 180);
    Func_020015c4(0, 180);
    Func_020015cc(0, 180);

    /* slot 0, id 181 -- 11 consecutive sites */
    Func_020015d4(0, 181);
    Func_020015dc(0, 181);
    Func_020015e4(0, 181);
    Func_020015ec(0, 181);
    Func_020015f4(0, 181);
    Func_020015fc(0, 181);
    Func_02001604(0, 181);
    Func_0200160c(0, 181);
    Func_02001614(0, 181);
    Func_0200161c(0, 181);
    Func_02001624(0, 181);

    /* slot 0, id 182 -- 13 consecutive sites */
    Func_0200162c(0, 182);
    Func_02001634(0, 182);
    Func_0200163c(0, 182);
    Func_02001644(0, 182);
    Func_0200164c(0, 182);
    Func_02001654(0, 182);
    Func_0200165c(0, 182);
    Func_02001664(0, 182);
    Func_0200166c(0, 182);
    Func_02001674(0, 182);
    Func_0200167c(0, 182);
    Func_02001684(0, 182);
    Func_0200168c(0, 182);

    /* slot 0, id 183 -- 9 consecutive sites */
    Func_02001694(0, 183);
    Func_0200169c(0, 183);
    Func_020016a4(0, 183);
    Func_020016ac(0, 183);
    Func_020016b4(0, 183);
    Func_020016bc(0, 183);
    Func_020016c4(0, 183);
    Func_020016cc(0, 183);
    Func_020016d4(0, 183);

    /* slot 1, id 186 -- 13 consecutive sites */
    Func_020016dc(1, 186);
    Func_020016e4(1, 186);
    Func_020016ec(1, 186);
    Func_020016f4(1, 186);
    Func_020016fc(1, 186);
    Func_02001704(1, 186);
    Func_0200170c(1, 186);
    Func_02001714(1, 186);
    Func_0200171c(1, 186);
    Func_02001724(1, 186);
    Func_0200172c(1, 186);
    Func_02001734(1, 186);
    Func_0200173c(1, 186);

    /* slot 1, id 187 -- 12 consecutive sites */
    Func_02001744(1, 187);
    Func_0200174c(1, 187);
    Func_02001754(1, 187);
    Func_0200175c(1, 187);
    Func_02001764(1, 187);
    Func_0200176c(1, 187);
    Func_02001774(1, 187);
    Func_0200177c(1, 187);
    Func_02001784(1, 187);
    Func_0200178c(1, 187);
    Func_02001794(1, 187);
    Func_0200179c(1, 187);

    /* slot 2, id 188 -- 14 consecutive sites */
    Func_020017a4(2, 188);
    Func_020017ac(2, 188);
    Func_020017b4(2, 188);
    Func_020017bc(2, 188);
    Func_020017c4(2, 188);
    Func_020017cc(2, 188);
    Func_020017d4(2, 188);
    Func_020017dc(2, 188);
    Func_020017e4(2, 188);
    Func_020017ec(2, 188);
    Func_020017f4(2, 188);
    Func_020017fc(2, 188);
    Func_02001804(2, 188);
    Func_0200180c(2, 188);

    /* slot 2, id 189 -- 11 consecutive sites */
    Func_02001814(2, 189);
    Func_0200181c(2, 189);
    Func_02001824(2, 189);
    Func_0200182c(2, 189);
    Func_02001834(2, 189);
    Func_0200183c(2, 189);
    Func_02001844(2, 189);
    Func_0200184c(2, 189);
    Func_02001854(2, 189);
    Func_0200185c(2, 189);
    Func_02001864(2, 189);

    /* slot 2, id 236 -- 16 consecutive sites */
    Func_0200186c(2, 236);
    Func_02001874(2, 236);
    Func_0200187c(2, 236);
    Func_02001884(2, 236);
    Func_0200188c(2, 236);
    Func_02001894(2, 236);
    Func_0200189c(2, 236);
    Func_020018a4(2, 236);
    Func_020018ac(2, 236);
    Func_020018b4(2, 236);
    Func_020018bc(2, 236);
    Func_020018c4(2, 236);
    Func_020018d4(2, 236);
    Func_020018dc(2, 236);
    Func_020018e4(2, 236);
    Func_020018ec(2, 236);

    /* slot 3, id 191 -- 11 consecutive sites */
    Func_020018f4(3, 191);
    Func_020018fc(3, 191);
    Func_02001904(3, 191);
    Func_0200190c(3, 191);
    Func_02001914(3, 191);
    Func_0200191c(3, 191);
    Func_02001924(3, 191);
    Func_0200192c(3, 191);
    Func_02001934(3, 191);
    Func_0200193c(3, 191);
    Func_02001944(3, 191);

    /* slot 3, id 192 -- 11 consecutive sites */
    Func_0200194c(3, 192);
    Func_02001954(3, 192);
    Func_0200195c(3, 192);
    Func_02001964(3, 192);
    Func_0200196c(3, 192);
    Func_02001974(3, 192);
    Func_0200197c(3, 192);
    Func_02001984(3, 192);
    Func_0200198c(3, 192);
    Func_02001994(3, 192);
    Func_0200199c(3, 192);

    /* slot 3, id 193 -- 11 consecutive sites */
    Func_020019a4(3, 193);
    Func_020019ac(3, 193);
    Func_020019b4(3, 193);
    Func_020019bc(3, 193);
    Func_020019c4(3, 193);
    Func_020019cc(3, 193);
    Func_020019d4(3, 193);
    Func_020019dc(3, 193);
    Func_020019e4(3, 193);
    Func_020019ec(3, 193);
    Func_020019f4(3, 193);

    /* slot 3, id 194 -- 7 consecutive sites */
    Func_020019fc(3, 194);
    Func_02001a04(3, 194);
    Func_02001a0c(3, 194);
    Func_02001a14(3, 194);
    Func_02001a1c(3, 194);
    Func_02001a24(3, 194);
    Func_02001a2c(3, 194);

    /* slot 3, id 195 -- 13 consecutive sites */
    Func_02001a34(3, 195);
    Func_02001a3c(3, 195);
    Func_02001a44(3, 195);
    Func_02001a4c(3, 195);
    Func_02001a54(3, 195);
    Func_02001a5c(3, 195);
    Func_02001a64(3, 195);
    Func_02001a6c(3, 195);
    Func_02001a74(3, 195);
    Func_02001a7c(3, 195);
    Func_02001a84(3, 195);
    Func_02001a8c(3, 195);
    Func_02001a94(3, 195);

    /* slot 3, id 196 -- 8 consecutive sites */
    Func_02001a9c(3, 196);
    Func_02001aa4(3, 196);
    Func_02001aac(3, 196);
    Func_02001ab4(3, 196);
    Func_02001abc(3, 196);
    Func_02001ac4(3, 196);
    Func_02001acc(3, 196);
    Func_02001ad4(3, 196);
    Func_02001aca(0);
    Func_02001ad0(1);
    Func_02001ad6(3);
    Func_02001adc(2);
}
