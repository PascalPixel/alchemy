#include "types.h"

/*
 * resource_38d owner at 0x020021bc, 588 bytes, exactly the advertised row:
 *   code 0x020021bc-0x02002305,
 *   INTERIOR literal pool 0x02002306-0x02002317 (a two-byte alignment gap plus
 *     the four words 0x00000000, 0x02410000, 0x02960000, 0xfffc0000),
 *   code 0x02002318-0x020023e9,
 *   trailing literal pool 0x020023ec-0x02002407.
 *
 * The interior pool is why this owner needs a control-flow walk rather than a
 * listing scan.  `b.n 0x02002318` at 0x02002304 hops it, and the disassembler
 * loses alignment inside it (it prints a bogus 32-bit instruction at
 * 0x02002316 that swallows the real `str r5, [r0, #12]` at 0x02002318).
 * Register state carries straight across the hop: r5 was loaded with
 * 0xfffc0000 at 0x020022f2 and r0 holds the record returned at 0x02002300, so
 * the store after the pool completes the four-record loop that begins before
 * it.  Reconstructed as one sequence, not two fragments.
 *
 * `overlay_show.ts` also lists 0x02002728 as a referenced pool word; that is
 * the documented false entry produced by the real word 0x00004ccc at
 * 0x020023f4 decoding as `ldr r4, [pc, #816]`.  It is not part of this owner.
 *
 * The scene body the router at 0x020006e0 selects when the scene selector is
 * 28, story flag 0x322 is set and 0x109 is clear.  It stages the whole cast -
 * slot 0, 19 and 17 get a heading and a position, the four-actor group 21-24
 * is placed in a row and reset - publishes two words into the workspace, then
 * plays the beat: scale, place, present, dialogue line, wait, reposition and
 * turn, finishing by clearing story flag 0x12f and setting 0x202.
 *
 * The workspace pointer is the word at 0x03001ebc (IWRAM).  Its two stores are
 * both the documented "one register is both value and displacement" shape: r3
 * is 224 << 1 = 448 when used as the displacement, then `adds r3, #65` makes
 * 513, which is the word stored at +448; `subs r3, #57` then makes 456, the
 * displacement of the second store, whose value is a fresh 16.  Neither value
 * is 448 +/- anything.
 *
 * Call accounting: 53 `bl` sites over 24 distinct targets, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_38d 21bc --json`, which reports
 * exactly `sites=53 distinct_targets=24 veneer=53` - no intra-overlay call and
 * no `call_via` slot (r3 is written only as Func_0808a210's fourth argument
 * and as workspace scratch).  Per-target multiset: Func_0808a080 x15,
 * Func_0808a0f0 x7, Func_080091e0 x4, and x2 each of Func_080091c0,
 * Func_0808a090, Func_0808a0d0, Func_0808a138, Func_0808a188, Func_0808a210,
 * with one site each for Func_080000c0, Func_08009128, Func_080770c8,
 * Func_080770d0, Func_0808a010, Func_0808a018, Func_0808a020, Func_0808a0c8,
 * Func_0808a100, Func_0808a110, Func_0808a170, Func_0808a1b8, Func_0808a228,
 * Func_0808a360, Func_0808a370.  Each appears below the same number of times.
 * The inventory row says calls=51, the documented lower bound.
 *
 * Func_080091c0 is the established six-argument renderer (four register
 * arguments plus [sp, #0] and [sp, #4], reserved by `sub sp, #8`); both sites
 * here are the same pair the router at 0x020006e0 issues.
 *
 * The epilogue is `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0`, so r0 holds
 * the popped return address and the owner is void.  The prologue is
 * `push {r5, r6, lr}` and the first branch is taken with no argument register
 * written, so the owner takes no arguments.
 *
 * UNCERTAINTIES:
 *  - Func_0808a228 is called with no argument register written and its result
 *    is used only to clear the record's byte at +85 - the same "+85 cleared"
 *    shape recorded elsewhere in the semantic tree for this import.
 *  - Record offset +6 is written as an unsigned halfword: 0 for slots 0 and
 *    19, 144 << 8 = 0x9000 for slot 17.  Sibling overlays model +6 as the
 *    heading; that reading fits the values but is not proven here.
 *  - Func_08009128 is called with no argument register written at its only
 *    site in this overlay.
 *  - Func_0808a0c8 and Func_0808a0d0 are handed the same argument shape one
 *    after the other for slots 19 and 0; they are distinct veneers.
 *  - The workspace words at +448 and +456 are written and never read here.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02004632();
void Func_02004754();
u8 *Func_02004760();
void Func_02004770();
void Func_0200463c();
void Func_0200464e();
u8 *Func_020046b4();
void Func_02004722();
u8 *Func_020046c8();
void Func_02004738();
u8 *Func_020046de();
void Func_02004750();
void Func_0200475e();
void Func_0200476c();
void Func_0200477a();
void Func_02004788();
u8 *Func_0200472e();
void Func_020046dc();
u8 *Func_0200473a();
void Func_020046e8();
u8 *Func_02004746();
void Func_020046f4();
u8 *Func_02004752();
void Func_02004700();
u8 *Func_0200475e_b();
u8 *Func_0200476a();
u8 *Func_02004774();
u8 *Func_0200477e();
u8 *Func_02004788_b();
u8 *Func_02004792();
u8 *Func_0200479a();
u8 *Func_020047b6();
void Func_02004744();
void Func_02004732();
void Func_020048dc();
void Func_020048f0();
void Func_020047f2();
void Func_020047fc();
void Func_0200483a();
void Func_02004850();
void Func_02004880();
void Func_020047ee();
void Func_020048ae();
void Func_020048cc();
void Func_020048ee();
void Func_02004882();
void Func_02004916();
void Func_020048de();
void Func_02004918();
void Func_020048d8();
void Func_02004836();
void Func_02004834();
void Func_02004858();
                                /* wait n frames */
                                /* scene request; no arguments set */
                                /* six-argument renderer, last two on the stack */
                                /* set presentation mode (record, phase) */
                                /* wait n frames */
                                /* scripted-scene bracket: open */
                                /* scripted-scene bracket: close */
                                /* record by slot index */
                                /* 16.16 scale pair */
                                /* place an entity */
                                /* place an entity */
                                /* place slot at (x, z), 16.16 */
                                /* per-record scene request */
                                /* scene-presentation request */
                                /* select an entity presentation */
                                /* show a dialogue line by id */
                                /* wait for the slot's action, with a duration */
                                /* face slot along heading */
                                /* scene request; interface not established */
                                /* returns a record; its byte at +85 is cleared */
                                /* scene bracket helper; no arguments set */
                                /* scene bracket helper; no arguments set */
                                /* set a story flag */
                                /* clear a story flag */

