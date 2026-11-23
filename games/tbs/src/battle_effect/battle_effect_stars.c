/// @file
/// Astral Blast and Shining Star.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define SHINING_STAR_FRAMES 128 ///< frames of Shining Star
#define ASTRAL_BLAST_FRAMES 60  ///< frames of Astral Blast
#define ASTRAL_BLAST_STARS 3
#define SHINING_STARS 8

// Astral Blast flies three five-pointed stars at the target; Shining Star drops
// eight that bounce near it. Each outline's corners are projected onto the
// canvas and each edge stamped with motes.

/// A star outline's corners, its five points and the five between, and the
/// turn from one to the next, a tenth of a turn rounded up.
#define STAR_CORNERS 10
#define STAR_CORNER_ANGLE ((TRIG_TURN + STAR_CORNERS - 1) / STAR_CORNERS)

/// How fast the camera swings round, slowing from frame 40 to a stop.
#define ASTRAL_BLAST_TURN_SPEED 128
/// A new star leaves every 12 frames, spinning a 64th of a turn a frame.
#define ASTRAL_BLAST_STAR_INTERVAL 12
#define ASTRAL_BLAST_SPIN (TRIG_TURN / 64)
/// A star is drawn only while its farthest corner is no deeper than this.
#define ASTRAL_BLAST_DEPTH_MAX 399999
/// The motes stamped along each edge of a star.
#define ASTRAL_BLAST_EDGE_MOTES 16
#define SHINING_STAR_EDGE_MOTES 12
/// How far a Shining Star turns each frame, a 32nd of a turn.
#define SHINING_STAR_SPIN (TRIG_TURN / 32)

