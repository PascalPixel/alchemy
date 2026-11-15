/// @file
/// Plasma, and the spark groups.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define SPARK_MAX 1024
#define PILLAR_BASE_Y 104

#define PLASMA_SPACING 8 ///< frames between pillars
/// The second picture in the sheet, after the first 48x112 one.
#define PLASMA_SECOND_PICTURE PICTURE_BYTES(LIGHTNING_PILLAR)

/// Pillars of lightning strike one after another, each with a white flash,
/// throwing sparks up from the ground and knocking the targets back.
///
/// Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
/// Thunderhead.
void BattleEffect_RunPlasma(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *variant;
  struct Particle *spark;
  struct Particle *sparks;

  // Set up: the canvas layer, an additive blitter and a mirrored max one,
  // the pillar sheet and the particle sprites, and a violet palette for all
  // but the strongest variant.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  if (args->variant != EFFECT_VARIANT_STRONGEST) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }

  // Every spark starts at rest.
  variant = &((struct Particle *)Ram_MapCellBuffer)->variant;
  for (k = 0; k != SPARK_MAX; k++) {
    *variant = 0;
    variant += 7;
  }
  sparks = (struct Particle *)Ram_MapCellBuffer;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  count = gPlasma_Counts[args->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    // The strongest variant pans the camera over the first 64 frames.
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame <= 63) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (args->side == EFFECT_SIDE_PARTY)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // Pillar i strikes at frame 8i: a flash, then its first cel for one
    // frame and its second for two.
    for (i = 0; i != count; i++) {
      start = i * PLASMA_SPACING;
      if (frame == start) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet, gPlasma_Columns[i + args->side * 7] - 24,
                  0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + PLASMA_SECOND_PICTURE,
                  gPlasma_Columns[i + args->side * 7] - 24, 0,
                  LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);

        // Throw sparks up from the pillar's base, within 90 degrees of
        // straight up, and shake the camera.
        if (frame == start + 2) {
          s32 thrown = 0;

          for (k = 0; k != SPARK_MAX; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - DEGtoANG(90);

              spark->x = INTtoFIXED(gPlasma_Columns[i + args->side * 7]);
              spark->y = toFIXED(PILLAR_BASE_Y);
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              thrown++;
              if (thrown == gPlasma_Sparks[args->variant * 2])
                break;
            }
          }
          work->shakeFrames = gPlasma_Sparks[args->variant * 2 + 1];
        }
      }

      // Four frames after each strike the targets recoil and flash.
      if (frame == start + 4) {
        for (k = 0; k != args->targetCount; k++) {
          BattleMotion_ApplyKnockbackFar(args->targets[k], KNOCKBACK_HOP);
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 8);
        }
      }
    }

    // The sparks fall and bounce at half speed off the ground, drawn
    // smaller as they die out.
    for (k = 0; k != SPARK_MAX; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, toFIXED(1) / 16);
        if (spark->y > toFIXED(PILLAR_BASE_Y)) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = FIXEDtoINT(spark->x);
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = FIXEDtoINT(spark->y);
            size = spark->variant / 8 + 1;
            draw[k & 1](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    BattleCamera_Shake(8, 16);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_FINISH_CANVAS();
}

/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb
///.
void BattleEffect_RunNova(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_NOVA);
}

/// Abilities: Dynamite, Angry Mine.
void BattleEffect_RunDynamite(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}

#define FLASH_POINTS 16 ///< flash points kept for each group
#define FLASH_DRAWN 12  ///< of which the first 12 are drawn
#define FLASH_FRAMES 18 ///< six cels of three frames
#define GROUND_Y toFIXED(108)

#define SHAPE(n) gPlasma_Shapes[args->variant][n]
#define SPARK_COUNT SHAPE(0)
#define GROUP_COUNT SHAPE(1)
#define GROUP_X(i) SHAPE((i) + 2)

#define SPARK_GROUPS_SPACING 8 ///< frames between bursts

