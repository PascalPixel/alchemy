#include "types.h"

/*
 * resource_3bf soft-float double unpack at 0x02005e04, 212 bytes.
 *
 * `overlay_twins resource_3bf --semantic` proves that this owner and
 * resource_3a7:1770 are byte-identical across all 106 halfwords.  The source
 * shape follows the generic GCC 2.96 `fp-bit.c` unpack routine in the local
 * compiler source, including its word-order swap and packed-bitfield reads.
 *
 * The packed pair is copied to a local union so the exponent and sign can be
 * read with the witnessed halfword and byte loads.  The first input word is
 * the IEEE high word carrying the sign and exponent.
 *
 * Record layout:
 *   +0  class (0 signalling NaN, 1 quiet NaN, 2 zero, 3 finite, 4 infinity)
 *   +4  sign
 *   +8  unbiased exponent
 *   +12 significand low word
 *   +16 significand high word, normalised so bit 28 is set
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

void Func_02005e04(PackedDouble *source, SoftFloatRecord *record)
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
