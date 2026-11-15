/// @file
/// The Procne summon, and the Tiamat summon and Dragon Driver.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "system/mul_q16.h"
#include "resource_ids.h"

/// Who breathes the dragon's fire (BattleEffect_RunDragonFire).
enum DragonFireMode {
  DRAGON_FIRE_TIAMAT,       ///< Tiamat lands beside the targets and breathes
                            ///< the fire
  DRAGON_FIRE_DRAGON_DRIVER ///< no dragon is drawn: the breath starts just in
                            ///< front of the targets
};

/// frame / 4 % 3, as the source writes it: the multiply-high routine with the
/// reciprocal of 3, where a plain % calls __modsi3.
#define MODULO_THREE(v) Math_UnsignedMulHighAdd((v), 0x55555556, -3)

#define WHIRLWIND_CEL (48 * 64) ///< one of the whirlwind's three frames
#define DUST_SHEET 0x2400       ///< the dust motes, after the whirlwind
#define COMET_SHEET 0x3600      ///< the violet comets
#define COMETS 9
#define FORMATION_RESOURCE 646

/// A whirlwind lifts the targets, then Procne, a great bird in a violet sky,
/// strikes them with comets: the whirlwind spins them while dust swirls up
/// through it (frames 0 to 159; A or B skips to 160), a cloud of sparks
/// gathers, and from frame 330 four comets strike one after another, each
/// with a burst of sparks. A or B skips frames 161 to 394.
///
/// Abilities: Procne.
void BattleEffect_RunProcne(struct BattleEffectParams *args) {
  void *canvas;
  void *camera;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *graphics;
  struct MotionObject *object;
  struct BattleObject *context;
  struct Particle *point;
  struct Particle *spark;
  struct Particle *mark;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  s32 vector[3];
  struct ScreenPoint screen;
  s32 shake[3];
  s32 place[4];
  struct Scale scale;

  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  work = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;

  // Set up: the max and additive pair, the whirlwind with its palette, the
  // dots, and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress((s32)&ResourceId_WhirlwindSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0,
                               0);
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    wave.x = px(128);
    wave.y = px(40);
    drift.x = 0;
    drift.y = 0;

    // Each enemy's place, heading, distance and facing, to restore at the
    // end; its bobbing stops.
    for (index = 0; index != args->targetCount; index++) {
      context = BattleObject_GetFar(args->targets[index]);
      object = context->object;
      home[index].x = object->x;
      home[index].y = object->z;
      baseAngle[index] = object->angle;
      heading[index] = (u16)ArcTan2(object->x, object->z);
      radius[index] = Iwram_Sqrt((object->x >> 8) * (object->x >> 8) +
                                 (object->z >> 8) * (object->z >> 8)) >>
                      7;
      spin[index] = 0;
      object->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after its own wait.
    index = 0;
    do {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
      index++;
    } while (index != 64);

    frame = 0;
    do {
      if ((gInput.repeat & (KEY_A | KEY_B)) != 0) {
        if (frame <= 159)
          frame = 160;
        else if (frame <= 394)
          frame = 395;
      }

      Transform_Reset();
      Transform_SetLookAt(camera, (s32 *)((u8 *)camera + 12));

      if (frame == 16)
        Sound_PlayFar(SE_RUMBLE);
      if (frame == 256)
        Sound_PlayFar(SE_SPIN);
      if (frame == 334)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 347)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 359)
        Sound_PlayFar(SE_POWER_UP);
      if (frame == 372)
        Sound_PlayFar(SE_POWER_UP);

      // The enemies, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each comet strike knocks them down.
      for (index = 0; index != args->targetCount; index++) {
        if (frame > index * 16) {
          context = BattleObject_GetFar(args->targets[index]);
          object = context->object;
          object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame <= 159) {
            if (frame > index * 16 + 16)
              spin[index] += 48;
            if (radius[index] <= 31)
              object->y += px(6);
            else
              object->y += px(1) / 2;
            if (object->y > px(124))
              object->y = px(124);
          }

          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > TRIG_TURN)
              heading[index] -= TRIG_TURN;
            object->angle += spin[index] / 4;
          }

          if (frame == 395)
            object->verticalMotionStrength = 0xab85;

          slot = 0;
          do {
            if (frame == gProcne_Frames[slot]) {
              object->velocityY = 0;
              BattleEffect_TintBattler(args->targets[index], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, index, 8);
            }
            slot++;
          } while (slot != 4);

          if (radius[index] > 16)
            radius[index] -= 2;
        }
      }

      // The whirlwind: its foot grows from frame 16, its top from 48, and
      // from 160 it stands whole, two cels high.
      if ((u32)(frame - 16) <= 143) {
        size = frame * 2 - 32;
        if (size > 48)
          size = 48;
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * WHIRLWIND_CEL +
                             (48 - size) * 48],
                32, 112 - size, 48, size);
      }
      if ((u32)(frame - 48) <= 111) {
        size = frame * 2 - 96;
        if (size > 64)
          size = 64;
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * WHIRLWIND_CEL +
                             (64 - size) * 48],
                32, 64 - size, 48, size);
      }
      if ((u32)(frame - 160) <= 239) {
        draw[0](canvas, &work->sheet[MODULO_THREE(frame / 4) * WHIRLWIND_CEL],
                8, 0, 48, 64);
        draw[0](canvas, &work->sheet[MODULO_THREE(frame / 4) * WHIRLWIND_CEL],
                8, 64, 48, 64);
      }

      // The dust motes swirl up the whirlwind, speeding up, and start again
      // from the foot when they pass the top.
      if (frame <= 159) {
        index = 0;
        point = work->particles;
        do {
          if (point[index].variant == 0) {
            vector[0] = point[index].z * Trig_Sin(point[index].velocityY);
            vector[1] = point[index].y;
            vector[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(vector, &screen);
            screen.x >>= 1;
            if (point[index].y <= 0x3fffff)
              draw[0](canvas, work->sheet + DUST_SHEET, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > px(1))
              point[index].velocityY -= px(1);
            point[index].velocityX += 50;
            if (point[index].velocityX > 4096)
              point[index].velocityX = 4096;
            point[index].velocityY += 2048;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > px(48)) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
          index++;
        } while (index != 24);
      }

      // Frame 160: the violet sky, the formation of comets, sparks scattered
      // round its centre, and the enemies dropped back at random heights.
      if (frame == 160) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
        BattleBackground_SetModeFar(1, (s32)&ResourceId_VioletSkyBackdrop, 8);
        BattleEffect_SelectLivingTargets(work->args);
        BattleEffect_SpawnObjects(COMETS, FORMATION_RESOURCE, 2);
        Resource_LoadAndDecompress((s32)&ResourceId_VioletCometSheet,
                                   work->sheet + COMET_SHEET, 1, 1);

        index = 0;
        do {
          PARTICLES[index].variant = 0;
          index++;
        } while (index != 512);

        for (index = 0; index != args->targetCount; index++) {
          context = BattleObject_GetFar(args->targets[index]);
          object = context->object;
          object->y = px((Random16() & 15) + 8);
        }

        gCameraSceneParameters[3] = 72;

        index = 0;
        do {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + 0x9fff;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = px(Random16() % 200 - 100);
          work->particles[index].variant = 0;
          index++;
        } while (index != 32);

        for (index = 0; index != args->targetCount; index++) {
          radius[index] = 16;
          spin[index] = 6000;
        }
      }

      if (frame > 159) {
        // The formation sways on a slow wave, pushed back by each strike.
        scale = gProcne_UnitScale;
        size = 16;
        if (frame - 160 <= 64)
          size = 32;
        wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
        wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = Q16_ONE;
        scale.y = Q16_ONE;
        place[3] = 0;
        index = 0;
        do {
          place[0] = px(gProcne_GridX[index]) + wave.x + px(40);
          place[1] = px(255) + index * px(64);
          place[2] = px(gProcne_GridY[index] + index * 64) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct ScaledSprite *)work->sprites[index], place, &scale.x, 0);
          index++;
        } while (index != COMETS);

        if (frame > 255) {
          s32 threshold;

          shake[2] = 0x1000000;
          alive = 0;
          shake[0] = 0;
          shake[1] = 0;
          Transform_Reset();
          SceneTransform_ApplyPosition(shake);

          // Until frame 330 the scattered sparks gather on the centre, each
          // a part of its distance a frame; those that have arrived make one
          // spark that grows with their number.
          if (frame <= 329) {
            index = 0;
            do {
              s32 xx;
              s32 yy;
              s32 zz;

              point = &work->particles[index];
              xx = (point->x >> 8) * (point->x >> 8);
              yy = (point->y >> 8) * (point->y >> 8);
              zz = (point->z >> 8) * (point->z >> 8);
              dist = Iwram_Sqrt(xx + yy + zz) >> 9;
              if (dist != 0) {
                EffectPosition_ProjectPoint(&point->x, &screen);
                screen.x = (screen.x >> 17) + (wave.x >> 17) + 32;
                screen.y = (screen.y >> 16) + (wave.y >> 16) - 4;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 6 - Math_UnsignedMulHigh(screen.depth - 170, 0x071c71c8);
                draw[1](canvas, graphics + gBattleEffect_MotePictures[span - 1],
                        screen.x - span / 2, screen.y - span, span, span * 2);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                alive++;
              }
              index++;
            } while (index != 32);

            if (alive > 0) {
              span = alive / 10 + 1;
              draw[1](canvas, graphics + gBattleEffect_MotePictures[span - 1],
                      (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                      span, span * 2);
            }
          }

          // Each strike throws out 96 sparks that fly and fade.
          slot = 0;
          do {
            if (frame == gProcne_Frames[slot]) {
              s32 speed;
              s32 angle;

              index = 0;
              do {
                speed = (Random16() & 127) + 16;
                angle = Random16() & 0xffff;
                spark = &PARTICLES[slot * 128 + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                spark->velocityY = ((Random16() & 255) - 128) << 10;
                SET_POSITION(spark, 0, 0, 0);
                spark->variant = (Random16() & 15) + 64;
                index++;
              } while (index != 96);
            }

            if (frame >= gProcne_Frames[0]) {
              spark = PARTICLES;
              index = 0;
              do {
                if (spark[index].variant > 0) {
                  /* FAKEMATCH(dead): the transform in a once-only block. */
                  do {
                    void (*transform)(const void *, s32 *) =
                        Iwram_TransformVectorEntry;
                    /* FAKEMATCH(pin): the result pointer in r1, set before the
                     * point in r0. */
                    register s32 *result __asm__("r1") = &screen.x;
                    transform(&spark[index].x, result);
                  } while (0);
                  screen.x = (screen.x >> 17) + 32;
                  screen.y = (screen.y >> 16) + 56;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas,
                          graphics + gBattleEffect_MotePictures[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
                  spark[index].z += spark[index].velocityY;
                  spark[index].variant--;
                }
                index++;
              } while (index != 512);
            }
            slot++;
          } while (slot != 4);

          // From frame 330, a comet every twelve frames: it strikes at the
          // formation's centre, the screen shakes and the formation is
          // pushed back; a ring of 28 sparks opens round the mark.
          index = 0;
          do {
            mark = &work->particles[index + 60];
            threshold = index * 12 + 330;
            if (frame == threshold) {
              mark->x = (wave.x >> 17) + 32;
              mark->y = (wave.y >> 16) - 4;
              mark->velocityX = (wave.x >> 17) + 32;
              mark->velocityY = (wave.y >> 16) - 4;

              slot = 0;
              do {
                work->particles[slot + 32].variant = 4;
                slot++;
              } while (slot != 28);

              drift.x = px(8);
              drift.y = -px(2);
              work->shakeFrames = 8;
            }

            if (frame >= threshold) {
              s32 cel;

              cel = (frame - threshold) / 2;
              if (cel > 2)
                cel = 2;
              draw[1](canvas, work->sheet + gProcne_Pictures[cel], mark->x,
                      mark->y - gProcne_PictureBiasY[cel],
                      gProcne_PictureWidths[cel], gProcne_PictureHeights[cel]);
              mark->x -= 8;
              mark->y += 2;

              if (frame < threshold + 8) {
                Transform_ApplyRoll(-2048);
                Transform_ApplyYaw(-4096);

                slot = 0;
                spark = &work->particles[32];
                do {
                  struct Particle *ring;

                  ring = &spark[slot];
                  vector[0] = 0;
                  vector[1] = ring->variant * Trig_Cos(slot * 2340);
                  vector[2] = ring->variant * Trig_Sin(slot * 2340);
                  ring->variant += 2;
                  EffectPosition_ProjectPoint(vector, &screen);
                  screen.x =
                      (screen.x >> 17) + work->particles[index + 60].velocityX;
                  screen.y =
                      (screen.y >> 16) + work->particles[index + 60].velocityY;
                  screen.depth >>= 16;
                  if (screen.depth <= 169)
                    screen.depth = 170;
                  if (screen.depth > 350)
                    screen.depth = 350;
                  span =
                      3 - Math_UnsignedMulHigh(screen.depth - 170, 0x02d82d83);
                  draw[0](canvas,
                          graphics + gBattleEffect_MotePictures[span - 1],
                          screen.x - span / 2, screen.y - span, span, span * 2);
                  slot++;
                } while (slot != 28);
              }
            }
            index++;
          } while (index != 4);
        }
      }

      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != 400);

    // The results; the enemies go back where they stood, facing as before.
    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (index = 0; index != args->targetCount; index++) {
      struct MotionObject *enemy;

      context = BattleObject_GetFar(args->targets[index]);
      enemy = context->object;
      enemy->x = home[index].x;
      enemy->z = home[index].y;
      enemy->angle = baseAngle[index];
    }
    gCameraSceneParameters[3] = 120;
    BattleEffect_SetupBlendedDisplay();

    index = 0;
    do {
      Sprite_DestroyFar(work->sprites[index]);
      index++;
    } while (index != COMETS);

    EFFECT_FINISH_CANVAS();
  }
}

