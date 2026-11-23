/// @file
/// The Procne summon, and the Tiamat summon and Dragon Driver.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "system/mul_fx.h"
#include "resource_ids.h"

/// Who breathes the dragon's fire (BattleEffect_RunDragonFire).
enum DragonFireMode {
  DRAGON_FIRE_TIAMAT,       ///< Tiamat lands beside the targets and breathes
                            ///< the fire, then bursts away
                            ///< (BattleEffect_RunImpactBurst)
  DRAGON_FIRE_DRAGON_DRIVER ///< no dragon is drawn: the breath starts just in
                            ///< front of the targets, in the LightningBolt
                            ///< colours
};

/// v % 3, through the multiply-high routine and the reciprocal of 3, sparing
/// a division.
#define MODULO_THREE(v) Math_UnsignedMulHighAdd((v), RECIPROCAL_OF(3), -3)

#define PROCNE_FRAMES 400 ///< frames of Procne
/// Procne appears in the violet sky, and the targets it lifted drop.
#define PROCNE_ARRIVAL_FRAME 160
#define PROCNE_DROP_FRAME 395
/// The first comet leaves Procne, and the next each PROCNE_COMET_INTERVAL
/// frames after.
#define PROCNE_COMET_FRAME 330
#define PROCNE_COMET_INTERVAL 12

#define PROCNE_SPRITES 9 ///< Procne's sprites, on gProcne_GridX and _GridY

/// The whirlwind sprite sheet: three whirlwind cels, shown 4 frames each, then
/// the dust.
#define PROCNE_WHIRLWIND_WIDTH 48
#define PROCNE_WHIRLWIND_HEIGHT 64
#define PROCNE_WHIRLWIND_CEL (PROCNE_WHIRLWIND_WIDTH * PROCNE_WHIRLWIND_HEIGHT)
/// The whirlwind rises in two pieces: its foot, a cel's upper rows, then a
/// whole cel above it.
#define PROCNE_WHIRLWIND_FOOT_HEIGHT 48
#define PROCNE_DUST (3 * PROCNE_WHIRLWIND_CEL)
#define PROCNE_COMET_PICTURES 13824

#define PROCNE_DUST_MOTES 24 ///< dust motes that swirl up the whirlwind
/// How much faster a dust mote circles the whirlwind each frame, up to
/// PROCNE_DUST_SPIN_MAX, in 1/65536 turns a frame.
#define PROCNE_DUST_SPIN_GAIN 50
#define PROCNE_DUST_SPIN_MAX (TRIG_TURN / 16)
/// The distance from the whirlwind's centre, in half pixels, that the lifted
/// targets spiral in to.
#define PROCNE_CORE_RADIUS 16
/// How much faster the lifted targets circle each frame, in 1/65536 turns a
/// frame, and how fast they circle once Procne arrives.
#define PROCNE_SPIN_GAIN 48
#define PROCNE_SKY_SPIN 6000

/// Procne's sway: sixty-fourths of the wave's sine, as 16.16 pixels a frame,
/// across and up and down; it bobs twice as far over its first 64 frames.
#define PROCNE_SWAY 32
#define PROCNE_BOB 16

/// Procne's particles in work->particles: the sparks that gather on it, the
/// ring of light each comet leaves, then one per comet.
#define PROCNE_GATHERING_SPARKS 32
#define PROCNE_RING PROCNE_GATHERING_SPARKS
#define PROCNE_RING_MOTES 28
#define PROCNE_COMET_PARTICLES (PROCNE_RING + PROCNE_RING_MOTES)

/// The four strikes, at gProcne_Frames: each throws out its sparks from its
/// own rows of PARTICLES.
#define PROCNE_STRIKES 4
#define PROCNE_STRIKE_SPARKS 96
#define PROCNE_STRIKE_SPARK_ROWS 128

/// The depths a spark is drawn between.
#define PROCNE_DEPTH_NEAR 170
#define PROCNE_DEPTH_FAR 350
/// A spark's mote size at `depth`: `largest` at PROCNE_DEPTH_NEAR, down to 1
/// at PROCNE_DEPTH_FAR.
#define PROCNE_SPARK_SIZE(depth, largest)                                      \
  ((largest) -                                                                 \
   Math_UnsignedMulHigh((depth) - PROCNE_DEPTH_NEAR,                           \
                        RECIPROCAL_OF((PROCNE_DEPTH_FAR - PROCNE_DEPTH_NEAR) / \
                                      ((largest) - 1))))

