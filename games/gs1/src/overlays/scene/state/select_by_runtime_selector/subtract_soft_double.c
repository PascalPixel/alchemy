#include "types.h"

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

void Func_02002b6c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_02002b76(const SoftDouble *packed, SoftFloatRecord *record);
SoftFloatRecord *Func_02002570(SoftFloatRecord *left, SoftFloatRecord *right,
                               SoftFloatRecord *result);
SoftDouble Func_020029c0(SoftFloatRecord *result);

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

    Func_02002b6c(&frame.packed_a, &frame.record_a);
    {
        SoftFloatRecord *rb = &frame.record_b;

        Func_02002b76(&frame.packed_b, rb);

        /*
         * Toggling the sign word of the unpacked second operand is what turns
         * the shared addition core into a subtraction, and is the only
         * difference from the addition entry.
         */
        rb->word[1] ^= 1u;

        return Func_020029c0(Func_02002570(&frame.record_a, rb, &frame.result));
    }
}
