/// @file
/// Rising Dragon and Heartbreak.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// On every strike after the first, the target's motion goes to this percent of
/// its anchor's x, lowered from patch 1.
#if TLA_PATCH_1
#define TARGET_ANCHOR_PERCENT 200
#else
#define TARGET_ANCHOR_PERCENT 300
#endif

#define DRAGON_SEGMENTS 33
/// The corners of one dragon's strip, two to a segment.
#define DRAGON_CORNERS (DRAGON_SEGMENTS * 2)
/// The sparks each strike scatters.
#define DRAGON_SPARKS 32
/// How far round its arc a dragon's head and tail travel, DRAGON_ARC_SPEED a
/// frame.
#define DRAGON_ARC_END 950
#define DRAGON_ARC_SPEED 44
/// The strips' widths, as a fraction of their rows' widths, and the heads'
/// scale.
#define DRAGON_SCALE toFIXED(0.3169)
/// How far the second dragon's arc is turned from the first's.
#define DRAGON_SECOND_TURN 14192
/// A head's turn at the start of its arc, less 32 for each unit of the arc.
#define DRAGON_HEAD_TURN 12384
/// The dragons' buffers in the map cell buffer: the strips' corners, then
/// their transformed vertices, then the strip mesh.
#define RISING_DRAGON_VERTICES 4096
#define RISING_DRAGON_STRIP 8192

/// A strip's vertex, as Render_TransformVertices reads it.
struct StripVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

/// Where ClawSlashSheet, a dragon's head of flame, is decoded in the effect's
/// pictures.
#define RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES 16384

