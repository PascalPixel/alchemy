#include "types.h"

/*
 * resource_3a7 soft-float double subtraction at 0x020013e4.
 *
 * TRANSPOSED from games/gs1/semantic/overlays/resource_3bf_c_02005a78.c.  The two owners are
 * the same routine shared verbatim; every differing halfword is part of a BL
 * pair and no pool word differs.
 *
 * What was changed: the entry symbol, and the four calls, re-resolved with
 * 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a7 13e4'.  All four target this
 * overlay's own prologues, and the corrected 'bl' rule collapses them the way
 * the code shape demands: the two operand unpacks are ONE routine, 0x02001770
 * (transposed alongside this file as resource_3a7_c_02001770.c), the arithmetic
 * core is 0x02001158, and the repack is 0x020015a4 - the same packer the sibling
 * integer conversion at 0x0200145c calls.  The 3bf source predates the corrected
 * rule and spelled the four sites as four unrelated Func_0200bxxx names, which
 * hid all of that.
 *
 * Frame map (sp-relative, 76 bytes):
 *   sp+0  .. sp+8   second operand, stored from r2/r3
 *   sp+8  .. sp+16  first operand, stored from r0/r1
 *   sp+16 .. sp+36  unpacked result record
 *   sp+36 .. sp+56  unpacked second operand
 *   sp+56 .. sp+76  unpacked first operand
 *
 * Word order: this library carries a double as r0 = high (sign/exponent) word
 * and r1 = low word, so the packed value is a SoftDouble (a u64 whose low half
 * is the r0 word) rather than a native double.
 *
 * Uncertainty (inherited): the record layout beyond the sign word at offset 4 is
 * taken from the integer-conversion sibling, not from the unpack routine, so it
 * is kept opaque here.
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
     * `ldr r3,[r4,#4]; eors r3,r2(=1); str r3,[r4,#4]` — the sign word of the
     * unpacked second operand is toggled, turning the shared addition core
     * into a subtraction.  This is the only difference from the sibling at
     * 0x020013ac.
     */
        rb->word[1] ^= 1u;

        return Func_020029c0(Func_02002570(&frame.record_a, rb, &frame.result));
    }
}
