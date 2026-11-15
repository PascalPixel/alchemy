/// @file
/// Rising Dragon and Heartbreak.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

// On every strike after the first, the target's motion goes to this percent of
// its anchor's x, lowered from patch 1.
#if TLA_PATCH_1
#define TARGET_ANCHOR_PERCENT 200
#else
#define TARGET_ANCHOR_PERCENT 300
#endif

#define DRAGON_SEGMENTS 33

/// A strip's vertex, as Render_TransformVertices reads it.
struct StripVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

/// Where ClawSlashSheet is decoded in the effect's pictures.
#define RISING_DRAGON_CLAW_SLASH_PICTURES 0x4000

/// One dragon (two at a higher level: effect->variant + 1) of smoke rises
/// from the caster and arcs onto the target, each a scrolling textured strip
/// of 33 segments with a clawed head; each strikes ten frames after the
/// last, scattering 32 sparks.
///
/// Abilities: Echo, Rising Dragon.
void BattleEffect_RunRisingDragon(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
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
  struct Particle *mote;
  s32 i;
  s32 j;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  {
    s32 endX[2] = {0, 0};
    s32 endY[2] = {0, 0};
    caster = BattleObject_GetFar(args->user)->object;
    Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL * 3);
    Actor_SelectAnimationFar(caster, 2);
    EffectPosition_ProjectBattler(args->user, &source);
    EffectPosition_ProjectBattler(args->targets[0], &target);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                               0);
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + RISING_DRAGON_CLAW_SLASH_PICTURES,
                               1, 1);
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
        SHEET_PALETTE_BYTES);
    REG_BG2PA = Q_8_8(0.5);
    Render_BuildQuadStrip(gMapGridCellBuffer + 0x2000, 4, 32, 32);

    for (frame = 0; frame != args->variant * 10 + 60; frame++) {
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      if (frame == 0) {
        arcs[0] = 0;
        arcs[1] = 0;
        arcs[2] = 0;
        arcs[3] = 0;
        for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
          work->particles[i].variant = -1;
        }
      }

      // Each dragon's strike.
      for (i = 0; i != args->variant + 1; i++) {
        if (frame != i * 10 + 28) {
          continue;
        }
        EffectPosition_ProjectBattler(args->targets[0], &hit);
        for (j = 0; j != 32; j++) {
          s32 angle;
          s32 speed = 255;

          spark = &work->particles[i * 32 + j];

          spark->x = INTtoFIXED(hit.x / 2);
          spark->y = INTtoFIXED(hit.y - 16);
          angle = Random16();
          speed &= Random16();
          spark->velocityX = (Trig_Sin(angle) * (speed += 127)) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 16;
        }
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        work->shakeFrames = 8;
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
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        mote = &work->particles[i];
        if (mote->variant >= 0) {
          s32 size = (mote->variant >> 3) + 2;

          blit[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                  FIXED_INTEGER_PART(mote->x) - size / 2,
                  FIXED_INTEGER_PART(mote->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(mote, 60, toFIXED(1) / 16);
          mote->variant--;
        }
      }

      if (frame > 15) {
        struct StripVertex *stripA = (struct StripVertex *)gMapGridCellBuffer;
        s32 scale = 20768;
        const u8 *stripMesh = gMapGridCellBuffer + 0x2000;

        // The arcs' heads and tails, 44 units a frame.
        arcs[0] = frame * 44 - 704;
        if (arcs[0] > 950) {
          arcs[0] = 950;
        }
        if (frame > 31) {
          arcs[1] = frame * 44 - 1408;
          if (arcs[1] > 950) {
            arcs[1] = 950;
          }
        }
        if (frame > 29) {
          arcs[2] = frame * 44 - 1320;
          if (arcs[2] > 950) {
            arcs[2] = 950;
          }
        }
        if (frame > 45) {
          arcs[3] = frame * 44 - 2024;
          if (arcs[3] > 950) {
            arcs[3] = 950;
          }
        }

        // The first dragon arcs over, the second under.
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[0] - arcs[1]) + arcs[1] * 33;
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
          if (j == 32) {
            endX[0] = (FIXEDtoINT(Trig_Cos(angle) * distance)) - distance;
            endY[0] = -(FIXEDtoINT(Trig_Sin(angle) * height));
          }
        }
        for (j = 0; j != DRAGON_SEGMENTS; j++) {
          s32 distance = source.x / 4 - target.x / 4;
          s32 angle = j * (arcs[2] - arcs[3]) + arcs[3] * 33;
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
                (struct StripVertex *)Ram_MapCellBuffer + 66;
            struct StripVertex *corner = stripB + (j * 2 + i);

            corner->x =
                (FIXEDtoINT(Trig_Cos(angle) * (distance + start + i * width))) -
                distance;
            corner->y =
                FIXEDtoINT(Trig_Sin(angle) * (height + start + i * width));
            corner->z = 0;
          }
          if (j == 32) {
            endX[1] = (FIXEDtoINT(Trig_Cos(angle) * distance)) - distance;
            endY[1] = FIXEDtoINT(Trig_Sin(angle) * height);
          }
        }

        model = Render_AllocateModelList(1);
        vertices = gMapGridCellBuffer + 0x1000;
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
          texture.pixels = work->sheet;
          model->mesh = stripMesh;
          model->scrollU = 127 & ~(frame << 3);
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
          Matrix_RotateZ(i * 14192 - TRIG_TURN / 16);
          Matrix_ScaleUniform(FIXED_ONE * 4);
          Render_TransformVertices(stripA + i * 66, vertices, 66);
          Render_DrawModelList(model);
          if (frame < i * 16 + 40) {
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            model->mesh = gBattleEffect_SquareMesh;
            texture.pixels = work->sheet + RISING_DRAGON_CLAW_SLASH_PICTURES;
            Matrix_Translate(INTtoFIXED(endX[i]), INTtoFIXED(endY[i]), 0);
            Matrix_RotateZ(0x3060 - (arcs[i * 2] << 5));
            Matrix_ScaleUniform(20768);
            Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
      BattleEffect_CountDownBattlerTints();
      BattleCamera_Shake(2, 2);
      EFFECT_PRESENT_CANVAS(work);
    }
    Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}

