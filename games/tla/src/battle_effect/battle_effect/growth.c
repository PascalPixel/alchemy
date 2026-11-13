/// @file
/// Growth, Punji, Vine and Thorn.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Abilities: Growth, Mad Growth, Wild Growth (kind 4).
void BattleEffect_RunGrowth(void *object) {
  BattleEffect_RunGrowingColumns(object, 0);
}

/// Abilities: Punji, Punji Trap, Punji Strike (kind 56).
void BattleEffect_RunPunji(void *object) {
  BattleEffect_RunGrowingColumns(object, 1);
}

// The dust puffs at the map cell buffer: 1024 cleared, the first 512 used.
#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer)

#define COLUMN_COUNT 16
#define COLUMN_INTERVAL 4
#define PUFF_COUNT 512
#define PUFF_LIFE 14      // seven cels, two frames each
#define PHASE_RESULTS 133 // BattlePlayback_StartFar: the results phase begins
#define CUE_COLUMN 133

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_TRANSPARENT (2 << 4)

void BattleEffect_RunGrowingColumns(struct BattleEffectArgument *effect,
                                    s32 mode) {
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
  struct EffectStep *puff;
  s32 puffLife;
  s32 cel;

  // Set up: the canvas layer, the sheet for the mode, and the canvas moved
  // 112 pixels right for the other side.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  // Odd columns draw mirrored.
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  rectangle[0] = Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  secondSlot = Ram_HeapSlots->unkb8[1];
  // Columns draw through a pointer to the pair, as the twin does; it holds
  // that one address all its life.
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gGrowth_ColumnCounts[effect->variant] * COLUMN_INTERVAL + 56;

  // Every puff starts free.
  for (i = 0; i != 1024; i++) {
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

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        BattlePlayback_StartFar(PHASE_RESULTS);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade the canvas out over the last 16 frames.
        REG_BLDCNT = 0x3f44;
        REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
      }
      // Column i grows 8 pixels a frame from frame i * 4 + 8 to its full
      // length (a pillar no taller than its height table allows); odd
      // columns are mirrored.
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[effect->variant] != 0) {
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
                  cell = work->sheet + gGrowth_VineCells[shape];
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
                  cell = work->sheet + gGrowth_PillarCells[shape];
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
            if (effect->count != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlayFar(CUE_COLUMN);
                  }
                  BattleEffect_TintBattler(effect->actors[n], 7, 5, n, 3);
                }
                n++;
              } while (n != effect->count);
            } else {
              reactFrame = start + 4;
            }

            // Four and eight frames in, a puff of dust rises at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              struct EffectStep *p = PUFFS;
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
          } while (i != gGrowth_ColumnCounts[effect->variant]);
        }
      }

      // The dust: seven cels, two frames each, drawn mirrored about x.
      i = 0;
      puff = PUFFS;
      do {
        puffLife = puff->variant;
        if (puffLife >= 0) {
          cel = puffLife / 2;
          puffCells = 0x1e59;
          if (mode != 0) {
            puffCells = 0xaff;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (gGrowth_PuffCells[cel] + puffCells),
              puff->x - gGrowth_PuffWidths[cel],
              puff->y - (s8)gGrowth_PuffHeights[cel] / 2,
              gGrowth_PuffWidths[cel], (s8)gGrowth_PuffHeights[cel]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (gGrowth_PuffCells[cel] + puffCells),
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
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != duration);
  }
  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#define VINE_COUNT 16
#define VINE_FRAMES 70
#define CUE_FRAME 32 // the vines reach the targets
#define CUE_VINES 143
#define CANVAS_MIDDLE 64

// BattleEffect_RunDjinniEntrance's flags: kind | side << 4 | anchor << 5.
#define PREPARE_KIND 4
#define PREPARE_ANCHOR 4

void BattleEffect_RunVine(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  struct EffectPosition screen;
  struct EffectPosition world;
  struct EffectPosition first;
  struct EffectPosition last;
  s32 i;
  s32 frame;
  s32 cell;
  s32 height;
  s32 delay;
  s32 middle;
  struct EffectStep *vine;

  // Set up: the canvas layer, the vine sheet, and the canvas scrolled so
  // that x = 64 falls midway between the first and last targets.
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      effect, PREPARE_KIND | effect->side << 4 | PREPARE_ANCHOR << 5, &screen,
      &world);
  REG_BG2CNT = 0x785;
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(effect->actors[effect->count - 1], &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (CANVAS_MIDDLE - first.x) << 8;

  // Odd vines draw mirrored.
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)heap->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)heap->unkb8[1];

  vine = work->particles;
  i = 0;
  do {
    vine[i].x = gGrowth_ColumnOffsets[i] + CANVAS_MIDDLE;
    i++;
  } while (i != VINE_COUNT);

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each frame: vine i starts at frame i * 2 + 4, grows 4 pixels a frame up
  // to 32, and from frame i * 2 + 32 sinks 4 pixels a frame.
  frame = 0;
  do {
    if (frame == CUE_FRAME) {
      Sound_PlayFar(CUE_VINES);
      for (i = 0; i != effect->count; i++)
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
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
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != VINE_FRAMES);

  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#undef PUFF_COUNT

// The dust puffs, 1024 particles at the map cell buffer.

#define THORN_INTERVAL 8
#define PUFF_COUNT 1024
#define PUFF_SCAN 64

#define CUE_THORN 133

void BattleEffect_RunThorn(struct BattleEffectArgument *effect) {
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
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y);
  draw = ((DrawRectangle *)heap)[26];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X |
                                    COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
  drawFlip = ((DrawRectangle *)heap)[47];
  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  total = gGrowth_Counts[effect->variant] * THORN_INTERVAL + 56;

  for (i = 0; i != PUFF_COUNT; i++) {
    PUFFS[i].variant = -1; // free
  }

  for (frame = 0; frame != total; frame++) {
    if (frame == total - 64) {
      BattlePlayback_StartFar(PHASE_RESULTS);
    }
    if (frame >= total - 16) {
      // Fade the canvas out over the last 16 frames.
      REG_BLDCNT = 0x3f44;
      REG_BLDALPHA = (total - frame - 1) | 0x1000;
    }

    // Thorn i starts at frame i * 8 + 8. A large one grows 16 pixels a
    // frame to 80 and leans up to 30 pixels; a small one grows 8 pixels a
    // frame to 64 and leans up to 8.
    for (i = 0; i != gGrowth_Counts[effect->variant]; i++) {
      start = i * THORN_INTERVAL + THORN_INTERVAL;
      age = frame - start;
      if (frame > start) {
        s32 height;
        s32 lean;

        if (gGrowth_Kinds[i] < 2) {
          height = age * 16;
          lean = (age * 2 + age) * 2;
          if (height > 80) {
            height = 80;
          }
          if (lean > 30) {
            lean = 30;
          }
          if (gGrowth_Kinds[i] & 1) {
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
          if (gGrowth_Kinds[i] & 1) {
            drawFlip(canvas, work->sheet + 0xf00, gGrowth_X[i] - lean,
                     108 - height, 32, height);
          } else {
            draw(canvas, work->sheet + 0xf00, gGrowth_X[i] + lean, 108 - height,
                 32, height);
          }
        }
        if (frame == start + 1) {
          work->shakeFrames = 3;
        }

        // For two frames after it breaks through, the thorn throws up one
        // dust puff a frame into the first free slot.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct EffectStep *puff;

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
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 4) {
          Sound_PlayFar(CUE_THORN);
          BattleEffect_TintBattler(effect->actors[j], 7, 5, j, 3);
        }
      }
    }

    // The dust: seven cels, two frames each, drawn mirrored about x.
    for (i = 0; i != PUFF_SCAN; i++) {
      if (PUFFS[i].variant >= 0) {
        s32 cel = PUFFS[i].variant / 2;

        draw(canvas, work->sheet + gGrowth_ThornPuffCells[cel],
             PUFFS[i].x - gGrowth_ThornPuffWidths[cel],
             PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
             gGrowth_ThornPuffWidths[cel], (s8)gGrowth_ThornPuffHeights[cel]);
        drawFlip(canvas, work->sheet + gGrowth_ThornPuffCells[cel], PUFFS[i].x,
                 PUFFS[i].y - (s8)gGrowth_ThornPuffHeights[cel] / 2,
                 gGrowth_ThornPuffWidths[cel],
                 (s8)gGrowth_ThornPuffHeights[cel]);
        if (++PUFFS[i].variant == PUFF_LIFE) {
          PUFFS[i].variant = -1;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
