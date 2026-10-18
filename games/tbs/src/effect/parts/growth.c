// Part of effect.c, the battle effect object, compiled only through it.
// Growth, Punji, Vine and Thorn.

// Growing columns and thorns spring up one after another on a fixed schedule,
// each shaking the screen as it appears, throwing up puffs of dust from its
// root and making the targets react a few frames later. The dust is a pool
// of EffectSteps in PARTICLES, a free one marked by variant -1; each puff
// plays seven pictures, drawn mirrored about its x. Both scenes fade out over
// their last 16 frames.

// Growing columns: vines or pillars spring from the ground one after
// another, throwing up dust. PARTICLES holds the dust.
// One growing column, laid over a struct EffectStep.
struct DualColumn {
  s32 x; // the root, in canvas pixels
  s32 y;
  s32 z; // the EffectStep's z and velocityX, unused here
  s32 velocityX;
  s32 height;    // the full length
  s32 velocityZ; // the EffectStep's, unused here
  s32 start;     // the frame it starts to grow
};

// Abilities: Growth, Mad Growth, Wild Growth (kind 4).
//
// Grow vines from the ground, scattering dust from each root.
DEFINE_EFFECT_KIND(BattleFx_RunGrowth, BattleFx_RunGrowingColumns, 0)

// Abilities: Punji, Punji Trap, Punji Strike (kind 56).
//
// Grow green pillars from the ground, scattering dust from each root.
DEFINE_EFFECT_KIND(BattleFx_RunPunji, BattleFx_RunGrowingColumns, 1)

// Vines (mode 0) or green pillars (mode 1) grow one every 4 frames,
// alternately mirrored, each throwing up dust from its root.
//
// Column i appears on frame 4i + 9, shaking the screen, and grows 8 pixels a
// frame to a random length capped by its shape (i % 3); a pillar also stops
// at its gGrowth_PillarHeights. On frame 4i + 12 the targets react (even
// columns with a sound), and on frames 4i + 12 and 4i + 16 the column throws
// up a puff of dust. The battle moves on to the results 64 frames before the
// end; the scene fades out over 16 frames, and nothing grows in its last 4.
//
// effect: the targets; variant picks the number of columns
//         (gGrowth_ColumnCounts).
void BattleFx_RunGrowingColumns(struct BattleEffectArgument *effect, s32 mode) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
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

  struct DualColumn *setup;
  s32 x;
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
  s32 puffAge;
  s32 image;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  rectangle[0] = gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  secondSlot = gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  /* FAKEMATCH: the columns draw through a second pointer to
   * the pair, whose address the frame keeps in a stack slot 2026-10-11:
   * rectangle[side] at the columns differs 22 (the address slot of the pair
   * moves from sp+28 to sp+16). */
  draw = rectangle;
  draw[1] = secondSlot;

  duration = gGrowth_ColumnCounts[work->effect->variant] * 4 + 56;

  // Mark all the dust free (variant -1).
  variantCursor = Ram_MapCellBuffer + offsetof(struct EffectStep, variant);
  i = 0;
  do {
    i++;
    *(s32 *)variantCursor = -1;
    variantCursor += sizeof(struct EffectStep);
  } while (i != 1024);

  // The 16 columns' roots and lengths.
  i = 0;
  setup = (struct DualColumn *)work->particles;
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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                        0); // clear the canvas every frame

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        EFFECT_SHOW_DAMAGE(SE_132);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade out over the last 16 frames.
        REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLEND_ALPHA((duration - frame) - 5, 16);
      }
      if (frame < fadeEnd) {
        i = 0;
        if (gGrowth_ColumnCounts[work->effect->variant] != 0) {
          do {
            column = (struct DualColumn *)((u8 *)work->particles +
                                           i * sizeof(struct DualColumn));
            start = i * 4 + 8;
            if (frame == i * 4 + 9) {
              work->shakeFrames = 2;
            }

            // Grow 8 pixels a frame (odd columns mirrored).
            if (frame > start) {
              u32 shape = i % 3;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == 0) {
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

            // The targets react 4 frames after the column starts to grow.
            n = 0;
            if (work->effect->count != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    AudioCommand_PlayFar(SE_133);
                  }
                  ObjectGroup_UpdateMembers(work->effect->actors[n],
                                            SPRITE_COLOR_WHITE, POSE_DAMAGE, n,
                                            3);
                }
                n++;
              } while (n != work->effect->count);
            } else {
              reactFrame = start + 4;
            }

            // On frames 4 and 8, put a free puff of dust at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              /* FAKEMATCH: the scan reuses length as its puff
               * pointer 2026-10-11: a block pointer of its own differs 48 (r5
               * and r6 swap); the puff pointer of the function 271. */
              length = (u32)PARTICLES;
              n = 0;
              while (n != 512) {
                if (((struct EffectStep *)length)->variant == -1) {
                  ((struct EffectStep *)length)->x =
                      ((Random16() & 0xf) + column->x) - 8;
                  ((struct EffectStep *)length)->y = (Random16() & 0xf) + 80;
                  ((struct EffectStep *)length)->variant = 0;
                  break;
                }
                length += sizeof(struct EffectStep);
                n++;
              }
            }
            i++;
          } while (i != gGrowth_ColumnCounts[work->effect->variant]);
        }
      }

      // Dust: seven pictures, 2 frames each, drawn mirrored about the middle.
      i = 0;
      puff = PARTICLES;
      do {
        puffAge = puff->variant;
        if (puffAge >= 0) {
          image = puffAge / 2;
          puffCells = 0x1e59;
          if (mode != 0) {
            puffCells = 0xaff;
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

      Camera_ApplyShake(4, 4);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != duration);
  }
  EFFECT_FINISH_CANVAS();
}

