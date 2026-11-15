/// @file
/// The body of Thunder Mine and Earth Force, then Megaera and Void Beam.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// The sparks, after the map cells.
#define BOLT_SPARKS ((struct Particle *)gData_02014000)

/// Where TornadoSheet is decoded in the effect's pictures.
#define BOLT_TRAIL_TORNADO_PICTURES 0x800

#define BOLT_TRAIL_FRAMES 50

/// The body of Thunder Mine (mode 0) and Earth Force (mode 1): a ball of
/// light (Thunder Mine) or a falling rock's glow (Earth Force) crosses to the
/// target, trailed by 40 spinning bolts, and grows; when it arrives the
/// target is hit, and in Thunder Mine 128 sparks scatter and bounce.
void BattleEffect_RunBoltTrail(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sprites;
  s32 radius;
  s32 frames;
  s32 x;
  s32 y;
  struct ScreenPoint target;
  struct ScreenPoint source;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *ball;
  struct Particle *bolt;
  struct Particle *spark;
  struct Particle *dot;
  void *vertices;
  s32 i;
  s32 k;
  u8 *buffer;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(1);
  if (mode == BOLT_TRAIL_THUNDER_MINE) {
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  } else {
    REG_BLDALPHA = BLEND_ALPHA(16, 8);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  radius = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                             work->sheet + BOLT_TRAIL_TORNADO_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, buffer, 1, 1);
  if (mode == BOLT_TRAIL_EARTH_FORCE) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_VioletPaletteC),
                    SHEET_PALETTE_BYTES);
  }

  // The bolts' four pictures, 24 by 120, each in a 32-wide cell.
  Iwram_ClearWordsEntry(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    for (k = 0; k != 120; k++) {
      s32 j;

      for (j = 0; j != 24; j++) {
        work->sheet[0x145a + i * 0x1000 + k * 32 + j] =
            buffer[(i * 45 << 6) + k * 24 + j];
      }
    }
  }
  if (mode == BOLT_TRAIL_THUNDER_MINE) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (mode == BOLT_TRAIL_THUNDER_MINE) {
    frames = 24;
    /* FAKEMATCH(barrier): an empty asm after Thunder Mine's 24. */
    asm("");
  } else {
    frames = 40;
  }

  for (frame = 0; frame != BOLT_TRAIL_FRAMES; frame++) {
    ball = work->particles;
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->user, &source);
      EffectPosition_ProjectBattler(args->targets[0], &target);
      if (mode == BOLT_TRAIL_THUNDER_MINE) {
        ball->x = px(source.x);
        ball->y = px(source.y - 32);
        ball->velocityX = (px(target.x) - ball->x) / frames;
      } else {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          ball->x = px(source.x - 32);
        } else {
          ball->x = px(source.x + 32);
        }
        ball->y = px(source.y - 80);
        ball->velocityX = (px(target.x - 32) - ball->x) / frames;
      }
      ball->velocityY = (px(target.y - 32) - ball->y) / frames;
      Sound_PlayFar(SE_RISE_WITH_SPARKS);
      for (i = 0, bolt = &work->particles[1]; i != 63; i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      // UB: the variants are cleared through one s32 pointer stepped a whole
      // step at a time, past the step's scalar members.
#ifdef UBFIX
      for (i = 0; i != 512; i++)
        BOLT_SPARKS[i].variant = -1;
#else
      {
        s32 *variant = &BOLT_SPARKS[0].variant;

        for (i = 0; i != 512; i++) {
          *variant = -1;
          variant += sizeof(struct Particle) / sizeof(s32);
        }
      }
#endif
    }

    // Arrival.
    if (frame == frames) {
      s32 n;

      for (n = 0, spark = BOLT_SPARKS; n != 128; n++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = ball->x / 2;
        spark->y = ball->y;
        angle = Random16();
        speed = Random16() % 400 + 32;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spark->variant = (Random16() & 7) + 16;
      }
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      work->shakeFrames = 16;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      if (mode == BOLT_TRAIL_THUNDER_MINE) {
        EFFECT_SHOW_DAMAGE(SE_CRASH);
      } else {
        EFFECT_SHOW_DAMAGE(-1);
        Sound_PlayFar(SE_CRASH);
      }
    }

    // The ball, growing to 48 (64), with four flashes about it.
    if (frame <= frames) {
      s32 largest;

      ball->variant++;
      x = ball->x >> 17;
      y = Q_16_16_TO_INT(ball->y);
      if (mode == BOLT_TRAIL_THUNDER_MINE) {
        radius = frame * 2 + 16;
        largest = 48;
      } else {
        radius = frame * 2 + 16;
        largest = 64;
      }
      if (radius > largest) {
        radius = largest;
      } else {
        BattleEffect_DrawShadedCircles(buffer, radius, 85536);
        if (mode == BOLT_TRAIL_EARTH_FORCE) {
          for (i = 0; i != radius * radius / 2; i++) {
            if (buffer[i] != 0) {
              buffer[i] = 64 - buffer[i];
            }
          }
        }
      }
      Canvas_BlitAtFoot(buffer, x, y, radius);
      for (i = 0; i != 4; i++) {
        // DEAD: i runs 0 to 3, so the mask changes nothing; the source keeps
        // it.
        s32 cell = i & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX = x + (Q_16_16_TO_INT(Trig_Sin(angle) * half)) -
                     (gBattleEffect_GlintPictureWidths[cell] >> 1);
        s32 flashY = y + (Q_16_16_TO_INT(Trig_Cos(angle) * half)) -
                     (gBattleEffect_GlintPictureHeights[cell] >> 1);
        s32 flags = 3 | gBoltTrail_GlintFlips[Random16() & 3];

        /* FAKEMATCH(dead): a once-only block. */
        do {
        } while (0);
        BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, flags, 1);
        ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
            canvas,
            work->sheet + BOLT_TRAIL_TORNADO_PICTURES +
                gBattleEffect_GlintPictures[cell],
            flashX, flashY, gBattleEffect_GlintPictureWidths[cell],
            gBattleEffect_GlintPictureHeights[cell]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
      BattleEffectKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == BOLT_TRAIL_THUNDER_MINE && frame >= frames) {
      for (i = 0, dot = BOLT_SPARKS; i != 512; i++, dot++) {
        if (dot->variant >= 0) {
          s32 size = (dot->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    (Q_16_16_TO_INT(dot->x)) - size / 2,
                    (Q_16_16_TO_INT(dot->y)) - size, size, size * 2);
            BattleEffectKernels_IntegrateVector2(dot, 62, 0x200);
          }
          if ((Q_16_16_TO_INT(dot->y)) > 111) {
            dot->velocityY = -dot->velocityY * 48 / DAMPING_SCALE;
          }
          dot->variant--;
        }
      }
    }

    // The trailing bolts, each eight frames long.
    if (frame <= frames) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 7;
      // DEAD: both kinds are overwritten by the 6 below; the source keeps them.
      if (mode == BOLT_TRAIL_THUNDER_MINE) {
        model->kind = 6;
      } else {
        model->kind = 9;
      }
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = vertices;
      for (i = 0, bolt = &work->particles[1]; i != 40; i++, bolt++) {
        s32 shade;

        if (frame < i || frame >= i + 8) {
          continue;
        }
        shade = 0;
        if (frame == i) {
          shade = -16;
        } else if (frame >= i + 4) {
          /* FAKEMATCH(temporary): the fade negated through a local. */
          s32 fade = (frame - i) * 16 - 64;

          shade = -fade;
        }
        texture.pixels = work->sheet + 0x1456 + (bolt->variant << 12);
        model->shade = shade;
        Transform_Reset();
        Transform_ApplyTranslation(ball->x / 2 - px(64), ball->y - px(64), 0);
        Transform_ApplyRoll(bolt->z);
        bolt->z += bolt->velocityZ;
        if (mode == BOLT_TRAIL_THUNDER_MINE) {
          Transform_ApplyAxisScale(Q16_ONE * 4, Q16_ONE, Q16_ONE);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, vertices,
                                   4);
        } else {
          Transform_ApplyAxisScale(radius << 11, radius * 312, Q16_ONE);
          Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
        }
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPARKS ((struct Particle *)Ram_MapCellBuffer) ///< 512 sparks
#define PICTURES (Ram_MapCellBuffer + 0x4000)
extern const struct Scale gMegaera_UnitScale;

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define MEGAERA_FLASH_BURST_PICTURES 0x3800
/// Where LightningBoltSheet is decoded in the effect's pictures.
#define MEGAERA_LIGHTNING_BOLT_PICTURES 0xc56
/// Where SparkleDots is decoded in the effect's pictures.
#define MEGAERA_SPARKLE_DOTS_PICTURES 0x2e16

