/// @file
/// Astral Blast and Shining Star.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"

#define SHINING_STAR_FRAMES 128 ///< frames of Shining Star
#define ASTRAL_BLAST_FRAMES 60  ///< frames of Astral Blast

// Astral Blast flies three five-pointed stars at the target; Shining Star drops
// eight that bounce near it. Each outline's corners are projected onto the
// canvas and each edge stamped with motes.

/// A star outline's corners, its five points and the five between, and the
/// turn from one to the next, a tenth of a turn rounded up.
#define STAR_CORNERS 10
#define STAR_CORNER_ANGLE ((TRIG_TURN + STAR_CORNERS - 1) / STAR_CORNERS)

/// How fast the camera swings round, in 1/65536 turns a frame, slowing from
/// frame 40 to a stop.
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
  FIXED scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  s32 i;
  u8 *graphics;
  struct BattleCamera *camera;
  struct Actor *caster;
  struct Actor *source;
  struct Actor *target;
  struct Particle *point;
  struct Particle *step;
  s32 start;
  s32 peak;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *trails;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  caster = (struct Actor *)BattleObject_GetFar(args->user)->object;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_RUNE_SHEET),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  Actor_SelectAnimationFar(caster, POSE_LEAP);
  Actor_SetAnimationSpeedFar(caster, ANIM_STEP_NORMAL * 3);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  // The stars leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = (struct Actor *)BattleObject_GetFar(args->user)->object;
  target = (struct Actor *)BattleObject_GetFar(args->targets[0])->object;
  for (i = 0, point = work->particles; i != 3; i++) {
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

  trails = (struct Particle *)gMapGridCellBuffer;
  for (frame = 0; frame != ASTRAL_BLAST_FRAMES; frame++) {
    if (frame <= 47) {
      s32 speed;

      if (frame <= 39)
        speed = ASTRAL_BLAST_TURN_SPEED;
      else
        speed = 768 - frame * 16;
      if (args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }

    for (i = 0; i != 3; i++) {
      start = i * ASTRAL_BLAST_STAR_INTERVAL;
      if (frame >= start) {
        step = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > MOTE_SIZE_MAX)
          size = MOTE_SIZE_MAX;
        EFFECT_VIEW_FROM_CAMERA(camera);
        Matrix_TranslateVector(Particle_Coordinates(step));
        peak = 0;
        for (j = 0; j != STAR_CORNERS; j++) {
          s32 depth;

          Matrix_Push();
          Matrix_RotateZ((frame - start) * ASTRAL_BLAST_SPIN);
          Matrix_RotateY(DEGtoANG(90));
          scale[0] = (frame - start) * (FIXED_ONE / 16) + FIXED_ONE / 16;
          if (scale[0] > FIXED_ONE)
            scale[0] = FIXED_ONE;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Matrix_ScaleVector(scale);
          Matrix_RotateZ(j * STAR_CORNER_ANGLE);
          depth = EffectPosition_ProjectPoint(&gStar_Points[(j & 1) * 3],
                                              &screenPosition);
          if (peak < depth)
            peak = depth;
          screenPosition.x >>= 1;
#ifndef UBFIX
          // UB: these first stores read base, which is never set; the next
          // two overwrite them.
          trails[i * STAR_CORNERS + j].velocityX = screenPosition.x + base.x;
          trails[i * STAR_CORNERS + j].velocityY = screenPosition.y + base.y;
#endif
          trails[i * STAR_CORNERS + j].velocityX = screenPosition.x;
          trails[i * STAR_CORNERS + j].velocityY = screenPosition.y;
          Matrix_Pop();
        }
        if (peak <= ASTRAL_BLAST_DEPTH_MAX) {
          for (j = 0; j != STAR_CORNERS; j++) {
            struct Particle *from =
                &((struct Particle *)gMapGridCellBuffer)[j + i * STAR_CORNERS];
            struct Particle *to = &(
                (struct Particle *)gMapGridCellBuffer)[(j + 1) % STAR_CORNERS +
                                                       i * STAR_CORNERS];

            for (k = 0; k != ASTRAL_BLAST_EDGE_MOTES; k++) {
              s32 x = from->velocityX + (to->velocityX - from->velocityX) * k /
                                            ASTRAL_BLAST_EDGE_MOTES;
              s32 y = from->velocityY + (to->velocityY - from->velocityY) * k /
                                            ASTRAL_BLAST_EDGE_MOTES;

              EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
            }
          }
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
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
          if (i == 2) {
            EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
          } else {
            Sound_PlayFar(SE_HEAVY_IMPACT);
            BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
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
  u8 *graphics;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct Particle *point;
  struct Particle *step;
  struct ScreenPoint screenPosition;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *trails;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_RUNE_SHEET),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, graphics, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = BattleObject_GetFar(args->user)->object;
  target = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0, point = work->particles; i != 8; i++) {
    s32 delay = i * 8;

    SET_POSITION(point, source->x / 2, source->y + toFIXED(120), source->z);
    point->velocityX =
        (target->x + INTtoFIXED((s32)((Random16() & 0x7f) - 64)) - point->x) /
        12;
    point->velocityY = (target->y - point->y + toFIXED(20)) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  /* LEFTOVER(template): size holds 2. */
  size = 2;
  trails = (struct Particle *)gMapGridCellBuffer;
  for (frame = 0; frame != SHINING_STAR_FRAMES; frame++) {
    // The stars run through the colours of the rainbow.
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
                                    DEGtoANG(120), 0);
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (i = 0; i != 8; i++) {
      step = &work->particles[i];
      if (frame >= step->variant) {
        EFFECT_VIEW_FROM_CAMERA(camera);
        EffectPosition_ProjectPoint(step, &screenPosition);
        screenPosition.x >>= 1;
        if (screenPosition.x >= -8 && screenPosition.x <= CANVAS_SIZE - 1 &&
            screenPosition.y <= CANVAS_SIZE - 1 && screenPosition.y >= -8) {
          for (j = 0; j != STAR_CORNERS; j++) {
            trails[i * STAR_CORNERS + j].velocityX =
                screenPosition.x +
                FIXEDtoINT(
                    (Trig_Sin(j * STAR_CORNER_ANGLE -
                              (frame - step->variant) * SHINING_STAR_SPIN) *
                     gStar_Radii[j & 1]) /
                    2);
            trails[i * STAR_CORNERS + j].velocityY =
                screenPosition.y - FIXEDtoINT(Trig_Cos(j * STAR_CORNER_ANGLE -
                                                       (frame - step->variant) *
                                                           SHINING_STAR_SPIN) *
                                              gStar_Radii[j & 1]);
          }
          for (j = 0; j != STAR_CORNERS; j++) {
            struct Particle *from = &trails[j + i * STAR_CORNERS];
            struct Particle *to =
                &trails[i * STAR_CORNERS + (j + 1) % STAR_CORNERS];

            for (k = 0; k != SHINING_STAR_EDGE_MOTES; k++) {
              s32 x = from->velocityX + (to->velocityX - from->velocityX) * k /
                                            SHINING_STAR_EDGE_MOTES;
              s32 y = from->velocityY + (to->velocityY - from->velocityY) * k /
                                            SHINING_STAR_EDGE_MOTES;

              EFFECT_DRAW_MOTE(draw[0], canvas, graphics, x, y, size);
            }
          }
        }
        // At the ground the star bounces up at half its sideways speed.
        if (step->y <= toFIXED(30) - 1) {
          step->velocityY = -step->velocityY;
          step->velocityX /= 2;
          step->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(SE_HEAVY_IMPACT);
          for (j = 0; j != args->targetCount; j++)
            BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, j, 8);
        }
        step->x += step->velocityX;
        step->y += step->velocityY;
        step->z += step->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
#undef SHINING_STAR_FRAMES
#undef ASTRAL_BLAST_FRAMES
#undef STAR_CORNERS
#undef STAR_CORNER_ANGLE
#undef ASTRAL_BLAST_TURN_SPEED
#undef ASTRAL_BLAST_STAR_INTERVAL
#undef ASTRAL_BLAST_SPIN
#undef ASTRAL_BLAST_DEPTH_MAX
#undef ASTRAL_BLAST_EDGE_MOTES
#undef SHINING_STAR_EDGE_MOTES
#undef SHINING_STAR_SPIN
