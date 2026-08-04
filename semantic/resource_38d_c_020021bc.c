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
 * `bun tools/overlay_call_targets.ts resource_38d 21bc --json`, which reports
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
void Func_080000c0();           /* wait n frames */
void Func_08009128();           /* scene request; no arguments set */
void Func_080091c0();           /* six-argument renderer, last two on the stack */
void Func_080091e0();           /* set presentation mode (record, phase) */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0c8();           /* place an entity */
void Func_0808a0d0();           /* place an entity */
void Func_0808a0f0();           /* place slot at (x, z), 16.16 */
void Func_0808a100();           /* per-record scene request */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a210();           /* scene request; interface not established */
u8 *Func_0808a228();            /* returns a record; its byte at +85 is cleared */
void Func_0808a360();           /* scene bracket helper; no arguments set */
void Func_0808a370();           /* scene bracket helper; no arguments set */
void Func_080770c8();           /* set a story flag */
void Func_080770d0();           /* clear a story flag */

void Func_020021bc(void)
{
    u8 *workspace;
    u8 *record;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);

    record = Func_0808a228();
    record[85] = 0;

    Func_0808a210(157 << 18, -1, 187 << 18, 0);

    Func_080091c0(38, 55, 4, 1, 38, 45);
    Func_080091c0(42, 55, 4, 1, 38, 46);

    record = Func_0808a080(0);
    *(u16 *)(record + 6) = 0;
    Func_0808a0f0(0, 0x02410000, 190 << 18);

    record = Func_0808a080(19);
    *(u16 *)(record + 6) = 0;
    Func_0808a0f0(19, 148 << 18, 190 << 18);

    record = Func_0808a080(17);
    *(u16 *)(record + 6) = 144 << 8;
    Func_0808a0f0(17, 0x02960000, 191 << 18);

    Func_0808a0f0(21, 154 << 18, 182 << 18);
    Func_0808a0f0(22, 158 << 18, 182 << 18);
    Func_0808a0f0(23, 162 << 18, 182 << 18);
    Func_0808a0f0(24, 166 << 18, 182 << 18);

    Func_080091e0(Func_0808a080(21), 0);
    Func_080091e0(Func_0808a080(22), 0);
    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);

    record = Func_0808a080(21);
    record[85] = 0;
    record = Func_0808a080(22);
    record[85] = 0;
    record = Func_0808a080(23);
    record[85] = 0;
    record = Func_0808a080(24);
    record[85] = 0;

    /* The interior literal pool is hopped in the middle of this group; r5 is
     * live across it and holds 0xfffc0000 for all four stores. */
    record = Func_0808a080(21);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(22);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(23);
    *(s32 *)(record + 12) = (s32)0xfffc0000;
    record = Func_0808a080(24);
    *(s32 *)(record + 12) = (s32)0xfffc0000;

    Func_08009128();
    Func_080000c0(1);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a360();
    Func_0808a370();

    Func_0808a090(19, 0x9999, 0x4ccc);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0c8(19, 628, 764);
    Func_0808a0d0(0, 612, 764);

    Func_0808a100(19, 1);
    Func_0808a010(20);
    Func_0808a138(19, 1);

    Func_0808a170(0x1746);
    Func_0808a188(19, 0, 10);

    Func_0808a0d0(19, 0x26e, 780);
    Func_0808a1b8(19, 0xc000, 10);

    Func_0808a138(17, 2);
    Func_0808a188(17, 0, 10);
    Func_0808a110(0, 3);

    Func_080770d0(0x12f);
    Func_080770c8(0x202);

    Func_0808a020();
}
