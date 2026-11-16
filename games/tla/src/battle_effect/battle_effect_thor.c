/// @file
/// The Thor summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The blitter pair's two heap slots.
#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define BLITTER_ALTERNATE ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

/// The sparks; the glint pictures that draw the crackles of lightning come
/// before them.
#define THOR_PARTICLES ((struct Particle *)(Ram_MapCellBuffer + 3160))
/// The violet mote in the second part's sheet: its left half, which
/// Canvas_BlitAtFoot draws and then mirrors.
#define THOR_MOTE_PICTURE 24576
#define THOR_SPARK_COUNT 910
#define THOR_DIMMED_DOTS 10000
#define THOR_DIMMED_DOT_BYTES MOTE_SET_BYTES
#define THOR_OPENING_FRAMES 170 ///< frames of the ring, triangle and beam
#define THOR_STORM_FRAMES 128   ///< frames of the pillar, Thor and his bolts
#define THOR_BOLTS 18

/// Where ParticleSpritesC is decoded in the effect's pictures.
#define THOR_SPRITE_PICTURES 6144

/// The opening's beam of light, and the storm's bolts.
#define THOR_BEAM_FRAME 151
#define THOR_BOLTS_FRAME 80
/// The second part's sheet: the pillar of lightning in two pictures, then
/// the bolts.
#define THOR_BOLT_WIDTH 72
#define THOR_BOLT_HEIGHT 72

#define THOR_LIGHT_FAN_WIDTH 32
#define THOR_LIGHT_FAN_HEIGHT 96