#define VINE_COLUMNS 16 // columns that rise
#define VINE_FRAMES 70

// Abilities: Vine (kind 202).
//
// Rising vines: sixteen green vines grow from the middle of the row of
// targets, 2 frames apart, and shrink back.
//
// Vine i appears on frame 2i + 5 and grows 4 pixels a frame up to 32,
// cycling through five 32x32 pictures, then shrinks away from frame 2i + 32;
// odd vines are mirrored. Vine i shakes the screen on frame 4i + 5. On frame
// 32 the targets react together, with a sound.
//
// effect: the targets.
void BattleFx_RunVine(struct BattleEffectArgument *effect) {
  u32 *slots, *cursor;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  s32 originX, originY;
  struct EffectPosition first, last;
  s32 i, frame, cell, height, offset, middle;
  struct EffectStep *column;

  slots = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_RunDjinniEntrance(effect, 4, effect->side, 4, &originX, &originY);
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1,
                             1);

  // Put the point halfway between the first and last targets at canvas x
  // 64.
  EffectPosition_ProjectScaledBattler(work->effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(
      work->effect->actors[work->effect->count - 1], &last);
  middle = first.x;
  middle += (last.x - middle) / 2;
  first.x = middle;
  REG_BG2X = (64 - first.x) << 8;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  column = work->particles;
  i = 0;
  do {
    column[i].x = gGrowth_ColumnOffsets[i] + 64;
    i++;
  } while (i != VINE_COLUMNS);
  // Clear the canvas every frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  frame = 0;
  do {
    if (frame == 32) {
      AudioCommand_PlayFar(SE_143);
      EFFECT_FOR_EACH_TARGET(i, work->effect)
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 16);
    }
    i = 0;
    column = work->particles;
    do {
      if (frame == i * 4 + 5)
        work->shakeFrames = 2;

      // Grow 4 pixels a frame up to 32, and shrink from frame i * 2 + 32.
      offset = i * 2;
      if (frame > offset + 4) {
        cell = (frame / 4 + i) % 5; // five 32x32 pictures
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
    Camera_ApplyShake(4, 4);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != VINE_FRAMES);
  EFFECT_FINISH_CANVAS();
}

// Abilities: Thorn, Briar, Nettle (kind 5).
//
// Rising thorns: thorns burst from the ground one every 8 frames, throwing
// up dust from their roots.
//
// Thorn i appears on frame 8i + 9, shaking the screen, and throws up a puff
// of dust on each of its first two frames; on frame 8i + 12 the targets
// react with a sound. The battle moves on to the results 64 frames before
// the end, and the scene fades out over its last 16 frames.
//
// effect: the targets; variant picks the number of thorns
//         (gGrowth_Counts). gGrowth_Kinds gives each
//         thorn's size and direction.
void BattleFx_RunThorn(struct BattleEffectArgument *effect) {
  void *canvas;
  s32 frame;
  DrawRectangle drawFlip;
  DrawRectangle draw;
  s32 frameCount;
  u32 *slots;
  u32 *cursor;
  struct BattleEffectWork *work;
  s32 i;
  s32 j;
  s32 age;
  s32 start;

  slots = (u32 *)(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  cursor = slots;
  work = (struct BattleEffectWork *)*cursor++;
  canvas = (void *)*cursor;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = Q_8_8(1); // the canvas at its own width
  REG_BLDCNT = 0;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
  if (work->effect->side == 1) {
    REG_BG2X = -0x7000; // 112 pixels right
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  draw = ((DrawRectangle *)slots)[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_TRANSPARENT);
  work->transferMode = CANVAS_COPY_AND_CLEAR; // clear the canvas every frame
  work->transferValue = 0;
  drawFlip = ((DrawRectangle *)
                  slots)[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_EFFECT];
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  frameCount = gGrowth_Counts[work->effect->variant] * 8 + 56;

  for (i = 0; i != 1024; i++) {
    PARTICLES[i].variant = -1; // free
  }

  for (frame = 0; frame != frameCount; frame++) {
    if (frame == frameCount - 64) {
      EFFECT_SHOW_DAMAGE(SE_132);
    }
    if (frame >= frameCount - 16) {
      // Fade out over the last 16 frames.
      REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
      REG_BLDALPHA = BLEND_ALPHA(frameCount - frame - 1, 16);
    }

    for (i = 0; i != gGrowth_Counts[work->effect->variant]; i++) {
      start = i * 8 + 8;
      age = frame - (i * 8 + 8);
      if (frame > start) {
        s32 height;
        s32 lean;

        // Kinds 0 and 1 are big thorns (48 wide), 2 and 3 small ones (32 wide);
        // odd kinds are mirrored. A thorn leans sideways as it grows.
        if (gGrowth_Kinds[i] < 2) {
          height = age * 16;
          lean = age * 6;
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

        // For 2 frames after the thorn appears, add a puff of dust each frame.
        if (frame < start + 3) {
          s32 y = (Random16() & 31) + 72;
          struct EffectStep *puff;
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
      EFFECT_FOR_EACH_TARGET(j, work->effect) {
        if (frame == start + 4) {
          AudioCommand_PlayFar(SE_132);
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 3);
        }
      }
    }

    // Dust: seven pictures, 2 frames each, drawn mirrored about the middle.
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
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef VINE_COLUMNS
#undef VINE_FRAMES
