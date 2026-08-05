#include "types.h"

/*
 * Resource 37a gated cutscene at 0x02001ca8 (1120 bytes,
 * 0x02001ca8-0x02002107).
 *
 * Complete owner: `push {r5, r6, r7, lr}` / `mov r7, r8` / `push {r7}` /
 * `sub sp, #8` prologue at 0x02001ca8 and the matching interworking return
 * `add sp, #8 / pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x020020fc.  r0 holds the popped return address, so nothing is returned:
 * the owner is `void`.  r8 is saved and restored, and holds the constant 3
 * across the whole scripted block.
 *
 * Pool map, derived by walking the owner's control flow from the prologue: the
 * only unreached range is 0x02002092-0x020020c7, the single literal pool.  The
 * bare `b.n 0x020020c8` at 0x02002090 hops it, and that hop is mid-computation:
 * r1 (the runtime record, loaded at 0x0200208a) and r3 (448, built at
 * 0x0200208c) are both live across it and are consumed at 0x020020c8.  Register
 * state must be carried across the pool or the runtime-defaults write is lost.
 * Code resumes at 0x020020c8 and runs to the epilogue, so there is no trailing
 * pool.  Pool words:
 *   0x00000811 0x011f0000 0x00000819 0x00008010 0x0000102e 0x00001333
 *   0x00009999 0x00004ccc 0x00002666 0x00003333 0x00001999 0xfffffc00
 *   0x03001ebc
 * overlay_show.ts's "pool words referenced" listing additionally names
 * 0x020023e4; that is the usual artefact of the constant 0x00004ccc decoding as
 * `ldr r4, [pc, #816]`, not a word of this owner.
 *
 * All 91 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are not the callees.
 * sites=91, distinct_targets=32 (89 veneer, 2 overlay-local prologue, both to
 * the two-argument helper Func_020025fc), against the inventory's calls=84
 * (that field counts distinct targets and is a known low lower bound).
 *
 * Control flow, all three branches verified against the resolved targets:
 *  - entry gate: `Func_080770c0(0x0811)` at 0x02001cb2; zero branches straight
 *    to the epilogue at 0x020020fc, so the whole scene is one-shot on flag
 *    0x0811 (the same flag the sibling owner at 0x020025b0 tests, and the one
 *    Func_02001be8 sets).
 *  - `Func_080770c0(0x0819)` at 0x02001d64 gates one extra Func_080f9010(0xdc)
 *    cue when the flag is CLEAR (`cmp r0,#0 / bne` skips it when set).
 *  - `Func_080770c0(0x0819)` again at 0x02001d7a: when the flag is SET the
 *    whole scripted dialogue block 0x02001d84-0x02001eb0 is skipped by a
 *    forward `b.n 0x02001eb2`; the block ends by setting flag 0x0819 through
 *    Func_080770c8, so it plays exactly once.
 * Func_080770c0/Func_080770c8 are the global story-flag test/set pair resolved
 * in semantic/overlays/resource_3cb_c_020010e8.c.
 *
 * Func_08009180 is the established six-argument renderer entry (r0-r3 plus
 * [sp,#0] and [sp,#4]).  Across the dialogue block the two stack slots are fed
 * from three long-lived registers set once at the top — r5 = 2 (always
 * [sp,#0]), r6 = 1 and r8 = 3 (alternating [sp,#4]) — and are frequently
 * written BEFORE the register arguments.  Those lifetimes were recovered from
 * the predecessor blocks; a per-site argument window would drop them.
 *
 * Func_0808a080 is the scene-record accessor (the sense established in
 * HANDOVER: 0x0808a080 is the accessor, not the four-argument action).  It is
 * called twice, with selector 0 and selector 16, and each result is used the
 * same way: clear bit 0 of the byte at +0x5a, zero the byte at +0x55, then
 * advance the 16.16 word at +0x0c by 0x3333 for 120 frames and by 0x1999 for a
 * further 60, while the sub-record pointed to by the word at +0x50 has its byte
 * at +0x26 zeroed and its 16.16 word at +0x18 advanced by -1024 during the
 * second phase.  Func_080000c0(1) is the per-frame advance inside both loops.
 * Neither result is null-checked here, unlike the sibling at 0x020009f4.
 *
 * The store block at 0x020020c8 is the shared runtime-defaults write: the
 * pointer table at 0x03001ebc yields the runtime record, whose word at +0x1c0
 * becomes 0x203 and whose word at +0x1c8 becomes 24.  In assembly r3 is a value
 * (448 + 67 = 515) and then a displacement (515 - 59 = 456 = 0x1c8); the two
 * uses are unrelated despite sharing the register.
 *
 * Shift-built constants are spelled with their assembled values and the source
 * shift noted, because the assembly builds them from 8-bit immediates.
 *
 * Uncertainties: 0x00008010, 0x0000102e, 0x00000811 and 0x00000819 are ids, not
 * addresses under the 0x02008000 link base; Func_0808a018 and Func_0808a020 are
 * reached with no argument register set by this owner; the record field widths
 * at +0x0c/+0x18 are taken from `ldr`/`str` and their fixed-point reading from
 * the magnitudes of the increments.  Arities are taken from the registers
 * written before each site.
 */

