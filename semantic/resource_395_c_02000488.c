typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 395 main cutscene at 0x02000488 (3048 bytes,
 * 0x02000488-0x0200106f).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x02000488 and the single
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02001050.  The
 * popped return address lands in r0, so nothing is returned and the owner is
 * `void`.  There is no early exit; every branch rejoins the straight line.
 * The next owner (Func_02001070) begins at 0x02001070, which confirms the end
 * boundary.
 *
 * POOL MAP, derived from a CONTROL-FLOW WALK, which is the only method immune
 * to the two traps here (a pool word that decodes as a live-register write,
 * and a pool that ends mid-row).  Walking from the prologue and following
 * every branch gives four pools, three of them hopped by a lone unconditional
 * `b.n` in the middle of a conditional:
 *   0x0200086c-0x020008a3  56 bytes, hopped by `b.n 0x02000926` at 0x0200086a.
 *                          Note 0x020008a4 is the `bne` target from
 *                          0x02000858, so the pool ends exactly there — the
 *                          "run to the next label" heuristic happens to agree
 *                          here, but see the third pool.
 *   0x02000a98-0x02000abf  40 bytes, hopped by `b.n 0x02000ad8` at 0x02000a96.
 *   0x02000ec0-0x02000eeb  44 bytes, hopped by `b.n 0x02000eec` at 0x02000ebe.
 *                          THIS is the case that requires the walk: r0=10 and
 *                          r1=1 are set at 0x02000eba/0x02000ebc, BEFORE the
 *                          hop, and consumed by the `bl` at 0x02000eec after
 *                          it.  A simulator that resets at a pool boundary
 *                          drops both arguments; the call is written as one
 *                          Func_020012f4(10, 1) below.
 *   0x02001056-0x0200106f  a two-byte alignment `nop` plus the trailing
 *                          six-word pool, after the epilogue.
 * 166 pool/alignment bytes, so 2,882 of the row's 3,048 bytes are executable.
 *
 * The 0x02008000 link base is confirmed four more times here, each pool word
 * being one of this overlay's own prologues plus the Thumb bit, and each
 * naming a routine this reconstruction has already converted:
 *   0x020092b5 = Func_020012b4 + 1  (installed and removed as a per-frame task)
 *   0x020095a1 = Func_020015a0 + 1  (installed twice, removed twice — the
 *                                    periodic burst emitter)
 * and the emitter's own scratch words are driven from here, which
 * cross-validates both files at no cost:
 *   0x02009dcc  the emitter's frame counter, cleared before each install;
 *   0x02009dc0  the emitter's three-word 16.16 origin, written before each
 *               install (Func_020015a0 reads exactly these);
 *   0x02009dd4  the step counter Func_020012b4 advances, cleared before that
 *               task is installed and then spun on until it reaches 24.
 * All four sit above the image end (0x1dbc), i.e. in the loader's working RAM
 * behind the loaded overlay, exactly as the sibling
 * semantic/overlays/resource_395_c_02001218.c records.
 * 0x02009ab8 / 0x02009af4 / 0x02009b30 / 0x02009b6c are EVEN in-image words
 * (file offsets 0x1ab8 / 0x1af4 / 0x1b30 / 0x1b6c) passed as the second
 * argument of Func_0808a098 / Func_0808a0b0 — the established motion-script
 * shape.  The parity test (odd = Thumb entry, even = data) classifies all six
 * pointer words with no further work.
 * 0x03001ebc is RAM below the image and is a pointer CELL, not the workspace:
 * `ldr r5,[pc] / ldr r2,[r5,#0]` loads the pointer, so the workspace is
 * *(u8 **)0x03001ebc.
 *
 * All 319 call sites were resolved with tools/overlay_call_targets.ts
 * (--annotate for the listing, site->target from --json, never from the
 * summary histogram); overlay_show.ts's own `bl` annotations are wrong on
 * every overlay because an overlay `bl` stores target_offset - 2.  The tool
 * reports sites=319 distinct_targets=34 veneer=269 prologue=50 unknown=0.
 * Three of the targets are this overlay's own prologues — Func_020012f4 (41
 * sites), Func_020018b8 (8) and Func_0200172c (1) — not veneers.
 *
 * PLACED MULTISET, checked PER TARGET against that report, never by total.
 * (A 137=137 total once hid five mutually mis-assigned targets on another
 * overlay, so the total is not the proof.)
 *   Func_020012f4 41  Func_0200172c  1  Func_020018b8  8  Func_080000c0  8
 *   Func_080000d0  3  Func_080000d8  3  Func_08015040  1  Func_080770c0  1
 *   Func_080770c8  1  Func_0808a010 51  Func_0808a070  3  Func_0808a080  3
 *   Func_0808a090  3  Func_0808a098  5  Func_0808a0a0  1  Func_0808a0b0  1
 *   Func_0808a0d0  1  Func_0808a0f0  3  Func_0808a100 16  Func_0808a110  9
 *   Func_0808a130  7  Func_0808a138  6  Func_0808a170  4  Func_0808a178  3
 *   Func_0808a180 22  Func_0808a188 18  Func_0808a1b8 46  Func_0808a1e8 14
 *   Func_0808a208  1  Func_0808a210  7  Func_0808a218  7  Func_0808a330  3
 *   Func_0808a348 11  Func_080f9010  7
 * = 319.  The inventory's calls=295 is the documented undercount.
 *
 * No shared call site is duplicated.  Every branch here is a plain diamond
 * whose arms rejoin the straight line (0x02000926 from the `b.n` at
 * 0x0200086a and by fall-through; 0x02000ad8 likewise), so the joins are
 * written as ordinary `if`/`else` and the following code appears once.  There
 * is no arm that falls into another arm's `bl`, and no `bl` into this owner's
 * own body.
 *
 * THE SKIP-BEAT COUNTER, recognised by its constants rather than by
 * asymmetry: `movs r3,#236 / lsls r3,r3,#1` is the u16 at workspace + 472,
 * off the 0x03001ebc state pointer.  It is bumped four times here — by 2 once
 * and by 1 three times — and three of those sit on the *absent* side of a
 * conditional, which is what makes the branches read as symmetric scene
 * variants rather than as missing code.
 *
 * Behaviour: the full scene.  Set up four presentation slots (0, 1, 2, and
 * optionally 3 when Func_080770c0(3) reports the fourth participant present),
 * run the approach with the Func_020012b4 task, spin until its step counter
 * reaches 24, flash four times, then play the dialogue through
 * Func_020012f4 gestures on actors 10 and 11 interleaved with camera moves;
 * twice, install the Func_020015a0 burst emitter over a positioned origin for
 * a hundred beats and take it down again; then close on the last lines and
 * hand over to Func_0200172c(184, 185).
 *
 * Uncertainties, recorded rather than smoothed over:
 *  - r6 is the Func_080770c0(3) result and is used purely as a boolean gate at
 *    three points (0x0200050c, 0x020008e2, 0x02000fdc).  Whether it is a count
 *    or a pointer is not established; it is only ever compared with 0.
 *  - r5 is a long-lived register REASSIGNED six times across the owner
 *    (0x02009dd4 pointer, then the 0..3 flash counter, then the 0x03001ebc
 *    cell, then the 0x020095a1 task pointer twice, then the 0x02009b6c script
 *    pointer).  Tracked per use below, never as one variable — the HANDOVER
 *    records this as a trap that silently corrupts a simulator's output.
 *  - the spin at 0x02000602 reads the counter another task writes, so it is
 *    read through a volatile view.
 *  - the two `if (record != 0)` guards around Func_0808a0f0 load the record's
 *    +8 and +16 words; no layout beyond those two offsets is asserted.
 *  - the argument roles of the 0x0808axxx imports are as far as this overlay's
 *    converted siblings establish them, and no meaning is asserted for the
 *    individual request ids.  Old-style declarations are used because one
 *    import can take different argument counts at different sites.
 */

