/// @file
/// Growth, Punji, Vine and Thorn.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// Growing columns and thorns spring up one after another on a fixed schedule,
// each shaking the screen as it appears, throwing up puffs of dust from its
// root and making the targets react a few frames later. The dust is a pool
// of EffectSteps in PARTICLES, a free one marked by variant -1, its x and y
// in canvas pixels; each puff plays seven pictures, 2 frames each, drawn
// mirrored about its x. Both scenes fade out over their last 16 frames.

/// The dust pictures in the vine sheet, after the vines.
#define GROWING_COLUMNS_VINE_DUST_CELLS 0x1e59
/// The dust pictures in the pillar sheet, after the pillars.
#define GROWING_COLUMNS_PILLAR_DUST_CELLS 0xaff
/// The small thorn in the thorn sheet, after the 48x80 big one.
#define THORN_SMALL_CELLS (48 * 80)

/// What the growing columns grow (BattleAnim_RunGrowingColumns).
enum GrowingColumnsMode {
  GROWING_COLUMNS_VINES,  ///< Growth
  GROWING_COLUMNS_PILLARS ///< Punji's green pillars
};

/// One growing column, laid over a struct Particle in work->particles.
struct ParticleGrowthColumnView {
  s32 x; ///< the root, in canvas pixels
  s32 y; ///< the root's row: 108 to 115, lower for later columns
  s32 z; ///< the Particle's z and velocityX, unused here
  s32 velocityX;
  s32 height;    ///< the full length, 55 to 118 capped by its shape
  s32 velocityZ; ///< the Particle's, unused here
  /// The frame it starts to grow; set, but the frame loop computes its own.
  s32 start;
};

