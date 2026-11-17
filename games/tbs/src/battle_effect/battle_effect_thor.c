/// @file
/// The Thor summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define THOR_OPENING_FRAMES 170 ///< frames of the ring, triangle and beam
#define THOR_STORM_FRAMES 192   ///< frames of the pillar, Thor and his bolts
/// The opening's beam of light, and the storm's bolts.
#define THOR_BEAM_FRAME 151
#define THOR_BOLTS_FRAME 80

/// The sparks, from the first word after the glint pictures that draw the
/// crackles of lightning.
#define THOR_PARTICLES                                                         \
  ((struct Particle *)(Ram_MapGridCellBuffer + ((GLINT_SET_BYTES + 3) & ~3)))
#define THOR_SPARK_COUNT 910
/// The sparks the pillar of lightning throws up from its foot, each bolt
/// throws out, and the violet mote sheds each frame; a spark's dot shrinks a
/// size for each THOR_SPARK_SHRINK_FRAMES of its life it loses.
#define THOR_PILLAR_SPARKS 512
#define THOR_BOLT_SPARKS 16
#define THOR_MOTE_SPARKS 4
#define THOR_SPARK_SHRINK_FRAMES 16

/// The particle sprites, after the light fan pictures.
#define THOR_SPRITE_PICTURES 6144

/// The violet mote in the second part's sheet, a quarter drawn four times
/// mirrored.
#define THOR_MOTE_PICTURE 24576
#define THOR_MOTE_PICTURE_WIDTH 12
#define THOR_MOTE_PICTURE_HEIGHT 24
/// The light fan, at the first part's sheet start, drawn twice mirrored.
#define THOR_LIGHT_FAN_WIDTH 32
#define THOR_LIGHT_FAN_HEIGHT 96
/// The second part's sheet: the pillar of lightning in two pictures, then
/// the bolts.
#define THOR_BOLT_WIDTH 72
#define THOR_BOLT_HEIGHT 72

/// The darker copies of the dot pictures, which the ring darkens through once
/// the beam has fallen: THOR_DIMMED_DOT_SETS sets, each capped 7 levels lower
/// than the one before, the first not at all.
#define THOR_DIMMED_DOTS 10000
#define THOR_DIMMED_DOT_SETS 8
#define THOR_DIMMED_DOT_CAP(set) (CANVAS_LEVEL_BRIGHTEST + 1 - (set) * 7)

#define THOR_SPRITES 9 ///< Thor's sprites, in SUMMON_FORMATION_THOR

/// Where the projection's centre falls on the canvas, which is shown at twice
/// its width.
#define THOR_CANVAS_CENTER_X (CANVAS_SIZE / 2)
#define THOR_CANVAS_CENTER_Y 60

/// The ring of dots, and the three lines across it, between the points of
/// lineEnds, measured from the corner of the ring's square. Each line
/// draws four dots a frame from THOR_LINES_FRAME, the next starting when one is
/// whole.
#define THOR_RING_RADIUS 96
#define THOR_RING_DOTS 128
#define THOR_LINE_DOTS 48
#define THOR_LINES_FRAME 64

/// The beam of light: a ramp of levels THOR_BEAM_WIDTH wide, brightest in its
/// middle, down from THOR_BEAM_TOP, its tip narrowing a pixel each side a row
/// over THOR_BEAM_TIP_ROWS rows, and THOR_BEAM_SPEED rows longer each frame;
/// it narrows a pixel each side a frame after its first.
#define THOR_BEAM_WIDTH 32
#define THOR_BEAM_X ((CANVAS_SIZE - THOR_BEAM_WIDTH) / 2)
#define THOR_BEAM_TOP 16
#define THOR_BEAM_TIP_ROWS 9
#define THOR_BEAM_SPEED 20
#define THOR_BEAM_LAST_FRAME (THOR_BEAM_FRAME + 16)

