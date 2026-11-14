/// @file
/// The blitter builder, BattleEffect_BuildBlitter.
/// The builder walks gBlitterTemplate, a template of ARM words, and splices in
/// the pixel fragments of the mode asked for, patching the widths and branches
/// as it goes.

#include "battle_effect_internal.h"

/// The displacement field of an ARM branch at `site` to `target`.
#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

/// The words of a partial run, one pixel at a time, forwards or mirrored.
#define BLITTER_PARTIAL_RUN(mode, flags)                                       \
  switch (mode) {                                                              \
  default:                                                                     \
  case COMPOSITE_COPY:                                                         \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_CopyReverse, 2);                                  \
    } else {                                                                   \
      COPY_FROM(BattleEffect_CopyForward, 2);                                  \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_TransparentReverse, 4);                           \
    } else {                                                                   \
      COPY_FROM(BattleEffect_TransparentForward, 4);                           \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_LIGHTEN:                                                      \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_MaxReverse, 4);                                   \
    } else {                                                                   \
      COPY_FROM(BattleEffect_MaxForward, 4);                                   \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleEffect_AdditiveReverse, 6);                              \
    } else {                                                                   \
      COPY_FROM(BattleEffect_AdditiveForward, 6);                              \
    }                                                                          \
    break;                                                                     \
  }

