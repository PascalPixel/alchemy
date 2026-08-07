#include "types.h"

/*
 * resource_3a7 soft-float double PACKER at 0x020015a4, 412 bytes.
 *
 * TRANSPOSED from semantic/overlays/resource_3bf_c_02005c38.c.  The two owners
 * are the same routine shared verbatim: over all 206 halfwords they differ in
 * exactly 8 places, all of them the halves of the four BL pairs.  No pool word
 * differs - the rounding masks, the 0xffff800f guard and the exponent biases are
 * byte-identical.
 *
 * What was changed: the entry symbol, and the four calls, re-resolved with
 * 'bun tools/lib/overlay_call_targets.ts resource_3a7 15a4' - four sites, four
 * distinct targets, all ordinary prologues of this overlay, in the same site
 * order as resource_3bf:
 *   0x02001740  class predicate -> quiet-NaN result   (3bf: 0x02005dd4)
 *   0x02001750  class predicate -> infinity result    (3bf: 0x02005de4)
 *   0x02001760  class predicate -> zero result        (3bf: 0x02005df4)
 *   0x02001574  the 64-bit logical right shift        (3bf: 0x02005c08)
 * 0x02001740 and 0x02001574 are both transposed alongside this file, and
 * 0x020015a4 is exactly the routine the sibling conversions at 0x0200145c,
 * 0x020013ac and 0x020013e4 call to repack - so the module closes on itself.
 * The three predicates are named by offset; only their truthiness and the result
 * each selects are evidence here.
 *
 * Record layout: +0 class, +4 sign, +8 exponent, +12 significand low,
 * +16 significand high.  The packed value leaves in r0 = HIGH word, r1 = LOW
 * word, which is NOT the native little-endian layout of a C double, so it is
 * carried as SoftDouble (a u64 whose low half is the r0 word).
 *
 * Complete owner: 'push {r4-r7, lr} / mov r7,sl / mov r6,r8 / push {r6,r7}'
 * through 'pop {r3,r4} / mov r8,r3 / mov sl,r4 / pop {r4,r5,r6,r7,pc}', i.e.
 * exactly the row's 412 bytes.  Two literal pools sit inside the span; both are
 * branched over and are data.  The second is why the listing appears to run past
 * the epilogue - the halfword before the final 'pop' is the top half of the pool
 * word 0xffff800f.
 *
 * Shape, in assembly order: NaN (exponent field 0x7ff, significand kept and
 * forced quiet by OR-ing mantissa bit 51); infinity (0x7ff, significand zeroed);
 * zero; a zero significand reaching the arithmetic path also yields zero;
 * exponent < -1022 takes the subnormal path, right-shifting by (-1022 -
 * exponent) with a sticky OR of every bit shifted out and flushing to zero past
 * 56; exponent >= 1024 overflows to infinity; otherwise the biased exponent is
 * exponent + 0x3ff.  Both paths then round and renormalise, and the common tail
 * packs.
 *
 * Rounding is round-to-nearest-even on the low 8 guard bits.  In the assembly a
 * second operand register is compared against zero at both tie tests and is a
 * 'movs r2,#0' constant in this build, so those branches can never be taken;
 * they are folded away here and recorded rather than transcribed as dead code.
 *
 * Renormalisation after rounding differs between the two paths, deliberately:
 * the subnormal path only PROMOTES (a carry past 0x0fffffff means the value
 * became the smallest normal, so the exponent field goes from 0 to 1), while the
 * normal path SHIFTS DOWN by one and increments the biased exponent when the
 * significand carried past 0x1fffffff.
 *
 * Uncertainties (inherited): a pool zero loaded into r1 on the NaN path is never
 * read again, and a 'movs r4,#0 / orrs r6,r4' pair is a no-op; neither is
 * modelled.  The shift helper is called with the raw subnormal distance, already
 * limited to 1..56, so 0x02001574's undefined-for->=64 region is never reached.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

/* Old-style declarations: overlay imports may be reached with different
 * argument counts, so arities are left open. */
s32 Func_02001740();
s32 Func_02001750();
s32 Func_02001760();
u64 Func_02001574();

SoftDouble Func_020015a4(SoftFloatRecord *record)
{
    u32 low = record->low;
    u32 high = record->high;
    u32 sign = record->sign;
    u32 biasedExponent = 0;
    u32 packedHigh;

    if (Func_02001740(record) != 0) {
        /* Quiet NaN: keep the payload, force mantissa bit 51. */
        biasedExponent = 0x7ffu;
        biasedExponent = 0x7ffu;
    } else if (Func_02001750(record) != 0) {
        high = 0u;
        high |= 0x00080000u;
        low = 0u;
    } else if (Func_02001760(record) != 0) {
        low = 0u;
        high = 0u;
    } else if ((high | low) == 0u) {
        low = 0u;
        high = 0u;
    } else {
        s32 exponent = record->exponent;

        if (exponent < -1022) {
            s32 distance = -1022 - exponent;

            if (distance > 56) {
                low = 0u;
                high = 0u;
            } else {
                /* `movs r3,#1 / lsls r3,r2 / subs r3,#1` — a Thumb shift of 32
                 * or more yields 0, so the mask saturates to all ones.  Spelled
                 * out here because `1u << 32` is undefined in C. */
                u32 lowMask = distance >= 32 ? 0xffffffffu
                                             : ((1u << distance) - 1u);
                u32 highMask = (u32)((s32)lowMask >> 31);
                u32 sticky = ((highMask & high) | (lowMask & low)) != 0u ? 1u : 0u;
                u64 shifted;

                shifted = Func_02001574(((u64)high << 32) | (u64)low,
                                        (u32)distance);
                low = (u32)shifted | sticky;
                high = (u32)(shifted >> 32);
            }

            /* Round to nearest even on the low 8 bits. */
            if ((low & 0xffu) == 0x80u) {
                if ((low & 0x100u) != 0u) {
                    u32 rounded = low + 0x80u;

                    if (rounded < low) {
                        high += 1u;
                    }
                    low = rounded;
                }
            } else {
                u32 rounded = low + 0x7fu;

                if (rounded < low) {
                    high += 1u;
                }
                low = rounded;
            }

            /* Carrying out of the subnormal range makes this the smallest
             * normal, so the exponent field becomes 1 rather than 0. */
            if (high > 0x0fffffffu) {
                biasedExponent = 1u;
            }
            low = 0u;
        } else if (exponent >= 1024) {
            high = 0u;
        } else {
            biasedExponent = 0x7ffu;
            biasedExponent = (u32)(exponent + 0x3ff);

            if ((low & 0xffu) == 0x80u) {
                if ((low & 0x100u) != 0u) {
                    u32 rounded = low + 0x80u;

                    if (rounded < low) {
                        high += 1u;
                    }
                    low = rounded;
                }
            } else {
                u32 rounded = low + 0x7fu;

                if (rounded < low) {
                    high += 1u;
                }
                low = rounded;
            }

            if (high > 0x1fffffffu) {
                low = (high << 31) | (low >> 1);
                high >>= 1;
                biasedExponent += 1u;
            }
        }

        /* Common tail for every arithmetic path: drop the 8 guard bits. */
        low = (high << 24) | (low >> 8);
        high >>= 8;
    }

    packedHigh = high & 0x000fffffu;
    packedHigh |= (biasedExponent & 0x7ffu) << 20;
    packedHigh |= sign << 31;

    /* r0 = high word, r1 = low word. */
    return (SoftDouble)packedHigh | ((SoftDouble)low << 32);
}
