#include "types.h"
#include "scene.h"

/*
 * Soft-float double unpack -- resource_3a7.  Splits a packed pair into the
 * class, sign, exponent and normalised significand record the rest of the
 * family works on.
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

/*
 * Record layout: class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite,
 * 4 infinity), sign taken from bit 31 of the high word, unbiased exponent,
 * then the significand normalised so that bit 28 of its high word is set.
 */
typedef struct SoftFloatRecord {
    FloatClass cls;
    unsigned int sign;
    int exponent;
    union {
        FractionType whole;
        HalfFractionType halves[2];
    } fraction;
} SoftFloatRecord;

/*
 * The packed value is a pair of words whose first word is the high word
 * carrying the sign and exponent, so it is copied into a local and read back
 * through the narrower bitfields below.
 */
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

/*
 * The four cases are taken in this order: zero; a denormal, seeded to -1022
 * and normalised with an unsigned compare; exponent 0x7ff, which stores the
 * significand unshifted for both NaN classes; and the normal case, which
 * re-inserts the implicit leading bit.  One constant materialised in the
 * normal path is never used, so it is left unmodelled.
 */
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
