#include "types.h"
#include "scene.h"
#include "add_parts_record.h"
#include "add_parts_body.inc"

/* map/locations/heidia/village/soft/add_parts.c */
/* Second embedded copy of the compiler runtime's soft-float addition core. */

extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
}

/* map/locations/heidia/village/soft/add.c */
/*
 * Soft-float double addition for overlay resource_3bf.  A packed double is
 * carried as SoftDouble, a u64 whose low half is the r0 word: this library
 * keeps the sign and exponent in r0 and the low word in r1, which is not the
 * native layout of a C `double`.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *Map_Run(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);

/*
 * Unpacks both operands into 20-byte records, runs the arithmetic core, and
 * repacks the result.  The frame members are declared in the order that
 * reproduces the stack layout.  The record layout past the sign word at
 * offset 4 is named by position and not verified, so it is kept opaque.
 */
SoftDouble AddSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packedB;
        SoftDouble packedA;
        SoftFloatRecord result;
        SoftFloatRecord recordB;
        SoftFloatRecord recordA;
    } frame;
    u32 *packedAWords = (u32 *)&frame.packedA;
    volatile u32 *packedBWords = (u32 *)&frame.packedB;

    packedAWords[0] = a0;
    packedAWords[1] = a1;
    packedBWords[0] = b0;
    packedBWords[1] = b1;

    Map_Apply(&frame.packedA, &frame.recordA);
    Map_Apply2(&frame.packedB, &frame.recordB);

    return Map_Do(Map_Run(&frame.recordA, &frame.recordB, &frame.result));
}

/* map/locations/heidia/village/soft/sub.c */
/*
 * Soft-float double subtraction for overlay resource_3bf.  A packed double is
 * carried as SoftDouble, a u64 whose low half is the r0 word: this library
 * keeps the sign and exponent in r0 and the low word in r1, which is not the
 * native layout of a C `double`.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *Map_Run(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);

/*
 * Unpacks both operands into 20-byte records, runs the shared addition core,
 * and repacks the result.  The frame members are declared in the order that
 * reproduces the stack layout.  The record layout past the sign word at
 * offset 4 is named by position and not verified, so it is kept opaque.
 */
SoftDouble SubtractSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packedB;
        SoftDouble packedA;
        SoftFloatRecord result;
        SoftFloatRecord recordB;
        SoftFloatRecord recordA;
    } frame;
    u32 *packedAWords = (u32 *)&frame.packedA;
    volatile u32 *packedBWords = (u32 *)&frame.packedB;

    packedAWords[0] = a0;
    packedAWords[1] = a1;
    packedBWords[0] = b0;
    packedBWords[1] = b1;

    Map_Apply(&frame.packedA, &frame.recordA);
    {
        SoftFloatRecord *recordB = &frame.recordB;

        Map_Apply2(&frame.packedB, recordB);

        /* Toggling the sign word of the unpacked second operand turns the
         * shared addition core into a subtraction. */
        recordB->word[1] ^= 1u;

        return Map_Do(Map_Run(&frame.recordA, recordB, &frame.result));
    }
}

/* map/locations/heidia/village/scene/status.c */
extern u8 gOv;

u8 *GetSoftFloatStatusStorage(void)
{
    return &gOv;
}

/* map/locations/heidia/village/soft/a_nan.c */
s32 SoftFloatRecordAIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/a_inf.c */
s32 SoftFloatRecordAIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/a_zero.c */
s32 SoftFloatRecordAIsZero(s32 *state)
{
    s32 match = 0;

    if (*state == 2) {
        match = 1;
    }
    return match;
}

/* map/locations/heidia/village/soft/from_int.c */
/*
 * Signed integer to soft-float conversion for resource_3bf.  The 124-byte
 * owner is 112 bytes of code plus its three pool words.  It is a leaf on
 * the non-interworking ABI, with r4 callee-saved.
 */