#define FIGURE_FRAMES 108
#define FIGURE_SWING_FRAME 30 ///< the first blow
#define FIGURE_HIT_FRAME 80   ///< the second, which starts the result phase

// The parts of the figure in BattleEffect_HeartbreakSheet, unpacked into the
// map cells.
#define FIGURE_BODY 0             ///< 96x83
#define FIGURE_ARM_LOWERED 0x1f20 ///< 42x35
#define FIGURE_ARM_RAISED 0x24de  ///< 48x51
#define FIGURE_SWING 0x2e6e       ///< four pictures, gHeartbreak_SwingPictures
#define FIGURE_FIST                                                            \
  0x4e9d // 32x29, then eight pictures, gHeartbreak_FistPictures

/// A great figure strikes. It sweeps down on the target from above the far
/// side, swaying on a small circle and fading in through the blend; its arm
/// is raised, and at frame 30 it swings down through four cels and the
/// screen flashes white; from frame 40 its fist falls through eight cels,
/// and at frame 80 the blow lands and the result phase starts. From frame
/// 92 it fades out and rises away; the battle background is reloaded at the
/// end. On the far side the whole figure is drawn mirrored.
///
/// Abilities: Heartbreak.
void BattleEffect_RunHeartbreak(struct BattleEffectParams *args) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 *parts;
  DrawRectangle draw[2];
  struct ScreenPoint target;
  s32 frame;
  s32 x;
  s32 y;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleEffect;
  canvas = heap->battleCanvas;
  parts = Ram_MapCellBuffer;
  BattleEffect_StartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_HeartbreakSheet, parts, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)heap->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = ((DrawRectangle *)heap)[47];
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EffectPosition_ProjectScaledBattler(args->targets[0], &target);

  for (frame = 0; frame != FIGURE_FRAMES; frame++) {
    // It starts 100 pixels above the canvas and to the target's far side,
    // and comes down at it for 32 frames.
    if (frame == 0) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        work->particles[0].x = INTtoFIXED(target.x + 40);
      else
        work->particles[0].x = INTtoFIXED(target.x - 168);
      work->particles[0].y = -toFIXED(100);
      if (args->side == EFFECT_SIDE_ENEMIES)
        work->particles[0].velocityX = -toFIXED(6);
      else
        work->particles[0].velocityX = toFIXED(6);
      work->particles[0].velocityY = toFIXED(8);
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
    }
    if (frame < 32)
      BattleEffectKernels_IntegrateVector2(&work->particles[0], 60, 0);

    // In over 32 frames, out over the last 16, rising away.
    if (frame <= 32) {
      s32 level = frame / 2;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
    }
    if (frame > 91) {
      s32 level = FIGURE_FRAMES - frame;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
      BattleEffectKernels_IntegrateVector2(&work->particles[0], 60,
                                           -toFIXED(1) / 2);
    }

    // The sway, and the canvas moved with the figure.
    x = FIXED_INTEGER_PART(work->particles[0].x) +
        (FIXEDtoINT(Trig_Sin(frame << 8) << 3));
    y = FIXED_INTEGER_PART(work->particles[0].y) +
        (FIXEDtoINT(Trig_Cos(frame << 8) << 3));
    REG_BG2X = -x << 8;

    // The body, and the arm raised until frame 48.
    if (args->side == EFFECT_SIDE_ENEMIES) {
      draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 25, y + 2, 96,
                       83);
      if ((u32)frame < 48)
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 31,
                         y + 22, 48, 51);
      else
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 64,
                         y + 16, 42, 35);
    } else {
      draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 7, y + 2, 96,
                       83);
      if ((u32)frame < 48)
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 49,
                         y + 22, 48, 51);
      else
        draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 22,
                         y + 16, 42, 35);
    }

    // The swing, three frames a cel from frame 28.
    if (frame >= 28 && frame < 40) {
      s32 cel = (frame - 28) / 3;

      if (args->side == EFFECT_SIDE_ENEMIES) {
        draw[args->side](
            canvas,
            Ram_MapCellBuffer + FIGURE_SWING + gHeartbreak_SwingPictures[cel],
            46 - gHeartbreak_SwingWidths[cel] / 2,
            y - gHeartbreak_SwingHeights[cel] / 2 + 61,
            gHeartbreak_SwingWidths[cel], gHeartbreak_SwingHeights[cel]);
      } else {
        draw[args->side](
            canvas,
            Ram_MapCellBuffer + FIGURE_SWING + gHeartbreak_SwingPictures[cel],
            gHeartbreak_SwingWidths[cel] / 2 - gHeartbreak_SwingWidths[cel] +
                82,
            y - gHeartbreak_SwingHeights[cel] / 2 + 61,
            gHeartbreak_SwingWidths[cel], gHeartbreak_SwingHeights[cel]);
      }
    }

    // The fist: raised to frame 47, then falling six frames a cel from 56.
    if (frame > 39) {
      if (frame <= 47) {
        if (args->side == EFFECT_SIDE_ENEMIES)
          draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 30, y + 46,
                           32, 29);
        else
          draw[args->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 66, y + 46,
                           32, 29);
      } else {
        s32 cel = (frame - 56) / 6;

        if (cel < 0)
          cel = 0;
        if (cel <= 7) {
          if (args->side == EFFECT_SIDE_ENEMIES)
            draw[args->side](canvas,
                             Ram_MapCellBuffer + FIGURE_FIST +
                                 gHeartbreak_FistPictures[cel],
                             63, y + gHeartbreak_FistY[cel] - 8, 32,
                             gHeartbreak_FistHeights[cel]);
          else
            draw[args->side](canvas,
                             Ram_MapCellBuffer + FIGURE_FIST +
                                 gHeartbreak_FistPictures[cel],
                             33, y + gHeartbreak_FistY[cel] - 8, 32,
                             gHeartbreak_FistHeights[cel]);
        }
      }
    }

    if (frame == FIGURE_HIT_FRAME) {
      work->shakeFrames = 6;
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }
    if (frame == FIGURE_SWING_FRAME) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      work->shakeFrames = 3;
      Sound_PlayFar(SE_POWER_UP);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 16);
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670),
                           24);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}
#undef TARGET_ANCHOR_PERCENT
#undef BLITTER
#undef DRAGON_SEGMENTS
#undef FIGURE_FRAMES
#undef FIGURE_SWING_FRAME
#undef FIGURE_HIT_FRAME
#undef FIGURE_BODY
#undef FIGURE_ARM_LOWERED
#undef FIGURE_ARM_RAISED
#undef FIGURE_SWING
#undef FIGURE_FIST
