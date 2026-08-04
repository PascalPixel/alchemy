#include "types.h"

/*
 * resource_3a4 owner at 0x02002a48, 272 bytes: the per-frame update of the
 * presentation record that resource_3a4_c_02002b58.c spawns -- drift the
 * record along one axis by a randomised amount whose sign is chosen by the
 * heading global, advance three fixed-point fields on a size-dependent
 * schedule, reseed a countdown at +100, and free the record when the
 * lifetime counter at +104 reaches zero.
 *
 * KNOWN BEFORE IT WAS OPENED. This row is a PUBLISHED CALLBACK with no
 * caller anywhere in the image, and the reason is written into its
 * neighbour: 0x02002b58 stores the plain word 0x0200aa49 -- image offset
 * 0x2a48 with the Thumb bit -- into a freshly spawned record at +108 and
 * never calls it. So the argument here is that record, and the fields the
 * two rows touch agree exactly: 0x02002b58 stamps +100 = 30, +102 = 1,
 * +104 = 20, and this row reads +100 and decrements +104 to zero. That
 * agreement is the corroboration, not an assumption carried over.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002a48 through the
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02002b3a-
 * 0x02002b3e, followed by the six-word literal pool 0x02002b40-0x02002b57
 * that closes the 272-byte span; the next owner is the already-drafted
 * 0x02002b58. measureSpan reports 248, so it undersizes this row by the
 * pool's last 24 bytes -- the seventh row on this overlay to run past it.
 * One argument (r0, held in r5 for the whole body), void.
 *
 * Six call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x2a48 0x2b58` and NONE
 * read from the listing's arrows, which print six different addresses in
 * 0x02006xxx that do not exist as functions:
 *   0x2002a62, 0x2002a82, 0x2002af4, 0x2002b1a  -> veneer 0x2003a90 -> Func_080000f8()   the established argument-less RNG
 *   0x2002b0a                                    -> veneer 0x2003c20 -> Func_0808a160(record, 7)
 *   0x2002b36                                    -> veneer 0x2003ae8 -> Func_080090d0(record)   established teardown
 * Four bit-identical `f003 fd47` encodings; the tool collapses them onto
 * the one veneer, which is exactly what the raw listing hides.
 *
 * Data_0200d23c is the word resource_3a4_c_02002bb0.c writes from its
 * fourth argument, immediately before the Data_0200d240 coordinate block
 * that 0x02002b58 spawns from. This row tests it against 0 and 0x8000 and
 * only ever adds or subtracts on those two values, falling through
 * untouched otherwise -- the shape of a two-way heading. Called a heading
 * here for description only; it is not named, because two observed values
 * do not establish a domain.
 *
 * Honest about the constants. +16 gains 0x00013333 and +24/+28 gain
 * 0x000007ae on the large branch; on the small branch +24 gains 0x00001999
 * and +28 gains 0xfda6f337. The first five read as plain 16.16 fractions
 * (1.2, 0.03, 0.1) against the 0x4ccc that resource_3a4_c_02002a10.c
 * stamps into these same two fields, but 0xfda6f337 does not -- it is
 * -600.06 in 16.16, three orders off its sibling. The word is verified
 * twice, from `overlay_show`'s pool listing and again by reading the image
 * bytes directly, and it is transcribed as it stands. The asymmetry remains
 * unexplained.
 *
 * The record field roles are recorded by offset: +8 the axis this row
 * drifts, +16, +24 and +28 the three advanced fields, +100 a halfword read
 * signed in three places and unsigned in two, +104 the lifetime counter.
 * The two arms that write +8 share one store at 0x02002aa0 and the two
 * that write +28 share one at 0x02002af2, so neither pair is an
 * independent block.
 */

extern s32 Data_0200d23c;

extern s32 Func_080000f8(void);
extern void Func_0808a160(u8 *record, s32 arg1);
extern void Func_080090d0(u8 *record);

void Func_02002a48(u8 *record)
{
    s32 heading;
    s32 spread;
    s32 jitter;
    s32 roll;
    u16 countdown;

    heading = Data_0200d23c;
    if (heading == 0) {
        jitter = (s32)((u32)((s32)((u32)(Func_080000f8() << 1) >> 16) - 1) << 16) >> 1;
        spread = *(s16 *)(record + 100) << 12;
        *(s32 *)(record + 8) = *(s32 *)(record + 8) + (spread + jitter);
    } else if (heading == 0x8000) {
        jitter = (s32)((u32)((s32)((u32)(Func_080000f8() << 1) >> 16) - 1) << 16) >> 1;
        spread = *(s16 *)(record + 100) << 12;
        *(s32 *)(record + 8) = *(s32 *)(record + 8) - (spread + jitter);
    }

    if (*(s16 *)(record + 100) > 3) {
        *(s32 *)(record + 16) += 0x00013333;
        *(s32 *)(record + 24) += 0x000007ae;
        *(s32 *)(record + 28) += 0x000007ae;
    } else {
        heading = Data_0200d23c;
        if (heading == 0) {
            *(s32 *)(record + 8) += 0x8000;
        } else if (heading == 0x8000) {
            *(s32 *)(record + 8) += 0xffff8000;
        }
        *(s32 *)(record + 24) += 0x00001999;
        *(s32 *)(record + 28) += 0xfda6f337;
    }

    roll = Func_080000f8();
    if (((u32)(*(s16 *)(record + 100) * roll) >> 16) == 0) {
        Func_0808a160(record, 7);
    }

    countdown = *(u16 *)(record + 100);
    if (countdown != 0) {
        *(u16 *)(record + 100) = (u16)(countdown - 2);
    } else {
        *(u16 *)(record + 100) =
            (u16)((((u32)(Func_080000f8() * 5) >> 16) << 1) + 2);
    }

    *(s32 *)(record + 104) -= 1;
    if (*(s32 *)(record + 104) == 0) {
        Func_080090d0(record);
    }
}