/// A dragon of blue flame (two at the next variant) rises from the caster and
/// arcs onto the target: a scrolling band of flame of DRAGON_SEGMENTS
/// segments, widening to its head, a dragon's head of flame. Each strikes ten
/// frames after the last, scattering DRAGON_SPARKS sparks.
///
/// Abilities: Rising Dragon (the Masamune's unleash).
void BattleEffect_RunRisingDragon(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  DrawRectangle blit[2];
  s32 frame;
  struct BattleCamera *camera;
  u8 *sprites;
  void *caster;
  s32 arcs[4];
  struct ScreenPoint source;
  struct ScreenPoint target;
  struct ScreenPoint hit;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct Particle *spark;
  s32 i;
  s32 j;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  {
    s32 endX[2] = {0, 0};
    s32 endY[2] = {0, 0};
    caster = BattleObject_GetFar(args->user)->object;
    Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL * 3);
    Actor_SelectAnimationFar(caster, POSE_LEAP);
    EffectPosition_ProjectBattler(args->user, &source);
    EffectPosition_ProjectBattler(args->targets[0], &target);
    Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
    Resource_LoadAndDecompress(
        RESOURCE_CLAW_SLASH_SHEET,
        state->sheet + RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES, 1, 1);
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                         SHEET_PALETTE_BYTES);
    REG_BG2PA = Q_8_8(0.5);
    Render_BuildQuadStrip(gMapGridCellBuffer + RISING_DRAGON_STRIP, 4, 32,
                          DRAGON_SEGMENTS - 1);

    for (frame = 0; frame != args->variant * 10 + 60; frame++) {
      EFFECT_VIEW_FROM_CAMERA(camera);
      if (frame == 0) {
        arcs[0] = 0;
        arcs[1] = 0;
        arcs[2] = 0;
        arcs[3] = 0;
        EFFECT_FREE_PARTICLES(i, state->particles,
                              ARRAY_COUNT(state->particles));
      }

      // Each dragon's strike.
      for (i = 0; i != args->variant + 1; i++) {
        if (frame != i * 10 + 28) {
          continue;
        }
        EffectPosition_ProjectBattler(args->targets[0], &hit);
        for (j = 0; j != DRAGON_SPARKS; j++) {
          s32 angle;
          s32 speed;

          spark = &state->particles[i * DRAGON_SPARKS + j];

          spark->x = INTtoFIXED(hit.x / 2);
          spark->y = INTtoFIXED(hit.y - 16);
          angle = Random16();
          speed = (Random16() & 255) + 127;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 16;
        }
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        state->shakeFrames = 8;
        if (frame == 28) {
          if (args->variant == EFFECT_VARIANT_BASIC) {
            EFFECT_SHOW_DAMAGE(SE_CLATTER);
          } else {
            Sound_PlayFar(SE_CLATTER);
          }
          BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                        toFIXED(4), toFIXED(4), 150);
        } else {
          EFFECT_SHOW_DAMAGE(SE_CLATTER);
          BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2),
                                        toFIXED(4), toFIXED(7),
                                        TARGET_ANCHOR_PERCENT);
        }
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
      }
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        spark = &state->particles[i];
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 3) + 2;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(spark->x) - size / 2,
                  FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
          Particle_AdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
          spark->variant--;
        }
      }

      if (frame > 15) {
        struct StripVertex *stripA = (struct StripVertex *)gMapGridCellBuffer;
        s32 scale = DRAGON_SCALE;
        const u8 *stripMesh = gMapGridCellBuffer + RISING_DRAGON_STRIP;

        // The arcs' heads and tails: the first dragon's head sets out at frame
        // 16 and its tail at 32, the second's at 30 and 46.
        arcs[0] = (frame - 16) * DRAGON_ARC_SPEED;
        if (arcs[0] > DRAGON_ARC_END) {
          arcs[0] = DRAGON_ARC_END;
        }
        if (frame > 31) {
          arcs[1] = (frame - 32) * DRAGON_ARC_SPEED;
          if (arcs[1] > DRAGON_ARC_END) {
            arcs[1] = DRAGON_ARC_END;
          }
        }
        if (frame > 29) {
          arcs[2] = (frame - 30) * DRAGON_ARC_SPEED;
          if (arcs[2] > DRAGON_ARC_END) {
            arcs[2] = DRAGON_ARC_END;
          }
        }
        if (frame > 45) {
          arcs[3] = (frame - 46) * DRAGON_ARC_SPEED;
          if (arcs[3] > DRAGON_ARC_END) {
            arcs[3] = DRAGON_ARC_END;
          }
        }

        // The first dragon arcs over, the second under.
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[0] - arcs[1]) + arcs[1] * DRAGON_SEGMENTS;
          s32 height;
          s32 width;
          s32 start;

          if (distance < 0) {
            distance = -distance;
          }
          height = distance / 3;
          width = j * 2 + 16;
          if (width > 32) {
            width = 32;
          }
          width = width * scale / (FIXED_ONE);
          start = -(width / 2);
          for (i = 0; i != 2; i++) {
            struct StripVertex *corner = stripA + (j * 2 + i);

            corner->x =
                (FIXEDtoINT(Trig_Cos(angle) * (distance + start + i * width))) -
                distance;
            corner->y =
                -(FIXEDtoINT(Trig_Sin(angle) * (height + start + i * width)));
            corner->z = 0;
          }
          if (j == DRAGON_SEGMENTS - 1) {
            endX[0] = (FIXEDtoINT(Trig_Cos(angle) * distance)) - distance;
            endY[0] = -(FIXEDtoINT(Trig_Sin(angle) * height));
          }
        }
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[2] - arcs[3]) + arcs[3] * DRAGON_SEGMENTS;
          s32 height;
          s32 width;
          s32 start;

          if (distance < 0) {
            distance = -distance;
          }
          height = distance * 32 / 36;
          width = j * 4;
          if (width > 24) {
            width = 24;
          }
          width = width * scale / (FIXED_ONE);
          start = -(width / 2);
          for (i = 0; i != 2; i++) {
            struct StripVertex *stripB =
                (struct StripVertex *)Ram_MapCellBuffer + DRAGON_CORNERS;
            struct StripVertex *corner = stripB + (j * 2 + i);

            corner->x =
                (FIXEDtoINT(Trig_Cos(angle) * (distance + start + i * width))) -
                distance;
            corner->y =
                FIXEDtoINT(Trig_Sin(angle) * (height + start + i * width));
            corner->z = 0;
          }
          if (j == DRAGON_SEGMENTS - 1) {
            endX[1] = (FIXEDtoINT(Trig_Cos(angle) * distance)) - distance;
            endY[1] = FIXEDtoINT(Trig_Sin(angle) * height);
          }
        }

        model = Render_AllocateModelList(1);
        vertices = gMapGridCellBuffer + RISING_DRAGON_VERTICES;
        model->texture = &texture;
        model->shade = 0;
        model->kind = MODEL_SHADED_MAX;
        model->scrollV = 0;
        model->vertices = vertices;
        for (i = 0; i != args->variant + 1; i++) {
          if (arcs[i * 2 + 1] >= arcs[i * 2]) {
            continue;
          }
          texture.logWidth = 6;
          texture.logHeight = 5;
          texture.pixels = state->sheet;
          model->mesh = stripMesh;
          model->scrollU = TEXTURE_SCROLL_MASK & ~(frame << 3);
          Matrix_Initialize();
          if (args->side == EFFECT_SIDE_ENEMIES) {
            Matrix_Translate(INTtoFIXED(source.x / 2 - 68),
                             INTtoFIXED(source.y) - toFIXED(96), 0);
          } else {
            Matrix_Translate(INTtoFIXED(source.x / 2 - 52),
                             INTtoFIXED(source.y) - toFIXED(96), 0);
          }
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_RotateZ(i * DRAGON_SECOND_TURN - TRIG_TURN / 16);
          Matrix_ScaleUniform(FIXED_ONE * 4);
          Render_TransformVertices(stripA + i * DRAGON_CORNERS, vertices,
                                   DRAGON_CORNERS);
          Render_DrawModelList(model);
          if (frame < i * 16 + 40) {
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            model->mesh = gBattleEffect_SquareMesh;
            texture.pixels =
                state->sheet + RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES;
            Matrix_Translate(INTtoFIXED(endX[i]), INTtoFIXED(endY[i]), 0);
            Matrix_RotateZ(DRAGON_HEAD_TURN - (arcs[i * 2] << 5));
            Matrix_ScaleUniform(DRAGON_SCALE);
            Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
      BattleEffect_CountDownBattlerTints();
      BattleCamera_Shake(2, 2);
      EFFECT_PRESENT_CANVAS(state);
    }
    Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}

