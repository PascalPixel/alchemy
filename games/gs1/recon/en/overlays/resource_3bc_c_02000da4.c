#include "types.h"

/*
 * resource_3bc owner at 0x02000da4, 1480 reviewed bytes (0x02000da4-0x0200136b).
 *
 * Stage setup and beat dispatch for the log-rolling stage overlay.  The body
 * runs in two halves.  The first half is unconditional scene construction: it
 * publishes the scene phase word, sets one progress flag, installs a per-frame
 * task and then lays out the stage's object records - 9..13 for the fixed
 * props, 15..17 for the three drifting obstacles, 18..27 for the two banks of
 * five paired actors, and 28..31 for the presentation extras.  Nine game flags
 * select between alternative layouts along the way, so the same 1480 bytes
 * build a different stage depending on how far the player has progressed.  The
 * second half reads the scene sub-state out of the shared record block and runs
 * one of five beats through the jump table that sits inline at 0x02001220
 * (five words, 0x02001220-0x02001233).  Every path falls into one shared
 * epilogue that returns 0.
 *
 * Extent note for the integrator.  The reviewed extent stops at 0x0200136b, but
 * the tail literal pool this body needs runs on to 0x0200137b: the words
 * 0x02001364 (Data_02000240), 0x02001368 (the jump-table base 0x02009220),
 * 0x0200136c (0x109), 0x02001370 (0xe6), 0x02001374 (0x0200a711) and
 * 0x02001378 (0x951), read back from
 * games/gs1/asm/overlays/resource_3bc_overlay.s, immediately before the next
 * owner at 0x0200137c.  Four of those six words land past the compared window,
 * so the candidate is about sixteen bytes longer than the reviewed extent.
 * Do not shorten the owner to absorb that.
 *
 * Constant evidence.  The reference's two literal pools hold exactly ten and
 * six words and both are in first-use order, which fixes the constant set:
 *   0x02001144  0x03001ebc   shared scene-work pointer cell
 *   0x02001148  0x02008659   per-frame task, image 0x02000658 with Thumb bit
 *   0x0200114c  0x00000362   flag 866
 *   0x02001150  0x00000367   flag 871
 *   0x02001154  0x00000361   flag 865
 *   0x02001158  0x0200804d   per-frame task, image 0x0200004c with Thumb bit
 *   0x0200115c  0x00000363   flag 867
 *   0x02001160  0x046a0000   x position, 16.16
 *   0x02001164  0x0200cbec   configuration block for the far initialiser
 *   0x02001168  0x00000369   flag 873
 * Flags 864, 872 and 880, and the value 324, are NOT pooled: each is built with
 * movs+lsls, which is this compiler's choice for a constant of the form
 * (0..255) << n, so they are spelled here as plain integers.  By the same test
 * 0xe6 = 230 would have fitted a bare movs and is pooled anyway, so that
 * operand cannot be an integer literal in the source; it is spelled as an
 * absolute-valued symbol, the convention this repository already uses in
 * games/gs1/src/overlays/state_update/dispatch_by_status_448.c.
 *
 * Loader relocation.  The image holds pre-relocation BL-shaped word pairs and
 * the resource loader rewrites each one from its own site, so the stored word
 * depends only on the runtime target and not on where the call sits.  The 104
 * call sites in this owner therefore collapse onto the twenty-eight runtime
 * targets declared below, one alias each.  Five of the legacy
 * disassembler names are ambiguous - Func_02005854, Func_020058ec,
 * Func_020059f0, Func_02005ade and Func_02005ca0 each name two different
 * runtime targets at two different sites - which is why the naming here is by
 * target rather than by site.
 *
 * Uncertainties.
 *  - Engine roles are named only in comments and only where this repository
 *    already binds the main-image address (GameFlag_IsSet 0x080770c0,
 *    GameFlag_Set 0x080770c8, Scene_GetRecord 0x0808a080, the task installer
 *    0x080000d0, ObjectDispatch_InitializeFar 0x08009098).  Every veneer keeps
 *    its raw pre-relocation alias.
 *  - Record offsets +8, +12, +16, +24, +28, +35 and +85 are named by position.
 *    +8 and +16 are 16.16 coordinates - they are shifted right by 20 to reach
 *    grid units - and +12 carries 16.16-shaped constants, so it is probably a
 *    third coordinate or a height.
 *  - Func_020058e4 (main 0x080770e0) returns a stored count; the fallback of 19
 *    when it reads back zero is the reference's own guard, kept as written.
 *  - Beat two re-runs the same three overlay routines beat one runs
 *    (restore_stage_actor_positions, 0x02002640 and run_scripted_transition)
 *    with different arguments.  That is the reference's own wiring, established
 *    by the shared runtime targets, not an assumption about the beats.
 *  - The two paired-actor loops each keep their constants in block-scoped
 *    temporaries.  That is forced by the reference and not a decoration: the
 *    cleared arm holds 0, 2 and 11 in r9, r8 and sl across its calls while the
 *    uncleared arm holds 2 in the low register r6 and 0 in r8, so the two arms
 *    cannot be reading one whole-function variable - this compiler gives a
 *    pseudo one hard register for the whole function.  The same reading fixes
 *    the third saved high register in the prologue.  The tail block shares one
 *    zero between the three record bytes and the byte at Data_02000240+498,
 *    which is what the reference's reuse of r6 there shows.
 *
 * Return types.  Func_02005616 (the task installer), Func_02004dc2 and
 * Func_020056e2 (setup_scene_descriptor) are declared here as returning s32
 * even though every caller discards the result.  That is not decoration: this
 * compiler sets r0 last of the argument registers for a value-returning call
 * and first for a void one, and the reference sets r0 last at every one of
 * those six sites - `movs r1,#3; movs r0,#39` at 0x020011ec, and `movs r1,#8;
 * movs r2,#6; movs r0,#0` at 0x02001246.  Declaring the three as void costs
 * nine halfwords, so the original declarations carried a return type.  The
 * same test leaves every other call here void.
 *
 * Residual: candidate 1496 bytes, reference 1480, differing_halfwords 14,
 * wrong_instructions 7, matched_prefix 253.  Eight of those halfwords are the
 * four tail-pool words past the reviewed extent, which have no reference
 * halfword to compare against.  The remaining six are the two paired-actor
 * loop preheaders: the reference interleaves its two high-register copies
 * between the low-register constants (0x02000fd8: r3=0, r9=r3, r2=2, r3=11,
 * r5=18, r8=r2, r6=33, sl=r3) while the candidate emits both copies before
 * the last two constants.  The constants themselves are created in the same
 * order and land in the same registers, so this is post-allocation
 * scheduling, not a source-order or lifetime difference.  Reordering the
 * initialisers and folding the loop variable into a comma for-init were both
 * tried and both made it worse, so it is left as a scheduling residual.
 */