/*
 * The 20-byte unpacked record the library packs from:
 *   +0  class (2 = zero, 3 = finite)
 *   +4  sign (1 for a negative input)
 *   +8  exponent seed (60)
 *   +12 significand, 64 bits
 *
 * The significand must stay one 64-bit field.  Split into two words, the
 * normalising loop keeps the pair in registers instead of loading and
 * storing it through the record on every iteration.  r0 carries the high
 * word of the packed double, so the packed value is a u64 and not a double.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

/* The packer, reached by a direct call rather than through a veneer. */

/*
 * A zero input takes class 2 straight to the pack call, and 0x80000000
 * returns its packed value directly.  The guard before the normalising loop
 * is an unsigned compare, so a significand already above 0x0fffffffffffffff
 * skips normalisation entirely.
 */
SoftDouble SignedIntToSoftDouble(s32 value)
{
    SoftFloatRecord record;
    u32 sign;

    record.cls = 3u;
    sign = (u32)value >> 31;
    record.sign = sign;

    if (value == 0) {
        record.cls = 2u;
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            return (SoftDouble)0xC1E00000u;
        }
        record.fraction = (u64)(s64)(-value);
    } else {
        record.fraction = (u64)(s64)value;
    }

    while (record.fraction <= 0x0FFFFFFFFFFFFFFFu) {
        record.fraction = record.fraction << 1;
        record.exponent -= 1;
    }

pack:
    return Map_Do(&record);
}

/* map/locations/heidia/village/soft/to_int.c */
/*
 * Resource 3bf soft-double to signed-integer conversion at 0x02005b6c.
 * The packed value arrives as the library's two ABI words: r0 is the high
 * word and r1 is the low word.  The unpacker consumes their address, so the
 * packed stack slot remains volatile while the unpacked record does not.
 */
typedef u64 SoftDouble;

typedef union FloUnion { SoftDouble value; struct { u32 lo; u32 hi; } words; } FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

