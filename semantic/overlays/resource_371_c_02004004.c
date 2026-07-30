typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x02004004 (82 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02004004 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02004050.  r0 holds the
 * popped return address, so the owner returns nothing.  No literal pool.
 *
 * The mirror image of 0x02003fb4: same 32-frame counter at +0x64, same
 * anchor pointer at +0x68, same field layout (proven by the byte-exact
 * sibling `assets/code/resource_371_c_020004a0.c`).  Three differences, all
 * of them sign flips or a different constant: the second amplitude store is
 * negated (`negs r3,r0`), the Z offset is subtracted rather than added, and
 * the fixed Z bias is 0x100000 (`movs #128 ; lsls #13`) rather than 0x80000.
 *
 * UNCERTAINTY: 0x020081cc, like 0x0200817c in the mirrored owner, is
 * identified only by use — one angle in, one 16.16 magnitude out.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style declarations, because the interfaces are unknown.
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

void Func_02008234();
s32 Func_020081cc();

void Func_02004004(struct Actor_02004004 *self)
{
    struct Actor_02004004 *anchor;
    s32 frame;
    s32 amplitude;

    self->frame = (u16)(self->frame + 1);
    anchor = self->anchor;
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_02008234(self);
        return;
    }

    amplitude = Func_020081cc(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = -amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z - (0x10000 - amplitude) * 5 + 0x100000;
}
