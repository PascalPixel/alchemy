#include "TYPES.H"
#include "DMA.H"
#include "BATTLE_EFX.H"

#define Blit_BuildRoutine BattleEffect_LoadWork

/*
 * Runtime code assembler at 0x080ed408 (1648 bytes, one function).
 *
 * Callers spell it BattleEffect_LoadWork(id, 7, 7, flags, mode) -- see
 * games/THE BROKEN SEAL/recon/en/main/080c91dc.c, 080ca60c.c, 080cb1a4.c, 080ca1fc.c and
 * 080c972c.c, whose header already records that the routine this builds is
 * later fetched out of heap slot `id` and called through the
 * DrawRectangleFn signature.  What the function actually does is emit an
 * ARM (not Thumb) routine into a freshly allocated heap block:
 *
 *   - A word budget is counted first, exactly mirroring the emit sequence
 *     below, and Runtime_AllocateHeapBlock(id, words * 4) reserves the block.
 *   - `src` then walks a 97-word ARM instruction template that starts at
 *     Data_080edcc4 and ends at Data_080ede48.  Fixed runs are moved with a
 *     DMA3 32-bit transfer; single instructions are copied by hand so that
 *     immediate fields can be patched (`+ (1 << a)`, `+ Data_080ef034[a]`,
 *     `+ ((a - 3) << 7)`) or so that one of two encodings can be selected
 *     from `flags`.
 *   - Optional template slots are skipped by the `if`, but `src` still
 *     advances past them, so the template layout is fixed and only the
 *     emitted routine shrinks.
 *   - Six emitted positions are remembered (`mark`, `mark_c`, `mark_g`,
 *     `fix`, `br_a`, `br_b`) and the ARM branch displacement
 *     ((target - site - 8) >> 2, masked to 24 bits) is folded into the
 *     template word afterwards -- `+` where the template word carries no
 *     displacement yet, `|=` where the word was already emitted.
 *   - The return value is the template walk's own consistency check:
 *     1 when `src` landed exactly on Data_080ede48, 0 otherwise.
 *
 * Parameter roles that are established by use only:
 *   - `id` is the heap slot; it is passed straight through.
 *   - `a` feeds `1 << a`, `(a - 3) << 7` and the u16 table
 *     Data_080ef034[a]; it patches address/stride immediates.  Every
 *     caller but one passes 7; 080e7404.c passes 8.
 *   - `b` feeds `1 << b` and `1 << (b - 3)`.  Callers all pass 7.
 *   - `flags` bit 0, bit 1, bit 2, bit 3 and the two-bit field
 *     `flags & 12` each select template variants.  Callers pass literal
 *     0, 2, 3, 6, 7, 11 and 15, and also computed masks.
 *   - `mode` selects one of four inner-loop bodies.  Callers pass literal
 *     0, 1, 2 and 3, and also computed values.
 *
 * `mode` is spelled unsigned because all six of the reference's decision
 * trees over it guard their low bound with `bcc`, not `blt`.  The callers'
 * own local prototypes still spell that parameter `s32`; the reference
 * comparison is the stronger evidence and is what this file follows.
 *
 * Not established here: what the generated routine draws, what the four
 * `mode` bodies (Data_080edaf0/af8, db00/db10, dbe8/dbf8, dc88/dca0,
 * db20/db84, dc08/dc48, Data_080eefa4/efdc) are, or what the individual
 * flag bits mean.  They are left as raw addresses and raw bit tests.
 *
 */

/* DMA enable, 32-bit transfer width and word count. */
#define DMA_WORDS(n) (0x84000000 | (n))

/* ARM branch displacement folded into an already-assembled template word. */
#define BRANCH_OFFSET(site, target) \
    ((((u32)((s32)(target) - (s32)(site) - 8)) >> 2) & 0x00FFFFFF)

void *Runtime_AllocateHeapBlock(s32 id, s32 size);

extern const u32 Data_080edcc4[];
extern const u32 Data_080ede48[];
extern const u16 Data_080ef034[];
extern const u32 Data_080edaf0[];
extern const u32 Data_080edaf8[];
extern const u32 Data_080edb00[];
extern const u32 Data_080edb10[];
extern const u32 Data_080edb20[];
extern const u32 Data_080edb84[];
extern const u32 Data_080edbe8[];
extern const u32 Data_080edbf8[];
extern const u32 Data_080edc08[];
extern const u32 Data_080edc48[];
extern const u32 Data_080edc88[];
extern const u32 Data_080edca0[];
extern const u32 Data_080edcb8[];
extern const u32 Data_080eefa4[];
extern const u32 Data_080eefdc[];