void Func_020012f4();           /* this overlay: gesture dispatcher (actor, gesture) */
void Func_0200172c();           /* this overlay: scene hand-off */
void Func_020018b8();           /* this overlay: screen-effect selector */
void Func_080000c0();           /* frame wait */
void Func_080000d0();           /* install a per-frame task (callback, rank) */
void Func_080000d8();           /* remove a per-frame task (callback) */
void Func_08015040();           /* scene resource request */
s32 Func_080770c0();            /* query participant presence */
void Func_080770c8();           /* scene completion flag */
void Func_0808a010();           /* scene beat / wait */
s32 Func_0808a070();            /* scene variant query */
u8 *Func_0808a080();            /* scene-record accessor */
void Func_0808a090();           /* 16.16 pair setter */
void Func_0808a098();           /* start a scripted motion (slot, script) */
void Func_0808a0a0();           /* select the focus slot */
void Func_0808a0b0();           /* start a scripted motion, camera variant */
void Func_0808a0d0();           /* scene bounds */
void Func_0808a0f0();           /* place a slot at another record's position */
void Func_0808a100();           /* per-slot scene request */
void Func_0808a110();           /* per-slot scene request, second family */
void Func_0808a130();           /* per-slot presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dismiss a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a188();           /* as Func_0808a180, with a duration */
void Func_0808a1b8();           /* face the slot along a heading over n frames */
void Func_0808a1e8();           /* move the slot to a waypoint over n frames */
void Func_0808a208();           /* camera distance pair (16.16) */
void Func_0808a210();           /* camera target (x, y, z, mode) */
void Func_0808a218();           /* commit the camera move */
void Func_0808a330();           /* scene amount (16.16) with a mode */
void Func_0808a348();           /* scene-effect request */
void Func_080f9010();           /* play a sound cue */

