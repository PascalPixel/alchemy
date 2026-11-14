/// @file
/// The Thor summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The sparks; the glint pictures that draw the crackles of lightning come
/// before them.
#define THOR_PARTICLES ((struct Particle *)(Ram_MapGridCellBuffer + 3160))

/// The particle sprites, after the light fan pictures.
#define THOR_SPRITE_PICTURES 6144

/// The violet mote in the second part's sheet.
#define THOR_MOTE_PICTURE 24576

#define THOR_DIMMED_DOTS 10000 ///< the darker copies of the dot pictures

/// Launches up to max sparks from a point, each at angle at a random speed.
#define THOR_LAUNCH_SPARKS(spark, index, startX, startY, angleExpr, max)       \
  {                                                                            \
    s32 spawned = 0;                                                           \
    for (index = 0, spark = THOR_PARTICLES; index != 910; index++, spark++) {  \
      if (spark->variant == 0) {                                               \
        s32 speed = (Random16() & 0x3ff) + 32;                                 \
        s32 angle = angleExpr;                                                 \
        spark->x = startX;                                                     \
        spark->y = startY;                                                     \
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;                     \
        spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;             \
        spark->variant = (Random16() & 7) + 32;                                \
        if (++spawned == (max)) {                                              \
          break;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// Over scrolling dusk clouds a ring with a triangle across it draws itself and
/// turns, its colours cycling, then tips flat, sinks into the sky and sends
/// down a beam of light; then a pillar of lightning strikes the targets, Thor
/// appears in a spray of sparks, violet bolts strike the targets again and
/// again, and Thor drifts back in crackling lightning.
///
/// Abilities: Thor.
void BattleEffect_RunThor(struct BattleEffectParams *args) {
  static s16 points[6][2] = {
      {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
  };
  u8 ramp[32];
  s32 point[3];
  struct ScreenPoint screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundWork *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  /* LEFTOVER(frame): blit[1], the idle half of the pair; the ROM's frame
   * reserves 336 bytes (sub sp, #336), 4 more than one blitter needs, and no
   * instruction addresses the word at sp+112; BlitterPair is the blitter set
   * every effect declares, and BattleEffect_FetchRectangleBlitters and 28
   * battle-effect files fill or call its second half; as one DrawRectangle
   * blit[1], 202 of 1640 instructions differ in tbs-en. */
  BlitterPair blit;
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  struct Particle *spark;

  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  Resource_LoadAndDecompress(RESOURCE_LIGHT_FAN_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_LIGHT_FAN_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_C,
                             work->sheet + THOR_SPRITE_PICTURES, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);

  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != MOTE_SET_BYTES; j++) {
        s32 v = *src++;

        EFFECT_CAP_LEVEL(v, 64 - i * 7);
        (work->sheet + THOR_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  view->busy = 1;
  Task_Add(BattleEffect_AdvanceScrollOnInterval, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  work->transferMode = CANVAS_COPY;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           RESOURCE_DUSK_CLOUDS_BACKDROP, 0);
  gProjection.centerY = 240;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  REG_BG2PA = Q_8_8(0.5);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_SHOW_WIDE_CANVAS(15);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

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
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // First part (A or B skips it): the ring, the triangle and the beam.
  for (frame = 0; frame != 170; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      Sound_PlayFar(SE_SPIN);
    }
    if (frame == 132) {
      Sound_PlayFar(SE_PSYNERGY_SPARKLE);
    }
    if (frame == 151) {
      Sound_PlayFar(SE_CRASH);
    }
    if (gKeysRepeat & (KEY_A | KEY_B)) {
      break;
    }
    // The colours cycle through the hues.
    BattleEffect_UpdatePhasePalette(frame * 2, TRIG_TURN - TRIG_TURN / 3,
                                    TRIG_TURN / 3, 0);
    if (frame == 150) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(26));
    } else {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    Projection_Set(BATTLE_CAMERA_DISTANCE,
                   Iwram_RatioMulQ14(BATTLE_CAMERA_DISTANCE, Q16_ONE * 3 / 4),
                   0x7fff0000);
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
        SET_VECTOR(scale, frame * 6144 - 889856, scale[0], scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, Q16_ONE - sinkFrame * 3 * (Q16_ONE / 64), scale[0],
                   scale[0]);
      }
      Transform_ApplyScale(scale);
    } else {
      SET_VECTOR(position, 0, 0, px(256));
      Transform_ApplyPosition(position);
      Transform_ApplyPitch(-TRIG_TURN / 16);
      Transform_ApplyRoll(frame << 8);
      Transform_ApplyPitch(frame << 8);
    }

    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, px(points[i][0] - 96), 0, px(points[i][1] - 96));
        Projection_Point(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (screen[i].y >> 16) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].x - screen[i * 2].x),
                                 Q16_ONE / 48);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16(j * (screen[i * 2 + 1].y - screen[i * 2].y),
                                 Q16_ONE / 48);
            EFFECT_DRAW_MOTE(blit[0], canvas, sprites, x, y, size);
          }
        }
      }
    }

    // The ring of dots, two more each frame, darkening once the beam has
    // fallen.
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
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        drawAlt = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2];
      } else {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        drawAlt = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2];
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        Projection_Point(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (screen[0].y >> 16) + 60;
        drawAlt(canvas,
                work->sheet +
                    (level * MOTE_SET_BYTES +
                     gBattleEffect_MotePictures[dotSize - 1]) +
                    THOR_DIMMED_DOTS,
                screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
                dotSize * 2);
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
    }

    if ((u32)(frame - 151) <= 16) {
      s32 inset = 0;
      s32 beamHeight = (frame - 151) * 20 + 44;
      if (frame > 151) {
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
        /* FAKEMATCH(dead): the width set to 0 and at once to 32; as the
         * constant 32 in the call, 209 of 1817 instructions differ in tbs-en.
         */
        s32 fullWidth = 0;

        fullWidth = 32;
        blit[0](canvas, ramp + inset, inset + 48, i + 16, fullWidth - inset * 2,
                1);
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 32,
                                            (frame - 151) * 20 - 12, 32, 96);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas, work->sheet, 64,
                                            (frame - 151) * 20 - 12, 32, 96);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Second part: the pillar, Thor and his bolts.
  view->busy = 0;
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  BattleEffect_SetupBlendedDisplay();
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(9, SPRITE_THOR, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, Ram_MapGridCellBuffer, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_VIOLET_MOTE_SHEET,
                             work->sheet + THOR_MOTE_PICTURE, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_CRYSTAL_SHEET), 128);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_B, sprites, 0, 0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 thorX;
    s32 thorY;
    s32 thorSpeedX;
    s32 thorSpeedY;
    s32 pillarX = 60;
    s32 pillarY = 112;

    thorX = px(80);
    thorY = px(64);
    i = 0;
    thorSpeedX = 0;
    thorSpeedY = 0;
    for (; i != 910; i++) {
      THOR_PARTICLES[i].variant = 0;
    }
    // Nothing in Thor reads these 16 particles, and BattleEffect_RunImpactBurst
    // overwrites them; only their random draws change what is drawn later.
    // XXX (no evidence): what they were for; no code that reads them is left.
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    Sound_PlayFar(SE_CRASH);

    for (frame = 0; frame != 192; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress(RESOURCE_VIOLET_LIGHTNING_SHEET_B,
                                   work->sheet, 1, 0);
      }
      stormFrame = frame - 80;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      if (frame == 140) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      // The pillar of lightning, throwing sparks up from its foot.
      // DEAD: a loop over one pillar, j being 0; the ROM keeps its counter,
      // the step of start by 128 and its exit test.
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit[0](canvas, work->sheet, 36, 0, 48, pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + (48 * 112), 36, 0, 48, 112);
          }
          if (frame == start + 2) {
            THOR_LAUNCH_SPARKS(spark, i, px(pillarX), px(pillarY),
                               (Random16() & 0x7fff) - TRIG_TURN / 4, 512);
            work->shakeFrames = 8;
          }
        }
      }
      if (frame == 10) {
        EFFECT_FOR_EACH_TARGET(i, work->args) {
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 8);
          BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                         KNOCKBACK_HURL);
        }
      }
      if ((u32)stormFrame <= 63) {
        s32 moteX;
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + 80) {
            Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
          }
        }
        for (i = 0; i != 30; i++) {
          if (frame >= i * 2 + 80) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](
                  canvas, work->sheet + i % 3 * (72 * 72), boltX, boltY, 72,
                  72);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
            }
            if (frame == i * 2 + 80) {
              THOR_LAUNCH_SPARKS(spark, j, px(16), px(80),
                                 Random16() & TRIG_ANGLE_MASK, 16);
              work->shakeFrames = 8;
              EFFECT_FOR_EACH_TARGET(j, work->args) {
                BattleEffect_TintBattler(work->args->targets[j],
                                         gThor_TargetPalettes[Random16() & 3],
                                         POSE_DAMAGE, j, 4);
                BattleMotion_ApplyKnockbackFar(work->args->targets[j],
                                               KNOCKBACK_HURL);
              }
            }
          }
        }
        {
          s32 radius = (Random16() & 7) + 8;
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          moteX = (Trig_Sin(angle) * radius) >> 16;
          mirrorX = moteX + 72;
          moteY = 32 - ((Trig_Cos(angle) * radius) >> 16);
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas,
                                              work->sheet + THOR_MOTE_PICTURE,
                                              moteX + 60, moteY - 24, 12, 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_LIGHTEN);
        EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](canvas,
                                              work->sheet + THOR_MOTE_PICTURE,
                                              mirrorX, moteY - 24, 12, 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                            COMPOSITE_LIGHTEN);
        EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](
            canvas, work->sheet + THOR_MOTE_PICTURE, moteX + 60, moteY, 12, 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                                COMPOSITE_FLIP_Y,
                            COMPOSITE_LIGHTEN);
        EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](
            canvas, work->sheet + THOR_MOTE_PICTURE, mirrorX, moteY, 12, 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        // Sparks fly off the bolts.
        j = 0;
        for (i = 0, spark = THOR_PARTICLES; i != 910; i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = (Random16() & 63) + 64;
            s32 angle = Random16() & TRIG_ANGLE_MASK;
            spark->x = px(mirrorX);
            spark->y = px(moteY);
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++j == 4) {
              break;
            }
          }
        }
      }

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      {
        // Sparks slow and bounce off the ground, shrinking as they die.
        DrawRectangle draw = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2];
        struct Particle *spark;
        for (i = 0, spark = THOR_PARTICLES; i != 910; i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            Particle_AdvanceWithGravity2D(spark, 60, 0);
            if (spark->y > px(120)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= px(127) - 1 && spark->y >= 0) {
              s32 x = spark->x >> 16;
              s32 y = spark->y >> 16;
              s32 dotSize = spark->variant / 16 + 1;
              EFFECT_DRAW_MOTE(draw, canvas, sprites, x, y, dotSize);
            }
          }
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);

      // Thor stands over the targets, then is pushed back.
      if (frame > 15) {
        if (frame == 32) {
          thorSpeedX = px(4);
          thorSpeedY = -px(1) / 4;
        }
        if (frame > 31) {
          thorX += thorSpeedX;
          thorY += thorSpeedY;
          thorSpeedX = thorSpeedX * 60 / 64;
          thorSpeedY = thorSpeedY * 60 / 64;
        }
        BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_THOR, thorX, thorY);
      }
      if ((u32)(frame - 16) <= 127) {
        s32 thorScreenX;
        for (i = 0, thorScreenX = thorX >> 17; i != 3; i++) {
          s32 glint = i & 3;
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 x = (((Trig_Sin(angle) * 8)) >> 16) + thorScreenX -
                  (gBattleEffect_GlintPictureWidths[glint] >> 1);
          s32 y = ((Trig_Cos(angle) * 40) >> 16) -
                  (gBattleEffect_GlintPictureHeights[glint] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              EFFECT_CLIP_BOUNDS |
                                  gThor_CrackleFlips[Random16() & 3],
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_2](
              canvas,
              Ram_MapGridCellBuffer + gBattleEffect_GlintPictures[glint], x,
              y + 56, gBattleEffect_GlintPictureWidths[glint],
              gBattleEffect_GlintPictureHeights[glint]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
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
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_THOR, thorX, thorY);
  }
  for (i = 0; i != 9; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef THOR_PARTICLES
#undef THOR_MOTE_PICTURE
#undef THOR_DIMMED_DOTS
#undef THOR_LAUNCH_SPARKS
#undef THOR_SPRITE_PICTURES
