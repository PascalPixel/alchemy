#include "types.h"

/*
 * resource_3aa owner at 0x02000360, 4268 bytes: file offsets 0x0360-0x140b.
 * The inventory splits it into three rows (0x0360, 0x04ae, 0x069c) but the
 * latter two are marked `contained_by` the first and there is no prologue or
 * epilogue between them: one `push {r5, r6, lr}` at 0x02000360 and one
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x020013f2.  This is a single owner and
 * the row after it (0x0200140c) is a different function.
 *
 * FIVE LITERAL POOLS sit inside the span and are DATA, not code:
 *   0x0200077c-0x020007a3 (10 words)  branched over by `b.n 0x020007a4`
 *   0x02000ac4-0x02000ad3 ( 4 words)  branched over by `b.n 0x02000afa`
 *   0x02000cd4-0x02000ce7 ( 5 words)  branched over by `b.n 0x02000d2a`
 *   0x020010f0-0x020010ff ( 4 words)  branched over by `b.n 0x02001100`
 *   0x020013f8-0x0200140b ( 5 words)  after the return
 * They were excluded from call detection outright rather than trusted to
 * decode as non-BL: no `bl` pair falls inside any of them, which is the check
 * that matters, since a pool word that decodes as a plausible register write
 * would silently corrupt an argument window.
 *
 * Two of those pools are jumped over with ARGUMENT REGISTERS ALREADY LIVE, and
 * that is the trap in this owner:
 *   0x02000770-0x02000776 sets r1 = 856 and r2 = 440, then branches over the
 *     first pool; r0 = 8 is set at 0x020007a4 and the call at 0x020007a6 is
 *     Func_0808a0d0(8, 856, 440).
 *   0x020010e8 sets r0 = 8 and r1 = 4, branches over the fourth pool, and the
 *     call at 0x02001100 is Func_0808a110(8, 4).
 * Both are reconstructed as single calls below.
 *
 * Return type, by the interworking-epilogue rule: `pop {r0} / bx r0` pops the
 * return address into r0, so the owner is void.  The first branch is taken
 * with no argument register written, so the owner takes no arguments.
 *
 * WHAT IT IS.  The overlay's long cutscene, the same construction as the
 * shorter one at 0x02001494 and roughly three times its length: bracketed by
 * Func_0808a018 / Func_0808a020, it places seven channels (0, 1, 2, 3, 8, 10,
 * 11), then plays a fixed sequence of pose / orientation / motion / wait steps
 * with four decision points, each on `Func_0808a070(0, 0)`, that bump a
 * halfword counter in the workspace by one or two.  It ends by publishing
 * request 521 (0x209) - the same code the dispatcher at 0x02000230 writes -
 * and firing two services.
 *
 * WORKSPACE.  r6 holds 0x03001ebc, the IWRAM slot carrying the scene
 * workspace pointer (also read by 0x02000184, 0x02000230 and 0x02001494).
 * Fields touched, all at the same displacements the sibling owners use:
 *   +448 (224 * 2) u32 request word: 513 (0x201) at the start, 521 (0x209) at
 *        the end.
 *   +456 (228 * 2) u32: 24 at the start, 16 at the end.
 *   +472 (236 * 2) u16 counter, incremented by 1 or 2 at eight places.
 * The four increments in decisions 3 and 4 reload the pointer from 0x03001ebc
 * through a fresh pool word instead of using r6; that reload is reproduced.
 *
 * REGISTER LIFETIMES.  r5 has two unrelated lives: a boolean flag over
 * 0x02000988-0x020009ac, and the pointer Data_02009b94 from 0x020013b0.  They
 * are spelled separately below.  r6 is live for the whole owner.
 *
 * CALL ACCOUNTING.  426 static `bl` sites, every one resolved with the
 * overlay displacement rule (`true_target = stored_displacement + 2`), applied
 * per site with the decoder behind `tools/overlay_call_targets.ts`:
 * 412 import veneers over 27 distinct imports, and 14 calls into this
 * overlay's own owners - 10 to Func_02001450 and 4 to Func_0200140c, both
 * already converted in this reconstruction.  No `call_via` site and no unresolved site.
 * Every one appears exactly once below.
 * NOTE: the inventory row advertises 396 calls for this span.  The disassembly
 * has 426, none inside the five pools, so the inventory figure is an
 * undercount; the same 7-8% undercount shows on this overlay's 0x1494 row.
 *
 * Import shapes reused from the already-converted overlays and from the
 * sibling owner 0x02001494:
 *   Func_0808a010(frames)                 -> frame wait.
 *   Func_0808a018()/Func_0808a020()       -> scripted-section brackets.
 *   Func_0808a090(selector, a, b)         -> 16.16 pair setter.
 *   Func_0808a0f0(selector, x, z)         -> 16.16 position.
 *   Func_0808a1b8(selector, value, frames)-> orientation over `frames`.
 *   Func_0808a1e8(selector, id, frames)   -> pose/animation over `frames`.
 *   Func_080770c8(id)/Func_080770d0(id)   -> one-argument services.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - Func_0808a188's first argument is a small selector (1, 3, 8, 10, 11) at
 *    most sites and a pooled 0x2002 / 0x6002 at others, the same
 *    flags | selector shape recorded in
 *    semantic/overlays/resource_373_c_02002cb0.c.  Left as literals.
 *  - The first decision point tests `Func_0808a070(0, 0) == 1` while the other
 *    three test against 0.  That asymmetry is in the assembly (`cmp r0, #1`
 *    versus `cmp r0, #0`) and is preserved rather than normalised.
 *  - Func_0808a338, Func_0808a330, Func_0808a348, Func_0808a360,
 *    Func_0808a370, Func_0808a180, Func_0808a190 and Func_0808a170 appear only
 *    here and in 0x02001494; only their argument counts are recovered.
 *  - Data_02009b94 is an in-image address: 0x02009b94 - 0x8000 = file offset
 *    0x1b94, inside the 0x2184-byte image, and the sibling owner 0x02001494
 *    passes the same pointer to the same three imports.
 *  - Func_0808a338 and Func_0808a330 are called back to back with the same
 *    (0x10002, 0) arguments from two different veneers; that is two distinct
 *    imports, not one, and the tool resolves them separately.
 */

