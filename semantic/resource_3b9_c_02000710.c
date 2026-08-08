#include "types.h"

/*
 * resource_3b9 owner at 0x02000710, 1724 bytes: the overlay's flag-gated
 * set-piece. A story flag opens it, a long linear staging block runs, and
 * then TWO nested Func_0808a070 gates choose between three tails that
 * reconverge on one closing sequence.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000710 through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000da8-0x02000dac; a zero
 * alignment halfword at 0x02000dae, then the seven-word tail literal pool
 * 0x02000db0-0x02000dcb (0x03001ebc, 0x0200ad3c, 0x6666, 0xccc,
 * 0x0200ade4, 0x0200ad74, 0x93f). The next owner's prologue is exactly at
 * 0x02000dcc. **1724 bytes**, measured to the named epilogue.
 *
 * THIS ROW CARRIES AN INTERIOR LITERAL POOL at 0x02000ab6-0x02000aeb,
 * jumped over by `b.n 0x02000c90` at 0x02000ab4; the body resumes at
 * 0x02000aec.
 *
 * When this row was drafted, `tools-rs/overlay-show` SILENTLY DROPPED a
 * second positional bound and fell back to its own extent scan, which
 * stops at the first return-shaped halfword. Given the true bounds
 * `710 dcc` it returned 101 of this row's 177 call sites -- losing BOTH
 * inner gates, two of the three tails and the epilogue, so a draft off
 * that listing would have been a branchless linear script: wrong about
 * the shape, not merely short. That bound is now honoured and the same
 * command returns all 177. The cheap cross-check remains the real
 * backstop: `overlay_call_targets` prints `sites=N` for the true bounds,
 * and the `bl` lines transcribed must equal it.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`. All 177 targets resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3b9 710 dcc` under the `+2`
 * rule -- 151 veneer sites, 26 to this overlay's own trampolines
 * Func_02001c48/Func_02001c5c, ZERO intra-body. The bl-as-long-branch
 * shape does not appear here; the `bne.n`-over-`b.n` pair at 0x0200071a
 * that resembles it is a plain long conditional branch, which is what that
 * idiom is for.
 *
 * CONTROL FLOW, read from the branches rather than assumed:
 *   0x0200071a  `cmp r0,#0 / bne.n 0x0200071e / b.n 0x02000da8` -- the
 *               whole body runs only when Func_080770c0(0x962) is
 *               NON-zero. 0x962 is this bank's shared story flag, and
 *               0x93f is SET by Func_080770c8 on the way out --
 *               Func_080770c8 is the setter, not the clearer; the clearer
 *               is Func_080770d0, per resource_3b9_c_020011ac.c which
 *               calls both.
 *   0x02000a34  `cmp r0,#0 / bne.n 0x02000aec` -- outer gate. The ZERO
 *               arm is the fall-through at 0x02000a36.
 *   0x02000c12  `cmp r0,#0 / bne.n 0x02000ca2` -- inner gate, inside the
 *               outer NON-zero arm, same polarity.
 * Both gates are `!= 0` here. That is the THIRD distinct sense this
 * overlay gives the same Func_0808a070(0, 0) call: 0x0200039c tests it for
 * zero, 0x020006bc for non-zero via a `beq`, 0x02001cd4 compares it
 * against 1. The sense is read per site, never inherited.
 *
 * THE OUTER ZERO ARM AND THE INNER ZERO ARM ARE THE SAME THIRTEEN CALLS,
 * EMITTED TWICE. 0x02000a36-0x02000ab4 and 0x02000c14-0x02000c8e are
 * instruction-for-instruction identical apart from which literal pool
 * supplies 0x0200ad3c/0x0200ade4 (the interior pool for the first, the
 * tail pool for the second). The compiler cross-jumped only their common
 * TAIL -- the first arm's `b.n 0x02000c90` lands on the shared
 * workspace-write that the second falls into -- and left the thirteen
 * calls duplicated. They are written out twice below because that is what
 * the row emits; factoring them would misreport the shape.
 *
 * The closing `workspace + 456 = 40` / Func_0808a368 / Func_0808a370
 * sequence is emitted TWICE, not three times: 0x02000c90 serves the outer
 * zero arm and the inner zero arm, and 0x02000d84 is a private copy for
 * the inner non-zero arm, which then falls into the shared
 * Func_0808a248(2) at 0x02000d98. Both physical copies are explicit below;
 * labels keep the two zero arms converged on the first without duplicating
 * it in source. The resulting source and machine censuses are both 177.
 *
 * The join at 0x02000c90 is reached with `r3` already holding the
 * workspace pointer and `r2` already holding 228, set separately in each
 * arriving arm; the join itself does `lsls r2,#1` for the displacement
 * 456. That is an ordinary `228 << 1`, NOT the displacement-from-value
 * chain that 0x02001cd4 and 0x020011c4 use -- there is only one field
 * written here, so there is no previous value to derive from.
 *
 * Func_0808a228 returns a record pointer whose byte at +85 is cleared,
 * the same record field 0x020011c4 writes. Func_0808a080(0) returns a
 * pointer that may be null and is guarded each time; its +8 and +16 words
 * are forwarded as the two coordinates of Func_0808a0f0, three times with
 * ids 1, 2 and 3. Three separate calls, three separate null tests -- not
 * one lookup reused.
 *
 * Three pool words are 0x02008000-based image data pointers: 0x0200ad3c,
 * 0x0200ade4 and 0x0200ad74 (image 0x2d3c, 0x2de4, 0x2d74). All EVEN, so
 * descriptors and not published function pointers -- do not feed them to
 * the published sweep.
 *
 * SHARED IDIOMS: scripted-scene bracket, the trampoline pair
 * (Func_02001c48/Func_02001c5c) and the Func_0808a098/Func_0808a0b0
 * `(id, descriptor)` pair are cited from resource_3b9_c_02002964.c and
 * resource_3b9_c_0200160c.c. The skip-beat counter
 * `*(u16 *)(workspace + 472) += 1` is cited from
 * resource_3b9_c_0200039c.c; here it is NOT a converging bump -- it fires
 * only on the two non-zero gate arms, so the counter advances zero, one or
 * two times depending on how far the gates run.
 *
 * Uncertainty: none of the thirty-two callees are identified beyond call
 * shape; every id, coordinate and constant below is recorded as a passed
 * value. Actor ids 17-20 and 0-3 are two distinct id spaces only by
 * inference from how they are used.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, void *data);
extern void Func_0808a0b0(s32 id, void *data);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern u8 *Func_0808a228(void);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_02001c48(s32 id);
extern void Func_02001c5c(s32 id, s32 arg1);

extern u8 *Data_03001ebc;

void Func_02000710(void)
{
    u8 *record;
    u8 *actor;
    u8 *workspace;

    if (Func_080770c0(0x962) == 0)
        return;

    Func_0808a018();
    Func_0808a138(17, 1);
    Func_0808a1b8(17, 192 << 6, 20);
    Func_0808a1b8(17, 0, 60);
    Func_0808a1e8(17, 128 << 1, 40);
    Func_02001c5c(17, 192 << 6);
    Func_0808a170(0x2267);
    Func_0808a130(17, 2);
    Func_02001c48(17);
    Func_0808a1b8(18, 192 << 6, 0);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_0808a1b8(20, 192 << 6, 0);

    record = Func_0808a228();
    *(u8 *)(record + 85) = 0;

    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(128 << 17, -1, 172 << 16, 1);
    Func_0808a218();

    Func_0808a010(20);
    Func_0808a1e8(18, 129 << 1, 40);
    Func_02001c48(18);
    Func_0808a110(17, 3);
    Func_02001c48(17);
    Func_0808a1f0(19, 129 << 1);
    Func_0808a010(40);
    Func_02001c5c(19, 0);
    Func_02001c48(19);
    Func_0808a1e8(20, 0x103, 40);
    Func_02001c5c(20, 0);
    Func_0808a130(20, 2);
    Func_0808a188(20, 0, 20);
    Func_0808a138(17, 1);
    Func_0808a010(10);
    Func_0808a1b8(17, 176 << 8, 20);
    Func_0808a188(17, 0, 20);
    Func_0808a138(18, 1);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_02001c5c(17, 128 << 8);
    Func_0808a188(17, 0, 20);
    Func_0808a138(19, 1);
    Func_0808a010(40);
    Func_0808a110(19, 4);
    Func_0808a188(17, 0, 20);
    Func_0808a138(20, 2);
    Func_0808a010(40);
    Func_0808a110(20, 3);
    Func_0808a010(20);

    Func_0808a100(17, 3);
    Func_0808a100(18, 3);
    Func_0808a100(19, 3);
    Func_0808a110(20, 3);
    Func_0808a1b8(17, 192 << 6, 0);
    Func_0808a1b8(18, 192 << 6, 0);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_02001c5c(20, 192 << 6);
    Func_0808a090(17, 0x9999, 0x4ccc);
    Func_0808a0d0(17, 129 << 1, 172);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 131 << 1, 188);
    Func_0808a1b8(0, 192 << 8, 0);

    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0f0(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0f0(2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0f0(3, *(s32 *)(actor + 8), *(s32 *)(actor + 16));

    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a0c8(1, 246, 200);
    Func_0808a0c8(2, 131 << 1, 200);
    Func_0808a0d0(3, 139 << 1, 200);
    Func_0808a100(2, 1);
    Func_0808a100(1, 1);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 40);
    Func_0808a138(17, 1);
    Func_02001c48(17);
    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(2, 2);
    Func_0808a138(3, 2);
    Func_0808a110(17, 3);
    Func_0808a178(17, 0);
    Func_0808a1b8(1, 224 << 8, 0);
    Func_0808a1b8(3, 160 << 8, 0);

    if (Func_0808a070(0, 0) != 0) {
        goto outer_gate_open;
    }

    Func_0808a110(17, 3);
    Func_02001c48(17);
    Func_0808a098(1, (void *)0x0200ad3c);
    Func_0808a098(2, (void *)0x0200ad3c);
    Func_0808a0b0(3, (void *)0x0200ad3c);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(128 << 17, -1, 200 << 15, 1);
    Func_0808a090(17, 128 << 9, 128 << 8);
    Func_0808a098(17, (void *)0x0200ade4);
    Func_0808a010(10);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a098(0, (void *)0x0200ade4);
    Func_0808a010(80);
    goto shared_gate_close;

outer_gate_open:
    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;

    Func_0808a1e8(17, 129 << 1, 40);
    Func_02001c48(17);
    Func_0808a1e8(18, 129 << 1, 40);
    Func_02001c48(18);
    Func_0808a1e8(19, 129 << 1, 40);
    Func_02001c48(19);
    Func_0808a1e8(20, 129 << 1, 40);
    Func_02001c48(19);
    Func_0808a110(17, 4);
    Func_02001c48(17);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_02001c48(2);
    Func_0808a1e8(3, 129 << 1, 40);
    Func_02001c48(3);
    Func_0808a1b8(17, 176 << 8, 0);
    Func_0808a1b8(19, 128 << 8, 0);
    Func_0808a1b8(20, 0, 60);
    Func_0808a1b8(17, 192 << 6, 0);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_0808a1b8(20, 192 << 6, 20);
    Func_0808a100(17, 3);
    Func_0808a100(18, 3);
    Func_0808a100(19, 3);
    Func_0808a110(20, 3);
    Func_0808a1e8(3, 128 << 1, 60);
    Func_02001c5c(3, 160 << 8);
    Func_02001c48(3);
    Func_0808a100(1, 4);
    Func_0808a010(20);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) != 0) {
        goto inner_gate_open;
    }

    /* Same thirteen-call arm as the outer zero path, physically emitted twice. */
    Func_0808a110(17, 3);
    Func_02001c48(17);
    Func_0808a098(1, (void *)0x0200ad3c);
    Func_0808a098(2, (void *)0x0200ad3c);
    Func_0808a0b0(3, (void *)0x0200ad3c);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(128 << 17, -1, 200 << 15, 1);
    Func_0808a090(17, 128 << 9, 128 << 8);
    Func_0808a098(17, (void *)0x0200ade4);
    Func_0808a010(10);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a098(0, (void *)0x0200ade4);
    Func_0808a010(80);

