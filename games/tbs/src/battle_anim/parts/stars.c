/// @file
/// Astral Blast and Shining Star.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// Astral Blast flies three ten-pointed rings at the target, knocking it back;
// Shining Star drops eight ten-pointed stars that bounce near the target.
// Each outline is drawn from ten corner points, a tenth of a turn (0x199a)
// apart, projected onto the canvas and stored in the velocity fields of
// PARTICLES, ten for each ring or star; each edge is sampled at evenly
// spaced points and a mote is stamped at each.

/// Three star-shaped rings of light fly from 40 pixels above the user at the
/// first target, 12 frames apart, growing and spinning as they go, while the
/// camera swings round; each knocks the target back on arrival, and the third
/// lets the battle show the damage.
///
/// Abilities: Astral Blast (BATTLE_ANIM_ASTRAL_BLAST).
///
/// work->particles 0 to 2 are the rings, battle positions in 16.16. Loads the
/// rune palette into the first 64 background colours, sets the user's
/// animation (2, at speed 48, three times the usual, and does not set it
/// back), turns the camera's yaw, sends the target back with its motion
/// fields and sets work->shakeFrames.
void BattleAnim_RunAstralBlast(struct BattleAnimArgs *effect) {
  struct ScreenPoint screenPosition;
#ifndef UBFIX
  struct ScreenPoint base;
#endif
  struct Vector3 scale;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH(dead): a blitter pair of which only draw[0] is used, the second
   * word filling four bytes of the frame; as one DrawRectangle, 24 of 490
   * instructions differ in tbs-en. */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct Object *caster;
  struct Object *source;
  struct Object *target;
  struct Particle *point;
  struct Particle *step;
  s32 start;
  s32 peak;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *trails;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  caster = GetBattleObjectSlotFar(effect->user)->object;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Decompress_Type01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                    graphics);
  Object_SelectAnimationFar(caster, POSE_LEAP);
  Object_SetAnimationSpeedFar((struct Object *)caster, 48);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  // Trails keep 75% of each pixel a frame.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75;
  draw[0] = (DrawRectangle) * (gHeapSlots + HEAP_SLOT_RAM_CODE);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);

  source = GetBattleObjectSlotFar(work->effect->user)->object;
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  // Each ring reaches the target in 12 frames; the second and third aim
  // twice as far across, past it.
  for (i = 0, point = work->particles; i != 3; i++) {
    SET_POSITION(point, source->x, source->y + 0x280000, source->z);
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  trails = PARTICLES;
  for (frame = 0; frame != 60; frame++) {
    // Swing the camera round, slowing over frames 40 to 47: 128/65536 turn a
    // frame, 5696/65536 in all, about a twelfth of a turn.
    if (frame <= 47) {
      struct BattleCamera *view = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (work->effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }
    for (i = 0; i != 3; i++) {
      start = i * 12;
      if (frame >= start) {
        step = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        ANIM_VIEW_FROM_CAMERA(camera);
        Transform_ApplyPosition(Particle_Coordinates(step));
        // The ring's ten points, alternately near and far from its centre,
        // projected onto the canvas; peak is the largest projection scale
        // (16.16), that of the point nearest the camera. The ring spins 1/64
        // turn a frame and grows from 1/16 of its size to full by 15 frames.
        peak = 0;
        for (j = 0; j != 10; j++) {
          s32 depth;

          Transform_Push();
          Transform_ApplyRoll((frame - start) << 10);
          Transform_ApplyYaw(0x4000);
          scale.x = ((frame - start) << 12) + 0x1000;
          if (scale.x > 0x10000)
            scale.x = 0x10000;
          scale.y = scale.x;
          scale.z = scale.x;
          Transform_ApplyScale(Vector3_Coordinates(&scale));
          Transform_ApplyRoll(j * 0x199a);
          depth = Projection_Point(&gStar_Points[j & 1], &screenPosition);
          if (peak < depth)
            peak = depth;
          screenPosition.x >>= 1;
#ifndef UBFIX
          // These first stores read base, which is never set; the next two
          // overwrite them. UBFIX leaves them out.
          trails[i * 10 + j].velocityX = screenPosition.x + base.x;
          trails[i * 10 + j].velocityY = screenPosition.y + base.y;
#endif
          trails[i * 10 + j].velocityX = screenPosition.x;
          trails[i * 10 + j].velocityY = screenPosition.y;
          Transform_Pop();
        }
        // Join the points with 16 motes each, unless a point projects at a
        // scale over 399999 (about 6.1), very near the camera.
        if (peak <= 399999) {
          for (j = 0; j != 10; j++) {
            struct Particle *from = &((struct Particle *)PARTICLES)[j + i * 10];
            struct Particle *to =
                &((struct Particle *)PARTICLES)[(j + 1) % 10 + i * 10];

            for (k = 0; k != 16; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              ANIM_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
            }
          }
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
        // Ring i hits 10 + i frames after its launch: knock the target 40
        // pixels back, away from the middle, and shake the screen. The third
        // ring lets the battle show the damage; the others flash the target.
        // The target hops at 5 pixels a frame under the usual gravity and
        // slides at up to 8 pixels a frame.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->steeringFlags = 0;
          Object_StartIdleFar((struct Object *)target);
          if (target->x < 0)
            Object_SetMoveTargetFar((struct Object *)target,
                                    target->x - 0x280000, 0, target->z);
          else
            Object_SetMoveTargetFar((struct Object *)target,
                                    target->x + 0x280000, 0, target->z);
          if (i == 2) {
            ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            Sound_PlaySongFar(SE_HEAVY_IMPACT);
            BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

/// Eight spinning star outlines fall from 120 pixels above the user towards the
/// first target, a few frames apart, and bounce off the ground near it, each
/// bounce shaking the screen and flashing the targets; the battle shows the
/// damage on frame 96.
///
/// Abilities: Shining Star (BATTLE_ANIM_SHINING_STAR).
///
/// It lasts 128 frames. work->particles 0 to 7 are the stars, battle positions
/// in 16.16, each variant the frame it starts. Rebuilds the background ramp
/// each frame (BattleAnim_UpdatePhasePalette, after loading the rune palette)
/// and sets work->shakeFrames at each bounce.
void BattleAnim_RunShiningStar(struct BattleAnimArgs *effect) {
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  /* FAKEMATCH(dead): a blitter pair of which only draw[0] is used, the second
   * word filling four bytes of the frame; as one DrawRectangle, 20 of 415
   * instructions differ in tbs-en. */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct Object *source;
  struct Object *target;
  struct Particle *point;
  struct Particle *step;
  struct ScreenPoint screenPosition;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *trails;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  graphics = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Decompress_Type01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                    graphics);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  // Trails keep half of each pixel a frame.
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 50;
  draw[0] = (DrawRectangle) * (gHeapSlots + HEAP_SLOT_RAM_CODE);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);

  source = GetBattleObjectSlotFar(work->effect->user)->object;
  target = GetBattleObjectSlotFar(work->effect->targets[0])->object;
  // Each star starts 120 pixels above the user, at half its x (towards the
  // middle of the field), aims at a random spot up to 64 pixels either side
  // of the target, 20 pixels up, over 12 frames, and starts 8i to 8i + 15
  // frames in (its variant).
  for (i = 0, point = work->particles; i != 8; i++) {
    s32 delay = i * 8;

    SET_POSITION(point, source->x / 2, source->y + 0x780000, source->z);
    // The signed -64..63 pixel offset scales without a negative left shift.
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) * 65536) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = 2;
  trails = PARTICLES;
  for (frame = 0; frame != 128; frame++) {
    // The three channels sway a third of a turn apart.
    BattleAnim_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (i = 0; i != 8; i++) {
      step = &work->particles[i];
      if (frame >= step->variant) {
        ANIM_VIEW_FROM_CAMERA(camera);
        Projection_Point(step, &screenPosition);
        screenPosition.x >>= 1;
        // On the canvas, draw the star: ten points, alternately near and
        // far, turning 1/32 turn a frame as it falls, joined by 12 motes
        // each; x is halved for the canvas.
        if (screenPosition.x >= -8 && screenPosition.x <= 127) {
          if (screenPosition.y <= 127) {
            if (screenPosition.y >= -8) {
              for (j = 0; j != 10; j++) {
                trails[i * 10 + j].velocityX =
                    screenPosition.x +
                    ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11)) *
                      gStar_Radii[j & 1]) /
                         2 >>
                     16);
                trails[i * 10 + j].velocityY =
                    screenPosition.y -
                    (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11)) *
                         gStar_Radii[j & 1] >>
                     16);
              }
              for (j = 0; j != 10; j++) {
                struct Particle *from = &trails[j + i * 10];
                struct Particle *to = &trails[i * 10 + (j + 1) % 10];

                for (k = 0; k != 12; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  ANIM_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
                }
              }
            }
          }
        }
        // Bounce below 30 pixels up, losing half the speed across. There is
        // no gravity: after a bounce a star rises as fast as it fell.
        if (step->y <= 0x1dffff) {
          step->velocityY = -step->velocityY;
          step->velocityX /= 2;
          step->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlaySongFar(SE_HEAVY_IMPACT);
          ANIM_FOR_EACH_TARGET(j, work->effect)
          BattleAnim_TintBattler(work->effect->targets[j], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, j, 8);
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}
