// Draft, differ 400 of 597, skeleton gap 68 (tla-en, 2026-11-01)
// Func_08196404 is The Lost Age's BattleEffect_BuildBlitter, The Broken Seal's
// step list (games/tbs/src/battle_effect/battle_effect/blitter.c) walked once:
// a fixed 436-byte block, short copies as ldmia/stmia pairs, longer ones by
// DMA, modes 0 additive, 1 max, 2 transparent, default copy, and the second
// run copied from the first by its measured length.
// Left, nearly all of it the block copies, which plain C cannot spell:
// - The native copies always go r0 to r1 with scratch from r2 up. A struct copy
//   reaches movmem12b/movmem8b, but local-alloc gives in and out r3 and r2, the
//   scratch becomes r0, r4, r5, and flags & 12 and the 0x00ffffff mask then stay
//   in low registers where the native needs ip and a second pool load. Most of
//   differ 400 is this register swap.
// - Four-word copies load four registers (ldmia r0!, {r2, r3, r4, r5}), here and
//   in 0801399c only. thumb_expand_movstrqi has only movmem12b and movmem8b, so
//   a 16-byte struct copy is ldmia of three plus ldr/str (gap about 6).
// - After a copy the native steps with adds r6, #12 and adds r7, #12.
//   Post-reload CSE (cselib) sees the pattern's advanced r0 and r1 and writes
//   register copies (gap about 30). Advancing dst before the copy is worse
//   (gap 98), and do-while blocks do not stop it: only a label or volatile asm
//   clears cselib.
// All three would follow from a handwritten macro, as Dma_Set is, or a block
// move with fixed registers. That is Pascal's call (S1, K3); earlier r0/r1
// inline asm helpers measured differ 139, gap 49 and stay out of this draft.
// Settled: an 8-byte struct must be BLKmode to reach movmem8b (MOVE_RATIO is 2),
// which struct Words2's odd arrays give; u32 w[2] is DImode and ldmias on r7
// and r6. COPY_FROM, COPY_DMA and COPY_TWICE are the twin's plain statements,
// with COPY_TWICE in each arm: its longer RTL moves live lengths so that runCopy
// outranks runSize (differ 580 to 400). The partial run's modes default, 2 and 1
// choose a part first, because arms that each copy get different scratch and do
// not cross-jump. run and cell are one variable (lr). PUT and PATCH are do-while
// macros, as in the twin. control is a kept 0x84000000 so the second DMA builds
// 5 | r5. The return is the store-flag spelled out.
// Also left: the native holds 436 in dst's register first (adds r1, r6, #0;
// dst = (u32 *)436 reaches it but is a reused variable, and a size local folds),
// the size block's register pair, and sched2 order at the second DMA.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"

struct Words2 {
  u8 head[3];
  u8 rest[5];
};
struct Words3 {
  u32 w[3];
};
struct Words4 {
  u32 w[4];
};

extern const u32 BattleEffect_PatchTemplateCode[];
extern const u32 BattleEffect_Mask[];
extern const u16 Data_081973f0[];
extern const u32 BattleEffect_CopyReverse[];
extern const u32 BattleEffect_CopyForward[];
extern const u32 BattleEffect_Copy8Reverse[];
extern const u32 BattleEffect_Copy8Forward[];
extern const u32 BattleEffect_TransparentReverse[];
extern const u32 BattleEffect_TransparentForward[];
extern const u32 BattleEffect_Transparent8Reverse[];
extern const u32 BattleEffect_Transparent8Forward[];
extern const u32 BattleEffect_MaxReverse[];
extern const u32 BattleEffect_MaxForward[];
extern const u32 BattleEffect_Max4Reverse[];
extern const u32 BattleEffect_Max4Forward[];
extern const u32 BattleEffect_AdditiveReverse[];
extern const u32 BattleEffect_AdditiveForward[];
extern const u32 BattleEffect_ColorReverse[];
extern const u32 BattleEffect_ColorForward[];

#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

#define PUT(word)                                                              \
  do {                                                                         \
    *dst++ = (word);                                                           \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  *(struct Words##n *)dst = *(const struct Words##n *)(from);                  \
  dst += (n)
#define COPY_DMA(from, n)                                                      \
  Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3);                             \
  dst += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    u32 control = DMA_COPY_WORDS(n);                                           \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
    Dma_Set(part, dst, control, REG_DMA3);                                     \
    dst += (n);                                                                \
  }
