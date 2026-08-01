typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;

/*
 * Resource 3bf soft-float double addition at 0x02005a40.
 *
 * Frame map (sp-relative, 76 bytes):
 *   sp+0  .. sp+8   second operand, stored from r2/r3
 *   sp+8  .. sp+16  first operand, stored from r0/r1
 *   sp+16 .. sp+36  unpacked result record
 *   sp+36 .. sp+56  unpacked second operand
 *   sp+56 .. sp+76  unpacked first operand
 *
 * The three 20-byte records are the library's expanded form (see the sibling
 * integer conversion at 0x02005af0, which builds the same record: class word,
 * sign word, exponent word, then a 64-bit significand).  The routine unpacks
 * both operands, runs the arithmetic core, then repacks and returns the
 * packed pair in r0/r1.
 *
 * Word order: this library carries a double as r0 = high (sign/exponent) word
 * and r1 = low word.  That is confirmed by the literal pair at 0x02005b60 in
 * the integer conversion, which returns -2147483648.0 as r0 = 0xC1E00000,
 * r1 = 0.  It is therefore NOT the native little-endian layout of a C
 * `double`, so the packed value is carried here as SoftDouble (a u64 whose low
 * half is the r0 word) rather than as `double`.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention; none of them is reconstructed here.
 *
 * Uncertainty: the record layout beyond the sign word at offset 4 is taken
 * from the 0x02005af0 sibling, not from the unpack routines themselves, so it
 * is kept opaque here.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 word[5];
} SoftFloatRecord;

void Func_0200b85c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_0200b866(const SoftDouble *packed, SoftFloatRecord *record);
SoftFloatRecord *Func_0200b258(SoftFloatRecord *left, SoftFloatRecord *right, SoftFloatRecord *result);
SoftDouble Func_0200b6a8(SoftFloatRecord *record);

SoftDouble Func_02005a40(u32 a0, u32 a1, u32 b0, u32 b1)
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
