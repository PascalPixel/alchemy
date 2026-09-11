#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/add_soft_double.h"

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