#define PUT_BRANCH(target)                                                     \
  *dst = *src++ + BRANCH_FIELD(dst, target);                                   \
  dst++
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, dst);                                        \
  } while (0)

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode) {
  const u32 *src;
  u32 *dst;
  u32 *noRows;
  u32 *noColumns;
  u32 *row;
  u32 *skip;
  u32 *run;
  s32 runSize;
  u32 *runCopy;
  u32 control;
  // Unused: gives the stack frame its 136 bytes.
  u32 frame[34];

  src = BattleEffect_PatchTemplateCode;
  dst = HeapSlot_Alloc(slot, 436);

  COPY_FROM(src, 4);
  src += 4;
  if (mode == 0) {
    COPY_FROM(BattleEffect_Mask, 3);
  }

  if ((flags & 12) == 4) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 8) {
    COPY_FROM(src, 4);
  }
  src += 4;
  if ((flags & 12) == 12) {
    COPY_FROM(src, 3);
  }
  src += 3;
  if ((flags & 12) == 0) {
    PUT(src[0]);
  }
  src += 1;

  if ((flags & 2) != 0) {
    COPY_FROM(src, 2);
    src += 2;
    if ((flags & 8) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY_FROM(src, 2);
    src += 2;
    PUT(src[0] + (1 << heightShift));
    PUT(src[1] + (1 << heightShift));
    src -= 6;
  }
  src += 8;

  PUT(*src++);
  noRows = dst;
  PUT(*src++);

  if ((flags & 1) == 0) {
    s32 width = 1 << widthShift;

    PUT(src[0] + width - 1);
  }
  src += 1;
  if ((flags & 1) != 0) {
    PUT(src[0]);
    PUT(src[1]);
    if ((flags & 4) != 0) {
      PUT(src[2]);
      PUT(src[3]);
    } else {
      PUT(src[4]);
      PUT(src[5]);
    }
    PUT(src[6]);
    PUT(src[7]);
    PUT(src[8] + Data_081973f0[widthShift]);
    if ((flags & 4) != 0) {
      PUT(src[9]);
    } else {
      PUT(src[10]);
    }
    PUT(src[11] + Data_081973f0[widthShift]);
  }
  src += 12;

  PUT(*src++);
  noColumns = dst;
  PUT(*src++);

  control = 0x84000000;
  Dma_Set(src, dst, control | 6, REG_DMA3);
  dst += 6;
  src += 6;

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  Dma_Set(src, dst, control | 5, REG_DMA3);
  dst += 5;
  src += 5;

  row = dst;
  PUT(src[0]);
  PUT(src[1] + (1 << (heightShift - 3)) - 1);
  PUT(src[2] + ((widthShift - 3) << 7));
  PUT(src[3]);
  src += 4;

  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  skip = dst + 1;
  COPY_FROM(src, 2);
  src += 2;
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;

  run = dst;
  runCopy = dst;
  switch (mode) {
  default: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_CopyReverse;
    } else {
      part = BattleEffect_CopyForward;
    }
    COPY_FROM(part, 2);
    break;
  }
  case 2: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_TransparentReverse;
    } else {
      part = BattleEffect_TransparentForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 1: {
    const u32 *part;

    if ((flags & 4) != 0) {
      part = BattleEffect_MaxReverse;
    } else {
      part = BattleEffect_MaxForward;
    }
    COPY_FROM(part, 4);
    break;
  }
  case 0:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_AdditiveReverse, 6);
    } else {
      COPY_DMA(BattleEffect_AdditiveForward, 6);
    }
    break;
  }

  PUT(*src++);
  PUT_BRANCH(run);
  runSize = (u8 *)dst - (u8 *)runCopy;
  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;
  PATCH(skip);

  skip = dst + 1;
  COPY_FROM(src, 2);
  src += 2;

  run = dst;
  switch (mode) {
  default:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Copy8Reverse, 16);
    } else {
      COPY_DMA(BattleEffect_Copy8Forward, 16);
    }
    break;
  case 2:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Transparent8Reverse, 25);
    } else {
      COPY_DMA(BattleEffect_Transparent8Forward, 25);
    }
    break;
  case 1:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_Max4Reverse, 16);
    } else {
      COPY_TWICE(BattleEffect_Max4Forward, 16);
    }
    break;
  case 0:
    if ((flags & 4) != 0) {
      COPY_TWICE(BattleEffect_ColorReverse, 14);
    } else {
      COPY_TWICE(BattleEffect_ColorForward, 14);
    }
    break;
  }

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;

  PUT(*src++);
  PUT_BRANCH(run);
  PATCH(skip);

  PUT(*src++);
  skip = dst;
  PUT(*src++);
  if ((flags & 4) != 0) {
    PUT(src[0]);
  } else {
    PUT(src[1]);
  }
  src += 2;

  {
    u32 size = (u32)runSize / 4 * 4;

    Dma_Set(runCopy, dst, DMA_COPY_WORDS((s32)size / 4), REG_DMA3);
    dst = (u32 *)((u8 *)dst + size);
  }
  PATCH(skip);

  COPY_FROM(src, 3);
  src += 3;

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  COPY_FROM(src, 2);
  {
    u32 mismatch = (u32)(src + 2) ^ (u32)Data_081973f0;

    return 1 - ((-mismatch | mismatch) >> 31);
  }
}