/// A dead loop's step between pillars of lightning, of which Thor strikes one,
/// THOR_PILLAR_FRAME frames into the storm.
#define THOR_PILLAR_INTERVAL 128
#define THOR_PILLAR_FRAME 8

#define THOR_BOLTS 30 ///< violet bolts, one each 2 frames from THOR_BOLTS_FRAME
#define THOR_FLASHES 6 ///< the canvas flashes, one each 8 frames from then

/// The crackles of lightning around Thor: within 8 pixels across and 40 up
/// and down of him.
#define THOR_CRACKLE_RADIUS_X 8
#define THOR_CRACKLE_RADIUS_Y 40

/// Particles Thor fills and never reads (see where they are set).
#define THOR_STRAY_PARTICLES 16

/// Launches up to max sparks from a point, each at angle at a random speed.
#define THOR_LAUNCH_SPARKS(spark, index, startX, startY, angleExpr, max)       \
  {                                                                            \
    s32 spawned = 0;                                                           \
    for (index = 0, spark = THOR_PARTICLES; index != THOR_SPARK_COUNT;         \
         index++, spark++) {                                                   \
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
  static s16 lineEnds[6][2] = {
      {8, 135}, {184, 135}, {128, 135}, {96, 32}, {96, 32}, {64, 135},
  };
  u8 ramp[THOR_BEAM_WIDTH];
  FIXED point[3];
  struct ScreenPoint screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundWork *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  /* LEFTOVER(frame): blit[1], the idle half of the pair. */
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
    for (i = 0, offset = 0; i != THOR_DIMMED_DOT_SETS; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != MOTE_SET_BYTES; j++) {
        s32 v = *src++;

        EFFECT_CAP_LEVEL(v, THOR_DIMMED_DOT_CAP(i));
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
  gProjection.centerY = SUMMON_PROJECTION_CENTER_Y;
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
  REG_BG2PA = Q_8_8(0.5);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  blit[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX - 1);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // One row of the beam, its edges dimmest.
  for (i = 0; i != THOR_BEAM_WIDTH / 2; i++) {
    ramp[i] = i * 8 + 3;
    ramp[THOR_BEAM_WIDTH - 1 - i] = i * 8 + 3;
    if (ramp[i] > CANVAS_LEVEL_BRIGHTEST) {
      ramp[i] = CANVAS_LEVEL_BRIGHTEST;
    }
    if (ramp[THOR_BEAM_WIDTH - 1 - i] > CANVAS_LEVEL_BRIGHTEST) {
      ramp[THOR_BEAM_WIDTH - 1 - i] = CANVAS_LEVEL_BRIGHTEST;
    }
  }
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // First part (A or B skips it): the ring, the triangle and the beam.
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
    if (gKeysRepeat & (KEY_A | KEY_B)) {
      break;
    }
    // The colours cycle through the hues.
    BattleEffect_UpdatePhasePalette(frame * 2, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);
    if (frame == 150) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(26));
    } else {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);
    }
    Projection_Set(BATTLE_CAMERA_DISTANCE,
                   Iwram_RatioMulQ14(BATTLE_CAMERA_DISTANCE, FIXED_ONE * 3 / 4),
                   toFIXED(32767));
    Matrix_Initialize();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame * toFIXED(2), toFIXED(512));
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      Matrix_TranslateVector(position);
      Matrix_RotateX(-TRIG_TURN / 16);
      Matrix_RotateZ(DEGtoANG(180));
      Matrix_RotateX(DEGtoANG(180));
      Matrix_RotateY(sinkFrame * (TRIG_TURN / 16));
      if (frame > 150) {
        SET_VECTOR(scale, frame * 6144 - 889856, scale[0], scale[0]);
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
      Matrix_RotateZ(frame * (TRIG_TURN / 256));
      Matrix_RotateX(frame * (TRIG_TURN / 256));
    }

    // The three lines, drawn dot by dot from end to end.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, INTtoFIXED(lineEnds[i][0] - THOR_RING_RADIUS), 0,
                   INTtoFIXED(lineEnds[i][1] - THOR_RING_RADIUS));
        Projection_Point(point, &screen[i]);
        screen[i].x = (FIXEDtoINT(screen[i].x) >> 1) + THOR_CANVAS_CENTER_X;
        screen[i].y = FIXEDtoINT(screen[i].y) + THOR_CANVAS_CENTER_Y;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * THOR_LINE_DOTS - THOR_LINES_FRAME * 4;
        if (dotCount > THOR_LINE_DOTS) {
          dotCount = THOR_LINE_DOTS;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulFX(j * (screen[i * 2 + 1].x - screen[i * 2].x),
                                FIXED_ONE / THOR_LINE_DOTS);
            s32 y = screen[i * 2].y +
                    Iwram_MulFX(j * (screen[i * 2 + 1].y - screen[i * 2].y),
                                FIXED_ONE / THOR_LINE_DOTS);
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
      if (level > THOR_DIMMED_DOT_SETS - 1) {
        level = THOR_DIMMED_DOT_SETS - 1;
      }
      if (frame <= 139) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        drawAlt = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
      } else {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        drawAlt = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
      }
      dotCount = count;
      if (dotCount > THOR_RING_DOTS) {
        dotCount = THOR_RING_DOTS;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] =
            Trig_Sin(i * (TRIG_TURN / THOR_RING_DOTS)) * THOR_RING_RADIUS;
        point[2] =
            -(Trig_Cos(i * (TRIG_TURN / THOR_RING_DOTS)) * THOR_RING_RADIUS);
        Projection_Point(point, &screen[0]);
        screen[0].x = (FIXEDtoINT(screen[0].x) >> 1) + THOR_CANVAS_CENTER_X;
        screen[0].y = FIXEDtoINT(screen[0].y) + THOR_CANVAS_CENTER_Y;
        drawAlt(canvas,
                work->sheet +
                    (level * MOTE_SET_BYTES +
                     gBattleEffect_MotePictures[dotSize - 1]) +
                    THOR_DIMMED_DOTS,
                screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
                dotSize * 2);
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }

    // The beam, and the light fan falling down it.
    if (frame >= THOR_BEAM_FRAME && frame <= THOR_BEAM_LAST_FRAME) {
      s32 inset = 0;
      s32 beamHeight = (frame - THOR_BEAM_FRAME) * THOR_BEAM_SPEED + 44;
      if (frame > THOR_BEAM_FRAME) {
        inset = frame - THOR_BEAM_FRAME - 1;
      }
      if (inset > THOR_BEAM_WIDTH / 2 - 1) {
        inset = THOR_BEAM_WIDTH / 2 - 1;
      }
      for (i = 1; i != THOR_BEAM_TIP_ROWS + 1; i++) {
        if (inset + i <= THOR_BEAM_WIDTH / 2 - 1) {
          blit[0](canvas, &ramp[inset + i], inset + THOR_BEAM_X + i,
                  THOR_BEAM_TOP - i, THOR_BEAM_WIDTH - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        /* FAKEMATCH(dead): the width set to 0 and at once to THOR_BEAM_WIDTH.
         */
        s32 fullWidth = 0;

        fullWidth = THOR_BEAM_WIDTH;
        blit[0](canvas, ramp + inset, inset + THOR_BEAM_X, i + THOR_BEAM_TOP,
                fullWidth - inset * 2, 1);
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet, CANVAS_SIZE / 2 - THOR_LIGHT_FAN_WIDTH,
          (frame - THOR_BEAM_FRAME) * THOR_BEAM_SPEED - 12,
          THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_LIGHTEN);
      EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
          canvas, work->sheet, CANVAS_SIZE / 2,
          (frame - THOR_BEAM_FRAME) * THOR_BEAM_SPEED - 12,
          THOR_LIGHT_FAN_WIDTH, THOR_LIGHT_FAN_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  // Second part: the pillar, Thor and his bolts.
  view->busy = 0;
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  BattleEffect_SetupBlendedDisplay();
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(THOR_SPRITES, SPRITE_THOR, 1);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, Ram_MapGridCellBuffer, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_VIOLET_MOTE_SHEET,
                             work->sheet + THOR_MOTE_PICTURE, 1, 0);
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_CRYSTAL_SHEET),
                  SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_B, sprites, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  {
    FIXED thorX;
    FIXED thorY;
    s32 thorSpeedX;
    s32 thorSpeedY;
    s32 pillarX = 60;
    s32 pillarY = LIGHTNING_PILLAR_HEIGHT;

    thorX = toFIXED(80);
    thorY = toFIXED(64);
    i = 0;
    thorSpeedX = 0;
    thorSpeedY = 0;
    for (; i != THOR_SPARK_COUNT; i++) {
      THOR_PARTICLES[i].variant = 0;
    }
    // Nothing in Thor reads these 16 particles, and BattleEffect_RunImpactBurst
    // overwrites them; only their random draws change what is drawn later.
    // XXX (no evidence): what they were for; no code that reads them is left.
    for (i = 0; i != THOR_STRAY_PARTICLES; i++) {
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
      if (stormFrame >= 0 && stormFrame <= 59 && !(frame & 7)) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
      }
      if (frame == 140) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
      // The pillar of lightning, throwing sparks up from its foot.
      // DEAD: a loop over one pillar, j being 0; the ROM keeps its counter,
      // the step of start by 128 and its exit test.
      for (j = 0; j != 1; j++) {
        s32 start = j * THOR_PILLAR_INTERVAL + THOR_PILLAR_FRAME;
        if (frame >= start && frame < j * THOR_PILLAR_INTERVAL + 17) {
          if (frame >= j * THOR_PILLAR_INTERVAL + 9 &&
              frame < j * THOR_PILLAR_INTERVAL + 12) {
            blit[0](canvas, work->sheet, 36, 0, LIGHTNING_PILLAR_WIDTH,
                    pillarY);
          }
          if (frame >= j * THOR_PILLAR_INTERVAL + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + PICTURE_BYTES(LIGHTNING_PILLAR), 36,
                    0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
          }
          if (frame == start + 2) {
            THOR_LAUNCH_SPARKS(
                spark, i, INTtoFIXED(pillarX), INTtoFIXED(pillarY),
                (Random16() & 0x7fff) - DEGtoANG(90), THOR_PILLAR_SPARKS);
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
      if (stormFrame >= 0 && stormFrame <= 63) {
        s32 moteX;
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != THOR_FLASHES; i++) {
          if (frame == i * 8 + THOR_BOLTS_FRAME) {
            Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
          }
        }
        for (i = 0; i != THOR_BOLTS; i++) {
          if (frame >= i * 2 + THOR_BOLTS_FRAME) {
            if (frame < i * 2 + THOR_BOLTS_FRAME + 2) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                                  COMPOSITE_LIGHTEN);
              EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
                  canvas, work->sheet + i % 3 * PICTURE_BYTES(THOR_BOLT), boltX,
                  boltY, THOR_BOLT_WIDTH, THOR_BOLT_HEIGHT);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
            if (frame == i * 2 + THOR_BOLTS_FRAME) {
              THOR_LAUNCH_SPARKS(spark, j, toFIXED(16), toFIXED(80),
                                 Random16() & TRIG_ANGLE_MASK,
                                 THOR_BOLT_SPARKS);
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
          moteX = FIXEDtoINT(Trig_Sin(angle) * radius);
          mirrorX = moteX + 72;
          moteY = 32 - FIXEDtoINT(Trig_Cos(angle) * radius);
        }
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
        EFFECT_DRAW_PICTURE(EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED], canvas,
                            work->sheet, THOR_MOTE_PICTURE, moteX + 60,
                            moteY - 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_LIGHTEN);
        EFFECT_DRAW_PICTURE(EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED], canvas,
                            work->sheet, THOR_MOTE_PICTURE, mirrorX,
                            moteY - 24);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                            COMPOSITE_LIGHTEN);
        EFFECT_DRAW_PICTURE(EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED], canvas,
                            work->sheet, THOR_MOTE_PICTURE, moteX + 60, moteY);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                                COMPOSITE_FLIP_Y,
                            COMPOSITE_LIGHTEN);
        EFFECT_DRAW_PICTURE(EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED], canvas,
                            work->sheet, THOR_MOTE_PICTURE, mirrorX, moteY);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        // Sparks fly off the bolts.
        j = 0;
        for (i = 0; i != THOR_SPARK_COUNT; i++) {
          spark = &THOR_PARTICLES[i];
          if (spark->variant == 0) {
            s32 speed = (Random16() & 63) + 64;
            s32 angle = Random16() & TRIG_ANGLE_MASK;
            spark->x = INTtoFIXED(mirrorX);
            spark->y = INTtoFIXED(moteY);
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++j == THOR_MOTE_SPARKS) {
              break;
            }
          }
        }
      }

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X |
                              COMPOSITE_FLIP_Y,
                          COMPOSITE_LIGHTEN);
      {
        // Sparks slow and bounce off the ground, shrinking as they die.
        DrawRectangle draw = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
        struct Particle *spark;
        for (i = 0; i != THOR_SPARK_COUNT; i++) {
          spark = &THOR_PARTICLES[i];
          if (spark->variant > 0) {
            spark->variant--;
            Particle_AdvanceWithGravity2D(spark, 60, 0);
            if (spark->y > toFIXED(120)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < toFIXED(CANVAS_SIZE - 1) &&
                       spark->y >= 0) {
              s32 x = FIXEDtoINT(spark->x);
              s32 y = FIXEDtoINT(spark->y);
              s32 dotSize = spark->variant / THOR_SPARK_SHRINK_FRAMES + 1;
              EFFECT_DRAW_MOTE(draw, canvas, sprites, x, y, dotSize);
            }
          }
        }
      }
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);

      // Thor stands over the targets, then is pushed back.
      if (frame > 15) {
        if (frame == 32) {
          thorSpeedX = toFIXED(4);
          thorSpeedY = -toFIXED(1) / 4;
        }
        if (frame > 31) {
          thorX += thorSpeedX;
          thorY += thorSpeedY;
          thorSpeedX = thorSpeedX * 60 / DAMPING_SCALE;
          thorSpeedY = thorSpeedY * 60 / DAMPING_SCALE;
        }
        BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_THOR, thorX, thorY);
      }
      if (frame >= 16 && frame <= 143) {
        s32 thorScreenX;
        for (i = 0, thorScreenX = FIXEDtoINT(thorX) >> 1; i != 3; i++) {
          s32 glint = i & 3;
          s32 angle = Random16() & TRIG_ANGLE_MASK;
          s32 x = FIXEDtoINT(Trig_Sin(angle) * THOR_CRACKLE_RADIUS_X) +
                  thorScreenX - (gBattleEffect_GlintPictureWidths[glint] >> 1);
          s32 y = FIXEDtoINT(Trig_Cos(angle) * THOR_CRACKLE_RADIUS_Y) -
                  (gBattleEffect_GlintPictureHeights[glint] >> 1);
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              EFFECT_CLIP_BOUNDS |
                                  gThor_CrackleFlips[Random16() & 3],
                              COMPOSITE_LIGHTEN);
          EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED](
              canvas,
              Ram_MapGridCellBuffer + gBattleEffect_GlintPictures[glint], x,
              y + 56, gBattleEffect_GlintPictureWidths[glint],
              gBattleEffect_GlintPictureHeights[glint]);
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
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_THOR, thorX, thorY);
  }
  for (i = 0; i != THOR_SPRITES; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef THOR_PARTICLES
#undef THOR_MOTE_PICTURE
#undef THOR_DIMMED_DOTS
#undef THOR_LAUNCH_SPARKS
#undef THOR_SPRITE_PICTURES