/// A whirlwind lifts the targets and spins them while dust swirls up it; then
/// Procne, a great bird in a violet sky, gathers sparks and strikes them with
/// four comets, each throwing out a burst of sparks. A or B skips to Procne's
/// arrival, then to the drop.
///
/// Abilities: Procne.
void BattleEffect_RunProcne(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *graphics;
  struct MotionObject *target;
  struct BattleObject *battleObject;
  struct Particle *point;
  struct Particle *spark;
  struct Particle *comet;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 arrived;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  FIXED world[3];
  struct ScreenPoint screen;
  FIXED depthOffset[3];
  struct SpritePlacement place;
  struct Scale scale;

  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  work = Ram_HeapSlots->battleEffect;
  graphics = Ram_HeapSlots->battleSheet;

  BattleEffect_StartCanvasLayer(0);
  BattleEffect_FetchRectangleBlitters(0, draw);
  {
    struct Point2D wave;
    struct Point2D drift;
    Resource_LoadAndDecompress(RESOURCE_WHIRLWIND_SHEET, work->sheet, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
    wave.x = toFIXED(128);
    wave.y = toFIXED(40);
    drift.x = 0;
    drift.y = 0;

    // Each target's place, for the whirlwind to turn it.
    EFFECT_FOR_EACH_TARGET(index, args) {
      battleObject = BattleObject_GetFar(args->targets[index]);
      target = battleObject->object;
      home[index].x = target->x;
      home[index].y = target->z;
      baseAngle[index] = target->angle;
      heading[index] = (u16)Trig_Atan(target->x, target->z);
      radius[index] = Iwram_Sqrt((target->x >> 8) * (target->x >> 8) +
                                 (target->z >> 8) * (target->z >> 8)) >>
                      7;
      spin[index] = 0;
      target->verticalMotionStrength = 0;
    }

    // The dust motes start at the whirlwind's foot, each after a wait of its
    // own.
    for (index = 0; index != ARRAY_COUNT(work->particles); index++) {
      point = &work->particles[index];
      point->z = 120;
      point->y = 0;
      Random16();
      point->velocityY = 0;
      point->velocityX = 0;
      point->variant = Random16() & 63;
    }

    for (frame = 0; frame != PROCNE_FRAMES; frame++) {
      if ((gInput.repeat & (KEY_A | KEY_B)) != 0) {
        if (frame < PROCNE_ARRIVAL_FRAME)
          frame = PROCNE_ARRIVAL_FRAME;
        else if (frame < PROCNE_DROP_FRAME)
          frame = PROCNE_DROP_FRAME;
      }

      EFFECT_VIEW_FROM_CAMERA(camera);

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

      // The targets, one every 16 frames, spiral in toward the centre and
      // rise, spinning faster; each strike flashes them.
      EFFECT_FOR_EACH_TARGET(index, args) {
        if (frame > index * 16) {
          battleObject = BattleObject_GetFar(args->targets[index]);
          target = battleObject->object;
          target->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
          target->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

          if (frame < PROCNE_ARRIVAL_FRAME) {
            if (frame > index * 16 + 16)
              spin[index] += PROCNE_SPIN_GAIN;
            if (radius[index] <= 31)
              target->y += toFIXED(6);
            else
              target->y += toFIXED(1) / 2;
            if (target->y > toFIXED(124))
              target->y = toFIXED(124);
          }

          // DEAD: always true, as the effect ends at frame 399; the ROM keeps
          // the test.
          if (frame <= 490) {
            heading[index] += spin[index];
            if (heading[index] > TRIG_TURN)
              heading[index] -= TRIG_TURN;
            target->angle += spin[index] / 4;
          }

          if (frame == PROCNE_DROP_FRAME)
            target->verticalMotionStrength = BATTLE_UNIT_GRAVITY;

          for (slot = 0; slot != PROCNE_STRIKES; slot++) {
            if (frame == gProcne_Frames[slot]) {
              target->velocityY = 0;
              BattleEffect_TintBattler(args->targets[index], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, index, 8);
            }
          }

          if (radius[index] > PROCNE_CORE_RADIUS)
            radius[index] -= 2;
        }
      }

      // The whirlwind's foot grows from frame 16, its top from 48; once Procne
      // arrives it stands two cels high.
      if (frame >= 16 && frame < PROCNE_ARRIVAL_FRAME) {
        size = frame * 2 - 32;
        if (size > PROCNE_WHIRLWIND_FOOT_HEIGHT)
          size = PROCNE_WHIRLWIND_FOOT_HEIGHT;
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * PROCNE_WHIRLWIND_CEL +
                             (PROCNE_WHIRLWIND_FOOT_HEIGHT - size) *
                                 PROCNE_WHIRLWIND_WIDTH],
                32, 112 - size, PROCNE_WHIRLWIND_WIDTH, size);
      }
      if (frame >= 48 && frame < PROCNE_ARRIVAL_FRAME) {
        size = frame * 2 - 96;
        if (size > PROCNE_WHIRLWIND_HEIGHT)
          size = PROCNE_WHIRLWIND_HEIGHT;
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * PROCNE_WHIRLWIND_CEL +
                             (PROCNE_WHIRLWIND_HEIGHT - size) *
                                 PROCNE_WHIRLWIND_WIDTH],
                32, 64 - size, PROCNE_WHIRLWIND_WIDTH, size);
      }
      if (frame >= PROCNE_ARRIVAL_FRAME && frame < PROCNE_FRAMES) {
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * PROCNE_WHIRLWIND_CEL], 8,
                0, PROCNE_WHIRLWIND_WIDTH, PROCNE_WHIRLWIND_HEIGHT);
        draw[0](canvas,
                &work->sheet[MODULO_THREE(frame / 4) * PROCNE_WHIRLWIND_CEL], 8,
                64, PROCNE_WHIRLWIND_WIDTH, PROCNE_WHIRLWIND_HEIGHT);
      }

      // The dust spirals in and up.
      if (frame < PROCNE_ARRIVAL_FRAME) {
        point = work->particles;
        for (index = 0; index != PROCNE_DUST_MOTES; index++) {
          if (point[index].variant == 0) {
            world[0] = point[index].z * Trig_Sin(point[index].velocityY);
            world[1] = point[index].y;
            world[2] = point[index].z * Trig_Cos(point[index].velocityY);
            EffectPosition_ProjectPoint(world, &screen);
            screen.x >>= 1;
            // DEAD: always true, as a mote restarts once past 48 pixels up and
            // climbs little more than 2 pixels a frame; the ROM keeps the test.
            if (point[index].y < toFIXED(64))
              draw[0](canvas, work->sheet + PROCNE_DUST, screen.x - 12,
                      screen.y - 12, 16, 16);
            if (point[index].z > 24)
              point[index].z -= 4;
            point[index].velocityY += point[index].velocityX * 2;
            if (point[index].velocityY > TRIG_TURN)
              point[index].velocityY -= TRIG_TURN;
            point[index].velocityX += PROCNE_DUST_SPIN_GAIN;
            if (point[index].velocityX > PROCNE_DUST_SPIN_MAX)
              point[index].velocityX = PROCNE_DUST_SPIN_MAX;
            point[index].velocityY += TRIG_TURN / 32;
            point[index].y += point[index].velocityY * 2;
            if (point[index].y > toFIXED(48)) {
              point[index].y = 0;
              point[index].z = 100;
              point[index].velocityY = 0;
              point[index].velocityX = 0;
            }
          } else {
            point[index].variant--;
          }
        }
      }

      if (frame == PROCNE_ARRIVAL_FRAME) {
        s32 speed;
        s32 angle;

        BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                                 RESOURCE_VIOLET_SKY_BACKDROP, 0);
        BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED,
                                    RESOURCE_VIOLET_SKY_BACKDROP, 8);
        BattleEffect_SelectLivingTargets(work->args);
        BattleEffect_SpawnObjects(PROCNE_SPRITES, SPRITE_PROCNE, 2);
        Resource_LoadAndDecompress(RESOURCE_VIOLET_COMET_SHEET,
                                   work->sheet + PROCNE_COMET_PICTURES, 1, 1);

        for (index = 0; index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
             index++) {
          PARTICLES[index].variant = 0;
        }

        EFFECT_FOR_EACH_TARGET(index, args) {
          battleObject = BattleObject_GetFar(args->targets[index]);
          target = battleObject->object;
          target->y = INTtoFIXED((Random16() & 15) + 8);
        }

        gProjection.centerX = 72;

        for (index = 0; index != PROCNE_GATHERING_SPARKS; index++) {
          speed = Random16() & 127;
          angle = (Random16() & 0x7fff) + DEGtoANG(225) - 1;
          work->particles[index].x = Trig_Sin(angle) * speed;
          work->particles[index].y = Trig_Cos(angle) * speed;
          work->particles[index].z = INTtoFIXED(Random16() % 200 - 100);
          work->particles[index].variant = 0;
        }

        EFFECT_FOR_EACH_TARGET(index, args) {
          radius[index] = PROCNE_CORE_RADIUS;
          spin[index] = PROCNE_SKY_SPIN;
        }
      }

      // Procne sways, pushed by each comet.
      if (frame >= PROCNE_ARRIVAL_FRAME) {
        scale = gProcne_UnitScale;
        size = PROCNE_BOB;
        if (frame - PROCNE_ARRIVAL_FRAME <= 64)
          size = PROCNE_BOB * 2;
        wave.x -=
            (Trig_Sin((frame - PROCNE_ARRIVAL_FRAME) << 7) * PROCNE_SWAY) >> 6;
        wave.y += (Trig_Sin((frame - PROCNE_ARRIVAL_FRAME) << 9) * size) >> 6;
        wave.x += drift.x;
        wave.y += drift.y;
        drift.x /= 2;
        drift.y /= 2;

        scale.x = FIXED_ONE;
        scale.y = FIXED_ONE;
        place.groundHeight = 0;
        // Sprite i stands lower and higher by the same amount, which cancel on
        // the screen but give it a later draw slot.
        for (index = 0; index != PROCNE_SPRITES; index++) {
          place.x = INTtoFIXED(gProcne_GridX[index]) + wave.x + toFIXED(40);
          place.height = toFIXED(255) + index * toFIXED(64);
          place.y = INTtoFIXED(gProcne_GridY[index] + index * 64) + wave.y;
          Render_ApplyProjectedPlacementFar(
              (struct Sprite *)work->sprites[index], &place.x, &scale.x, 0);
        }

        if (frame > 255) {
          s32 launch;

          depthOffset[2] = toFIXED(256);
          arrived = 0;
          depthOffset[0] = 0;
          depthOffset[1] = 0;
          Matrix_Initialize();
          Matrix_TranslateVector(depthOffset);

          // Until the first comet, the sparks gather on Procne, each a part of
          // its distance a frame; those that have arrived make one spark that
          // grows with their number.
          if (frame < PROCNE_COMET_FRAME) {
            for (index = 0; index != PROCNE_GATHERING_SPARKS; index++) {
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
                // The canvas is shown at twice its width.
                screen.x = (FIXEDtoINT(screen.x) >> 1) +
                           (FIXEDtoINT(wave.x) >> 1) + 32;
                screen.y = FIXEDtoINT(screen.y) + FIXEDtoINT(wave.y) - 4;
                screen.depth = FIXEDtoINT(screen.depth);
                CLAMP(screen.depth, PROCNE_DEPTH_NEAR, PROCNE_DEPTH_FAR);
                span = PROCNE_SPARK_SIZE(screen.depth, 6);
                EFFECT_DRAW_MOTE(draw[1], canvas, graphics, screen.x, screen.y,
                                 span);
                point->x -= point->x / dist;
                point->y -= point->y / dist;
                point->z -= point->z / dist;
              } else {
                arrived++;
              }
            }

            if (arrived > 0) {
              span = arrived / 10 + 1;
              draw[1](canvas, graphics + gBattleEffect_MotePictures[span - 1],
                      (FIXEDtoINT(wave.x) >> 1) - span / 2 + 32,
                      FIXEDtoINT(wave.y) - span - 4, span, span * 2);
            }
          }

          // Each strike throws out sparks.
          for (slot = 0; slot != PROCNE_STRIKES; slot++) {
            if (frame == gProcne_Frames[slot]) {
              s32 speed;
              s32 angle;

              for (index = 0; index != PROCNE_STRIKE_SPARKS; index++) {
                speed = (Random16() & 127) + 16;
                angle = Random16() & TRIG_ANGLE_MASK;
                spark = &PARTICLES[slot * PROCNE_STRIKE_SPARK_ROWS + index];
                spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
                spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
                // BUG: the random depth speed is stored over velocityY, not in
                // velocityZ as the other bursts store it, and z moves by
                // velocityY: the up-and-down speed just set is lost, and a
                // spark's depth moves with its height.
#ifdef BUGFIX
                spark->velocityZ = ((Random16() & 255) - 128) << 10;
#else
                spark->velocityY = ((Random16() & 255) - 128) << 10;
#endif
                SET_POSITION(spark, 0, 0, 0);
                spark->variant = (Random16() & 15) + 64;
              }
            }

            // Inside the strikes' loop, so the sparks move and fade four
            // steps a frame.
            if (frame >= gProcne_Frames[0]) {
              spark = PARTICLES;
              for (index = 0;
                   index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
                   index++) {
                if (spark[index].variant > 0) {
                  Iwram_TransformVector(&spark[index].x, &screen.x);
                  screen.x = (FIXEDtoINT(screen.x) >> 1) + 32;
                  screen.y = FIXEDtoINT(screen.y) + 56;
                  screen.depth = FIXEDtoINT(screen.depth);
                  CLAMP(screen.depth, PROCNE_DEPTH_NEAR, PROCNE_DEPTH_FAR);
                  span = PROCNE_SPARK_SIZE(screen.depth, 3);
                  EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x,
                                   screen.y, span);
                  spark[index].x += spark[index].velocityX;
                  spark[index].y += spark[index].velocityY;
#ifdef BUGFIX
                  spark[index].z += spark[index].velocityZ;
#else
                  spark[index].z += spark[index].velocityY;
#endif
                  spark[index].variant--;
                }
              }
            }
          }

          // The comets leave Procne one after another; each pushes it back
          // and shakes the screen.
          for (index = 0; index != PROCNE_STRIKES; index++) {
            comet = &work->particles[index + PROCNE_COMET_PARTICLES];
            launch = index * PROCNE_COMET_INTERVAL + PROCNE_COMET_FRAME;
            if (frame == launch) {
              comet->x = (FIXEDtoINT(wave.x) >> 1) + 32;
              comet->y = FIXEDtoINT(wave.y) - 4;
              // Where it leaves Procne, kept for the ring of light.
              comet->velocityX = comet->x;
              comet->velocityY = comet->y;

              for (slot = 0; slot != PROCNE_RING_MOTES; slot++) {
                work->particles[slot + PROCNE_RING].variant = 4;
              }

              drift.x = toFIXED(8);
              drift.y = -toFIXED(2);
              work->shakeFrames = 8;
            }

            if (frame >= launch) {
              s32 picture;

              // The comet grows over its first frames as it flies.
              picture = (frame - launch) / 2;
              if (picture > 2)
                picture = 2;
              draw[1](canvas, work->sheet + gProcne_Pictures[picture], comet->x,
                      comet->y - gProcne_PictureBiasY[picture],
                      gProcne_PictureWidths[picture],
                      gProcne_PictureHeights[picture]);
              comet->x -= 8;
              comet->y += 2;

              if (frame < launch + 8) {
                // A ring of light grows where the comet left Procne.
                Matrix_RotateZ(-DEGtoANG(11.25));
                Matrix_RotateY(-DEGtoANG(22.5));

                spark = &work->particles[PROCNE_RING];
                for (slot = 0; slot != PROCNE_RING_MOTES; slot++) {
                  struct Particle *step;

                  step = &spark[slot];
                  world[0] = 0;
                  world[1] = step->variant *
                             Trig_Cos(slot * (TRIG_TURN / PROCNE_RING_MOTES));
                  world[2] = step->variant *
                             Trig_Sin(slot * (TRIG_TURN / PROCNE_RING_MOTES));
                  step->variant += 2;
                  EffectPosition_ProjectPoint(world, &screen);
                  screen.x =
                      (FIXEDtoINT(screen.x) >> 1) +
                      work->particles[index + PROCNE_COMET_PARTICLES].velocityX;
                  screen.y =
                      FIXEDtoINT(screen.y) +
                      work->particles[index + PROCNE_COMET_PARTICLES].velocityY;
                  screen.depth = FIXEDtoINT(screen.depth);
                  CLAMP(screen.depth, PROCNE_DEPTH_NEAR, PROCNE_DEPTH_FAR);
                  span = PROCNE_SPARK_SIZE(screen.depth, 3);
                  EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x,
                                   screen.y, span);
                }
              }
            }
          }
        }
      }

      EFFECT_END_FRAME(work);
    }

    EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    EFFECT_FOR_EACH_TARGET(index, args) {
      struct MotionObject *battler;

      battleObject = BattleObject_GetFar(args->targets[index]);
      battler = battleObject->object;
      battler->x = home[index].x;
      battler->z = home[index].y;
      battler->angle = baseAngle[index];
    }
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    BattleEffect_SetupBlendedDisplay();

    for (index = 0; index != PROCNE_SPRITES; index++) {
      Sprite_DestroyFar(work->sprites[index]);
    }

    EFFECT_FINISH_CANVAS();
  }
}