void Func_020021bc(void)
{
    u8 *workspace;
    u8 *record;

    Func_02004632();
    Func_02004754(-1, -1, -1, 0);

    record = Func_02004760();
    record[85] = 0;

    Func_02004770(157 << 18, -1, 187 << 18, 0);

    Func_0200463c(38, 55, 4, 1, 38, 45);
    Func_0200464e(42, 55, 4, 1, 38, 46);

    record = Func_020046b4(0);
    *(u16 *)(record + 6) = 0;
    Func_02004722(0, 0x02410000, 190 << 18);

    record = Func_020046c8(19);
    *(u16 *)(record + 6) = 0;
    Func_02004738(19, 148 << 18, 190 << 18);

    record = Func_020046de(17);
    *(u16 *)(record + 6) = 144 << 8;
    Func_02004750(17, 0x02960000, 191 << 18);

    Func_0200475e(21, 154 << 18, 182 << 18);
    Func_0200476c(22, 158 << 18, 182 << 18);
    Func_0200477a(23, 162 << 18, 182 << 18);
    Func_02004788(24, 166 << 18, 182 << 18);

    Func_020046dc(Func_0200472e(21), 0);
    Func_020046e8(Func_0200473a(22), 0);
    Func_020046f4(Func_02004746(23), 0);
    Func_02004700(Func_02004752(24), 0);

    record = Func_0200475e_b(21);
    record[85] = 0;
    record = Func_0200476a(22);
    record[85] = 0;
    record = Func_02004774(23);
    record[85] = 0;
    record = Func_0200477e(24);
    record[85] = 0;

    /* The interior literal pool is hopped in the middle of this group; r5 is
     * live across it and holds 0xfffc0000 for all four stores. */
    record = Func_02004788_b(21);
    record = Func_02004792(22);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0200479a(23);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_020047b6(24);
    *(s32 *)(record + 12) = (s32)0xfffc0000;

    Func_02004744();
    Func_02004732(1);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_020048dc();
    Func_020048f0();

    Func_020047f2(19, 0x9999, 0x4ccc);
    Func_020047fc(0, 0x9999, 0x4ccc);
    Func_0200483a(19, 628, 764);
    Func_02004850(0, 612, 764);

    Func_02004880(19, 1);
    Func_020047ee(20);
    Func_020048ae(19, 1);

    Func_020048cc(0x1746);
    Func_020048ee(19, 0, 10);

    Func_02004882(19, 0x26e, 780);
    Func_02004916(19, 0xc000, 10);

    Func_020048de(17, 2);
    Func_02004918(17, 0, 10);
    Func_020048d8(0, 3);

    Func_02004836(0x12f);
    Func_02004834(0x202);

    Func_02004858();
}
