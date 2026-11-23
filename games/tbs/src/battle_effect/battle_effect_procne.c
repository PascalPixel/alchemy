/// @file
/// The Procne summon.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define PROCNE_FRAMES 400 ///< frames of Procne
/// Procne appears in the violet sky, and the targets it lifted drop.
#define PROCNE_ARRIVAL_FRAME 160
#define PROCNE_DROP_FRAME 395
/// The first comet leaves Procne, and the next each 12 frames after.
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
  ((largest) - ((depth) - PROCNE_DEPTH_NEAR) /                                 \
                   ((PROCNE_DEPTH_FAR - PROCNE_DEPTH_NEAR) / ((largest) - 1)))

/// A whirlwind lifts the targets and spins them while dust swirls up it; then
/// Procne, a great bird in a violet sky, gathers sparks and strikes them with
/// four comets, each throwing out a burst of sparks. A or B skips to Procne's
/// arrival, then to the drop.
///
/// Abilities: Procne.
void BattleEffect_RunProcne(struct BattleEffectParams *args) {
  static struct SpriteScale unitScale = {{{FIXED_ONE, FIXED_ONE}}};
  void **slots;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *graphics;
  struct Point2D wave;
  struct Point2D drift;
  struct Actor *actor;
  struct BattleObject *battleObject;
  struct Particle *point;
  struct Particle *spark;
  struct Particle *comet;
  u8 *resource;
  u8 *image;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 arrived;
  s32 width = PROCNE_WHIRLWIND_WIDTH;
  s32 footHeight = PROCNE_WHIRLWIND_FOOT_HEIGHT;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  struct Vector3 world;
  struct ScreenPoint screen;
  struct Vector3 depthOffset;
  struct SpritePlacement place;
  struct SpriteScale scale;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = slots[0];
  camera = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_CAMERA);
  work =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
  graphics =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;

  BattleEffect_StartCanvasLayer(0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_CANVAS,
                                          HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_CANVAS,
                                          HEAP_SLOT_RAM_CODE_PAIRED);

  resource = Resource_GetTableEntry(RESOURCE_WHIRLWIND_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, work->sheet);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      graphics);

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  wave.x = toFIXED(128);
  wave.y = toFIXED(40);
  drift.x = 0;
  drift.y = 0;

  // Each target's place, for the whirlwind to turn it.
  EFFECT_FOR_EACH_TARGET(index, work->args) {
    battleObject = GetBattleObjectSlotFar(work->args->targets[index]);
    actor = battleObject->actor;
    home[index].x = actor->x.fixed;
    home[index].y = actor->z.fixed;
    baseAngle[index] = actor->facing;
    heading[index] = (u16)Trig_Atan(actor->x.fixed, actor->z.fixed);
    radius[index] = Iwram_Sqrt((actor->x.fixed >> 8) * (actor->x.fixed >> 8) +
                               (actor->z.fixed >> 8) * (actor->z.fixed >> 8)) >>
                    7;
    spin[index] = 0;
    actor->gravity = 0;
  }

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
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
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

    // The targets, one every 16 frames, spiral in toward the centre and rise,
    // spinning faster; each strike flashes them.
    EFFECT_FOR_EACH_TARGET(index, work->args) {
      if (frame > index * 16) {
        battleObject = GetBattleObjectSlotFar(work->args->targets[index]);
        actor = battleObject->actor;
        actor->x.fixed = (radius[index] * Trig_Sin(heading[index])) >> 1;
        actor->z.fixed = (radius[index] * Trig_Cos(heading[index])) >> 1;

        if (frame < PROCNE_ARRIVAL_FRAME) {
          if (frame > index * 16 + 16)
            spin[index] += PROCNE_SPIN_GAIN;
          if (radius[index] <= 31)
            actor->y.fixed += toFIXED(6);
          else
            actor->y.fixed += toFIXED(1) / 2;
          if (actor->y.fixed > toFIXED(124))
            actor->y.fixed = toFIXED(124);
        }

        // DEAD: always true, as the effect ends at frame 399; the ROM keeps
        // the test.
        if (frame <= 490) {
          heading[index] += spin[index];
          if (heading[index] > TRIG_TURN)
            heading[index] -= TRIG_TURN;
          actor->facing += spin[index] / 4;
        }

        if (frame == PROCNE_DROP_FRAME)
          actor->gravity = BATTLE_UNIT_GRAVITY;

        for (slot = 0; slot != PROCNE_STRIKES; slot++) {
          if (frame == gProcne_Frames[slot]) {
            actor->velocityY = 0;
            BattleEffect_TintBattler(work->args->targets[index],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, index, 8);
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
      if (size > footHeight)
        size = footHeight;
      draw[0](canvas,
              &work->sheet[frame / 4 % 3 * PROCNE_WHIRLWIND_CEL +
                           (footHeight - size) * PROCNE_WHIRLWIND_WIDTH],
              32, 112 - size, width, size);
    }

    if (frame >= 48 && frame < PROCNE_ARRIVAL_FRAME) {
      size = frame * 2 - 96;
      if (size > PROCNE_WHIRLWIND_HEIGHT)
        size = PROCNE_WHIRLWIND_HEIGHT;
      draw[0](canvas,
              &work->sheet[frame / 4 % 3 * PROCNE_WHIRLWIND_CEL +
                           (PROCNE_WHIRLWIND_HEIGHT - size) *
                               PROCNE_WHIRLWIND_WIDTH],
              32, 64 - size, width, size);
    }

    if (frame >= PROCNE_ARRIVAL_FRAME && frame < PROCNE_FRAMES) {
      image = work->sheet + frame / 4 % 3 * PROCNE_WHIRLWIND_CEL;
      draw[0](canvas, image, 8, 0, width, PROCNE_WHIRLWIND_HEIGHT);
      draw[0](canvas, image, 8, 64, width, PROCNE_WHIRLWIND_HEIGHT);
    }

    // The dust spirals in and up.
    if (frame < PROCNE_ARRIVAL_FRAME) {
      point = work->particles;
      for (index = 0; index != PROCNE_DUST_MOTES; index++) {
        if (point[index].variant == 0) {
          world.x = point[index].z * Trig_Sin(point[index].velocityY);
          world.y = point[index].y;
          world.z = point[index].z * Trig_Cos(point[index].velocityY);
          Projection_Point(&world.x, &screen);
          screen.x >>= 1;
          // DEAD: always true, as a mote restarts once past 48 pixels up and
          // climbs little more than 2 pixels a frame; the ROM keeps the test.
          if (point[index].y < toFIXED(64)) {
            draw[0](canvas, work->sheet + PROCNE_DUST, screen.x - 12,
                    screen.y - 12, 16, 16);
          }
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

      resource = Resource_GetTableEntry(RESOURCE_VIOLET_COMET_SHEET);
      Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
      resource += SHEET_PALETTE_BYTES;
      Decompress_TaggedLz(resource, work->sheet + PROCNE_COMET_PICTURES);

      for (index = 0; index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
           index++) {
        PARTICLES[index].variant = 0;
      }

      EFFECT_FOR_EACH_TARGET(index, work->args) {
        battleObject = GetBattleObjectSlotFar(work->args->targets[index]);
        actor = battleObject->actor;
        actor->y.fixed = INTtoFIXED((Random16() & 15) + 8);
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

      EFFECT_FOR_EACH_TARGET(index, work->args) {
        radius[index] = PROCNE_CORE_RADIUS;
        spin[index] = PROCNE_SKY_SPIN;
      }
    }

    // Procne sways, pushed by each comet.
    if (frame >= PROCNE_ARRIVAL_FRAME) {
      scale = unitScale;
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
        Actor_ApplyProjectedPlacementFar(work->sprites[index], &place, &scale,
                                         0);
      }

      if (frame > 255) {
        s32 launch;

        depthOffset.z = toFIXED(256);
        arrived = 0;
        depthOffset.x = 0;
        depthOffset.y = 0;
        Matrix_Initialize();
        Matrix_TranslateVector(Vector3_Coordinates(&depthOffset));

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
              Projection_Point(&point->x, &screen);
              // The canvas is shown at twice its width.
              screen.x =
                  (FIXEDtoINT(screen.x) >> 1) + (FIXEDtoINT(wave.x) >> 1) + 32;
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
            for (index = 0; index != PROCNE_STRIKES * PROCNE_STRIKE_SPARK_ROWS;
                 index++) {
              if (spark[index].variant > 0) {
                Projection_Point(&spark[index].x, &screen);
                screen.x = (FIXEDtoINT(screen.x) >> 1) + 32;
                screen.y = FIXEDtoINT(screen.y) + 56;
                screen.depth = FIXEDtoINT(screen.depth);
                CLAMP(screen.depth, PROCNE_DEPTH_NEAR, PROCNE_DEPTH_FAR);
                span = PROCNE_SPARK_SIZE(screen.depth, 3);
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
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
            comet->velocityX = (FIXEDtoINT(wave.x) >> 1) + 32;
            comet->velocityY = FIXEDtoINT(wave.y) - 4;

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
                world.x = 0;
                world.y = step->variant *
                          Trig_Cos(slot * (TRIG_TURN / PROCNE_RING_MOTES));
                world.z = step->variant *
                          Trig_Sin(slot * (TRIG_TURN / PROCNE_RING_MOTES));
                step->variant += 2;
                Projection_Point(&world.x, &screen);
                screen.x =
                    (FIXEDtoINT(screen.x) >> 1) +
                    work->particles[index + PROCNE_COMET_PARTICLES].velocityX;
                screen.y =
                    FIXEDtoINT(screen.y) +
                    work->particles[index + PROCNE_COMET_PARTICLES].velocityY;
                screen.depth = FIXEDtoINT(screen.depth);
                CLAMP(screen.depth, PROCNE_DEPTH_NEAR, PROCNE_DEPTH_FAR);
                span = PROCNE_SPARK_SIZE(screen.depth, 3);
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
              }
            }
          }
        }
      }
    }

    EFFECT_END_FRAME(work);
  }

  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

  // Put the targets back.
  EFFECT_FOR_EACH_TARGET(index, work->args) {
    struct Actor *actor;

    battleObject = GetBattleObjectSlotFar(work->args->targets[index]);
    actor = battleObject->actor;
    actor->x.fixed = home[index].x;
    actor->z.fixed = home[index].y;
    actor->facing = baseAngle[index];
  }

  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;

  BattleEffect_SetupBlendedDisplay();

  for (index = 0; index != PROCNE_SPRITES; index++) {
    Sprite_DestroyFar(work->sprites[index]);
  }

  EFFECT_FINISH_CANVAS();
}

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
