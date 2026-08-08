#include "types.h"

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
 * All 319 call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
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

s32 Func_02001e0e();
void Func_02001e7c();
void Func_02001ee8();
void Func_02001f5e();
void Func_02001e16();
void Func_02001e8a();
u8 *Func_02001e88();
void Func_02001ece();
u8 *Func_02001e9c();
void Func_02001ee2();
void Func_02001ec2();
void Func_02001eca();
void Func_02001ed4();
u8 *Func_02001ed2();
void Func_02001f18();
void Func_02001ef8();
void Func_02001f06();
void Func_02001e02();
void Func_02001fd8();
void Func_02001e5e();
void Func_02001e76();
void Func_02001efc();
void Func_02001fb8();
void Func_02001ff2();
void Func_02001ffe();
void Func_0200204c();
void Func_02001f32();
void Func_02001fee();
void Func_0200201c();
void Func_02002028();
void Func_02002076();
void Func_02001f5c();
void Func_02002018();
void Func_02002024();
void Func_02002056();
void Func_02002062();
void Func_020020b0();
void Func_02001f14();
void Func_02001f30();
void Func_02001f26();
void Func_02001ede();
void Func_020020b4();
void Func_02001f3a();
void Func_02001ef0();
void Func_020020c6();
void Func_02001f4e();
void Func_02001f08();
void Func_020020de();
void Func_02001f64();
void Func_020020ce();
void Func_020020da();
void Func_02002008();
void Func_02001974();
void Func_02002016();
void Func_0200213c();
void Func_020020b2();
void Func_020020ca();
void Func_020020aa();
void Func_020020ba();
void Func_02002106();
void Func_02002112();
void Func_0200211e();
void Func_0200212a();
void Func_020019e6();
void Func_02002088();
void Func_020019f4();
void Func_02002096();
void Func_02001a02();
void Func_020020a4();
void Func_0200214c();
void Func_02002176();
void Func_02002180();
void Func_0200218a();
void Func_02002194();
void Func_020021b4();
void Func_020021c0();
void Func_020020ee();
void Func_02001a5a();
void Func_020020fc();
void Func_02001a68();
void Func_0200210a();
void Func_020021bc();
void Func_02001a80();
void Func_02002122();
void Func_020021de();
void Func_020021ea();
void Func_020021f6();
void Func_02002202();
void Func_02001abe();
void Func_02002160();
void Func_02001acc();
void Func_0200216e();
void Func_02001ada();
void Func_02002228();
void Func_02001aec();
void Func_0200218e();
void Func_02002236();
void Func_02001b02();
void Func_020021a4();
void Func_02002244();
void Func_02002268();
void Func_02002274();
void Func_02002280();
void Func_0200228c();
s32 Func_020021fc();
void Func_02002290();
void Func_02002298();
void Func_0200230c();
void Func_02002316();
void Func_02002320();
void Func_020022c8();
void Func_02002310();
void Func_020022fc();
void Func_02002324();
void Func_02002304();
void Func_02002344();
void Func_0200234c();
void Func_02002354();
void Func_0200231c();
void Func_0200232c();
void Func_0200233c();
void Func_020023b6();
void Func_020023c2();
void Func_020022f0();
void Func_02001c5c();
void Func_020022fe();
void Func_0200222c();
void Func_02002402();
void Func_02002288();
void Func_02002400();
void Func_02002416();
void Func_020022ca();
void Func_02002350();
void Func_020022de();
void Func_02002446();
void Func_0200245c();
void Func_0200236a();
void Func_0200246e();
void Func_0200237c();
void Func_02001ce8();
void Func_0200238a();
void Func_02002420();
void Func_02002438();
void Func_02002408();
void Func_02002410();
void Func_02002498();
void Func_020024a4();
void Func_020023d2();
void Func_0200247a();
void Func_0200248c();
void Func_0200246c();
void Func_020024a8();
void Func_020024b4();
s32 Func_0200242c();
void Func_020024e4();
void Func_020024be();
void Func_0200250e();
void Func_02001dda();
void Func_0200247c();
void Func_02002512();
void Func_0200252a();
void Func_02002546();
void Func_02002552();
void Func_0200254a();
void Func_0200251a();
void Func_02002522();
void Func_02002532();
void Func_02001e36();
void Func_020024d8();
void Func_0200258a();
void Func_02001e4e();
void Func_0200259c();
void Func_02001e60();
void Func_02002502();
void Func_02001e6e();
void Func_02002510();
void Func_020025c2();
void Func_02001e86();
void Func_02002528();
void Func_020025d0();
void Func_020025fc();
void Func_02002608();
void Func_02002614();
void Func_02002620();
void Func_02001ecc();
void Func_0200256e();
void Func_02001eda();
void Func_0200257c();
void Func_0200262e();
void Func_02001ef2();
void Func_02002594();
void Func_02001f00();
void Func_020025a2();
void Func_02002654();
void Func_02002634();
void Func_02002670();
void Func_02002668();
void Func_02002684();
void Func_0200267c();
void Func_02001f48();
void Func_020025ea();
void Func_0200269c();
void Func_020026b0();
void Func_020026bc();
void Func_020026be();
void Func_02001f82();
void Func_020026d0();
void Func_02001f94();
void Func_02002636();
void Func_02001fa2();
void Func_02002644();
void Func_02001fb0();
void Func_02002652();
void Func_02002580();
void Func_02002756();
void Func_020025dc();
void Func_02002754();
void Func_0200276a();
void Func_02002678();
void Func_0200261e();
void Func_020026a4();
void Func_02002760();
void Func_0200276c();
void Func_02002778();
void Func_02002784();
void Func_020027a8();
void Func_0200279a();
void Func_0200277a();
void Func_020027b6();
void Func_02002776();
void Func_0200207a();
void Func_0200271c();
void Func_020027c4();
void Func_020027ee();
void Func_020027ea();
void Func_020027ec();
void Func_02002800();
void Func_0200280c();
void Func_02002818();
void Func_0200281a();
void Func_020027da();
void Func_020027e2();
void Func_020027fa();
void Func_02002798();
void Func_02002854();
void Func_02002860();
void Func_0200286c();
void Func_02002878();
void Func_020027d4();
void Func_02002702();
void Func_020028d8();
void Func_020028a0();
void Func_02002164();
void Func_020028a8();
void Func_020028b0();
void Func_0200217c();
void Func_0200281e();
void Func_020028d0();
void Func_020028b8();
void Func_020028c8();
void Func_020021e2();
void Func_0200291e();
void Func_02002942();
void Func_0200294e();
void Func_0200295a();
void Func_02002966();
s32 Func_020028d6();
void Func_020028e6();
void Func_02002252();
void Func_020028f4();
void Func_02002260();
void Func_02002902();
void Func_020029b4();
void Func_020029c8();
void Func_020029d2();
void Func_020029de();
void Func_020029ea();
void Func_020029a2();
void Func_020029aa();
void Func_020029b2();
void Func_020029c2();
void Func_02002a80();
void Func_0200299a();
void Func_020029a6();
void Func_020029be();
void Func_020022ea();
void Func_020022f2();
void Func_02002994();
void Func_02002a2a();
void Func_02002a42();
void Func_0200230e();
void Func_020029b8();
void Func_02002a6a();
void Func_02002a32();
void Func_02002af6();
                                /* this overlay: gesture dispatcher (actor, gesture) */
                                /* this overlay: scene hand-off */
                                /* this overlay: screen-effect selector */
                                /* frame wait */
                                /* install a per-frame task (callback, rank) */
                                /* remove a per-frame task (callback) */
                                /* scene resource request */
                                /* query participant presence */
                                /* scene completion flag */
                                /* scene beat / wait */
                                /* scene variant query */
                                /* scene-record accessor */
                                /* 16.16 pair setter */
                                /* start a scripted motion (slot, script) */
                                /* select the focus slot */
                                /* start a scripted motion, camera variant */
                                /* scene bounds */
                                /* place a slot at another record's position */
                                /* per-slot scene request */
                                /* per-slot scene request, second family */
                                /* per-slot presentation request */
                                /* select an entity presentation */
                                /* show a dialogue line by id */
                                /* dismiss a dialogue line by id */
                                /* wait for the slot's action to finish */
                                /* as Func_0808a180, with a duration */
                                /* face the slot along a heading over n frames */
                                /* move the slot to a waypoint over n frames */
                                /* camera distance pair (16.16) */
                                /* camera target (x, y, z, mode) */
                                /* commit the camera move */
                                /* scene amount (16.16) with a mode */
                                /* scene-effect request */
                                /* play a sound cue */

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

    fourth = Func_02001e0e(3);

    Func_02001e7c(0, 328, 212);                 /* 164 << 1 */
    Func_02001ee8(0, 0xc000, 20);               /* 192 << 8 */
    Func_02001f5e(17);
    Func_02001e16(0x14ed, 1);

    Func_02001e7c(1, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_02001e8a(2, 0x10000, 0x8000);

    record = Func_02001e88(0);
    if (record != 0) {
        Func_02001ece(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_02001e9c(0);
    if (record != 0) {
        Func_02001ee2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_02001ec2(1, (void *)0x02009ab8);
    Func_02001eca(2, (void *)0x02009af4);

    if (fourth != 0) {
        Func_02001ed4(3, 0x10000, 0x8000);
        record = Func_02001ed2(0);
        if (record != 0) {
            Func_02001f18(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_02001ef8(3, (void *)0x02009b30);
    }

    Func_02001f06(2);
    Func_02001ed4(40);
    Func_02001e02(0);
    Func_02001fd8(32);
    Func_02001e5e(40);

    APPROACH_STEP = 0;
    /* Func_020012b4 + 1 under the proven 0x02008000 base. */
    Func_02001e76((void *)0x020092b5, 3200);    /* 200 << 4 */
    Func_02001efc(40);

    Func_02001fb8(1, 0x6000, 20);               /* 192 << 7 */
    Func_02001fd8(0x00033333, 0x00006666);
    Func_02001ff2(0x01000000, -1, 0x00fe0000, 1);   /* 128 << 17, 254 << 16 */
    Func_02001ffe();
    Func_0200204c(246);
    Func_02001f32(40);

    Func_02001fee(2, 0x2000, 20);               /* 128 << 6 */
    Func_0200201c(0x019d0000, -1, 0x01050000, 1);
    Func_02002028();
    Func_02002076(246);
    Func_02001f5c(40);

    Func_02002018(0, 0x4000, 0);                /* 128 << 7 */
    Func_02002024(3, 0x4000, 20);
    Func_02002056(0x01460000, -1, 0x01800000, 1);   /* 163 << 17, 192 << 17 */
    Func_02002062();
    Func_020020b0(246);

    while (APPROACH_STEP != 24) {
        Func_02001f14(1);
    }

    Func_02001f30((void *)0x020092b5);
    Func_02001f26(10);

    /* Four flashes; the counter runs 0..3 inclusive (`cmp r5,#3 / bls`). */
    for (i = 0; i <= 3; i++) {
        Func_02001ede(0);
        Func_020020b4(6);
        Func_02001f3a(6);
        Func_02001ef0(1);
        Func_020020c6(6);
        Func_02001f4e(6);
    }

    Func_02001f08(0);
    Func_020020de(40);
    Func_02001f64(80);

    Func_020020ce(0x01480000, 0x00080000, 0x00d40000, 1);   /* 164<<17, 128<<12, 212<<16 */
    Func_020020da();
    Func_02002008(40);
    Func_02001974(10, 1);
    Func_02002016(40);
    Func_0200213c(7);
    Func_020020b2(0x14ee);
    Func_020020ca(8, 0);

    Func_020020aa(0, 2);
    Func_020020b2(1, 2);
    Func_020020ba(3, 2);
    Func_020020ca(2, 2);

    Func_02002106(0, 0xc000, 0);
    Func_02002112(1, 0xc000, 0);
    Func_0200211e(3, 0xc000, 0);
    Func_0200212a(2, 0xc000, 20);

    Func_020019e6(10, 2);
    Func_02002088(20);
    Func_020019f4(10, 3);
    Func_02002096(40);
    Func_02001a02(10, 1);
    Func_020020a4(20);
    Func_0200214c(8, 0);

    Func_02002176(0, 0x105, 0);
    Func_02002180(1, 0x105, 0);
    Func_0200218a(3, 0x105, 0);
    Func_02002194(2, 0x105, 40);

    Func_020021b4(0x00ea0000, 0, 0x00e80000, 1);    /* 234 << 16, 232 << 16 */
    Func_020021c0();
    Func_020020ee(40);
    Func_02001a5a(11, 1);
    Func_020020fc(40);
    Func_02001a68(11, 3);
    Func_0200210a(20);
    Func_020021bc(0x4009, 0, 20);
    Func_02001a80(11, 2);
    Func_02002122(10);

    Func_020021de(0, 0x4000, 0);
    Func_020021ea(1, 0x4000, 0);
    Func_020021f6(2, 0x4000, 0);
    Func_02002202(3, 0x4000, 20);

    Func_02001abe(11, 3);
    Func_02002160(20);
    Func_02001acc(11, 2);
    Func_0200216e(20);
    Func_02001ada(11, 3);
    Func_02002228(0x4009, 0, 10);
    Func_02001aec(10, 0);
    Func_0200218e(20);
    Func_02002236(0x8008, 0);
    Func_02001b02(10, 1);
    Func_020021a4(20);
    Func_02002244(0x8008, 0);

    Func_02002268(0, 0xc000, 0);
    Func_02002274(1, 0xe000, 0);                /* 224 << 8 */
    Func_02002280(2, 0xa000, 0);                /* 160 << 8 */
    Func_0200228c(3, 0xc000, 0);

    if (Func_020021fc(0, 0) == 0) {
        Func_02002290(0x4009, 0);
        Func_02002298(0x8008, 0);
    } else {
        SKIP_BEAT = (u16)(SKIP_BEAT + 2);

        Func_0200230c(3, 0x103, 0);
        Func_02002316(1, 0x103, 0);
        Func_02002320(2, 0x103, 40);
        Func_020022c8(1, 4);
        Func_02002310(1, 0);

        if (fourth != 0) {
            Func_020022fc(3, 2);
            Func_02002324(3, 0);
        } else {
            SKIP_BEAT = (u16)(SKIP_BEAT + 1);
        }

        Func_02002304(2, 3);
        Func_02002344(2, 0);
        Func_0200234c(0x4009, 0);
        Func_02002354(0x8008, 0);
    }

    /* 0x02000926: the two arms rejoin here. */
    Func_0200231c(0, 3);
    Func_02002324(1, 3);
    Func_0200232c(3, 3);
    Func_0200233c(2, 3);

    Func_020023b6(0x01480000, 0x00080000, 0x00d40000, 1);
    Func_020023c2();
    Func_020022f0(20);
    Func_02001c5c(10, 0);
    Func_020022fe(20);
    Func_0200222c(0);
    Func_02002402(1);
    Func_02002288(1);
    Func_02002400(0x00406218, 1);
    Func_02002416(40);
    Func_02002324(60);

    /* First burst: position the emitter's origin, then install it. */
    EMITTER_COUNTER = 0;
    EMITTER_ORIGIN[2] = 0x00cd0000;             /* 205 << 16 */
    EMITTER_ORIGIN[0] = 0x01480000;             /* 164 << 17 */
    EMITTER_ORIGIN[1] = 0x00300000;             /* 192 << 14 */
    /* Func_020015a0 + 1 under the proven 0x02008000 base. */
    Func_020022ca((void *)0x020095a1, 3200);
    Func_02002350(100);
    Func_020022de((void *)0x020095a1);

    Func_02002446(0x7fff, 0);
    Func_0200245c(60);
    Func_0200236a(100);
    Func_02002298(0);
    Func_0200246e(20);
    Func_0200237c(40);
    Func_02001ce8(10, 1);
    Func_0200238a(10);
    Func_02002420(0x14fb);
    Func_02002438(0x8008, 0);

    Func_02002400(0, 3);
    Func_02002408(1, 3);
    Func_02002410(3, 3);
    Func_02002420(2, 3);

    Func_02002498(0x00ea0000, 0, 0x00e80000, 1);
    Func_020024a4();
    Func_020023d2(20);
    Func_0200247a(0x4009, 0);
    Func_0200248c(0x8008, 0, 10);
    Func_0200246c(1, 2);
    Func_020024a8(0, 0x4000, 0);
    Func_020024b4(1, 0xe000, 10);
    Func_020024a4(1, 0);

    if (Func_0200242c(0, 0) == 0) {
        Func_020024e4(1, 258, 40);              /* 129 << 1 */
    } else {
        SKIP_BEAT = (u16)(SKIP_BEAT + 1);
        Func_020024be(1, 4);
    }

    /* 0x02000ad8: the two arms rejoin here. */
    Func_0200250e(1, 0);
    Func_02001dda(10, 4);
    Func_0200247c(20);
    Func_02002512(0x1501);
    Func_0200252a(0x8008, 0);
    Func_02002546(0, 0xc000, 0);
    Func_02002552(1, 0xc000, 0);
    Func_0200254a(0x8008, 0);

    Func_02002512(0, 3);
    Func_0200251a(1, 3);
    Func_02002522(3, 3);
    Func_02002532(2, 3);

    Func_02001e36(10, 4);
    Func_020024d8(20);
    Func_0200258a(0x8008, 0, 20);
    Func_02001e4e(11, 0);
    Func_0200259c(0x4009, 0, 20);
    Func_02001e60(11, 3);
    Func_02002502(40);
    Func_02001e6e(11, 1);
    Func_02002510(20);
    Func_020025c2(0x4009, 0, 20);
    Func_02001e86(10, 2);
    Func_02002528(20);
    Func_020025d0(0x8008, 0);

    Func_020025fc(0, 258, 0);
    Func_02002608(1, 258, 0);
    Func_02002614(3, 258, 0);
    Func_02002620(2, 258, 80);

    Func_02001ecc(11, 5);
    Func_0200256e(60);
    Func_02001eda(11, 3);
    Func_0200257c(20);
    Func_0200262e(0x4009, 0, 20);
    Func_02001ef2(10, 5);
    Func_02002594(40);
    Func_02001f00(10, 2);
    Func_020025a2(20);
    Func_02002654(0x4008, 0, 20);
    Func_02002634(1, 2);
    Func_02002670(1, 0x8000, 10);               /* 128 << 8 */
    Func_02002668(1, 0);
    Func_02002684(2, 0x8000, 20);
    Func_0200267c(0x8002, 0);
    Func_02001f48(11, 4);
    Func_020025ea(20);
    Func_0200269c(0x4009, 0, 20);
    Func_020026b0(1, 0xe000, 0);
    Func_020026bc(0, 0x4000, 10);
    Func_020026be(1, 0, 20);
    Func_02001f82(10, 1);
    Func_020026d0(0x8008, 0, 10);
    Func_02001f94(10, 2);
    Func_02002636(20);
    Func_02001fa2(11, 3);
    Func_02002644(40);
    Func_02001fb0(11, 0);
    Func_02002652(20);
    Func_02002580(0);
    Func_02002756(1);
    Func_020025dc(1);
    Func_02002754(0x00406218, 1);
    Func_0200276a(40);
    Func_02002678(60);

    /* Second burst, at a different origin. */
    EMITTER_ORIGIN[0] = 0x00880000;             /* 136 << 16 */
    EMITTER_ORIGIN[1] = 0x00140000;             /* 160 << 13 */
    EMITTER_ORIGIN[2] = 0x01020000;             /* 129 << 17 */
    EMITTER_COUNTER = 0;
    Func_0200261e((void *)0x020095a1, 3200);
    Func_020026a4(100);

    Func_02002760(0, 0x4000, 0);
    Func_0200276c(1, 0x4000, 0);
    Func_02002778(3, 0x4000, 0);
    Func_02002784(2, 0x4000, 40);
    Func_02002754(2, 1);
    Func_020027a8(2, 256, 20);                  /* 128 << 1 */
    Func_0200279a(0x8002, 0, 10);
    Func_0200277a(0, 2);
    Func_020027b6(0, 0x2000, 10);
    Func_02002776(0, 3);
    Func_0200207a(10, 4);
    Func_0200271c(20);
    Func_020027c4(0x8008, 0);
    Func_020027ee(2, 0x101, 60);
    Func_020027ea(2, 0xc000, 10);
    Func_020027ec(0x8002, 0, 10);
    Func_02002800(0, 0xc000, 0);
    Func_0200280c(3, 0xc000, 0);
    Func_02002818(1, 0xc000, 20);
    Func_0200281a(0x8008, 0, 10);

    Func_020027da(0, 3);
    Func_020027e2(1, 3);
    Func_020027ea(3, 3);
    Func_020027fa(2, 3);

    Func_02002798(10);
    Func_02002854(0, 0x4000, 0);
    Func_02002860(1, 0x4000, 0);
    Func_0200286c(3, 0x4000, 0);
    Func_02002878(2, 0x4000, 120);
    Func_02002756((void *)0x020095a1);
    Func_020027d4(60);
    Func_02002702(0);
    Func_020028d8(40);
    Func_0200214c(10, 2);
    Func_020027ee(20);
    Func_020028a0(0x8008, 0, 20);
    Func_02002164(11, 3);
    Func_020028a8(0x4009, 0);
    Func_020028b0(0x8008, 0);
    Func_0200217c(11, 4);
    Func_0200281e(20);
    Func_020028d0(0x4009, 0, 10);

    Func_020028a8(0, 2);
    Func_020028b0(1, 2);
    Func_020028b8(3, 2);
    Func_020028c8(2, 2);

    /* r0=10, r1=1 are set before the `b.n` that hops the third pool. */
    Func_020021e2(10, 1);

    Func_0200291e(0x8008, 0);
    Func_02002942(0, 0xc000, 0);
    Func_0200294e(1, 0xe000, 0);
    Func_0200295a(3, 0xc000, 0);
    Func_02002966(2, 0xa000, 0);

    if (Func_020028d6(0, 0) == 1) {
        SKIP_BEAT = (u16)(SKIP_BEAT + 1);
    }

    Func_020028d8(10);
    Func_02002244(10, 2);
    Func_020028e6(20);
    Func_02002252(11, 3);
    Func_020028f4(40);
    Func_02002260(10, 1);
    Func_02002902(20);
    Func_020029b4(0x8008, 0, 10);

    Func_020029c8(0, 0x4000, 0);
    Func_020029d2(1, 0, 0);
    Func_020029de(3, 0xc000, 0);
    Func_020029ea(2, 0x8000, 10);

    Func_020029a2(0, 3);
    Func_020029aa(1, 3);
    Func_020029b2(3, 3);
    Func_020029c2(2, 3);

    Func_02002a80(17);

    Func_0200299a(1, (void *)0x02009b6c);
    if (fourth != 0) {
        Func_020029a6(3, (void *)0x02009b6c);
    }
    Func_020029be(2, (void *)0x02009b6c);

    Func_020022ea(10, 4);
    Func_020022f2(10, 4);
    Func_02002994(20);
    Func_02002a2a(0x1519);
    Func_02002a42(0x8008, 0);
    Func_0200230e(11, 4);
    Func_02002316(11, 4);
    Func_020029b8(20);
    Func_02002a6a(0x4009, 0, 10);
    Func_02002a32(0, 3);
    Func_020029c8(0x845);
    Func_02002af6(1);
    Func_0200277a(184, 185);
}
