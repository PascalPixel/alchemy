/// @file
/// Plasma, and the spark groups of Blast and Dynamite.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// The lightning pillars throw their sparks into one pool in PARTICLES, as
/// Volcano's fire pillars do.
#define PLASMA_SPARK_COUNT PARTICLE_POOL_COUNT
/// The ground the sparks fly up from and bounce off.
#define PLASMA_GROUND_Y 104

#define PLASMA_SPACING 8 ///< frames between pillars
/// The second picture in the sprite sheet, after the first 48x112 one.
#define PLASMA_SECOND_PICTURE PICTURE_BYTES(LIGHTNING_PILLAR)
/// The canvas level a strike floods the whole canvas with: the flash that
/// tints the battle as each pillar comes down.
#define PLASMA_FLASH_LEVEL 16

/// gPlasma_Columns has seven pillars for each side.
#define PLASMA_COLUMN_X(side, pillar) gPlasma_Columns[(pillar) + (side) * 7]
/// Each Plasma variant's pair in gPlasma_Sparks: the sparks per pillar and the
/// frames of shake.
#define PLASMA_SPARKS 0       ///< sparks each lightning pillar throws
#define PLASMA_SHAKE_FRAMES 1 ///< frames the screen shakes at each strike

/// Pillars of lightning strike one after another, each with a flash,
/// throwing sparks up from the ground and knocking the targets back.
///
/// Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
/// Thunderhead.
void BattleEffect_RunPlasma(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  struct BattleEffectState *state;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  struct Particle *spark;
  struct Particle *sparks;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_PILLAR_SHEET, state->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  if (args->variant != EFFECT_VARIANT_STRONGEST) {
    LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_B);
  }

  for (k = 0; k != PLASMA_SPARK_COUNT; k++)
    PARTICLES[k].variant = 0;
  sparks = PARTICLES;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  count = gPlasma_Counts[args->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame <= 63) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraState;
      s32 speed;

      // A 256th of a turn a frame, slowing from frame 56.
      if (frame <= 55)
        speed = TRIG_TURN / 256;
      else
        speed = TRIG_TURN / 256 - (frame - 56) * 8;
      if (args->side == EFFECT_SIDE_PARTY)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // Each pillar strikes with a flash.
    for (i = 0; i != count; i++) {
      start = i * PLASMA_SPACING;
      if (frame == start) {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(PLASMA_FLASH_LEVEL));
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, state->sheet,
                  PLASMA_COLUMN_X(args->side, i) - LIGHTNING_PILLAR_WIDTH / 2,
                  0, LIGHTNING_PILLAR_WIDTH, LIGHTNING_PILLAR_HEIGHT);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, state->sheet + PLASMA_SECOND_PICTURE,
                  PLASMA_COLUMN_X(args->side, i) - LIGHTNING_PILLAR_WIDTH / 2,
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

              spark->x = INTtoFIXED(PLASMA_COLUMN_X(args->side, i));
              spark->y = toFIXED(PLASMA_GROUND_Y);
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              spawned++;
              if (spawned == gPlasma_Sparks[args->variant * 2 + PLASMA_SPARKS])
                break;
            }
          }
          state->shakeFrames =
              gPlasma_Sparks[args->variant * 2 + PLASMA_SHAKE_FRAMES];
        }
      }
      // The targets recoil.
      if (frame == start + 4) {
        EFFECT_FOR_EACH_TARGET(k, args) {
          BattleMotion_ApplyKnockbackFar(args->targets[k], KNOCKBACK_HOP);
          BattleEffect_TintBattler(args->targets[k], SPRITE_COLOR_WHITE,
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
        if (spark->y > toFIXED(PLASMA_GROUND_Y)) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = FIXEDtoINT(spark->x);
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = FIXEDtoINT(spark->y);
            size = spark->variant / 8 + 1;
            EFFECT_DRAW_MOTE(draw[k & 1], canvas, sprites, x, y, size);
          }
        }
      }
    }

    BattleCamera_Shake(8, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

/// Bursts go off behind the battle's first layer.
///
/// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb.
void BattleEffect_RunNova(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_NOVA);
}