#undef MODULO_THREE
#undef PROCNE_FRAMES
#undef PROCNE_ARRIVAL_FRAME
#undef PROCNE_DROP_FRAME
#undef PROCNE_COMET_FRAME
#undef PROCNE_COMET_INTERVAL
#undef PROCNE_SPRITES
#undef PROCNE_WHIRLWIND_WIDTH
#undef PROCNE_WHIRLWIND_HEIGHT
#undef PROCNE_WHIRLWIND_CEL
#undef PROCNE_WHIRLWIND_FOOT_HEIGHT
#undef PROCNE_DUST
#undef PROCNE_COMET_PICTURES
#undef PROCNE_DUST_MOTES
#undef PROCNE_DUST_SPIN_GAIN
#undef PROCNE_DUST_SPIN_MAX
#undef PROCNE_CORE_RADIUS
#undef PROCNE_SPIN_GAIN
#undef PROCNE_SKY_SPIN
#undef PROCNE_SWAY
#undef PROCNE_BOB
#undef PROCNE_GATHERING_SPARKS
#undef PROCNE_RING
#undef PROCNE_RING_MOTES
#undef PROCNE_COMET_PARTICLES
#undef PROCNE_STRIKES
#undef PROCNE_STRIKE_SPARKS
#undef PROCNE_STRIKE_SPARK_ROWS
#undef PROCNE_DEPTH_NEAR
#undef PROCNE_DEPTH_FAR
#undef PROCNE_SPARK_SIZE