#define StageSetup_BuildAndDispatch Func_02000da4

/* Cell holding the shared scene-work pointer; +448 is the scene phase word. */
extern u8 Data_03001ebc[];

/* Shared cross-overlay scene-record block: +450 is the scene sub-state, read as
 * a signed halfword, and +498 is a byte this owner clears on the way in. */
extern u8 Data_02000240[];

/* Configuration block handed to the far initialiser for record 30. */
extern u8 Data_0200cbec[];

/* Pooled operand that would have fitted a bare movs, so it reaches the call as
 * a relocated symbol rather than as an integer literal. */
extern u8 Value_000000e6;

/* One alias per runtime target.  The comment records the resolved target: a
 * main-image address for an engine import, an overlay address for local code. */
u8 *Func_02005800();   /* main 0808a080  Scene_GetRecord */
s32 Func_020057da();   /* main 080770c0  GameFlag_IsSet */
void Func_0200577c();  /* main 080770c8  GameFlag_Set */
s32 Func_020058e4();   /* main 080770e0  stored count */
s32 Func_02005616();  /* main 080000d0  per-frame task installer */
void Func_02005722();  /* main 080091c0  grid write, six arguments */
void Func_02005baa();  /* main 080091c8  grid write, six arguments */
s32 Func_020058ac();   /* main 080091a8 */
void Func_02005746();  /* main 080091e0 */
void Func_020058dc();  /* main 08009080  object mode setter */
void Func_020059d8();  /* main 08009098  ObjectDispatch_InitializeFar */
void Func_020059e8();  /* main 08009190 */
void Func_020058a6();  /* main 0808a100 */
void Func_02005c88();  /* main 0808a088 */
void Func_02005caa();  /* main 0808a158 */
void Func_02005e36();  /* main 0808a588 */
void Func_02005bd6();  /* main 0808a590 */
void Func_02005e3c();  /* main 080f9010  sound cue */
void Func_02005e38();  /* main, beats four and five only */
s32 Func_02004dc2();  /* overlay 0x02003bd0 */
s32 Func_020056e2();  /* overlay 0x02004494  setup_scene_descriptor */
void Func_0200584e();  /* overlay 0x0200457c  initialize_scene_control */
void Func_0200386e();  /* overlay 0x020025c8  restore_stage_actor_positions */
void Func_020038e2();  /* overlay 0x02002640 */
void Func_0200471c();  /* overlay 0x02003468  run_scripted_transition */
void Func_02002796();  /* overlay 0x02001474 */
void Func_02002d22();  /* overlay 0x020019a0 */
void Func_02003d6a();  /* overlay 0x02002a50  clear_saved_actor_positions */

