#include "types.h"

#define Runtime_UnpackSoftDouble Func_02001770

/*
 * resource_3a7 soft-float double unpack at 0x02001770, 212 bytes.
 *
 * `overlay_twins resource_3a7 --semantic` proves that this owner and
 * resource_3bf:5e04 are byte-identical across all 106 halfwords.  The source
 * shape follows the generic GCC 2.96 `fp-bit.c` unpack routine in the local
 * compiler source, including its word-order swap and packed-bitfield reads.
 *
 * r0 addresses the packed pair, r1 the 20-byte record to fill.  The packed pair
 * is copied to the local sp+0/sp+4 slots purely so the exponent and sign can be
 * read back with narrower loads ('ldrh [sp,#6]', 'ldrb [sp,#7]'); those reads
 * confirm the library's word order, in which the FIRST word of the pair is the
 * IEEE high word carrying the sign and exponent.  That is why the packed value
 * is carried as SoftDouble (a u64 whose low half is the first word) rather than
 * as a native double.
 *
 * Record layout, which this routine defines for the whole family:
 *   +0  class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite, 4 infinity)
 *   +4  sign (bit 31 of the high word)
 *   +8  unbiased exponent
 *   +12 significand low word
 *   +16 significand high word, normalised so bit 28 is set
 *
 * Four cases, in the assembly's order: biased exponent 0 with a zero
 * significand (class 2); biased exponent 0 with a non-zero significand, a
 * denormal seeded to -1022 and normalised with an UNSIGNED compare against
 * 0x0FFFFFFF; biased exponent 0x7ff (class 4 for infinity, else class 1 or 0 on
 * the quiet bit, both storing the significand UNSHIFTED); and anything else,
 * exponent minus 1023 with the implicit leading bit re-inserted.
 *
 * Uncertainty (inherited): the constant 0 loaded into r1 in the normal-number
 * path is never used before the routine returns, so it is left unmodelled.
 */
typedef float DoubleType __attribute__((mode(DF)));
typedef unsigned int HalfFractionType __attribute__((mode(SI)));
typedef unsigned int FractionType __attribute__((mode(DI)));

typedef enum FloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
} FloatClass;

typedef struct SoftFloatRecord {
    FloatClass cls;
    unsigned int sign;
    int exponent;
    union {
        FractionType whole;
        HalfFractionType halves[2];
    } fraction;
} SoftFloatRecord;

typedef union PackedDouble {
    DoubleType value;
    FractionType raw;
    HalfFractionType words[2];
    struct {
        FractionType fraction : 52 __attribute__((packed));
        unsigned int exponent : 11 __attribute__((packed));
        unsigned int sign : 1 __attribute__((packed));
    } bits;
} PackedDouble;

void Runtime_UnpackSoftDouble(PackedDouble *source, SoftFloatRecord *record)
{
    FractionType fraction;
    int exponent;
    int sign;
    PackedDouble swapped;

    swapped.words[0] = source->words[1];
    swapped.words[1] = source->words[0];
    source = &swapped;

    fraction = source->bits.fraction;
    exponent = source->bits.exponent;
    sign = source->bits.sign;

    record->sign = sign;
    if (exponent == 0) {
        if (fraction == 0) {
            record->cls = CLASS_ZERO;
        } else {
            record->exponent = exponent - 1023 + 1;
            fraction <<= 8;
            record->cls = CLASS_NUMBER;
            while (fraction < (((FractionType)1) << (52 + 8))) {
                fraction <<= 1;
                record->exponent--;
            }
            record->fraction.whole = fraction;
        }
    } else if (exponent == 0x7ff) {
        if (fraction == 0) {
            record->cls = CLASS_INFINITY;
        } else {
            if (fraction & 0x8000000000000LL) {
                record->cls = CLASS_QNAN;
            } else {
                record->cls = CLASS_SNAN;
            }
            record->fraction.whole = fraction;
        }
    } else {
        record->exponent = exponent - 1023;
        record->cls = CLASS_NUMBER;
        record->fraction.whole = (fraction << 8) | (((FractionType)1) << (52 + 8));
    }
}
