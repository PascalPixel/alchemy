/*
 * resource_3b7 owner at 0x02000ac8, 680 bytes (0x02000ac8-0x02000d6f).
 *
 * The overlay's minigame front end: a two-entry wager menu that loops until the
 * player cancels, then a confirm-with-A / abort-with-B wait, then the round
 * itself via Func_0200173c, then either a coin payout (wager 0) or a
 * weighted-random prize draw (wager 1).
 *
 * Code runs 0x02000ac8-0x02000d4b.  The trailing 36 bytes,
 * 0x02000d4c-0x02000d6f, are a literal pool of nine words - 0x00000e43,
 * 0x02000240, 0x00000e49, 0x00000e4c, 0x03001c94, 0x0200a00c, 0x00000e46,
 * 0x0000011d, 0x02009fd0.  They lie past the `bx r1` return and are read only
 * by `ldr rN, [pc, ...]`, so they are data.  Note that `overlay_show.ts` lists
 * a tenth "pool word referenced" at 0x02001b7c, which this owner never touches:
 * it comes from 0x02009fd0 decoding as `ldr r7, [sp, #832]`.  The pool map here
 * is taken from the owner's own control flow, not from that listing.
 *
 * LINK BASE - 0x02008000 (three witnesses in
 * semantic/overlays/resource_3b7_c_020016a8.c), and this owner adds a fourth,
 * self-contained one.  The image is 0x206a bytes, and the two in-image pool
 * words resolve to adjacent, exactly-sized tables:
 *   0x02009fd0 -> file offset 0x1fd0: fifteen s32 words - 0x17, 0x7c, 0x51,
 *     0x98, 0x25, 0x6c, 0xab, 0x8e, 0x30, 0xa0, 0x85, 0x61, 0xb7, 0xba, 0xbd -
 *     ending at 0x200c, which is exactly fifteen, the bound the draw loop
 *     clamps to.
 *   0x0200a00c -> file offset 0x200c: four u16 words - 20, 10, 2, 1 - the coin
 *     payouts, indexed by a result the code separately special-cases at 4.
 * They abut the eight BLDALPHA halfwords at 0x1fc0 that Func_02000880 consumes.
 * `assets/code/resource_3b7_overlay.s` spells all of these literally in its
 * trailing data.  The other two addresses are outside the in-image band and so
 * are unshifted: 0x02000240 is the RAM workspace the byte-exact siblings
 * `assets/code/resource_3b7_c_02000044.c` and `..._020001a8.c` name, and
 * 0x03001c94 is the IWRAM key-state word (bit 0 and bit 1 are tested, i.e. A
 * and B).
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts` (an overlay
 * `bl` stores `target_offset - 2`).  In program order:
 *   0x02000adc Func_0808a018    0x02000bd2 Func_0808a070
 *   0x02000ae0 Func_0808a460    0x02000be8 Func_08015010
 *   0x02000af4 Func_080772e0    0x02000bf8 Func_08015080
 *   0x02000afc Func_0808a170    0x02000c04 Func_08015080
 *   0x02000b06 Func_0808a178    0x02000c0a Func_080000c0
 *   0x02000b16 Func_08015010    0x02000c10 Func_080f9010
 *   0x02000b26 Func_08015080    0x02000c1c Func_080000c0
 *   0x02000b36 Func_080150b0    0x02000c34 Func_080f9010
 *   0x02000b42 Func_08015080    0x02000c40 Func_08015018
 *   0x02000b52 Func_080150b0    0x02000c56 Func_08077230
 *   0x02000b58 Func_08015398    0x02000c64 Func_08077048
 *   0x02000b62 Func_08015018    0x02000c6a Func_0200173c  (prologue)
 *   0x02000b66 Func_08015140    0x02000c80 Func_08077230
 *   0x02000b94 Func_0808a170    0x02000c86 Func_080f9010
 *   0x02000b9e Func_0808a180    0x02000c8e Func_08015120
 *   0x02000ba4 Func_080000c0    0x02000c94 Func_0808a170
 *   0x02000bac Func_080f9010    0x02000c9e Func_0808a180
 *   0x02000bb4 Func_08077308    0x02000ca6 Func_080f9010
 *   0x02000bc0 Func_0808a170    0x02000cac Func_0808a010
 *   0x02000bca Func_0808a178    0x02000cd6 Func_080000f8
 *                               0x02000d14 Func_020004bc  (prologue)
 *                               0x02000d34 Func_0808a020
 * Completeness, as a multiset: Func_080f9010 x5, Func_0808a170 x4,
 * Func_08015080 x4, Func_080000c0 x3, Func_08015010 x2, Func_080150b0 x2,
 * Func_08015018 x2, Func_0808a178 x2, Func_0808a180 x2, Func_08077230 x2, and
 * one each of Func_0808a018, Func_0808a460, Func_080772e0, Func_08015398,
 * Func_08015140, Func_08077308, Func_0808a070, Func_08077048, Func_0200173c,
 * Func_08015120, Func_0808a010, Func_080000f8, Func_020004bc, Func_0808a020 -
 * 42 sites over 24 distinct targets, exactly the tool's
 * `sites=42 distinct_targets=24`.  The inventory row says calls=37, which is
 * the documented lower bound, not a discrepancy to chase.
 *
 * Func_0200173c and Func_020004bc are this overlay's own functions (converted
 * in semantic/overlays/resource_3b7_c_0200173c.c and the row at 0x020004bc).
 * The rest are imports; Func_0808a018 / Func_0808a020 are the established
 * cutscene bracket, Func_08015010 / Func_08015018 / Func_08015080 /
 * Func_080150b0 / Func_08015120 / Func_08015140 / Func_08015398 are the text
 * window family, and Func_080772e0 / Func_08077230 / Func_08077048 /
 * Func_08077308 are the inventory-and-coins family this overlay's other owners
 * also use.
 *
 * The epilogue is `add sp, #4 / pop {r3, r5, r6, r7} / ... / pop {r5, r6, r7} /
 * pop {r1} / bx r1`; the popped register is r1, so r0 survives and is the
 * result.  r0 is set to 0 immediately before it, so the owner returns the
 * constant 0 as s32.
 *
 * `goto` is used for the assembly's join points rather than duplicating the
 * shared tails; each label is one real join.
 *
 * Uncertainties:
 *  - r5, r6 and r7 each carry several unrelated lifetimes (r5 is in turn two
 *    line-id bases, the key-state pointer, an abort flag and the round result).
 *    They are split into separate variables here, per the HANDOVER warning
 *    about reassigned long-lived aliases.
 *  - Func_0808a460 at 0x02000ae0 and Func_08015140 at 0x02000b66 have no
 *    argument register set for them; r0 holds a leftover from the immediately
 *    preceding call in both cases.  They are spelled with no arguments.
 *  - The draw's running sum covers only `(round * 3) + 3` weights while the
 *    subtract loop is bounded at fifteen.  That is in the bytes; the loop
 *    cannot in practice run past the summed prefix because the pick is strictly
 *    below the sum, but nothing enforces it.
 *  - The `>> 1` write-back carries a sign correction (`lsrs r3, r3, #31`) that
 *    can never fire, because the branch above it requires the weight to exceed
 *    1.  It is written as a plain halving.
 *  - The imports' interfaces are unknown, hence the old-style declarations.
 */

