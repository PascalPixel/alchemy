/* 2026-09-29 alchemy permute: score 1050 to 1005 on the permuter's scorer
   (0 is exact); remaining 10 register-only, 7 operand, 5 reordered, 2
   inserted, 3 deleted. Kept rewrites: 7x swap commutative operands, 7x
   reorder independent statements, 4x change loop form, 3x split or join a
   compound assignment, 2x reorder local declarations, 2x introduce a
   temporary, 2x add a same-width cast, 2x pointer arithmetic or indexing,
   2x toggle register, 1x remove a temporary, 1x drop a same-width cast, 1x
   test truth or compare with zero. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
/* 2026-09-29 alchemy permute: score 1360 to 1050 on the permuter's scorer
   (0 is exact); remaining 11 register-only, 6 operand, 6 reordered, 2
   inserted, 3 deleted. Kept rewrites: 14x swap commutative operands, 12x
   introduce a temporary, 11x reorder independent statements, 11x reorder
   local declarations, 11x split or join a compound assignment, 8x pointer
   arithmetic or indexing, 6x change loop form, 5x add a same-width cast,
   4x remove a temporary, 4x drop a same-width cast, 1x toggle register, 1x
   test truth or compare with zero. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
/* Draft, not exact: 136 of 136 bytes, 37 differing halfwords / 34 edits.
   Fresh scoring shows the fill routine is hoisted into fp, not reloaded
   per row as the old header claimed; the reference keeps DMA control in fp
   and reloads the routine into r3. The row pointer and fill size therefore
   also get different saved registers/spills. The exact item-menu pattern
   (IwramIrqMain + 0x168) emits identical bytes, as does changing the fill
   result to void. Stop that call-address axis without new loop evidence. */
#include "DMA.H"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    ((FillWordsFn)0x03000168)(dst, size, value);
}

void UiWork_ShiftPanelRowsLeft(s32 step)
{
    u32 *row;
    u32 *source;
    s32 size;
    u32 *destination;
    register u8 *tmp4;
    register s32 n;
    s32 shift;
    u32 *tmp2;

    destination = (u32 *)0x06002520;
    shift = step * 6;
    tmp4 = (u8 *)destination;
    size = 4 * shift;
    source = (u32 *)&tmp4[size];
    n = 29;
    tmp2 = (u32 *)0x06002500;
    row = tmp2;
    if (n >= 0) {
        while (1) {
            register u32 *tmp;
            u32 *tmp3;
            u32 *tmp5;
            s32 tmp6;
            u32 *tmp7;
            u32 tmp8;
            s32 tmp10;
            tmp10 = (u32)32;
            tmp8 = 0x84000000 | (24 - shift);
            (u32)(tmp6 = tmp10 - shift);
            Dma_Set(source, destination, tmp8, (volatile u32 *)0x040000d4);
            source = 32 + source;
            tmp3 = &row[tmp6];
            tmp = tmp3;
            tmp7 = 32 + destination;
            tmp5 = row + 32;
            FillWords(tmp, size, 0);
            --n;
            row = tmp5;
            destination = tmp7;
            if (0 > n)
                break;
        }
    }
}
