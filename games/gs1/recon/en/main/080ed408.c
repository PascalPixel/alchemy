#include "types.h"

#define Blit_BuildRoutine Func_080ed408

/*
 * Runtime code assembler at 0x080ed408 (1648 bytes, one function).
 *
 * Callers spell it Func_080ed408(id, 7, 7, flags, mode) -- see
 * games/gs1/recon/en/main/080c91dc.c, 080ca60c.c, 080cb1a4.c, 080ca1fc.c and
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
 * Measured state (alchemy score --owner 080ed408): candidate 1612 bytes
 * against a 1648-byte reference, 779 differing halfwords, 1086 wrong
 * instructions.  Every reference operation is present, in reference order,
 * and the template walk is proved: the 97 `src` words this file consumes
 * are exactly Data_080ede48 - Data_080edcc4, which is what the reference's
 * own return value checks.  The single BL (Runtime_AllocateHeapBlock) and
 * both u16 Data_080ef034 loads land on their reference instructions.
 *
 * The whole residual is one unreproduced code shape, repeated at every one
 * of the reference's twenty store-multiple DMA sites (this file spells
 * twenty-two, because the reference merges the shared tails of the mode 1
 * and mode 2 arms).  The reference writes the three DMA3 words as
 *
 *     adds r0, r6, #0 / adds r1, r5, #0 / ldr r2, <control>
 *     stmia r3!, {r0, r1, r2} / subs r3, #12
 *
 * -- a store-multiple into hard registers r0, r1, r2 with the Thumb-forced
 * writeback undone afterwards.  Ordinary C on this route emits three plain
 * `str rN, [base, #k]` instead.  Five spellings have been measured:
 *   - this struct-record assignment: 1612 bytes / 779 differing;
 *   - a `u32 *` with DMA3[0..2] index stores: 1696 / 817 (worse: the index
 *     form also costs the reference's `adds r5,#12` pointer bumps);
 *   - the same record qualified `volatile`: 1616 / 781, and the merge the
 *     reference performs is itself proof the original stores were not
 *     volatile-qualified;
 *   - mirroring the emit code's inner if/else in the word budget so the
 *     reference's unfolded `adds #2 / adds #2 / adds #5` survives: folded
 *     back to `adds #9`, no change;
 *   - three post-increment stores through a local `u32 *p = 0x040000D4`
 *     (the shape that produces `stmia rB!, {rV}` elsewhere in this
 *     function): 1672 / 802, the worst of the five.
 * A 12-byte stack record was ruled out without measuring: the reference
 * frame is 16 bytes and all four words are accounted for (three saved
 * emit positions plus the spilled `b`), so the stored values come from
 * registers, not memory.  Because that idiom pins one low register for the
 * DMA base here, this candidate has one fewer low register to spend and
 * parks `a`/`flags` in r7/r6 where the reference uses r9/r8 and spills `b`
 * to [sp, #12]; that allocation difference, not structure, is what the
 * remaining diff hunks show.
 *
 * The other known small divergence is the shape of the six `mode` decision
 * trees: GCC balances three case nodes onto pivot 2 here, while the
 * reference walks them 1 -> 2 -> 3 unbalanced.  Worth about two halfwords
 * per switch and not resolved.
 *
 * A third, smaller divergence, also unresolved: in the word budget the
 * reference keeps `adds #2 / adds #2 / adds #5` in three separate blocks
 * and re-tests `flags & 1` instead of turning the two `flags & 1` guards
 * into one if/else, which is what GCC does here.
 */

typedef struct DmaChannel {
    const void *src;
    void *dst;
    u32 cnt;
} DmaChannel;

/* DMA3 source/destination/control, written as one three-word record.  The
   control word is enable | 32-bit unit | word count. */
#define DMA3 (*(DmaChannel *)0x040000D4)
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
    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = DMA_WORDS(3);
    dst += 3;
    src += 3;
    if (mode == 3) {
        DMA3.src = Data_080edcb8;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(3);
        dst += 3;
    }

    /* Source-format prologue: one of four variants, or a single word. */
    if ((flags & 12) == 4) {
        DMA3.src = src;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(3);
        dst += 3;
    }
    src += 3;
    if ((flags & 12) == 8) {
        DMA3.src = src;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(4);
        dst += 4;
    }
    src += 4;
    if ((flags & 12) == 12) {
        DMA3.src = src;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(3);
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

    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = DMA_WORDS(6);
    dst += 6;
    src += 6;

    *dst++ = *src++;
    if ((flags & 1) == 0) {
        *dst++ = src[0];
    }
    src += 1;

    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = DMA_WORDS(5);
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
        DMA3.src = (flags & 4) != 0 ? Data_080edb10 : Data_080edb00;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(4);
        dst += 4;
        break;
    case 2:
        DMA3.src = (flags & 4) != 0 ? Data_080edbf8 : Data_080edbe8;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(4);
        dst += 4;
        break;
    case 3:
        DMA3.src = (flags & 4) != 0 ? Data_080edca0 : Data_080edc88;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(6);
        dst += 6;
        break;
    default:
        DMA3.src = (flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(2);
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
        DMA3.src = (flags & 4) != 0 ? Data_080edb84 : Data_080edb20;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(25);
        dst += 25;
        break;
    case 2:
        DMA3.src = (flags & 4) != 0 ? Data_080edc48 : Data_080edc08;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(16);
        dst += 16;
        DMA3.src = (flags & 4) != 0 ? Data_080edc48 : Data_080edc08;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(16);
        dst += 16;
        break;
    case 3:
        DMA3.src = (flags & 4) != 0 ? Data_080eefdc : Data_080eefa4;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(14);
        dst += 14;
        DMA3.src = (flags & 4) != 0 ? Data_080eefdc : Data_080eefa4;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(14);
        dst += 14;
        break;
    default:
        for (i = 0; i <= 7; i++) {
            DMA3.src = (flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8;
            DMA3.dst = dst;
            DMA3.cnt = DMA_WORDS(2);
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
        DMA3.src = (flags & 4) != 0 ? Data_080edb10 : Data_080edb00;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(4);
        dst += 4;
        break;
    case 2:
        DMA3.src = (flags & 4) != 0 ? Data_080edbf8 : Data_080edbe8;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(4);
        dst += 4;
        break;
    case 3:
        DMA3.src = (flags & 4) != 0 ? Data_080edca0 : Data_080edc88;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(6);
        dst += 6;
        break;
    default:
        DMA3.src = (flags & 4) != 0 ? Data_080edaf0 : Data_080edaf8;
        DMA3.dst = dst;
        DMA3.cnt = DMA_WORDS(2);
        dst += 2;
        break;
    }
    src += 4;

    *dst++ = *src++;
    *dst = *src++ + BRANCH_OFFSET(dst, mark);
    dst++;
    *fix |= BRANCH_OFFSET(fix, dst);

    DMA3.src = src;
    DMA3.dst = dst;
    DMA3.cnt = DMA_WORDS(3);
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
