#include "types.h"

/*
 * Resource 373: decide whether a follower has noticed its target.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp saves and the
 * 8-byte local frame at 0x0200564c, through the single `add sp, #8` epilogue
 * at 0x02005716..0x02005726, followed by its one-word literal pool at
 * 0x02005728.  Every path — including the early "already alerted" exit —
 * converges on that epilogue.
 *
 * All five calls are placed.  None is an interworking `call_via rN` site.
 *
 * The routine answers 1 when the follower is alerted on this call and 0 when
 * it is cleared; the early exit answers 1 without touching the flags again.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image (shared 0x02000000 namespace, load-time fixups).
 * UNCERTAINTY 2: 0xfb94f006 is the literal pool word added to the bearing
 * before the 0xf000 mask.  Only bits 12..15 of the sum survive, so it acts as
 * a -0x0ffa bias on the 16-bit bearing, i.e. it selects the neighbouring
 * facing sector.  It is spelled as the pool word rather than as the derived
 * bias because that is what the reference materialises.
 */

struct Resource373Follower {
    u8 unknown_00[6];
    u16 facing;                     /* top nibble is the facing sector */
    s32 x;                          /* 0x08 */
    u8 unknown_0c[4];
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x47];
    u8 alerted;                     /* 0x5b */
    u8 unknown_5c[6];
    u8 alertLatch;                  /* 0x62 */
};

s32 Func_02005610(const s32 *targetPosition, const s32 *followerPosition);
s32 Func_08000100(s32 dz, s32 dx);
void Func_08009080(struct Resource373Follower *follower, s32 mode);

s32 Func_0200564c(struct Resource373Follower *follower,
                  struct Resource373Follower *target,
                  s32 noticeRange,
                  s32 force)
{
    s32 result = 0;
    s32 distance;

    if (follower->alerted == 1 && follower->alertLatch == 0) {
        Func_08009080(follower, 1);
        return 1;
    }

    distance = Func_02005610(&target->x, &follower->x);
    if (distance < noticeRange || force != 0) {
        s32 bearing = Func_08000100(target->z - follower->z,
                                    target->x - follower->x);
        s32 sector = (s32)(u16)bearing;   /* lsls #16 ; lsrs #16 */
        s32 previous;
        s32 next;
        s32 facing;

        sector = sector & 0xf000;
        previous = (sector + (s32)0xfb94f006) & 0xf000;
        next = (sector + 0x1000) & 0xf000;
        facing = follower->facing & 0xf000;

        if (sector == facing || next == facing || previous == facing ||
            force != 0) {
            follower->alerted = 1;
            Func_08009080(follower, 1);
            result = 1;
            follower->alertLatch = 1;
            return result;
        }
    }

    follower->alerted = (u8)force;
    Func_08009080(follower, 2);
    follower->alertLatch = (u8)force;
    return result;
}
