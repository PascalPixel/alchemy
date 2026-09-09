#include "types.h"

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

void Func_0200b85c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_0200b866(const SoftDouble *packed, SoftFloatRecord *record);
SoftFloatRecord *Func_0200b258(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);
SoftDouble Func_0200b6a8(SoftFloatRecord *record);

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

    Func_0200b85c(&frame.packedA, &frame.recordA);
    Func_0200b866(&frame.packedB, &frame.recordB);

    return Func_0200b6a8(Func_0200b258(&frame.recordA, &frame.recordB, &frame.result));
}
