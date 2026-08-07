#include "types.h"

/*
 * Resource 38b door/collision update at 0x020009cc (274 bytes, 16 call
 * sites over 9 distinct callees).
 *
 * Complete owner: prologue `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r9 /
 * mov r5, r8 / push {r5, r6, r7} / sub sp, #8` at 0x020009cc, and the exactly
 * matching epilogue at 0x02000ace which restores sl, r9, r8 before
 * `pop {r5, r6, r7} / pop {r0} / bx r0`.  The popped r0 is the return
 * address, so the owner is `void`.  The span carries NO literal pool — every
 * constant is built with `movs` + `lsls` — so there is no pool map to derive
 * and nothing in the range is data.
 *
 * Called from 0x020008f0.
 *
 * COMPLETENESS, per target: Func_080091c0 x5, Func_0808a080 x2,
 * Func_0808a0c0 x2, Func_0808a1b8 x2, Func_0808a018 x1, Func_0808a1e8 x1,
 * Func_0808a090 x1, Func_0808a128 x1, Func_0808a020 x1 = 16 sites, matching
 * the inventory's `calls=16`, and each appears exactly that many times below.
 * Every site has its own `bl`; no site is shared between arms, so none of the
 * inflating shared-call-site shapes applies.  The two Func_0808a0c0 /
 * Func_0808a1b8 pairs are separate sites on opposite arms of one test and are
 * deliberately NOT merged.
 *
 * All 16 targets are import veneers resolved with
 * tools/lib/overlay_call_targets.ts.
 *
 * `asrs #20` on the +0x08 and +0x10 record words is the tile-grid idiom
 * (>> 16 to pixels, >> 4 to the 16-pixel grid), signed.
 *
 * REGISTER NOTE.  r5 holds the slot-0 record only until 0x020009fa, where
 * `movs r5, #15` reuses it as the constant 15 for the stacked tile-x of the
 * first three repaints.  Nothing is lost: the player's tile coordinates have
 * already been captured into r9 and sl.  This is the "reassigned long-lived
 * alias" shape HANDOVER section 0 warns about — r5 must be tracked per use,
 * not as one variable.
 */

/* Slot accessor. */


/* Six-argument renderer ABI: four register arguments plus tile x, tile z. */


/* Imports; old-style, arity open. */








extern u8 * Func_02001aac();
extern u8 * Func_02001ab4();
extern void Func_02001a82(s32, s32, s32, s32, s32, s32);
extern void Func_02001a96(s32, s32, s32, s32, s32, s32);
extern void Func_02001aa8(s32, s32, s32, s32, s32, s32);
extern void Func_02001aba(s32, s32, s32, s32, s32, s32);
extern void Func_02001ad6(s32, s32, s32, s32, s32, s32);
extern void Func_02001b26();
extern void Func_02001bda();
extern void Func_02001b60();
extern void Func_02001baa();
extern void Func_02001b82();
extern void Func_02001bfe();
extern void Func_02001b9c();
extern void Func_02001c18();
extern void Func_02001b8c();
void Func_020009cc(void)
{
    u8 *player;
    u8 *mover;
    s32 player_x;
    s32 player_z;
    s32 mover_x;
    s32 mover_z;

    player = Func_02001aac(0);
    mover = Func_02001ab4(20);

    mover_z = *(s32 *)(mover + 0x10) >> 20;
    player_x = *(s32 *)(player + 0x08) >> 20;
    player_z = *(s32 *)(player + 0x10) >> 20;
    mover_x = *(s32 *)(mover + 0x08) >> 20;

    /* Vertical strip of three cells at column 15, rows 12..14. */
    Func_02001a82(15, 11, 3, 1, 15, 12);
    Func_02001a96(15, 11, 3, 1, 15, 13);
    Func_02001aa8(15, 11, 3, 1, 15, 14);

    /* Repaint the moving actor's own cell. */
    Func_02001aba(1, 0, 1, 1, mover_x, mover_z);

    /* Restore cell (16, 13) unless the mover is standing on it. */
    if (!(mover_x == 16 && mover_z == 13)) {
        Func_02001ad6(0, 0, 1, 1, 16, 13);
    }

    /* The player reaching (16, 13) triggers the camera/scene change. */
    if (player_x == 16 && player_z == 13) {
        Func_02001b26();

        /* 128 << 1 = 256. */
        Func_02001bda(0, 256, 20);
        /* 128 << 10 = 0x20000, 128 << 9 = 0x10000. */
        Func_02001b60(0, 0x20000, 0x10000);
        Func_02001baa(0, 6, 0);

        if (mover_z == 13) {
            /* 131 << 1 = 262; 128 << 7 = 0x4000. */
            Func_02001b82(0, 262, 196);
            Func_02001bfe(0, 0x4000, 20);
        } else {
            /* 143 << 1 = 286; 128 << 8 = 0x8000. */
            Func_02001b9c(0, 286, 218);
            Func_02001c18(0, 0x8000, 20);
        }

        Func_02001b8c();
    }
}