/* Working RAM behind the loaded overlay; see the pool notes above. */
#define EMITTER_COUNTER  (*(s32 *)0x02009dcc)
#define EMITTER_ORIGIN   ((s32 *)0x02009dc0)
#define APPROACH_STEP    (*(volatile s32 *)0x02009dd4)

/* The skip-beat counter: the u16 at workspace + 472. */
#define SKIP_BEAT        (*(u16 *)(*(u8 **)0x03001ebc + 472))

void Func_02000488(void)
{
    s32 fourth;
    u8 *record;
    s32 i;

    fourth = Func_080770c0(3);

    Func_0808a0d0(0, 328, 212);                 /* 164 << 1 */
    Func_0808a1b8(0, 0xc000, 20);               /* 192 << 8 */
    Func_080f9010(17);
    Func_08015040(0x14ed, 1);

    Func_0808a090(1, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_0808a090(2, 0x10000, 0x8000);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a098(1, (void *)0x02009ab8);
    Func_0808a098(2, (void *)0x02009af4);

    if (fourth != 0) {
        Func_0808a090(3, 0x10000, 0x8000);
        record = Func_0808a080(0);
        if (record != 0) {
            Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_0808a098(3, (void *)0x02009b30);
    }

    Func_0808a0a0(2);
    Func_0808a010(40);
    Func_020018b8(0);
    Func_0808a348(32);
    Func_080000c0(40);

    APPROACH_STEP = 0;
    /* Func_020012b4 + 1 under the proven 0x02008000 base. */
    Func_080000d0((void *)0x020092b5, 3200);    /* 200 << 4 */
    Func_0808a010(40);

    Func_0808a1b8(1, 0x6000, 20);               /* 192 << 7 */
    Func_0808a208(0x00033333, 0x00006666);
    Func_0808a210(0x01000000, -1, 0x00fe0000, 1);   /* 128 << 17, 254 << 16 */
    Func_0808a218();
    Func_080f9010(246);
    Func_0808a010(40);

    Func_0808a1b8(2, 0x2000, 20);               /* 128 << 6 */
    Func_0808a210(0x019d0000, -1, 0x01050000, 1);
    Func_0808a218();
    Func_080f9010(246);
    Func_0808a010(40);

    Func_0808a1b8(0, 0x4000, 0);                /* 128 << 7 */
    Func_0808a1b8(3, 0x4000, 20);
    Func_0808a210(0x01460000, -1, 0x01800000, 1);   /* 163 << 17, 192 << 17 */
    Func_0808a218();
    Func_080f9010(246);

    while (APPROACH_STEP != 24) {
        Func_080000c0(1);
    }

    Func_080000d8((void *)0x020092b5);
    Func_080000c0(10);

    /* Four flashes; the counter runs 0..3 inclusive (`cmp r5,#3 / bls`). */
    for (i = 0; i <= 3; i++) {
        Func_020018b8(0);
        Func_0808a348(6);
        Func_080000c0(6);
        Func_020018b8(1);
        Func_0808a348(6);
        Func_080000c0(6);
    }

    Func_020018b8(0);
    Func_0808a348(40);
    Func_080000c0(80);

    Func_0808a210(0x01480000, 0x00080000, 0x00d40000, 1);   /* 164<<17, 128<<12, 212<<16 */
    Func_0808a218();
    Func_0808a010(40);
    Func_020012f4(10, 1);
    Func_0808a010(40);
    Func_080f9010(7);
    Func_0808a170(0x14ee);
    Func_0808a180(8, 0);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);

    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_020012f4(10, 3);
    Func_0808a010(40);
    Func_020012f4(10, 1);
    Func_0808a010(20);
    Func_0808a180(8, 0);

    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 0);
    Func_0808a1e8(3, 0x105, 0);
    Func_0808a1e8(2, 0x105, 40);

    Func_0808a210(0x00ea0000, 0, 0x00e80000, 1);    /* 234 << 16, 232 << 16 */
    Func_0808a218();
    Func_0808a010(40);
    Func_020012f4(11, 1);
    Func_0808a010(40);
    Func_020012f4(11, 3);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 20);
    Func_020012f4(11, 2);
    Func_0808a010(10);

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 0);
    Func_0808a1b8(3, 0x4000, 20);

    Func_020012f4(11, 3);
    Func_0808a010(20);
    Func_020012f4(11, 2);
    Func_0808a010(20);
    Func_020012f4(11, 3);
    Func_0808a188(0x4009, 0, 10);
    Func_020012f4(10, 0);
    Func_0808a010(20);
    Func_0808a180(0x8008, 0);
    Func_020012f4(10, 1);
    Func_0808a010(20);
    Func_0808a178(0x8008, 0);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xe000, 0);                /* 224 << 8 */
    Func_0808a1b8(2, 0xa000, 0);                /* 160 << 8 */
    Func_0808a1b8(3, 0xc000, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a180(0x4009, 0);
        Func_0808a180(0x8008, 0);
    } else {
        SKIP_BEAT = (u16)(SKIP_BEAT + 2);

        Func_0808a1e8(3, 0x103, 0);
        Func_0808a1e8(1, 0x103, 0);
        Func_0808a1e8(2, 0x103, 40);
        Func_0808a100(1, 4);
        Func_0808a180(1, 0);

        if (fourth != 0) {
            Func_0808a138(3, 2);
            Func_0808a180(3, 0);
        } else {
            SKIP_BEAT = (u16)(SKIP_BEAT + 1);
        }

        Func_0808a110(2, 3);
        Func_0808a180(2, 0);
        Func_0808a180(0x4009, 0);
        Func_0808a180(0x8008, 0);
    }

    /* 0x02000926: the two arms rejoin here. */
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);

    Func_0808a210(0x01480000, 0x00080000, 0x00d40000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_020012f4(10, 0);
    Func_0808a010(20);
    Func_020018b8(0);
    Func_0808a348(1);
    Func_080000c0(1);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(40);
    Func_0808a010(60);

    /* First burst: position the emitter's origin, then install it. */
    EMITTER_COUNTER = 0;
    EMITTER_ORIGIN[0] = 0x01480000;             /* 164 << 17 */
    EMITTER_ORIGIN[1] = 0x00300000;             /* 192 << 14 */
    EMITTER_ORIGIN[2] = 0x00cd0000;             /* 205 << 16 */
    /* Func_020015a0 + 1 under the proven 0x02008000 base. */
    Func_080000d0((void *)0x020095a1, 3200);
    Func_0808a010(100);
    Func_080000d8((void *)0x020095a1);

    Func_0808a330(0x7fff, 0);
    Func_0808a348(60);
    Func_0808a010(100);
    Func_020018b8(0);
    Func_0808a348(20);
    Func_0808a010(40);
    Func_020012f4(10, 1);
    Func_0808a010(10);
    Func_0808a170(0x14fb);
    Func_0808a180(0x8008, 0);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);

    Func_0808a210(0x00ea0000, 0, 0x00e80000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a180(0x4009, 0);
    Func_0808a188(0x8008, 0, 10);
    Func_0808a138(1, 2);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0xe000, 10);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a1e8(1, 258, 40);              /* 129 << 1 */
    } else {
        Func_0808a110(1, 4);
        SKIP_BEAT = (u16)(SKIP_BEAT + 1);
    }

    /* 0x02000ad8: the two arms rejoin here. */
    Func_0808a180(1, 0);
    Func_020012f4(10, 4);
    Func_0808a010(20);
    Func_0808a170(0x1501);
    Func_0808a180(0x8008, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a180(0x8008, 0);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);

    Func_020012f4(10, 4);
    Func_0808a010(20);
    Func_0808a188(0x8008, 0, 20);
    Func_020012f4(11, 0);
    Func_0808a188(0x4009, 0, 20);
    Func_020012f4(11, 3);
    Func_0808a010(40);
    Func_020012f4(11, 1);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 20);
    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_0808a180(0x8008, 0);

    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a1e8(3, 258, 0);
    Func_0808a1e8(2, 258, 80);

    Func_020012f4(11, 5);
    Func_0808a010(60);
    Func_020012f4(11, 3);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 20);
    Func_020012f4(10, 5);
    Func_0808a010(40);
    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_0808a188(0x4008, 0, 20);
    Func_0808a138(1, 2);
    Func_0808a1b8(1, 0x8000, 10);               /* 128 << 8 */
    Func_0808a180(1, 0);
    Func_0808a1b8(2, 0x8000, 20);
    Func_0808a180(0x8002, 0);
    Func_020012f4(11, 4);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 20);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(0, 0x4000, 10);
    Func_0808a188(1, 0, 20);
    Func_020012f4(10, 1);
    Func_0808a188(0x8008, 0, 10);
    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_020012f4(11, 3);
    Func_0808a010(40);
    Func_020012f4(11, 0);
    Func_0808a010(20);
    Func_020018b8(0);
    Func_0808a348(1);
    Func_080000c0(1);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(40);
    Func_0808a010(60);

    /* Second burst, at a different origin. */
    EMITTER_COUNTER = 0;
    EMITTER_ORIGIN[0] = 0x00880000;             /* 136 << 16 */
    EMITTER_ORIGIN[1] = 0x00140000;             /* 160 << 13 */
    EMITTER_ORIGIN[2] = 0x01020000;             /* 129 << 17 */
    Func_080000d0((void *)0x020095a1, 3200);
    Func_0808a010(100);

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 40);
    Func_0808a130(2, 1);
    Func_0808a1e8(2, 256, 20);                  /* 128 << 1 */
    Func_0808a188(0x8002, 0, 10);
    Func_0808a138(0, 2);
    Func_0808a1b8(0, 0x2000, 10);
    Func_0808a110(0, 3);
    Func_020012f4(10, 4);
    Func_0808a010(20);
    Func_0808a180(0x8008, 0);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a188(0x8002, 0, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 20);
    Func_0808a188(0x8008, 0, 10);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);

    Func_0808a010(10);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(2, 0x4000, 120);
    Func_080000d8((void *)0x020095a1);
    Func_0808a010(60);
    Func_020018b8(0);
    Func_0808a348(40);
    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_0808a188(0x8008, 0, 20);
    Func_020012f4(11, 3);
    Func_0808a180(0x4009, 0);
    Func_0808a180(0x8008, 0);
    Func_020012f4(11, 4);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 10);

    Func_0808a130(0, 2);
    Func_0808a130(1, 2);
    Func_0808a130(3, 2);
    Func_0808a138(2, 2);

    /* r0=10, r1=1 are set before the `b.n` that hops the third pool. */
    Func_020012f4(10, 1);

    Func_0808a178(0x8008, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0xa000, 0);

    if (Func_0808a070(0, 0) == 1) {
        SKIP_BEAT = (u16)(SKIP_BEAT + 1);
    }

    Func_0808a010(10);
    Func_020012f4(10, 2);
    Func_0808a010(20);
    Func_020012f4(11, 3);
    Func_0808a010(40);
    Func_020012f4(10, 1);
    Func_0808a010(20);
    Func_0808a188(0x8008, 0, 10);

    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(2, 0x8000, 10);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);

    Func_080f9010(17);

    Func_0808a098(1, (void *)0x02009b6c);
    if (fourth != 0) {
        Func_0808a098(3, (void *)0x02009b6c);
    }
    Func_0808a0b0(2, (void *)0x02009b6c);

    Func_020012f4(10, 4);
    Func_020012f4(10, 4);
    Func_0808a010(20);
    Func_0808a170(0x1519);
    Func_0808a180(0x8008, 0);
    Func_020012f4(11, 4);
    Func_020012f4(11, 4);
    Func_0808a010(20);
    Func_0808a188(0x4009, 0, 10);
    Func_0808a110(0, 3);
    Func_080770c8(0x845);
    Func_080f9010(1);
    Func_0200172c(184, 185);
}