/// Vines grow from the ground, scattering dust from each root.
///
/// Abilities: Growth, Mad Growth, Wild Growth (BATTLE_ANIM_GROWTH).
void BattleAnim_RunGrowth(struct BattleAnimArgs *effect) {
  BattleAnim_RunGrowingColumns(effect, GROWING_COLUMNS_VINES);
}
/// Green pillars grow from the ground, scattering dust from each root.
///
/// Abilities: Punji, Punji Trap, Punji Strike (BATTLE_ANIM_PUNJI).
///
/// Punji Strike's row of gAbilityAnimations gives the stronger variant, as
/// Punji Trap's does, so no ability plays BATTLE_ANIM_PUNJI's strongest
/// variant.
void BattleAnim_RunPunji(struct BattleAnimArgs *effect) {
  BattleAnim_RunGrowingColumns(effect, GROWING_COLUMNS_PILLARS);
}
/// Vines or green pillars grow one every 4 frames,
/// alternately mirrored, each throwing up dust from its root.
///
/// Column i appears on frame 4i + 9, shaking the screen, and grows 8 pixels a
/// frame to a random length capped by its shape (i % 3); a pillar also stops
/// at its gGrowth_PillarHeights. On frame 4i + 12 the targets react (even
/// columns with a sound), and on frames 4i + 12 and 4i + 16 the column throws
/// up a puff of dust. The battle moves on to the results 64 frames before the
/// end; the scene fades out over 16 frames, and nothing grows in its last 4.
///
/// Called by BattleAnim_RunGrowth and BattleAnim_RunPunji.
/// work->particles holds the 16 columns (struct ParticleGrowthColumnView) and
/// PARTICLES the dust. Writes REG_BG2PA, REG_BLDCNT, REG_BLDALPHA and, on side
/// 1, REG_BG2X; sets work->shakeFrames as each column appears.
///
/// effect: the targets; variant picks the number of columns
///         (gGrowth_ColumnCounts): 4 for Growth and Punji, 8 for Mad Growth,
///         Punji Trap and Punji Strike, 12 for Wild Growth. The scene runs 4
///         frames a column and 56 more.
void BattleAnim_RunGrowingColumns(struct BattleAnimArgs *effect, s32 mode) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  void *rectangle[2];
  void *secondSlot;

  s32 frame;
  s32 duration;
  void **draw;
  s32 cueFrame;
  s32 fadeStart;
  s32 fadeEnd;
  s32 start;

  u8 *variantCursor;

  struct ParticleGrowthColumnView *setup;
  s32 x;
  s32 height;
  s32 limit;

  s32 puffCells;
  struct ParticleGrowthColumnView *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;

  s32 n;
  s32 reactFrame;

  s32 i;
  struct Particle *puff;
  s32 puffAge;
  s32 image;

  ANIM_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  if (mode == GROWING_COLUMNS_PILLARS) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  rectangle[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_TRANSPARENT);
  /* FAKEMATCH(temporary): secondSlot and draw, a second pointer to the pair,
   * order the frame's spill slots; as rectangle[1] = gHeapSlots[n] and
   * rectangle[side], 11 of 503 instructions differ in tbs-en. */
  secondSlot = gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gGrowth_ColumnCounts[work->effect->variant] * 4 + 56;

  // Mark the dust free (variant -1): 1024 entries, though only the first 512
  // are used.
  variantCursor = Ram_MapGridCellBuffer + offsetof(struct Particle, variant);
  i = 0;
  do {
    i++;
    *(s32 *)variantCursor = -1;
    variantCursor += sizeof(struct Particle);
  } while (i != 1024);

  // The 16 columns' roots and lengths: each root up to 4 pixels either side
  // of its gGrowth_ColumnX, each length random and capped by its shape. All
  // 16 are placed, whatever the variant's count.
  i = 0;
  setup = (struct ParticleGrowthColumnView *)work->particles;
  do {
    x = (gGrowth_ColumnX[i] + (7 & Random16())) - 4;
    setup->y = i / 2 + 108;
    setup->x = x;
    height = (63 & Random16()) + 55;
    setup->height = height;
    limit = gGrowth_HeightLimits[i % 3];
    if (limit < height) {
      setup->height = limit;
    }
    limit = i * 4 + 8;
    setup->start = limit;
    i++;
    setup++;
  } while (i != 16);

  // Each transfer clears the canvas: everything is redrawn every frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        ANIM_SHOW_DAMAGE(SE_WATER);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade out over 16 frames, the canvas's share falling from 15/16 to
        // nothing, which it stays at for the last 4.
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA((duration - frame) - 5, 16);
      }
      // The columns, the targets' reactions and the dust they throw up.
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[work->effect->variant] != 0) {
          do {
            column = (struct ParticleGrowthColumnView
                          *)((u8 *)work->particles +
                             i * sizeof(struct ParticleGrowthColumnView));
            start = i * 4 + 8;
            if (frame == i * 4 + 9) {
              work->shakeFrames = 2;
            }

            // Grow 8 pixels a frame (odd columns mirrored), standing on the
            // root and centred across it. A pillar also stops at its
            // gGrowth_PillarHeights entry.
            if (frame > start) {
              u32 shape = i % 3;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == GROWING_COLUMNS_VINES) {
                {
                  s32 side = 1 & i;
                  s32 cell = gGrowth_VineCells[shape];
                  shape = gGrowth_VineWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              } else {
                pillarHeight = gGrowth_PillarHeights[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  s32 side = 1 & i;
                  s32 cell = gGrowth_PillarCells[shape];
                  shape = gGrowth_PillarWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              }
            }

            // The targets react 4 frames after the column starts to grow,
            // with a sound for even columns only.
            n = 0;
            if (work->effect->targetCount != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlaySongFar(SE_IMPACT);
                  }
                  BattleAnim_TintBattler(work->effect->targets[n],
                                         SPRITE_COLOR_WHITE, POSE_DAMAGE, n, 3);
                }
                n++;
              } while (n != work->effect->targetCount);
            } else {
              reactFrame = start + 4;
            }

            // On frames 4 and 8, put the first free puff of dust near the
            // root: x within 8 pixels of it, y on rows 80 to 95.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              /* FAKEMATCH(temporary): the scan reuses length as its puff
               * pointer; as a pointer of its own, 25 of 503 instructions
               * differ in tbs-en. */
              length = (u32)PARTICLES;
              n = 0;
              while (n != 512) {
                if (((struct Particle *)length)->variant == -1) {
                  ((struct Particle *)length)->x =
                      ((Random16() & 0xf) + column->x) - 8;
                  ((struct Particle *)length)->y = (Random16() & 0xf) + 80;
                  ((struct Particle *)length)->variant = 0;
                  break;
                }
                length += sizeof(struct Particle);
                n++;
              }
            }
            i++;
          } while (i != gGrowth_ColumnCounts[work->effect->variant]);
        }
      }

      // Dust, after the columns so it is drawn over them; it keeps going
      // through the last 4 frames.
      i = 0;
      puff = PARTICLES;
      do {
        puffAge = puff->variant;
        if (puffAge >= 0) {
          image = puffAge / 2;
          puffCells = GROWING_COLUMNS_VINE_DUST_CELLS;
          if (mode != GROWING_COLUMNS_VINES) {
            puffCells = GROWING_COLUMNS_PILLAR_DUST_CELLS;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (gGrowth_PuffCells[image] + puffCells),
              puff->x - gGrowth_PuffWidths[image],
              puff->y - (s8)gGrowth_PuffHeights[image] / 2,
              gGrowth_PuffWidths[image], (s8)gGrowth_PuffHeights[image]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (gGrowth_PuffCells[image] + puffCells),
              puff->x, puff->y - (s8)gGrowth_PuffHeights[image] / 2,
              gGrowth_PuffWidths[image], (s8)gGrowth_PuffHeights[image]);
          puff->variant++;
          if (puff->variant == 14) {
            puff->variant = -1;
          }
        }
        puff++;
        i++;
      } while (i != 512);

      BattleCamera_Shake(4, 4);
      ANIM_END_FRAME(work);
      frame++;
    } while (frame != duration);
  }
  ANIM_FINISH_CANVAS();
}