/* Overlay-local callee (two arguments). */
void Func_020025fc();

/* Old-style declarations: arities vary per call site across this overlay. */
void Func_080000c0();
void Func_08009180();
void Func_08009188();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a248();
void Func_0808a330();
void Func_0808a348();
void Func_0808a368();
void Func_0808a370();
void Func_080f9010();

/* Used for their return values. */
s32 Func_080770c0();
u8 *Func_0808a080();

/* The two identical drift phases applied to each scene record. */
static void drift_record(u8 *rec)
{
    u8 *sub;
    s32 i;

    rec[0x5a] &= 0xfe;
    rec[0x55] = 0;
    Func_080f9010(0xc9);
    Func_0808a158((s32)0, 256);                 /* selector patched below */
    sub = *(u8 **)(rec + 0x50);
    sub[0x26] = 0;

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x3333;
        i += 1;
        Func_080000c0(1);
    } while (i != 120);

    Func_080f9010(0xbe);

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x1999;
        *(s32 *)(sub + 0x18) += -1024;          /* 0xfffffc00 */
        i += 1;
        Func_080000c0(1);
    } while (i != 60);
}

void Func_02001ca8(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    u8 *rec;
    u8 *sub;
    s32 i;
    s32 two = 2;        /* r5 */
    s32 three = 3;      /* r8 */
    s32 one = 1;        /* r6 */
    s32 actor;

    if (Func_080770c0(0x811) == 0)
        return;

    Func_0808a018();
    Func_0808a208(0x10000, 0x2000);             /* 128 << 9, 128 << 6 */
    Func_0808a210(0x011f0000, -1, 0x940000, 1); /* 148 << 16 */
    Func_0808a0d0(0, 288, 120);                 /* 144 << 1 */
    Func_0808a100(0, 0);
    Func_0808a128(0, 4, 30);
    Func_0808a0f0(16, 0x1200000, 0x780000);     /* 144 << 17, 240 << 15 */
    Func_0808a090(16, 0x10000, 0x8000);         /* 128 << 9, 128 << 8 */
    Func_0808a0d0(16, 276, 136);                /* 138 << 1 */
    Func_0808a0c8(16, 264, 136);                /* 132 << 1 */
    Func_0808a0d0(0, 312, 136);                 /* 156 << 1 */
    Func_0808a100(0, 1);
    Func_0808a100(16, 1);
    Func_0808a1b8(0, 0xb000, 0);                /* 176 << 8 */
    Func_0808a1b8(16, 0xd000, 20);              /* 208 << 8 */

    if (Func_080770c0(0x819) == 0)
        Func_080f9010(0xdc);

    Func_0808a010(40);

    if (Func_080770c0(0x819) == 0) {
        Func_08009180(36, 62, 17, 36, two, three);
        Func_08009180(44, 59, 17, 38, two, one);
        Func_0808a010(10);
        Func_08009180(38, 62, 17, 36, two, three);
        Func_08009180(44, 59, 17, 39, two, one);
        Func_0808a010(10);
        Func_08009180(40, 62, 17, 36, two, three);
        Func_08009180(0, 32, 17, 39, two, one);
        Func_08009180(44, 59, 17, 40, two, one);
        Func_0808a010(10);
        Func_0808a1e8(0, 256, 0);               /* 128 << 1 */
        Func_0808a1e8(16, 256, 0);              /* 128 << 1 */
        Func_08009180(42, 62, 17, 36, two, three);
        Func_08009180(0, 32, 17, 40, two, one);
        Func_08009180(44, 59, 17, 41, two, one);
        Func_0808a010(10);
        Func_08009180(0, 32, 17, 41, two, one);
        Func_08009180(44, 59, 17, 42, two, one);
        Func_0808a010(10);
        Func_08009180(0, 32, 17, 42, two, three);
        Func_0808a010(80);
        Func_08009188(9);
        Func_08009188(10);
        Func_080770c8(0x819);
    }

    Func_0808a150(16, 0, 30);
    Func_0808a100(16, 3);
    Func_0808a110(0, 3);
    Func_0808a100(16, 1);
    actor = 0x8010;
    Func_0808a100(0, 0);
    Func_0808a170(0x102e);
    Func_020025fc(actor, 6);
    Func_0808a110(16, 3);
    Func_0808a100(16, 1);
    Func_0808a180(actor, 0);
    Func_0808a100(0, 3);
    Func_0808a010(60);

    rec = Func_0808a080(0);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x011f0000, -1, 0x720000, 1); /* 228 << 15 */
    Func_0808a0d0(0, 288, 120);                 /* 144 << 1 */
    Func_0808a010(20);
    Func_0808a1b8(0, 0xc000, 20);               /* 192 << 8 */
    Func_0808a090(0, 0x4ccc, 0x2666);

    Func_080f9010(0xc9);
    rec[0x55] = 0;
    Func_0808a158(0, 256);                      /* 128 << 1 */
    rec[0x5a] &= 0xfe;
    sub = *(u8 **)(rec + 0x50);
    sub[0x26] = 0;

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x3333;
        i += 1;
        Func_080000c0(1);
    } while (i != 120);

    Func_080f9010(0xbe);

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x1999;
        *(s32 *)(sub + 0x18) += -1024;          /* 0xfffffc00 */
        i += 1;
        Func_080000c0(1);
    } while (i != 60);

    Func_0808a0f0(0, 0, 0);
    Func_0808a128(16, 4, 20);
    Func_020025fc(16, 6);
    Func_0808a0d0(16, 288, 120);                /* 144 << 1 */
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_0808a1b8(16, 0xc000, 20);              /* 192 << 8 */
    Func_0808a090(16, 0x4ccc, 0x2666);

    rec = Func_0808a080(16);

    Func_080f9010(0xc9);
    Func_0808a158(16, 256);                     /* 128 << 1 */
    rec[0x55] = 0;
    rec[0x5a] &= 0xfe;
    sub[0x26] = 0;
    sub = *(u8 **)(rec + 0x50);

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x3333;
        i += 1;
        Func_080000c0(1);
    } while (i != 120);

    Func_080f9010(0xbe);

    i = 0;
    do {
        *(s32 *)(rec + 0x0c) += 0x1999;
        *(s32 *)(sub + 0x18) += -1024;          /* 0xfffffc00 */
        i += 1;
        Func_080000c0(1);
    } while (i != 60);

    Func_0808a0f0(16, 0, 0);
    Func_0808a010(80);

    runtime = runtime_globals[0];
    *(s32 *)(runtime + 0x1c0) = 0x203;
    *(s32 *)(runtime + 0x1c8) = 24;

    Func_0808a368();
    Func_0808a370();
    Func_0808a330(0, 0);
    Func_0808a348(1);
    Func_080000c0(1);
    Func_0808a248(7);
    Func_0808a020();
}
