/// @file
/// The blitter builder, BattleFx_BuildBlitter.
/// Part of effect.c, the battle effect object, compiled only through it. The
/// folder README describes the routine it writes, block by block, under
/// "Blitters"; the fragments' registers and modes are described at the top of
/// battle_effect_composite.s.

#include "../effect_internal.h"

/// The 24-bit displacement field of an ARM branch at `site` to `target`: in
/// words, counted from the branch plus 8 bytes, as the processor reads two
/// words ahead.
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
  /* Block 1, the prologue (README, "Blitters"). */                            \
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
  /* Block 2, the vertical clip: the height enters the last two words. */      \
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
  /* Block 3, the columns: the empty-rows skip, the width mask or the twelve   \
   * clip words, the empty-columns skip, then the rest of the set-up. */       \
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
  /* Block 4, the row loop: its head, the leading partial run, the full-cell   \
   * loop, the trailing partial run and the step back to the head. */          \
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
  /* Block 5, the exit, where both skips of block 3 land. */                   \
  PUT(*templateWords++);                                                       \
  PUT(*templateWords++)

/// Writes a drawing routine for the canvas into heap slot `slot`, built from
/// the template BattleFx_Composite for one clip, mirroring and blend mode.
///
/// Called through EFFECT_LOAD_BLITTER by nearly every canvas effect and by
/// BattleFx_FetchRectangleBlitters, directly by the particle streams for a
/// canvas 256 pixels wide, and from other banks through its far stub,
/// BattleEffect_LoadWorkFar (the star field in battle/star_field.c and
/// object/colorfx.c).
///
/// It allocates the slot's block (from IWRAM when it fits) at exactly the
/// routine's size. A slot that already holds a block keeps it, whatever its
/// size, and the routine is written over it: callers release the slot first
/// when the new routine may be longer. The caller calls the slot's block as a
/// DrawRectangle.
///
/// widthShift, heightShift: the canvas's width and height as powers of two.
/// flags: the COMPOSITE_CLIP_ and COMPOSITE_FLIP_ bits.
/// mode: how source pixels combine with the canvas (COMPOSITE_COPY,
/// COMPOSITE_TRANSPARENT, COMPOSITE_MAX or COMPOSITE_ADDITIVE).
///
/// Returns nonzero when the walk ended exactly at gBattleFx_MoteCells, the end
/// of the template; no caller reads it.
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

  // First walk: count the words selected by clipping, mirroring and blend
  // mode. Branch sites are only marked in the second walk, so this one needs
  // no buffer.
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

  // Each macro has the shape all its uses match with: PUT, COPY and PATCH as
  // do-while statements, the others as plain statements or a block.
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

  // Second walk: allocate, copy the selected fragments with DMA 3 and patch
  // their branches. A forward branch is written with its template word, its
  // site marked, and its displacement ORed in when the walk reaches its
  // target; a backward branch is written whole at once.
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