/// Three pink stars of light fly from above the user at the first target,
/// growing and spinning, while the camera swings round; each knocks the target
/// back.
///
/// Abilities: Astral Blast.
void BattleEffect_RunAstralBlast(struct BattleEffectParams *args) {
  struct ScreenPoint screenPosition;
#ifndef UBFIX
  struct ScreenPoint base;
#endif
  struct Vector3 scale;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  s32 i;
  u8 *motes;
  struct BattleCamera *camera;
  struct Actor *user;
  struct Actor *source;
  struct Actor *target;
  struct Particle *point;
  struct Particle *star;
  s32 start;
  s32 peak;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *corners;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  user = GetBattleObjectSlotFar(args->user)->actor;
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_RUNE_SHEET),
                  SHEET_PALETTE_BYTES);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      motes);
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, ANIM_STEP_NORMAL * 3);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  source = GetBattleObjectSlotFar(work->args->user)->actor;
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0, point = work->particles; i != ASTRAL_BLAST_STARS; i++) {
    SET_POSITION(point, source->x.fixed, source->y.fixed + toFIXED(40),
                 source->z.fixed);
    if (i == 0)
      point->velocityX = (target->x.fixed - point->x) / 12;
    else
      point->velocityX = (target->x.fixed * 2 - point->x) / 12;
    point->velocityY = (target->y.fixed - point->y + toFIXED(40)) / 12;
    point->velocityZ = (target->z.fixed - point->z) / 12;
    point->variant = 0;
    point++;
  }

  // Each star's ten corners on the canvas, kept in PARTICLES' velocities.
  corners = PARTICLES;
  for (frame = 0; frame != ASTRAL_BLAST_FRAMES; frame++) {
    if (frame <= 47) {
      struct BattleCamera *view = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
      s32 speed;

      if (frame <= 39)
        speed = ASTRAL_BLAST_TURN_SPEED;
      else
        speed = 768 - frame * 16;
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }
    for (i = 0; i != ASTRAL_BLAST_STARS; i++) {
      start = i * ASTRAL_BLAST_STAR_INTERVAL;
      if (frame >= start) {
        star = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > MOTE_SIZE_MAX)
          size = MOTE_SIZE_MAX;
        EFFECT_VIEW_FROM_CAMERA(camera);
        Matrix_TranslateVector(Particle_Coordinates(star));
        peak = 0;
        for (j = 0; j != STAR_CORNERS; j++) {
          s32 depth;

          Matrix_Push();
          Matrix_RotateZ((frame - start) * ASTRAL_BLAST_SPIN);
          Matrix_RotateY(DEGtoANG(90));
          scale.x = (frame - start) * (FIXED_ONE / 16) + FIXED_ONE / 16;
          if (scale.x > FIXED_ONE)
            scale.x = FIXED_ONE;
          scale.y = scale.x;
          scale.z = scale.x;
          Matrix_ScaleVector(Vector3_Coordinates(&scale));
          Matrix_RotateZ(j * STAR_CORNER_ANGLE);
          depth = Projection_Point(&gStar_Points[j & 1], &screenPosition);
          if (peak < depth)
            peak = depth;
          screenPosition.x >>= 1;
#ifndef UBFIX
          // UB: these first stores read base, which is never set; the next
          // two overwrite them.
          corners[i * STAR_CORNERS + j].velocityX = screenPosition.x + base.x;
          corners[i * STAR_CORNERS + j].velocityY = screenPosition.y + base.y;
#endif
          corners[i * STAR_CORNERS + j].velocityX = screenPosition.x;
          corners[i * STAR_CORNERS + j].velocityY = screenPosition.y;
          Matrix_Pop();
        }
        if (peak <= ASTRAL_BLAST_DEPTH_MAX) {
          for (j = 0; j != STAR_CORNERS; j++) {
            struct Particle *from =
                &((struct Particle *)PARTICLES)[j + i * STAR_CORNERS];
            struct Particle *to =
                &((struct Particle *)
                      PARTICLES)[(j + 1) % STAR_CORNERS + i * STAR_CORNERS];

            for (k = 0; k != ASTRAL_BLAST_EDGE_MOTES; k++) {
              s32 x = from->velocityX + (to->velocityX - from->velocityX) * k /
                                            ASTRAL_BLAST_EDGE_MOTES;
              s32 y = from->velocityY + (to->velocityY - from->velocityY) * k /
                                            ASTRAL_BLAST_EDGE_MOTES;

              EFFECT_DRAW_MOTE(draw[0], canvas, motes, x, y, size);
            }
          }
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
        // Each star knocks the target back and shakes the screen.
        if (frame == start + i + 10) {
          target->acceleration = toFIXED(2);
          target->speedLimit = toFIXED(8);
          target->velocityY = toFIXED(5);
          target->gravity = BATTLE_UNIT_GRAVITY;
          target->steeringFlags = 0;
          Actor_StartIdleFar(target);
          if (target->x.fixed < 0)
            Actor_SetMoveTargetFar(target, target->x.fixed - toFIXED(40), 0,
                                   target->z.fixed);
          else
            Actor_SetMoveTargetFar(target, target->x.fixed + toFIXED(40), 0,
                                   target->z.fixed);
          if (i == ASTRAL_BLAST_STARS - 1) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            Sound_PlayFar(SE_HEAVY_IMPACT);
            BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// Eight spinning star outlines fall from above the user and bounce on the
/// ground near the first target, each bounce flashing the targets.
///
/// Abilities: Shining Star.
void BattleEffect_RunShiningStar(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  s32 i;
  u8 *motes;
  struct BattleCamera *camera;
  struct Actor *source;
  struct Actor *target;
  struct Particle *point;
  struct Particle *star;
  struct ScreenPoint screenPosition;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *corners;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(RESOURCE_RUNE_SHEET),
                  SHEET_PALETTE_BYTES);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_PARTICLE_SPRITES_A),
                      motes);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = CANVAS_FADE_HALF;
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  source = GetBattleObjectSlotFar(work->args->user)->actor;
  target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0, point = work->particles; i != SHINING_STARS; i++) {
    s32 delay = i * 8;

    SET_POSITION(point, source->x.fixed / 2, source->y.fixed + toFIXED(120),
                 source->z.fixed);
    point->velocityX =
        (target->x.fixed + INTtoFIXED((s32)((Random16() & 0x7f) - 64)) -
         point->x) /
        12;
    point->velocityY = (target->y.fixed - point->y + toFIXED(20)) / 12;
    point->velocityZ = (target->z.fixed - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  /* LEFTOVER(template): size holds 2. */
  size = 2;
  corners = PARTICLES;
  for (frame = 0; frame != SHINING_STAR_FRAMES; frame++) {
    // The stars run through the colours of the rainbow.
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (i = 0; i != SHINING_STARS; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        Projection_Point(star, &screenPosition);
        screenPosition.x >>= 1;
        if (screenPosition.x >= -8 && screenPosition.x <= CANVAS_SIZE - 1 &&
            screenPosition.y <= CANVAS_SIZE - 1 && screenPosition.y >= -8) {
          for (j = 0; j != STAR_CORNERS; j++) {
            corners[i * STAR_CORNERS + j].velocityX =
                screenPosition.x +
                FIXEDtoINT(
                    (Trig_Sin(j * STAR_CORNER_ANGLE -
                              (frame - star->variant) * SHINING_STAR_SPIN) *
                     gStar_Radii[j & 1]) /
                    2);
            corners[i * STAR_CORNERS + j].velocityY =
                screenPosition.y - FIXEDtoINT(Trig_Cos(j * STAR_CORNER_ANGLE -
                                                       (frame - star->variant) *
                                                           SHINING_STAR_SPIN) *
                                              gStar_Radii[j & 1]);
          }
          for (j = 0; j != STAR_CORNERS; j++) {
            struct Particle *from = &corners[j + i * STAR_CORNERS];
            struct Particle *to =
                &corners[i * STAR_CORNERS + (j + 1) % STAR_CORNERS];

            for (k = 0; k != SHINING_STAR_EDGE_MOTES; k++) {
              s32 x = from->velocityX + (to->velocityX - from->velocityX) * k /
                                            SHINING_STAR_EDGE_MOTES;
              s32 y = from->velocityY + (to->velocityY - from->velocityY) * k /
                                            SHINING_STAR_EDGE_MOTES;

              EFFECT_DRAW_MOTE(draw[0], canvas, motes, x, y, size);
            }
          }
        }
        if (star->y <= toFIXED(30) - 1) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(SE_HEAVY_IMPACT);
          EFFECT_FOR_EACH_TARGET(j, work->args)
            BattleEffect_TintBattler(work->args->targets[j], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef SHINING_STAR_FRAMES
#undef ASTRAL_BLAST_FRAMES
#undef ASTRAL_BLAST_STARS
#undef SHINING_STARS
#undef STAR_CORNERS
#undef STAR_CORNER_ANGLE
#undef ASTRAL_BLAST_TURN_SPEED
#undef ASTRAL_BLAST_STAR_INTERVAL
#undef ASTRAL_BLAST_SPIN
#undef ASTRAL_BLAST_DEPTH_MAX
#undef ASTRAL_BLAST_EDGE_MOTES
#undef SHINING_STAR_EDGE_MOTES
#undef SHINING_STAR_SPIN
