/// @file
/// Growth, Punji, Vine and Thorn.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Abilities: Growth, Mad Growth, Wild Growth.
void BattleEffect_RunGrowth(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, 0);
}

/// Abilities: Punji, Punji Trap, Punji Strike.
void BattleEffect_RunPunji(struct BattleEffectParams *args) {
  BattleEffect_RunGrowingColumns(args, 1);
}

/// The dust puffs at the map cell buffer: 1024 cleared, the first 512 used.
#define PUFFS ((struct Particle *)Ram_MapCellBuffer)

#define COLUMN_COUNT 16
#define COLUMN_INTERVAL 4
#define PUFF_COUNT 512
#define PUFF_LIFE 14 ///< seven cels, two frames each
#define CUE_COLUMN 133

/// The dust pictures in the vine sheet, after the vines.
#define GROWING_COLUMNS_VINE_DUST_PICTURES 7769
/// The dust pictures in the pillar sheet, after the pillars.
#define GROWING_COLUMNS_PILLAR_DUST_PICTURES 2815

/// Vines or green pillars grow one after another, alternately mirrored, each
/// throwing up dust from its root.
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  void *rectangle[2];
  void *secondSlot;
  s32 frame;
  s32 duration;
  void **draw;
  s32 fadeStart;
  s32 fadeEnd;
  s32 cueFrame;
  s32 start;
  struct DualColumn *root;
  s32 rootX;
  s32 height;
  s32 limit;
  s32 puffCells;
  struct DualColumn *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;
  s32 n;
  s32 reactFrame;
  s32 i;
  struct Particle *puff;
  s32 puffLife;
  s32 cel;

  // Set up: the canvas layer, the sheet for the mode, and the canvas moved
  // 112 pixels right for the other side.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -0x7000;
  }
  // Odd columns draw mirrored.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  rectangle[0] = Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  secondSlot = Ram_HeapSlots->ramCodePaired;
  // Columns draw through a pointer to the pair, which holds that one address
  // all its life.
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gGrowth_ColumnCounts[args->variant] * COLUMN_INTERVAL + 56;

  // Every puff starts free.
  for (i = 0; i != PARTICLE_POOL_COUNT; i++) {
    PUFFS[i].variant = -1;
  }

  // Each column's root, full length (random, capped by its shape) and the
  // frame it starts to grow.
  i = 0;
  do {
    root = (struct DualColumn *)((u8 *)work->particles +
                                 i * sizeof(struct DualColumn));
    rootX = (gGrowth_ColumnX[i] + (7 & Random16())) - 4;
    root->y = i / 2 + 108;
    root->x = rootX;
    height = (63 & Random16()) + 55;
    root->height = height;
    limit = gGrowth_HeightLimits[i % 3];
    if (limit < height) {
      root->height = limit;
    }
    limit = i * COLUMN_INTERVAL + 8;
    root->start = limit;
    i++;
  } while (i != COLUMN_COUNT);

  work->transferMode =
      CANVAS_COPY_AND_CLEAR; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade the canvas out over the last 16 frames.
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA((duration - frame) - 5, BLEND_WEIGHT_MAX);
      }
      // Column i grows 8 pixels a frame from frame i * 4 + 8 to its full
      // length (a pillar no taller than its height table allows); odd
      // columns are mirrored.
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[args->variant] != 0) {
          do {
            column = (struct DualColumn *)((u8 *)work->particles +
                                           i * sizeof(struct DualColumn));
            start = i * COLUMN_INTERVAL + 8;
            if (frame == i * COLUMN_INTERVAL + 9) {
              work->shakeFrames = 2;
            }

            if (frame > start) {
              u32 shape = i % 3;
              s32 side;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == 0) {
                {
                  u8 *cell;
                  u32 width;
                  // side is set to 1 and then masked with i, which builds the 1
                  side = 1;
                  side &= i;
                  cell = work->sheet + gGrowth_VinePictures[shape];
                  width = gGrowth_VineWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, cell, column->x - (width >> 1),
                      column->y - length, width, length);
                }
              } else {
                pillarHeight = gGrowth_PillarHeights[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  u8 *cell;
                  u32 width;
                  // side is set to 1 and then masked with i, which builds the 1
                  side = 1;
                  side &= i;
                  cell = work->sheet + gGrowth_PillarPictures[shape];
                  width = gGrowth_PillarWidths[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, cell, column->x - (width >> 1),
                      column->y - length, width, length);
                }
              }
            }

            // The targets flinch four frames after the column starts; even
            // columns sound the cue.
            n = 0;
            if (args->targetCount != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlayFar(CUE_COLUMN);
                  }
                  BattleEffect_TintBattler(args->targets[n], SPRITE_COLOR_WHITE,
                                           POSE_DAMAGE, n, 3);
                }
                n++;
              } while (n != args->targetCount);
            } else {
              reactFrame = start + 4;
            }

            // Four and eight frames in, a puff of dust rises at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              struct Particle *p = PUFFS;
              n = 0;
              while (n != PUFF_COUNT) {
                if (p->variant == -1) {
                  p->x = ((Random16() & 0xf) + column->x) - 8;
                  p->y = (Random16() & 0xf) + 80;
                  p->variant = 0;
                  break;
                }
                p++;
                n++;
              }
            }
            i++;
          } while (i != gGrowth_ColumnCounts[args->variant]);
        }
      }

      // The dust: seven cels, two frames each, drawn mirrored about x.
      i = 0;
      puff = PUFFS;
      do {
        puffLife = puff->variant;
        if (puffLife >= 0) {
          cel = puffLife / 2;
          puffCells = GROWING_COLUMNS_VINE_DUST_PICTURES;
          if (mode != 0) {
            puffCells = GROWING_COLUMNS_PILLAR_DUST_PICTURES;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (gGrowth_PuffPictures[cel] + puffCells),
              puff->x - gGrowth_PuffWidths[cel],
              puff->y - (s8)gGrowth_PuffHeights[cel] / 2,
              gGrowth_PuffWidths[cel], (s8)gGrowth_PuffHeights[cel]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (gGrowth_PuffPictures[cel] + puffCells),
              puff->x, puff->y - (s8)gGrowth_PuffHeights[cel] / 2,
              gGrowth_PuffWidths[cel], (s8)gGrowth_PuffHeights[cel]);
          puff->variant++;
          if (puff->variant == PUFF_LIFE) {
            puff->variant = -1;
          }
        }
        puff++;
        i++;
      } while (i != PUFF_COUNT);

      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != duration);
  }
  // Tear down: stop the transfer, free both blitters, close the layer.
  EFFECT_FINISH_CANVAS();
}

