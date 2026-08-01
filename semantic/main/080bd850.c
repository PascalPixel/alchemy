/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact reconstruction can settle
 * it, and the type below says only what this call site proves.
 *
 * Specific to this file: r2 is provably NOT set before the `bl` at
 * 0x080bd87e -- r0 takes the destination, r1 the pixel count, r3 the
 * callee, and nothing writes r2 in the whole function.  So this caller
 * passes two arguments where several others pass three.  Either the
 * routine takes two and the three-argument callers pass a harmless extra,
 * or it takes three and this site is relying on a stale register.  Not
 * resolved here; typed as two because that is what the ROM sets.
 */
#include "types.h"

typedef void (*Transfer_080bd850)(void *, u32);

typedef void (*Resident_03000164)(void *destination, u32 size);

struct Sprite_080bd850 {
    u8 unknown_00[28];
    u8 shape;
    u8 unknown_1d[3];
    u8 width;
    u8 height;
};

struct ShapeEntry_080bd850 {
    u16 unknown_00;
    u16 tile_offset;
};

extern struct ShapeEntry_080bd850 Data_03001b10[];


/*
 * Upload one sprite's tiles.  The shape index selects a tile offset from the
 * runtime shape table, which places the destination inside the second object
 * VRAM bank, and the sprite's width times its height gives the byte count.
 * The transfer itself is the IWRAM-relocated ARM helper at 0x03000164, reached
 * through the call-via-r3 slot.
 * Called from 0x080bdf36.
 */
void Func_080bd850(struct Sprite_080bd850 *sprite)
{
    void *destination =
        (void *)(0x06010000 + Data_03001b10[sprite->shape].tile_offset);

    ((Resident_03000164)0x03000164)(
        destination, (u32)(sprite->width * sprite->height));
}
