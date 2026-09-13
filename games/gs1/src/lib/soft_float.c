#include "types.h"

#define SoftFloatLeftIsNaN Func_0200259a
#define SoftFloatRightIsNaN Func_020025a8
#define SoftFloatLeftIsInfinity Func_020025c2
#define GetSoftFloatNaNRecord Func_020025c8
#define SoftFloatRightIsInfinityWithLeftInfinity Func_020025cc
#define SoftFloatRightIsInfinity Func_020025e8
#define SoftFloatRightIsZero Func_02002602
#define SoftFloatLeftIsZeroWithRightZero Func_0200260c
#define SoftFloatLeftIsZero Func_02002632
#define SoftFloat_IsNaNForConvert Func_02001544
#define SoftFloat_IsNaNForPack Func_02001740
#define SoftFloat_AddParts Func_02001158
#define SoftFloat_Add Func_020013ac
#define SoftFloat_Subtract Func_020013e4
#define SoftFloat_GetNaNRecord Func_02001424
#define SoftFloat_IsNaNForAdd Func_0200142c
#define SoftFloat_IsInfinityForAdd Func_0200143c
#define SoftFloat_IsZeroForAdd Func_0200144c
#define SoftFloat_FromInt Func_0200145c
#define SoftFloat_ToInt Func_020014d8
#define SoftFloat_IsInfinityForConvert Func_02001554
#define SoftFloat_IsZeroForConvert Func_02001564
#define SoftFloat_ShiftRight Func_02001574
#define SoftFloat_Pack Func_020015a4
#define SoftFloat_IsInfinityForPack Func_02001750
#define SoftFloat_IsZeroForPack Func_02001760
#define SoftFloat_Unpack Func_02001770

#include "add_parts_record.h"

/* This overlay carries a private copy of the soft-float runtime. */
typedef u64 SoftDouble;
typedef float DoubleType __attribute__((mode(DF)));
typedef SoftFloatFraction FractionType;
typedef SoftFloatHalfFraction HalfFractionType;

typedef union FloUnion {
    SoftDouble value;
    struct {
        u32 lo;
        u32 hi;
    } words;
} FloUnion;

typedef union {
    u64 whole;
    struct {
        u32 lo;
        u32 hi;
    } parts;
} SplitU64;

/* Packed double words are reversed relative to the integer pair. */
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

extern u8 Data_02009888[];

