#include "types.h"

/* Table at a fixed ROM address, indexed by an 8-byte stride: word 0 of each
   row is a data pointer, word 1 is a halfword adjustment array pointer.
   Inferred from the retained assembly's shift-by-3/register-offset ldr
   pattern (not a compile-time struct-field offset -- both fields are
   fetched through the same running byte offset), cross-checked against the
   ARM twin at games/gs1/asm/relocated_arm_helpers/08015430.s which
   references the same literal address and lookup shape. */
#define Data_0803842c ((const u8 *)0x0803842c)

struct Func_08019bfcState {
    u32 code;   /* in: context index (hi byte = table row, lo byte = column);
                   out: decoded value, also the return value */
    u8 *ptr;    /* persistent byte cursor for the "stream A" bit reader */
    s32 bits;   /* persistent bit buffer for "stream A" */
};

s32 Func_08019bfc(struct Func_08019bfcState *state) {
    u32 lo;
    u32 hi;
    u32 offset;
    const u8 *tableBase;
    const u16 *tableDeltas;
    const u8 *pos;
    const u8 *anchor;
    u8 *readPtr;
    s32 bufA;
    s32 bufB;
    s32 rank;
    s32 counter;
    s32 bitA;
    s32 bitB;
    s32 result;
    u32 idx;
    u32 half;
    s32 mask;
    s32 sentinel;
    s32 mask2;
    s32 sentinel2;

    hi = state->code >> 8;
    lo = state->code & 0xff;
    offset = hi << 3;
    tableBase = *(const u8 * const *)(Data_0803842c + offset);
    offset += 4;
    tableDeltas = *(const u16 * const *)(Data_0803842c + offset);
    pos = tableBase + tableDeltas[lo];

    readPtr = state->ptr;
    anchor = pos - 1;
    bufA = state->bits;
    bufB = 1;
    rank = 0;
    mask = 1;
    sentinel = 0x80;

    goto L0;

L7:
    bitA = bufA & mask;
    bufA >>= 1;
    if (bitA == 0)
        goto L0;
    if (bufA != 0)
        goto L1;
    bufA = *readPtr;
    bitA = bufA & mask;
    bufA >>= 1;
    bufA |= sentinel;
    readPtr++;

L1:
    if (bitA == 0)
        goto L0;

    mask2 = 1;
    sentinel2 = 0x80;
    counter = 0;
L6:
    bitB = bufB & mask2;
    bufB >>= 1;
    if (bitB == 0)
        goto L2;
    if (bufB != 0)
        goto L3;
    bufB = *pos;
    bitB = bufB & mask2;
    bufB >>= 1;
    pos++;
    bufB |= sentinel2;

L3:
    if (bitB != 0)
        goto L4;

L2:
    counter++;
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
    if (bitB == 0)
        goto L7;
    if (bufB != 0)
        goto L8;
    bufB = *pos;
    bitB = bufB & mask;
    bufB >>= 1;
    pos++;
    bufB |= sentinel;

L8:
    if (bitB == 0)
        goto L7;

    idx = (u32)rank * 3;
    if (((idx * 4) & 7) == 0) {
        half = idx >> 1;
        result = anchor[-(s32)half] << 4;
        result |= anchor[-(s32)half - 1] >> 4;
    } else {
        half = idx >> 1;
        result = (anchor[-(s32)half] & 0xf) << 8;
        result |= anchor[-(s32)half - 1];
    }

    state->bits = bufA;
    state->code = (u32)result;
    state->ptr = readPtr;
    return result;
}
