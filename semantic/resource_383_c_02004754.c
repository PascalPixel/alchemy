#include "types.h"

/*
 * resource_383 owner at 0x02004754, 104 bytes (0x02004754-0x020047bb):
 * code 0x02004754-0x020047b5, two bytes of alignment at 0x020047b6, and a
 * one-word literal pool at 0x020047b8 (0x0200cf2c).
 *
 * There are no calls at all in this owner (the inventory's calls=0 is
 * confirmed by cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- reporting zero sites).
 *
 * LINK BASE.  The pool word 0x0200cf2c is an in-image address under this
 * overlay's 0x02008000 link base: file offset 0x4f2c is the first byte after
 * the last import veneer (0x02004f24 + 8), and a 16-byte-per-entry record
 * table starts exactly there - which is precisely how the code below indexes
 * it (`lsls #4` then add).  That makes this owner the cheapest witness in
 * resource_383 for the base already proven on six other overlays, so
 * `Data_0200cf2c` is in-image data, not a RAM global.
 *
 * BEHAVIOUR.  Three four-byte candidate records live at `options + 4`; each
 * holds an id byte at +0 (0xff = empty slot) and a heading byte at +1.  The
 * heading byte is promoted to a halfword angle by `<< 8`, and the candidate
 * whose angle is closest to the current heading `*facing` - measured as the
 * absolute value of the s16 difference, so the comparison wraps correctly
 * through the angle's full turn - wins, provided it beats the initial bound
 * 0x8000 (half a turn).  On a win the chosen angle is written back through
 * `facing` and the record for its id is returned; otherwise 0 is returned and
 * `*facing` is left alone.
 *
 * Epilogue is `pop {r5, r6, r7} / pop {r1} / bx r1`, so r0 survives and is the
 * result.
 */

/* In-image table at file offset 0x4f2c, sixteen bytes per entry. */
extern u8 Data_0200cf2c[];

u8 *Func_02004754(u8 *options, s16 *facing)
{
    u8 *p;
    s32 best_id;
    s32 best_dist;
    s32 best_angle;
    s32 target;
    s32 dist;
    s32 angle;
    u32 i;

    best_id = -1;
    /* Read twice, exactly as the assembly does: unsigned for the difference
     * and sign-extended for the fallback value of the write-back register. */
    best_angle = *facing;
    target = *(u16 *)facing;

    best_dist = 0x8000;
    p = options + 4;

    for (i = 0; i < 3; i++) {
        p += 4;
        angle = p[1] << 8;
        dist = (s16)(angle - target);
        if (dist < 0) {
            dist = -dist;
        }
        if (p[0] != 0xff && dist < best_dist) {
            best_dist = dist;
            best_angle = (s16)angle;
            best_id = p[0];
        }
    }

    if (best_id == -1) {
        return 0;
    }
    *facing = (s16)best_angle;
    return Data_0200cf2c + (best_id << 4);
}
