typedef unsigned int u32;
typedef unsigned long long u64;

/*
 * resource_3a7 soft-float double addition at 0x020013ac.
 *
 * TRANSPOSED from semantic/overlays/resource_3bf_c_02005a40.c.  The two owners are
 * the same routine shared verbatim; every differing halfword is part of a BL
 * pair and no pool word differs.
 *
 * What was changed: the entry symbol, and the four calls, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_3a7 13ac'.  All four target this
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

void Func_02001770(const SoftDouble *packed, SoftFloatRecord *out);
SoftFloatRecord *Func_02001158(SoftFloatRecord *a, SoftFloatRecord *b,
                               SoftFloatRecord *result);
SoftDouble Func_020015a4(SoftFloatRecord *result);

SoftDouble Func_020013ac(SoftDouble a, SoftDouble b)
{
    SoftDouble packedA = a;
    SoftDouble packedB = b;
    SoftFloatRecord recordA;
    SoftFloatRecord recordB;
    SoftFloatRecord result;

    Func_02001770(&packedA, &recordA);
    Func_02001770(&packedB, &recordB);

    return Func_020015a4(Func_02001158(&recordA, &recordB, &result));
}
