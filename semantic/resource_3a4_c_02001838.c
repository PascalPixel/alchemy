#include "types.h"

/*
 * resource_3a4 owner at 0x02001838, 1236 bytes: a long linear cutscene on
 * actor 10 -- four camera placements, six sound cues, a repeated
 * fade-out/fade-in pair, three dialogue lines and three more after a
 * scene change, closing by setting story flag 0x904.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. No
 * arguments, void.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02001838 through the
 * high-register unwind `add sp, #8 / pop {r3, r5, r6} / mov r8, r3 /
 * mov r9, r5 / mov sl, r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001cd8-0x02001ce6 and the nine-word literal pool
 * 0x02001ce8-0x02001d0b; the next residue owner is 0x02001d0c. That is
 * 1236 bytes, which is EXACTLY the upper bound the recorded owner list
 * gives, so 0x02001d0c's own bound of 788 is now fixed too -- the two
 * tile 0x02001838..0x02002020 with nothing between them.
 *
 * measureSpan reports 1200, undersizing by the closing pool. It is a
 * hint in neither direction and was not used to bound this row.
 *
 * THIS ROW IS THE OVERLAY'S THIRD PUBLISHED-POINTER PUBLISHER, and the
 * one that names a publisher for a row already drafted blind. The pool
 * word at 0x020018fc is 0x02009771. In-image pointers are spelled
 * base + 0x8000 (Data_0200cd18 is image offset 0x4d18), so 0x02009771 is
 * image offset 0x1770 with the Thumb bit -- the already-drafted
 * 0x02001770, this overlay's per-frame record updater. The row stores it
 * into the actor handle at +108 three times and clears that same field
 * three times, and never calls it. After 0x02002b58 -> 0x02002a48 and
 * 0x02002f10 -> 0x02002eec, that is three instances of the mechanism in
 * one overlay, and the first where the published row was drafted before
 * its publisher was found.
 *
 * A hundred and seven call sites, every one resolved with the `+2` rule
 * via `bun tools/lib/overlay_call_targets.ts resource_3a4 0x1838 0x1d0c` and
 * none read from the listing's arrows, which print a hundred and seven
 * nonexistent 0x02004xxx-0x02005xxx addresses. They collapse onto
 * nineteen targets: eighteen import veneers and ONE in-image prologue.
 * The site at 0x0200196e resolves to 0x2002d08, a prologue rather than a
 * veneer, so Func_02002d08() -- an already-drafted owner of this same
 * overlay. Twenty sites share Func_0808a010 alone and eleven share
 * Func_0808a1b8.
 *
 * Site count was cross-checked: the resolver reports sites=107 for these
 * bounds and the listing carries 107 `bl` lines, so nothing was lost to
 * the interior-pool truncation that has bitten this overlay's big rows.
 * The two interior pools, 0x020018e0-0x02001913 and the closing one, are
 * jumped with `b.n 0x2001914` at 0x020018de and reached by falling off
 * the end respectively.
 *
 * THREE CALLEE-SAVED REGISTERS ARE RECYCLED MID-BODY and reading any of
 * them as live across that point would be wrong:
 *   - r9 holds the actor-10 handle from 0x0200184a until 0x02001b32,
 *     where `mov r9, r2` overwrites it with the constant 17;
 *   - sl holds 0 from 0x020018ae until 0x02001b48, where it becomes 18;
 *   - r8 holds handle + 100 from 0x020018a8 until 0x02001b5e, where it
 *     becomes 19;
 *   - r5 holds 0 until 0x02001b2e, where it becomes 13;
 *   - r6 holds 0x02009771 until 0x02001b8e, where it becomes 160 << 11.
 * The three new values 17, 18 and 19 are the ids the six Func_080091c0
 * calls pass on the stack. Every site below is transcribed from its own
 * instructions rather than from a register assumed live.
 *
 * The handle field group +100/+102/+104/+108 is stamped three times as a
 * unit -- word +104 cleared, halfwords +100 and +102 cleared, and +108
 * set to the published pointer -- and +108 is cleared on its own shortly
 * after each. +72 is written once, at the head, with 0x6666.
 *
 * Constants are written in the shifted-literal form the encodings build
 * -- 149 << 17, 167 << 17, 152 << 17, 128 << 14, 212 << 16, 244 << 16,
 * 215 << 16, 176 << 8, 128 << 8, 240 << 8, 208 << 8, 160 << 7, 192 << 6,
 * 128 << 9, 128 << 11, 160 << 11, 154 << 1, 170 << 1, 187 << 1, 190 << 1,
 * 130 << 1, 160 << 1, 131 << 1 -- because that shape, not the flattened
 * value, is what the row shows. Pool constants 0x26666, 0x4ccc,
 * 0x1510000, 0xcccc, 0x1999, 0x1270000, 0x6666, 0x13333, 0x9999, 0x123,
 * 0x137, 0xe666, 0x149, 0xb333, 0x16666, 0x127, 0x4cccc and 0x904 are
 * transcribed as words.
 *
 * The only argument-bearing pool address is 0x0200bd34 at 0x02001d04,
 * image offset 0x3d34, handed to Func_0808a168 and never dereferenced
 * here, so it is declared as in-image data.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a168(s32 id, s32 arg1, u8 *table);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(u8 *handle, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flag_id);
extern void Func_080f9010(s32 sound_id);
extern void Func_02002d08(void);

extern u8 Data_0200bd34[];   /* image offset 0x3d34 */

