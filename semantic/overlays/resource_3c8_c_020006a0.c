typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x020006a0: per-frame particle emitter callback.
 *
 * This is the routine the byte-exact sibling 0x02000754 installs: it writes
 * `object->callback = 0x020086a1`, which is this entry point plus the Thumb
 * bit at the overlay's 0x02008000 link base (0x020086a1 - 0x8000 - 1 =
 * 0x020006a0).  The same install appears in this reconstruction's 0x020007d8.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7, sl ; mov r6, r8 ; push {r6, r7}` at 0x020006a0, a 56-byte frame,
 * and the matching unwind at 0x0200073e..0x0200074a, followed by a two-word
 * literal pool at 0x0200074c-0x02000753.  No live frame or register state
 * escapes the row; the next prologue is at 0x02000754.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive and r0 is set to 0 immediately
 * before the unwind, on both paths, so the owner returns 0 - the same `s32`
 * callback shape as the byte-exact siblings 0x02000594 and 0x020005ac.
 *
 * The eight-argument spawn ABI is the one proven by the byte-exact sibling
 * 0x02000b98 (`Func_02000d52(x, y, z, dx, dy, dz, flags, &params)`): r0-r3
 * then [sp,#0], [sp,#4], [sp,#8], [sp,#12].  The parameter block sits at
 * sp+16 with the layout proven by 0x020009c8 and 0x02000b08.
 *
 * `params.mode` (offset 4) is never written by this owner, on either path.
 * Only `unk00`, `color1` and `color2` are set.  That is correct rather than
 * an omission: the spawner at 0x02000118 reads `mode` only when the flag word
 * carries bit 0x00010000, and the flag word used here (0x000a0000) does not.
 *
 * 0x03001e40 is the global phase counter the byte-exact sibling 0x020005ac
 * masks the same way.  It is read twice: once before the first spawn, whose
 * low nibble biases the emitter's x offset, and once after it, whose low
 * nibble gates the second spawn.  The second read is a fresh load, not a
 * reuse.
 *
 * The random result is scaled to `-(random8 * 0x3333)` by the shift/add chain
 * at 0x020006c2-0x020006d0 (x3, x17, x257 = x13107 = x0x3333) followed by
 * `negs`.  0x3333 is the same 16.16 fifth this package uses elsewhere.
 *
 * All four `bl` sites are accounted for: 0x02005470, 0x0200081a, 0x020054cc,
 * 0x02000852.  Import naming follows the note in
 * resource_3c8_c_020002f0.c - the two spawn addresses fall numerically inside
 * this overlay's own code region but are import identities, not locations.
 */

struct Object_020006a0 {
    u8 unknown_00[8];
    s32 x;      /* 0x08 */
    s32 y;      /* 0x0c */
    s32 z;      /* 0x10 */
};

struct EffectParams_020006a0 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

/* Random source; the byte-exact siblings declare this family `(void)`. */
s32 Func_02000118();
s32 Func_080000f8();

/* Old-style declarations: the imports' real interfaces are not known here. */

s32 Func_020006a0(struct Object_020006a0 *object)
{
    struct EffectParams_020006a0 params;
    s32 speed;
    s32 phase;

    params.color1 = 0x0000cccc;
    params.color2 = 0x0000cccc;
    params.unk00 = 0;

    speed = (s32)((u32)(Func_080000f8() * 8) >> 16) * 0x3333;
    phase = *(u32 *)0x03001e40 & 15;

    Func_02000118(
        object->x + ((8 - phase) << 16),
        object->y + 0x001a0000,
        object->z,
        0,
        -speed,
        0,
        0x000a0000,
        &params);

    /* Fresh read of the phase counter, after the spawn. */
    phase = *(u32 *)0x03001e40 & 15;
    if (phase == 0) {
        params.color1 = 0x00008000;
        params.color2 = 0x00008000;
        Func_080000f8(
            object->x
                + (((s32)((u32)(Func_02000118() * 9) >> 16) - 4) << 16),
            object->y,
            object->z,
            0,
            0,
            0,
            0x000a0000,
            &params);
    }

    return 0;
}
