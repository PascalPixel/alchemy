/// @file
/// The Procne summon.
// Part of effect.c.

#include "../effect_internal.h"

/// The whirlwind sheet holds three 48x64 frames, then the dust; the violet
/// comets are decoded further on.
#define PROCNE_DUST_CELLS (3 * (48 * 64))
#define PROCNE_COMET_CELLS 0x3600

/// A whirlwind lifts the targets, then comets from a block of objects in a
/// violet sky strike them.
///
/// Abilities: Procne (SCENE_EFFECT_PROCNE).
///
/// The scene runs for 400 frames:
///   0 to 159   : a whirlwind rises over the field and lifts the targets,
///                spinning them round it and drawing them in, while dust
///                spirals up;
///   160        : the backdrop becomes the violet sky, and a 3x3 block of
///                nine objects appears and sways;
///   256 to 329 : motes gather into the middle of the block;
///   330 to 372 : four comets shoot out of it, 12 frames apart, each with a
///                ring of sparks, striking the targets on frames 334, 347,
///                359 and 372 (gProcne_Frames);
///   395        : the targets fall back to the ground.
/// Pressing A or B skips to frame 160, or from there to frame 395. At the
/// end the battle shows the damage and the targets go back to their places.
///
/// work->particles hold the dust (0 to 23 drawn), then from frame 160 the
/// gathering motes (0 to 31), the comets' rings (32 to 59) and the four
/// comets (60 to 63). PARTICLES 0 to 511 hold the sparks, 128 for each comet
/// of which 96 are thrown. The scene moves the targets' objects (position,
/// height, angle and gravity) and puts back their places and angles at the
/// end; it loads the whirlwind's and then the comets' palette into the first
/// 64 background colours, moves gProjection.centerX to 72 from frame 160 and
/// back to 120, and sets work->shakeFrames on each comet. It spawns the nine
/// objects of the block on frame 160 and releases them before closing the
/// canvas.
void BattleEffect_RunProcne(struct BattleEffectArgument *effect) {
  static struct SpriteScale unitScale = {{{0x10000, 0x10000}}};
  void **slots;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *graphics;
  struct Point2D wave;
  struct Point2D drift;
  struct MotionObject *object;
  struct BattleObj *battleObject;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *mark;
  u8 *resource;
  u8 *image;
  s32 index;
  s32 slot;
  s32 size;
  s32 span;
  s32 dist;
  s32 alive;
  s32 width;
  s32 heading[8];
  s32 radius[8];
  s32 spin[8];
  struct Point2D home[8];
  s32 baseAngle[8];
  struct Vector3 world;
  struct EffectPosition screen;
  struct Vector3 depthOffset;
  struct SpritePlacement place;
  struct SpriteScale scale;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = slots[0];
  camera = CANVAS_SLOT(slots, HEAP_SLOT_CAMERA);
  work = slots[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
  graphics = slots[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->effect = effect;

  BattleEffect_StartCanvasLayer(0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)
      slots[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_CANVAS];

  resource = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
  Iwram_CopyWords((void *)BG_PLTT, resource, 0x80);
  resource += 0x80;
  Resource_DecodeType01(resource, work->sheet);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

  // Trails keep half of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // wave is where the block of objects sways, from (128, 40); drift is a
  // push from each comet.
  wave.x = 0x800000;
  wave.y = 0x280000;
  drift.x = 0;
  drift.y = 0;

  // Remember each target's place, and its heading and distance from the
  // middle of the field for the whirlwind to turn. The distance is in half
  // pixels: the root of the 8.8 squares is 8.8, and >> 7 keeps one more bit.
  EFFECT_FOR_EACH_TARGET(index, work->effect) {
    battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
    object = battleObject->object;
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

  // The dust: each mote starts 120 out, after a random delay (variant). z
  // is its distance from the whirlwind's axis; velocityY is its heading
  // round the axis and, doubled, its rise; velocityX the turn added to it.
  // The draw before the delay is discarded but still advances the random
  // sequence.
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

  width = 48;
  frame = 0;
  do {
    // A or B skips ahead.
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
      if (frame <= 159)
        frame = 160;
      else if (frame <= 394)
        frame = 395;
    }

    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame == 16)
      AudioCommand_PlayFar(SE_141);
    if (frame == 256)
      AudioCommand_PlayFar(SE_140);
    if (frame == 334)
      AudioCommand_PlayFar(SE_212);
    if (frame == 347)
      AudioCommand_PlayFar(SE_212);
    if (frame == 359)
      AudioCommand_PlayFar(SE_212);
    if (frame == 372)
      AudioCommand_PlayFar(SE_212);

    // The targets, from 16i frames: spin faster round the middle and close
    // in, rising quickly once near it, until frame 160; they keep turning
    // until frame 490 (past the end), fall on frame 395 and flash on each
    // comet strike.
    EFFECT_FOR_EACH_TARGET(index, work->effect) {
      if (frame > index * 16) {
        battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
        object = battleObject->object;
        object->x = (radius[index] * Trig_Sin(heading[index])) >> 1;
        object->z = (radius[index] * Trig_Cos(heading[index])) >> 1;

        // The spin gains 48/65536 turn a frame each frame. Within 16 pixels
        // of the middle a target rises 6 pixels a frame, else half a pixel,
        // up to y 124.
        if (frame <= 159) {
          if (frame > index * 16 + 16)
            spin[index] += 48;
          if (radius[index] <= 31)
            object->y += 0x60000;
          else
            object->y += 0x8000;
          if (object->y > 0x7c0000)
            object->y = 0x7c0000;
        }

        if (frame <= 490) {
          heading[index] += spin[index];
          if (heading[index] > 0x10000)
            heading[index] -= 0x10000;
          object->angle += spin[index] / 4;
        }

        if (frame == 395)
          object->verticalMotionStrength = 0xab85;

        slot = 0;
        do {
          if (frame == gProcne_Frames[slot]) {
            object->velocityY = 0;
            ObjectGroup_UpdateMembers(work->effect->actors[index],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, index,
                                      8);
          }
          slot++;
        } while (slot != 4);

        // Closes in a pixel a frame, to 8 pixels from the middle.
        if (radius[index] > 16)
          radius[index] -= 2;
      }
    }

    // The whirlwind: three 48x64 images in turn, a new one every 4 frames.
    // Its lower section grows two rows a frame from frame 16, to its full 48
    // by frame 40; a second section above it grows the same way from frame
    // 48, to 64 rows by frame 80; both are drawn to frame 159, and from frame
    // 160 to 399 two full images stand further left.
    if ((u32)(frame - 16) <= 143) {
      size = frame * 2 - 32;
      if (size > 48)
        size = 48;
      draw[0](canvas,
              &work->sheet[frame / 4 % 3 * (48 * 64) + (width - size) * 48], 32,
              112 - size, width, size);
    }

    if ((u32)(frame - 48) <= 111) {
      size = frame * 2 - 96;
      if (size > 64)
        size = 64;
      draw[0](canvas,
              &work->sheet[frame / 4 % 3 * (48 * 64) + (64 - size) * 48], 32,
              64 - size, width, size);
    }

    if ((u32)(frame - 160) <= 239) {
      image = work->sheet + frame / 4 % 3 * (48 * 64);
      draw[0](canvas, image, 8, 0, width, 64);
      draw[0](canvas, image, 8, 64, width, 64);
    }

    // The dust (24 motes): spiral in and up, starting again from the
    // bottom when it passes 48 pixels up.
    if (frame <= 159) {
      index = 0;
      point = work->particles;
      do {
        if (point[index].variant == 0) {
          world.x = point[index].z * Trig_Sin(point[index].velocityY);
          world.y = point[index].y;
          world.z = point[index].z * Trig_Cos(point[index].velocityY);
          EffectPosition_ProjectPoint(&world.x, &screen);
          screen.x >>= 1;
          // Always true: a mote restarts once past 48 pixels up, and climbs
          // little more than 2 pixels a frame.
          if (point[index].y <= 0x3fffff) {
            draw[0](canvas, work->sheet + PROCNE_DUST_CELLS, screen.x - 12,
                    screen.y - 12, 16, 16);
          }
          if (point[index].z > 24)
            point[index].z -= 4;
          point[index].velocityY += point[index].velocityX * 2;
          if (point[index].velocityY > 0x10000)
            point[index].velocityY -= 0x10000;
          point[index].velocityX += 50;
          if (point[index].velocityX > 4096)
            point[index].velocityX = 4096;
          point[index].velocityY += 2048;
          point[index].y += point[index].velocityY * 2;
          if (point[index].y > 0x300000) {
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

    // Frame 160: the violet sky, the nine objects and the comet images; the
    // sparks are cleared, the targets hang 8 to 23 pixels up, the 32 motes
    // that will gather are scattered round the block, and the targets
    // settle into a tight, fast orbit.
    if (frame == 160) {
      s32 speed;
      s32 angle;

      BattleBackground_LoadFar(BATTLE_BACKGROUND_FLAT,
                               (s32)&ResourceId_VioletSkyBackdrop, 0);
      BattlePresentation_ConfigurePaletteFadeFar(
          BATTLE_BACKGROUND_FLAT, (s32)&ResourceId_VioletSkyBackdrop, 8);
      BattleEffect_SelectLivingTargets(work->effect);
      BattleEffect_SpawnObjects(9, 376, 2);

      resource = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
      Iwram_CopyWords((void *)BG_PLTT, resource, 0x80);
      resource += 0x80;
      Resource_DecodeType01(resource, work->sheet + PROCNE_COMET_CELLS);

      index = 0;
      do {
        PARTICLES[index].variant = 0;
        index++;
      } while (index != 512);

      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
        object = battleObject->object;
        object->y = ((Random16() & 15) + 8) << 16;
      }

      // The block and its comets are projected about a centre left of the
      // screen's.
      gProjection.centerX = 72;

      // Each mote starts up to 127 pixels out across and up, in a random
      // direction from 225 degrees round to 45 (the angle wraps), and up to
      // 100 pixels nearer or further.
      index = 0;
      do {
        speed = Random16() & 127;
        angle = (Random16() & 0x7fff) + 0x9fff;
        work->particles[index].x = Trig_Sin(angle) * speed;
        work->particles[index].y = Trig_Cos(angle) * speed;
        work->particles[index].z = (Random16() % 200 - 100) << 16;
        work->particles[index].variant = 0;
        index++;
      } while (index != 32);

      // 8 pixels out, turning 6000/65536 turn a frame, about a full turn
      // every 11 frames.
      EFFECT_FOR_EACH_TARGET(index, work->effect) {
        radius[index] = 16;
        spin[index] = 6000;
      }
    }

    // From frame 160: sway the block of objects, faster for its first 64
    // frames, plus what is left of each comet's push. Each frame it moves up
    // to half a pixel across, on a 512-frame swing, and half a pixel (a
    // quarter after the first 64 frames) up and down, on a 128-frame swing;
    // a push halves every frame.
    if (frame > 159) {
      scale = unitScale;
      size = 16;
      if (frame - 160 <= 64)
        size = 32;

      wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
      wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
      wave.x += drift.x;
      wave.y += drift.y;
      drift.x /= 2;
      drift.y /= 2;

      scale.x = 0x10000;
      scale.y = 0x10000;
      place.groundHeight = 0;
      // Object i is placed 64i pixels lower and raised 64i pixels higher,
      // which cancel on the screen but give it a draw slot 32i later
      // (Render_ApplyProjectedPlacement takes the slot from y / 2, and
      // Runtime_PushSlotEntry stops at 255).
      index = 0;
      do {
        place.x = (gProcne_GridX[index] << 16) + wave.x + 0x280000;
        place.height = 0xff0000 + index * 0x400000;
        place.y = ((gProcne_GridY[index] + index * 64) << 16) + wave.y;
        Object_ApplyProjectedPlacementFar(work->objects[index], &place, &scale,
                                          0);
        index++;
      } while (index != 9);

      // From frame 256 the motes, sparks and comets are drawn 256 pixels
      // into the scene.
      // XXX: likely none of these points projects, because each result is
      // read back as a 16.16 point (x >> 17 for the canvas, y and depth
      // >> 16): Render_ProjectPoint leaves the transformed point for a depth
      // outside gProjection's range, and from the reset transform these
      // points lie at depths near -256, below the battle's near depth.
      if (frame > 255) {
        s32 threshold;

        depthOffset.z = 0x1000000;
        alive = 0;
        depthOffset.x = 0;
        depthOffset.y = 0;
        Render_ResetTransformState();
        SceneTransform_ApplyPosition(Vector3_Coordinates(&depthOffset));

        // Frames 256 to 329: the motes close in on the middle of the
        // block two pixels a frame (dist is half the distance in pixels, the
        // root's 8.8 >> 9); those that arrive swell one mote there, a size
        // larger for every ten, and each is drawn smaller the deeper it lies.
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
              span = 6 - (screen.depth - 170) / 36;
              EFFECT_DRAW_MOTE(draw[1], canvas, graphics, screen.x, screen.y,
                               span);
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
            draw[1](canvas, graphics + gBattleEffect_MoteCells[span - 1],
                    (wave.x >> 17) - span / 2 + 32, (wave.y >> 16) - span - 4,
                    span, span * 2);
          }
        }

        // Each strike throws out 96 sparks.
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
              // Each spark lives 64 to 79 steps, 16 to 20 frames at the four
              // steps a frame below, in its comet's 128 map particles; the
              // speed just set is overwritten at once.
              spark->velocityY = ((Random16() & 255) - 128) << 10;
              SET_POSITION(spark, 0, 0, 0);
              spark->variant = (Random16() & 15) + 64;
              index++;
            } while (index != 96);
          }

          // Every live spark is drawn and moved once per comet slot, so the
          // sparks step four times a frame; z moves by velocityY as y does.
          if (frame >= gProcne_Frames[0]) {
            spark = PARTICLES;
            index = 0;
            do {
              if (spark[index].variant > 0) {
                EffectPosition_ProjectPoint(&spark[index].x, &screen);
                screen.x = (screen.x >> 17) + 32;
                screen.y = (screen.y >> 16) + 56;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - (screen.depth - 170) / 90;
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
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

        // The four comets, from frame 330 + 12i: each leaves the middle of
        // the block, flies down to the left growing through three images,
        // and spreads a ring of 28 motes for 8 frames; it pushes the block
        // and shakes the screen.
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

            drift.x = 0x80000;
            drift.y = -0x20000;
            work->shakeFrames = 8;
          }

          if (frame >= threshold) {
            s32 size;

            size = (frame - threshold) / 2;
            if (size > 2)
              size = 2;
            draw[1](canvas, work->sheet + gProcne_Cells[size], mark->x,
                    mark->y - gProcne_CellBiasY[size], gProcne_CellWidths[size],
                    gProcne_CellHeights[size]);
            mark->x -= 8;
            mark->y += 2;

            if (frame < threshold + 8) {
              // The ring is work->particles 32 to 59, each variant its radius,
              // growing 2 a frame round where the comet left the block (kept
              // in its mark's velocity); each comet turns the view further.
              // Its 28 motes lie 2340/65536 turn apart, a full circle.
              SceneTransform_ApplyRoll(-2048);
              SceneTransform_ApplyYaw(-4096);

              slot = 0;
              spark = &work->particles[32];
              do {
                struct EffectStep *step;

                step = &spark[slot];
                world.x = 0;
                world.y = step->variant * Trig_Cos(slot * 2340);
                world.z = step->variant * Trig_Sin(slot * 2340);
                step->variant += 2;
                EffectPosition_ProjectPoint(&world.x, &screen);
                screen.x =
                    (screen.x >> 17) + work->particles[index + 60].velocityX;
                screen.y =
                    (screen.y >> 16) + work->particles[index + 60].velocityY;
                screen.depth >>= 16;
                if (screen.depth <= 169)
                  screen.depth = 170;
                if (screen.depth > 350)
                  screen.depth = 350;
                span = 3 - (screen.depth - 170) / 90;
                EFFECT_DRAW_MOTE(draw[0], canvas, graphics, screen.x, screen.y,
                                 span);
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

  // The end.
  EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

  // Put the targets back.
  EFFECT_FOR_EACH_TARGET(index, work->effect) {
    struct MotionObject *obj;

    battleObject = GetBattleObjectSlotFar(work->effect->actors[index]);
    obj = battleObject->object;
    obj->x = home[index].x;
    obj->z = home[index].y;
    obj->angle = baseAngle[index];
  }

  gProjection.centerX = 120;

  BattleEffect_SetupBlendedDisplay();

  index = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[index]);
    index++;
  } while (index != 9);

  EFFECT_FINISH_CANVAS();
}

#undef PROCNE_DUST_CELLS
#undef PROCNE_COMET_CELLS
