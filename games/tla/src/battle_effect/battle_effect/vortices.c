/// @file
/// Froth and Whirlwind.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/battle_effect/battle_effect.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"
#include "macros.h"

// Heap slots The Lost Age addresses by byte offset in Ram_HeapSlots.
#define MOTE_COUNT 30
#define SEAT_FIRST 32 // the particles that hold each target's seat

// Thirty motes rise from the caster, swirl, and home in on the targets; each
// one that reaches the ground flashes and knocks back its target and plays a
// three-cel splash where it landed.
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
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_VortexSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                      BLITTER_MAX);
  REG_BLDALPHA = BLEND_ALPHA(15, 15);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCode2;

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
  work->cameraPhase = 0;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_SCATTER);

  for (frame = 0; frame != gVortices_Counts[args->variant * 2 + 1]; frame++) {
    // The camera turns between frames 17 and 63.
    camera = Ram_HeapSlots->cameraWork;
    if (frame >= 17 && frame < 64)
      work->cameraDelta = 0x180;
    else
      work->cameraDelta = 0;
    if (frame == gVortices_Counts[args->variant * 2 + 1] - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);

    for (i = 0; i != gVortices_Counts[args->variant * 2]; i++) {
      point = &work->particles[i];

      // A mote leaves the caster two frames after the one before it.
      if (frame > i * 2 && landed[i] == 0) {
        EffectPosition_ProjectPoint((s32 *)point, &pos);
        pos.x >>= 1;
        if (pos.depth < 160)
          pos.depth = 160;
        if (pos.depth > 0x31f)
          pos.depth = 0x31f;
        draw[0](canvas, work->sheet + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
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
          point->velocityX = point->velocityX * 60 / 64;
          point->velocityY = point->velocityY * 60 / 64;
          point->velocityZ = point->velocityZ * 60 / 64;
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
                                         0);
          work->shakeFrames = 4;
          Sound_PlayFar(SE_WATER);
        }
      }

      // The splash: three cels, two frames each, drawn as mirrored halves.
      if (point->variant >= 0 && point->variant < 16) {
        draw[0](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x - 16, point->y - 56, 16, 64);
        draw[1](canvas, work->sheet + ((point->variant / 2) % 3 << 10),
                point->x, point->y - 56, 16, 64);
        point->variant++;
      }
    }

    BattleCamera_Shake(args->variant * 2 + 2, args->variant * 2 + 2);
    if (work->cameraPhase == 0)
      work->cameraPhase = 1;
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  EFFECT_FINISH_CANVAS();
}

#define TORNADO_FRAMES 80
#define GLINT_COUNT 16 // four glints for each of up to four funnels
#define GLINT_CELS 6   // three cels of two frames each

// One to four funnels of wind sway across the targets for 80 frames, each
// drawn as three stacked segments with glints swirling around it; the canvas
// is wiped every eight frames, and each target is struck seven times.
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
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCode2;

  // The glints start at random places in their funnel, staggered in time.
  for (j = 0; j != GLINT_COUNT; j++) {
    glint = &work->particles[j];
    glint->x = Random16() & 31;
    glint->y = (Random16() & 63) + 16;
    glint->variant = -(Random16() & 15);
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  wait(1);
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != TORNADO_FRAMES; frame++) {
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
    if (args->side == 1)
      wave += 0x200000;
    else
      wave -= 0x200000;
    if (frame <= 16)
      REG_BLDALPHA = frame | 0x1000;
    if (frame > 63)
      REG_BLDALPHA = (0x4f - frame) | 0x1000;

    for (i = 0; i != gVortices_Shapes[args->variant * 3]; i++) {
      // Each funnel circles its own phase, a quarter turn from the next.
      x = ((gVortices_Shapes[args->variant * 3 + 1] *
                Trig_Sin((frame << 11) + i * (TRIG_TURN / 4)) +
            wave) >>
           16) +
          40;
      y = (Trig_Cos((frame << 11) + i * (TRIG_TURN / 4)) * 2) >> 16;
      segment = (frame / 2) % 3;
      draw[0](canvas, work->sheet + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
      draw[0](canvas, work->sheet + segment * 0x500 + 0x2a56, x, y + 48, 40,
              32);
      draw[0](canvas, work->sheet + segment * 0xa00 + 0x1156, x, y + 80, 40,
              32);

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
          BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, j, 4);
          BattleMotion_ApplyKnockbackFar(args->targets[j], 6);
        }
      }
    }

    work->shakeFrames = 1;
    BattleCamera_Shake(gVortices_Shapes[args->variant * 3 + 2],
                       gVortices_Shapes[args->variant * 3 + 2] * 2);
    EFFECT_END_FRAME(work);
  }

  // Tear down.
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