/// The template walk, written once and expanded twice: first to count the
/// words, then to write them.
#define BLITTER_WALK()                                                         \
  /* Block 1, the prologue (the file's opening comment). */                    \
  COPY(3);                                                                     \
  if (mode == COMPOSITE_ADDITIVE) {                                            \
    COPY_FROM(BattleEffect_Mask, 3);                                           \
  }                                                                            \
                                                                               \
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_X) {                          \
    COPY_FROM(templateWords, 3);                                               \
  }                                                                            \
  templateWords += 3;                                                          \
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP_Y) {                          \
    COPY_FROM(templateWords, 4);                                               \
  }                                                                            \
  templateWords += 4;                                                          \
  if ((flags & COMPOSITE_FLIP) == COMPOSITE_FLIP) {                            \
    COPY_FROM(templateWords, 3);                                               \
  }                                                                            \
  templateWords += 3;                                                          \
  if ((flags & COMPOSITE_FLIP) == 0) {                                         \
    PUT(templateWords[0]);                                                     \
  }                                                                            \
  templateWords += 1;                                                          \
                                                                               \
  /* Block 2, the vertical clip: the height enters the last two words. */      \
  if ((flags & BLITTER_CLIP_Y) != 0) {                                         \
    PUT(templateWords[0]);                                                     \
    PUT(templateWords[1]);                                                     \
    if ((flags & COMPOSITE_FLIP_Y) != 0) {                                     \
      PUT(templateWords[2]);                                                   \
    } else {                                                                   \
      PUT(templateWords[3]);                                                   \
    }                                                                          \
    PUT(templateWords[4]);                                                     \
    PUT(templateWords[5]);                                                     \
    PUT(templateWords[6] + (1 << heightShift));                                \
    PUT(templateWords[7] + (1 << heightShift));                                \
  }                                                                            \
  templateWords += 8;                                                          \
                                                                               \
  /* Block 3, the columns: the empty-rows skip, the width mask or the twelve   \
   * clip words, the empty-columns skip, then the rest of the set-up. */       \
  PUT(*templateWords++);                                                       \
  MARK(emptyRowsBranch);                                                       \
  PUT(*templateWords++);                                                       \
                                                                               \
  if ((flags & BLITTER_CLIP_X) == 0) {                                         \
    PUT(templateWords[0] + (1 << widthShift) - 1);                             \
  }                                                                            \
  templateWords += 1;                                                          \
  if ((flags & BLITTER_CLIP_X) != 0) {                                         \
    PUT(templateWords[0]);                                                     \
    PUT(templateWords[1]);                                                     \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      PUT(templateWords[2]);                                                   \
      PUT(templateWords[3]);                                                   \
    } else {                                                                   \
      PUT(templateWords[4]);                                                   \
      PUT(templateWords[5]);                                                   \
    }                                                                          \
    PUT(templateWords[6]);                                                     \
    PUT(templateWords[7]);                                                     \
    PUT(templateWords[8] + gBattleEffect_BitOperands[widthShift]);             \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      PUT(templateWords[9]);                                                   \
    } else {                                                                   \
      PUT(templateWords[10]);                                                  \
    }                                                                          \
    PUT(templateWords[11] + gBattleEffect_BitOperands[widthShift]);            \
  }                                                                            \
  templateWords += 12;                                                         \
                                                                               \
  PUT(*templateWords++);                                                       \
  MARK(emptyColumnsBranch);                                                    \
  PUT(*templateWords++);                                                       \
                                                                               \
  COPY(6);                                                                     \
                                                                               \
  PUT(*templateWords++);                                                       \
  if ((flags & BLITTER_CLIP_X) == 0) {                                         \
    PUT(templateWords[0]);                                                     \
  }                                                                            \
  templateWords += 1;                                                          \
                                                                               \
  COPY(5);                                                                     \
                                                                               \
  /* Block 4, the row loop: its head, the leading partial run, the full-cell   \
   * loop, the trailing partial run and the step back to the head. */          \
  MARK(rowLoop);                                                               \
  PUT(templateWords[0]);                                                       \
  PUT(templateWords[1] + (1 << (heightShift - 3)) - 1);                        \
  PUT(templateWords[2] + ((widthShift - 3) << 7));                             \
  PUT(templateWords[3]);                                                       \
  templateWords += 4;                                                          \
                                                                               \
  if ((flags & BLITTER_CLIP_X) == 0) {                                         \
    PUT(templateWords[0]);                                                     \
  }                                                                            \
  templateWords += 1;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  MARK(skipBranch);                                                            \
  PUT(*templateWords++);                                                       \
  if ((flags & COMPOSITE_FLIP_X) != 0) {                                       \
    PUT(templateWords[0]);                                                     \
  } else {                                                                     \
    PUT(templateWords[1]);                                                     \
  }                                                                            \
  templateWords += 2;                                                          \
                                                                               \
  MARK(partialRun);                                                            \
  BLITTER_PARTIAL_RUN(mode, flags);                                            \
  templateWords += 4;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  PUT_BRANCH(partialRun);                                                      \
  PUT(*templateWords++);                                                       \
  if ((flags & BLITTER_CLIP_X) == 0) {                                         \
    PUT(templateWords[0]);                                                     \
    PUT(templateWords[1]);                                                     \
  }                                                                            \
  templateWords += 2;                                                          \
  PATCH(skipBranch);                                                           \
                                                                               \
  PUT(*templateWords++);                                                       \
  MARK(skipBranch);                                                            \
  PUT(*templateWords++);                                                       \
                                                                               \
  MARK(fullCellLoop);                                                          \
  /* The words of the full-cell loop body, eight pixels per pass: the copy     \
   * fragment eight times, or the mode's 8-pixel fragment (the 4-pixel maximum \
   * and colour fragments twice). */                                           \
  switch (mode) {                                                              \
  default:                                                                     \
  case COMPOSITE_COPY: {                                                       \
    s32 i;                                                                     \
                                                                               \
    for (i = 0; i < 8; i++) {                                                  \
      if ((flags & COMPOSITE_FLIP_X) != 0) {                                   \
        COPY_FROM(BattleEffect_CopyReverse, 2);                                \
      } else {                                                                 \
        COPY_FROM(BattleEffect_CopyForward, 2);                                \
      }                                                                        \
    }                                                                          \
  } break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_FROM(BattleEffect_Transparent8Reverse, 25);                         \
    } else {                                                                   \
      COPY_FROM(BattleEffect_Transparent8Forward, 25);                         \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_LIGHTEN:                                                      \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_TWICE(BattleEffect_Max4Reverse, 16);                                \
    } else {                                                                   \
      COPY_TWICE(BattleEffect_Max4Forward, 16);                                \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_TWICE(BattleEffect_ColorReverse, 14);                               \
    } else {                                                                   \
      COPY_TWICE(BattleEffect_ColorForward, 14);                               \
    }                                                                          \
    break;                                                                     \
  }                                                                            \
  templateWords += 4;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  if ((flags & BLITTER_CLIP_X) == 0) {                                         \
    PUT(templateWords[0]);                                                     \
    PUT(templateWords[1]);                                                     \
  }                                                                            \
  templateWords += 2;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  PUT_BRANCH(fullCellLoop);                                                    \
  PATCH(skipBranch);                                                           \
                                                                               \
  PUT(*templateWords++);                                                       \
  MARK(skipBranch);                                                            \
  PUT(*templateWords++);                                                       \
  if ((flags & COMPOSITE_FLIP_X) != 0) {                                       \
    PUT(templateWords[0]);                                                     \
  } else {                                                                     \
    PUT(templateWords[1]);                                                     \
  }                                                                            \
  templateWords += 2;                                                          \
                                                                               \
  MARK(partialRun);                                                            \
  BLITTER_PARTIAL_RUN(mode, flags);                                            \
  templateWords += 4;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  PUT_BRANCH(partialRun);                                                      \
  PATCH(skipBranch);                                                           \
                                                                               \
  COPY(3);                                                                     \
                                                                               \
  PUT_BRANCH(rowLoop);                                                         \
  PATCH(emptyRowsBranch);                                                      \
  PATCH(emptyColumnsBranch);                                                   \
                                                                               \
  /* Block 5, the exit, where both skips of block 3 land. */                   \
  PUT(*templateWords++);                                                       \
  PUT(*templateWords++)

