#include "types.h"

/*
 * Resource 381 owner at 0x02002e5c (82 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` and the interworking return
 * `pop {r5, r6} ; pop {r0} ; bx r0`.  r0 holds the popped return address, so
 * the owner returns nothing.  No literal pool.
 *
 * The twin of the byte-exact `assets/code/resource_371_c_02004004.c`, which
 * `overlay twins` names as its group source: same 32-frame counter at +0x64,
 * same anchor pointer at +0x68, same field layout, same two calls in the same
 * places.  Only the import addresses differ, because each overlay reaches its
 * imports through its own veneer table.  This file is that sibling's shape with
 * this overlay's call sites, which is what the twin relation is for.
 *
 * The mirrored sibling documents the reading of the arithmetic: one angle in,
 * one 16.16 magnitude out of Func_02006284, the second amplitude store negated,
 * the Z offset subtracted, and a 0x100000 Z bias.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a load-time-relocated
 * import, not a place to disassemble and not a global identity.  Old-style
 * declarations, because the interfaces are unknown.
 */

struct Actor_02002e5c {
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
    struct Actor_02002e5c *anchor;  /* 0x68 */
};

void Func_020062d4();
s32 Func_02006284();

void Func_02002e5c(struct Actor_02002e5c *self)
{
    struct Actor_02002e5c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_020062d4(self);
        return;
    }

    amplitude = Func_02006284(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = -amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z - (0x10000 - amplitude) * 5 + 0x100000;
}
