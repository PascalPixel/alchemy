// Draft, differ 576 of 597, skeleton gap 90 (tla-en, 2026-11-01)
// Func_08196404 is The Lost Age's BattleEffect_BuildBlitter, The Broken Seal's
// step list (games/tbs/src/battle_effect/battle_effect/blitter.c) walked once:
// a fixed 436-byte block, short copies as ldmia/stmia pairs, longer ones by
// DMA, modes 0 additive, 1 max, 2 transparent, default copy, and the second
// run copied from the first by its measured length.
// Left, nearly all of it the block copies, which plain C cannot spell:
// - Four-word copies load four registers (ldmia r0!, {r2, r3, r4, r5}).
//   thumb_expand_movstrqi has only movmem12b and movmem8b, so a 16-byte struct
//   copy here is ldmia of three plus ldr/str.
// - Eight-byte copies always go through r0 and r1. A struct that size is moved
//   by pieces, and a packed one calls memcpy, so ldmia/stmia on r7 and r6 is
//   the nearest; at the last copy and one mode it is ldr/ldr/str/str.
// - After a 12-byte copy the native steps with adds r6, #12. Post-reload CSE
//   sees the pattern's advanced r1 and writes a register copy instead.
// The registers, the four-register list and the missing CSE look like a
// handwritten macro, as Dma_Set is. Whether that is a CAMELOT_ASM helper is
// Pascal's call. With r0/r1 inline asm helpers in COPY2 to COPY4 this
// draft measures differ 139, skeleton gap 49; they stay out of it until then,
// so this draft is plain C.
// Settled: PUT, PATCH and the block copies are do-while macros, as in the
// twin (PUT alone took 44 off the gap). control is a kept 0x84000000 so the
// second DMA builds 5 | r5. The return is the store-flag spelled out.
// Also left: the native holds 436 in dst's register first (adds r1, r6, #0;
// dst = (u32 *)436 reaches it but is a reused variable, and a size local
// folds, gap +5), one spill where the native keeps a high register, and a
// default-mode tail cross-jumped two instructions further than the native's.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"

struct Words2 {
  u32 w[2];
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

#define DMA_COPY_WORDS(n) (0x84000000 | (n))
#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

#define PUT(word)                                                              \
  do {                                                                         \
    *dst++ = (word);                                                           \
  } while (0)
#define COPY2(from)                                                            \
  do {                                                                         \
    *(struct Words2 *)dst = *(const struct Words2 *)(from);                    \
  } while (0)
#define COPY3(from)                                                            \
  do {                                                                         \
    *(struct Words3 *)dst = *(const struct Words3 *)(from);                    \
  } while (0)
#define COPY4(from)                                                            \
  do {                                                                         \
    *(struct Words4 *)dst = *(const struct Words4 *)(from);                    \
  } while (0)
#define COPY_DMA(from, n) Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3)
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
  u32 *cell;
  s32 runSize;
  u32 *runCopy;
  u32 control;
  /* FAKEMATCH: trial, 136 unused bytes of the native stack frame */
  u32 frame[34];

  src = BattleEffect_PatchTemplateCode;
  dst = HeapSlot_Alloc(slot, 436);

  COPY4(src);
  dst += 4;
  src += 4;
  if (mode == 0) {
    COPY3(BattleEffect_Mask);
    dst += 3;
  }

  if ((flags & 12) == 4) {
    COPY3(src);
    dst += 3;
  }
  src += 3;
  if ((flags & 12) == 8) {
    COPY4(src);
    dst += 4;
  }
  src += 4;
  if ((flags & 12) == 12) {
    COPY3(src);
    dst += 3;
  }
  src += 3;
  if ((flags & 12) == 0) {
    PUT(src[0]);
  }
  src += 1;

  if ((flags & 2) != 0) {
    COPY2(src);
    dst += 2;
    src += 2;
    if ((flags & 8) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY2(src);
    dst += 2;
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
    PUT(src[0] + (1 << widthShift) - 1);
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
  COPY2(src);
  dst += 2;
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
  default:
    if ((flags & 4) != 0) {
      COPY2(BattleEffect_CopyReverse);
    } else {
      COPY2(BattleEffect_CopyForward);
    }
    dst += 2;
    break;
  case 2:
    if ((flags & 4) != 0) {
      COPY4(BattleEffect_TransparentReverse);
    } else {
      COPY4(BattleEffect_TransparentForward);
    }
    dst += 4;
    break;
  case 1:
    if ((flags & 4) != 0) {
      COPY4(BattleEffect_MaxReverse);
    } else {
      COPY4(BattleEffect_MaxForward);
    }
    dst += 4;
    break;
  case 0:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_AdditiveReverse, 6);
    } else {
      COPY_DMA(BattleEffect_AdditiveForward, 6);
    }
    dst += 6;
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
  COPY2(src);
  dst += 2;
  src += 2;

  cell = dst;
  switch (mode) {
  default:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Copy8Reverse, 16);
    } else {
      COPY_DMA(BattleEffect_Copy8Forward, 16);
    }
    dst += 16;
    break;
  case 2:
    if ((flags & 4) != 0) {
      COPY_DMA(BattleEffect_Transparent8Reverse, 25);
    } else {
      COPY_DMA(BattleEffect_Transparent8Forward, 25);
    }
    dst += 25;
    break;
  case 1: {
    const u32 *part;
    u32 control;

    if ((flags & 4) != 0) {
      part = BattleEffect_Max4Reverse;
    } else {
      part = BattleEffect_Max4Forward;
    }
    control = DMA_COPY_WORDS(16);
    Dma_Set(part, dst, control, REG_DMA3);
    dst += 16;
    Dma_Set(part, dst, control, REG_DMA3);
    dst += 16;
    break;
  }
  case 0: {
    const u32 *part;
    u32 control;

    if ((flags & 4) != 0) {
      part = BattleEffect_ColorReverse;
    } else {
      part = BattleEffect_ColorForward;
    }
    control = DMA_COPY_WORDS(14);
    Dma_Set(part, dst, control, REG_DMA3);
    dst += 14;
    Dma_Set(part, dst, control, REG_DMA3);
    dst += 14;
    break;
  }
  }

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
    PUT(src[1]);
  }
  src += 2;

  PUT(*src++);
  PUT_BRANCH(cell);
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

  COPY3(src);
  dst += 3;
  src += 3;

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  COPY2(src);
  {
    u32 mismatch = (u32)(src + 2) ^ (u32)Data_081973f0;

    return 1 - ((-mismatch | mismatch) >> 31);
  }
}
