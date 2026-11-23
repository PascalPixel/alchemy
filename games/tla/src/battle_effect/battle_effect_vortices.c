/// @file
/// Froth and Whirlwind.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define WHIRLWIND_FRAMES 80 ///< frames of the tornadoes

/// The vortex's three halves, then the bubble picture.
#define FROTH_VORTEX_HALF_WIDTH 16
#define FROTH_VORTEX_HALF_HEIGHT 64
#define FROTH_BUBBLE_PICTURE_WIDTH 12
#define FROTH_BUBBLE_PICTURE_HEIGHT 24
#define FROTH_BUBBLE_PICTURE (3 * PICTURE_BYTES(FROTH_VORTEX_HALF))
/// The bubbles set out in the effect's particles, of which each variant flies
/// its FROTH_BUBBLE_COUNT; the targets' places follow from
/// FROTH_FIRST_TARGET.
#define FROTH_BUBBLE_SLOTS 30
#define FROTH_FIRST_TARGET 32
/// The depths a bubble's projection is held between.
#define FROTH_BUBBLE_DEPTH_NEAR 160
#define FROTH_BUBBLE_DEPTH_FAR 799
/// The frames a vortex turns where a bubble landed.
#define FROTH_VORTEX_FRAMES 16

/// The glints that sparkle on the tornadoes, four on each.
#define WHIRLWIND_GLINTS 16
/// The frames a glint lives: three pictures, two frames each.
#define WHIRLWIND_GLINT_LIFE 6
/// The sweep sways this far either way, a sway every 64 frames, and each
/// tornado turns round it every 32.
#define WHIRLWIND_SWEEP_SWAY 16

/// The tornado pictures, after the six glint pictures in the sprite sheet.
#define WHIRLWIND_TORNADO_PICTURES GLINT_SET_BYTES
#define WHIRLWIND_SECTION_BYTES PICTURE_BYTES(TORNADO_STRIP)

/// Bubbles fly out of the top of the user, hang, and home in on the targets;
/// each that lands bursts into a swirling vortex, knocking back its target,
/// while the camera swings round.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral.
void BattleEffect_RunFroth(struct BattleEffectParams *args) {
  struct ScreenPoint position;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  struct MotionObject *source;
  struct MotionObject *object;
  struct Particle *point;
  struct Particle *seat;
  struct Particle *target;
  struct BattleCamera *camera;
  s32 frame;
  s32 y;
  s32 i;
  s8 *landed;

  // Set up: the canvas layer, the vortex sprite sheet and two blitters, the one
  // on the right mirrored. landed is nonzero once a bubble has struck.
  landed = (s8 *)Ram_MapCellBuffer;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_VORTEX_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  // The bubbles start at the user's chest with random velocities.
  source = BattleObject_GetFar(args->user)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(args->user);
  for (i = 0; i != FROTH_BUBBLE_SLOTS; i++) {
    point = &work->particles[i];
    SET_POSITION(point, source->x, y, source->z);
    SET_VELOCITY(point, INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5,
                 INTtoFIXED((s32)((Random16() & 127) - 16)) >> 6,
                 INTtoFIXED((s32)((Random16() & 255) - 127)) >> 5);
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  EFFECT_FOR_EACH_TARGET(i, args) {
    seat = &work->particles[FROTH_FIRST_TARGET + i];
    object = BattleObject_GetFar(args->targets[i])->object;
    SET_POSITION(seat, object->x, 0, object->z);
  }

  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SCATTER);

  for (frame = 0; frame != gVortices_Counts[args->variant][FROTH_FRAMES];
       frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = TRIG_TURN * 3 / 512;
    else
      work->cameraDelta = 0;
    if (frame == gVortices_Counts[args->variant][FROTH_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);

    for (i = 0; i != gVortices_Counts[args->variant][FROTH_BUBBLE_COUNT]; i++) {
      point = &work->particles[i];

      // A bubble leaves the user two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &position);
        position.x >>= 1;
        // DEAD: the clamped depth is never read; the ROM keeps the clamp.
        CLAMP(position.depth, FROTH_BUBBLE_DEPTH_NEAR, FROTH_BUBBLE_DEPTH_FAR);
        draw[0](canvas, work->sheet + FROTH_BUBBLE_PICTURE,
                position.x - FROTH_BUBBLE_PICTURE_WIDTH / 2,
                position.y - FROTH_BUBBLE_PICTURE_HEIGHT / 2,
                FROTH_BUBBLE_PICTURE_WIDTH, FROTH_BUBBLE_PICTURE_HEIGHT);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[FROTH_FIRST_TARGET + i % args->targetCount];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          SET_VELOCITY(point, point->velocityX * 60 / DAMPING_SCALE,
                       point->velocityY * 60 / DAMPING_SCALE,
                       point->velocityZ * 60 / DAMPING_SCALE);
        }
        // On the ground it strikes.
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = position.x;
          point->y = position.y + (Random16() & 31) - 16;
          BattleEffect_TintBattler(args->targets[i % args->targetCount],
                                   SPRITE_COLOR_WHITE, POSE_DAMAGE,
                                   i % args->targetCount, 4);
          BattleMotion_ApplyKnockbackFar(args->targets[i % args->targetCount],
                                         KNOCKBACK_NUDGE);
          work->shakeFrames = 4;
          Sound_PlayFar(SE_WATER);
        }
      }

      // The vortex where a bubble landed: three pictures, two frames each,
      // drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < FROTH_VORTEX_FRAMES) {
        draw[0](canvas,
                work->sheet +
                    (point->variant / 2) % 3 * PICTURE_BYTES(FROTH_VORTEX_HALF),
                point->x - FROTH_VORTEX_HALF_WIDTH, point->y - 56,
                FROTH_VORTEX_HALF_WIDTH, FROTH_VORTEX_HALF_HEIGHT);
        draw[1](canvas,
                work->sheet +
                    (point->variant / 2) % 3 * PICTURE_BYTES(FROTH_VORTEX_HALF),
                point->x, point->y - 56, FROTH_VORTEX_HALF_WIDTH,
                FROTH_VORTEX_HALF_HEIGHT);
        point->variant++;
      }
    }

    BattleCamera_Shake(args->variant * 2 + 2, args->variant * 2 + 2);
    if (work->cameraPhase == CAMERA_PHASE_IDLE)
      work->cameraPhase = CAMERA_PHASE_FRAME_DONE;
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