/// Abilities: Dragon Driver.
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}

/// Abilities: Tiamat.
void BattleEffect_RunTiamat(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_TIAMAT);
}

#undef FORMATION_RESOURCE

#define STREAK_SHEET 0x3000 ///< the ember streaks, after the flashes
#define FORMATION_RESOURCE 647

/// Streaks rain down and splash, sparks and grains rise from where the
/// formation stands, flashes burst at frame 128, then the screen whitens and
/// the canvas fades away. For Tiamat a formation of eight drops in at frame 40
/// and bursts at the end; Dragon Driver is the same scene cast from the
/// caster's side, without the formation. A or B skips frames 49 to 159.
void BattleEffect_RunDragonFire(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint origin;
  DrawRectangle draw[2];
  void *canvas;
  s32 *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  work = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;

  // Set up: half-and-half blending, the max and additive pair, the flashes
  // with their palette, the streaks, the dots, and in mode 1 the lightning
  // palette.
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             work->sheet + STREAK_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
    const void *lightning =
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, lightning, 128);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    s32 dir;
    s32 alternate;

    // When the first target's animation kind is 2 the formation stands 8
    // pixels further right and lower, with its sparks and flashes.
    alternate = 0;

    if (((struct Actor *)BattleObject_GetFar(args->targets[0])->object)
            ->spriteStorageKind == 2)
      alternate = 1;

    // Mode 1 on the far side mirrors everything sideways, and starts from
    // the caster.
    if (mode == DRAGON_FIRE_TIAMAT)
      dir = 1;
    else if (args->side == EFFECT_SIDE_PARTY)
      dir = -1;
    else
      dir = 1;
    if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
      EffectPosition_ProjectScaledBattler(args->user, &origin);
      origin.x /= 2;
      origin.y = 66;
      if (args->side == EFFECT_SIDE_PARTY)
        origin.x = 76;
      else
        origin.x = 44;
    }

    // The formation enters from above the right edge.
    if (alternate == 1) {
      focus.x = px(220);
      focus.y = -px(52);
    } else {
      focus.x = px(212);
      focus.y = -px(60);
    }
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    for (i = 0; i != 16; i++) {
      struct Particle *streak = &work->particles[24 + i];

      if (dir == 1)
        streak->x = (Random16() & 127) + 128;
      else
        streak->x = (Random16() & 127) - 128;
      streak->y = (Random16() & 7) - 72;
      streak->variant = -(Random16() & 31); // frames before it falls
    }
    for (i = 0; i != 512; i++)
      PARTICLES[i].variant = -1;
    if (mode == DRAGON_FIRE_TIAMAT) {
      BattleEffect_SelectLivingTargets(work->args);
      wait(1);
      BattleEffect_SpawnObjects(8, FORMATION_RESOURCE, 2);
    }

    for (frame = 0; frame != 208; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
        if (mode == DRAGON_FIRE_TIAMAT) {
          Sprite_StartAnimationFar(work->sprites[0], 8);
          Sprite_StartAnimationFar(work->sprites[1], 9);
          Sprite_StartAnimationFar(work->sprites[3], 10);
          Sprite_StartAnimationFar(work->sprites[4], 11);
        }
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                   POSE_DAMAGE, TINT_KEEP, 0);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        }
        frame = 160;
      }
      Transform_Reset();
      Transform_SetLookAt(camera, camera + 3);
      if (frame == 178)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

      // From frame 128 the trails halve; from 160 to 175 the canvas is filled
      // and the palette brightens; from 176 the canvas fades by 2 a frame.
      if (frame == 128)
        work->transferValue = 50;
      if (frame == 176) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet), 128);
      } else if (frame >= 160 && frame <= 175) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
        if (frame > 173)
          work->transferValue = CANVAS_FILL_WORD(63);
        Palette_BrightenBgEntries(2, 2, 2);
      }

      // Grains rise from y = 112: one a frame from 33, eight from 104.
      if (frame >= 33 && frame <= 175) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != 512; i++) {
          struct Particle *grain = &PARTICLES[i];

          if (grain->variant == -1) {
            grain->x = px((Random16() & 255) - 32);
            grain->y = px(112);
            grain->velocityX = ((Random16() & 127) + gTiamat_EmberSpeeds[i & 3])
                               << 9;
            grain->velocityY =
                -((Random16() & 127) + gTiamat_EmberSpeeds[i & 3]) << 11;
            grain->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up and out from the formation, one every other frame.
      if (frame >= 41 && frame <= 127) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != 24; i++) {
            struct Particle *spark = &work->particles[40 + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == DRAGON_FIRE_TIAMAT) {
                if (alternate == 1) {
                  spark->x = px((Random16() & 7) + 78);
                  spark->y = px(78);
                } else {
                  spark->x = px((Random16() & 7) + 78);
                  spark->y = px(70);
                }
              } else {
                spark->x = px((Random16() & 7) + origin.x - 8);
                spark->y = px(origin.y);
              }
              spark->velocityX = speed * Trig_Sin(angle) >> 9;
              spark->velocityY = speed * Trig_Cos(angle) >> 9;
              spark->variant = 0;
              spawned++;
              if (spawned == 1)
                break;
            }
          }
        }
      }
      if (frame == 48)
        Sound_PlayFar(SE_RUMBLE);
      if (frame == 128)
        Sound_PlayFar(SE_CRASH);

      // Flashes fly out sideways, one a frame from 129.
      if (frame >= 129 && frame <= 175) {
        s32 spawned = 0;

        for (i = 0; i != 24; i++) {
          struct Particle *flash = &work->particles[i];

          if (flash->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == DRAGON_FIRE_TIAMAT) {
              if (alternate == 1) {
                flash->x = px(72);
                flash->y = px(72);
              } else {
                flash->x = px(68);
                flash->y = px(64);
              }
            } else {
              flash->x = px(origin.x);
              flash->y = px(origin.y);
            }
            flash->velocityX = speed * Trig_Sin(angle) >> 6;
            flash->velocityY = speed * Trig_Cos(angle) >> 6;
            flash->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      // A flash steps its picture every four frames and lives 24.
      if (frame <= 175) {
        for (i = 0; i != 24; i++) {
          struct Particle *flash = &work->particles[i];

          if (flash->variant >= 0) {
            draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                    Q_16_16_UPPER_HALF(flash->x) - 16,
                    Q_16_16_UPPER_HALF(flash->y) - 32, 32, 64);
            flash->x += flash->velocityX * dir;
            flash->y += flash->velocityY;
            flash->variant++;
            if (flash->variant == 24)
              flash->variant = -1;
          }
        }
      }

      // A spark is two pixels, speeding upward, and lives 48 frames.
      for (i = 0; i != 24; i++) {
        struct Particle *spark = &work->particles[40 + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, gTiamat_SparkPixels, Q_16_16_UPPER_HALF(spark->x),
                  Q_16_16_UPPER_HALF(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -(px(1) / 64);
          spark->variant++;
          if (spark->variant == 48)
            spark->variant = -1;
        }
      }

      // Grains speed upward until frame 128, then bend away sideways (the odd
      // ones harder); each lives 256 frames.
      if (frame <= 175) {
        for (i = 0; i != 512; i++) {
          struct Particle *grain = &PARTICLES[i];

          if (grain->variant >= 0) {
            u8 size = gTiamat_EmberSizes[i & 3];
            s32 height = size * 2;

            draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    Q_16_16_UPPER_HALF(grain->x) - size / 2,
                    Q_16_16_UPPER_HALF(grain->y) - size, size, height);
            grain->x += grain->velocityX * dir;
            grain->y += grain->velocityY;
            if (frame > 128) {
              if (i & 1)
                grain->velocityX += -(px(1) / 2);
              else
                grain->velocityX += -(px(1) / 8);
            } else {
              grain->velocityY += -(px(1) / 64);
            }
            grain->variant++;
            if (grain->variant == 256)
              grain->variant = -1;
          }
        }
      }

      // The shakes, and the formation: it drops in over frames 40 to 47 and
      // turns to face the targets from 128 to 175.
      if (frame == 128)
        work->shakeFrames = 48;
      if (mode == DRAGON_FIRE_TIAMAT && frame == 48)
        work->shakeFrames = 8;
      if (frame >= 40 && frame <= 47) {
        focus.x += -px(8);
        focus.y += px(16);
      }
      if (mode == DRAGON_FIRE_TIAMAT) {
        if (frame == 128) {
          Sprite_StartAnimationFar(work->sprites[0], 8);
          Sprite_StartAnimationFar(work->sprites[1], 9);
          Sprite_StartAnimationFar(work->sprites[3], 10);
          Sprite_StartAnimationFar(work->sprites[4], 11);
        }
        if (frame == 176) {
          Sprite_StartAnimationFar(work->sprites[0], 0);
          Sprite_StartAnimationFar(work->sprites[1], 1);
          Sprite_StartAnimationFar(work->sprites[3], 3);
          Sprite_StartAnimationFar(work->sprites[4], 4);
        }
        if (alternate == 1)
          BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT_ALTERNATE,
                                             focus.x, focus.y);
        else
          BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT, focus.x,
                                             focus.y);
      }
      if (frame == 138) {
        for (i = 0; i != args->targetCount; i++) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                   POSE_DAMAGE, TINT_KEEP, 0);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        }
      }

      // A streak falls six pixels a frame after its wait, then below y = 55
      // splashes through six pictures, two frames each, over and over.
      if (frame <= 175) {
        for (i = 0; i != 16; i++) {
          struct Particle *streak = &work->particles[24 + i];

          if (streak->y > 55) {
            if (streak->variant >= 0 && streak->variant <= 11) {
              s32 cel = streak->variant / 2;

              draw[0](canvas,
                      work->sheet + gTiamat_FlamePictures[cel] + STREAK_SHEET,
                      streak->x - gTiamat_FlameWidths[cel] / 2,
                      streak->y + gTiamat_FlameDrops[cel],
                      gTiamat_FlameWidths[cel], gTiamat_FlameHeights[cel]);
            }
            streak->variant++;
            if (streak->variant == 12)
              streak->variant = 0;
          } else if (streak->variant == 0) {
            s32 size = 10;

            streak->x -= dir * 6;
            streak->y += 6;
            if (frame <= 47 && streak->y > 55)
              Sound_PlayFar(SE_WHOOSH);
            draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    streak->x - size / 2, streak->y + 30, size, size * 2);
          } else {
            streak->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }

    // Tear down: the formation bursts where it stands and is released.
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    if (mode == DRAGON_FIRE_TIAMAT) {
      if (alternate == 1)
        BattleEffect_RunImpactBurst(SUMMON_FORMATION_TIAMAT_ALTERNATE, focus.x,
                                    focus.y);
      else
        BattleEffect_RunImpactBurst(SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
    }
    if (mode == DRAGON_FIRE_TIAMAT) {
      for (i = 0; i != 8; i++)
        Sprite_DestroyFar(work->sprites[i]);
    }
  }
  BattleEffect_EndCanvasLayer();
}