#define FIGURE_FRAMES 108
/// The burst at its hand flashes the screen.
#define FIGURE_SPARK_BURST_FRAME 30
#define FIGURE_HIT_FRAME 80 ///< the blow, which starts the result phase

/// The parts of the figure in BattleEffect_HeartbreakSheet, unpacked into the
/// map cells.
#define FIGURE_BODY 0
#define FIGURE_BODY_WIDTH 96
#define FIGURE_BODY_HEIGHT 83
#define FIGURE_ARM_CLENCHED 7968
#define FIGURE_ARM_CLENCHED_WIDTH 42
#define FIGURE_ARM_CLENCHED_HEIGHT 35
#define FIGURE_ARM_REACHING 9438
#define FIGURE_ARM_REACHING_WIDTH 48
#define FIGURE_ARM_REACHING_HEIGHT 51
/// A burst of sparks growing, four pictures (gHeartbreak_SwingPictures).
#define FIGURE_SPARK_BURST 11886
/// A heart, then seven pictures of green flame burning it away
/// (gHeartbreak_FistPictures); the first is also the heart held up.
#define FIGURE_HEART 20125
#define FIGURE_HEART_WIDTH 32
#define FIGURE_HEART_HEIGHT 29
/// The figure sways on a circle of this radius, a 256th of a turn a frame.
#define FIGURE_SWAY 8

