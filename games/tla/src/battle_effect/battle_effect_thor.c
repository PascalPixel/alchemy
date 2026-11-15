/// @file
/// The Thor summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// Heap slots 104 and 188 (byte offsets from 0x03000000) hold the blitters.
#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define BLITTER_ALTERNATE ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

#define STORM_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 0xc58))
#define STORM_MOTE_CELLS 0x6000
#define STORM_PARTICLE_COUNT 910
#define STORM_DIMMED_DOTS 10000
#define STORM_DIMMED_DOT_BYTES 0x302
#define STORM_DUSK_FRAMES 170
#define STORM_LIGHTNING_FRAMES 128
#define STORM_BOLTS 18

/// Where ParticleSpritesC is decoded in the effect's pictures.
#define THOR_PARTICLE_SPRITES_C_PICTURES 0x1800

/// The opening's beam of light, and the storm's bolts.
#define THOR_BEAM_FRAME 151
#define THOR_BOLTS_FRAME 80
/// The second part's sheet: the pillar of lightning in two pictures, then
/// the bolts.
#define THOR_BOLT_WIDTH 72
#define THOR_BOLT_HEIGHT 72

/// Thor. In the dusk part (STORM_DUSK_FRAMES, which A or B cuts
/// short) a circle of 128 dots and three lines across it form over scrolling
/// dusk clouds, turn, sink into the sky and send down a beam of light; in the
/// lightning part (STORM_LIGHTNING_FRAMES) a pillar of lightning strikes the
/// formation and throws sparks, STORM_BOLTS violet bolts follow while a violet
/// mote sways above, and the formation is pushed back in smoke until an
/// impact burst ends the effect.
void BattleEffect_RunThor(struct BattleEffectParams *args) {
  u8 ramp[32];
  s32 point[3];
  struct ScreenPoint screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundView *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  DrawRectangle blit[2]; // Unused: blit[1] gives the stack frame its 4 bytes.
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  s32 spawned;
  struct Particle *spark;

  view = (struct BattleBackgroundView *)Ram_HeapSlots->battleBackground;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC,
                             work->sheet + THOR_PARTICLE_SPRITES_C_PICTURES, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != STORM_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        if (v > 64 - i * 7) {
          v = 64 - i * 7;
        }
        if (v < 0) {
          v = 0;
        }
        (work->sheet + STORM_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  REG_BG2CNT =
      BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  view->busy = 1;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_FRAME);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->transferMode = CANVAS_COPY;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  REG_BG2PA = Q_8_8(0.5);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = BLITTER;
  EFFECT_SHOW_WIDE_CANVAS(15);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // One row of the beam: brightest in the middle, darker to each side.
  for (i = 0; i != 16; i++) {
    ramp[i] = i * 8 + 3;
    ramp[31 - i] = i * 8 + 3;
    if (ramp[i] > CANVAS_LEVEL_BRIGHTEST) {
      ramp[i] = CANVAS_LEVEL_BRIGHTEST;
    }
    if (ramp[31 - i] > CANVAS_LEVEL_BRIGHTEST) {
      ramp[31 - i] = CANVAS_LEVEL_BRIGHTEST;
    }
  }
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlastSheet),
                  SHEET_PALETTE_BYTES);

  for (frame = 0; frame != STORM_DUSK_FRAMES; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 132) {
      Sound_PlayFar(SE_PSYNERGY_SPARKLE);
    }
    if (frame == THOR_BEAM_FRAME) {
      Sound_PlayFar(SE_CRASH);
    }
    if (gInput.repeat & (KEY_A | KEY_B)) {
      break;
    }
    for (i = 0; i != 2; i++) {
      if ((frame & 63) <= 15) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      } else if ((frame & 63) <= 31) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MercuryDjinnSheet);
      } else if ((frame & 63) <= 47) {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
      } else {
        BattleEffect_StepPaletteToResource((s32)&ResourceId_JupiterDjinnSheet);
      }
    }
    if (frame == 150) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(26));
    } else {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    Projection_Set(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
    Transform_Reset();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame << 17, px(512));
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      Transform_ApplyPosition(position);
      Transform_ApplyPitch(-TRIG_TURN / 16);
      Transform_ApplyRoll(TRIG_TURN / 2);
      Transform_ApplyPitch(TRIG_TURN / 2);
      Transform_ApplyYaw(sinkFrame << 12);
      if (frame > 150) {
        SET_VECTOR(scale, ((frame * 2 + frame) << 11) - 0xd9400, scale[0],
                   scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, Q16_ONE - sinkFrame * 3 * 0x400, scale[0], scale[0]);
      }
      Transform_ApplyScale(scale);
    } else {
      SET_VECTOR(position, 0, 0, px(256));
      Transform_ApplyPosition(position);
      Transform_ApplyPitch(-TRIG_TURN / 16);
      Transform_ApplyRoll(frame << 8);
      Transform_ApplyPitch(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, px(gThor_Points[i][0] - 96), 0,
                   px(gThor_Points[i][1] - 96));
        EffectPosition_ProjectPoint(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (Q_16_16_TO_INT(screen[i].y)) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16((screen[i * 2 + 1].x - screen[i * 2].x) * j,
                                 0x555);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16((screen[i * 2 + 1].y - screen[i * 2].y) * j,
                                 0x555);
            blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    // The circle of 128 dots, darkening once the beam has fallen.
    if (frame <= 179) {
      DrawRectangle drawAlt;
      s32 level = 0;

      if (frame > 155) {
        level = frame - 156;
      }
      if (level > 7) {
        level = 7;
      }
      if (frame <= 139) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
        drawAlt = BLITTER_ALTERNATE;
      } else {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            BLITTER_MAX);
        drawAlt = BLITTER_ALTERNATE;
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        EffectPosition_ProjectPoint(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (Q_16_16_TO_INT(screen[0].y)) + 60;
        drawAlt(canvas,
                work->sheet +
                    (level * STORM_DIMMED_DOT_BYTES +
                     gBattleEffect_MotePictures[dotSize - 1]) +
                    STORM_DIMMED_DOTS,
                screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
                dotSize * 2);
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    // The beam: rows of the ramp narrowing at its top, and the light fan
    // falling down it.
    if ((u32)(frame - 151) <= 16) {
      s32 beamHeight;
      s32 inset = 0;
      s32 never = 0;
      s32 neverToo = 0;
      beamHeight = (frame - THOR_BEAM_FRAME) * 20 + 44;
      if (frame > THOR_BEAM_FRAME) {
        inset = frame - 152;
      }
      if (inset > 15) {
        inset = 15;
      }
      for (i = 1; i != 10; i++) {
        if (inset + i <= 15) {
          blit[0](canvas, &ramp[inset + i], inset + 48 + i, 16 - i,
                  32 - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        blit[0](canvas, ramp + inset, inset + 48, i + 16, 32 - inset * 2, 1);
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
      BLITTER_ALTERNATE(canvas, work->sheet, 32,
                        (frame - THOR_BEAM_FRAME) * 20 - 12, 32, 96);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      /* FAKEMATCH(dead): a never-true test of never and neverToo. */
      if (never || neverToo) {
        beamHeight = 0;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      BLITTER_ALTERNATE(canvas, work->sheet, 64,
                        (frame - THOR_BEAM_FRAME) * 20 - 12, 32, 96);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The lightning part.
  view->busy = 0;
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  BattleEffect_SetupBlendedDisplay();
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(9, 0x282, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet,
                             work->sheet + STORM_MOTE_CELLS, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_CrystalSheet),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress((s32)&ResourceId_Data135, sprites, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    s32 formationX = px(80);
    s32 formationY = px(64);
    s32 formationSpeedX = 0;
    s32 formationSpeedY = 0;
    s32 pillarX;
    s32 pillarY;

    for (i = 0; i != STORM_PARTICLE_COUNT; i++) {
      STORM_PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Sound_PlayFar(SE_CRASH);

    for (frame = 0; frame != STORM_LIGHTNING_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB,
                                   work->sheet, 1, 0);
      }
      stormFrame = frame - THOR_BOLTS_FRAME;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      if (frame == 120) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          pillarX = 60;
          pillarY = 112;
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit[0](canvas, work->sheet, 36, 0, LIGHTNING_PILLAR_WIDTH,
                    pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + 0x1500, 36, 0, LIGHTNING_PILLAR_WIDTH,
                    LIGHTNING_PILLAR_HEIGHT);
          }
          if (frame == start + 2) {
            spawned = 0;
            for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
                 i++, spark++) {
              if (spark->variant == 0) {
                s32 speed = 0x3ff;
                s32 angle;
                speed &= Random16();
                angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
                spark->x = px(pillarX);
                spark->y = px(pillarY);
                speed += 32;
                spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                spark->variant = (Random16() & 7) + 32;
                if (++spawned == 512) {
                  break;
                }
              }
            }
            work->shakeFrames = 8;
          }
        }
      }
      if (frame == 10) {
        for (i = 0; i != args->targetCount; i++) {
          EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_HURL);
        }
      }
      // Frames 80 to 143: the violet bolts, 2 frames apart, and the violet
      // mote swaying in four mirrored copies.
      if ((u32)stormFrame <= 63) {
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + THOR_BOLTS_FRAME) {
            Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
          }
        }
        for (i = 0; i != STORM_BOLTS; i++) {
          if (frame >= i * 2 + THOR_BOLTS_FRAME) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
              BLITTER_ALTERNATE(canvas, work->sheet + i % 3 * 0x1440, boltX,
                                boltY, THOR_BOLT_WIDTH, THOR_BOLT_HEIGHT);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            if (frame == i * 2 + THOR_BOLTS_FRAME) {
              spawned = 0;
              for (j = 0, spark = STORM_PARTICLES; j != STORM_PARTICLE_COUNT;
                   j++, spark++) {
                if (spark->variant == 0) {
                  s32 speed = 0x3ff;
                  s32 angle;
                  speed &= Random16();
                  angle = Random16() & TRIG_ANGLE_MASK;
                  spark->x = px(16);
                  spark->y = px(80);
                  speed += 32;
                  spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                  spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                  spark->variant = (Random16() & 7) + 32;
                  if (++spawned == 16) {
                    break;
                  }
                }
              }
              work->shakeFrames = 8;
              for (j = 0; j != args->targetCount; j++) {
                BattleEffect_TintBattler(args->targets[j],
                                         gThor_TargetPalettes[Random16() & 3],
                                         POSE_DAMAGE, j, 4);
                BattleMotion_ApplyKnockbackFar(args->targets[j],
                                               KNOCKBACK_HURL);
              }
            }
          }
        }
        {
          s32 radius = 7;
          s32 angle;
          radius &= Random16();
          angle = Random16() & TRIG_ANGLE_MASK;
          mirrorX = (Q_16_16_TO_INT(Trig_Sin(angle) * (radius + 8))) + 72;
          moteY = 32 - (Q_16_16_TO_INT(Trig_Cos(angle) * (radius + 8)));
        }
        Canvas_BlitAtFoot(work->sheet + STORM_MOTE_CELLS, mirrorX, moteY, 24);
        spawned = 0;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            speed &= Random16();
            angle = Random16() & TRIG_ANGLE_MASK;
            spark->x = px(mirrorX);
            spark->y = px(moteY);
            speed += 64;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++spawned == 4) {
              break;
            }
          }
        }
      }

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
      {
        DrawRectangle draw = BLITTER_ALTERNATE;
        struct Particle *spark;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            BattleEffectKernels_IntegrateVector2(spark, 60, 0);
            if (spark->y > px(120)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = Q_16_16_TO_INT(spark->x);
              s32 y = Q_16_16_TO_INT(spark->y);
              s32 dotSize = spark->variant / 16 + 1;
              draw(canvas, sprites + gBattleEffect_MotePictures[dotSize - 1],
                   x - dotSize / 2, y - dotSize, dotSize, dotSize * 2);
            }
          }
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

      // The formation is pushed back from frame 32.
      if (frame > 15) {
        if (frame == 32) {
          formationSpeedX = px(4);
          formationSpeedY = -px(1) / 4;
        }
        if (frame > 31) {
          formationX += formationSpeedX;
          formationY += formationSpeedY;
          formationSpeedX = formationSpeedX * 60 / DAMPING_SCALE;
          formationSpeedY = formationSpeedY * 60 / DAMPING_SCALE;
        }
        BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_THOR, formationX,
                                           formationY);
      }
      // Smoke around the formation.
      if ((u32)(frame - 16) <= 127) {
        s32 formationScreenX;
        for (i = 0, formationScreenX = formationX >> 17; i != 3; i++) {
          s32 cell;
          s32 angle;
          s32 x;
          s32 y;
          s32 never = 0;
          s32 neverToo = 0;
          cell = i & 3;
          angle = Random16() & TRIG_ANGLE_MASK;
          x = (Q_16_16_TO_INT(Trig_Sin(angle) * 8)) + formationScreenX -
              (gBattleEffect_GlintPictureWidths[cell] >> 1);
          y = (Q_16_16_TO_INT(Trig_Cos(angle) * 40)) -
              (gBattleEffect_GlintPictureHeights[cell] >> 1);
          /* FAKEMATCH(dead): a never-true test of never and neverToo, both 0,
           * that clears y. */
          if (never || neverToo) {
            y = 0;
          }
          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7,
                                    3 | gThor_CrackleFlips[Random16() & 3], 1);
          BLITTER_ALTERNATE(
              canvas, Ram_MapCellBuffer + gBattleEffect_GlintPictures[cell], x,
              y + 56, gBattleEffect_GlintPictureWidths[cell],
              gBattleEffect_GlintPictureHeights[cell]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
      if (frame <= 31) {
        BattleCamera_Shake(8, 16);
      } else {
        BattleCamera_Shake(4, 4);
      }
      EFFECT_END_FRAME(work);
    }

    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_THOR, formationX, formationY);
  }
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}
