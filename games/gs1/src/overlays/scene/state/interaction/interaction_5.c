#include "types.h"
#include "scene.h"
#include "add_parts_record.h"
#include "add_parts_body.inc"

/* overlays/scene/state/interaction/add_parts.c */
extern SoftFloatRecord *GetSoftFloatNaNRecord(void);

SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
}

/* overlays/scene/state/interaction/add_soft_double.c */
/*
 * Soft-float double addition for resource_3a7.  Both operands are unpacked into
 * records, added, and the result repacked.
 *
 * The frame is laid out in this order and no other: second operand, first
 * operand, result record, second record, first record.  A double is carried as
 * a high (sign and exponent) word followed by a low word, so the packed value
 * is a u64 whose low half is the high word, not a native double.  The record
 * layout beyond the sign word is not established here and is kept opaque.  The
 * two unpack calls reach one routine; each site keeps its own name.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *State_Run(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftDouble AddSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
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

    State_Apply(&frame.packed_a, &frame.record_a);
    State_Apply2(&frame.packed_b, &frame.record_b);

    return State_Do(State_Run(&frame.record_a, &frame.record_b, &frame.result));
}

/* overlays/scene/state/interaction/subtract_soft_double.c */
/*
 * Soft-float double subtraction for resource_3a7.  It shares the addition
 * core, reaching it with the second operand's sign toggled.
 *
 * The frame is laid out in this order and no other: second operand, first
 * operand, result record, second record, first record.  A double is carried as
 * a high (sign and exponent) word followed by a low word, so the packed value
 * is a u64 whose low half is the high word, not a native double.  The record
 * layout beyond the sign word is not established here and is kept opaque.  The
 * two unpack calls reach one routine; each site keeps its own name.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

SoftFloatRecord *State_Run(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);

SoftDouble SubtractSoftDouble(u32 a0, u32 a1, u32 b0, u32 b1)
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

    State_Apply(&frame.packed_a, &frame.record_a);
    {
        SoftFloatRecord *rb = &frame.record_b;

        State_Apply2(&frame.packed_b, rb);

        /*
         * Toggling the sign word of the unpacked second operand is what turns
         * the shared addition core into a subtraction, and is the only
         * difference from the addition entry.
         */
        rb->word[1] ^= 1u;

        return State_Do(State_Run(&frame.record_a, rb, &frame.result));
    }
}

/* overlays/scene/state/interaction/get_table_9888.c */
extern u8 gOv[];

u8 *SceneData_GetTable9888(void)
{
    return gOv;
}

/* overlays/scene/state/interaction/is_state_at_most_one.c */
s32 State_IsStateAtMostOne(u32 *state)
{
    return *state <= 1;
}

/* overlays/scene/state/interaction/is_state_4.c */
s32 State_IsState4(s32 *state)
{
    return *state == 4;
}

/* overlays/scene/state/interaction/is_state_two_c.c */
s32 State_IsStateTwoC(s32 *state)
{
    return *state == 2;
}

/* overlays/scene/state/interaction/signed_int_to_soft_double.c */
/*
 * Signed integer to soft-float conversion in resource_3a7: build the
 * library's unpacked record on the stack and hand it to the packer.
 */

/*
 * The packed double is carried as a u64 whose low half is the r0 word,
 * because r0 holds the high word of the double here; a `double` would not
 * spell that.
 */
typedef u64 SoftDouble;

/*
 * The unpacked record is 20 bytes: class at +0 (2 = zero, 3 = finite),
 * sign at +4, exponent seed at +8, and the significand at +12 and +16.
 */
typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u64 fraction;
} SoftFloatRecord;

/*
 * Zero takes class 2 straight to the pack call; 0x80000000 returns the
 * literal -2147483648.0 without packing. Otherwise normalisation loops on
 * an unsigned compare, so a significand already above the threshold is
 * left alone. The 124-byte owner includes its three pool words.
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
    return State_Do(&record);
}

/* overlays/scene/state/interaction/soft_double_to_signed_int.c */
typedef u64 SoftDouble;

typedef union FloUnion {
    SoftDouble value;
    struct {
        u32 lo;
        u32 hi;
    } words;
} FloUnion;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exp;
    u64 fraction;
} SoftFloatRecord;

s32 Runtime_SoftDoubleToSignedInt(u32 high, u32 low)
{
    SoftFloatRecord rec;
    volatile FloUnion u;
    volatile FloUnion *p;
    s32 exp;
    u64 frac;

    p = &u;
    p->words.lo = high;
    p->words.hi = low;
    State_Apply((FloUnion *)p, &rec);

    if (State_Do(&rec) != 0u) {
        return 0;
    }
    if (State_unk2_2(&rec) != 0u) {
        return 0;
    }
    if (State_unk3_2(&rec) == 0u) {
        exp = rec.exp;
        if (exp < 0) {
            return 0;
        }
        if (exp <= 30) {
            goto convert;
        }
    }
    return 0x7fffffff + (s32)(rec.sign != 0u);

convert:
    frac = State_Apply2(rec.fraction, (u32)(60 - exp));
    if (rec.sign != 0u) {
        return -(s32)frac;
    }
    return (s32)frac;
}
