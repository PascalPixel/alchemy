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
 * All 196 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
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
void Func_02001454();
void Func_020014dc();
void Func_020014e4();
void Func_020014ec();
void Func_020014f4();
void Func_020014fc();
void Func_02001504();
void Func_0200150c();
void Func_02001514();
void Func_0200151c();
void Func_02001524();
void Func_0200152c();
void Func_02001534();
void Func_0200153c();
void Func_02001544();
void Func_0200154c();
void Func_02001554();
void Func_0200155c();
void Func_02001564();
void Func_0200156c();
void Func_02001574();
void Func_0200157c();
void Func_02001584();
void Func_0200158c();
void Func_02001594();
void Func_0200159c();
void Func_020015a4();
void Func_020015ac();
void Func_020015b4();
void Func_020015bc();
void Func_020015c4();
void Func_020015cc();
void Func_020015d4();
void Func_020015dc();
void Func_020015e4();
void Func_020015ec();
void Func_020015f4();
void Func_020015fc();
void Func_02001604();
void Func_0200160c();
void Func_02001614();
void Func_0200161c();
void Func_02001624();
void Func_0200162c();
void Func_02001634();
void Func_0200163c();
void Func_02001644();
void Func_0200164c();
void Func_02001654();
void Func_0200165c();
void Func_02001664();
void Func_0200166c();
void Func_02001674();
void Func_0200167c();
void Func_02001684();
void Func_0200168c();
void Func_02001694();
void Func_0200169c();
void Func_020016a4();
void Func_020016ac();
void Func_020016b4();
void Func_020016bc();
void Func_020016c4();
void Func_020016cc();
void Func_020016d4();
void Func_020016dc();
void Func_020016e4();
void Func_020016ec();
void Func_020016f4();
void Func_020016fc();
void Func_02001704();
void Func_0200170c();
void Func_02001714();
void Func_0200171c();
void Func_02001724();
void Func_0200172c();
void Func_02001734();
void Func_0200173c();
void Func_02001744();
void Func_0200174c();
void Func_02001754();
void Func_0200175c();
void Func_02001764();
void Func_0200176c();
void Func_02001774();
void Func_0200177c();
void Func_02001784();
void Func_0200178c();
void Func_02001794();
void Func_0200179c();
void Func_020017a4();
void Func_020017ac();
void Func_020017b4();
void Func_020017bc();
void Func_020017c4();
void Func_020017cc();
void Func_020017d4();
void Func_020017dc();
void Func_020017e4();
void Func_020017ec();
void Func_020017f4();
void Func_020017fc();
void Func_02001804();
void Func_0200180c();
void Func_02001814();
void Func_0200181c();
void Func_02001824();
void Func_0200182c();
void Func_02001834();
void Func_0200183c();
void Func_02001844();
void Func_0200184c();
void Func_02001854();
void Func_0200185c();
void Func_02001864();
void Func_0200186c();
void Func_02001874();
void Func_0200187c();
void Func_02001884();
void Func_0200188c();
void Func_02001894();
void Func_0200189c();
void Func_020018a4();
void Func_020018ac();
void Func_020018b4();
void Func_020018bc();
void Func_020018c4();
void Func_020018d4();
void Func_020018dc();
void Func_020018e4();
void Func_020018ec();
void Func_020018f4();
void Func_020018fc();
void Func_02001904();
void Func_0200190c();
void Func_02001914();
void Func_0200191c();
void Func_02001924();
void Func_0200192c();
void Func_02001934();
void Func_0200193c();
void Func_02001944();
void Func_0200194c();
void Func_02001954();
void Func_0200195c();
void Func_02001964();
void Func_0200196c();
void Func_02001974();
void Func_0200197c();
void Func_02001984();
void Func_0200198c();
void Func_02001994();
void Func_0200199c();
void Func_020019a4();
void Func_020019ac();
void Func_020019b4();
void Func_020019bc();
void Func_020019c4();
void Func_020019cc();
void Func_020019d4();
void Func_020019dc();
void Func_020019e4();
void Func_020019ec();
void Func_020019f4();
void Func_020019fc();
void Func_02001a04();
void Func_02001a0c();
void Func_02001a14();
void Func_02001a1c();
void Func_02001a24();
void Func_02001a2c();
void Func_02001a34();
void Func_02001a3c();
void Func_02001a44();
void Func_02001a4c();
void Func_02001a54();
void Func_02001a5c();
void Func_02001a64();
void Func_02001a6c();
void Func_02001a74();
void Func_02001a7c();
void Func_02001a84();
void Func_02001a8c();
void Func_02001a94();
void Func_02001a9c();
void Func_02001aa4();
void Func_02001aac();
void Func_02001ab4();
void Func_02001abc();
void Func_02001ac4();
void Func_02001acc();
void Func_02001ad4();
void Func_02001aca();
void Func_02001ad0();
void Func_02001ad6();
void Func_02001adc();

                     

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
