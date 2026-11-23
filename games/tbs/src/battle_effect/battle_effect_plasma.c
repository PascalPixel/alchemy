/// @file
/// Plasma, and the spark groups of Blast and Dynamite.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// The lightning pillars throw their sparks into one pool in PARTICLES, as
// Volcano's fire pillars do, and the sparks bounce off the ground.

#define PLASMA_SPARK_COUNT                                                     \
  PARTICLE_POOL_COUNT ///< sparks in PARTICLES, shared by every pillar

#define PLASMA_SPACING 8 ///< frames between pillars
/// The second picture in the sheet, after the first 48x112 one.
#define PLASMA_SECOND_PICTURE PICTURE_BYTES(LIGHTNING_PILLAR)

/// gPlasma_Columns has seven pillars for each side.
#define PLASMA_COLUMN_X(side, pillar) gPlasma_Columns[(side)][(pillar)]

/// Pillars of lightning strike one after another, each with a white flash,
/// throwing sparks up from the ground and knocking the targets back.
///
/// Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
/// Thunderstorm.
void BattleEffect_RunPlasma(struct BattleEffectParams *args) {
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *motes;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  struct Particle *spark;
  struct Particle *sparks;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);
  if (work->args->variant != EFFECT_VARIANT_STRONGEST) {
    void *palette = Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_B);
    Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }

  for (k = 0; k != PLASMA_SPARK_COUNT; k++)
    PARTICLES[k].variant = 0;
  sparks = PARTICLES;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  count = gPlasma_Counts[work->args->variant];

  for (frame = 0; frame != count * 8 - count + 48; frame++) {
    if (work->args->variant == EFFECT_VARIANT_STRONGEST && frame <= 63) {
      struct BattleCamera *camera = gBattleCamera;
      s32 speed;

      // A 256th of a turn a frame, slowing from frame 56.
      if (frame <= 55)
        speed = TRIG_TURN / 256;
      else
        speed = TRIG_TURN / 256 - (frame - 56) * 8;
      if (work->args->side == EFFECT_SIDE_PARTY)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // Each pillar strikes with a flash.
    for (i = 0; i != count; i++) {
      start = i * PLASMA_SPACING;
      if (frame == start) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet,
                  PLASMA_COLUMN_X(work->args->side, i) -
                      LIGHTNING_PILLAR_WIDTH / 2,
                  0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + PLASMA_SECOND_PICTURE,
                  PLASMA_COLUMN_X(work->args->side, i) -
                      LIGHTNING_PILLAR_WIDTH / 2,
                  0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);

        // Sparks fly up from the pillar's foot, and the screen shakes.
        if (frame == start + 2) {
          s32 spawned = 0;

          for (k = 0; k != PLASMA_SPARK_COUNT; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - DEGtoANG(90);

              spark->x = INTtoFIXED(PLASMA_COLUMN_X(work->args->side, i));
              spark->y = toFIXED(104);
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              spawned++;
              if (spawned == gPlasma_Sparks[work->args->variant][PLASMA_SPARKS])
                break;
            }
          }
          work->shakeFrames =
              gPlasma_Sparks[work->args->variant][PLASMA_SHAKE_FRAMES];
        }
      }
      // The targets recoil.
      if (frame == start + 4) {
        EFFECT_FOR_EACH_TARGET(k, work->args) {
          BattleMotion_ApplyKnockbackFar(work->args->targets[k], KNOCKBACK_HOP);
          BattleEffect_TintBattler(work->args->targets[k], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, k, 8);
        }
      }
    }

    // Sparks fall, bounce and shrink as they die.
    for (k = 0; k != PLASMA_SPARK_COUNT; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
        if (spark->y > toFIXED(104)) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = FIXEDtoINT(spark->x);
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = FIXEDtoINT(spark->y);
            size = spark->variant / 8 + 1;
            EFFECT_DRAW_MOTE(draw[k & 1], canvas, motes, x, y, size);
          }
        }
      }
    }
    BattleCamera_Shake(8, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// Bursts of light go off among the targets, each ringed by fading flashes
/// and throwing sparks.
///
/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb.
void BattleEffect_RunNova(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_NOVA);
}
/// Orange bursts go off in front of the battle.
///
/// Abilities: Dynamite.
void BattleEffect_RunDynamite(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}
#define SPARK_GROUPS_FLASHES 16     ///< flashes kept per burst
#define SPARK_GROUPS_SPACING 8      ///< frames between bursts
#define SPARK_GROUPS_RING 12        ///< of which drawn as the ring
#define SPARK_GROUPS_FLASH_DRAWN 18 ///< a flash is drawn for its last 18 frames
/// A ring flash's frames, a frame more for every second flash out.
#define SPARK_GROUPS_FLASH_LIFE 25

