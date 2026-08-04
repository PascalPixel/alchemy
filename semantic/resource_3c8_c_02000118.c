typedef signed int s32;
typedef signed short s16;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02000118: the package's particle spawner.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x02000118, an 8-byte frame, and the matching
 * unwind at 0x020002d2..0x020002e2, followed by a three-word literal pool at
 * 0x020002e4-0x020002ef that ends the row.  No live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  Both early exits branch to that same epilogue.
 *
 * SIGNATURE.  This is the eight-argument spawn ABI used all over this
 * package: r0-r3 then four incoming stack words.  Eight registers are pushed
 * (32 bytes) and the frame is 8 more, so [sp,#40], [sp,#44], [sp,#48] and
 * [sp,#52] are arguments five to eight.  Cross-checked against the byte-exact
 * callers, which spell the same ABI as
 * `Func_02000d52(x, y, z, dx, dy, dz, flags, &params)` (0x02000b98) and
 * `Func_02000b9a(object->x, object->y, object->z, vec.x, vec.y, vec.z,
 * 0x01090001, &params)` (0x020009c8).
 *
 * WHAT THE FLAG WORD SELECTS.  The low nibble indexes the pointer table
 * `Data_0200d1d4`; `(flags + 1) & 15` is a second, independent selector.  The
 * high half is a bit set, and every bit was confirmed against a byte-exact or
 * semantic caller in this same overlay:
 *
 *   0x00010000  pass `params->mode` to 0x02005132   (0x020009c8, 0x02000b08)
 *   0x00020000  clear bit 0 of `object->flags23` and copy `params->unk00 & 3`
 *               into bits 2-3 of the sprite's byte at +9   (0x02000b08)
 *   0x00080000  copy `params->color1/color2` into +0x18/+0x1c   (0x02000b98)
 *   0x00040000  derive +0x30/+0x34 from `params->unk10/unk14`   (0x02000b98)
 *   0x00100000  take the effect id from `(s16)params->id24`, else use 222
 *               (0x02000b98 sets that field to 286)
 *   0x00200000  0x02005068(object, 1) then 0x02005078(object, params->unk1c)
 *   0x00400000  copy `params->unk20` into the sprite halfword at +30
 *   0x00800000  copy `params->unk22` into the object halfword at +100
 *   0x01000000  install `params->callback` in +0x6c   (0x020009c8, 0x02000b08)
 *
 * That table also resolves an open question left in
 * semantic/overlays/resource_3c8_c_020006a0.c: that caller never writes
 * `params.mode`, and it also never sets bit 0x00010000, so the stale slot is
 * never read.
 *
 * The record built here is exactly the one that
 * semantic/overlays/resource_3c8_c_0200096c.c integrates per frame: +0x44,
 * +0x48, +0x4c are the velocity triple it adds into +0x08/+0x0c/+0x10,
 * +0x30/+0x34 are the rates it adds into +0x18/+0x1c, +0x50 is the sprite
 * whose halfword at +30 it advances, and +100 is the halfword it advances it
 * by.  The installed `object->callback = 0x020080e1` is the routine at file
 * offset 0x00e0 - at this overlay's proven 0x02008000 link base,
 * 0x020080e1 - 0x8000 - 1 = 0x020000e0, whose byte-exact source is
 * `assets/code/resource_3c8_c_020000e0.c`.
 *
 * The object returned by 0x02004fe6 is used only as a template: bits 2-3 of
 * its sprite's byte at +9 are copied onto the new sprite.
 *
 * Constants are built as `movs r3, #128 ; lsls r3, #n`: 0x100000 (<<13),
 * 0x10000 (<<9), 0x20000 (<<10), 0x40000 (<<11), 0x80000 (<<12), 0x200000
 * (<<14), 0x400000 (<<15), 0x800000 (<<16), 0x1000000 (<<17).  0xffff0000 is
 * pooled and used both as a mask and, in the else arm of the 0x40000 block,
 * as the addend -0x10000.
 *
 * All ten `bl` sites are accounted for: 0x02004fe6, 0x02004f44, 0x02004f4e,
 * 0x02004f68, 0x02005132, 0x02004ff4, 0x0200500c, 0x0200501a, 0x02005068,
 * 0x02005078.  0x0200501a is a shared tail reached from both arms of the
 * 0x40000 block; it is declared, not defined.  Import naming follows the note
 * in resource_3c8_c_020002f0.c.
 */

struct Sprite_02000118 {
    u8 unknown_00[9];
    u8 flags9;                  /* 0x09 - bits 2-3 are copied here */
    u8 unknown_0a[0x14];
    u16 angle;                  /* 0x1e = 30 - the halfword
                                 * resource_3c8_c_0200096c.c advances */
    u8 unknown_20[6];
    u8 state26;                 /* 0x26 */
};

struct Particle_02000118 {
    u8 unknown_00[0x18];
    s32 accum18;                /* 0x18 */
    s32 accum1c;                /* 0x1c */
    u8 unknown_20[3];
    u8 flags23;                 /* 0x23 */
    u8 unknown_24[0x0c];
    s32 rate30;                 /* 0x30 */
    s32 rate34;                 /* 0x34 */
    u8 unknown_38[0x0c];
    s32 velocityX;              /* 0x44 */
    s32 velocityY;              /* 0x48 */
    s32 velocityZ;              /* 0x4c */
    struct Sprite_02000118 *sprite;  /* 0x50 */
    u8 unknown_54[1];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[0x0e];
    u16 step64;                 /* 0x64 = 100 */
    u8 unknown_66[6];
    s32 callback;               /* 0x6c */
};

struct EffectParams_02000118 {
    s32 unk00;                  /* 0x00 - low 2 bits into the sprite */
    s32 mode;                   /* 0x04 */
    s32 color1;                 /* 0x08 */
    s32 color2;                 /* 0x0c */
    s32 unk10;                  /* 0x10 */
    s32 unk14;                  /* 0x14 */
    s16 id24;                   /* 0x18 = 24 */
    u8 unknown_1a[2];
    s32 unk1c;                  /* 0x1c = 28 */
    u16 unk20;                  /* 0x20 = 32 */
    u16 unk22;                  /* 0x22 = 34 */
    s32 callback;               /* 0x24 = 36 */
};

struct Selector_02000118 {
    u8 unknown_00[12];
    s32 unkC;                   /* 0x0c */
};

/* In-image pointer table, indexed by the flag word's low nibble. */
extern struct Selector_02000118 *Data_0200d1d4[];

/* Used for their return values. */
s32 Func_03000380();
void Func_08009080();
void Func_08009098();
struct Particle_02000118 *Func_080090c8();
struct Particle_02000118 *Func_0808a080();
void Func_0808a160();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02000118(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct EffectParams_02000118 *params)
{
    struct Particle_02000118 *template;
    struct Particle_02000118 *object;
    struct Sprite_02000118 *sprite;
    struct Selector_02000118 *selector;
    s32 id;
    s32 delta;

    template = Func_0808a080(0);

    if ((flags & 0x00100000) != 0 && params != 0) {
        id = params->id24;
    } else {
        id = 222;
    }

    object = Func_080090c8(id, x, y, z);
    if (object == 0) {
        return;
    }
    sprite = object->sprite;

    Func_08009080(object, (flags + 1) & 15);
    Func_08009098(object, Data_0200d1d4[flags & 15]);

    object->mode55 = 0;
    sprite->state26 = 0;
    object->callback = 0x020080e1;      /* = Func_020000e0 | 1 */
    object->velocityX = velocityX;
    object->velocityY = velocityY;
    object->velocityZ = velocityZ;

    /* Bits 2-3 of the sprite flag byte come from the template object. */
    sprite->flags9 = (u8)((sprite->flags9 & ~12) | (template->sprite->flags9 & 12));

    object->rate30 = 0;
    object->rate34 = 0;
    object->step64 = 0;

    if ((flags & 0xffff0000) == 0 || params == 0) {
        return;
    }

    if ((flags & 0x00010000) != 0) {
        Func_0808a160(object, params->mode);
    }

    if ((flags & 0x00020000) != 0) {
        object->flags23 &= 0xfe;
        sprite->flags9 = (u8)((sprite->flags9 & ~12)
                              | ((params->unk00 & 3) << 2));
    }

    if ((flags & 0x00080000) != 0) {
        object->accum18 = params->color1;
        object->accum1c = params->color2;
    }

    if ((flags & 0x00040000) != 0) {
        selector = Data_0200d1d4[flags & 15];
        if ((flags & 0x00080000) != 0) {
            object->rate30 = Func_03000380(params->unk10 - object->accum18,
                                           selector->unkC);
            delta = params->unk14 - object->accum1c;
        } else {
            object->rate30 = Func_03000380(params->unk10 + 0xffff0000,
                                           selector->unkC);
            delta = params->unk14 + 0xffff0000;
        }
        object->rate34 = Func_03000380(delta, selector->unkC);
    }

    if ((flags & 0x00200000) != 0) {
        Func_08009080(object, 1);
        Func_08009098(object, params->unk1c);
    }

    if ((flags & 0x00400000) != 0) {
        sprite->angle = params->unk20;
    }

    if ((flags & 0x00800000) != 0) {
        object->step64 = params->unk22;
    }

    if ((flags & 0x01000000) != 0) {
        object->callback = params->callback;
    }
}
