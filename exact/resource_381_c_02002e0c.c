#include "types.h"

/*
 * Resource 381 owner at 0x02002e0c (80 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` and the interworking return
 * `pop {r5, r6} ; pop {r0} ; bx r0`.  r0 holds the popped return address, so
 * the owner returns nothing.  No literal pool.
 *
 * The twin of the byte-exact `assets/code/resource_371_c_02003fb4.c`, which
 * `overlay twins` names as its group source: same 32-frame counter at +0x64,
 * same anchor pointer at +0x68, same field layout, same two calls in the same
 * places.  Only the import addresses differ, because each overlay reaches its
 * imports through its own veneer table.
 *
 * Its own mirror inside this overlay is 0x02002e5c, exactly as 0x02003fb4 and
 * 0x02004004 mirror each other in resource_371: this one stores the amplitude
 * unnegated, ADDS the Z offset, and biases Z by 0x80000, where 0x02002e5c
 * negates, subtracts and biases by 0x100000.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a load-time-relocated
 * import, not a place to disassemble and not a global identity.  Func_02006284
 * is the escape call here and the magnitude call in 0x02002e5c, which is what
 * that convention means in practice -- the number names a site, not a function.
 * Old-style declarations, because the interfaces are unknown.
 */

struct Actor_02002e0c {
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
    struct Actor_02002e0c *anchor;  /* 0x68 */
};

void Func_02006284();
s32 Func_02006234();

void Func_02002e0c(struct Actor_02002e0c *self)
{
    struct Actor_02002e0c *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_02006284(self);
        return;
    }

    amplitude = Func_02006234(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}