/// Fireballs rain on the targets and a blast of fire sweeps across them, in
/// the LightningBolt colours.
///
/// Abilities: Dragon Driver.
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_DRAGON_DRIVER);
}

/// Tiamat lands while fireballs rain on the targets, breathes fire across
/// them, and bursts away.
///
/// Abilities: Tiamat.
void BattleEffect_RunTiamat(struct BattleEffectParams *args) {
  BattleEffect_RunDragonFire(args, DRAGON_FIRE_TIAMAT);
}

#define DRAGON_FIRE_FRAMES 208
/// The breath of fire, then the flash that ends it.
#define DRAGON_FIRE_BREATH_FRAME 128
#define DRAGON_FIRE_FLASH_FRAME 160
#define DRAGON_FIRE_FLASH_END_FRAME 176
#define DRAGON_FIRE_FIREBALLS 16      ///< falling fireballs
#define DRAGON_FIRE_FIRST_FIREBALL 24 ///< the first in work->particles
#define DRAGON_FIRE_FIRST_SPARK 40    ///< the first spark in work->particles
#define DRAGON_FIRE_EMBERS 512        ///< embers in PARTICLES
#define DRAGON_FIRE_BREATH 24 ///< puffs of breath, work->particles 0 to 23
#define DRAGON_FIRE_SPARKS 24 ///< sparks, work->particles 40 to 63
/// The flames a fireball burns with where it lands, in the sprite sheet.
#define DRAGON_FIRE_FLAME_PICTURES 12288
/// A puff of breath shows the flash burst's six pictures, four frames each.
#define DRAGON_FIRE_PUFF_PICTURES 6
#define DRAGON_FIRE_PUFF_FRAMES 4
/// A landed fireball's flame burns through its six pictures, two frames each,
/// over and over.
#define DRAGON_FIRE_BURN_PICTURES 6
#define DRAGON_FIRE_BURN_FRAMES 2
#define DRAGON_FIRE_SPARK_LIFE 48  ///< frames a spark flies
#define DRAGON_FIRE_EMBER_LIFE 256 ///< frames an ember rises
/// The level the canvas is cleared to each frame of the flash, until its last
/// two frames turn it CANVAS_LEVEL_BRIGHTEST.
#define DRAGON_FIRE_FLASH_LEVEL 16