/// Thor. In the dusk part (THOR_OPENING_FRAMES, which A or B cuts
/// short) a circle of 128 dots and three lines across it form over scrolling
/// dusk clouds, turn, sink into the sky and send down a beam of light; in the
/// lightning part (THOR_STORM_FRAMES) a pillar of lightning strikes the
/// formation and throws sparks, THOR_BOLTS violet bolts follow while a violet
/// mote sways above, and the formation is pushed back in smoke until an
/// impact burst ends the effect.
void BattleEffect_RunThor(struct BattleEffectParams *args) {
  u8 ramp[32];
  FIXED point[3];
  struct ScreenPoint screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundWork *view;
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

  view = Ram_HeapSlots->battleBackground;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  Resource_LoadAndDecompress(RESOURCE_LIGHT_FAN_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_C,
                             work->sheet + THOR_SPRITE_PICTURES, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != THOR_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        if (v > 64 - i * 7) {
          v = 64 - i * 7;
        }
        if (v < 0) {
          v = 0;
        }
        (work->sheet + THOR_DIMMED_DOTS)[offset + j] = v;
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
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_DUSK_CLOUDS_BACKDROP, 0);
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
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(RESOURCE_BLAST_SHEET),
                  SHEET_PALETTE_BYTES);

  for (frame = 0; frame != THOR_OPENING_FRAMES; frame++) {
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
        BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
      } else if ((frame & 63) <= 31) {
        BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
      } else if ((frame & 63) <= 47) {
        BattleEffect_StepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
      } else {
        BattleEffect_StepPaletteToResource(RESOURCE_JUPITER_DJINN_SHEET);
      }
    }
    if (frame == 150) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(26));
    } else {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    Projection_Set(BATTLE_CAMERA_DISTANCE,
                   Iwram_RatioMulQ14(BATTLE_CAMERA_DISTANCE, FIXED_ONE * 3 / 4),
                   0x7fff0000);
    Matrix_Initialize();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame << 17, toFIXED(512));
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      Matrix_TranslateVector(position);
      Matrix_RotateX(-TRIG_TURN / 16);
      Matrix_RotateZ(DEGtoANG(180));
      Matrix_RotateX(DEGtoANG(180));
      Matrix_RotateY(sinkFrame << 12);
      if (frame > 150) {
        SET_VECTOR(scale, ((frame * 2 + frame) << 11) - 889856, scale[0],
                   scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, FIXED_ONE - sinkFrame * 3 * (FIXED_ONE / 64),
                   scale[0], scale[0]);
      }
      Matrix_ScaleVector(scale);
    } else {
      SET_VECTOR(position, 0, 0, toFIXED(256));
      Matrix_TranslateVector(position);
      Matrix_RotateX(-TRIG_TURN / 16);
      Matrix_RotateZ(frame << 8);
      Matrix_RotateX(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, INTtoFIXED(gThor_Points[i][0] - 96), 0,
                   INTtoFIXED(gThor_Points[i][1] - 96));
        EffectPosition_ProjectPoint(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (FIXEDtoINT(screen[i].y)) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulFX((screen[i * 2 + 1].x - screen[i * 2].x) * j,
                                FIXED_ONE / 48);
            s32 y = screen[i * 2].y +
                    Iwram_MulFX((screen[i * 2 + 1].y - screen[i * 2].y) * j,
                                FIXED_ONE / 48);
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
        screen[0].y = (FIXEDtoINT(screen[0].y)) + 60;
        drawAlt(canvas,
                work->sheet +
                    (level * THOR_DIMMED_DOT_BYTES +
                     gBattleEffect_MotePictures[dotSize - 1]) +
                    THOR_DIMMED_DOTS,
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
      s32 flags;
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
      flags = EFFECT_CLIP_BOUNDS;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      BLITTER_ALTERNATE(canvas, work->sheet, 32,
                        (frame - THOR_BEAM_FRAME) * 20 - 12,
                        THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
      BLITTER_ALTERNATE(canvas, work->sheet, 64,
                        (frame - THOR_BEAM_FRAME) * 20 - 12,
                        THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // The lightning part.
  view->busy = 0;
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  BattleEffect_SetupBlendedDisplay();
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(9, SPRITE_THOR, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, Ram_MapCellBuffer, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_VIOLET_MOTE_SHEET,
                             work->sheet + THOR_MOTE_PICTURE, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_CRYSTAL_SHEET),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_B, sprites, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    FIXED formationX = toFIXED(80);
    FIXED formationY = toFIXED(64);
    s32 formationSpeedX = 0;
    s32 formationSpeedY = 0;
    s32 pillarX;
    s32 pillarY;

    for (i = 0; i != THOR_SPARK_COUNT; i++) {
      THOR_PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Sound_PlayFar(SE_CRASH);

    for (frame = 0; frame != THOR_STORM_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress(RESOURCE_VIOLET_LIGHTNING_SHEET_B,
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
            blit[0](canvas, work->sheet + PICTURE_BYTES(LIGHTNING_PILLAR), 36,
                    0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
          }
          if (frame == start + 2) {
            spawned = 0;
            for (i = 0, spark = THOR_PARTICLES; i != THOR_SPARK_COUNT;
                 i++, spark++) {
              if (spark->variant == 0) {
                s32 speed = 0x3ff;
                s32 angle;
                speed &= Random16();
                angle = (Random16() & 0x7fff) - DEGtoANG(90);
                spark->x = INTtoFIXED(pillarX);
                spark->y = INTtoFIXED(pillarY);
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
        for (i = 0; i != THOR_BOLTS; i++) {
          if (frame >= i * 2 + THOR_BOLTS_FRAME) {
            if (frame < i * 2 + THOR_BOLTS_FRAME + 2) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  BLITTER_MAX);
              BLITTER_ALTERNATE(
                  canvas, work->sheet + i % 3 * PICTURE_BYTES(THOR_BOLT), boltX,
                  boltY, THOR_BOLT_WIDTH, THOR_BOLT_HEIGHT);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            if (frame == i * 2 + THOR_BOLTS_FRAME) {
              spawned = 0;
              for (j = 0, spark = THOR_PARTICLES; j != THOR_SPARK_COUNT;
                   j++, spark++) {
                if (spark->variant == 0) {
                  s32 speed = 0x3ff;
                  s32 angle;
                  speed &= Random16();
                  angle = Random16() & TRIG_ANGLE_MASK;
                  spark->x = toFIXED(16);
                  spark->y = toFIXED(80);
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
          mirrorX = (FIXEDtoINT(Trig_Sin(angle) * (radius + 8))) + 72;
          moteY = 32 - (FIXEDtoINT(Trig_Cos(angle) * (radius + 8)));
        }
        Canvas_BlitAtFoot(work->sheet + THOR_MOTE_PICTURE, mirrorX, moteY, 24);
        spawned = 0;
        for (i = 0, spark = THOR_PARTICLES; i != THOR_SPARK_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            speed &= Random16();
            angle = Random16() & TRIG_ANGLE_MASK;
            spark->x = INTtoFIXED(mirrorX);
            spark->y = INTtoFIXED(moteY);
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
        for (i = 0, spark = THOR_PARTICLES; i != THOR_SPARK_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            Particle_AdvanceWithGravity2D(spark, 60, 0);
            if (spark->y > toFIXED(120)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = FIXEDtoINT(spark->x);
              s32 y = FIXEDtoINT(spark->y);
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
          formationSpeedX = toFIXED(4);
          formationSpeedY = -toFIXED(1) / 4;
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
          u32 flip;
          cell = i & 3;
          angle = Random16() & TRIG_ANGLE_MASK;
          x = (FIXEDtoINT(Trig_Sin(angle) * 8)) + formationScreenX -
              (gBattleEffect_GlintPictureWidths[cell] >> 1);
          y = (FIXEDtoINT(Trig_Cos(angle) * 40)) -
              (gBattleEffect_GlintPictureHeights[cell] >> 1);
          flip = Random16();
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              3 | gThor_CrackleFlips[flip & 3], 1);
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