#include "types.h"

extern s8 Data_02000240[];      /* RAM workspace; also indexed as s16 elsewhere */
extern s32 Data_02009fd0[];     /* in-image 0x1fd0: fifteen draw ids */
extern u16 Data_0200a00c[];     /* in-image 0x200c: four coin payouts */

s32 Func_080772e0();
s32 Func_08077308();
s32 Func_08015010();
s32 Func_08015398();
s32 Func_0808a070();
s32 Func_080000f8();
s32 Func_0200173c();
void Func_020004bc();
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a460();
void Func_08015018();
void Func_08015080();
void Func_080150b0();
void Func_08015120();
void Func_08015140();
void Func_080f9010();
void Func_080000c0();
void Func_08077230();
void Func_08077048();

s32 Func_02000ac8(void)
{
    s8 *weights = &Data_02000240[285];      /* the 0x11d offset, fifteen s8 weights */
    s32 choice = 0;
    s32 coins;
    s32 tickets;
    s32 window;
    s32 line;
    s32 aborted;
    s32 round;
    s32 index;
    s32 total;
    s32 pick;
    s32 weight;

    Func_0808a018();

    /* r0 still holds whatever the call above left; no argument is set. */
    Func_0808a460();

menu:
    coins = *(s32 *)&Data_02000240[16];
    tickets = Func_080772e0(229);

    Func_0808a170(0xe43);
    Func_0808a178(-1, 0);

    window = Func_08015010(0, 0, 17, 4, 2);
    Func_08015080(0xe49, window, 0, 0);
    Func_080150b0(coins, 6, window, 0x48, 0);
    Func_08015080(0xe4a, window, 0, 8);
    Func_080150b0(tickets, 6, window, 0x48, 8);

    /* The previous selection is passed back in as the initial cursor. */
    choice = Func_08015398(choice);

    Func_08015018(window, 2);
    Func_08015140();

    if (choice == -1) {
        goto done;
    }

    if (choice == 0) {
        if (coins != 0) {
            goto play;
        }
        line = 0xe44;
        goto say_and_retry;
    }

    if (choice == 1) {
        if (tickets == 0) {
            line = 0xe45;
            goto say_and_retry;
        }
        goto confirm_ticket;
    }
    goto play;

say_and_retry:
    Func_0808a170(line);
    Func_0808a180(-1, 0);
    Func_080000c0(1);
    goto menu;

input_accepted:
    Func_080f9010(0x70);
    aborted = 0;
    goto input_finished;

confirm_ticket:
    if (Func_08077308() == 0) {
        Func_0808a170(0xe47);
        Func_0808a178(-1, 0);
        if (Func_0808a070(0, 0) != 0) {
            goto done;
        }
    }

play:
    window = Func_08015010(20, 15, 9, 4, 2);
    Func_08015080(0xe4c, window, 0, 0);
    Func_08015080(0xe4d, window, 0, 8);
    Func_080000c0(5);
    Func_080f9010(0x74);
    goto input_check;

input_wait:
    Func_080000c0(1);

input_check:
    if ((*(volatile s32 *)0x03001c94 & 1) != 0) {       /* A */
        goto input_accepted;
    }
    if ((*(volatile s32 *)0x03001c94 & 2) == 0) {       /* B */
        goto input_wait;
    }

    Func_080f9010(0x71);
    aborted = -1;

input_finished:
    Func_08015018(window, 2);

    if (aborted == -1) {
        goto done;
    }

    if (choice == 0) {
        Func_08077230(-1);
    } else if (choice == 1) {
        Func_08077048(229);
    }

    round = Func_0200173c(choice);

    if (choice != 0) {
        goto weighted_draw;
    }
    if (round != 4) {
        Func_08077230(Data_0200a00c[round]);
        Func_080f9010(0x5b);
        Func_08015120(Data_0200a00c[round], 5);
        Func_0808a170(0xe46);
        Func_0808a180(-1, 0);
        goto done;
    }

    Func_080f9010(0x71);
    Func_0808a010(10);
    goto done;

weighted_draw:
    /* Weighted draw over the first (round * 3) + 3 weights. */
    total = 0;
    for (index = 0; index < round * 3 + 3; index++) {
        total += weights[index];
    }

    pick = (s32)((u32)(total * Func_080000f8()) >> 16);
    pick -= weights[0];

    index = 0;
    if (pick >= 0) {
        for (;;) {
            index++;
            if (index > 14) {
                break;
            }
            pick -= weights[index];
            if (pick < 0) {
                break;
            }
        }
    }
    if (index == 15) {
        index = 14;
    }

    Func_020004bc(Data_02009fd0[index]);

    weight = weights[index];
    if (weight > 1) {
        weights[index] = (s8)(weight / 2);
    }

done:
    Func_0808a020();
    return 0;
}
