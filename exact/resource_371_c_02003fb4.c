#include "types.h"

/*
 * Resource 371 owner at 0x02003fb4 (80 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003fb4 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02003ffe.  r0 holds the
 * popped return address, so the owner returns nothing.  No literal pool: all
 * constants are built with `movs`/`lsls`.
 *
 * Per-frame step of a 32-frame animation.  The halfword counter at +0x64 is
 * incremented in place, then sign-extended (`lsls #16 ; asrs #16`) before the
 * limit test, so the comparison is signed 16-bit.  Once it passes 31 the
 * owner hands off to the import at 0x020081e4 and does nothing else.
 *
 * The field layout is the one proven by the byte-exact sibling
 * `assets/code/resource_371_c_020004a0.c`: +0x08 is X and +0x10 is Z, with
 * +0x0c between them.  +0x68 is a pointer to the anchor object whose X and Z
 * this owner tracks; it is loaded unconditionally at 0x02003fc0, before the
 * limit test, and is a plain load with no side effect.
 *
 * `lsls r0,r0,#10` turns the 0..31 frame into a 0..0x7c00 angle, and the
 * import at 0x0200817c maps it to the 16.16 amplitude stored into both +0x18
 * and +0x1c.  The Z offset is `(0x10000 - amplitude) * 5` built as
 * `lsls #2 ; adds`, plus a fixed 0x80000.
 *
 * UNCERTAINTY: 0x0200817c is identified only by its use — one angle in, one
 * 16.16 magnitude out.  The mirrored owner at 0x02004004 calls 0x020081cc
 * the same way and negates the second store, which is consistent with a
 * sine/cosine pair, but neither import is resolved here.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown.
 */

struct Actor_02003fb4 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor_02003fb4 *anchor;  /* 0x68 */
};

void Func_020081e4();
s32 Func_0200817c();

void Func_02003fb4(struct Actor_02003fb4 *self)
{
    struct Actor_02003fb4 *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_020081e4(self);
        return;
    }

    amplitude = Func_0200817c(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}
