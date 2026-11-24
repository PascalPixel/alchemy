/// @file
/// Dragon Cloud.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "system/io_write.h"

#define DRAGON_CLOUD_FRAMES 150
#define DRAGON_CLOUD_STRIKE_FRAME 83 ///< the flame strikes the first target
/// The size of the trail's picture, in pixels.
#define DRAGON_CLOUD_TRAIL_WIDTH 24
#define DRAGON_CLOUD_TRAIL_HEIGHT 39
#define DRAGON_CLOUD_TRAIL_PICTURE_BYTES                                       \
  PICTURE_BYTES(DRAGON_CLOUD_TRAIL) ///< bytes in each trail copy
/// The first of the particles the embers take.
#define DRAGON_CLOUD_FIRST_EMBER 8
/// FirePillarSheetB: five flames, then the trail picture.
#define DRAGON_CLOUD_FLAME_WIDTH 24
#define DRAGON_CLOUD_FLAME_HEIGHT 48
#define DRAGON_CLOUD_TRAIL_PICTURE (5 * PICTURE_BYTES(DRAGON_CLOUD_FLAME))
#define DRAGON_CLOUD_PUFF_PICTURES 15360
#define DRAGON_CLOUD_PUFFS 8 ///< puffs, particles 0 to 7
#define DRAGON_CLOUD_EMBERS 40
#define DRAGON_CLOUD_FLASHES 16 ///< flashes, after the embers
/// The trail's copies of the flame's path, the older ones from
/// DRAGON_CLOUD_TRAIL_BRIGHT on drawn darker, four levels a copy.
#define DRAGON_CLOUD_TRAIL_LENGTH 20
#define DRAGON_CLOUD_TRAIL_BRIGHT 10
/// The puffs' arc: this many pixels across and down from its centre.
#define DRAGON_CLOUD_PUFF_RADIUS_X 24
#define DRAGON_CLOUD_PUFF_RADIUS_Y 16
/// An ember is drawn at size DRAGON_CLOUD_EMBER_SIZE_MAX at depth
/// DRAGON_CLOUD_EMBER_DEPTH_NEAR or nearer, one size smaller every
/// DRAGON_CLOUD_EMBER_DEPTH_STEP farther, its depth held below
/// DRAGON_CLOUD_EMBER_DEPTH_FAR.
#define DRAGON_CLOUD_EMBER_SIZE_MAX 9
#define DRAGON_CLOUD_EMBER_DEPTH_NEAR 160
#define DRAGON_CLOUD_EMBER_DEPTH_STEP 64
#define DRAGON_CLOUD_EMBER_DEPTH_FAR 799
/// The flash and puff animations: six pictures, two frames each (Epicenter's
/// flashes, four frames each).
#define DRAGON_CLOUD_ANIMATION_PICTURES 6

/// Epicenter's sparks, EPICENTER_SPARK_COUNT of EXTRA_PARTICLES, come in three
/// batches: at the target, thrown up from the user's side, and shed by the
/// flame. The sparks thrown up start on EPICENTER_SPARK_GROUND and bounce
/// there.
#define EPICENTER_SPARK_COUNT 512
#define EPICENTER_SPARK_BATCH 128
#define EPICENTER_SPARK_GROUND 108

/// Draws one puff picture sized by the puff tables.
#define DRAGON_CLOUD_DRAW_PUFF(blit, canvas, sheet, picture, x, y)             \
  blit(canvas,                                                                 \
       &(sheet)[gBattleEffect_PuffArcPictures[picture] +                       \
                DRAGON_CLOUD_PUFF_PICTURES],                                   \
       x, y, gBattleEffect_PuffArcPictureWidths[picture],                      \
       gBattleEffect_PuffArcPictureHeights[picture])