/* image offset 0x1770 with the Thumb bit: this overlay's own 0x02001770 */
#define PUBLISHED_02001770 0x02009771

void Func_02001838(void)
{
    u8 *actor;
    u8 *other;

    actor = Func_0808a080(10);
    Func_0808a018();

    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(149 << 17, -1, 0x1510000, 1);
    Func_0808a218();
    Func_080f9010(147);
    Func_0808a138(10, 2);
    Func_0808a010(40);

    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x1270000, 128 << 14, 212 << 16, 1);

    *(u16 *)(actor + 102) = 0;
    *(s32 *)(actor + 104) = 0;
    *(s32 *)(actor + 108) = PUBLISHED_02001770;
    *(s32 *)(actor + 72) = 0x6666;
    *(u16 *)(actor + 100) = 0;

    Func_0808a090(10, 0x13333, 0x9999);
    Func_0808a0c0(10, 154 << 1, 0x123);
    Func_0808a0c0(10, 0x137, 215);

    *(s32 *)(actor + 108) = 0;
    actor[91] = 0;

    Func_0808a010(16);
    Func_0808a100(10, 1);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    Func_0808a1b8(10, 176 << 8, 20);
    Func_0808a1b8(10, 128 << 8, 40);
    Func_02002d08();
    Func_0808a010(40);
    Func_0808a1b8(10, 160 << 7, 20);
    Func_0808a1b8(10, 192 << 6, 40);

    Func_0808a210(167 << 17, -1, 244 << 16, 1);

    *(s32 *)(actor + 104) = 0;
    *(u16 *)(actor + 100) = 0;
    *(u16 *)(actor + 102) = 0;
    *(s32 *)(actor + 108) = PUBLISHED_02001770;

    Func_0808a0c0(10, 160 << 1, 232);
    Func_0808a0c0(10, 170 << 1, 131 << 1);
    Func_0808a0c0(10, 187 << 1, 131 << 1);

    *(s32 *)(actor + 108) = 0;

    Func_0808a010(16);
    Func_0808a100(10, 1);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(20);

    Func_0808a1b8(10, 240 << 8, 20);
    Func_0808a1b8(10, 208 << 8, 40);
    Func_080f9010(153);

    other = Func_0808a080(10);
    *(s32 *)(other + 40) = 128 << 11;

    Func_0808a100(10, 2);
    Func_0808a0c0(10, 190 << 1, 248);
    Func_0808a010(10);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(6);
    Func_0808a100(10, 1);
    Func_0808a010(40);

    Func_0808a1b8(10, 176 << 8, 20);
    Func_0808a1b8(10, 128 << 8, 40);

    Func_0808a210(152 << 17, -1, 215 << 16, 1);

    *(s32 *)(actor + 104) = 0;
    *(u16 *)(actor + 100) = 0;
    *(u16 *)(actor + 102) = 0;
    *(s32 *)(actor + 108) = PUBLISHED_02001770;

    Func_0808a0c0(10, 0x149, 219);

    *(s32 *)(actor + 108) = 0;

    Func_0808a100(10, 1);
    Func_0808a010(16);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);
    Func_0808a1b8(10, 128 << 8, 40);

    other = Func_0808a080(9);
    other[85] = 0;

    Func_080091c0(3, 0, 1, 1, 17, 13);
    Func_080091c0(3, 0, 1, 1, 18, 13);
    Func_080091c0(3, 0, 1, 1, 19, 13);

    Func_0808a090(10, 0x16666, 0xb333);
    Func_080091e0(Func_0808a080(10), 0);
    Func_080f9010(153);

    other = Func_0808a080(10);
    *(s32 *)(other + 40) = 160 << 11;

    Func_0808a100(10, 3);
    Func_0808a0c0(10, 0x127, 215);
    Func_0808a100(10, 1);
    Func_080091e0(Func_0808a080(10), 1);

    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    Func_080f9010(153);
    other = Func_0808a080(10);
    *(s32 *)(other + 40) = 160 << 11;

    Func_0808a100(10, 3);
    Func_0808a0c0(10, 130 << 1, 215);
    Func_0808a100(10, 1);

    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    Func_0808a1b8(10, 160 << 7, 20);
    Func_0808a1b8(10, 192 << 6, 20);
    Func_080f9010(147);
    Func_0808a138(10, 2);
    Func_0808a010(40);

    Func_080091c0(4, 0, 1, 1, 17, 13);
    Func_080091c0(2, 0, 1, 1, 18, 13);
    Func_080091c0(4, 0, 1, 1, 19, 13);

    other = Func_0808a080(0);
    Func_0808a208(0x4cccc, 0x9999);
    Func_0808a210(*(s32 *)(other + 8), *(s32 *)(other + 12),
                  *(s32 *)(other + 16), 1);
    Func_0808a218();

    Func_0808a168(10, 128 << 9, Data_0200bd34);
    Func_080770c8(0x904);
    Func_0808a020();
}