#define VINE_COLUMNS 16 ///< columns that rise
#define VINE_FRAMES 70

/// Sixteen green vines grow from the middle of the row of targets, 2 frames
/// apart, and shrink back.
///
/// Abilities: Vine (BATTLE_ANIM_VINE).
///
/// Vine i appears on frame 2i + 5 and grows 4 pixels a frame up to 32,
/// cycling through five 32x32 pictures, then shrinks away from frame 2i + 32;
/// odd vines are mirrored. Vine i shakes the screen on frame 4i + 5. On frame
/// 32 the targets react together, with a sound.
///
/// A Djinni appears first (BattleAnim_RunDjinniEntrance). work->particles
/// holds the 16 vines' x. Writes REG_BG2PA, REG_BLDCNT and REG_BG2X, sets
/// work->shakeFrames.
void BattleAnim_RunVine(struct BattleAnimArgs *effect) {
  u32 *slots, *cursor;
  struct BattleAnimWork *work;
  void *canvas;
  BlitterPair draw;
  s32 originX, originY;
  struct ScreenPoint first, last;
  s32 i, frame, cell, height, offset, middle;
  struct Particle *column;

  slots = (u32 *)(gHeapSlots + HEAP_SLOT_BATTLE_ANIM);
  cursor = slots;
  work = (struct BattleAnimWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleAnim_RunDjinniEntrance(effect, ELEMENT_VENUS + DJINNI_ENTRANCE_LONG,
                               effect->side, DJINNI_ANCHOR_CENTRE, &originX,
                               &originY);
  BattleAnim_StartCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  // Put the point halfway between the first and last targets at canvas x
  // 64.
  Projection_ScaledUnit(work->effect->targets[0], &first);
  Projection_ScaledUnit(work->effect->targets[work->effect->targetCount - 1],
                        &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (64 - first.x) << 8;
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)ANIM_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  column = work->particles;
  i = 0;
  do {
    column[i].x = gGrowth_ColumnOffsets[i] + 64;
    i++;
  } while (i != VINE_COLUMNS);
  // Each transfer clears the canvas: the vines are redrawn every frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  frame = 0;
  do {
    // No ANIM_SHOW_DAMAGE here: the targets only flash, on frame 32.
    if (frame == 32) {
      Sound_PlaySongFar(SE_CRACK);
      ANIM_FOR_EACH_TARGET(i, work->effect)
      BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_WHITE,
                             POSE_DAMAGE, i, 16);
    }
    i = 0;
    column = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      // Grow 4 pixels a frame up to 32, and shrink from frame i * 2 + 32,
      // gone by i * 2 + 40. Each vine stands on row 104 + (i & 7) and steps
      // through the five 32x32 pictures every 4 frames from its own start.
      offset = i * 2;
      if (frame > offset + 4) {
        cell = (frame / 4 + i) % 5;
        if (frame < offset + 32) {
          height = (frame - offset) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - offset) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (cell << 10), column->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      column++;
    } while (i != VINE_COLUMNS);
    BattleCamera_Shake(4, 4);
    ANIM_END_FRAME(work);
    frame++;
  } while (frame != VINE_FRAMES);
  ANIM_FINISH_CANVAS();
}

