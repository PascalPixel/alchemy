/// @file
/// Growth, Punji, Vine and Thorn.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// Growing columns and thorns spring up one after another on a fixed schedule,
// each shaking the screen as it appears, throwing up puffs of dust from its
// root and making the targets react a few frames later. The dust is a pool
// of EffectSteps in PARTICLES, a free one marked by variant -1, its x and y
// in canvas pixels; each puff plays seven pictures, 2 frames each, drawn
// mirrored about its x. Both effects fade out over their last 16 frames.

/// The dust pictures in the vine sheet, after the vines.
#define GROWING_COLUMNS_VINE_DUST_PICTURES 7769
/// The dust pictures in the pillar sheet, after the pillars.
#define GROWING_COLUMNS_PILLAR_DUST_PICTURES 2815
/// The small thorn in the thorn sheet, after the 48x80 big one.
#define THORN_SMALL_PICTURE (48 * 80)

/// What the growing columns grow (BattleEffect_RunGrowingColumns).
enum GrowingColumnsMode {
  GROWING_COLUMNS_VINES,  ///< Growth
  GROWING_COLUMNS_PILLARS ///< Punji's green pillars
};

/// One growing column, laid over a struct Particle in work->particles.
struct GrowingColumn {
  s32 x; ///< the root, in canvas pixels
  s32 y; ///< the root's row: 108 to 115, lower for later columns
  s32 unused[2];
  s32 height; ///< the full length, 55 to 118 capped by its shape
  s32 unused2;
  /// The frame it starts to grow; set, but the frame loop computes its own.
  s32 start;
};

