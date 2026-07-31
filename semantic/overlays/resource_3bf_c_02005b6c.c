typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;

/*
 * Resource 3bf soft-float double to signed-integer truncation at 0x02005b6c.
 *
 * The packed operand arrives in r0/r1 and is spilled to sp+0 so the unpack
 * routine can take its address; the 20-byte unpacked record lands at sp+8.
 * The record layout is the one established by the sibling conversion at
 * 0x02005af0 (class, sign, exponent, significand low, significand high), and
 * the packed word order is likewise the library's own: r0 carries the HIGH
 * word of the double, which is why the operand is typed SoftDouble here and
 * not `double`.
 *
 * Decision sequence, in assembly order:
 *   - the first two class queries (0x0200b77c, 0x0200b766) each force a 0
 *     result when they report the operand is not a usable finite value;
 *   - the third query (0x0200b784) diverts to the saturating path;
 *   - otherwise the exponent field is range-checked: negative means the
 *     magnitude is below 1 and the result is 0, above 30 saturates;
 *   - the in-range case right-shifts the 64-bit significand by (60 - exponent)
 *     using the same 64-bit shift helper shape as 0x02005c08 and negates the
 *     result when the sign word is set.
 *
 * The saturating path is written branchlessly in the original:
 *   `negs r0,sign; orrs r0,sign; lsrs r0,#31; adds r0,#0x7fffffff`
 * which yields 0x7fffffff for a clear sign word and 0x80000000 (INT_MIN) for
 * any non-zero sign word.
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention (see the note on the module's link base below);
 * none of them is reconstructed here.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c, and every entry in it likewise
 * points 0x8000 past the case body it selects.  Both the existing byte-exact
 * sources under assets/code and this file keep the printed-offset spelling for
 * symbol names and the raw pool value for absolute data addresses, which is
 * self-consistent; nothing here depends on which of the two bases is real.
 */
typedef u64 SoftDouble;

typedef struct SoftFloatRecord {
    u32 cls;
    u32 sign;
    s32 exponent;
    u32 low;
    u32 high;
} SoftFloatRecord;

s32 Func_02005bd8();
s32 Func_02005be8();
s32 Func_02005bf8();
void Func_02005c08();
void Func_02005e04();
void Func_0200b982(const SoftDouble *packed, SoftFloatRecord *out);
s32 Func_0200b77c(const SoftFloatRecord *record);
s32 Func_0200b766(const SoftFloatRecord *record);
s32 Func_0200b784(const SoftFloatRecord *record);
u64 Func_0200b7ce(u64 significand, u32 count);

s32 Func_02005b6c(SoftDouble value)
{
    SoftDouble packed = value;
    SoftFloatRecord record;
    u32 magnitude;
    int saturate;

    Func_02005e04(&packed, &record);

    if (Func_02005bf8(&record) != 0) {
        return 0;
    }
    if (Func_02005bd8(&record) != 0) {
        return 0;
    }

    saturate = Func_02005be8(&record) != 0;
    if (!saturate) {
        if (record.exponent < 0) {
            return 0;
        }
        saturate = record.exponent > 30;
    }

    if (saturate) {
        u32 sign = record.sign;

        return (s32)((((u32)(0u - sign) | sign) >> 31) + 0x7fffffffu);
    }

    magnitude = (u32)Func_02005c08(((u64)record.high << 32) | record.low,
                                   (u32)(60 - record.exponent));

    if (record.sign != 0u) {
        return -(s32)magnitude;
    }
    return (s32)magnitude;
}
