#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "DMA.H"
#include "BATTLE_EFX.H"

/*
 * Builds one rectangle blitter out of the instruction templates of
 * SENTOU_KOUKA_GOUSEI.S and leaves it in a heap slot, where the battle
 * effects fetch it and call it as a BattleEffectDrawRectangle.
 *
 * The canvas such a blitter draws on is one byte a pixel, stored cell by
 * cell (8 by 8 pixels, 64 bytes), 1 << width_shift pixels wide and
 * 1 << height_shift high.  `flags` says what happens at its edges and which
 * way the source is read, `mode` what a pixel does when it lands.
 *
 * The work is the step list of SENTOU_KOUKA_GOUSEI_STEPS.INC, run twice with
 * two sets of steps: the first only counts the words, so that the slot can
 * be sized, and the second writes them.
 */

#define DMA3_REGISTERS ((volatile u32 *)0x040000d4)
/* Enable, 32-bit units, this many of them. */
#define DMA_COPY_WORDS(n) (0x84000000 | (n))

/* How far the ARM branch at `site` has to go to land on `target`. */
#define BRANCH_FIELD(site, target) \
    ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)


extern const u32 SentouKouka_YomiGyaku[];
extern const u32 SentouKouka_YomiJun[];
extern const u32 SentouKouka_NuriJun[];
extern const u32 SentouKouka_NuriGyaku[];
extern const u32 SentouKouka_Nuri8Jun[];
extern const u32 SentouKouka_Nuri8Gyaku[];
extern const u32 SentouKouka_HikakuJun[];
extern const u32 SentouKouka_HikakuGyaku[];
extern const u32 SentouKouka_Hikaku4Jun[];
extern const u32 SentouKouka_Hikaku4Gyaku[];
extern const u32 SentouKouka_KasanJun[];
extern const u32 SentouKouka_KasanGyaku[];
extern const u32 SentouKouka_Mask[];
extern const u32 SentouKouka_Gousei[];
extern const u32 SentouKouka_IroJun[];
extern const u32 SentouKouka_IroGyaku[];
/* 1 << n, as the operand field of an ARM instruction holds it. */
extern const u16 SentouKouka_BitOperands[];
/* The table that follows the Gousei template: where the template ends. */
extern const u32 ParticleStreams_CellOffsets[];

s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode)
{
    const u32 *src;
    u32 *dst;
    u32 *no_rows;
    u32 *no_columns;
    u32 *row;
    u32 *skip;
    u32 *run;
    u32 *cell;
    const u32 *end;
    s32 words;

    /* First the steps that count. */
#define PUT(word) words++
#define COPY(n) words += (n); src += (n)
#define COPY_FROM(from, n) words += (n)
#define COPY_TWICE(from, n) words += (n); words += (n)
#define PUT_BRANCH(target) words++
#define MARK(site)
#define PATCH(site)

    words = 0;
    src = SentouKouka_Gousei;
#include "SENTOU_KOUKA_GOUSEI_STEPS.INC"

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

    /* FAKEMATCH: each writing step is a do-while statement of its own. The
       reference keeps every step's instructions together and in the order
       of the list, though the scheduler moves instructions freely inside a
       step and all through the counting pass; only the loop notes such a
       statement leaves behind stop it. */
#define PUT(word) do { *dst++ = (word); } while (0)
#define COPY(n) do { \
        Dma_Set(src, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
        src += (n); \
    } while (0)
#define COPY_FROM(from, n) do { \
        Dma_Set(from, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
    } while (0)
#define COPY_TWICE(from, n) do { \
        const u32 *part = (from); \
        Dma_Set(part, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
        Dma_Set(part, dst, DMA_COPY_WORDS(n), DMA3_REGISTERS); \
        dst += (n); \
    } while (0)
/* A branch back to `target`. */
#define PUT_BRANCH(target) do { \
        *dst = *src++ + BRANCH_FIELD(dst, target); \
        dst++; \
    } while (0)
/* Remember where the next word goes. */
#define MARK(site) site = dst
/* Make the branch written at `site` land on the next word. */
#define PATCH(site) do { *(site) |= BRANCH_FIELD(site, dst); } while (0)

    dst = (u32 *)Runtime_AllocateHeapBlock(slot, words << 2);
    src = SentouKouka_Gousei;
#include "SENTOU_KOUKA_GOUSEI_STEPS.INC"

    /* Every word of the template was either taken or passed over. */
    end = ParticleStreams_CellOffsets;
    return src == end;
}
