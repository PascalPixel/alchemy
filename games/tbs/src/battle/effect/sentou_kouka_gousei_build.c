#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "dma.h"
#include "battle_efx.h"
#include "io_reg.h"

#define DMA3_REGISTERS (REG_DMA3)
#define DMA_COPY_WORDS(n) (0x84000000 | (n))

#define BRANCH_FIELD(site, target) \
    ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)


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

#define PUT(word) words++
#define COPY(n) words += (n); src += (n)
#define COPY_FROM(from, n) words += (n)
#define COPY_TWICE(from, n) words += (n); words += (n)
#define PUT_BRANCH(target) words++
#define MARK(site)
#define PATCH(site)

    words = 0;
    src = SentouKouka_Gousei;
#include "sentou_kouka_gousei_steps.inc"

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

    /* FAKEMATCH: needed to match: one-pass blocks keep each writing step together and in list order while allowing scheduling within each step */
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
#define PUT_BRANCH(target) do { \
        *dst = *src++ + BRANCH_FIELD(dst, target); \
        dst++; \
    } while (0)
#define MARK(site) site = dst
#define PATCH(site) do { *(site) |= BRANCH_FIELD(site, dst); } while (0)

    dst = (u32 *)Runtime_AllocateHeapBlock(slot, words << 2);
    src = SentouKouka_Gousei;
#include "sentou_kouka_gousei_steps.inc"

#define ParticleStreams_CellOffsets ((const u32 (*))&ParticleStreams_CellOffsets)

    end = ParticleStreams_CellOffsets;
    return src == end;
}