#define VINE_COUNT 16
#define VINE_FRAMES 70
#define CUE_FRAME 32 ///< the vines reach the targets
#define CUE_VINES 143
#define CANVAS_MIDDLE 64

/// Sixteen green vines grow from the middle of the row of targets, 2 frames
/// apart, and shrink back.
///
/// Abilities: Vine.
void BattleEffect_RunVine(struct BattleEffectParams *args) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct ScreenPoint screen;
  struct ScreenPoint world;
  struct ScreenPoint first;
  struct ScreenPoint last;
  s32 i;
  s32 frame;
  s32 cell;
  s32 height;
  s32 delay;
  s32 middle;
  struct Particle *vine;

  // Set up: the canvas layer, the vine sheet, and the canvas scrolled so
  // that x = 64 falls midway between the first and last targets.
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleEffect;
  canvas = heap->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_VENUS + DJINNI_ENTRANCE_LONG, args->side,
                      DJINNI_ANCHOR_CENTRE),
      &screen, &world);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  REG_BG2PA = Q_8_8(1);
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  EffectPosition_ProjectScaledBattler(args->targets[0], &first);
  EffectPosition_ProjectScaledBattler(args->targets[args->targetCount - 1],
                                      &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (CANVAS_MIDDLE - first.x) << 8;

  // Odd vines draw mirrored.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)heap->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)heap->ramCodePaired;

  vine = work->particles;
  i = 0;
  do {
    vine[i].x = gGrowth_ColumnOffsets[i] + CANVAS_MIDDLE;
    i++;
  } while (i != VINE_COUNT);

  work->transferMode =
      CANVAS_COPY_AND_CLEAR; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // Each frame: vine i starts at frame i * 2 + 4, grows 4 pixels a frame up
  // to 32, and from frame i * 2 + 32 sinks 4 pixels a frame.
  frame = 0;
  do {
    if (frame == CUE_FRAME) {
      Sound_PlayFar(CUE_VINES);
      for (i = 0; i != args->targetCount; i++)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
    }
    i = 0;
    vine = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      delay = i * 2;
      if (frame > delay + 4) {
        cell = (frame / 4 + i) % 5;
        if (frame < delay + 32) {
          height = (frame - delay) * 4 - 16;
          if (height > 32)
            height = 32;
        } else {
          height = 160 - (frame - delay) * 4;
        }
        if (height > 0)
          draw[i & 1](canvas, work->sheet + (cell << 10), vine->x - 16,
                      (i & 7) - height + 104, 32, height);
      }
      i++;
      vine++;
    } while (i != VINE_COUNT);
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != VINE_FRAMES);

  // Tear down: stop the transfer, free both blitters, close the layer.
  EFFECT_FINISH_CANVAS();
}

#undef PUFF_COUNT

// The dust puffs, 1024 particles at the map cell buffer.

