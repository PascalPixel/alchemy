#include "types.h"

/*
 * resource_38d owner at 0x020019b0, 2060 bytes, exactly the advertised row.
 * Ranges, settled by a control-flow walk from the prologue (follow every
 * branch; whatever is never reached as an instruction is pool):
 *   code            0x020019b0-0x02001caf
 *   INTERIOR pool   0x02001cb0-0x02001ceb   (15 words)
 *   code            0x02001cec-0x02001e27
 *   INTERIOR pool   0x02001e28-0x02001e47   (8 words)
 *   code            0x02001e48-0x02002181   (0x02002182 is alignment)
 *   trailing pool   0x02002184-0x020021bb   (14 words)
 * Both interior pools are hopped by a lone unconditional `b.n` (0x02001cae ->
 * 0x02001cf2 and 0x02001e26 -> 0x02001e4e) and each is immediately followed by
 * the other arm of the same conditional, whose branch target is the first
 * instruction after the pool.  Register state carries straight across them.
 *
 * `overlay_show.ts` lists 0x02002004 as a fourteenth "pool word referenced";
 * that is the documented false entry produced by the real word 0x00004ccc at
 * 0x02001cd0 decoding as `ldr r4, [pc, #816]`.  It is not part of this owner.
 *
 * The scene body the router at 0x020006e0 selects when the scene selector is
 * 29, story flag 0x84e is clear, 0x85e is clear and 0x845 is set.  It is the
 * overlay's longest scene: it stages the cast, publishes two workspace words,
 * runs a long sequence of pose / turn / wait / dialogue beats across slots 0,
 * 1, 2, 3, 18 and 19, asks two yes-no prompts whose answers only steer a beat
 * counter, and finishes by setting story flags 0x322, 0x84f and 0x84a.
 *
 * BEAT COUNTER.  Nine sites bump the u16 at workspace + 472 by one and one
 * site bumps it by four.  This is the documented skip-beat idiom: an r5 flag
 * is set to 1, cleared to 0 on the arm that bumps immediately, and the *other*
 * arm bumps after the following wait, so exactly one bump happens per gated
 * beat either way.  The counter is written and never read here.  Workspace is
 * the pointer at 0x03001ebc (IWRAM); the owner keeps its *address* in r6 for
 * the whole body and reloads the pointer at each use, which is what the macro
 * below reproduces.  472 is built as 236 << 1 every time.
 *
 * The two workspace stores at 0x02001a0e/0x02001a16 are the documented "one
 * register is both value and displacement" shape: r3 is 224 << 1 = 448 as the
 * displacement, `adds r3, #65` makes 513 (the stored word), `subs r3, #57`
 * makes 456 (the next displacement) and 16 is a fresh value.  The owners at
 * 0x020021bc and 0x020005f4 perform the same pair.
 *
 * COORDINATE VIEWS.  0x02001b1e reads the record's s32 words at +8 and +16 and
 * passes them to Func_0808a0f0 (16.16); 0x0200204e reads the s16 halfwords at
 * +10 and +18 and passes them to Func_0808a0b8.  Those are the integer parts
 * of the same two 16.16 words, the double view the byte-exact overlay sources
 * already model.  The `movs r3, #10 / ldrsh r1, [r0, r3]` offset register is
 * NOT a fourth argument to the following call - it is the load offset.
 *
 * Call accounting: 186 `bl` sites over 37 distinct targets, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 19b0 --json`, which reports
 * exactly `sites=186 distinct_targets=37 veneer=178 prologue=8`.  The eight
 * prologue sites are four calls each to the byte-exact intra-overlay owners
 * Func_02001958 and Func_02001984 (`assets/code/resource_38d_c_02001958.c` and
 * `_02001984.c`), which take no arguments.  No `call_via` slot: r3 is written
 * only as an immediate argument, a load offset or workspace scratch, never
 * with an IWRAM code address.  Every site appears below exactly once and the
 * per-target multisets were compared target by target.  The inventory row says
 * calls=178, the documented lower bound.
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.  The prologue is `push {r5, r6, lr}`
 * and the first branch is taken with no argument register written, so the
 * owner takes no arguments.
 *
 * Link base 0x02008000 (witnessed by the installed handler words 0x020085f5 =
 * Func_020005f4 + 1 and 0x02008569 = Func_02000568 + 1).  Two pool words are
 * in-image addresses under it: 0x0200a5d4 (file offset 0x25d4) passed to
 * Func_0808a098 as a motion script, and 0x0200a5c0 (0x25c0) passed to
 * Func_0808a168.  Both are even, which is the documented one-bit test for
 * in-image data rather than a Thumb entry point.
 *
 * UNCERTAINTIES:
 *  - Func_0808a070's result is compared against 0 at two sites and against 1
 *    at a third; it is the established dialogue-prompt result but the encoding
 *    of the answers is not settled here.
 *  - Func_0808a188/Func_0808a180/Func_0808a178 are reached both with small
 *    slot numbers and with the pooled selectors 0x2012, 0x4001, 0x4002 and
 *    0x4003.  0x2012 is 0x2000 | 18 and the 0x400N group is 0x4000 | N, i.e.
 *    a flag field packed above the slot, but nothing here proves the split, so
 *    they are left as literals.
 *  - Func_0808a208, Func_0808a210, Func_0808a0a0, Func_0808a0e8, Func_0808a168
 *    and Func_0808a368 have no established interface; their argument registers
 *    are set explicitly at each site and are asserted as written.
 *  - Func_0808a360/Func_0808a368/Func_0808a370 are called with no argument
 *    register written, like the Func_0808a018 bracket.
 *  - The three `if (record != 0)` guards are in the bytes: Func_0808a080 can
 *    return 0 and the code tests it before dereferencing.
 */