/// A flame leaves the user along a stored path with a fading trail behind it,
/// strikes the first target and bursts into embers and flashes. Epicenter's
/// burns orange and throws sparks: up from the user's side at the start, off
/// the flame as it flies, and over the target at the strike.
///
/// Abilities: Dragon Cloud, Epicenter.
void BattleEffect_RunDragonCloud(struct BattleEffectParams *args) {
  struct Point2D trail[DRAGON_CLOUD_TRAIL_LENGTH];
  struct ScreenPoint target;
  struct ScreenPoint position;
  DrawRectangle draw[2];
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *motes;
  struct Actor *actor;
  struct Particle *particle;
  s32 i;
  s32 j;
  s32 angle;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  drift = 0;
  x = 0;
  y = 0;
  motes = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_B, state->sheet, 1, 1);
  // Darker copies of the trail picture, for the trail's tail.
  for (i = 1; i != DRAGON_CLOUD_TRAIL_LENGTH; i++) {
    for (j = 0; j != DRAGON_CLOUD_TRAIL_PICTURE_BYTES; j++) {
      s32 shade = state->sheet[DRAGON_CLOUD_TRAIL_PICTURE + j];

      if (i > DRAGON_CLOUD_TRAIL_BRIGHT) {
        shade = shade - (i - DRAGON_CLOUD_TRAIL_BRIGHT) * 4;
        if (shade < 0)
          shade = 0;
        state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
                     (i - DRAGON_CLOUD_TRAIL_BRIGHT) *
                         DRAGON_CLOUD_TRAIL_PICTURE_BYTES +
                     j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
                             &state->sheet[DRAGON_CLOUD_PUFF_PICTURES], 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, Ram_MapCellBuffer, 1,
                             0);
  if (args->variant == EFFECT_VARIANT_STRONGER) {
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  }
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);

  // The embers and flashes thrown at the strike wait at the target: the first
  // gDragonCloud_SparkCounts[variant] are drawn as embers, the rest play the
  // flash pictures and rise more slowly.
  actor = (struct Actor *)BattleObject_GetFar(args->targets[0])->object;
  for (i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
    particle = &state->particles[DRAGON_CLOUD_FIRST_EMBER + i];
    SET_POSITION(particle, actor->x.fixed, 0, actor->z.fixed);
    if (i >= gDragonCloud_SparkCounts[args->variant]) {
      particle->velocityX = ((Random16() & 63) - 32) << 14;
      particle->velocityY = (Random16() & 63) << 12;
    } else {
      particle->velocityX = ((Random16() & 63) - 32) << 14;
      particle->velocityY = (Random16() & 63) << 13;
    }
    Io_Write32(((Random16() & 63) - 32) << 14, &particle->velocityZ);
    particle->variant = 1;
  }

  // Epicenter's sparks: all asleep but the first batch, at the target (only
  // Epicenter draws them).
  for (i = 0; i != EPICENTER_SPARK_COUNT; i++)
    EXTRA_PARTICLES[i].variant = -1;
  for (i = 0; i != EPICENTER_SPARK_BATCH; i++) {
    particle = &EXTRA_PARTICLES[i];
    SET_POSITION(particle, actor->x.fixed, 0, actor->z.fixed);
    particle->velocityX = ((Random16() & 63) - 32) << 14;
    particle->velocityY = (Random16() & 63) << 12;
    Io_Write32(((Random16() & 63) - 32) << 14, &particle->velocityZ);
    particle->variant = (Random16() & 15) + 24;
  }

  // The eight puffs, on an arc over the user, each two frames after the last.
  for (i = 0, angle = -DEGtoANG(90); i != DRAGON_CLOUD_PUFFS;
       angle += TRIG_TURN / 16, i++) {
    particle = &state->particles[i];
    if (args->side == EFFECT_SIDE_PARTY)
      particle->x =
          FIXEDtoINT(Trig_Sin(angle) * DRAGON_CLOUD_PUFF_RADIUS_X) + 88;
    else
      particle->x =
          FIXEDtoINT(-(Trig_Sin(angle) * DRAGON_CLOUD_PUFF_RADIUS_X)) + 16;
    Io_Write32(FIXEDtoINT(Trig_Cos(angle) * DRAGON_CLOUD_PUFF_RADIUS_Y) + 40,
               &particle->y);
    particle->variant = -(i * 2);
  }
  path = Resource_GetTableEntry(RESOURCE_DRAGON_CLOUD_FLAME_PATH);

  for (frame = 0; frame != DRAGON_CLOUD_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraState;

    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      if (args->variant == EFFECT_VARIANT_BASIC)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 50)
      Sound_PlayFar(SE_WHOOSH);

    // For 64 frames the camera turns away from the user's side.
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if (frame <= 63)
        camera->yaw -= TRIG_TURN / 256;
    } else {
      if (frame <= 63)
        camera->yaw += TRIG_TURN / 256;
    }
    BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);

    // Frames 0-17: the puff arc rises, one picture every three frames.
    if (frame <= 17) {
      s32 picture = frame / 3;
      s32 width;
      s32 offset;

      offset = gBattleEffect_PuffArcPictures[picture];
      /* LEFTOVER(template): width, read from the puff table as
       * BattleEffect_RunHaures and BattleEffect_RunUlysses read it to draw
       * with, and unused here; as no width at all. */
      width = gBattleEffect_PuffArcPictureWidths[picture];
      draw[0](canvas, &state->sheet[offset + DRAGON_CLOUD_PUFF_PICTURES],
              56 - gBattleEffect_PuffArcPictureWidths[picture] / 2,
              gBattleEffect_PuffArcPictureBiasY[picture] + 60,
              gBattleEffect_PuffArcPictureWidths[picture],
              gBattleEffect_PuffArcPictureHeights[picture]);
      DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, state->sheet, picture,
                             64 - gBattleEffect_PuffArcPictureWidths[picture] /
                                      2,
                             gBattleEffect_PuffArcPictureBiasY[picture] + 60);
    }

    // Epicenter: a batch of sparks thrown up from the user's side at the start.
    if (args->variant == EFFECT_VARIANT_STRONGER) {
      if (frame == 0) {
        for (i = 0; i != EPICENTER_SPARK_BATCH; i++) {
          s32 angle;
          s32 force;

          particle = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH + i];
          particle->x = toFIXED(60);
          particle->y = toFIXED(EPICENTER_SPARK_GROUND);
          angle = (Random16() & 0x7fff) + DEGtoANG(90);
          force = (Random16() & 255) + 127;
          particle->velocityX = (Trig_Sin(angle) * force) >> 7;
          particle->velocityY = (Trig_Cos(angle) * force) >> 5;
          particle->variant = (Random16() & 15) + 24;
        }
      }
      // Only the first half of them is drawn.
      for (i = 0; i != EPICENTER_SPARK_BATCH / 2; i++) {
        particle = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH + i];
        if (particle->variant >= 0 && frame >= i / 8) {
          s32 size = (particle->variant >> 4) + 1;

          draw[i & 1](canvas, motes + gBattleEffect_MotePictures[size - 1],
                      FIXED_INTEGER_PART(particle->x) - size / 2,
                      FIXED_INTEGER_PART(particle->y) - size, size, size * 2);
          Particle_AdvanceWithGravity2D(particle, 62, toFIXED(1) / 4);
          if (particle->y > INTtoFIXED((i & 3) + EPICENTER_SPARK_GROUND))
            particle->velocityY = -particle->velocityY / 2;
          particle->variant--;
        }
      }
    }

    // The flame flies along its path, a 16-bit start and then steps; from
    // frame 78 it dives from above the target.
    if (frame >= 18 && frame <= 58) {
      if (frame == 18) {
        x = (path[0] << 8) + (u8)path[1];
        y = (path[2] << 8) + (u8)path[3] + 16;
        path += 4;
      } else {
        x += path[0];
        y += path[1];
        path += 2;
      }
    }
    if (frame >= 78 && frame <= 118) {
      EffectPosition_ProjectBattler(args->targets[0], &target);
      if (frame == 78) {
        x = target.x / 2;
        y = 48;
      } else {
        y -= 16;
      }
    }

    // The trail: each copy takes the place of the one before, the older
    // ones darker.
    for (j = DRAGON_CLOUD_TRAIL_LENGTH - 1; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > DRAGON_CLOUD_TRAIL_BRIGHT)
          draw[0](canvas,
                  &state->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
                                (j - DRAGON_CLOUD_TRAIL_BRIGHT) *
                                    DRAGON_CLOUD_TRAIL_PICTURE_BYTES],
                  trail[j].x - DRAGON_CLOUD_TRAIL_WIDTH / 2, trail[j].y,
                  DRAGON_CLOUD_TRAIL_WIDTH, DRAGON_CLOUD_TRAIL_HEIGHT);
        else
          draw[0](canvas, &state->sheet[DRAGON_CLOUD_TRAIL_PICTURE],
                  trail[j].x - DRAGON_CLOUD_TRAIL_WIDTH / 2, trail[j].y,
                  DRAGON_CLOUD_TRAIL_WIDTH, DRAGON_CLOUD_TRAIL_HEIGHT);
      }
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    // The flame leans as it rises and falls.
    if (frame >= 18 && frame <= DRAGON_CLOUD_STRIKE_FRAME) {
      s32 lift;

      if (frame >= 78 && frame <= 118)
        position.x = x;
      else if (args->side == EFFECT_SIDE_PARTY)
        position.x = 64 - x / 2;
      else
        position.x = x / 2 + 64;
      position.y = 60 - y;
      lift = (position.y - trail[0].y - 24) / 2;
      if (lift > 2)
        lift = 2;
      if (lift < -2)
        lift = -2;
      drift += lift;
      if (drift > 8)
        drift = 8;
      if (drift < -8)
        drift = -8;
      lift = drift / 4 + 2;
      trail[0].x = position.x;
      trail[0].y = position.y - 20;
      draw[0](canvas, &state->sheet[lift * PICTURE_BYTES(DRAGON_CLOUD_FLAME)],
              trail[0].x - DRAGON_CLOUD_FLAME_WIDTH / 2, trail[0].y - 2,
              DRAGON_CLOUD_FLAME_WIDTH, DRAGON_CLOUD_FLAME_HEIGHT);

      // The flame sheds four sparks a frame (only Epicenter draws them).
      for (i = 0; i != 4; i++) {
        s32 angle;
        s32 force;
        s32 index;

        index = ((frame * 4 + i) & (EPICENTER_SPARK_BATCH - 1)) +
                EPICENTER_SPARK_BATCH * 2;
        particle = &EXTRA_PARTICLES[index];
        particle->x = INTtoFIXED(position.x);
        particle->y = INTtoFIXED(position.y);
        angle = Random16();
        force = (Random16() & 255) + 127;
        particle->velocityX = (Trig_Sin(angle) * force) >> 7;
        particle->velocityY = (Trig_Cos(angle) * force) >> 5;
        particle->variant = (Random16() & 15) + 24;
      }
    }

    // After the strike the embers rise and fall back, sized by depth, and the
    // flashes play the flash pictures.
    if (frame >= DRAGON_CLOUD_STRIKE_FRAME) {
      for (i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
        particle = &state->particles[DRAGON_CLOUD_FIRST_EMBER + i];
        if (particle->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&particle->x, &position);
          position.x >>= 1;
          if (position.depth < DRAGON_CLOUD_EMBER_DEPTH_NEAR)
            position.depth = DRAGON_CLOUD_EMBER_DEPTH_NEAR;
          if (position.depth > DRAGON_CLOUD_EMBER_DEPTH_FAR)
            position.depth = DRAGON_CLOUD_EMBER_DEPTH_FAR;
          size = DRAGON_CLOUD_EMBER_SIZE_MAX -
                 (position.depth - DRAGON_CLOUD_EMBER_DEPTH_NEAR) /
                     DRAGON_CLOUD_EMBER_DEPTH_STEP;
          if (i >= gDragonCloud_SparkCounts[args->variant]) {
            s32 picture;

            if (args->variant == EFFECT_VARIANT_BASIC)
              picture = particle->variant / 2;
            else
              picture = particle->variant / 4;
            if (picture < DRAGON_CLOUD_ANIMATION_PICTURES) {
              draw[args->variant](
                  canvas,
                  Ram_MapCellBuffer + picture * PICTURE_BYTES(FLASH_BURST),
                  position.x - FLASH_BURST_WIDTH / 2, position.y - 40,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
              particle->variant++;
              Particle_AdvanceWithGravity3D(particle, 62, toFIXED(1) / 8);
            }
          } else {
            draw[0](canvas, motes + gBattleEffect_MotePictures[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            Particle_AdvanceWithGravity3D(particle, PARTICLE_DAMPING_NONE,
                                          -toFIXED(1) / 8);
          }
        }
      }

      // Epicenter's first batch of sparks, at the target.
      if (args->variant == EFFECT_VARIANT_STRONGER) {
        for (i = 0; i != EPICENTER_SPARK_BATCH; i++) {
          s32 size;

          particle = &EXTRA_PARTICLES[i];
          size = particle->variant;
          if (size >= 0) {
            EffectPosition_ProjectPoint(&particle->x, &position);
            size >>= 3;
            size += 1;
            position.x >>= 1;
            draw[i & 1](canvas, motes + gBattleEffect_MotePictures[size - 1],
                        position.x - size / 2, position.y - size, size,
                        size * 2);
            Particle_AdvanceWithGravity3D(particle, 62, toFIXED(1) / 8);
            particle->variant--;
          }
        }
      }
    }

    // Epicenter: the sparks the flame shed.
    if (args->variant == EFFECT_VARIANT_STRONGER) {
      struct Particle *spark = &EXTRA_PARTICLES[EPICENTER_SPARK_BATCH * 2];

      for (i = 0; i != EPICENTER_SPARK_BATCH; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 4) + 1;

          draw[i & 1](canvas, motes + gBattleEffect_MotePictures[size - 1],
                      FIXED_INTEGER_PART(spark->x) - size / 2,
                      FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
          Particle_AdvanceWithGravity2D(spark, 60, -toFIXED(1) / 4);
          spark->variant--;
        }
      }
    }

    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      state->shakeFrames = 8;
      EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_HOP);
    }
    if (frame == 50) {
      state->shakeFrames = 12;
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }
    // The puffs.
    if (frame > 49) {
      for (i = 0; i != DRAGON_CLOUD_PUFFS; i++) {
        particle = &state->particles[i];
        if (particle->variant >= 0 &&
            particle->variant < 2 * DRAGON_CLOUD_ANIMATION_PICTURES) {
          s32 picture = particle->variant / 2;

          DRAGON_CLOUD_DRAW_PUFF(
              draw[1], canvas, state->sheet, picture,
              particle->x - gBattleEffect_PuffArcPictureWidths[picture] / 2,
              particle->y + gBattleEffect_PuffArcPictureBiasY[picture]);
        }
        particle->variant++;
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}
#undef DRAGON_CLOUD_FRAMES
#undef DRAGON_CLOUD_STRIKE_FRAME
#undef DRAGON_CLOUD_TRAIL_WIDTH
#undef DRAGON_CLOUD_TRAIL_HEIGHT
#undef DRAGON_CLOUD_TRAIL_PICTURE_BYTES
#undef DRAGON_CLOUD_FIRST_EMBER
#undef DRAGON_CLOUD_FLAME_WIDTH
#undef DRAGON_CLOUD_FLAME_HEIGHT
#undef DRAGON_CLOUD_TRAIL_PICTURE
#undef DRAGON_CLOUD_PUFF_PICTURES
#undef DRAGON_CLOUD_PUFFS
#undef DRAGON_CLOUD_EMBERS
#undef DRAGON_CLOUD_FLASHES
#undef DRAGON_CLOUD_TRAIL_LENGTH
#undef DRAGON_CLOUD_TRAIL_BRIGHT
#undef DRAGON_CLOUD_PUFF_RADIUS_X
#undef DRAGON_CLOUD_PUFF_RADIUS_Y
#undef DRAGON_CLOUD_EMBER_SIZE_MAX
#undef DRAGON_CLOUD_EMBER_DEPTH_NEAR
#undef DRAGON_CLOUD_EMBER_DEPTH_STEP
#undef DRAGON_CLOUD_EMBER_DEPTH_FAR
#undef DRAGON_CLOUD_ANIMATION_PICTURES
#undef EPICENTER_SPARK_COUNT
#undef EPICENTER_SPARK_BATCH
#undef EPICENTER_SPARK_GROUND
#undef DRAGON_CLOUD_DRAW_PUFF
