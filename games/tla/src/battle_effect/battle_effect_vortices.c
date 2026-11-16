/// @file
/// Froth and Whirlwind.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define MOTE_COUNT 30 ///< bubbles set out, the most a variant flies
#define SEAT_FIRST 32 ///< the particles that hold each target's seat

/// The vortex's three halves, then the bubble picture.
#define FROTH_VORTEX_HALF_WIDTH 16
#define FROTH_VORTEX_HALF_HEIGHT 64
#define FROTH_BUBBLE_PICTURE_WIDTH 12
#define FROTH_BUBBLE_PICTURE_HEIGHT 24
#define FROTH_BUBBLE_PICTURE (3 * PICTURE_BYTES(FROTH_VORTEX_HALF))

/// Bubbles fly out of the top of the user, hang, and home in on the targets;
/// each that lands bursts into a swirling vortex, knocking back its target,
/// while the camera swings round.
///
/// Abilities: Froth, Froth Sphere, Froth Spiral.
void BattleEffect_RunFroth(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
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

  // Set up: the canvas layer, the vortex sheet and two blitters, the one on
  // the right mirrored.
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

  // The motes start at the caster's chest with random velocities.
  source = BattleObject_GetFar(args->user)->object;
  y = source->y + BattleUnit_GetEffectHeightFar(args->user);
  for (i = 0; i != MOTE_COUNT; i++) {
    point = &work->particles[i];
    SET_POSITION(point, source->x, y, source->z);
    point->velocityX = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->velocityY = ((s32)((Random16() & 127) - 16) << 16) >> 6;
    point->velocityZ = ((s32)((Random16() & 255) - 127) << 16) >> 5;
    point->variant = -1;
    landed[i] = 0;
  }

  // Each target's seat on the ground.
  for (i = 0; i != args->targetCount; i++) {
    seat = &work->particles[SEAT_FIRST + i];
    object = BattleObject_GetFar(args->targets[i])->object;
    SET_POSITION(seat, object->x, 0, object->z);
  }

  work->cameraDelta = 0;
  work->cameraPhase = CAMERA_PHASE_IDLE;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_FRAME);
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
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    for (i = 0; i != gVortices_Counts[args->variant][FROTH_BUBBLE_COUNT]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        CLAMP(pos.depth, 160, 799);
        draw[0](canvas, work->sheet + FROTH_BUBBLE_PICTURE,
                pos.x - FROTH_BUBBLE_PICTURE_WIDTH / 2,
                pos.y - FROTH_BUBBLE_PICTURE_HEIGHT / 2,
                FROTH_BUBBLE_PICTURE_WIDTH, FROTH_BUBBLE_PICTURE_HEIGHT);
        point->x += point->velocityX;
        point->y += point->velocityY;
        point->z += point->velocityZ;
      }

      // After 48 frames it steers toward its target's seat, damped for the
      // first 37 of them, and lands.
      if (frame > i * 2 + 48 && landed[i] == 0) {
        target = &work->particles[SEAT_FIRST + i % args->targetCount];
        point->velocityX += (target->x - point->x) >> 9;
        point->velocityY += (target->y - point->y) >> 9;
        point->velocityZ += (target->z - point->z) >> 9;
        if (frame < i * 2 + 85) {
          point->velocityX = point->velocityX * 60 / DAMPING_SCALE;
          point->velocityY = point->velocityY * 60 / DAMPING_SCALE;
          point->velocityZ = point->velocityZ * 60 / DAMPING_SCALE;
        }
        if (point->y < 0) {
          landed[i] = 1;
          point->variant = 0;
          point->x = pos.x;
          point->y = pos.y + (Random16() & 31) - 16;
          BattleEffect_TintBattler(args->targets[i % args->targetCount],
                                   SPRITE_COLOR_WHITE, POSE_DAMAGE,
                                   i % args->targetCount, 4);
          BattleMotion_ApplyKnockbackFar(args->targets[i % args->targetCount],
                                         KNOCKBACK_NUDGE);
          work->shakeFrames = 4;
          Sound_PlayFar(SE_WATER);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - FROTH_VORTEX_HALF_WIDTH, point->y - 56,
                FROTH_VORTEX_HALF_WIDTH, FROTH_VORTEX_HALF_HEIGHT);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
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

#define WHIRLWIND_FRAMES 80 ///< frames of the tornadoes
#define GLINT_COUNT 16      ///< four glints for each of up to four funnels
#define GLINT_CELS 6        ///< three cels of two frames each

/// The tornado pictures, after the six glint pictures in the sheet.
#define WHIRLWIND_TORNADO_PICTURES GLINT_SET_BYTES
#define WHIRLWIND_SECTION_BYTES PICTURE_BYTES(TORNADO_STRIP)

/// Tornadoes sweep across the targets, sparkling with glints, and the targets
/// reel again and again.
///
/// Abilities: Whirlwind, Tornado, Tempest, Gale, Typhoon, Hurricane.
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 wave;
  struct Particle *glint;
  s32 frame;
  s32 x;
  s32 y;
  s32 segment;
  s32 cell;
  s32 i;
  s32 j;

  // Set up: the canvas layer, the tornado sheet and two max blitters, the
  // second mirrored.
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
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
  wait(1);
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != WHIRLWIND_FRAMES; frame++) {
    wave = Trig_Sin(frame << 10) << 4;
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // Wipe the canvas every eight frames from frame 16, leaving a trail.
    for (i = 0; i != 7; i++) {
      if (frame == i * 8 + 16)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(8));
    }

    // Sway toward the targets' side; fade in over 16 frames and out over the
    // last 16.
    if (args->side == EFFECT_SIDE_PARTY)
      wave += toFIXED(32);
    else
      wave -= toFIXED(32);
    if (frame <= 16)
      REG_BLDALPHA = BLEND_ALPHA(frame, BLEND_WEIGHT_MAX);
    if (frame > 63)
      REG_BLDALPHA = BLEND_ALPHA(79 - frame, BLEND_WEIGHT_MAX);

    for (i = 0; i != gVortices_Shapes[args->variant][WHIRLWIND_TORNADOES];
         i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = (FIXEDtoINT(gVortices_Shapes[args->variant][WHIRLWIND_SWAY] *
                          Trig_Sin((frame << 11) + i * (DEGtoANG(90))) +
                      wave)) +
          40;
      y = FIXEDtoINT(Trig_Cos((frame << 11) + i * (DEGtoANG(90))) * 2);
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
      // a new place when its cels run out.
      for (j = 0; j != 4; j++) {
        glint = &work->particles[i * 4 + j];
        if (glint->variant >= 0) {
          cell = glint->variant / 2 + (j / 2) * 3;
          draw[j & 1](canvas, work->sheet + gBattleEffect_GlintPictures[cell],
                      glint->x + x, glint->y + y,
                      gBattleEffect_GlintPictureWidths[cell],
                      gBattleEffect_GlintPictureHeights[cell]);
        }
        glint->variant++;
        if (glint->variant == GLINT_CELS) {
          glint->x = Random16() & 31;
          glint->y = (Random16() & 63) + 16;
          glint->variant = 0;
        }
      }
    }

    // Strike each target seven times, every eight frames, three frames
    // after the target before it.
    for (j = 0; j != args->targetCount; j++) {
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
#undef MOTE_COUNT