/// Orange bursts go off in front of the battle, which darkens behind them.
///
/// Abilities: Dynamite, Angry Mine.
void BattleEffect_RunDynamite(struct BattleEffectParams *args) {
  BattleEffect_RunSparkGroups(args, SPARK_GROUPS_DYNAMITE);
}

/// Each spark group variant's row of gSparkGroups_Shapes: sparks per burst,
/// bursts, and from SPARK_GROUPS_OFFSET each burst's offset across.
#define SPARK_GROUPS_SPARKS 0 ///< sparks each burst throws
#define SPARK_GROUPS_BURSTS 1 ///< bursts, SPARK_GROUPS_SPACING frames apart
#define SPARK_GROUPS_OFFSET 2 ///< burst i's offset across, at [2 + i]

#define SPARK_GROUPS_FLASHES 16     ///< flashes kept per burst
#define SPARK_GROUPS_SPACING 8      ///< frames between bursts
#define SPARK_GROUPS_RING 12        ///< of which drawn as the ring
#define SPARK_GROUPS_FLASH_DRAWN 18 ///< a flash is drawn for its last 18 frames
/// A ring flash's frames, a frame more for every second flash out.
#define SPARK_GROUPS_FLASH_LIFE 25
/// The ground the sparks bounce off.
#define SPARK_GROUPS_GROUND_Y 108
/// Burst i is centred at the origin plus its offset less this, across the
/// screen; when the party casts, at the origin less its offset plus this.
#define SPARK_GROUPS_CENTRE_SHIFT 28