/// Thorns burst from the ground one every 8 frames, throwing up dust from
/// their roots.
///
/// Abilities: Thorn, Briar, Nettle (BATTLE_ANIM_THORN).
///
/// Thorn i appears on frame 8i + 9, shaking the screen, and throws up a puff
/// of dust on each of its first two frames; on frame 8i + 12 the targets
/// react with a sound. The battle moves on to the results 64 frames before
/// the end, and the scene fades out over its last 16 frames.
///
/// effect: the targets; variant picks the number of thorns
///         (gGrowth_Counts): 3 for Thorn, 5 for Briar, 8 for Nettle; the
///         scene runs 8 frames a thorn and 56 more. gGrowth_Kinds gives each
///         thorn's size and direction.
///
/// PARTICLES holds the dust, 64 puffs. Writes REG_BG2PA, REG_BLDCNT,
/// REG_BLDALPHA and, on side 1, REG_BG2X; sets work->shakeFrames as each
/// thorn appears.
void BattleAnim_RunThorn(struct BattleAnimArgs *effect) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 frameCount;
  u32 *slots;
  u32 *cursor;
  struct BattleAnimWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  slots = (u32 *)(gHeapSlots + HEAP_SLOT_BATTLE_ANIM);
  cursor = slots;
  work = (struct BattleAnimWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                    COMPOSITE_TRANSPARENT);
  draw = ((DrawRectangle *)slots)[HEAP_SLOT_RAM_CODE - HEAP_SLOT_BATTLE_ANIM];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  drawFlip =
      ((DrawRectangle *)slots)[HEAP_SLOT_RAM_CODE_2 - HEAP_SLOT_BATTLE_ANIM];
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PRIORITY_VBLANK);
  frameCount = gGrowth_Counts[work->effect->variant] * 8 + 56;

  // Mark the dust free: 1024 entries, though only the first 64 are used.
  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1;
  }

  for (frame = 0; frame != frameCount; frame++) {
    if (frame == frameCount - 64) {
      ANIM_SHOW_DAMAGE(SE_WATER);
    }
    if (frame >= frameCount - 16) {
      // Fade out over the last 16 frames, the canvas's share falling from
      // 15/16 to nothing.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(frameCount - frame - 1, 16);
    }

    for (i = 0; i != gGrowth_Counts[work->effect->variant]; i++) {
      start = i * 8 + 8;
      age = frame - (i * 8 + 8);
      if (frame > start) {
        s32 height;
        s32 lean;

        // A thorn leans sideways as it grows: a big
        // one 16 pixels up and 6 across a frame to 80 and 30, a small one 8
        // up and 1 across to 64 and 8. Both stand on row 108.
        if (gGrowth_Kinds[i] < THORN_SMALL) {
          height = age * 16;
          lean = age * 6;
          if (height > 80) {
            height = 80;
          }
          if (lean > 30) {
            lean = 30;
          }
          if (gGrowth_Kinds[i] & THORN_MIRRORED) {
            drawFlip(canvas, work->sheet, gGrowth_X[i] - lean, 108 - height, 48,
                     height);
          } else {
            draw(canvas, work->sheet, gGrowth_X[i] + lean, 108 - height, 48,
                 height);
          }
        } else {
          height = age * 8;
          lean = age;
          if (height > 64) {
            height = 64;
          }
          if (age > 8) {
            lean = 8;
          }
          if (gGrowth_Kinds[i] & THORN_MIRRORED) {
            drawFlip(canvas, work->sheet + THORN_SMALL_CELLS,
                     gGrowth_X[i] - lean, 108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + THORN_SMALL_CELLS, gGrowth_X[i] + lean,
                 108 - height, 32, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For 2 frames after the thorn appears, add a puff of dust each frame
        // in the first free slot: 32 to 63 pixels right of the thorn's x, no
        // further than 96, on rows 72 to 103.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct Particle *puff;
          for (j = 0; j != 64; j++) {
            puff = &PARTICLES[j];
            if (puff->variant == -1) {
              puff->x = gGrowth_X[i] + (Random16() & 31) + 32;
              if (puff->x > 96) {
                puff->x = 96;
              }
              puff->y = y;
              puff->variant = 0;
              break;
            }
          }
        }
      }
      ANIM_FOR_EACH_TARGET(j, work->effect) {
        if (frame == start + 4) {
          Sound_PlaySongFar(SE_WATER);
          BattleAnim_TintBattler(work->effect->targets[j], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, j, 3);
        }
      }
    }

    // Dust, drawn over the thorns.
    for (i = 0; i != 64; i++) {
      if (PARTICLES[i].variant >= 0) {
        s32 cell = PARTICLES[i].variant / 2;
        draw(canvas, work->sheet + gGrowth_ThornPuffCells[cell],
             PARTICLES[i].x - gGrowth_ThornPuffWidths[cell],
             PARTICLES[i].y - (s8)gGrowth_ThornPuffHeights[cell] / 2,
             gGrowth_ThornPuffWidths[cell], (s8)gGrowth_ThornPuffHeights[cell]);
        drawFlip(
            canvas, work->sheet + gGrowth_ThornPuffCells[cell], PARTICLES[i].x,
            PARTICLES[i].y - (s8)gGrowth_ThornPuffHeights[cell] / 2,
            gGrowth_ThornPuffWidths[cell], (s8)gGrowth_ThornPuffHeights[cell]);
        if (++PARTICLES[i].variant == 14) {
          PARTICLES[i].variant = -1;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

#undef GROWING_COLUMNS_VINE_DUST_CELLS
#undef GROWING_COLUMNS_PILLAR_DUST_CELLS
#undef THORN_SMALL_CELLS
#undef VINE_COLUMNS
#undef VINE_FRAMES
