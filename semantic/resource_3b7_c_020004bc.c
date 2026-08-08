/*
 * resource_3b7 owner at 0x020004bc, 962 bytes (0x020004bc-0x0200087d).
 *
 * A scripted musical performance: 29 timed note events through Func_08009180,
 * each followed by a sound trigger and a wait, with one mid-piece flourish that
 * targets the actor passed in.
 *
 * Code runs 0x020004bc-0x0200087d with NO literal pool at all - every operand
 * is an immediate or a shifted immediate, and the span ends on the `bx r0` of
 * the interworking return.  Nothing here depends on the overlay's link base.
 *
 * Call targets were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay
 * `bl` stores `target_offset - 2`).  Completeness, as a multiset:
 * Func_0808a010 x33, Func_080f9010 x30, Func_08009180 x29, and one each of
 * Func_0808a018, Func_0808a020, Func_0808a060, Func_0808a1b8, Func_0808a398 -
 * 97 sites over 8 distinct targets, exactly the tool's
 * `sites=97 distinct_targets=8`.  The inventory row says calls=95, the
 * documented lower bound.
 *
 * Every call's arguments were recovered by an argument-window walk over the
 * disassembly with the documented guard (an unmodelled instruction clears only
 * its destination register, never the window), which is safe here because
 * there is no pool to hop and no branch of any kind in the owner: it is one
 * straight-line block from prologue to epilogue.
 *
 * Func_08009180 takes six arguments - four in registers and two on the stack
 * at [sp, #0] and [sp, #4].  The prologue reserves those eight bytes with
 * `sub sp, #8`, keeps 3 in r6 and 8 in r8 for the whole owner, and rewrites
 * both slots before every call.  The four constant arguments (70, 0, 3, 8)
 * never change; only the first two vary, and they read as a pitch and a
 * duration: the piece runs 82, 85, 88, 91, 94, 97, 100 at duration 20, then
 * 79, 82, 85, 88, 91, 94, 97, 100 at duration 29, then descends 97, 94, 91,
 * 88, 85, 82 at 29 and 100, 97, 94, 91, 88, 85, 82, 79 at 20.  Each step is
 * three, which is the shape of a scale walk, but the units are not
 * established.
 *
 * The epilogue is `add sp, #8 / pop {r3} / mov r8, r3 / pop {r5, r6} /
 * pop {r0} / bx r0`, so r0 holds the popped return address and the owner
 * returns nothing: void.  The single argument arrives in r0, is saved to r5 in
 * the prologue, and is used only by the Func_0808a398 / Func_0808a060 pair in
 * the middle.
 *
 * This owner is called from Func_02000ac8, which passes it one of the fifteen
 * in-image draw ids at file offset 0x1fd0.
 *
 * Uncertainties:
 *  - Func_0808a018 at the top has no argument register set, and Func_0808a020
 *    at the bottom likewise; both are spelled with no arguments.  They are the
 *    established cutscene bracket.
 *  - The first note is followed by a 3-frame wait and no Func_080f9010(154);
 *    every later note gets the trigger and an 8-frame wait.  That asymmetry is
 *    in the bytes, not a transcription slip - Func_080f9010(148) at the top and
 *    Func_080f9010(126) mid-piece are the other two distinct trigger ids.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

void Func_02001de4();
void Func_02001de2();
void Func_02001e98();
void Func_02001dee();
void Func_02001e62();
void Func_02001e00();
void Func_02001d9e();
void Func_02001e1c();
void Func_02001db6();
void Func_02001ee4();
void Func_02001e3a();
void Func_02001dd4();
void Func_02001f02();
void Func_02001e58();
void Func_02001df2();
void Func_02001f20();
void Func_02001e76();
void Func_02001e10();
void Func_02001f3e();
void Func_02001e94();
void Func_02001e2e();
void Func_02001f5c();
void Func_02001eb2();
void Func_02001e4c();
void Func_02001f7a();
void Func_02001ed0();
void Func_02001e6a();
void Func_02001f98();
void Func_02001eee();
void Func_02001e88();
void Func_02001fb6();
void Func_02001f0c();
void Func_02001ea6();
void Func_02001fd4();
void Func_02001f2a();
void Func_02001ec4();
void Func_02001ff2();
void Func_02001f48();
void Func_02001ee2();
void Func_02002010();
void Func_02001f66();
void Func_02001f00();
void Func_0200202e();
void Func_02001f84();
void Func_02001f1e();
void Func_0200204c();
void Func_02001fa2();
void Func_02001f3c();
void Func_0200206a();
void Func_02001fc0();
void Func_02002076();
void Func_0200205e();
void Func_02001fee();
void Func_02001fdc();
void Func_02001f76();
void Func_020020a4();
void Func_02001ffa();
void Func_02001f94();
void Func_020020c2();
void Func_02002018();
void Func_02001fb2();
void Func_020020e0();
void Func_02002036();
void Func_02001fd0();
void Func_020020fe();
void Func_02002054();
void Func_0200211c();
void Func_02002072();
void Func_0200200c();
void Func_0200213a();
void Func_02002090();
void Func_0200202a();
void Func_02002158();
void Func_020020ae();
void Func_02002048();
void Func_02002176();
void Func_020020cc();
void Func_02002066();
void Func_02002194();
void Func_020020ea();
void Func_02002084();
void Func_020021b2();
void Func_02002108();
void Func_020020a2();
void Func_020021d0();
void Func_02002126();
void Func_020020c0();
void Func_020021ee();
void Func_02002144();
void Func_020020de();
void Func_0200220c();
void Func_02002162();
void Func_020020fc();
void Func_0200222a();
void Func_02002180();

                     

                     

void Func_020004bc(s32 actor)
{
    Func_02001de4();
    Func_02001de2(30);
    Func_02001e98(148);
    Func_02001dee(100);
    Func_02001e62(0, 0xc000, 0);
    Func_02001e00(40);
    Func_02001d9e(82, 20, 70, 0, 3, 8);
    Func_02001e1c(3);
    Func_02001db6(85, 20, 70, 0, 3, 8);
    Func_02001ee4(154);
    Func_02001e3a(8);
    Func_02001dd4(88, 20, 70, 0, 3, 8);
    Func_02001f02(154);
    Func_02001e58(8);
    Func_02001df2(91, 20, 70, 0, 3, 8);
    Func_02001f20(154);
    Func_02001e76(8);
    Func_02001e10(94, 20, 70, 0, 3, 8);
    Func_02001f3e(154);
    Func_02001e94(8);
    Func_02001e2e(97, 20, 70, 0, 3, 8);
    Func_02001f5c(154);
    Func_02001eb2(8);
    Func_02001e4c(100, 20, 70, 0, 3, 8);
    Func_02001f7a(154);
    Func_02001ed0(8);
    Func_02001e6a(79, 29, 70, 0, 3, 8);
    Func_02001f98(154);
    Func_02001eee(8);
    Func_02001e88(82, 29, 70, 0, 3, 8);
    Func_02001fb6(154);
    Func_02001f0c(8);
    Func_02001ea6(85, 29, 70, 0, 3, 8);
    Func_02001fd4(154);
    Func_02001f2a(8);
    Func_02001ec4(88, 29, 70, 0, 3, 8);
    Func_02001ff2(154);
    Func_02001f48(8);
    Func_02001ee2(91, 29, 70, 0, 3, 8);
    Func_02002010(154);
    Func_02001f66(8);
    Func_02001f00(94, 29, 70, 0, 3, 8);
    Func_0200202e(154);
    Func_02001f84(8);
    Func_02001f1e(97, 29, 70, 0, 3, 8);
    Func_0200204c(154);
    Func_02001fa2(8);
    Func_02001f3c(100, 29, 70, 0, 3, 8);
    Func_0200206a(154);
    Func_02001fc0(70);
    Func_02002076(126);
    Func_0200205e(actor, 3);
    Func_02001fee(actor, 0);
    Func_02001fdc(20);
    Func_02001f76(97, 29, 70, 0, 3, 8);
    Func_020020a4(154);
    Func_02001ffa(8);
    Func_02001f94(94, 29, 70, 0, 3, 8);
    Func_020020c2(154);
    Func_02002018(8);
    Func_02001fb2(91, 29, 70, 0, 3, 8);
    Func_020020e0(154);
    Func_02002036(8);
    Func_02001fd0(88, 29, 70, 0, 3, 8);
    Func_020020fe(154);
    Func_02002054(8);
    Func_02001fee(85, 29, 70, 0, 3, 8);
    Func_0200211c(154);
    Func_02002072(8);
    Func_0200200c(82, 29, 70, 0, 3, 8);
    Func_0200213a(154);
    Func_02002090(8);
    Func_0200202a(100, 20, 70, 0, 3, 8);
    Func_02002158(154);
    Func_020020ae(8);
    Func_02002048(97, 20, 70, 0, 3, 8);
    Func_02002176(154);
    Func_020020cc(8);
    Func_02002066(94, 20, 70, 0, 3, 8);
    Func_02002194(154);
    Func_020020ea(8);
    Func_02002084(91, 20, 70, 0, 3, 8);
    Func_020021b2(154);
    Func_02002108(8);
    Func_020020a2(88, 20, 70, 0, 3, 8);
    Func_020021d0(154);
    Func_02002126(8);
    Func_020020c0(85, 20, 70, 0, 3, 8);
    Func_020021ee(154);
    Func_02002144(8);
    Func_020020de(82, 20, 70, 0, 3, 8);
    Func_0200220c(154);
    Func_02002162(8);
    Func_020020fc(79, 20, 70, 0, 3, 8);
    Func_0200222a(154);
    Func_02002180(8);
    Func_02002194();

}