/// One to three bursts go off: each flashes, rings itself with smaller flashes
/// that fade, and throws out sparks that bounce on the ground, knocking the
/// targets back.
void BattleEffect_RunSparkGroups(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint screenPosition;
  DrawRectangle draw[2];
  struct BattleEffectState *state;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct Particle *sparks;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;

  if (mode == SPARK_GROUPS_NOVA) {
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
    originX = 60;
    originY = 48;
  } else if (mode == SPARK_GROUPS_DYNAMITE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    state->fadeFrames = BACKGROUND_DARKEN_FRAMES;
    state->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(args->user, &screenPosition);
    originX = screenPosition.x / 2;
    originY = screenPosition.y + 48;
  }

  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
  if (mode == SPARK_GROUPS_DYNAMITE) {
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  } else if (mode == SPARK_GROUPS_RISING) {
    LOAD_BG_PALETTE(RESOURCE_LIGHTNING_BOLT_SHEET);
  }

  sparks = PARTICLES;

  for (i = 0; i != gSparkGroups_Shapes[args->variant][SPARK_GROUPS_BURSTS];
       i++) {
    for (j = 0; j != SPARK_GROUPS_FLASHES; j++) {
      struct Particle *point;
      s32 radius;
      s32 angle;

      point = &state->particles[i * SPARK_GROUPS_FLASHES + j];
      radius = j * 2;
      angle = Random16() & TRIG_ANGLE_MASK;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + SPARK_GROUPS_FLASH_LIFE;
    }
    for (j = 0; j != gSparkGroups_Shapes[args->variant][SPARK_GROUPS_SPARKS];
         j++) {
      struct Particle *spark;
      s32 speed;
      s32 angle;

      spark = sparks +
              i * gSparkGroups_Shapes[args->variant][SPARK_GROUPS_SPARKS] + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      if (args->side == EFFECT_SIDE_PARTY)
        spark->x = INTtoFIXED(
            originX -
            gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] +
            SPARK_GROUPS_CENTRE_SHIFT);
      else
        spark->x = INTtoFIXED(
            originX +
            gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] -
            SPARK_GROUPS_CENTRE_SHIFT);
      spark->y = INTtoFIXED(originY);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0;
       frame != gSparkGroups_Shapes[args->variant][SPARK_GROUPS_BURSTS] *
                        SPARK_GROUPS_SPACING +
                    40;
       frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;

    // The strongest variant turns the camera.
    if (args->variant == EFFECT_VARIANT_STRONGEST && frame <= 51) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw += TRIG_TURN / 256;
      else
        camera->yaw -= TRIG_TURN / 256;
    }
    // Severe Blow's flash.
    if (args->variant == SPARK_GROUPS_SEVERE_BLOW && frame == 4)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));

    if (mode == SPARK_GROUPS_DYNAMITE || mode == SPARK_GROUPS_RISING) {
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

    // Each burst flashes, then its ring and sparks, and the targets reel.
    for (i = 0; i != gSparkGroups_Shapes[args->variant][SPARK_GROUPS_BURSTS];
         i++) {
      start = i * SPARK_GROUPS_SPACING;
      if (frame == start)
        state->shakeFrames = 12;

      if (frame >= start && frame < start + 2) {
        if (args->side == EFFECT_SIDE_PARTY)
          draw[0](
              canvas, state->sheet,
              originX -
                  gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] +
                  SPARK_GROUPS_CENTRE_SHIFT - FLASH_BURST_WIDTH / 2,
              originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
        else
          draw[0](
              canvas, state->sheet,
              originX +
                  gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] -
                  SPARK_GROUPS_CENTRE_SHIFT - FLASH_BURST_WIDTH / 2,
              originY - FLASH_BURST_HEIGHT / 2, FLASH_BURST_WIDTH,
              FLASH_BURST_HEIGHT);
      }

      if (frame >= start) {
        for (j = 0; j != SPARK_GROUPS_RING; j++) {
          struct Particle *point;
          s32 x;
          s32 y;

          point = &state->particles[i * SPARK_GROUPS_FLASHES + j];
          y = FIXED_INTEGER_PART(point->y) + originY;
          if (args->side == EFFECT_SIDE_PARTY)
            x = FIXED_INTEGER_PART(point->x) + originX -
                gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] +
                SPARK_GROUPS_CENTRE_SHIFT;
          else
            x = FIXED_INTEGER_PART(point->x) + originX +
                gSparkGroups_Shapes[args->variant][SPARK_GROUPS_OFFSET + i] -
                SPARK_GROUPS_CENTRE_SHIFT;
          if (point->variant >= 0 && point->variant < SPARK_GROUPS_FLASH_DRAWN)
            draw[0](canvas,
                    state->sheet + gPlasma_FlashPictures[point->variant / 3] *
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
             j != gSparkGroups_Shapes[args->variant][SPARK_GROUPS_SPARKS];
             j++) {
          struct Particle *spark =
              sparks +
              i * gSparkGroups_Shapes[args->variant][SPARK_GROUPS_SPARKS] + j;
          if (spark->variant > 0) {
            Particle_AdvanceWithGravity2D(spark, 60, gravity);
            spark->variant--;
            if (spark->y > toFIXED(SPARK_GROUPS_GROUND_Y)) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < toFIXED(127) &&
                       spark->y >= 0) {
              s16 y = FIXEDtoINT(spark->y);
              s16 x = FIXEDtoINT(spark->x);
              s32 size = spark->variant / 5 + 1;

              EFFECT_DRAW_MOTE(draw[j & 1], canvas, graphics, x, y, size);
            }
          }
        }
      }

      EFFECT_FOR_EACH_TARGET(j, args) {
        if (frame == start + 6) {
          EFFECT_STRIKE_TARGET(args, j, 10, KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef PLASMA_SPARK_COUNT
#undef PLASMA_GROUND_Y
#undef PLASMA_SPACING
#undef PLASMA_SECOND_PICTURE
#undef PLASMA_FLASH_LEVEL
#undef PLASMA_COLUMN_X
#undef PLASMA_SPARKS
#undef PLASMA_SHAKE_FRAMES
#undef SPARK_GROUPS_SPARKS
#undef SPARK_GROUPS_BURSTS
#undef SPARK_GROUPS_OFFSET
#undef SPARK_GROUPS_FLASHES
#undef SPARK_GROUPS_SPACING
#undef SPARK_GROUPS_RING
#undef SPARK_GROUPS_FLASH_DRAWN
#undef SPARK_GROUPS_FLASH_LIFE
#undef SPARK_GROUPS_GROUND_Y
#undef SPARK_GROUPS_CENTRE_SHIFT
