/* 2026-09-29 alchemy permute: score 1205 to 1170 on the permuter's scorer
   (0 is exact); remaining 79 register-only, 1 operand, 7 reordered, 1
   inserted, 2 deleted. Kept rewrites: 2x swap commutative operands, 1x
   reorder independent statements, 1x introduce a temporary. FAKEMATCH: the
   permuter's temporaries, register hints and swapped operand orders below
   only steer allocation and scheduling; no programmer would write them, so
   they stay tagged until a natural spelling replaces them. */
/* 2026-09-29 alchemy permute: score 2025 to 1205 on the permuter's scorer
   (0 is exact); remaining 79 register-only, 1 operand, 6 reordered, 1
   inserted, 3 deleted. Kept rewrites: 17x reorder independent statements,
   12x swap commutative operands, 11x reorder local declarations, 5x drop a
   same-width cast, 4x introduce a temporary, 4x add a same-width cast, 3x
   move an assignment into or out of a condition, 2x remove a temporary, 2x
   pointer arithmetic or indexing, 2x split or join a compound assignment,
   2x toggle register, 2x test truth or compare with zero. FAKEMATCH: the
   permuter's temporaries, register hints and swapped operand orders below
   only steer allocation and scheduling; no programmer would write them, so
   they stay tagged until a natural spelling replaces them. */
#include "TYPES.H"

/* Table at a fixed ROM address, indexed by an 8-byte stride: word 0 of each
   row is a data pointer, word 1 is a halfword adjustment array pointer.
   Inferred from the retained assembly's shift-by-3/register-offset ldr
   pattern (not a compile-time struct-field offset -- both fields are
   fetched through the same running byte offset), cross-checked against the
   ARM twin at games/THE BROKEN SEAL/SRC/GRAPHICS/TEXT/DECODE_SYMBOL.S which
   references the same literal address and lookup shape. */
extern const u8 Text_MessageContexts[];

struct Func_08019bfcState {
    u32 code;   /* in: context index (hi byte = table row, lo byte = column);
                   out: decoded value, also the return value */
    u8 *ptr;    /* persistent byte cursor for the "stream A" bit reader */
    s32 bits;   /* persistent bit buffer for "stream A" */
};

s32 Func_08019bfc(struct Func_08019bfcState *state)
{
    u32 lo;
    u32 hi;
    const u8 *tableBase;
    u32 offset;
    const u8 *pos;
    const u8 *anchor;
    s32 bufB;
    register u8 *readPtr;
    s32 bufA;
    s32 rank;
    s32 bitA;
    s32 counter;
    s32 result;
    s32 bitB;
    u32 idx;
    u32 half;
    s32 mask;
    s32 sentinel;
    s32 sentinel2;
    s32 tmp2;
    s32 mask2;
    s32 tmp3;
    u8 tmp4;

    hi = state->code >> 8;
    offset = hi << 3;
    lo = state->code & 0xff;
    tableBase = *(const u8 *const *)(offset + Text_MessageContexts);
    offset += 4;
    pos = tableBase + (*(const u16 *const *)(Text_MessageContexts + offset))[lo];
    mask = 1;
    anchor = pos - 1;
    bufA = state->bits;
    readPtr = state->ptr;
    rank = 0;
    bufB = 1;
    sentinel = 0x80;
    goto L0;
L7:
    bitA = mask & bufA;
    bufA = bufA >> 1;
    if (0 == bitA)
        goto L0;
    if (0 != bufA)
        goto L1;
    tmp4 = *readPtr;
    readPtr++;
    bufA = tmp4;
    bitA = mask & bufA;
    bufA >>= 1;
    bufA |= sentinel;
L1:
    if ((u32)(0 == bitA))
        goto L0;
    counter = 0;
    mask2 = 1;
    sentinel2 = (u32)0x80;
L6:
    bitB = bufB & mask2;
    tmp3 = bufB >> 1;
    bufB = tmp3;
    if (!bitB)
        goto L2;
    if (bufB)
        goto L3;
    bufB = *pos;
    bitB = bufB & mask2;
    pos++;
    bufB >>= 1;
    bufB |= sentinel2;
L3:
    if (bitB != 0)
        goto L4;
L2:
    (u32)counter++;
    goto L5;
L4:
    rank++;
    counter--;
L5:
    if (counter >= 0)
        goto L6;
L0:
    bitB = bufB & mask;
    bufB >>= 1;
    if (0 == bitB)
        goto L7;
    if (bufB != 0)
        goto L8;
    bufB = *pos;
    pos++;
    tmp2 = bufB & mask;
    bitB = tmp2;
    bufB >>= 1;
    bufB |= sentinel;
L8:
    if (bitB == 0)
        goto L7;
    if ((((idx = 3 * rank) * 4) & 7) == 0) {
        u8 tmp;
        half = idx >> 1;
        result = anchor[-(s32)half] << 4;
        tmp = anchor[-(s32)half - 1];
        result |= tmp >> 4;
    } else {
        half = idx >> 1;
        result = (anchor[-half] & 0xf) << 8;
        result |= anchor[-half - 1];
    }
    state->bits = bufA;
    state->code = result;
    state->ptr = readPtr;
    return result;
}