#define THORN_INTERVAL 8
#define PUFF_COUNT 1024
#define PUFF_SCAN 64

#define CUE_THORN 133

#define THORN_BIG_HEIGHT 80
#define THORN_SMALL_PICTURE_WIDTH 32
#define THORN_SMALL_PICTURE_HEIGHT 64

/// ThornSheet: the big thorn, then the small one.
#define THORN_BIG_WIDTH 48
#define THORN_SMALL_PICTURE PICTURE_BYTES(THORN_BIG)

/// Thorns burst from the ground one every 8 frames, throwing up dust from
/// their roots.
///
/// Abilities: Thorn, Briar, Nettle.
void BattleEffect_RunThorn(struct BattleEffectParams *args) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 total;
  u32 *heap;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  // Set up: the canvas layer at full alpha, the thorn sheet, and the canvas
  // moved 112 pixels right for the other side.
  heap = (u32 *)Ram_HeapSlots;
  /* FAKEMATCH(barrier): an empty asm through heap hides that it holds the heap
   * slots' fixed address. */
  __asm__("" : "+r"(heap));
  work = (struct BattleEffectWork *)heap[23];
  canvas = (void *)heap[24];
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDCNT = 0;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -0x7000;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw = ((DrawRectangle *)heap)[26];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  drawFlip = ((DrawRectangle *)heap)[47];
  work->transferMode =
      CANVAS_COPY_AND_CLEAR; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  total = gGrowth_Counts[args->variant] * THORN_INTERVAL + 56;

  for (i = 0; i != PUFF_COUNT; i++) {
    PUFFS[i].variant = -1; // free
  }

  for (frame = 0; frame != total; frame++) {
    if (frame == total - 64) {
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    }
    if (frame >= total - 16) {
      // Fade the canvas out over the last 16 frames.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(total - frame - 1, BLEND_WEIGHT_MAX);
    }

    // Thorn i starts at frame i * 8 + 8. A large one grows 16 pixels a
    // frame to 80 and leans up to 30 pixels; a small one grows 8 pixels a
    // frame to 64 and leans up to 8.
    for (i = 0; i != gGrowth_Counts[args->variant]; i++) {
      start = i * THORN_INTERVAL + THORN_INTERVAL;
      age = frame - start;
      if (frame > start) {
        s32 height;
        s32 lean;

        if (gGrowth_Kinds[i] < THORN_SMALL) {
          height = age * 16;
          lean = (age * 2 + age) * 2;
          if (height > THORN_BIG_HEIGHT) {
            height = THORN_BIG_HEIGHT;
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
          if (height > THORN_SMALL_PICTURE_HEIGHT) {
            height = THORN_SMALL_PICTURE_HEIGHT;
          }
          if (age > 8) {
            lean = 8;
          }
          if (gGrowth_Kinds[i] & THORN_MIRRORED) {
            drawFlip(canvas, work->sheet + THORN_SMALL_PICTURE,
                     gGrowth_X[i] - lean, 108 - height,
                     THORN_SMALL_PICTURE_WIDTH, height);
          } else {
            draw(canvas, work->sheet + 0xf00, gGrowth_X[i] + lean, 108 - height,
                 THORN_SMALL_PICTURE_WIDTH, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For two frames after it breaks through, the thorn throws up one
        // dust puff a frame into the first free slot.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct Particle *puff;

          for (j = 0; j != PUFF_SCAN; j++) {
            puff = &PUFFS[j];
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

      // The targets flinch as the thorn reaches them.
      for (j = 0; j != args->targetCount; j++) {
        if (frame == start + 4) {
          Sound_PlayFar(CUE_THORN);
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 3);
        }
      }
    }

    // The dust: seven cels, two frames each, drawn mirrored about x.
    for (i = 0; i != PUFF_SCAN; i++) {
      if (PUFFS[i].variant >= 0) {
        s32 cel = PUFFS[i].variant / 2;

        draw(canvas, work->sheet + gGrowth_ThornPuffPictures[cel],
             PUFFS[i].x - gGrowth_ThornPuffWidths[cel],
             PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
             gGrowth_ThornPuffWidths[cel], (s8)gGrowth_ThornPuffHeights[cel]);
        drawFlip(canvas, work->sheet + gGrowth_ThornPuffPictures[cel],
                 PUFFS[i].x, PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
                 gGrowth_ThornPuffWidths[cel],
                 (s8)gGrowth_ThornPuffHeights[cel]);
        if (++PUFFS[i].variant == PUFF_LIFE) {
          PUFFS[i].variant = -1;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // Tear down: stop the transfer, free both blitters, close the layer.
  EFFECT_FINISH_CANVAS();
}
#undef COLUMN_COUNT
#undef PUFF_COUNT