shared_gate_close:
    workspace = Data_03001ebc;
    Func_0808a368();
    *(s32 *)(workspace + 456) = 40;
    goto gates_done;
    Func_0808a370();

inner_gate_open:
    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;

    Func_0808a010(20);
    Func_0808a130(1, 2);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(2, 129 << 1, 60);
    Func_02001c48(2);
    Func_02001c5c(3, 128 << 8);
    Func_0808a100(3, 3);
    Func_02001c48(3);
    Func_0808a098(2, (void *)0x0200ad74);
    Func_0808a0b0(3, (void *)0x0200ad74);
    Func_0808a010(20);
    Func_0808a0b0(0, (void *)0x0200ad74);
    Func_0808a0d0(1, 131 << 1, 188);
    Func_02001c5c(1, 192 << 8);
    Func_0808a110(1, 3);
    Func_02001c48(1);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(128 << 17, -1, 200 << 15, 1);
    Func_0808a090(17, 128 << 9, 128 << 8);
    Func_0808a098(17, (void *)0x0200ade4);
    Func_0808a010(10);
    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a098(1, (void *)0x0200ade4);
    Func_0808a010(80);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 456) = 40;
    Func_0808a368();
    Func_0808a370();

gates_done:

    Func_0808a248(2);
    Func_080770c8(0x93f);
    Func_0808a020();
}
