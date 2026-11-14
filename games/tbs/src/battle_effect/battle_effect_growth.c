/// @file
/// Growth, Punji, Vine and Thorn.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// Growing columns and thorns spring up one after another, each shaking the
// screen and throwing up dust from its root, and the targets react a few frames
// later.

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
/// Abilities: Growth, Mad Growth, Wild Growth.
void BattleEffect_RunGrowth(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, GROWING_COLUMNS_VINES);
}
/// Green pillars grow from the ground, scattering dust from each root.
///
/// Abilities: Punji, Punji Trap, Punji Strike.
void BattleEffect_RunPunji(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, GROWING_COLUMNS_PILLARS);
}
/// Vines or green pillars grow one after another, alternately mirrored, each
/// throwing up dust from its root.
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *args, s32 mode) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *rectangle[2];

  s32 frame;
  s32 duration;
  void **draw;
  s32 cueFrame;
  s32 fadeStart;
  s32 fadeEnd;
  s32 start;

  u8 *variantCursor;

  struct GrowingColumn *setup;
  s32 x;
  s32 height;
  s32 limit;

  s32 puffPictures;
  struct GrowingColumn *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;

  s32 n;
  s32 reactFrame;

  s32 i;
  struct Particle *puff;
  s32 puffAge;
  s32 image;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = *cursor++;
  canvas = *cursor;
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  if (mode == GROWING_COLUMNS_PILLARS) {
    Resource_LoadAndDecompress(RESOURCE_GREEN_PILLAR_SHEET, work->sheet, 1, 1);
  } else {
    Resource_LoadAndDecompress(RESOURCE_VINE_SHEET, work->sheet, 1, 1);
  }
  if (work->args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(112 << 8); // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  rectangle[0] = gHeapSlots[HEAP_SLOT_RAM_CODE];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  {
    void *mirroredBlitter = gHeapSlots[HEAP_SLOT_RAM_CODE_2];

    draw = rectangle;
    draw[1] = mirroredBlitter;
  }

  duration = gGrowth_ColumnCounts[work->args->variant] * 4 + 56;

  variantCursor = Ram_MapGridCellBuffer + offsetof(struct Particle, variant);
  i = 0;
  do {
    i++;
    *(s32 *)variantCursor = -1;
    variantCursor += sizeof(struct Particle);
  } while (i != 1024);

  i = 0;
  setup = (struct GrowingColumn *)work->particles;
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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        EFFECT_SHOW_DAMAGE(SE_WATER);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade out over the last 16 frames.
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA((duration - frame) - 5, 16);
      }
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[work->args->variant] != 0) {
          do {
            column = (struct GrowingColumn *)((u8 *)work->particles +
                                              i * sizeof(struct GrowingColumn));
            start = i * 4 + 8;
            if (frame == i * 4 + 9) {
              work->shakeFrames = 2;
            }

            // A column grows from its root.
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
                  s32 picture = gGrowth_VinePictures[shape];
                  shape = gGrowth_VineWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + picture, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              } else {
                pillarHeight = gGrowth_PillarHeights[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  s32 side = 1 & i;
                  s32 picture = gGrowth_PillarPictures[shape];
                  shape = gGrowth_PillarWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + picture, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              }
            }

            // The targets react as it grows.
            n = 0;
            if (work->args->targetCount != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlayFar(SE_IMPACT);
                  }
                  BattleEffect_TintBattler(work->args->targets[n],
                                           SPRITE_COLOR_WHITE, POSE_DAMAGE, n,
                                           3);
                }
                n++;
              } while (n != work->args->targetCount);
            } else {
              reactFrame = start + 4;
            }

            // Dust puffs from the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              for (n = 0; n != 512; n++) {
                struct Particle *dust = &PARTICLES[n];

                if (dust->variant == -1) {
                  dust->x = ((Random16() & 0xf) + column->x) - 8;
                  dust->y = (Random16() & 0xf) + 80;
                  dust->variant = 0;
                  break;
                }
              }
            }
            i++;
          } while (i != gGrowth_ColumnCounts[work->args->variant]);
        }
      }

      // Dust, over the columns.
      i = 0;
      puff = PARTICLES;
      do {
        puffAge = puff->variant;
        if (puffAge >= 0) {
          image = puffAge / 2;
          puffPictures = GROWING_COLUMNS_VINE_DUST_PICTURES;
          if (mode != GROWING_COLUMNS_VINES) {
            puffPictures = GROWING_COLUMNS_PILLAR_DUST_PICTURES;
          }
          ((DrawRectangle)rectangle[0])(
              canvas,
              work->sheet + (gGrowth_PuffPictures[image] + puffPictures),
              puff->x - gGrowth_PuffWidths[image],
              puff->y - (s8)gGrowth_PuffHeights[image] / 2,
              gGrowth_PuffWidths[image], (s8)gGrowth_PuffHeights[image]);
          ((DrawRectangle)rectangle[1])(
              canvas,
              work->sheet + (gGrowth_PuffPictures[image] + puffPictures),
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
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != duration);
  }
  EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS 16 ///< columns that rise
#define VINE_FRAMES 70

/// Sixteen green vines grow from the middle of the row of targets, 2 frames
/// apart, and shrink back.
///
/// Abilities: Vine.
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
  Resource_LoadAndDecompress(RESOURCE_GREEN_VINE_SHEET, work->sheet, 1, 1);

  // Centre the vines between the first and last targets.
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  for (frame = 0; frame != VINE_FRAMES; frame++) {
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

      // Each vine grows, then shrinks back.
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
/// Abilities: Thorn, Briar, Nettle.
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
  Resource_LoadAndDecompress(RESOURCE_THORN_SHEET, work->sheet, 1, 1);
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

  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1;
  }

  for (frame = 0; frame != frameCount; frame++) {
    if (frame == frameCount - 64) {
      EFFECT_SHOW_DAMAGE(SE_WATER);
    }
    if (frame >= frameCount - 16) {
      // Fade out over the last 16 frames.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(frameCount - frame - 1, 16);
    }

    for (i = 0; i != gGrowth_Counts[work->args->variant]; i++) {
      start = i * 8 + 8;
      age = frame - (i * 8 + 8);
      if (frame > start) {
        s32 height;
        s32 lean;

        // A thorn leans sideways as it grows.
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

        // Dust puffs beside the thorn.
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

    // Dust, over the thorns.
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