/* Six-argument sites are spelled through this wrapper: it passes their
 * constants straight into the argument registers, where a direct call
 * precomputes a costly constant into a pseudo that the compiler then shares
 * with later uses in the block.  Measured: replacing Call6 with direct calls
 * costs 185 halfwords, so the wrapper is load-bearing.  The one- and
 * two-argument wrappers this file once carried were measured to be worth
 * nothing and have been removed in favour of plain calls. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Builds the log-rolling stage, then runs the beat named by the scene
 * sub-state.  Returns 0 on every path. */
s32 StageSetup_BuildAndDispatch(void)
{
    u8 *rec;
    s32 i;
    s32 val;
    s32 hit;
    s32 cnt;

    *(s32 *)(*(u8 **)Data_03001ebc + 448) = 256;
    Func_0200577c(324);
    Func_02005616(0x02008659, 3200);
    Call6(Func_02005722, 74, 60, 8, 6, 120, 60);

    Func_02005746(Func_02005800(9), 0);

    rec = Func_02005800(10);
    Func_02005746(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x200000;

    rec = Func_02005800(11);
    Func_02005746(rec, 0);
    rec[85] = 0;
    *(s32 *)(rec + 12) = 0x40000;

    if (Func_020057da(866) != 0) {
        Func_020058a6(9, 5);
        rec = Func_02005800(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec = Func_02005800(11);
        *(s32 *)(rec + 12) = 0x200000;
        Call6(Func_02005722, 15, 12, 1, 1, 13, 12);
    } else {
        rec = Func_02005800(9);
        *(s32 *)(rec + 24) = 0x18000;
        *(s32 *)(rec + 28) = 0x18000;
        if (Func_020057da(871) != 0) {
            Call6(Func_02005722, 0, 24, 1, 1, 9, 12);
        } else {
            Call6(Func_02005722, 0, 25, 1, 1, 9, 12);
        }
    }

    if (Func_020057da(872) != 0) {
        Call6(Func_02005722, 15, 12, 1, 1, 13, 12);
        Call6(Func_02005722, 1, 25, 1, 1, 9, 12);
        rec = Func_02005800(12);
        Func_02005746(rec, 0);
        rec[85] = 0;
        *(s32 *)(rec + 12) = 0x20000;
        rec[35] = 2;
        rec = Func_02005800(10);
        *(s32 *)(rec + 12) = 0x40000;
        rec[35] = 2;
        rec = Func_02005800(11);
        *(s32 *)(rec + 12) = 0x200000;
    }

    cnt = Func_020058e4(880);
    if (cnt == 0) {
        cnt = 19;
    }
    rec = Func_02005800(13);
    *(s32 *)(rec + 8) = (cnt << 20) + 0x80000;
    rec[85] = 0;
    rec[35] = 2;
    Call6(Func_02005722, 18, 10, 3, 1, 18, 11);
    Call6(Func_02005722, 17, 11, 1, 1, cnt, 11);

    /* The three drifting obstacles: any one still at rest and clear of the
     * grid is planted and drawn twice, once on its own row and once 52 rows
     * further down. */
    for (i = 15; i <= 17; i++) {
        rec = Func_02005800(i);
        hit = Func_020058ac(0, *(s32 *)(rec + 8), *(s32 *)(rec + 16));
        if (*(s32 *)(rec + 12) == 0 && hit == 0) {
            rec[35] = 2;
            rec[85] = hit;
            Call6(Func_02005722, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20,
                          *(s32 *)(rec + 16) >> 20);
            Call6(Func_02005722, 83, 13, 1, 1, *(s32 *)(rec + 8) >> 20,
                          (*(s32 *)(rec + 16) >> 20) + 52);
        }
    }

    /* Two banks of five paired actors, 18..22 and 23..27.  The cleared arm
     * also puts each pair on the grid; the uncleared arm only prepares the
     * records and installs the second per-frame task. */
    if (Func_020057da(865) != 0) {
        s32 state;
        s32 mode;
        s32 row;

        state = 0;
        mode = 2;
        row = 11;
        val = 33;
        for (i = 18; i <= 22; i++) {
            rec = Func_02005800(i);
            rec[35] = mode;
            Func_020058dc(rec, 2);
            rec = Func_02005800(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
            Func_020058dc(rec, 10);
            Call6(Func_02005722, 74, 12, 1, 1, val, row);
            val += 2;
        }
        Func_020058a6(28, 10);
        Func_02005bd6(28);
    } else {
        s32 state;
        s32 mode;

        state = 0;
        mode = 2;
        for (i = 18; i <= 22; i++) {
            rec = Func_02005800(i);
            rec[35] = mode;
            rec = Func_02005800(i + 5);
            rec[35] = mode;
            rec[85] = state;
            *(s32 *)(rec + 12) = 0x200000;
        }
        Func_02005616(0x0200804d, 3200);
    }

    if (Func_020057da(864) != 0) {
        Func_020058a6(29, 4);
        Call6(Func_02005722, 47, 61, 1, 4, 49, 61);
    }

    if (Func_020057da(867) != 0) {
        Func_020059e8(1);
        rec = Func_02005800(30);
        rec[85] = 0;
        *(s32 *)(rec + 8) = 0x046a0000;
        *(s32 *)(rec + 16) = 0xb80000;
        Func_02005746(rec, 0);
        Func_020058dc(rec, 3);
        Func_020059d8(rec, Data_0200cbec);
    } else {
        Func_020059e8(2);
    }

    if (Func_020057da(873) != 0) {
        rec = Func_02005800(31);
        Func_020058dc(rec, 8);
        rec[35] = 2;
        Call6(Func_02005722, 86, 10, 1, 2, 84, 10);
        Call6(Func_02005722, 86, 9, 1, 1, 84, 12);
    } else {
        rec = Func_02005800(31);
        Call6(Func_02005722, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 9);
        Call6(Func_02005722, 85, 9, 1, 4, *(s32 *)(rec + 8) >> 20, 61);
    }

    {
        s32 state;
        s32 mode;

        state = 0;
        mode = 2;
        rec = Func_02005800(9);
        rec[85] = state;
        rec[35] = mode;
        rec = Func_02005800(10);
        rec[85] = state;
        rec[35] = mode;
        rec = Func_02005800(11);
        rec[85] = state;
        rec[35] = mode;
        Func_020058a6(8, 9);
        Data_02000240[498] = state;
    }
    Func_02004dc2(39, 3);
    Func_02004dc2(40, 17);
    Func_02005caa(8, 2);

    switch (*(s16 *)(Data_02000240 + 450)) {
    case 1:
        Func_020056e2(0, 8, 6, 0x5e80000, 0xc00000, 39, 40);
        Call6(Func_02005baa, 127, 0, 1, 2, 5, 2);
        Func_02005c88(32);
        Func_02005c88(33);
        Func_02005c88(34);
        Func_02005c88(35);
        Func_02005c88(36);
        Func_02005c88(37);
        Func_02005c88(38);
        if (Func_020057da(265) == 0) {
            Func_02005e3c(17);
            Func_020038e2(0);
            Func_0200386e();
            Func_02005e36(1, 0);
            Func_0200471c(3);
        }
        Func_02005e36(1, 0);
        Func_02005e36(2, 0);
        Func_02005e36(3, 0);
        Func_0200584e((s32)&Value_000000e6);
        break;

    case 2:
        Func_02005616(0x0200a711, 3200);
        Func_02005c88(39);
        Func_02005c88(40);
        if (Func_020057da(265) != 0) {
            break;
        }
        Func_0200386e();
        Func_020038e2(1);
        Func_0200471c(0);
        break;

    case 3:
        if (Func_020057da(265) != 0) {
            break;
        }
        Func_02002d22(32);
        Func_02003d6a();
        break;

    case 4:
        Func_02002796(1);
        Func_02005e38(4);
        Func_0200577c(2384);
        Func_0200577c(2385);
        break;

    case 5:
        Func_02002796(-1);
        Func_02005e38(5);
        Func_0200577c(2384);
        break;
    }
    return 0;
}
