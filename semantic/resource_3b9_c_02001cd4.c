#include "types.h"

/*
 * resource_3b9 owner at 0x02001cd4, 1804 bytes: this overlay's longest
 * scripted-scene script -- the same bracket its twin 0x02002964 opens
 * and closes, with a very long body of actor position/animation beats
 * between them, and one gate branch near the end.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl spill at
 * 0x02001cd4 through `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020023b8-0x020023c4; a zero alignment halfword at 0x020023c6, then
 * the six-word tail pool 0x020023c8-0x020023df (0x03001ebc, 0x105,
 * 0x0200adac, 0x33e, 0x6666, 0xccc). The next owner's prologue is
 * exactly at 0x020023e0. **1804 bytes**, measured to the named
 * epilogue -- an earlier note called this row ">700 bytes" off
 * a truncated dump and was wrong by a factor of 2.5.
 *
 * THIS ROW CARRIES AN INTERIOR LITERAL POOL. `b.n 0x02002154` at
 * 0x0200213a jumps over the six-word pool 0x0200213c-0x02002153
 * (0x03001ebc, 0x00010002, 0x20f8, 0x105, 0x103, 0x101), and the body
 * resumes at 0x02002154 with the gate's compare.
 *
 * When this row was drafted, `tools/overlay_show.ts` SILENTLY DROPPED a
 * second positional bound and fell back to its own extent scan, which
 * stops at the first return-shaped halfword -- and a pool word routinely
 * looks like `bx rN`. So `overlay_show resource_3b9 1cd4 23e0` returned
 * 122 of this row's 180 call sites under a tidy "pool words referenced"
 * footer, which reads exactly like a finished function. That bound is now
 * honoured and the tool rejects anything it cannot use; the same command
 * returns all 180. The cross-check that caught it in the first place is
 * still the cheap one: `overlay_call_targets` prints `sites=N`, and the
 * `bl` lines transcribed must equal it.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`. All 180 call sites resolved with
 * `bun tools/overlay_call_targets.ts resource_3b9 1cd4 23e0` under the
 * `+2` rule; the census (36 x Func_0808a1b8, 25 x Func_02001c48,
 * 24 x Func_0808a1e8, 17 x Func_02001c5c, ...) is reproduced exactly by
 * the calls below. 138 veneer sites and 42 to this overlay's own two
 * trampolines Func_02001c48/Func_02001c5c; zero intra-body targets, so
 * the bl-as-long-branch shape does NOT appear here.
 *
 * TWIN OF 0x02002964, cross-verified rather than derived. Two links
 * were found before drafting -- byte-identical fixed placements at the
 * head (`Func_0808a0f0(1, 198 << 18, 136 << 16)` and
 * `(2, 206 << 18, 136 << 16)`) and the same shared descriptor
 * 0x0200adac in the pool -- and the closing run from
 * `Func_0808a1b8(1, 224 << 8, 0)` to `Func_0808a248(64)` is then a
 * statement-for-statement match with semantic/overlays/
 * resource_3b9_c_02002964.c, with three real departures:
 *   1. three extra beats (`Func_0808a100(1, 3)`, `(2, 3)`,
 *      `Func_0808a110(3, 3)`) before the Func_0808a090 block;
 *   2. the first `Func_0808a0c0(11, ...)` passes the POOLED 0x33e (830)
 *      where the twin passes `202 << 2` (808) -- 830 is odd-shifted and
 *      cannot be built by movs+lsls, which is why it is pooled;
 *   3. this row writes the workspace pair that the twin does not.
 * The head is likewise NOT identical: the twin goes straight from the
 * three placements to Func_0808a360/Func_0808a370, where this row runs
 * a Func_080000c0 / Func_0808a210 / Func_0808a338 / Func_0808a330 /
 * Func_0808a348 opening and the first workspace write first.
 *
 * THE DISPLACEMENT-FROM-VALUE CHAIN, THIRD SIGHTING, AND BOTH ENDS OF
 * THIS ROW USE IT. Head at 0x02001d46:
 *     movs r3,#224 / lsls r3,#1   -> 448      displacement
 *     adds r2, r1, r3                         &workspace[448]
 *     adds r3, #67                -> 515      VALUE
 *     str  r3, [r2]                           workspace+448 = 515
 *     subs r3, #59                -> 456      displacement, FROM 515
 * Tail at 0x02002394 runs the same chain the other way (448 -> value
 * 256 -> +200 -> displacement 456). Read either `adds`/`subs` against
 * the BASE 448 rather than against the value just stored and you write
 * the wrong field: 448+67 is not a displacement and 448+200 is 648.
 * `resource_3b9_c_020011c4.c` has the chain a third time (448 -> 512 ->
 * 456), so it is this compiler's normal habit for a two-field workspace
 * write, not a curiosity.
 *
 * THE GATE'S POLARITY, READ AND NOT INHERITED. At 0x02002154 the test
 * is `cmp r0,#1 / bne.n`, so the fall-through THEN arm is taken when
 * Func_0808a070(0, 0) returns exactly 1 -- the OPPOSITE sense to
 * `resource_3b9_c_0200039c.c` and `resource_3b9_c_020006bc.c`, which
 * test the same call for zero and for non-zero respectively. This
 * overlay has now inverted a branch four times; the sense is read per
 * site.
 *
 * The skip-beat counter here IS the converging shape: the else arm
 * bumps `*(u16 *)(workspace + 472)` and the then arm sets a flag that
 * causes the same bump after the join, so workspace+472 advances
 * exactly once on either path. That is the form recorded for the
 * resource_3c9 copies, and NOT the form of 0x0200039c, which bumps once
 * or twice. Two shapes coexist in one overlay.
 *
 * SHARED IDIOMS: scripted-scene bracket open/close, the id-8/9/10/11
 * trampoline pair (Func_02001c48/Func_02001c5c), and the
 * Func_0808a098/Func_0808a0b0 `(id, descriptor)` pair are all cited
 * from resource_3b9_c_02002964.c and resource_3b9_c_0200160c.c rather
 * than re-derived. 0x0200adac is an in-image data address and is EVEN,
 * so it is a descriptor and not a published function pointer -- do not
 * feed it to the published sweep.
 *
 * Uncertainty: none of the thirty-one callees are identified beyond
 * call shape; every id, coordinate and constant below is recorded as a
 * passed value. What Func_0808a070(0, 0) reports is unknown; only the
 * compare against 1 is transcribed.
 */

extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a0b0(s32 id, void *data);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a338(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_02001c48(s32 id);
extern void Func_02001c5c(s32 id, s32 arg1);

extern u8 *Data_03001ebc;

void Func_02001cd4(void)
{
    void *shared;
    u8 *workspace;
    s32 took_gate;

    Func_0808a018();
    Func_0808a0f0(1, 198 << 18, 136 << 16);
    Func_0808a0f0(2, 206 << 18, 136 << 16);
    Func_0808a0f0(3, 202 << 18, 152 << 16);
    Func_080000c0(1);
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a338(0, 0);
    Func_0808a330(0, 0);
    Func_0808a348(1);
    Func_080000c0(1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 515;
    *(s32 *)(workspace + 456) = 1;

    Func_0808a360();
    Func_0808a370();
    Func_0808a338(0, 0);
    Func_0808a330(0x00010002, 0);
    Func_0808a348(40);

    Func_0808a010(80);
    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_0808a138(2, 2);
    Func_0808a010(40);
    Func_02001c5c(8, 192 << 6);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a330(128 << 9, 0);
    Func_0808a348(40);
    Func_0808a010(80);
    Func_0808a1e8(2, 128 << 1, 0);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a170(0x20f8);
    Func_02001c48(2);
    Func_0808a110(8, 3);
    Func_02001c48(8);
    Func_0808a1e8(3, 129 << 1, 40);
    Func_02001c48(3);

    Func_0808a1b8(8, 128 << 8, 0);
    Func_0808a1b8(9, 0, 40);
    Func_0808a1b8(8, 160 << 7, 0);
    Func_02001c5c(9, 192 << 6);
    Func_0808a1e8(9, 0x105, 20);
    Func_02001c48(9);
    Func_0808a1e8(1, 0x103, 20);
    Func_0808a130(1, 2);
    Func_02001c48(1);
    Func_0808a1e8(10, 129 << 1, 40);
    Func_02001c48(10);
    Func_0808a110(11, 3);
    Func_02001c48(11);
    Func_02001c5c(2, 160 << 8);
    Func_0808a110(2, 4);
    Func_02001c48(2);
    Func_0808a138(3, 1);
    Func_02001c48(3);
    Func_0808a1e8(1, 0x103, 20);
    Func_02001c5c(1, 0);
    Func_0808a130(1, 2);
    Func_02001c48(1);
    Func_02001c5c(0, 192 << 7);
    Func_0808a1e8(0, 129 << 1, 60);
    Func_0808a138(2, 1);
    Func_02001c5c(2, 128 << 8);
    Func_02001c48(2);
    Func_0808a100(3, 4);
    Func_0808a010(20);
    Func_02001c48(3);
    Func_0808a138(1, 1);
    Func_02001c5c(1, 128 << 6);
    Func_02001c48(1);
    Func_0808a1e8(2, 0x105, 60);
    Func_0808a1b8(2, 128 << 6, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(3, 224 << 8, 20);

    Func_0808a1e8(0, 129 << 1, 0);
    Func_0808a1e8(1, 129 << 1, 0);
    Func_0808a1e8(3, 129 << 1, 80);
    Func_0808a1e8(2, 131 << 1, 0);
    Func_0808a138(2, 1);
    Func_02001c5c(2, 192 << 8);
    Func_02001c48(2);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 20);
    Func_02001c5c(8, 192 << 6);
    Func_0808a110(8, 3);
    Func_02001c48(8);
    Func_0808a1e8(0, 128 << 1, 0);
    Func_0808a1e8(1, 128 << 1, 0);
    Func_0808a1e8(3, 128 << 1, 40);
    Func_0808a138(9, 1);
    Func_02001c48(9);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 40);
    Func_0808a138(10, 1);
    Func_02001c48(10);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_02001c5c(3, 192 << 8);
    Func_0808a110(11, 3);
    Func_02001c48(11);
    Func_0808a1b8(0, 192 << 7, 0);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 192 << 7, 0);
    Func_0808a1b8(3, 224 << 8, 20);

    Func_0808a1e8(1, 0x105, 0);
    Func_0808a1e8(2, 0x105, 0);
    Func_0808a1e8(3, 0x105, 80);
    Func_0808a1e8(2, 131 << 1, 0);
    Func_0808a138(2, 1);
    Func_02001c5c(2, 224 << 8);
    Func_02001c48(2);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0, 20);
    Func_0808a1e8(11, 0x101, 60);
    Func_0808a178(2, 0);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 0);

    took_gate = 0;
    if (Func_0808a070(0, 0) == 1) {
        Func_02001c48(2);
        took_gate = 1;
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_0808a110(2, 3);
        Func_02001c5c(2, 192 << 8);
        Func_02001c48(2);
    }
    if (took_gate != 0) {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
    }

    Func_0808a1b8(8, 128 << 8, 0);
    Func_0808a1b8(9, 0, 0);
    Func_0808a1b8(10, 192 << 6, 0);
    Func_0808a1b8(11, 176 << 8, 20);
    Func_0808a1e8(8, 0x105, 0);
    Func_0808a1e8(9, 0x105, 0);
    Func_0808a1e8(10, 0x105, 0);
    Func_0808a1e8(11, 0x105, 60);
    Func_0808a138(8, 1);
    Func_02001c5c(8, 192 << 6);
    Func_02001c48(8);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 20);
    Func_0808a138(9, 1);
    Func_02001c5c(9, 192 << 6);
    Func_02001c48(9);
    Func_0808a138(10, 1);
    Func_02001c5c(10, 160 << 7);
    Func_02001c48(10);
    Func_02001c5c(11, 128 << 8);
    Func_0808a110(11, 3);
    Func_02001c48(11);

    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(2, 160 << 8, 20);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 128 << 9, 128 << 8);

    shared = (void *)0x0200adac;
    Func_0808a098(1, shared);
    Func_0808a098(2, shared);
    Func_0808a0b0(3, shared);

    Func_0808a010(20);
    Func_02001c5c(0, 0);
    Func_0808a110(0, 3);
    Func_0808a110(11, 3);
    Func_0808a090(11, 128 << 9, 128 << 8);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a100(11, 2);
    Func_0808a0c0(11, 0x33e, 152);
    Func_0808a0c0(11, 202 << 2, 164);
    Func_0808a0b8(11, 202 << 2, 156 << 1);

    Func_0808a010(20);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(202 << 18, -1, 156 << 17, 1);
    Func_0808a0d0(0, 202 << 2, 164);
    Func_0808a0c8(0, 202 << 2, 156 << 1);

    Func_0808a010(60);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 40;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(64);
}
