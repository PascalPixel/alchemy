typedef unsigned int u32;
typedef unsigned long long u64;

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

int Func_020057ec();
int Func_02005c38();
void Func_02005e04();

SoftDouble Func_02005a40(SoftDouble a, SoftDouble b)
{
    SoftDouble packedA = a;
    SoftDouble packedB = b;
    SoftFloatRecord recordA;
    SoftFloatRecord recordB;
    SoftFloatRecord result;

    Func_02005e04(&packedA, &recordA);
    Func_02005e04(&packedB, &recordB);

    return Func_020057ec(Func_02005c38(&recordA, &recordB, &result));
}