/// Tiamat's pieces (work->sprites), each showing its own picture of the sprite
/// sheet: piece n shows animation n. As it breathes, its head, horns, jaw and
/// throat show the fire-lit pictures of its open mouth.
enum TiamatPiece {
  TIAMAT_HEAD = 0,
  TIAMAT_HORNS = 1,
  TIAMAT_JAW = 3,
  TIAMAT_THROAT = 4,
};
/// The animations of Tiamat's pieces as it breathes fire, the fire-lit pictures
/// of its open mouth.
enum TiamatAnimation {
  TIAMAT_HEAD_BREATHING = 8,
  TIAMAT_HORNS_BREATHING,
  TIAMAT_JAW_BREATHING,
  TIAMAT_THROAT_BREATHING,
};

/// Fireballs rain on the targets and burn where they land while embers rise
/// from the ground; then a blast of fire sweeps across them and the screen
/// flares white. Against a two-part enemy Tiamat comes at its own size and
/// stands 8 pixels further right and lower, with its sparks and breath.
void BattleEffect_RunDragonFire(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint origin;
  BlitterPair draw;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  work = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;

  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
                             work->sheet + DRAGON_FIRE_FLAME_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
    LOAD_BG_PALETTE(RESOURCE_LIGHTNING_BOLT_SHEET);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  {
    struct Point2D focus;
    s32 dir;
    s32 twoPartTarget;

    twoPartTarget = 0;
    if (BattleObject_GetFar(args->targets[0])->object->recordStorageKind ==
        ACTOR_SPRITE_GROUP)
      twoPartTarget = 1;

    if (mode == DRAGON_FIRE_TIAMAT)
      dir = 1;
    else if (args->side == EFFECT_SIDE_PARTY)
      dir = -1;
    else
      dir = 1;
    if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
      EffectPosition_ProjectScaledBattler(args->user, &origin);
      // DEAD: the halved x is replaced below; the ROM keeps the halving and
      // its store.
      origin.x /= 2;
      origin.y = 66;
      if (args->side == EFFECT_SIDE_PARTY)
        origin.x = 76;
      else
        origin.x = 44;
    }

    if (twoPartTarget == 1) {
      focus.x = toFIXED(220);
      focus.y = -toFIXED(52);
    } else {
      focus.x = toFIXED(212);
      focus.y = -toFIXED(60);
    }
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
      struct Particle *fireball =
          &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

      if (dir == 1)
        fireball->x = (Random16() & 127) + 128;
      else
        fireball->x = (Random16() & 127) - 128;
      fireball->y = (Random16() & 7) - 72;
      fireball->variant = -(Random16() & 31);
    }
    EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
    if (mode == DRAGON_FIRE_TIAMAT) {
      BattleEffect_SelectLivingTargets(work->args);
      wait(1);
      BattleEffect_SpawnObjects(8, SPRITE_TIAMAT, 2);
    }

    for (frame = 0; frame != DRAGON_FIRE_FRAMES; frame++) {
      // A or B skips to the flash.
      if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 48 &&
          frame < DRAGON_FIRE_FLASH_FRAME) {
        // Tiamat's open jaws of the breath and the targets' reel of frame 138.
        if (mode == DRAGON_FIRE_TIAMAT) {
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HEAD],
                                   TIAMAT_HEAD_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HORNS],
                                   TIAMAT_HORNS_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_JAW],
                                   TIAMAT_JAW_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_THROAT],
                                   TIAMAT_THROAT_BREATHING);
        }
        EFFECT_FOR_EACH_TARGET(i, args) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                   POSE_DAMAGE, TINT_KEEP, 0);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        }
        frame = DRAGON_FIRE_FLASH_FRAME;
      }
      EFFECT_VIEW_FROM_CAMERA(camera);
      if (frame == 178)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

      if (frame == DRAGON_FIRE_BREATH_FRAME)
        work->transferValue = CANVAS_FADE_HALF;
      if (frame == DRAGON_FIRE_FLASH_END_FRAME) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry(RESOURCE_EMBER_STREAK_SHEET),
            SHEET_PALETTE_BYTES);
      } else if (frame >= DRAGON_FIRE_FLASH_FRAME &&
                 frame < DRAGON_FIRE_FLASH_END_FRAME) {
        EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR,
                            CANVAS_FILL_WORD(DRAGON_FIRE_FLASH_LEVEL));
        if (frame > 173)
          work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
        Palette_BrightenBgEntries(2, 2, 2);
      }

      // Embers rise from the ground.
      if (frame >= 33 && frame < DRAGON_FIRE_FLASH_END_FRAME) {
        s32 spawned = 0;
        s32 limit = 1;

        if (frame > 103)
          limit = 8;
        for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
          struct Particle *ember = &PARTICLES[i];

          if (ember->variant == -1) {
            ember->x = INTtoFIXED((Random16() & 255) - 32);
            ember->y = toFIXED(112);
            ember->velocityX = ((Random16() & 127) + gTiamat_EmberSpeeds[i & 3])
                               << 9;
            ember->velocityY =
                -((Random16() & 127) + gTiamat_EmberSpeeds[i & 3]) << 11;
            ember->variant = 0;
            spawned++;
            if (spawned == limit)
              break;
          }
        }
      }

      // Sparks fly up from Tiamat's head.
      if (frame >= 41 && frame < DRAGON_FIRE_BREATH_FRAME) {
        s32 spawned = 0;

        if (frame & 1) {
          for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
            struct Particle *spark =
                &work->particles[DRAGON_FIRE_FIRST_SPARK + i];

            if (spark->variant == -1) {
              s32 speed = (Random16() & 255) + 128;
              s32 angle = (Random16() & 0x1fff) + 20000;

              if (mode == DRAGON_FIRE_TIAMAT) {
                if (twoPartTarget == 1) {
                  spark->x = INTtoFIXED((Random16() & 7) + 78);
                  spark->y = toFIXED(78);
                } else {
                  spark->x = INTtoFIXED((Random16() & 7) + 78);
                  spark->y = toFIXED(70);
                }
              } else {
                spark->x = INTtoFIXED((Random16() & 7) + origin.x - 8);
                spark->y = INTtoFIXED(origin.y);
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
      if (frame == DRAGON_FIRE_BREATH_FRAME)
        Sound_PlayFar(SE_CRASH);

      // Breath: puffs of fire thrown across the targets.
      if (frame > DRAGON_FIRE_BREATH_FRAME &&
          frame < DRAGON_FIRE_FLASH_END_FRAME) {
        s32 spawned = 0;

        for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
          struct Particle *puff = &work->particles[i];

          if (puff->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) - 20000;

            if (mode == DRAGON_FIRE_TIAMAT) {
              if (twoPartTarget == 1) {
                puff->x = toFIXED(72);
                puff->y = toFIXED(72);
              } else {
                puff->x = toFIXED(68);
                puff->y = toFIXED(64);
              }
            } else {
              puff->x = INTtoFIXED(origin.x);
              puff->y = INTtoFIXED(origin.y);
            }
            puff->velocityX = speed * Trig_Sin(angle) >> 6;
            puff->velocityY = speed * Trig_Cos(angle) >> 6;
            puff->variant = 0;
            spawned++;
            if (spawned == 1)
              break;
          }
        }
      }

      if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
        for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
          struct Particle *puff = &work->particles[i];

          if (puff->variant >= 0) {
            draw[0](canvas,
                    work->sheet +
                        (puff->variant >> 2) * PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(puff->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(puff->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            puff->x += puff->velocityX * dir;
            puff->y += puff->velocityY;
            puff->variant++;
            if (puff->variant ==
                DRAGON_FIRE_PUFF_PICTURES * DRAGON_FIRE_PUFF_FRAMES)
              puff->variant = -1;
          }
        }
      }

      for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
        struct Particle *spark = &work->particles[DRAGON_FIRE_FIRST_SPARK + i];
        s32 size = 1;

        if (spark->variant >= 0) {
          draw[1](canvas, gTiamat_SparkPixels, FIXED_INTEGER_PART(spark->x),
                  FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
          spark->x += spark->velocityX * dir;
          spark->y += spark->velocityY;
          spark->velocityY += -toFIXED(1) / 64;
          spark->variant++;
          if (spark->variant == DRAGON_FIRE_SPARK_LIFE)
            spark->variant = -1;
        }
      }

      // Embers rise, then curve away.
      if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
        for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
          struct Particle *ember = &PARTICLES[i];

          if (ember->variant >= 0) {
            u8 size = gTiamat_EmberSizes[i & 3];

            draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    FIXED_INTEGER_PART(ember->x) - size / 2,
                    FIXED_INTEGER_PART(ember->y) - size, size, size * 2);
            ember->x += ember->velocityX * dir;
            ember->y += ember->velocityY;
            if (frame > DRAGON_FIRE_BREATH_FRAME) {
              if (i & 1)
                ember->velocityX += -toFIXED(1) / 2;
              else
                ember->velocityX += -toFIXED(1) / 8;
            } else {
              ember->velocityY += -toFIXED(1) / 64;
            }
            ember->variant++;
            if (ember->variant == DRAGON_FIRE_EMBER_LIFE)
              ember->variant = -1;
          }
        }
      }

      if (frame == DRAGON_FIRE_BREATH_FRAME)
        work->shakeFrames = 48;
      if (mode == DRAGON_FIRE_TIAMAT && frame == 48)
        work->shakeFrames = 8;

      // Tiamat swoops down.
      if (frame >= 40 && frame <= 47) {
        focus.x += -toFIXED(8);
        focus.y += toFIXED(16);
      }
      if (mode == DRAGON_FIRE_TIAMAT) {
        if (frame == DRAGON_FIRE_BREATH_FRAME) {
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HEAD],
                                   TIAMAT_HEAD_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HORNS],
                                   TIAMAT_HORNS_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_JAW],
                                   TIAMAT_JAW_BREATHING);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_THROAT],
                                   TIAMAT_THROAT_BREATHING);
        }
        if (frame == DRAGON_FIRE_FLASH_END_FRAME) {
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HEAD], TIAMAT_HEAD);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_HORNS], TIAMAT_HORNS);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_JAW], TIAMAT_JAW);
          Sprite_StartAnimationFar(work->sprites[TIAMAT_THROAT], TIAMAT_THROAT);
        }
        if (twoPartTarget == 1)
          BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT_ALTERNATE,
                                             focus.x, focus.y);
        else
          BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT, focus.x,
                                             focus.y);
      }
      if (frame == 138) {
        EFFECT_FOR_EACH_TARGET(i, args) {
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_RED_TINT,
                                   POSE_DAMAGE, TINT_KEEP, 0);
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        }
      }

      if (frame < DRAGON_FIRE_FLASH_END_FRAME) {
        for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
          struct Particle *fireball =
              &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

          if (fireball->y > 55) {
            if (fireball->variant >= 0 &&
                fireball->variant <
                    DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_FRAMES) {
              s32 picture = fireball->variant / DRAGON_FIRE_BURN_FRAMES;

              draw[0](canvas,
                      work->sheet + gTiamat_FlamePictures[picture] +
                          DRAGON_FIRE_FLAME_PICTURES,
                      fireball->x - gTiamat_FlameWidths[picture] / 2,
                      fireball->y + gTiamat_FlameDrops[picture],
                      gTiamat_FlameWidths[picture],
                      gTiamat_FlameHeights[picture]);
            }
            fireball->variant++;
            if (fireball->variant ==
                DRAGON_FIRE_BURN_PICTURES * DRAGON_FIRE_BURN_FRAMES)
              fireball->variant = 0;
          } else if (fireball->variant == 0) {
            s32 size = MOTE_PICTURES;

            fireball->x -= dir * 6;
            fireball->y += 6;
            if (frame <= 47 && fireball->y > 55)
              Sound_PlayFar(SE_WHOOSH);
            draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                    fireball->x - size / 2, fireball->y + 30, size, size * 2);
          } else {
            fireball->variant++;
          }
        }
      }
      BattleCamera_Shake(8, 8);
      EFFECT_END_FRAME(work);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    if (mode == DRAGON_FIRE_TIAMAT) {
      if (twoPartTarget == 1)
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

#undef DRAGON_FIRE_FRAMES
#undef DRAGON_FIRE_BREATH_FRAME
#undef DRAGON_FIRE_FLASH_FRAME
#undef DRAGON_FIRE_FLASH_END_FRAME
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_PICTURES
#undef DRAGON_FIRE_PUFF_PICTURES
#undef DRAGON_FIRE_PUFF_FRAMES
#undef DRAGON_FIRE_BURN_PICTURES
#undef DRAGON_FIRE_BURN_FRAMES
#undef DRAGON_FIRE_SPARK_LIFE
#undef DRAGON_FIRE_EMBER_LIFE
#undef DRAGON_FIRE_FLASH_LEVEL