/// A great cloaked figure with a horned skull for a head sweeps down on the
/// target from above the far side, swaying and fading in through the blend,
/// its clawed hand reaching out. From frame 28 a burst of sparks grows at its
/// hand and the screen flashes white; from frame 40 a heart is held up, from
/// frame 48 the hand closes into a fist, and from frame 62 green flame burns
/// the heart away. At frame 80 the blow lands and the result phase starts;
/// from frame 92 the figure fades out and rises away, and the battle
/// background is reloaded at the end. On the far side the whole figure is
/// drawn mirrored.
///
/// Abilities: Heartbreak (the Thanatos Mace's unleash).
void BattleEffect_RunHeartbreak(struct BattleEffectParams *args) {
  struct HeapSlots *heap;
  struct BattleEffectState *state;
  u8 *canvas;
  u8 *parts;
  DrawRectangle draw[2];
  struct ScreenPoint target;
  s32 frame;
  s32 x;
  s32 y;

  heap = Ram_HeapSlots;
  state = (struct BattleEffectState *)heap->battleEffect;
  canvas = heap->battleCanvas;
  parts = Ram_MapCellBuffer;
  BattleEffect_StartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress(RESOURCE_HEARTBREAK_SHEET, parts, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)heap->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)heap->ramCodePaired;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  EffectPosition_ProjectScaledBattler(args->targets[0], &target);

  for (frame = 0; frame != FIGURE_FRAMES; frame++) {
    // It starts 100 pixels above the canvas and to the target's far side,
    // and comes down at it for 32 frames.
    if (frame == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        state->particles[0].x = INTtoFIXED(target.x + 40);
      else
        state->particles[0].x = INTtoFIXED(target.x - 168);
      state->particles[0].y = -toFIXED(100);
      if (args->side == EFFECT_SIDE_ENEMIES)
        state->particles[0].velocityX = -toFIXED(6);
      else
        state->particles[0].velocityX = toFIXED(6);
      state->particles[0].velocityY = toFIXED(8);
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
    }
    if (frame < 32)
      Particle_AdvanceWithGravity2D(&state->particles[0], 60, 0);

    // In over 32 frames, out over the last 16, rising away.
    if (frame <= 32) {
      s32 level = frame / 2;

      REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level / 2) << 8) | level;
    }
    if (frame > 91) {
      s32 level = FIGURE_FRAMES - frame;

      REG_BLDALPHA = ((BLEND_WEIGHT_MAX - level / 2) << 8) | level;
      Particle_AdvanceWithGravity2D(&state->particles[0], 60, -toFIXED(1) / 2);
    }

    // The sway, and the canvas moved with the figure.
    x = FIXED_INTEGER_PART(state->particles[0].x) +
        (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 256)) * FIGURE_SWAY));
    y = FIXED_INTEGER_PART(state->particles[0].y) +
        (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 256)) * FIGURE_SWAY));
    REG_BG2X = -x << 8;

    // The body, and the arm reaching until frame 48, then clenched.
    if (args->side == EFFECT_SIDE_ENEMIES) {
      draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 25, y + 2,
                       FIGURE_BODY_WIDTH, FIGURE_BODY_HEIGHT);
      if ((u32)frame < 48)
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_REACHING, 31,
                         y + 22, FIGURE_ARM_REACHING_WIDTH,
                         FIGURE_ARM_REACHING_HEIGHT);
      else
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_CLENCHED, 64,
                         y + 16, FIGURE_ARM_CLENCHED_WIDTH,
                         FIGURE_ARM_CLENCHED_HEIGHT);
    } else {
      draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 7, y + 2,
                       FIGURE_BODY_WIDTH, FIGURE_BODY_HEIGHT);
      if ((u32)frame < 48)
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_REACHING, 49,
                         y + 22, FIGURE_ARM_REACHING_WIDTH,
                         FIGURE_ARM_REACHING_HEIGHT);
      else
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_CLENCHED, 22,
                         y + 16, FIGURE_ARM_CLENCHED_WIDTH,
                         FIGURE_ARM_CLENCHED_HEIGHT);
    }

    // The burst of sparks, three frames a picture from frame 28.
    if (frame >= 28 && frame < 40) {
      s32 cel = (frame - 28) / 3;

      if (args->side == EFFECT_SIDE_ENEMIES) {
        draw[args->side](canvas,
                         Ram_MapCellBuffer + FIGURE_SPARK_BURST +
                             gHeartbreak_SwingPictures[cel],
                         46 - gHeartbreak_SwingWidths[cel] / 2,
                         y - gHeartbreak_SwingHeights[cel] / 2 + 61,
                         gHeartbreak_SwingWidths[cel],
                         gHeartbreak_SwingHeights[cel]);
      } else {
        draw[args->side](canvas,
                         Ram_MapCellBuffer + FIGURE_SPARK_BURST +
                             gHeartbreak_SwingPictures[cel],
                         gHeartbreak_SwingWidths[cel] / 2 -
                             gHeartbreak_SwingWidths[cel] + 82,
                         y - gHeartbreak_SwingHeights[cel] / 2 + 61,
                         gHeartbreak_SwingWidths[cel],
                         gHeartbreak_SwingHeights[cel]);
      }
    }

    // The heart: held up until frame 47, then in the closed fist, where from
    // frame 62 it burns away, a picture every six frames.
    if (frame > 39) {
      if (frame <= 47) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_HEART, 30, y + 46,
                           FIGURE_HEART_WIDTH, FIGURE_HEART_HEIGHT);
        else
          draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_HEART, 66, y + 46,
                           FIGURE_HEART_WIDTH, FIGURE_HEART_HEIGHT);
      } else {
        s32 cel = (frame - 56) / 6;

        if (cel < 0)
          cel = 0;
        if (cel <= 7) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            draw[args->side](canvas,
                             Ram_MapCellBuffer + FIGURE_HEART +
                                 gHeartbreak_FistPictures[cel],
                             63, y + gHeartbreak_FistY[cel] - 8,
                             FIGURE_HEART_WIDTH, gHeartbreak_FistHeights[cel]);
          else
            draw[args->side](canvas,
                             Ram_MapCellBuffer + FIGURE_HEART +
                                 gHeartbreak_FistPictures[cel],
                             33, y + gHeartbreak_FistY[cel] - 8,
                             FIGURE_HEART_WIDTH, gHeartbreak_FistHeights[cel]);
        }
      }
    }

    if (frame == FIGURE_HIT_FRAME) {
      state->shakeFrames = 6;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }
    if (frame == FIGURE_SPARK_BURST_FRAME) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      state->shakeFrames = 3;
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           Ram_HeapSlots->battleState->background,
                           BACKGROUND_DARKEN_FRAMES);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}