/// Vines grow from the ground, scattering dust from each root.
///
/// Abilities: Growth, Mad Growth, Wild Growth (BATTLE_EFFECT_GROWTH).
void BattleEffect_RunGrowth(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, GROWING_COLUMNS_VINES);
}
/// Green pillars grow from the ground, scattering dust from each root.
///
/// Abilities: Punji, Punji Trap, Punji Strike (BATTLE_EFFECT_PUNJI).
///
/// Punji Strike's row of gAbilityBattleEffects gives the stronger variant, as
/// Punji Trap's does, so no ability plays BATTLE_EFFECT_PUNJI's strongest
/// variant.
void BattleEffect_RunPunji(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, GROWING_COLUMNS_PILLARS);
}
/// Vines or green pillars grow one every 4 frames,
/// alternately mirrored, each throwing up dust from its root.
///
/// Column i appears on frame 4i + 9, shaking the screen, and grows 8 pixels a
/// frame to a random length capped by its shape (i % 3); a pillar also stops
/// at its gGrowth_PillarHeights. On frame 4i + 12 the targets react (even
/// columns with a sound), and on frames 4i + 12 and 4i + 16 the column throws
/// up a puff of dust. The battle moves on to the results 64 frames before the
/// end; the effect fades out over 16 frames, and nothing grows in its
/// last 4.
///
/// Called by BattleEffect_RunGrowth and BattleEffect_RunPunji.
/// work->particles holds the 16 columns (struct GrowingColumn) and
/// PARTICLES the dust. Writes REG_BG2PA, REG_BLDCNT, REG_BLDALPHA and, on side
/// 1, REG_BG2X; sets work->shakeFrames as each column appears.
///
/// args: the targets; variant picks the number of columns
///         (gGrowth_ColumnCounts): 4 for Growth and Punji, 8 for Mad Growth,
///         Punji Trap and Punji Strike, 12 for Wild Growth. The effect runs
///         4 frames a column and 56 more.
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  u8 *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct GrowingColumn *columns = (struct GrowingColumn *)work->particles;
  DrawRectangle draw[2];
  s32 columnCount;
  s32 duration;
  s32 dustPictures;
  s32 frame;
  s32 i;

  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  if (mode == GROWING_COLUMNS_PILLARS) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
    dustPictures = GROWING_COLUMNS_PILLAR_DUST_PICTURES;
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
    dustPictures = GROWING_COLUMNS_VINE_DUST_PICTURES;
  }
  if (work->args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -(112 << 8); // 112 pixels right
  // The odd columns draw through the mirrored blitter, draw[1].
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  columnCount = gGrowth_ColumnCounts[work->args->variant];
  duration = columnCount * 4 + 56;

  // Mark the dust free: 1024 entries, though only the first 512 are used.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;

  // The 16 columns' roots and lengths: each root up to 4 pixels either side
  // of its gGrowth_ColumnX, each length random and capped by its shape. All
  // 16 are placed, whatever the variant's count.
  for (i = 0; i != 16; i++) {
    struct GrowingColumn *column = &columns[i];

    column->x = gGrowth_ColumnX[i] + (Random16() & 7) - 4;
    column->y = i / 2 + 108;
    column->height = (Random16() & 63) + 55;
    if (column->height > gGrowth_HeightLimits[i % 3])
      column->height = gGrowth_HeightLimits[i % 3];
    column->start = i * 4 + 8;
  }

  // Each transfer clears the canvas: everything is redrawn every frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  for (frame = 0; frame != duration; frame++) {
    if (frame == duration - 64)
      EFFECT_SHOW_DAMAGE(SE_WATER);
    if (frame >= duration - 20 && frame < duration - 4) {
      // Fade out over 16 frames, the canvas's share falling from 15/16 to
      // nothing, which it stays at for the last 4.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(duration - frame - 5, 16);
    }
    // The columns, the targets' reactions and the dust they throw up.
    if (frame < duration - 4) {
      for (i = 0; i != columnCount; i++) {
        struct GrowingColumn *column = &columns[i];
        s32 start = i * 4 + 8;
        s32 n;

        if (frame == start + 1)
          work->shakeFrames = 2;

        // Grow 8 pixels a frame (odd columns mirrored), standing on the
        // root and centred across it. A pillar also stops at its
        // gGrowth_PillarHeights entry.
        if (frame > start) {
          s32 shape = i % 3;
          s32 length = (frame - start) * 8;
          s32 picture;
          s32 width;

          if (length > column->height)
            length = column->height;
          if (mode == GROWING_COLUMNS_VINES) {
            picture = gGrowth_VinePictures[shape];
            width = gGrowth_VineWidths[shape];
          } else {
            if (length > gGrowth_PillarHeights[i & 7])
              length = gGrowth_PillarHeights[i & 7];
            picture = gGrowth_PillarPictures[shape];
            width = gGrowth_PillarWidths[shape];
          }
          draw[i & 1](canvas, work->sheet + picture, column->x - width / 2,
                      column->y - length, width, length);
        }

        // The targets react 4 frames after the column starts to grow, with
        // a sound for even columns only.
        if (frame == start + 4) {
          for (n = 0; n != work->args->targetCount; n++) {
            if (!(i & 1))
              Sound_PlayFar(SE_IMPACT);
            BattleEffect_TintBattler(work->args->targets[n], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, n, 3);
          }
        }

        // On frames 4 and 8, put the first free puff of dust near the root:
        // x within 8 pixels of it, y on rows 80 to 95.
        if (frame == start + 4 || frame == start + 8) {
          for (n = 0; n != 512; n++) {
            struct Particle *dust = &PARTICLES[n];

            if (dust->variant == -1) {
              dust->x = (Random16() & 15) + column->x - 8;
              dust->y = (Random16() & 15) + 80;
              dust->variant = 0;
              break;
            }
          }
        }
      }
    }

    // Dust, after the columns so it is drawn over them; it keeps going
    // through the last 4 frames. Each puff is drawn twice, its left half
    // mirrored.
    for (i = 0; i != 512; i++) {
      struct Particle *puff = &PARTICLES[i];

      if (puff->variant >= 0) {
        s32 image = puff->variant / 2;
        u8 *picture = work->sheet + gGrowth_PuffPictures[image] + dustPictures;
        s32 width = gGrowth_PuffWidths[image];
        s32 height = (s8)gGrowth_PuffHeights[image];

        draw[0](canvas, picture, puff->x - width, puff->y - height / 2, width,
                height);
        draw[1](canvas, picture, puff->x, puff->y - height / 2, width, height);
        puff->variant++;
        if (puff->variant == 14)
          puff->variant = -1;
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS 16 ///< columns that rise
#define VINE_FRAMES 70

/// Sixteen green vines grow from the middle of the row of targets, 2 frames
/// apart, and shrink back.
///
/// Abilities: Vine (BATTLE_EFFECT_VINE).
///
/// Vine i appears on frame 2i + 5 and grows 4 pixels a frame up to 32,
/// cycling through five 32x32 pictures, then shrinks away from frame 2i + 32;
/// odd vines are mirrored. Vine i shakes the screen on frame 4i + 5. On frame
/// 32 the targets react together, with a sound.
///
/// A Djinni appears first (BattleEffect_RunDjinniEntrance). work->particles
/// holds the 16 vines' x. Writes REG_BG2PA, REG_BLDCNT and REG_BG2X, sets
/// work->shakeFrames.
void BattleEffect_RunVine(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  s32 originX, originY;
  struct ScreenPoint first, last;
  s32 i, frame, picture, height, offset, middle;
  struct Particle *column;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_RunDjinniEntrance(args, ELEMENT_VENUS + DJINNI_ENTRANCE_LONG,
                                 args->side, DJINNI_ANCHOR_CENTRE, &originX,
                                 &originY);
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  // Put the point halfway between the first and last targets at canvas x
  // 64.
  Projection_ScaledUnit(work->args->targets[0], &first);
  Projection_ScaledUnit(work->args->targets[work->args->targetCount - 1],
                        &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  column = work->particles;
  for (i = 0; i != VINE_COLUMNS; i++) {
    column[i].x = gGrowth_ColumnOffsets[i] + 64;
  }
  // Each transfer clears the canvas: the vines are redrawn every frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  for (frame = 0; frame != VINE_FRAMES; frame++) {
    // No EFFECT_SHOW_DAMAGE here: the targets only flash, on frame 32.
    if (frame == 32) {
      Sound_PlayFar(SE_CRACK);
      EFFECT_FOR_EACH_TARGET(i, work->args)
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
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
        picture = (frame / 4 + i) % 5;
        if (frame < offset + 32) {
          height = (frame - offset) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - offset) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (picture << 10), column->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      column++;
    } while (i != VINE_COLUMNS);
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS();
}

/// Thorns burst from the ground one every 8 frames, throwing up dust from
/// their roots.
///
/// Abilities: Thorn, Briar, Nettle (BATTLE_EFFECT_THORN).
///
/// Thorn i appears on frame 8i + 9, shaking the screen, and throws up a puff
/// of dust on each of its first two frames; on frame 8i + 12 the targets
/// react with a sound. The battle moves on to the results 64 frames before
/// the end, and the effect fades out over its last 16 frames.
///
/// args: the targets; variant picks the number of thorns
///         (gGrowth_Counts): 3 for Thorn, 5 for Briar, 8 for Nettle; the
///         effect runs 8 frames a thorn and 56 more. gGrowth_Kinds gives
///         each thorn's size and direction.
///
/// PARTICLES holds the dust, 64 puffs. Writes REG_BG2PA, REG_BLDCNT,
/// REG_BLDALPHA and, on EFFECT_SIDE_PARTY, REG_BG2X; sets work->shakeFrames as
/// each thorn appears.
void BattleEffect_RunThorn(struct BattleEffectParams *args) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 frameCount;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(112 << 8); // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  drawFlip = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  frameCount = gGrowth_Counts[work->args->variant] * 8 + 56;

  // Mark the dust free: 1024 entries, though only the first 64 are used.
  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1;
  }

  for (frame = 0; frame != frameCount; frame++) {
    if (frame == frameCount - 64) {
      EFFECT_SHOW_DAMAGE(SE_WATER);
    }
    if (frame >= frameCount - 16) {
      // Fade out over the last 16 frames, the canvas's share falling from
      // 15/16 to nothing.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(frameCount - frame - 1, 16);
    }

    for (i = 0; i != gGrowth_Counts[work->args->variant]; i++) {
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
            drawFlip(canvas, work->sheet + THORN_SMALL_PICTURE,
                     gGrowth_X[i] - lean, 108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + THORN_SMALL_PICTURE, gGrowth_X[i] + lean,
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
      EFFECT_FOR_EACH_TARGET(j, work->args) {
        if (frame == start + 4) {
          Sound_PlayFar(SE_WATER);
          BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 3);
        }
      }
    }

    // Dust, drawn over the thorns.
    for (i = 0; i != 64; i++) {
      if (PARTICLES[i].variant >= 0) {
        s32 picture = PARTICLES[i].variant / 2;
        draw(canvas, work->sheet + gGrowth_ThornPuffPictures[picture],
             PARTICLES[i].x - gGrowth_ThornPuffWidths[picture],
             PARTICLES[i].y - (s8)gGrowth_ThornPuffHeights[picture] / 2,
             gGrowth_ThornPuffWidths[picture],
             (s8)gGrowth_ThornPuffHeights[picture]);
        drawFlip(canvas, work->sheet + gGrowth_ThornPuffPictures[picture],
                 PARTICLES[i].x,
                 PARTICLES[i].y - (s8)gGrowth_ThornPuffHeights[picture] / 2,
                 gGrowth_ThornPuffWidths[picture],
                 (s8)gGrowth_ThornPuffHeights[picture]);
        if (++PARTICLES[i].variant == 14) {
          PARTICLES[i].variant = -1;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef GROWING_COLUMNS_VINE_DUST_PICTURES
#undef GROWING_COLUMNS_PILLAR_DUST_PICTURES
#undef THORN_SMALL_PICTURE
#undef VINE_COLUMNS
#undef VINE_FRAMES
