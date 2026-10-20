/// @file
/// The blitter builder, BattleFx_BuildBlitter.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// Builds canvas blitters from the resident ARM kernels.
// The folder README describes the routine, block by block, under "Blitters".

// The fragments' registers and modes are described at the top of
// battle_effect_composite.s.

#define BRANCH_FIELD(site, target)                                             \
  ((((u32)(target) - (u32)(site) - 8) >> 2) & 0x00FFFFFF)

/// The words of a partial run, one pixel at a time: the mode's one-pixel
/// fragment, read forwards or, mirrored across, backwards. Each walk writes a
/// leading and a trailing run.
#define BLITTER_PARTIAL_RUN(mode, flags)                                       \
  switch (mode) {                                                              \
  default:                                                                     \
  case COMPOSITE_COPY:                                                         \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleFx_CopyReverse, 2);                                      \
    } else {                                                                   \
      COPY_FROM(BattleFx_CopyForward, 2);                                      \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleFx_TransparentReverse, 4);                               \
    } else {                                                                   \
      COPY_FROM(BattleFx_TransparentForward, 4);                               \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_MAX:                                                          \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleFx_MaxReverse, 4);                                       \
    } else {                                                                   \
      COPY_FROM(BattleFx_MaxForward, 4);                                       \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if (((flags) & COMPOSITE_FLIP_X) != 0) {                                   \
      COPY_FROM(BattleFx_AdditiveReverse, 6);                                  \
    } else {                                                                   \
      COPY_FROM(BattleFx_AdditiveForward, 6);                                  \
    }                                                                          \
    break;                                                                     \
  }

/// The template walk itself, written once. BattleFx_BuildBlitter expands it
/// twice under two definitions of PUT, COPY, COPY_FROM, COPY_TWICE,
/// PUT_BRANCH, MARK and PATCH: first to count the words, then to write them.
/// It uses the caller's templateWords, flags, mode, widthShift, heightShift,
/// and the branch sites emptyRowsBranch, emptyColumnsBranch, rowLoop,
/// skipBranch, partialRun and fullCellLoop.
#define BLITTER_WALK()                                                         \
  COPY(3);                                                                     \
  if (mode == COMPOSITE_ADDITIVE) {                                            \
    COPY_FROM(BattleFx_Mask, 3);                                               \
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
  if ((flags & COMPOSITE_CLIP_Y) != 0) {                                       \
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
  PUT(*templateWords++);                                                       \
  MARK(emptyRowsBranch);                                                       \
  PUT(*templateWords++);                                                       \
                                                                               \
  if ((flags & COMPOSITE_CLIP_X) == 0) {                                       \
    PUT(templateWords[0] + (1 << widthShift) - 1);                             \
  }                                                                            \
  templateWords += 1;                                                          \
  if ((flags & COMPOSITE_CLIP_X) != 0) {                                       \
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
    PUT(templateWords[8] + gBattleFx_BitOperands[widthShift]);                 \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      PUT(templateWords[9]);                                                   \
    } else {                                                                   \
      PUT(templateWords[10]);                                                  \
    }                                                                          \
    PUT(templateWords[11] + gBattleFx_BitOperands[widthShift]);                \
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
  if ((flags & COMPOSITE_CLIP_X) == 0) {                                       \
    PUT(templateWords[0]);                                                     \
  }                                                                            \
  templateWords += 1;                                                          \
                                                                               \
  COPY(5);                                                                     \
                                                                               \
  MARK(rowLoop);                                                               \
  PUT(templateWords[0]);                                                       \
  PUT(templateWords[1] + (1 << (heightShift - 3)) - 1);                        \
  PUT(templateWords[2] + ((widthShift - 3) << 7));                             \
  PUT(templateWords[3]);                                                       \
  templateWords += 4;                                                          \
                                                                               \
  if ((flags & COMPOSITE_CLIP_X) == 0) {                                       \
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
  if ((flags & COMPOSITE_CLIP_X) == 0) {                                       \
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
        COPY_FROM(BattleFx_CopyReverse, 2);                                    \
      } else {                                                                 \
        COPY_FROM(BattleFx_CopyForward, 2);                                    \
      }                                                                        \
    }                                                                          \
  } break;                                                                     \
  case COMPOSITE_TRANSPARENT:                                                  \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_FROM(BattleFx_Transparent8Reverse, 25);                             \
    } else {                                                                   \
      COPY_FROM(BattleFx_Transparent8Forward, 25);                             \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_MAX:                                                          \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_TWICE(BattleFx_Max4Reverse, 16);                                    \
    } else {                                                                   \
      COPY_TWICE(BattleFx_Max4Forward, 16);                                    \
    }                                                                          \
    break;                                                                     \
  case COMPOSITE_ADDITIVE:                                                     \
    if ((flags & COMPOSITE_FLIP_X) != 0) {                                     \
      COPY_TWICE(BattleFx_ColorReverse, 14);                                   \
    } else {                                                                   \
      COPY_TWICE(BattleFx_ColorForward, 14);                                   \
    }                                                                          \
    break;                                                                     \
  }                                                                            \
  templateWords += 4;                                                          \
                                                                               \
  PUT(*templateWords++);                                                       \
  if ((flags & COMPOSITE_CLIP_X) == 0) {                                       \
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
  PUT(*templateWords++);                                                       \
  PUT(*templateWords++)

/// Installs a canvas blitter in heap slot `slot`. widthShift and heightShift
/// are the cell's dimensions as powers of two; flags select clipping and
/// mirroring, and mode how source pixels combine with the destination.
/// Returns nonzero when the walk ended exactly at gBattleFx_MoteCells,
/// the expected end of the template.
s32 BattleFx_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                          u32 mode) {
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

  // First walk: count the words selected by clipping, mirroring and blend mode.
  wordCount = 0;
  templateWords = BattleFx_Composite;

  BLITTER_WALK();

#undef PUT
#undef COPY
#undef COPY_FROM
#undef COPY_TWICE
#undef PUT_BRANCH
#undef MARK
#undef PATCH

  /* FAKEMATCH: one-pass blocks keep each writing step together and in list
   * order while allowing scheduling within it; written as plain statements,
   * PUT differs 38 in all six editions. */
#define PUT(word)                                                              \
  do {                                                                         \
    *codeWords++ = (word);                                                     \
  } while (0)
  /* FAKEMATCH: a one-pass block, as PUT's tag explains; written as plain
   * statements, COPY differs 12 in all six editions. */
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
  /* FAKEMATCH: a one-pass block, as PUT's tag explains; written as a plain
   * statement, PATCH differs 2 in all six editions. */
#define PATCH(site)                                                            \
  do {                                                                         \
    *(site) |= BRANCH_FIELD(site, codeWords);                                  \
  } while (0)

  // Second walk: allocate, copy the selected kernels and patch their branches.
  codeWords = (u32 *)Runtime_AllocateHeapBlock(slot, wordCount << 2);
  templateWords = BattleFx_Composite;

  BLITTER_WALK();

  // The template is the last thing in battle_effect_composite.s, which the
  // object list links straight before effect_table.c, whose first table is
  // gBattleFx_MoteCells: a complete walk ends at that table's address.
  codeEnd = (const u32 *)gBattleFx_MoteCells;
  return templateWords == codeEnd;
}

// These macros describe the two walks above; they are not an effect API.
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
