// Draft of Func_08196404, The Lost Age's BattleEffect_BuildBlitter: differ 241
// of 597 (tla-en, 2026-10-15; first written in the effects trial of the
// same day), on The Broken Seal's step list. Blocked under S1 as written:
// its copy helpers are inline asm, there only to measure the rest, and stay
// so until Pascal decides whether the four-register copies are a
// CAMELOT_ASM helper (plan D3). The Lost Age's builder is one pass: a fixed
// 436-byte block, short copies as ldmia/stmia pairs, longer ones by DMA, modes
// numbered 0 additive, 1 max, 2 transparent, default copy, and the second
// run copied from the first by its measured length. Three copies use four
// registers (ldmia r0!, {r2, r3, r4, r5}), which agscc never emits
// (thumb_expand_movstrqi has only movmem12b and movmem8b); and agscc's
// movmem12b/8b expose the advanced pointers, so CSE reuses them for dst
// (adds r6, r2, #0) where the native keeps adds r6, #12. Both copy forms
// are fixed-register asm helpers here only to measure the rest: from
// differ 627 (struct copies) to 252. Left: a store-flag return (native
// eors/negs/orrs/lsrs, here a branch; three forms tried), the DMA
// control 0x84000005 built as 5 | r5 from a kept 0x84000000, one spill
// slot fewer (frame 148 against 152 with a 136-byte local), and
// scheduling.
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"

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

#define PUT(word) *dst++ = (word)
// Trial: a four-word block copy, ldmia and stmia of four registers, which
// agscc never emits (thumb_expand_movstrqi has only 12- and 8-byte moves).
// A CAMELOT_ASM candidate for Pascal, written here to measure the rest.
static __inline__ void Copy4Words(const u32 *from, u32 *to) {
  register const u32 *s __asm__("r0") = from;
  register u32 *d __asm__("r1") = to;

  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+l"(s), "+l"(d)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}
static __inline__ void Copy3Words(const u32 *from, u32 *to) {
  register const u32 *s __asm__("r0") = from;
  register u32 *d __asm__("r1") = to;

  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"
                   : "+l"(s), "+l"(d)
                   :
                   : "r2", "r3", "r4", "memory");
}
static __inline__ void Copy2Words(const u32 *from, u32 *to) {
  register const u32 *s __asm__("r0") = from;
  register u32 *d __asm__("r1") = to;

  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"
                   : "+l"(s), "+l"(d)
                   :
                   : "r2", "r3", "memory");
}
#define COPY2(from) Copy2Words(from, dst)
#define COPY3(from) Copy3Words(from, dst)
#define COPY4(from) Copy4Words(from, dst)
#define COPY_DMA(from, n) Dma_Set(from, dst, DMA_COPY_WORDS(n), REG_DMA3)
#define PUT_BRANCH(target)                                                     \
  *dst = *src++ + BRANCH_FIELD(dst, target);                                   \
  dst++
#define PATCH(site) *(site) |= BRANCH_FIELD(site, dst)

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
  /* FAKEMATCH: trial, 136 unused bytes of the native stack frame */
  u32 frame[34];

  dst = HeapSlot_Alloc(slot, 436);
  src = BattleEffect_PatchTemplateCode;

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

  COPY_DMA(src, 6);
  dst += 6;
  src += 6;

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  COPY_DMA(src, 5);
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
  return 1 - (((u32)(src + 2) ^ (u32)Data_081973f0) != 0);
}