#define MEGAERA_FRAMES 170

/// Two bolts of lightning fall at frames 0 and 16, each with a burst of
/// sparks; a whirl of flame turns over the field from 64 while smoke rises,
/// and at 108 Megaera (eleven objects) swoops in across the targets in a
/// trail of sparkles. Holding A or B after frame 10 skips to the swoop.
void BattleEffect_RunMegaera(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  DrawRectangle *rects;
  struct Point2D position;
  struct Point2D velocity;
  DrawRectangle blitters[2];
  struct Scale request;
  s32 placement[4];
  struct Particle *p;
  u8 *pictures;
  s32 i;
  s32 j;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  pictures = PICTURES;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  rects = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)rects);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + MEGAERA_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, pictures, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             pictures + MEGAERA_LIGHTNING_BOLT_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots,
                             pictures + MEGAERA_SPARKLE_DOTS_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    work->particles[i].variant = -1;
  }
  for (i = 0, p = &work->particles[24]; i != 16; i++, p++) {
    p->x = Random16() & 127;
    p->y = (Random16() & 7) + 56;
    p->variant = -(Random16() & 15);
  }
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    p->x = px((Random16() & 31) + 28);
    p->y = px((Random16() % 48) + 56);
    p->velocityX = ((Random16() & 255) - 128) << 12;
    p->velocityY = (16 - (Random16() & 127)) << 10;
    p->variant = ~i;
  }
  for (i = 0; i != 512; i++) {
    SPARKS[i].variant = -1;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                       SHEET_PALETTE_BYTES);
  BattleEffect_SelectLivingTargets(work->args);
  wait(1);

  for (frame = 0; frame != MEGAERA_FRAMES; frame++) {
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 10) {
      if (frame > 107) {
        goto skipped;
      }
      frame = 107;
    }
    if (frame == 107) {
      Sound_PlayFar(SE_POWER_UP);
    }
  skipped:
    if (frame == 148) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame == 0) {
      position.x = px(32);
      position.y = px(72);
      velocity.x = px(9);
      velocity.y = -px(2);
      BattleEffect_SpawnObjects(2, 657, 2);
    }
    if (frame == 108) {
      Sprite_DestroyFar(work->sprites[0]);
      Sprite_DestroyFar(work->sprites[1]);
      BattleEffect_SpawnObjects(11, 656, 2);
    }

    // Megaera swoops, trailing sparkles.
    if (frame > 107) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 59 / DAMPING_SCALE;
      velocity.y = velocity.y * 59 / DAMPING_SCALE;
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_MEGAERA, position.x,
                                         position.y);
      if (frame == 108) {
        for (i = 0; i != 512; i++) {
          SPARKS[i].variant = -1;
        }
      }
      {
        s32 t = frame - 108;
        if ((u32)t <= 23) {
          s32 count = 10 - t / 2;

          if (count > 0) {
            s32 half;

            for (i = 0, half = position.x / 2; i != count / 5; i++) {
              struct Particle *cell;
              s32 angle;
              s32 radius;

              cell = &SPARKS[((t << 5) + i) & 511];
              angle = Random16();
              radius = (Random16() & 31) + 16;
              cell->x = Trig_Sin(angle) * radius + half + 0x100000;
              cell->y = Trig_Cos(angle) * radius + position.y - px(8);
              cell->velocityX = 0;
              cell->velocityY = 0;
              cell->variant = Random16() & 15;
            }
          }
        }
      }
    }
    if ((u32)(frame - 64) <= 3) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
    }
    if (frame > 103) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_TanPalette);
    }
    if (frame == 64) {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }

    // The whirl of flame, and smoke.
    if (frame > 63) {
      if (frame <= 111) {
        s32 picture = (frame / 4) % 3;
        s32 sway = (Q_16_16_TO_INT(Trig_Sin(frame << 11) << 3)) - 16;
        s32 which = frame == 108;
        DrawRectangle *draw = &blitters[which];
        u8 *cells = work->sheet + picture * 3 * 0x800;

        (*draw)(canvas, cells, sway, 0, 96, 64);
        (*draw)(canvas, cells, sway, 64, 96, 52);
        {
          s32 flickerFrame = (frame / 2) % 3;
          u8 *flickerPixels;

          sway = Q_16_16_TO_INT(Trig_Sin((frame << 12) + TRIG_TURN / 2) << 3);
          flickerPixels = work->sheet + flickerFrame * 3 * 0x800;
          (*draw)(canvas, flickerPixels, sway, 0, 96, 64);
          (*draw)(canvas, flickerPixels, sway, 64, 96, 52);
        }
      }
      for (i = 0, p = work->particles; i != 24; i++, p++) {
        if ((u32)p->variant <= 15) {
          s32 row = p->variant / 4;
          s32 which = i & 1;
          u8 *smoke = work->sheet + 0x4800 + (row << 11);

          rects[which](canvas, smoke, Q_16_16_UPPER_HALF(p->x) - 16,
                       Q_16_16_UPPER_HALF(p->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 62, -px(1) / 4);
        }
        p->variant++;
        if (frame == 108) {
          p->velocityX += px(2);
          p->velocityY /= 4;
        }
        if (frame <= 107) {
          s32 v = p->variant;

          if (v == 16) {
            p->variant = v = 0;
          }
          if (v == 0) {
            p->x = px((Random16() & 31) + 28);
            p->y = px((Random16() % 48) + 56);
            p->velocityX = ((Random16() & 255) - 128) << 12;
            p->velocityY = (16 - (Random16() & 127)) << 10;
          }
        }
      }
    }
    if (frame == 64) {
      Sound_PlayFar(SE_163);
    }
    {
      s32 crackle;

      for (i = 0, crackle = 75; i != 8; i++, crackle += 4) {
        if (frame == crackle) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
        }
      }
    }
    if (frame == 10) {
      work->shakeFrames = i;
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
      }
    }
    if (frame > 107) {
      for (i = 0, p = SPARKS; i != 512; i++, p++) {
        s32 k;

        if ((u32)p->variant > 35) {
          continue;
        }
        k = (p->variant / 2) % 9;
        blitters[0](
            canvas, (PICTURES + 0x2e16) + gBattleEffect_SparklePictures[k],
            Q_16_16_UPPER_HALF(p->x) - gBattleEffect_SparkleSizes[k] / 2,
            Q_16_16_UPPER_HALF(p->y) - gBattleEffect_SparkleSizes[k] / 2,
            gBattleEffect_SparkleSizes[k], gBattleEffect_SparkleSizes[k]);
        BattleEffectKernels_IntegrateVector2(p, 58, 0);
        if (i & 1) {
          p->velocityX += -(px(1) / 32);
          p->velocityY += px(1) / 16;
        } else {
          p->velocityX += -(px(1) / 16);
          p->velocityY += px(1) / 32;
        }
        p->variant++;
      }
    }

    // Two bolts of lightning.
    if (frame <= 63) {
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      for (i = 0; i != 2; i++) {
        s32 start = i * 16;
        s32 column = i * 32 + 24;

        if (frame == start + 5) {
          Sound_PlayFar(SE_POWER_UP);
        }
        if (frame == start + 9) {
          work->shakeFrames = 8;
          Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                               CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          Sound_PlayFar(SE_CLATTER);
        }
        if (frame >= start) {
          s32 t = frame - start;
          s32 fall = t * 16 - 16;

          request = gMegaera_UnitScale;
          if (fall > 131) {
            fall = 132;
          }
          placement[3] = 0;
          placement[1] = 0;
          placement[0] = column << 17;
          placement[2] = px(fall);
          Render_ApplyProjectedPlacementFar(work->sprites[i], placement,
                                            &request.x, 0);
          if (t == 9) {
            Iwram_ClearWordsEntry(
                (u8 *)0x06010e00 +
                    ((u32)(((u16 *)work->sprites[i])[4] << 22) >> 17),
                0x200);
          }
          if (frame == start + 10) {
            for (j = 0; j != 256; j++) {
              struct Particle *spark = &SPARKS[i * 256 + j];
              s32 angle = Random16() & 0x7fff;
              s32 speed = Random16() & 255;

              spark->x = px(column);
              spark->y = px(112);
              spark->velocityY = (-Trig_Sin(angle) * speed) >> 5;
              spark->velocityX = (Trig_Cos(angle) * speed) >> 5;
              spark->variant = 0;
            }
          }
          if (frame >= start + 10 && frame < start + 18) {
            blitters[0](canvas,
                        (PICTURES + 0xc56) + (((frame + i) / 4) & 3) * 2880,
                        column - 12, 0, 24, 112);
          }
          if (frame >= start + 10) {
            for (j = 0; j != 2;) {
              s32 k = j & 3;
              s32 gx = column + (Random16() & 7) -
                       (gBattleEffect_GlintPictureWidths[k] >> 1);
              s32 gy = (Random16() & 63) -
                       (gBattleEffect_GlintPictureHeights[k] >> 1);

              /* FAKEMATCH(dead): an empty once-only loop. */
              do {
              } while (0);
              BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
                                        3 | gMegaera_GlintFlips[Random16() & 3],
                                        gMegaera_GlintModes[args->variant]);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas, PICTURES + gBattleEffect_GlintPictures[k], gx - 4,
                  gy + 40, gBattleEffect_GlintPictureWidths[k],
                  gBattleEffect_GlintPictureHeights[k]);
              j++;
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
          }
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      rects[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      if (frame <= 69) {
        for (i = 0, p = SPARKS; i != 512; i++, p++) {
          s32 size;

          if (p->variant < 0) {
            continue;
          }
          size = i % 3 + 1;
          blitters[p->velocityY > 0](
              canvas, sprites + gBattleEffect_MotePictures[size - 1],
              Q_16_16_UPPER_HALF(p->x) - size / 2,
              Q_16_16_UPPER_HALF(p->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(p, 62, px(1) / 4);
          p->variant++;
          if (p->velocityY > 0 && Q_16_16_UPPER_HALF(p->y) > 104) {
            p->variant = -1;
          }
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_RunImpactBurst(SUMMON_FORMATION_MEGAERA, position.x, position.y);
  for (i = 0; i != 11; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

struct StripQuad;

#define VOID_BEAM_FRAMES 88
#define BEAM_SEGMENTS 10

/// Where CometSheetB is decoded in the effect's pictures.
#define VOID_BEAM_COMET_B_PICTURES 0x1000
/// Where RuneSheet is decoded in the effect's pictures.
#define VOID_BEAM_RUNE_PICTURES 0x1000

/// A hexagon of motes is traced, side by side, around the caster while a
/// ring of 64 gathers behind; at frame 72 a beam (a 32-line gradient,
/// repeated ten times) shoots from the caster toward the targets' side,
/// fading out from frame 80 with the blend.
///
/// Abilities: Void Beam.
void BattleEffect_RunVoidBeam(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  u8 *sprites;
  s32 point[3];
  struct ScreenPoint corners[11];
  struct ScreenPoint ring;
  struct ScreenPoint segment;
  s32 flip;
  s32 count;
  u8 *row;
  s32 i;
  s32 k;
  u8 *buffer;
  s32 size;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->battleSheet;
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                             work->sheet + VOID_BEAM_COMET_B_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // The motes, one shade dimmer.
  for (i = 0; i != 770; i++) {
    s32 pixel = sprites[i];

    if (pixel != 0) {
      pixel -= 8;
      if (pixel <= 0) {
        pixel = 1;
      }
      sprites[i] = pixel;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet,
                             buffer + VOID_BEAM_RUNE_PICTURES, 1, 0);
  Iwram_ClearWordsEntry(buffer, 0x1000);

  // The beam's head: the rune, mirrored to 48 columns.
  for (i = 0; i != 48; i++) {
    s32 j;

    for (j = 0; j != 24; j++) {
      buffer[0x208 + i * 64 + j] = buffer[0x1000 + i * 24 + j];
      buffer[0x208 + i * 64 + 47 - j] = buffer[0x1000 + i * 24 + j];
    }
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                  SHEET_PALETTE_BYTES);

  // The beam's body: a 32-line gradient.
  row = work->sheet + 0x1a20;
  for (i = 0; i != 32; i++) {
    s32 shade = Q_16_16_TO_INT(Trig_Sin(i << 9) * 63);
    s32 j;

    for (j = 0; j != 32; j++) {
      *row++ = shade;
    }
  }
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  flip = 4;
  if (args->side != EFFECT_SIDE_ENEMIES) {
    flip = 0;
  }
  BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 7, 7, flip | 3, 1);
  BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, flip | 11, 1);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Sound_PlayFar(SE_SPIN);
  size = 4;

  for (frame = 0; frame != VOID_BEAM_FRAMES; frame++) {
    if ((u32)(frame - 9) <= 54) {
      work->cameraDelta = -256;
    } else if ((u32)(frame - 64) <= 15) {
      work->cameraDelta = frame * 16 - 1280;
    } else {
      work->cameraDelta = 0;
    }
    if (args->side == EFFECT_SIDE_PARTY) {
      work->cameraDelta = -work->cameraDelta;
    }
    if (frame > 79) {
      s32 fade = frame * 2 - 160;

      REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
    }
    BattleCamera_SetViewFar(0, 0, 0, 100);
    Transform_Reset();
    Transform_SetLookAt(camera, camera + 3);
    {
      struct MotionObject *caster;
      s32 traced;

      caster = BattleObject_GetFar(args->user)->object;
      if (caster->x > 0) {
        point[0] = caster->x - px(8);
      } else {
        point[0] = caster->x + px(8);
      }

      // The hexagon, one side every 36 frames.
      traced = frame * 8 - 336;
      if (traced >= 0) {
        for (i = 0; i != 6; i++) {
          s32 corner = (i + 4) % 6;

          point[1] = px(gVoidBeam_Corners[corner * 2 + 1]) + px(30);
          point[2] = px(gVoidBeam_Corners[corner * 2]) + caster->z;
          EffectPosition_ProjectPoint(point, &corners[i]);
          corners[i].x >>= 1;
        }
        for (i = 0; i != 6; i++) {
          s32 motes = traced - i * 36;

          if (motes > 36) {
            motes = 36;
          }
          if (motes < 0) {
            continue;
          }
          for (k = 0; k != motes; k++) {
            s32 x =
                corners[i].x +
                Iwram_MulQ16(k * (corners[(i + 1) % 6].x - corners[i].x), 1820);
            s32 y =
                corners[i].y +
                Iwram_MulQ16(k * (corners[(i + 1) % 6].y - corners[i].y), 1820);

            draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The ring behind the caster.
      count = frame * 3 / 2;
      if (count > 64) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        s32 angle = i << 10;

        point[1] = caster->y - Trig_Cos(angle) * 40 + 0x200000;
        point[2] = caster->z + Trig_Sin(angle) * 40;
        EffectPosition_ProjectPoint(point, &ring);
        ring.x >>= 1;
        if (ring.depth <= 159) {
          ring.depth = 160;
        }
        if (ring.depth > 799) {
          ring.depth = 799;
        }
        draw[1](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                ring.x - size / 2, ring.y - size, size, size * 2);
      }

      // The beam.
      if (frame > 71) {
        s32 fade;

        if (frame == 72) {
          work->particles[0].x = point[0];
          if (work->particles[0].x > 0) {
            work->particles[0].velocityX = -px(32);
          } else {
            work->particles[0].velocityX = px(32);
          }
          REG_BG2CNT =
              BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        }
        if (frame == 72) {
          work->shakeFrames = 8;
          EFFECT_SHOW_DAMAGE(SE_CRASH);
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        for (i = 0; i != BEAM_SEGMENTS; i++) {
          point[0] = work->particles[0].x + work->particles[0].velocityX * i;
          point[1] = px(30);
          point[2] = caster->z;
          EffectPosition_ProjectPoint(point, &segment);
          segment.x >>= 1;
          if (args->side == EFFECT_SIDE_ENEMIES) {
            segment.x -= 32;
          }
          if (i > 0) {
            draw[0](canvas, work->sheet + 0x1a20, segment.x, segment.y - 32, 32,
                    32);
            draw[1](canvas, work->sheet + 0x1a20, segment.x, segment.y, 32, 32);
          } else if (frame <= 81) {
            draw[0](canvas, work->sheet + VOID_BEAM_COMET_B_PICTURES, segment.x,
                    segment.y - 36, 32, 36);
            draw[1](canvas, work->sheet + VOID_BEAM_COMET_B_PICTURES, segment.x,
                    segment.y, 32, 36);
          }
        }
        row = work->sheet + 0x1a20;
        fade = 0;
        if (frame > 79) {
          fade = frame * 8 - 640;
        }
        for (i = 0; i != 32; i++) {
          s32 shade = (Q_16_16_TO_INT(Trig_Sin(i << 9) * 63)) - fade;
          s32 j;

          if (shade < 0) {
            shade = 0;
          }
          for (j = 0; j != 32; j++) {
            *row++ = shade;
          }
        }
      }
    }
    if (work->cameraPhase == 0) {
      work->cameraPhase = 1;
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef SPARKS
#undef PICTURES
#undef VOID_BEAM_FRAMES
#undef BEAM_SEGMENTS