s32 Blit_BuildRoutine(s32 id, s32 a, s32 b, s32 flags, u32 mode)
{
    const u32 *src;
    u32 *dst;
    u32 *mark;
    u32 *mark_c;
    u32 *mark_g;
    u32 *fix;
    u32 *br_a;
    u32 *br_b;
    s32 n;
    s32 i;

    /* Word budget.  Every term below pairs with one emit step further
       down, in the same order. */
    n = 3;
    if (mode == 3) {
        n += 3;
    }
    if ((flags & 12) == 4) {
        n += 3;
    }
    if ((flags & 12) == 8) {
        n += 4;
    }
    if ((flags & 12) == 12) {
        n += 3;
    }
    if ((flags & 12) == 0) {
        n += 1;
    }
    if ((flags & 2) != 0) {
        n += 7;
    }
    n += 2;
    if ((flags & 1) == 0) {
        n += 1;
    }
    if ((flags & 1) != 0) {
        n += 2;
        n += 2;
        n += 5;
    }
    n += 9;
    if ((flags & 1) == 0) {
        n += 1;
    }
    n += 9;
    if ((flags & 1) == 0) {
        n += 1;
    }
    n += 3;
    switch (mode) {
    case 1:
        n += 4;
        break;
    case 2:
        n += 4;
        break;
    case 3:
        n += 6;
        break;
    default:
        n += 2;
        break;
    }
    n += 3;
    if ((flags & 1) == 0) {
        n += 2;
    }
    n += 2;
    switch (mode) {
    case 1:
        n += 25;
        break;
    case 2:
        n += 32;
        break;
    case 3:
        n += 28;
        break;
    default:
        for (i = 0; i <= 7; i++) {
            n += 2;
        }
        break;
    }
    n += 1;
    if ((flags & 1) == 0) {
        n += 2;
    }
    n += 5;
    switch (mode) {
    case 1:
        n += 4;
        break;
    case 2:
        n += 4;
        break;
    case 3:
        n += 6;
        break;
    default:
        n += 2;
        break;
    }
    n += 8;

    dst = (u32 *)Runtime_AllocateHeapBlock(id, n << 2);
    src = Data_080edcc4;

    /* Entry sequence. */
    Dma_Set(src, dst, DMA_WORDS(3), (volatile u32 *)0x040000d4);
    dst += 3;
    src += 3;
    if (mode == 3) {
        Dma_Set(Data_080edcb8, dst, DMA_WORDS(3), (volatile u32 *)0x040000d4);
        dst += 3;
    }

    /* Source-format prologue: one of four variants, or a single word. */
    if ((flags & 12) == 4) {
        Dma_Set(src, dst, DMA_WORDS(3), (volatile u32 *)0x040000d4);
        dst += 3;
    }
    src += 3;
    if ((flags & 12) == 8) {
        Dma_Set(src, dst, DMA_WORDS(4), (volatile u32 *)0x040000d4);
        dst += 4;
    }
    src += 4;
    if ((flags & 12) == 12) {
        Dma_Set(src, dst, DMA_WORDS(3), (volatile u32 *)0x040000d4);
        dst += 3;
    }
    src += 3;
    if ((flags & 12) == 0) {
        *dst++ = src[0];
    }
    src += 1;

    if ((flags & 2) != 0) {
        *dst++ = src[0];
        *dst++ = src[1];
        *dst++ = (flags & 8) != 0 ? src[2] : src[3];
        *dst++ = src[4];
        *dst++ = src[5];
        *dst++ = src[6] + (1 << b);
        *dst++ = src[7] + (1 << b);
    }
    src += 8;

    *dst++ = *src++;
    br_a = dst;
    *dst++ = *src++;

    if ((flags & 1) == 0) {
        *dst++ = src[0] + (1 << a) - 1;
    }
    src += 1;
    if ((flags & 1) != 0) {
        *dst++ = src[0];
        *dst++ = src[1];
        if ((flags & 4) != 0) {
            *dst++ = src[2];
            *dst++ = src[3];
        } else {
            *dst++ = src[4];
            *dst++ = src[5];
        }
        *dst++ = src[6];
        *dst++ = src[7];
        *dst++ = src[8] + Data_080ef034[a];
        *dst++ = (flags & 4) != 0 ? src[9] : src[10];
        *dst++ = src[11] + Data_080ef034[a];
    }
    src += 12;

    *dst++ = *src++;
    br_b = dst;
    *dst++ = *src++;

    Dma_Set(src, dst, DMA_WORDS(6), (volatile u32 *)0x040000d4);
    dst += 6;
    src += 6;

    *dst++ = *src++;
    if ((flags & 1) == 0) {
        *dst++ = src[0];
    }
    src += 1;

    Dma_Set(src, dst, DMA_WORDS(5), (volatile u32 *)0x040000d4);
    dst += 5;
    src += 5;

    mark_c = dst;
    *dst++ = src[0];
    *dst++ = src[1] + (1 << (b - 3)) - 1;
    *dst++ = src[2] + ((a - 3) << 7);
    *dst++ = src[3];
    src += 4;

    if ((flags & 1) == 0) {
        *dst++ = src[0];
    }
    src += 1;

    *dst++ = *src++;
    fix = dst;
    *dst++ = *src++;
    *dst++ = (flags & 4) != 0 ? src[0] : src[1];
    src += 2;

    /* First per-mode body; the loop below branches back to it. */
    mark = dst;
    switch (mode) {
    case 1:
        Dma_Set((flags & 4) != 0 ? Data_080edb10 : Data_080edb00, dst, DMA_WORDS(4), (volatile u32 *)0x040000d4);
        dst += 4;
        break;
    case 2:
        Dma_Set((flags & 4) != 0 ? Data_080edbf8 : Data_080edbe8, dst, DMA_WORDS(4), (volatile u32 *)0x040000d4);
        dst += 4;
        break;
    case 3:
        Dma_Set((flags & 4) != 0 ? Data_080edca0 : Data_080edc88, dst, DMA_WORDS(6), (volatile u32 *)0x040000d4);
        dst += 6;
        break;
    default:
        Dma_Set((flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8, dst, DMA_WORDS(2), (volatile u32 *)0x040000d4);
        dst += 2;
        break;
    }
    src += 4;

    *dst++ = *src++;
    *dst = *src++ + BRANCH_OFFSET(dst, mark);
    dst++;
    *dst++ = *src++;
    if ((flags & 1) == 0) {
        *dst++ = src[0];
        *dst++ = src[1];
    }
    src += 2;
    *fix |= BRANCH_OFFSET(fix, dst);

    *dst++ = *src++;
    fix = dst;
    *dst++ = *src++;

    /* Second per-mode body; the run below it branches back here. */
    mark_g = dst;
    switch (mode) {
    case 1:
        Dma_Set((flags & 4) != 0 ? Data_080edb84 : Data_080edb20, dst, DMA_WORDS(25), (volatile u32 *)0x040000d4);
        dst += 25;
        break;
    case 2:
        Dma_Set((flags & 4) != 0 ? Data_080edc48 : Data_080edc08, dst, DMA_WORDS(16), (volatile u32 *)0x040000d4);
        dst += 16;
        Dma_Set((flags & 4) != 0 ? Data_080edc48 : Data_080edc08, dst, DMA_WORDS(16), (volatile u32 *)0x040000d4);
        dst += 16;
        break;
    case 3:
        Dma_Set((flags & 4) != 0 ? Data_080eefdc : Data_080eefa4, dst, DMA_WORDS(14), (volatile u32 *)0x040000d4);
        dst += 14;
        Dma_Set((flags & 4) != 0 ? Data_080eefdc : Data_080eefa4, dst, DMA_WORDS(14), (volatile u32 *)0x040000d4);
        dst += 14;
        break;
    default:
        for (i = 0; i <= 7; i++) {
            Dma_Set((flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8, dst, DMA_WORDS(2), (volatile u32 *)0x040000d4);
            dst += 2;
        }
        break;
    }
    src += 4;

    *dst++ = *src++;
    if ((flags & 1) == 0) {
        *dst++ = src[0];
        *dst++ = src[1];
    }
    src += 2;

    *dst++ = *src++;
    *dst = *src++ + BRANCH_OFFSET(dst, mark_g);
    dst++;
    *fix |= BRANCH_OFFSET(fix, dst);

    *dst++ = *src++;
    fix = dst;
    *dst++ = *src++;
    *dst++ = (flags & 4) != 0 ? src[0] : src[1];
    src += 2;

    /* Third per-mode body, same four variants as the first. */
    mark = dst;
    switch (mode) {
    case 1:
        Dma_Set((flags & 4) != 0 ? Data_080edb10 : Data_080edb00, dst, DMA_WORDS(4), (volatile u32 *)0x040000d4);
        dst += 4;
        break;
    case 2:
        Dma_Set((flags & 4) != 0 ? Data_080edbf8 : Data_080edbe8, dst, DMA_WORDS(4), (volatile u32 *)0x040000d4);
        dst += 4;
        break;
    case 3:
        Dma_Set((flags & 4) != 0 ? Data_080edca0 : Data_080edc88, dst, DMA_WORDS(6), (volatile u32 *)0x040000d4);
        dst += 6;
        break;
    default:
        Dma_Set((flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8, dst, DMA_WORDS(2), (volatile u32 *)0x040000d4);
        dst += 2;
        break;
    }
    src += 4;

    *dst++ = *src++;
    *dst = *src++ + BRANCH_OFFSET(dst, mark);
    dst++;
    *fix |= BRANCH_OFFSET(fix, dst);

    Dma_Set(src, dst, DMA_WORDS(3), (volatile u32 *)0x040000d4);
    dst += 3;
    src += 3;

    *dst = *src++ + BRANCH_OFFSET(dst, mark_c);
    dst++;
    *br_a |= BRANCH_OFFSET(br_a, dst);
    *br_b |= BRANCH_OFFSET(br_b, dst);

    *dst++ = *src++;
    *dst = *src++;

    return src == Data_080ede48;
}
