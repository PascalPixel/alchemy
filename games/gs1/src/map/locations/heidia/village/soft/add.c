#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/soft/add.h"

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
