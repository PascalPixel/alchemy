#include "types.h"

/*
 * resource_3a4 owner at 0x02002be0, 256 bytes: a per-frame emitter. On
 * every third frame, spawn one presentation record at a randomised
 * horizontal offset, optionally play one of two sounds, then stamp it with
 * six randomised or fixed fields and hand it to the animation and
 * descriptor primitives.
 *
 * A PUBLISHED CALLBACK (sweep B): nothing in the image `bl`s to it, so
 * neither a whole-image BL sweep nor overlay_unindexed can see it. It has
 * the shape resource_3a4_c_02002bb0.c installs -- a void, no-argument
 * per-frame task -- and it takes no argument in fact: r0 is dead on entry,
 * loaded from the frame counter before its first use.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002be0 through the
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02002cbc-
 * 0x02002cc0, one alignment halfword, and the seven-word literal pool
 * 0x02002cc4-0x02002cdf; the next owner is the already-drafted 0x02002ce0.
 * measureSpan reports 226, undersizing by the pool's 30 bytes.
 *
 * Thirteen call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x2be0 0x2ce0` and none
 * read from the listing, which prints thirteen nonexistent 0x02006xxx
 * addresses. They collapse onto seven veneers:
 *   0x2002be8, 0x2002c18                                     -> Func_030003e0   the established IWRAM helper
 *   0x2002bf0, 0x2002c20, 0x2002c42, 0x2002c92, 0x2002caa    -> Func_080000f8   the established RNG
 *   0x2002c0a                                                -> Func_080090c8   spawns and RETURNS the record, may return zero
 *   0x2002c2e, 0x2002c36                                     -> Func_080f9010   established, a sound id
 *   0x2002c62                                                -> Func_080091e0(record, 0)
 *   0x2002c86                                                -> Func_08009080(record, 1)
 *   0x2002c8e                                                -> Func_08009098(record, Data_0200c01c)
 * Five bit-identical `f003 fd47` encodings for the RNG and two identical
 * `f003 fe8f` for the sound call; the listing gives each a different
 * address.
 *
 * Func_030003e0's role read from its two uses here. Its first argument is
 * the frame counter at 0x03001e40 -- the free-running word the tree
 * already masks with 15, 8, 4, 3 and 1 -- and the results gate "run at
 * all" on 3 and "play a sound" on 9. Together with the corpus's
 * `Func_030003e0(Func_080000f8(), 90) + 60` producing a 60..149 range,
 * that reads as a remainder helper. Read, not named: the established
 * spelling is kept and no new name is minted.
 *
 * Field roles are recorded by offset. +24 and +28 both take the same
 * randomised 0x4ccc-based value, which is exactly the constant
 * resource_3a4_c_02002a10.c stamps into those two fields flat -- the two
 * rows corroborate that this pair is one quantity, not two. +72 takes the
 * fixed 0x6666, byte +85 is cleared and byte +97 set to 1 the way
 * 0x02002a10 clears +0x55, byte +35 has bit 0 cleared, and the handle at
 * +80 gets the same `(flags09 & ~0x0c) | 0x04` update 0x02002a10 performs
 * -- the same field, reached the same way, in two independent rows.
 * +36, +40 and +44 close it with two more randomised values and 0x80000.
 *
 * Data_0200c01c is image offset 0x401c, spelled by its linked address per
 * this overlay's existing files. The call passes the ADDRESS -- the pool
 * word is loaded into r1 and used directly, never dereferenced -- so it is
 * declared as an in-image table the way Data_020094c4 already is at the
 * other Func_08009098 sites.
 */

extern s32 Data_03001e40;       /* the free-running frame counter */
extern s16 Data_0200c01c[];     /* image offset 0x401c, handed to Func_08009098 */

extern s32 Func_030003e0(s32 value, s32 divisor);
extern s32 Func_080000f8(void);
extern u8 *Func_080090c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080f9010(s32 sound_id);
extern void Func_080091e0(u8 *record, s32 arg1);
extern void Func_08009080(u8 *record, s32 arg1);
extern void Func_08009098(u8 *record, s16 *descriptor);

void Func_02002be0(void)
{
    u8 *record;
    u8 *handle;
    s32 spread;

    if (Func_030003e0(Data_03001e40, 3) != 0) {
        return;
    }

    record = Func_080090c8(
        200,
        (s32)(((u32)(Func_080000f8() * 3 << 4) >> 16) << 16) + 0x02fd0000,
        0xffc00000,
        152 << 18);
    if (record == 0) {
        return;
    }

    if (Func_030003e0(Data_03001e40, 9) == 0) {
        if (((u32)(Func_080000f8() << 1) >> 16) != 0) {
            Func_080f9010(145);
        } else {
            Func_080f9010(144);
        }
    }

    record[85] = 0;

    spread = (s32)((u32)(Func_080000f8() << 15) >> 16) + 0x4ccc;
    *(s32 *)(record + 72) = 0x6666;
    *(s32 *)(record + 28) = spread;
    *(s32 *)(record + 24) = spread;
    record[97] = 1;

    Func_080091e0(record, 0);

    record[35] = (u8)(record[35] & 0xfe);

    handle[9] = (u8)((handle[9] & ~0x0c) | 0x04);
    handle = *(u8 **)(record + 80);

    Func_08009080(record, 1);
    Func_08009098(record, Data_0200c01c);

    *(s32 *)(record + 36) =
        (s32)(((u32)(Func_080000f8() * 3 << 1) >> 16) - 3) << 16;
    *(s32 *)(record + 44) =
        (s32)((u32)(Func_080000f8() * 3 << 9) >> 16) + (s32)0xfffffd00;
    *(s32 *)(record + 40) = 128 << 12;
}