s32 SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord record;
    volatile FloUnion au;
    volatile FloUnion *slot;
    s32 exponent;
    u64 shifted;

    slot = &au;
    slot->words.lo = high;
    slot->words.hi = low;
    Map_Apply((FloUnion *)slot, &record);

    if (Map_Do(&record) != 0u) {
        return 0;
    }
    if (Map_unk2_2(&record) != 0u) {
        return 0;
    }
    if (Map_unk3_2(&record) == 0u) {
        exponent = record.exponent;
        if (exponent < 0) {
            return 0;
        }
        if (exponent <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(record.sign != 0u);

convert:
    shifted = Map_Apply2(record.fraction, (u32)(60 - exponent));
    if (record.sign != 0u) {
        return -(s32)shifted;
    }
    return (s32)shifted;
}

/* map/locations/heidia/village/soft/b_nan.c */
s32 SoftFloatRecordBIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/b_inf.c */
s32 SoftFloatRecordBIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/b_zero.c */
s32 SoftFloatRecordBIsZero(s32 *state)
{
    return *state == 2;
}

/* map/locations/heidia/village/soft/lsr64.c */
/* resource_3bf soft-float library: 64-bit logical right shift at 0x02005c08. */

/*
 * r0/r1 hold the low and high words and r2 the count -- the ordinary pair
 * placement for an unsigned long long, so it is spelled that way here.  A
 * count of 32 or more takes the `32 - n <= 0' path with a shift distance of
 * zero, avoiding an undefined `>> 32'; counts of 64 and over are unmasked
 * here and undefined in the original too.  The else arm's statement order --
 * carry, shifted low, high, then the OR -- is load-bearing, as is zeroing the
 * high half first in the other arm.
 */
typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;

u64 LogicalShiftRight64(u64 value, u32 count)
{
    SplitU64 in;
    SplitU64 out;
    s32 spare;
    u32 carry;
    u32 shifted;

    if (count == 0u) {
        return value;
    }
    in.whole = value;
    spare = 32 - (s32)count;
    if (spare <= 0) {
        spare = -spare;
        out.parts.hi = 0u;
        out.parts.lo = in.parts.hi >> spare;
    } else {
        carry = in.parts.hi << spare;
        shifted = in.parts.lo >> count;
        out.parts.hi = in.parts.hi >> count;
        out.parts.lo = shifted | carry;
    }
    return out.whole;
}

/* map/locations/heidia/village/soft/pack.c */
/*
 * Double-precision packer from the GCC soft-float runtime shape.  This overlay
 * copy calls the shared runtime predicates and shift helper at its witnessed
 * targets.  The final word swap is the runtime's FLOAT_WORD_ORDER_MISMATCH
 * path and preserves the packed-word order observed by its callers.
 */
typedef u64 SoftDouble;

enum SoftFloatClass {
    CLASS_SNAN,
    CLASS_QNAN,
    CLASS_ZERO,
    CLASS_NUMBER,
    CLASS_INFINITY
};

typedef struct SoftFloatRecord {
    enum SoftFloatClass cls;
    u32 sign;
    s32 exponent;
    union {
        u64 ll;
        u32 words[2];
    } fraction;
} SoftFloatRecord;

typedef union SoftDoubleUnion {
    SoftDouble value;
    u64 raw;
    u32 words[2];
    struct {
        u64 fraction : 52 __attribute__((packed));
        u32 exponent : 11 __attribute__((packed));
        u32 sign : 1 __attribute__((packed));
    } bits;
} SoftDoubleUnion;

SoftDouble PackSoftDouble(SoftFloatRecord *src)
{
    SoftDoubleUnion dst;
    u64 fraction = src->fraction.ll;
    s32 sign = (s32)src->sign;
    s32 exp = 0;

    if (Map_Check(src)) {
        exp = 0x7ff;
        if (src->cls == CLASS_QNAN || 1) {
            fraction |= 0x8000000000000ULL;
        }
    } else if (Map_unk2(src)) {
        exp = 0x7ff;
        fraction = 0;
    } else if (Map_unk3(src)) {
        exp = 0;
        fraction = 0;
    } else if (fraction == 0) {
        exp = 0;
    } else {
        if (src->exponent < -1022) {
            s32 shift = -1022 - src->exponent;

            exp = 0;
            if (shift > 56) {
                fraction = 0;
            } else {
                s32 lowbit = (fraction & ((1 << shift) - 1)) ? 1 : 0;
                fraction = Map_Apply(fraction, (u32)shift) | (u32)lowbit;
            }
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 60)) {
                exp += 1;
            }
            fraction >>= 8;
        } else if (src->exponent > 1023) {
            exp = 0x7ff;
            fraction = 0;
        } else {
            exp = src->exponent + 1023;
            if ((fraction & 0xff) == 0x80) {
                if (fraction & (1 << 8)) {
                    fraction += 0x80;
                }
            } else {
                fraction += 0x7f;
            }
            if (fraction >= (1ULL << 61)) {
                fraction >>= 1;
                exp += 1;
            }
            fraction >>= 8;
        }
    }

    dst.bits.fraction = fraction;
    dst.bits.exponent = (u32)exp;
    dst.bits.sign = (u32)sign;

    {
        u32 tmp = dst.words[0];
        dst.words[0] = dst.words[1];
        dst.words[1] = tmp;
    }

    return dst.value;
}

/* map/locations/heidia/village/soft/c_nan.c */
s32 SoftFloatRecordCIsNaN(u32 *state)
{
    return *state <= 1;
}

/* map/locations/heidia/village/soft/c_inf.c */
s32 SoftFloatRecordCIsInfinity(s32 *state)
{
    return *state == 4;
}

/* map/locations/heidia/village/soft/c_zero.c */
s32 SoftFloatRecordCIsZero(s32 *state)
{
    return *state == 2;
}

/* map/locations/heidia/village/soft/unpack.c */
/*
 * Soft-float double unpack for overlay resource_3bf.
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
 * The exponent is unbiased and the significand is normalised so that bit 28
 * of its high half is set.
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
 * Unpacks a packed double into record. The pair is copied to a local union
 * with its two words swapped, so the sign and exponent come from the IEEE
 * high word and are read as packed bitfields rather than reassembled.
 */
void UnpackSoftDouble(PackedDouble *source, SoftFloatRecord *record)
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
