/*
 * resource_3b7 owner at 0x020009f8, 208 bytes (0x020009f8-0x02000ac7).
 *
 * A priced yes/no transaction beat: quote a price, refuse if the player cannot
 * pay, otherwise charge it, ask for confirmation, and take one of two arms.
 *
 * Code runs 0x020009f8-0x02000aab.  The trailing 28 bytes,
 * 0x02000aac-0x02000ac7, are a literal pool of seven words - 0x02000240,
 * 0x00000e12, 0x02001000, 0x00000e0e, 0x000001fd, 0x00000089, 0x03001ebc.
 * They sit past the `bx r0` return and are reached only by `ldr rN, [pc, ...]`,
 * so they are data.
 *
 * None of those words is in the 0x0200_8xxx..0200_bxxx in-image band, so no
 * link-base shift applies to any of them: 0x02000240 is the RAM workspace the
 * byte-exact siblings `assets/code/resource_3b7_c_02000044.c` and
 * `..._020001a8.c` already name, and 0x03001ebc is the IWRAM workspace pointer
 * this project has already established (it is 0x03001e70 + 76, the base
 * Func_02000880 in this same overlay loads).  0x02001000 is written but never
 * read here.  (The overlay's link base is 0x02008000 - see
 * semantic/overlays/resource_3b7_c_020016a8.c for the three witnesses - it just
 * does not bear on this owner.)
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x020009fa -> veneer 0x020018fc -> Func_080772c8
 *   0x02000a04 -> veneer 0x0200191c -> Func_0808a018
 *   0x02000a12 -> veneer 0x02001964 -> Func_0808a170
 *   0x02000a1a -> veneer 0x0200196c -> Func_0808a178
 *   0x02000a24 -> veneer 0x020019b4 -> Func_0808a5b0
 *   0x02000a2a -> veneer 0x02001964 -> Func_0808a170
 *   0x02000a32 -> veneer 0x020018c4 -> Func_08015120
 *   0x02000a3a -> veneer 0x0200196c -> Func_0808a178
 *   0x02000a42 -> veneer 0x02001934 -> Func_0808a070
 *   0x02000a4e -> veneer 0x02001974 -> Func_0808a180
 *   0x02000a58 -> veneer 0x02001944 -> Func_0808a0d0
 *   0x02000a62 -> veneer 0x02001944 -> Func_0808a0d0
 *   0x02000a6e -> veneer 0x0200197c -> Func_0808a1b8
 *   0x02000a74 -> veneer 0x02001914 -> Func_0808a010
 *   0x02000a7c -> veneer 0x0200198c -> Func_0808a270
 *   0x02000a84 -> veneer 0x02001984 -> Func_0808a260
 *   0x02000a9e -> veneer 0x02001974 -> Func_0808a180
 *   0x02000aa2 -> veneer 0x02001924 -> Func_0808a020
 * Completeness: that multiset is 18 sites over 14 distinct targets, which is
 * exactly the tool's `sites=18 distinct_targets=14`.  The inventory row's
 * calls=14 counts distinct targets, not sites, and agrees on that reading -
 * per HANDOVER the field is a lower bound on sites, never an upper one.
 *
 * The Func_0808a018 / Func_0808a020 bracket, Func_0808a170(id) line playback,
 * Func_0808a178 / Func_0808a180 slot open/close and the zero-tested
 * Func_0808a070(0, 0) prompt are all the established cutscene-script family
 * (semantic/overlays/resource_373_c_02002f14.c uses the same prompt with the
 * same "counter runs on the nonzero result" shape found here).
 *
 * Note the early-out arm: the "cannot pay" path branches straight to the
 * epilogue at 0x02000aa6 and therefore does NOT run the closing Func_0808a020.
 * That asymmetry is in the bytes (`b.n 0x02000aa6` at 0x02000a1e), not a
 * transcription slip.
 *
 * The epilogue is `pop {r5} / pop {r0} / bx r0`, so r0 holds the popped return
 * address and the owner returns nothing: void.
 *
 * Uncertainties:
 *  - r0 is NOT written before the first branch, so if Func_080772c8 reads r0
 *    this owner is forwarding an incoming first argument.  Nothing in this
 *    overlay calls 0x020009f8 (`overlay_call_targets.ts` over the whole overlay
 *    lists no site for it), so there is no local evidence either way; it is
 *    declared with no parameter and the ambiguity is recorded here rather than
 *    guessed at.
 *  - The `bcs` is an UNSIGNED compare, so `balance` and `price` are treated as
 *    unsigned; the price is `Func_080772c8() * 10` via `(x << 2) + x) << 1`.
 *  - Func_08015120(price, 5) is read as the charge because it is the only call
 *    that receives the price, but its interface is unknown.
 *  - The imports' interfaces are unknown generally, hence the old-style
 *    declarations; Func_080772c8 and Func_0808a070 are declared with return
 *    types because their results are used.
 */

#include "types.h"

extern s32 Data_02000240[];     /* RAM workspace; also indexed as s16 elsewhere */
extern s32 Data_02001000;

s32 Func_080772c8();
s32 Func_0808a070();
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a260();
void Func_0808a270();
void Func_0808a5b0();
void Func_08015120();

void Func_020009f8(void)
{
    u32 price;
    u32 balance;

    /* (n << 2) + n, then << 1 - ten times the returned count. */
    price = (u32)(Func_080772c8() * 10);

    Func_0808a018();

    balance = (u32)Data_02000240[4];        /* the word at byte offset 16 */

    if (balance < price) {
        Func_0808a170(0xe12);
        Func_0808a178(9, 0);
        /* Straight to the epilogue: Func_0808a020 is deliberately skipped. */
        return;
    }

    Data_02001000 = (s32)balance;

    Func_0808a5b0();
    Func_0808a170(0xe0e);
    Func_08015120(price, 5);
    Func_0808a178(9, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a180(9, 0);
        Func_0808a0d0(0, 0x78, 0x80);
        Func_0808a0d0(0, 0x78, 0x98);
        Func_0808a1b8(0, 0x8000, 0);        /* 128 << 8 */
        Func_0808a010(20);
        Func_0808a270(0x1fd, 0);
        Func_0808a260(0x89, 13);
    } else {
        u16 *counter = (u16 *)(*(char **)0x03001ebc + 472);   /* 236 * 2 */

        *counter = (u16)(*counter + 1);
        Func_0808a180(9, 0);
    }

    Func_0808a020();
}
