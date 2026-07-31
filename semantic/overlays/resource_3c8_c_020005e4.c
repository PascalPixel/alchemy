typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x020005e4: per-frame callback that spawns one randomised
 * particle above a scene object, on the frames the global phase counter
 * allows.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7, sl ; mov r6, r8 ; push {r6, r7}` at 0x020005e4, a 56-byte frame,
 * and the matching unwind at 0x0200067a..0x02000686 (`add sp, #56 ;
 * pop {r3, r5} ; mov r8, r3 ; mov sl, r5 ; pop {r5, r6, r7} ; pop {r1} ;
 * bx r1`), followed by a two-word literal pool at 0x02000688-0x0200068f.  No
 * live frame or register state escapes the row; the next prologue is at
 * 0x02000690.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive and r0 is set to 0 on the
 * instruction before the unwind, on both paths, so the owner returns 0 - the
 * same `s32` callback shape as the byte-exact siblings 0x02000594,
 * 0x02000690 and 0x020005ac.
 *
 * The gate `*(u32 *)0x03001e40 & 7` is the same phase test the byte-exact
 * sibling 0x020005ac uses.  Its result stays in r7 for the whole body and is
 * spilled to [sp,#4] as the sixth argument of the spawn call; on that path it
 * is provably 0, and it is spelled 0 here.
 *
 * The four calls at 0x020053b2/0x020053d0/0x020053e4/0x020053f8 are the
 * package's random source: each result is scaled by a small odd multiplier
 * built from shift-and-add and then reduced by an unsigned `>> 16`, exactly
 * the `(u32)(Func_02005966() * 6) >> 16` idiom in the byte-exact sibling
 * 0x02000b98, whose random imports are declared `(void)`.  No argument
 * register is written before any of them.
 *
 * The eight-argument spawn call at 0x0200078e uses the ABI proven by
 * 0x02000b98's `Func_02000d52(x, y, z, dx, dy, dz, flags, &params)`:
 * r0-r3 then [sp,#0], [sp,#4], [sp,#8], [sp,#12].  The direction triple here
 * is (0, scaledRandom, 0) and the parameter block lives at sp+16 with the
 * layout proven by 0x020009c8 and 0x02000b08 (unk00, mode, color1, color2 at
 * offsets 0, 4, 8, 12).  Only those four fields are written; the rest of the
 * 40-byte block is left as the caller found it, which is an observable
 * uncertainty carried from the original.
 *
 * All six `bl` sites are accounted for.  Import naming follows the note in
 * resource_3c8_c_020002f0.c: 0x0200078e falls numerically inside this
 * overlay's own code region but is an import identity, not a location.
 */

struct Object_020005e4 {
    u8 unknown_00[8];
    s32 x;      /* 0x08 */
    s32 y;      /* 0x0c */
    s32 z;      /* 0x10 */
};

struct EffectParams_020005e4 {
    s32 unk00;
    s32 mode;
    s32 color1;
    s32 color2;
    u8 unknown_10[0x18];
};

/* Random source; the byte-exact siblings declare this family `(void)`. */
void Func_02000118();
s32 Func_03000380();
s32 Func_080000f8();

/* Old-style declarations: the imports' real interfaces are not known here. */

s32 Func_020005e4(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase;
    s32 x;
    s32 y;
    s32 speed;

    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) {
        return 0;
    }

    params.unk00 = 3 - (s32)((u32)(Func_080000f8() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;

    x = object->x + (((s32)((u32)(Func_080000f8() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Func_080000f8() * 32) >> 16)) << 16);
    speed = Func_080000f8(
        ((s32)((u32)(Func_03000380() * 5) >> 16) << 16) + 0x00050000,
        10);

    Func_02000118(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
    return 0;
}