/// One to three groups of sparks burst one after another, eight frames apart:
/// each opens with a big flash, a ring of small flashes and a spray of sparks
/// that fall (or, from the caster, rise) and bounce; six frames after each
/// burst the targets flash and recoil.
void BattleEffect_RunSparkGroups(struct BattleEffectParams *args, s32 kind) {
  struct ScreenPoint pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct Particle *sparks;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  // The burst's origin on the canvas.
  if (kind == SPARK_GROUPS_NOVA) {
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
    originX = 60;
    originY = 48;
  } else if (kind == SPARK_GROUPS_DYNAMITE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    work->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(args->user, &pos);
    originX = pos.x / 2;
    originY = pos.y + 48;
  }

  // The flash sheet, the particle sprites and the kind's palette.
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (kind == SPARK_GROUPS_DYNAMITE) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  } else if (kind == SPARK_GROUPS_RISING) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }

  sparks = gMapGridCellBuffer;

  for (i = 0; i != GROUP_COUNT; i++) {
    // The group's ring of flash points, wider and later further out.
    for (j = 0; j != FLASH_POINTS; j++) {
      struct Particle *point;
      s32 radius;
      s32 angle;

      point = &work->particles[i * FLASH_POINTS + j];
      radius = j * 2;
      angle = Random16() & TRIG_ANGLE_MASK;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }

    // Its sparks, thrown in every direction from the group's centre.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct Particle *spark;
      s32 speed;
      s32 angle;

      spark = sparks + i * SPARK_COUNT + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      if (args->side == EFFECT_SIDE_PARTY)
        spark->x = INTtoFIXED(originX - GROUP_X(i) + 28);
      else
        spark->x = INTtoFIXED(originX + GROUP_X(i) - 28);
      spark->y = INTtoFIXED(originY);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != GROUP_COUNT * 8 + 40; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    // The strongest variant pans the camera; the next one wipes the canvas
    // at frame 4.
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame <= 51) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw += 256;
      else
        camera->yaw -= 256;
    }
    if (args->variant == 3 && frame == 4)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));

    if (kind == SPARK_GROUPS_DYNAMITE || kind == SPARK_GROUPS_RISING) {
      if (frame == 2) {
        Sound_PlayFar(SE_CRASH);
        EFFECT_SHOW_DAMAGE(SE_CRASH);
      }
    } else {
      if (frame == 2)
        Sound_PlayFar(SE_CRASH);
      if (frame == 24)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    for (i = 0; i != GROUP_COUNT; i++) {
      start = i * SPARK_GROUPS_SPACING;
      if (frame == start)
        work->shakeFrames = 12;

      // The big flash, for two frames.
      if (frame >= start && frame < start + 2) {
        if (args->side == EFFECT_SIDE_PARTY)
          draw[0](canvas, work->sheet, originX - GROUP_X(i) + 12, originY - 32,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        else
          draw[0](canvas, work->sheet, originX + GROUP_X(i) - 44, originY - 32,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      }

      // The ring of small flashes, each counting down through six cels.
      if (frame >= start) {
        for (j = 0; j != FLASH_DRAWN; j++) {
          struct Particle *point;
          s32 x;
          s32 y;

          point = &work->particles[i * FLASH_POINTS + j];
          y = FIXED_INTEGER_PART(point->y) + originY;
          if (args->side == EFFECT_SIDE_PARTY)
            x = FIXED_INTEGER_PART(point->x) + originX - GROUP_X(i) + 28;
          else
            x = FIXED_INTEGER_PART(point->x) + originX + GROUP_X(i) - 28;
          if (point->variant >= 0 && point->variant < FLASH_FRAMES)
            draw[0](canvas,
                    work->sheet +
                        (gPlasma_FlashPictures[point->variant / 3] << 11),
                    x - 16, y - 32, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }

      // From the sixth frame the sparks fly, bounce off the ground at half
      // speed, and shrink as they die out.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == SPARK_GROUPS_RISING)
          gravity = -toFIXED(1) / 16;
        else
          gravity = toFIXED(1) / 16;
        for (j = 0; j != SPARK_COUNT; j++) {
          struct Particle *spark = sparks + i * SPARK_COUNT + j;
          if (spark->variant > 0) {
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
            if (spark->y > GROUND_Y) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < toFIXED(127) &&
                       spark->y >= 0) {
              s16 y = FIXEDtoINT(spark->y);
              s16 x = FIXEDtoINT(spark->x);
              s32 size = spark->variant / 5 + 1;

              draw[j & 1](canvas,
                          graphics + gBattleEffect_MotePictures[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      // The targets flash and recoil six frames after the burst.
      for (j = 0; j != args->targetCount; j++) {
        if (frame == start + 6) {
          EFFECT_STRIKE_TARGET(args, j, 10, KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_FINISH_CANVAS();
}
#undef SPARK_COUNT
