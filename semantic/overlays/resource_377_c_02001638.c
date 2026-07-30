typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_377 owner at 0x02001638, 82 bytes: per-frame step of a 32-frame
 * animation.  Complete owner: 'push {r5, r6, lr}' and the interworking return
 * 'pop {r5, r6} ; pop {r0} ; bx r0'.  r0 holds the popped return address, so the
 * owner returns nothing.
 *
 * TRANSPOSED from semantic/overlays/resource_371_c_02004004.c.  The two owners are
 * the same routine shared verbatim: over all 41 halfwords they differ in exactly
 * 4 places, all of them BL halfwords.  No pool word differs.
 *
 * What was changed: the entry symbol, and both calls, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 1638' to the veneers publishing
 * Func_08000118 (the hand-off taken once the counter passes 31) and
 * Func_080090d0 (the angle-to-amplitude helper).  resource_371's own sites
 * resolve to the same two imports; the 371 source predates the corrected 'bl'
 * rule and named them by their encoded displacements, which also made the two
 * sibling owners look as though they called different helpers.  They do not.
 *
 * The halfword counter at +0x64 is incremented in place, then sign-extended
 * ('lsls #16 ; asrs #16') before the limit test, so the comparison is signed
 * 16-bit.  +0x68 points at the anchor object whose X and Z this owner tracks;
 * it is loaded unconditionally, before the limit test.  'lsls r0,r0,#10' turns
 * the 0..31 frame into a 0..0x7c00 angle.  Field layout is the one proven by the
 * byte-exact sibling assets/code/resource_371_c_020004a0.c.
 */

struct Actor_02004004 {
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
    struct Actor_02004004 *anchor;  /* 0x68 */
};

void Func_08000118();
s32 Func_080090d0();

void Func_02001638(struct Actor_02004004 *self)
{
    struct Actor_02004004 *anchor;
    s32 frame;
    s32 amplitude;

    self->frame = (u16)(self->frame + 1);
    anchor = self->anchor;
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_08000118(self);
        return;
    }

    amplitude = Func_080090d0(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = -amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z - (0x10000 - amplitude) * 5 + 0x100000;
}
