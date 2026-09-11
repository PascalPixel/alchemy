/*
 * Signed integer to soft-float conversion in resource_3a7: build the
 * library's unpacked record on the stack and hand it to the packer.
 */
#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/signed_int_to_soft_double.h"

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