/* Imports, named by the main-image address in the trailing word of each
 * overlay veneer. Old-style declarations are mandatory in this overlay. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a190();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_080000c0();
void Func_08009128();
void Func_080770c8();
void Func_080770d0();

/* Used for its return value: the scene query driving all four branches. */
s32 Func_0808a070();

/* In-overlay owners at file offsets 0x140c and 0x1450. */
void Func_0200140c();
void Func_02001450();

/* In-image data at file offset 0x1b94 (0x02009b94 - 0x8000). */
extern u8 Data_02009b94[];

/* IWRAM slot carrying the scene workspace pointer. */
#define WORKSPACE (*(u8 **)0x03001ebc)

#define REQUEST_WORD(w) (*(u32 *)((w) + 448))
#define SETUP_WORD(w)   (*(u32 *)((w) + 456))
#define EVENT_COUNT(w)  (*(u16 *)((w) + 472))

void Func_02000360(void)
{
    u8 *workspace;
    s32 skipped;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a338(0x10002, 0);
    Func_0808a330(0x10002, 0);
    Func_0808a348(1);
    Func_080000c0(1);

    SETUP_WORD(workspace) = 24;
    REQUEST_WORD(workspace) = 513;
    workspace = WORKSPACE;

    Func_0808a0f0(8, 0x03580000, 0x01b80000);
    Func_0808a0f0(0, 0x03580000, 0x01e60000);
    Func_0808a0f0(1, 0x03400000, 0x01f60000);
    Func_0808a0f0(2, 0x03680000, 0x01e60000);
    Func_0808a0f0(3, 0x03700000, 0x01f60000);
    Func_0808a0f0(10, 0x03480000, 0x02060000);
    Func_0808a0f0(11, 0x03780000, 0x02060000);
    Func_0808a210(0x03600000, -1, 0x01d80000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();

    Func_0808a010(40);
    Func_0808a138(8, 1);
    Func_0808a110(8, 3);
    Func_0808a010(10);
    Func_0808a138(2, 1);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a1b8(2, 0x8000, 20);
    Func_0808a138(0, 2);
    Func_0808a1b8(0, 0, 10);
    Func_0808a110(0, 3);
    Func_0808a130(1, 1);
    Func_0808a138(3, 1);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(3, 0xa000, 10);
    Func_0808a100(1, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_02001450(20);

    Func_0808a1f0(8, 258);
    Func_0808a130(8, 2);
    Func_0808a010(60);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a110(8, 3);
    Func_0808a1e8(10, 256, 0);
    Func_0808a130(10, 2);
    Func_0808a010(60);
    Func_0808a1b8(10, 0xf000, 20);
    Func_0808a138(11, 2);
    Func_0808a1b8(11, 0x9000, 40);
    Func_0808a1b8(10, 0xd000, 0);
    Func_0808a1b8(11, 0xb000, 20);
    Func_0808a110(11, 3);
    Func_0808a010(10);
    Func_0808a138(8, 2);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a138(11, 2);
    Func_0808a110(11, 4);
    Func_0808a110(11, 4);
    Func_0808a330(0x10000, 0);
    Func_0808a348(40);
    Func_080000c0(60);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a170(0x1b21);
    Func_0808a188(8, 0, 10);
    Func_0808a138(10, 2);
    Func_0808a188(10, 0, 10);
    Func_0808a110(11, 4);
    Func_0808a188(11, 0, 10);
    Func_0808a1b8(2, 0x6000, 10);
    Func_0808a188(0x6002, 0, 10);
    Func_0808a138(11, 2);
    Func_0808a188(11, 0, 10);
    Func_0808a100(10, 4);
    Func_0808a188(10, 0, 10);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a188(8, 0, 10);
    Func_0808a138(11, 2);
    Func_0808a188(11, 0, 10);
    Func_0808a1b8(2, 0x2000, 10);
    Func_0808a110(10, 3);
    Func_0808a188(10, 0, 10);
    Func_0808a1b8(2, 0x6000, 10);
    Func_0808a130(2, 1);
    Func_0808a188(0x6002, 0, 10);
    Func_0808a110(10, 3);
    Func_0808a1b8(2, 0x8000, 10);
    Func_0808a190(0x6002, 0);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_0808a188(8, 0, 10);
    Func_0808a138(2, 2);
    Func_0808a010(10);
    Func_0808a1b8(2, 0xc000, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a110(1, 3);
    Func_0808a188(1, 0, 10);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a110(3, 4);
    Func_0808a188(3, 0, 40);
    Func_0808a1b8(8, 0x1000, 40);
    Func_0808a090(8, 0x6666, 0x3333);
    Func_0808a0d0(8, 892, 440);
    Func_0808a010(40);
    Func_0808a1b8(8, 0xd000, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0200140c(60);
    Func_02001450(40);

    /* r1 and r2 are set before the branch over the first literal pool; r0 is
     * set on the far side. One call, not two. */
    Func_0808a0d0(8, 856, 440);

    Func_0808a010(40);
    Func_0808a1b8(8, 0x9000, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a1b8(10, 0xf000, 0);
    Func_0808a1b8(11, 0x9000, 40);
    Func_0808a1b8(10, 0xd000, 0);
    Func_0808a1b8(11, 0xb000, 10);
    Func_0808a138(8, 1);
    Func_0808a188(8, 0, 20);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 20);
    Func_0808a138(10, 2);
    Func_0808a188(10, 0, 10);
    Func_0808a138(8, 2);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0x6000, 0);
    Func_0808a1b8(3, 0x6000, 0);
    Func_0808a1b8(11, 0x9000, 40);
    Func_0808a1b8(10, 0xf000, 20);
    Func_0808a188(10, 0, 20);
    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a1b8(11, 0xb000, 20);
    Func_0808a110(11, 3);
    Func_0808a188(11, 0, 10);
    Func_0808a1b8(11, 0x9000, 40);
    Func_0808a100(10, 3);
    Func_0808a110(11, 3);
    Func_0808a1b8(10, 0xd000, 0);
    Func_0808a1b8(11, 0xb000, 10);
    Func_0808a138(10, 2);
    Func_0808a188(10, 0, 20);
    Func_02001450(20);
    Func_0808a138(8, 2);
    Func_0808a010(40);
    Func_0808a188(8, 0, 10);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_0808a188(8, 0, 20);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a100(8, 4);
    Func_0808a188(8, 0, 20);
    Func_0808a138(1, 2);
    Func_0808a1b8(1, 0xe000, 20);
    Func_0808a178(1, 0);
    Func_0808a1b8(0, 0x6000, 0);

    /* Decision 1. Note this one tests against 1, not 0. */
    skipped = 1;
    if (Func_0808a070(0, 0) == 1) {
        EVENT_COUNT(workspace) += 1;
        skipped = 0;
    }
    Func_0808a188(1, 0, 10);
    if (skipped != 0) {
        EVENT_COUNT(workspace) += 1;
    }

    Func_0808a1b8(2, 0x8000, 10);
    Func_0808a1f0(2, 258);
    Func_0808a010(60);
    Func_0808a1e8(8, 0x101, 0);
    Func_0808a1b8(8, 0x3000, 40);
    Func_0808a1b8(8, 0x5000, 20);
    Func_0808a1b8(8, 0x3000, 20);
    Func_0808a188(8, 0, 10);
    Func_0808a138(2, 1);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 10);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1e8(8, 0x107, 60);
    Func_0808a188(8, 0, 10);
    Func_0808a110(8, 4);
    Func_0808a178(8, 0);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(3, 0xa000, 0);

    /* Decision 2. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a110(8, 3);
        Func_0808a188(8, 0, 10);
        EVENT_COUNT(workspace) += 1;
    } else {
        Func_0808a010(20);
        Func_0808a110(8, 4);
        EVENT_COUNT(workspace) += 1;
        Func_0808a188(8, 0, 10);
    }

    Func_02001450(20);
    Func_0808a1b8(2, 0x2000, 0);
    Func_0808a1b8(3, 0xa000, 40);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 10);
    Func_0808a188(3, 0, 20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a188(8, 0, 40);
    Func_0808a138(8, 2);
    Func_0808a188(8, 0, 40);
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a0d0(2, 868, 472);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0200140c(40);
    Func_02001450(20);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a188(8, 0, 10);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a188(8, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a188(8, 0, 10);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a1f0(2, 258);
    Func_0808a1f0(3, 258);
    Func_0808a010(40);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a178(8, 0);
    Func_0808a130(1, 1);
    Func_0808a130(2, 1);
    Func_0808a138(3, 1);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0x6000, 0);
    Func_0808a1b8(3, 0xa000, 0);

    /* Decision 3. Both arms end at the same point; the zero arm bumps the
     * counter by two, the other by one and plays two extra steps. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a1f0(8, 258);
        Func_0808a010(40);
        Func_0808a188(8, 0, 10);
        EVENT_COUNT(WORKSPACE) += 2;
    } else {
        Func_0808a010(20);
        Func_0808a138(1, 2);
        EVENT_COUNT(WORKSPACE) += 1;
        Func_0808a188(1, 0, 10);
        Func_0808a1f0(8, 258);
        Func_0808a010(40);
        Func_0808a188(8, 0, 10);
    }

    Func_0808a1e8(3, 0x101, 40);
    Func_0808a1b8(3, 0xc000, 10);
    Func_0808a180(3, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a1f0(2, 258);
    Func_0808a1f0(3, 258);
    Func_0808a010(40);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(3, 0xa000, 10);
    Func_0808a110(2, 4);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a188(8, 0, 10);
    Func_02001450(10);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 20);
    Func_0808a100(0, 4);
    Func_0808a100(1, 4);
    Func_0808a100(2, 4);
    Func_0808a110(3, 4);
    Func_0808a1f0(8, 258);
    Func_0808a010(60);
    Func_0808a188(8, 0, 10);
    Func_0200140c(40);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_02001450(20);
    Func_0808a138(2, 2);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a188(3, 0, 10);
    Func_0808a1e8(8, 264, 60);
    Func_0808a188(8, 0, 10);
    Func_0808a110(3, 3);
    Func_0808a188(3, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(1, 0x101, 40);
    Func_0808a188(1, 0, 10);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a138(10, 2);
    Func_0808a110(10, 4);
    Func_0808a188(10, 0, 10);
    Func_0808a1e8(8, 0x105, 40);
    Func_0808a188(8, 0, 10);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a100(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a188(8, 0, 10);
    Func_0808a138(2, 1);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1e8(8, 256, 40);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a188(8, 0, 10);
    Func_0200140c(40);
    Func_02001450(20);
    Func_0808a138(1, 1);
    Func_0808a188(1, 0, 10);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a100(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a138(8, 1);
    Func_0808a188(8, 0, 10);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a010(60);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a130(2, 1);
    Func_0808a138(3, 1);
    Func_0808a010(20);
    Func_0808a138(8, 1);
    Func_0808a188(8, 0, 10);
    Func_0808a100(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a1e8(2, 0x105, 60);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a1b8(3, 0xa000, 10);
    Func_0808a188(3, 0, 10);
    Func_0808a110(2, 4);
    Func_0808a110(2, 4);
    Func_0808a1b8(2, 0x2000, 10);
    Func_0808a100(2, 4);
    Func_0808a188(0x2002, 0, 10);

    /* r0 and r1 are set before the branch over the fourth literal pool. */
    Func_0808a110(8, 4);

    Func_0808a188(8, 0, 10);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a138(3, 2);
    Func_0808a1b8(3, 0xc000, 10);
    Func_0808a188(3, 0, 10);
    Func_0808a110(8, 4);
    Func_0808a188(8, 0, 10);
    Func_0808a130(10, 2);
    Func_0808a188(10, 0, 10);
    Func_0808a110(11, 3);
    Func_0808a188(11, 0, 20);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a110(2, 4);
    Func_0808a100(2, 4);
    Func_0808a188(0x2002, 0, 10);
    Func_0808a110(1, 3);
    Func_0808a188(1, 0, 10);
    Func_0808a138(8, 1);
    Func_0808a178(8, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0x6000, 0);
    Func_0808a1b8(3, 0xa000, 0);

    /* Decision 4, the last one. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_02001450(10);
        Func_0808a100(1, 3);
        Func_0808a100(2, 3);
        Func_0808a110(3, 3);
        Func_0808a010(10);
        Func_0808a188(1, 0, 10);
        EVENT_COUNT(WORKSPACE) += 2;
    } else {
        Func_0808a010(10);
        Func_0808a138(1, 2);
        EVENT_COUNT(WORKSPACE) += 1;
        Func_0808a188(1, 0, 10);
        Func_02001450(10);
        Func_0808a100(1, 3);
        Func_0808a100(2, 3);
        Func_0808a110(3, 3);
        Func_0808a010(10);
        Func_0808a188(1, 0, 10);
    }

    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 40);
    Func_0808a1b8(8, 0x5000, 10);
    Func_0808a138(8, 1);
    Func_0808a010(10);
    Func_0808a138(10, 1);
    Func_0808a010(20);
    Func_0808a1b8(8, 0x3000, 10);
    Func_0808a138(8, 1);
    Func_0808a010(10);
    Func_0808a138(11, 1);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a188(8, 0, 10);
    Func_0808a100(10, 3);
    Func_0808a110(11, 3);
    Func_0808a090(10, 0x10000, 0x8000);
    Func_0808a090(11, 0x10000, 0x8000);
    Func_0808a0c8(10, 848, 540);
    Func_0808a0d0(11, 880, 540);
    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(11, 0, 0);
    Func_0808a110(8, 3);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);

    Func_0808a098(1, Data_02009b94);
    Func_0808a098(2, Data_02009b94);
    Func_0808a0b0(3, Data_02009b94);

    workspace = WORKSPACE;
    SETUP_WORD(workspace) = 16;
    REQUEST_WORD(workspace) = 521;

    Func_080770d0(0x12f);
    Func_080770c8(0x912);
    Func_0808a020();
}