s32 Func_0200259a(SoftFloatRecord *record);
s32 Func_020025a8(SoftFloatRecord *record);
s32 Func_020025c2(SoftFloatRecord *record);
SoftFloatRecord *Func_020025c8(void);
s32 Func_020025cc(SoftFloatRecord *record);
s32 Func_020025e8(SoftFloatRecord *record);
s32 Func_02002602(SoftFloatRecord *record);
s32 Func_0200260c(SoftFloatRecord *record);
s32 Func_02002632(SoftFloatRecord *record);
void Func_02002b34(const SoftDouble *packed, SoftFloatRecord *record);
void Func_02002b3e(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_02002980(SoftFloatRecord *result);
void Func_02002b6c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_02002b76(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_020029c0(SoftFloatRecord *result);
SoftDouble Func_02002a6a(SoftFloatRecord *record);
void Func_02002c5a(FloUnion *packed, SoftFloatRecord *rec);
u32 Func_02002a54(SoftFloatRecord *rec);
u32 Func_02002a3e(SoftFloatRecord *rec);
u32 Func_02002a5c(SoftFloatRecord *rec);
u64 Func_02002aa6(u64 fraction, u32 count);
s32 Func_02002cfc(SoftFloatRecord *record);
s32 Func_02002d24(SoftFloatRecord *record);
s32 Func_02002d3e(SoftFloatRecord *record);
u64 Func_02002b9a(u64 value, u32 count);
SoftFloatRecord *Func_02002530(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftFloatRecord *Func_02002570(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftFloatRecord *SoftFloat_AddParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
#include "add_parts_body.inc"
}

SoftDouble SoftFloat_Add(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packed_b;
        SoftDouble packed_a;
        SoftFloatRecord result;
        SoftFloatRecord record_b;
        SoftFloatRecord record_a;
    } frame;
    u32 *wa = (u32 *)&frame.packed_a;
    volatile u32 *wb = (u32 *)&frame.packed_b;

    wa[0] = a0;
    wa[1] = a1;
    wb[0] = b0;
    wb[1] = b1;

    Func_02002b34(&frame.packed_a, &frame.record_a);
    Func_02002b3e(&frame.packed_b, &frame.record_b);

    return Func_02002980(Func_02002530(&frame.record_a, &frame.record_b, &frame.result));
}

SoftDouble SoftFloat_Subtract(u32 a0, u32 a1, u32 b0, u32 b1)
{
    struct {
        SoftDouble packed_b;
        SoftDouble packed_a;
        SoftFloatRecord result;
        SoftFloatRecord record_b;
        SoftFloatRecord record_a;
    } frame;
    u32 *wa = (u32 *)&frame.packed_a;
    volatile u32 *wb = (u32 *)&frame.packed_b;

    wa[0] = a0;
    wa[1] = a1;
    wb[0] = b0;
    wb[1] = b1;

    Func_02002b6c(&frame.packed_a, &frame.record_a);
    {
        SoftFloatRecord *rb = &frame.record_b;

        Func_02002b76(&frame.packed_b, rb);

        /*
         * Toggling the sign word of the unpacked second operand is what turns
         * the shared addition core into a subtraction, and is the only
         * difference from the addition entry.
         */
        rb->sign ^= 1u;

        return Func_020029c0(Func_02002570(&frame.record_a, rb, &frame.result));
    }
}

u8 *SoftFloat_GetNaNRecord(void)
{
    return Data_02009888;
}

s32 SoftFloat_IsNaNForAdd(u32 *state)
{
    return *state <= 1;
}

s32 SoftFloat_IsInfinityForAdd(s32 *state)
{
    return *state == 4;
}

s32 SoftFloat_IsZeroForAdd(s32 *state)
{
    return *state == 2;
}

/*
 * Zero takes class 2 straight to the pack call; 0x80000000 returns the
 * literal -2147483648.0 without packing. Otherwise normalisation loops on
 * an unsigned compare, so a significand already above the threshold is
 * left alone. The 124-byte owner includes its three pool words.
 */
SoftDouble SoftFloat_FromInt(s32 value)
{
    SoftFloatRecord record;
    u32 sign;

    record.classification = 3u;
    sign = (u32)value >> 31;
    record.sign = sign;

    if (value == 0) {
        record.classification = 2u;
        goto pack;
    }

    record.exponent = 60;

    if (sign != 0u) {
        if ((u32)value == 0x80000000u) {
            return (SoftDouble)0xC1E00000u;
        }
        record.fraction.whole = (u64)(s64)(-value);
    } else {
        record.fraction.whole = (u64)(s64)value;
    }

    while (record.fraction.whole <= 0x0FFFFFFFFFFFFFFFu) {
        record.fraction.whole = record.fraction.whole << 1;
        record.exponent -= 1;
    }

pack:
    return Func_02002a6a(&record);
}

s32 SoftFloat_ToInt(u32 high, u32 low)
{
    SoftFloatRecord rec;
    volatile FloUnion u;
    volatile FloUnion *p;
    s32 exp;
    u64 frac;

    p = &u;
    p->words.lo = high;
    p->words.hi = low;
    Func_02002c5a((FloUnion *)p, &rec);

    if (Func_02002a54(&rec) != 0u) {
        return 0;
    }
    if (Func_02002a3e(&rec) != 0u) {
        return 0;
    }
    if (Func_02002a5c(&rec) == 0u) {
        exp = rec.exponent;
        if (exp < 0) {
            return 0;
        }
        if (exp <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(rec.sign != 0u);

convert:
    frac = Func_02002aa6(rec.fraction.whole, (u32)(60 - exp));
    if (rec.sign != 0u) {
        return -(s32)frac;
    }
    return (s32)frac;
}

s32 SoftFloat_IsNaNForConvert(u32 *state)
{
    return *state <= 1;
}

s32 SoftFloat_IsInfinityForConvert(s32 *state)
{
    s32 ret = 0;

    if (*state == 4) {
        ret = 1;
    }
    return ret;
}

s32 SoftFloat_IsZeroForConvert(s32 *state)
{
    return *state == 2;
}

u64 SoftFloat_ShiftRight(u64 value, u32 count)
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

SoftDouble SoftFloat_Pack(SoftFloatRecord *src)
{
    SoftDoubleUnion dst;
    u64 fraction = src->fraction.whole;
    s32 sign = (s32)src->sign;
    s32 exp = 0;

    if (Func_02002cfc(src)) {
        exp = 0x7ff;
        if (src->classification == SOFT_FLOAT_QUIET_NAN || 1) {
            fraction |= 0x8000000000000ULL;
        }
    } else if (Func_02002d24(src)) {
        exp = 0x7ff;
        fraction = 0;
    } else if (Func_02002d3e(src)) {
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
                fraction = Func_02002b9a(fraction, (u32)shift) | (u32)lowbit;
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

s32 SoftFloat_IsNaNForPack(u32 *state)
{
    s32 ret = 0;

    if (*state <= 1) {
        ret = 1;
    }
    return ret;
}

s32 SoftFloat_IsInfinityForPack(s32 *state)
{
    return *state == 4;
}

s32 SoftFloat_IsZeroForPack(s32 *state)
{
    return *state == 2;
}

/*
 * The four cases are taken in this order: zero; a denormal, seeded to -1022
 * and normalised with an unsigned compare; exponent 0x7ff, which stores the
 * significand unshifted for both NaN classes; and the normal case, which
 * re-inserts the implicit leading bit.  One constant materialised in the
 * normal path is never used, so it is left unmodelled.
 */
void SoftFloat_Unpack(PackedDouble *source, SoftFloatRecord *record)
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
            record->classification = SOFT_FLOAT_ZERO;
        } else {
            record->exponent = exponent - 1023 + 1;
            fraction <<= 8;
            record->classification = SOFT_FLOAT_NUMBER;
            while (fraction < (((FractionType)1) << (52 + 8))) {
                fraction <<= 1;
                record->exponent--;
            }
            record->fraction.whole = fraction;
        }
    } else if (exponent == 0x7ff) {
        if (fraction == 0) {
            record->classification = SOFT_FLOAT_INFINITY;
        } else {
            if (fraction & 0x8000000000000LL) {
                record->classification = SOFT_FLOAT_QUIET_NAN;
            } else {
                record->classification = SOFT_FLOAT_SIGNALING_NAN;
            }
            record->fraction.whole = fraction;
        }
    } else {
        record->exponent = exponent - 1023;
        record->classification = SOFT_FLOAT_NUMBER;
        record->fraction.whole = (fraction << 8) | (((FractionType)1) << (52 + 8));
    }
}