/// Writes a drawing routine for the canvas into heap slot `slot`, built from
/// the template gBlitterTemplate for one clip, mirroring and blend mode.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode) {
  const u32 *templateWords;
  u32 *codeWords;
  u32 *emptyRowsBranch;
  u32 *emptyColumnsBranch;
  u32 *rowLoop;
  u32 *skipBranch;
  u32 *partialRun;
  u32 *fullCellLoop;
  const u32 *codeEnd;
  s32 wordCount;

  // XXX (no evidence): why blitters are generated, perhaps because one
  // routine testing every option for every pixel would be too slow and IWRAM
  // too small to keep one blitter for each combination; the ROM holds the
  // builder, not its author's reasons.
#define PUT(word) wordCount++
#define COPY(n)                                                                \
  wordCount += (n);                                                            \
  templateWords += (n)
#define COPY_FROM(from, n) wordCount += (n)
#define COPY_TWICE(from, n)                                                    \
  wordCount += (n);                                                            \
  wordCount += (n)
#define PUT_BRANCH(target) wordCount++
#define MARK(site)
#define PATCH(site)

  wordCount = 0;
  templateWords = gBlitterTemplate;

  BLITTER_WALK();

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

#define PUT(word)                                                              \
  do {                                                                         \
    *codeWords++ = (word);                                                     \
  } while (0)
#define COPY(n)                                                                \
  do {                                                                         \
    Dma_Set(templateWords, codeWords, DMA_COPY_WORDS(n), REG_DMA3);            \
    codeWords += (n);                                                          \
    templateWords += (n);                                                      \
  } while (0)
#define COPY_FROM(from, n)                                                     \
  Dma_Set(from, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                       \
  codeWords += (n)
#define COPY_TWICE(from, n)                                                    \
  {                                                                            \
    const u32 *part = (from);                                                  \
    Dma_Set(part, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                     \
    codeWords += (n);                                                          \
    Dma_Set(part, codeWords, DMA_COPY_WORDS(n), REG_DMA3);                     \
    codeWords += (n);                                                          \
  }
#define PUT_BRANCH(target)                                                     \
  *codeWords = *templateWords++ + BRANCH_FIELD(codeWords, target);             \
  codeWords++
#define MARK(site) site = codeWords
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, codeWords);                                  \
  } while (0)

  codeWords = (u32 *)HeapSlot_Alloc(slot, wordCount << 2);
  templateWords = gBlitterTemplate;

  BLITTER_WALK();

  // The template ends where battle_effect_table.c's first table begins, so a
  // complete walk ends at that table.
  codeEnd = (const u32 *)gBattleEffect_MotePictures;
  return templateWords == codeEnd;
}

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH
#undef BLITTER_PARTIAL_RUN
#undef BLITTER_WALK
#undef BRANCH_FIELD
