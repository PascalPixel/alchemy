/// @file
/// Func_08196404 walks The Lost Age's blitter template once and writes the
/// blitter's ARM code into a heap slot: The Broken Seal's step list
/// (battle_effect/blitter.c) with short copies as block moves and longer ones
/// by DMA, modes 0 additive, 1 max, 2 transparent, default copy, and the
/// second run copied from the first by its measured length.

#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"

CAMELOT_ASM("ldmia and stmia of two registers from r0 to r1 with scratch from "
            "r2 up, in the shape of Dma_Set: agscc's block moves leave r0 and "
            "r1 to the allocator")
static __inline__ void CopyWords2(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3}\n\tstmia %1!, {r2, r3}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "memory");
}

CAMELOT_ASM("ldmia and stmia of three registers from r0 to r1 with scratch "
            "from r2 up, in the shape of Dma_Set: agscc's block moves leave r0 "
            "and r1 to the allocator")
static __inline__ void CopyWords3(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4}\n\tstmia %1!, {r2, r3, r4}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "memory");
}

CAMELOT_ASM("ldmia and stmia of four registers: agscc's block moves stop at "
            "three, so a 16-byte struct copy compiles to 12 and 4")
static __inline__ void CopyWords4(const void *source, void *destination) {
  register const void *from __asm__("r0") = source;
  register void *to __asm__("r1") = destination;
  __asm__ volatile("ldmia %0!, {r2, r3, r4, r5}\n\tstmia %1!, {r2, r3, r4, r5}"
                   : "+r"(from), "+r"(to)
                   :
                   : "r2", "r3", "r4", "r5", "memory");
}

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
  CopyWords##n(from, dst);                                                     \
  dst += (n)
#define COPY(n)                                                                \
  do {                                                                         \
    COPY_FROM(src, n);                                                         \
    src += (n);                                                                \
  } while (0)
#define COPY_LONG(n)                                                           \
  do {                                                                         \
    Dma_Set(src, dst, control | (n), REG_DMA3);                                \
    dst += (n);                                                                \
    src += (n);                                                                \
  } while (0)
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
  /* FAKEMATCH(temporary): dst holds the block's size before its address; as
   * HeapSlot_Alloc(slot, 436), 31 of 597 instructions differ in tla-en. */
  dst = (u32 *)436;
  dst = HeapSlot_Alloc(slot, (s32)dst);

  COPY(4);
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
    COPY(2);
    if ((flags & 8) != 0) {
      PUT(src[0]);
    } else {
      PUT(src[1]);
    }
    src += 2;
    COPY(2);
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
  COPY_LONG(6);

  PUT(*src++);
  if ((flags & 1) == 0) {
    PUT(src[0]);
  }
  src += 1;

  COPY_LONG(5);

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
  COPY(2);
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
  COPY(2);

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
  // An empty once-only loop keeps the step before the size's load; without
  // it, 2 of 597 instructions differ in tla-en.
  do {
  } while (0);

  {
    u32 words = (u32)runSize / 4;
    /* FAKEMATCH(pin): count fixed in r2, where the rounded size is built; as a
     * plain local, 35 of 597 instructions differ in tla-en. */
    register s32 count __asm__("r2") = words * 4;
    s32 size = count;

    /* FAKEMATCH(barrier): an empty asm keeps words alive past count's set; as
     * nothing, 6 of 597 instructions differ in tla-en. */
    __asm__("" : : "r"(words));
    Dma_Set(runCopy, dst, DMA_COPY_WORDS(count / 4), REG_DMA3);
    dst = (u32 *)((u8 *)dst + size);
  }
  PATCH(skip);

  COPY(3);

  PUT_BRANCH(row);
  PATCH(noRows);
  PATCH(noColumns);

  COPY(2);
  {
    const u32 *end = (const u32 *)Data_081973f0;

    return src == end;
  }
}