/* The scene's beat counter: u16 at workspace + 472, workspace being the
 * pointer stored at 0x03001ebc.  Reloaded at every use, as the assembly does. */
#define BEAT_COUNTER (*(u16 *)(*(u8 **)0x03001ebc + 472))

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_080000c0();           /* wait n frames */
void Func_08009128();           /* scene request; no arguments set */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
s32 Func_0808a070();            /* dialogue prompt; result selects the branch */
u8 *Func_0808a080();            /* record by slot index, or 0 */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a098();           /* start a scripted motion (slot, script) */
void Func_0808a0a0();           /* scene request; interface not established */
void Func_0808a0b8();           /* place slot at an integer (x, z) */
void Func_0808a0c8();           /* place an entity */
void Func_0808a0d0();           /* place an entity */
void Func_0808a0e0();           /* start a move; interface not established */
void Func_0808a0e8();           /* scene request; interface not established */
void Func_0808a0f0();           /* place slot at (x, z), 16.16 */
void Func_0808a100();           /* per-record scene request */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a168();           /* scene request with an in-image data table */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dialogue-related request */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a208();           /* scene request; interface not established */
void Func_0808a210();           /* scene request; interface not established */
u8 *Func_0808a228();            /* returns a record; its byte at +85 is cleared */
void Func_0808a248();           /* scene request keyed by a selector */
void Func_0808a360();           /* scene bracket helper; no arguments set */
void Func_0808a368();           /* scene bracket helper; no arguments set */
void Func_0808a370();           /* scene bracket helper; no arguments set */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */

/* Intra-overlay, both byte-exact in assets/code and both void(void). */
void Func_02001958();
void Func_02001984();