#undef TARGET_ANCHOR_PERCENT
#undef DRAGON_SEGMENTS
#undef DRAGON_CORNERS
#undef DRAGON_SPARKS
#undef DRAGON_ARC_END
#undef DRAGON_ARC_SPEED
#undef DRAGON_SCALE
#undef DRAGON_SECOND_TURN
#undef DRAGON_HEAD_TURN
#undef RISING_DRAGON_VERTICES
#undef RISING_DRAGON_STRIP
#undef RISING_DRAGON_FLAME_DRAGON_HEAD_PICTURES
#undef FIGURE_FRAMES
#undef FIGURE_SPARK_BURST_FRAME
#undef FIGURE_HIT_FRAME
#undef FIGURE_BODY
#undef FIGURE_BODY_WIDTH
#undef FIGURE_BODY_HEIGHT
#undef FIGURE_ARM_CLENCHED
#undef FIGURE_ARM_CLENCHED_WIDTH
#undef FIGURE_ARM_CLENCHED_HEIGHT
#undef FIGURE_ARM_REACHING
#undef FIGURE_ARM_REACHING_WIDTH
#undef FIGURE_ARM_REACHING_HEIGHT
#undef FIGURE_SPARK_BURST
#undef FIGURE_HEART
#undef FIGURE_HEART_WIDTH
#undef FIGURE_HEART_HEIGHT
#undef FIGURE_SWAY