/// Tornadoes sweep across the targets, sparkling with glints, and the targets
/// reel again and again.
///
/// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane.
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args) {
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  FIXED wave;
  struct Particle *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 picture;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sprite sheet and two max blitters,
  // the second mirrored.
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != WHIRLWIND_GLINTS; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); // invisible to start with
  wait(1);
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != WHIRLWIND_FRAMES; frame++) {
    wave = Trig_Sin(frame * (TRIG_TURN / 64)) * WHIRLWIND_SWEEP_SWAY;
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The canvas flashes every eight frames from frame 16.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
    }

    // The sweep sways from side to side over the targets; it fades in over
    // 16 frames and out over the last 16.
    if (args->side == EFFECT_SIDE_PARTY)
      wave += toFIXED(32);
    else
      wave -= toFIXED(32);
    if (frame <= 16)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame);
    if (frame > 63)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(79 - frame);

    for (i = 0; i != gVortices_Shapes[args->variant][WHIRLWIND_TORNADOES];
         i++) {
      // Each tornado circles its own phase, a quarter turn from the next.
      x = FIXEDtoINT(gVortices_Shapes[args->variant][WHIRLWIND_SWAY] *
                         Trig_Sin(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) +
                     wave) +
          40;
      y = FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32) + i * DEGtoANG(90)) * 2);
      segment = (frame / 2) % 3;
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  WHIRLWIND_TORNADO_PICTURES,
              x, y + 16, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              work->sheet + segment * WHIRLWIND_SECTION_BYTES +
                  (WHIRLWIND_TORNADO_PICTURES + 6 * WHIRLWIND_SECTION_BYTES),
              x, y + 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
      draw[0](canvas,
              work->sheet + segment * (2 * WHIRLWIND_SECTION_BYTES) +
                  (WHIRLWIND_TORNADO_PICTURES + WHIRLWIND_SECTION_BYTES),
              x, y + 80, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);

      // Its four glints, the second of each pair mirrored; each restarts at
      // a new place when its pictures run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          picture = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas,
                      work->sheet + gBattleEffect_GlintPictures[picture],
                      glint->x + x, glint->y + y,
                      gBattleEffect_GlintPictureWidths[picture],
                      gBattleEffect_GlintPictureHeights[picture]);
        }
        glint->variant++;
        if (glint->variant == WHIRLWIND_GLINT_LIFE) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    EFFECT_FOR_EACH_TARGET(j, args) {
      for (i = 0; i != 7; i++) {
        if (frame == i * 8 + j * 3 + 16) {
          EFFECT_STRIKE_TARGET(args, j, 4, KNOCKBACK_BOUNCE);
        }
      }
    }

    work->shakeFrames = 1;
    BattleCamera_Shake(gVortices_Shapes[args->variant][WHIRLWIND_SHAKE],
                       gVortices_Shapes[args->variant][WHIRLWIND_SHAKE] * 2);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef WHIRLWIND_FRAMES
#undef FROTH_VORTEX_HALF_WIDTH
#undef FROTH_VORTEX_HALF_HEIGHT
#undef FROTH_BUBBLE_PICTURE_WIDTH
#undef FROTH_BUBBLE_PICTURE_HEIGHT
#undef FROTH_BUBBLE_PICTURE
#undef FROTH_BUBBLE_SLOTS
#undef FROTH_FIRST_TARGET
#undef FROTH_BUBBLE_DEPTH_NEAR
#undef FROTH_BUBBLE_DEPTH_FAR
#undef FROTH_VORTEX_FRAMES
#undef WHIRLWIND_GLINTS
#undef WHIRLWIND_GLINT_LIFE
#undef WHIRLWIND_SWEEP_SWAY
#undef WHIRLWIND_TORNADO_PICTURES
#undef WHIRLWIND_SECTION_BYTES