void Func_020019b0(void)
{
    u8 *workspace;
    u8 *record;
    s32 pending;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);

    record = Func_0808a228();
    record[85] = 0;
    Func_0808a210(0x037e0000, -1, 166 << 18, 0);
    Func_080000c0(1);

    Func_0808a0f0(0, 0, 0);
    Func_08009128();
    Func_080000c0(1);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_0808a0f0(19, 222 << 18, 0x031e0000);
    Func_0808a0f0(0, 226 << 18, 0x031e0000);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x037e0000, -1, 0x02ba0000, 1);

    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0c8(19, 888, 720);
    Func_0808a0c8(0, 904, 736);
    Func_0808a010(60);

    Func_0808a0e8(19);
    Func_0808a100(19, 1);
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    Func_0808a010(20);

    Func_0808a138(19, 2);
    Func_0808a170(0x1728);

    pending = 1;
    if (Func_080770c0(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_0808a180(19, 0);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_0808a210(0x037e0000, -1, 166 << 18, 1);
    Func_0808a098(19, (void *)0x0200a5d4);
    Func_0808a0d0(0, 0x037e, 684);

    /* Slots 1, 2 and 3 are put where slot 0's record already is. */
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a090(2, 0x9999, 0x4ccc);
    Func_0808a090(3, 0x10000, 0x8000);

    Func_0808a100(1, 2);
    Func_0808a100(2, 2);
    Func_0808a100(3, 2);

    Func_0808a0e0(1, -16, 16);
    Func_0808a0e0(2, 16, 16);
    Func_0808a0e0(3, 32, 16);

    Func_0808a0e8(2);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a100(3, 1);
    Func_0808a010(10);

    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a0e8(3);
    Func_0808a1b8(3, 0xa000, 0);

    Func_0808a0a0(19);
    Func_0808a010(20);

    pending = 1;
    if (Func_080770c0(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_0808a138(18, 3);
    Func_0808a188(0x2012, 0, 20);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    pending = 1;
    if (Func_080770c0(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_0808a138(18, 1);
    Func_0808a188(0x2012, 0, 10);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_02001958();
    Func_0808a010(20);

    if (Func_080770c0(0x84f) != 0) {
        Func_0808a1e8(0, 258, 60);
        Func_0808a1e8(1, 0x105, 40);
    } else {
        Func_0808a010(40);
    }

    Func_0808a1b8(1, 0xc000, 10);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a110(2, 3);
    Func_0808a180(0x4002, 0);
    Func_0808a1b8(3, 0xa000, 10);
    Func_0808a110(3, 3);
    Func_0808a188(0x4003, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(0x2012, 0, 20);

    Func_0808a1e8(1, 0x103, 0);
    Func_0808a1e8(0, 258, 60);

    if (Func_080770c0(0x84f) != 0) {
        Func_0808a138(18, 1);
        Func_0808a110(18, 4);
        Func_0808a178(0x2012, 0);
        Func_02001958();

        pending = 1;
        if (Func_0808a070(0, 0) != 0) {
            BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
            pending = 0;
        }
        Func_0808a1b8(18, 0x5000, 0);
        Func_02001984();
        Func_0808a010(10);
        Func_0808a188(0x2012, 0, 10);
        if (pending != 0) {
            BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        }
        Func_0808a1e8(18, 258, 60);
    } else {
        /* The whole gated block above is worth four beats. */
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 4);
    }

    Func_0808a178(0x2012, 0);
    Func_02001958();
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0x1737);
    } else {
        Func_0808a170(0x1738);
    }
    Func_02001984();
    Func_0808a188(0x2012, 0, 20);

    Func_0808a138(19, 1);
    Func_0808a170(0x1739);
    Func_0808a188(19, 0, 10);

    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0, 0);
    Func_0808a1b8(3, 0x8000, 40);

    Func_0808a138(18, 2);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_02001984();
    Func_0808a010(10);

    Func_0808a1e8(18, 0x105, 60);
    Func_0808a188(0x2012, 0, 10);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a110(18, 3);
    Func_0808a188(0x2012, 0, 10);

    Func_0808a1e8(18, 264, 60);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(18, 0x3000, 10);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a110(18, 3);

    Func_0808a178(0x2012, 0);
    Func_02001958();

    pending = 1;
    if (Func_0808a070(0, 0) == 1) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_02001984();
    Func_0808a188(0x2012, 0, 10);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_0808a1b8(18, 0x7000, 10);
    Func_0808a138(19, 1);
    Func_0808a1b8(19, 0x1000, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a110(19, 3);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(19, 0x3000, 10);
    Func_0808a1b8(18, 0x3000, 20);
    Func_0808a138(18, 1);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a110(18, 3);
    Func_0808a188(0x2012, 0, 10);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    /* Integer (x, z) views of slot 0's position, offsets +10 and +18. */
    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a100(3, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);
    Func_0808a010(20);

    Func_0808a1b8(18, 0x5000, 0);
    Func_0808a168(0, 0x00010013, (void *)0x0200a5c0);

    Func_0808a0d0(19, 852, 0x286);
    Func_0808a0d0(19, 852, 0x29a);
    Func_0808a0d0(19, 864, 672);
    Func_0808a1b8(19, 0x1000, 10);
    Func_0808a138(19, 1);
    Func_0808a010(10);
    Func_0808a188(19, 0, 10);
    Func_0808a0d0(19, 0x376, 708);
    Func_0808a0c8(19, 0x37e, 764);
    Func_0808a0d0(0, 0x37e, 764);

    Func_0808a368();
    Func_0808a370();

    Func_080770c8(0x322);
    if (Func_080770c0(0x84f) == 0) {
        Func_080770c8(0x84f);
        Func_080770c8(0x84a);
    }

    Func_0808a248(6);
    Func_0808a020();
}