/// One to three bursts go off: each flashes, rings itself with smaller flashes
/// that fade, and throws out sparks that bounce on the ground, knocking the
/// targets back.
void BattleEffect_RunSparkGroups(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *motes;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct Particle *point;
  struct Particle *spark;
  struct Particle *sparks;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  if (mode == SPARK_GROUPS_NOVA) {
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
    originX = 60;
    originY = 48;
  } else if (mode == SPARK_GROUPS_DYNAMITE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
  } else {
    BattleEffect_StartCanvasLayer(0);
    Projection_ScaledUnit(work->args->user, &screenPosition);
    originX = screenPosition.x / 2;
    originY = screenPosition.y + 48;
  }
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);
  if (mode == SPARK_GROUPS_DYNAMITE) {
    Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  } else if (mode == SPARK_GROUPS_RISING) {
    Iwram_CopyWords(BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                    SHEET_PALETTE_BYTES);
  }
  sparks = PARTICLES;

  for (i = 0; i != gPlasma_Shapes[work->args->variant][SPARK_GROUPS_BURSTS];
       i++) {
    for (j = 0; j != SPARK_GROUPS_FLASHES; j++) {
      s32 radius;
      s32 angle;

      point = &work->particles[i * SPARK_GROUPS_FLASHES + j];
      radius = j * 2;
      angle = Random16() & TRIG_ANGLE_MASK;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + SPARK_GROUPS_FLASH_LIFE;
    }
    for (j = 0; j != gPlasma_Shapes[work->args->variant][SPARK_GROUPS_SPARKS];
         j++) {
      s32 speed;
      s32 angle;

      spark =
          &sparks[i *
                  gPlasma_Shapes[work->args->variant][SPARK_GROUPS_SPARKS]] +
          j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      if (work->args->side == EFFECT_SIDE_PARTY)
        spark->x = INTtoFIXED(
            originX -
            gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] + 28);
      else
        spark->x = INTtoFIXED(
            originX +
            gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] - 28);
      spark->y = INTtoFIXED(originY);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0;
       frame != gPlasma_Shapes[work->args->variant][SPARK_GROUPS_BURSTS] *
                        SPARK_GROUPS_SPACING +
                    56;
       frame++) {
    struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);

    // The strongest variant turns the camera.
    if (work->args->variant == EFFECT_VARIANT_STRONGEST && frame <= 51) {
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw += TRIG_TURN / 256;
      else
        camera->yaw -= TRIG_TURN / 256;
    }
    // Severe Blow's flash.
    if (work->args->variant == SPARK_GROUPS_SEVERE_BLOW && frame == 4)
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    if (mode == SPARK_GROUPS_DYNAMITE || mode == SPARK_GROUPS_RISING) {
      if (frame == 2)
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    } else {
      if (frame == 2)
        Sound_PlayFar(SE_CRASH);
      if (frame == 24)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // Each burst flashes, then its ring and sparks, and the targets reel.
    for (i = 0; i != gPlasma_Shapes[work->args->variant][SPARK_GROUPS_BURSTS];
         i++) {
      start = i * SPARK_GROUPS_SPACING;
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start && frame < start + 2) {
        if (work->args->side == EFFECT_SIDE_PARTY)
          draw[0](
              canvas, work->sheet,
              originX -
                  gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] +
                  12,
              originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
        else
          draw[0](
              canvas, work->sheet,
              originX +
                  gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] -
                  44,
              originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
      }
      if (frame >= start) {
        for (j = 0; j != SPARK_GROUPS_RING; j++) {
          s32 x;
          s32 y;

          point = &work->particles[i * SPARK_GROUPS_FLASHES] + j;
          y = FIXED_INTEGER_PART(point->y) + originY;
          if (work->args->side == EFFECT_SIDE_PARTY)
            x = FIXED_INTEGER_PART(point->x) + originX -
                gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] +
                28;
          else
            x = FIXED_INTEGER_PART(point->x) + originX +
                gPlasma_Shapes[work->args->variant][SPARK_GROUPS_OFFSET + i] -
                28;
          if (point->variant >= 0 && point->variant < SPARK_GROUPS_FLASH_DRAWN)
            draw[0](canvas,
                    work->sheet + gPlasma_FlashPictures[point->variant / 3] *
                                      PICTURE_BYTES(FLASH_BURST),
                    x - FLASH_BURST_WIDTH / 2, y - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }
      // The sparks fly and fall (or rise, for SPARK_GROUPS_RISING).
      if (frame > start + 5) {
        FIXED gravity;

        if (mode == SPARK_GROUPS_RISING)
          gravity = -toFIXED(1) / 16;
        else
          gravity = toFIXED(1) / 16;
        for (j = 0;
             j != gPlasma_Shapes[work->args->variant][SPARK_GROUPS_SPARKS];
             j++) {
          spark = &sparks[i * gPlasma_Shapes[work->args->variant]
                                            [SPARK_GROUPS_SPARKS]] +
                  j;
          if (spark->variant > 0) {
            Particle_AdvanceWithGravity2D(spark, 60, gravity);
            spark->variant--;
            if (spark->y > toFIXED(108)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < toFIXED(127) &&
                       spark->y >= 0) {
              s16 x = FIXEDtoINT(spark->x);
              s16 y = FIXEDtoINT(spark->y);
              s32 size = spark->variant / 5 + 1;

              EFFECT_DRAW_MOTE(draw[j & 1], canvas, motes, x, y, size);
            }
          }
        }
      }
      EFFECT_FOR_EACH_TARGET(j, work->args) {
        if (frame == start + 6) {
          BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 10);
          BattleMotion_ApplyKnockbackFar(work->args->targets[j],
                                         KNOCKBACK_HURL);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef PLASMA_SPARK_COUNT
#undef PLASMA_SPACING
#undef PLASMA_SECOND_PICTURE
#undef PLASMA_COLUMN_X
#undef SPARK_GROUPS_FLASHES
#undef SPARK_GROUPS_SPACING
#undef SPARK_GROUPS_RING
#undef SPARK_GROUPS_FLASH_DRAWN
#undef SPARK_GROUPS_FLASH_LIFE
