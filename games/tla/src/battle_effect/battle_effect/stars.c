/// @file
/// Astral Blast and Shining Star.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "module/battle_effect/battle_effect.h"

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 // dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct Actor *)GetBattleObjectSlotFar(id)->object)

// Three rings of light fly one after another from the caster to the target,
// each growing as it spins, while the camera swings round; each ring that
// lands throws the target back, and the last begins the results.
void BattleEffect_RunAstralBlast(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition base;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct Actor *caster;
  struct Actor *source;
  struct Actor *target;
  struct EffectStep *point;
  struct EffectStep *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(effect->actor);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Actor_SetMode(caster, 2);
  Actor_SetAnimationSpeedFar(caster, 48);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  work->transferMode = 2;
  work->transferValue = 75;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(effect->actor);
  target = ACTOR(effect->actors[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    point->x = source->x;
    point->y = source->y + 0x280000;
    point->z = source->z;
    if (i == 0)
      point->velocityX = (target->x - point->x) / 12;
    else
      point->velocityX = (target->x * 2 - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x280000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 60; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct CameraView *view = (struct CameraView *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (effect->side == 0)
        view->yaw -= speed;
      else
        view->yaw += speed;
    }

    // A new ring every twelve frames: it spins and grows from a dot to
    // full size, and is drawn while it is near enough to the camera.
    for (i = 0; i != BOLT_COUNT; i++) {
      start = i * 12;
      if (frame >= start) {
        bolt = &work->particles[i];
        size = (frame - start) / 4 + 2;
        if (size > 10)
          size = 10;
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        SceneTransform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          SceneTransform_ApplyRoll((frame - start) << 10);
          SceneTransform_ApplyYaw(0x4000);
          scale[0] = ((frame - start) << 12) + 0x1000;
          if (scale[0] > 0x10000)
            scale[0] = 0x10000;
          scale[1] = scale[0];
          scale[2] = scale[0];
          SceneTransform_ApplyScale(scale);
          SceneTransform_ApplyRoll(j * 0x199a);
          depth =
              EffectPosition_ProjectPoint(&gData_08197924[(j & 1) * 3], &pos);
          if (nearest < depth)
            nearest = depth;
          pos.x >>= 1;
          // The first two stores read base, which nothing sets, and are
          // overwritten at once; both games' code keeps them.
          ring[i * 10 + j].velocityX = pos.x + base.x;
          ring[i * 10 + j].velocityY = pos.y + base.y;
          ring[i * 10 + j].velocityX = pos.x;
          ring[i * 10 + j].velocityY = pos.y;
          Transform_Pop();
        }
        if (nearest <= FAR_DEPTH) {
          for (j = 0; j != RING_POINTS; j++) {
            struct EffectStep *from =
                &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
            struct EffectStep *to =
                &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gData_08197410[size - 1], x - size / 2,
                      y - size, size, size * 2);
            }
          }
        }
        bolt->x += bolt->velocityX;
        bolt->y += bolt->velocityY;
        bolt->z += bolt->velocityZ;

        // Ten frames after it starts (plus one per ring) the ring lands: the
        // target is thrown back 40 pixels and up, and flashes; the third ring
        // begins the results instead.
        if (frame == start + i + 10) {
          target->acceleration = 0x20000;
          target->speedLimit = 0x80000;
          target->velocityY = 0x50000;
          target->gravity = 0xab85;
          target->actionFlags = 0;
          Actor_ResetMotion(target);
          if (target->x < 0)
            Actor_SetTargetPosition(target, target->x - 0x280000, 0, target->z);
          else
            Actor_SetTargetPosition(target, target->x + 0x280000, 0, target->z);
          if (i == 2) {
            Func_081180e8(134);
          } else {
            Sound_PlayFar(134);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
          }
          work->shakeFrames = 4;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef RING_STEPS

// Shining Star's rings of stars.

#define STAR_COUNT 8

#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

/// Eight rings of stars fly from the caster to the target, one every eight
/// frames and spinning as they go; each bounces where it meets the ground,
/// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 i;
  u8 *graphics;
  s32 *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  struct EffectStep *point;
  struct EffectStep *star;
  struct EffectPosition pos;
  s32 size;
  s32 j;
  s32 k;
  struct EffectStep *ring;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(1);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    point->x = source->x / 2;
    point->y = source->y + 0x780000;
    point->z = source->z;
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + 0x140000) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct EffectStep *)gMapCellBuffer;
  for (frame = 0; frame != 128; frame++) {
    Func_081496c8(frame, 0xaaab, 0x5555, 0);
    if (frame == 96)
      Func_081180e8(134);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    ((Trig_Sin(j * 0x199a - ((frame - star->variant) << 11)) *
                      gData_0819793c[j & 1]) /
                         2 >>
                     16);
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                         gData_0819793c[j & 1] >>
                     16);
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct EffectStep *from = &ring[j + i * 10];
                struct EffectStep *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
                }
              }
            }
          }
        }

        // At the ground the star bounces up at half its sideways speed.
        if (star->y <= GROUND_Y) {
          star->velocityY = -star->velocityY;
          star->velocityX /= 2;
          star->velocityZ /= 2;
          work->shakeFrames = 4;
          Sound_PlayFar(134);
          for (j = 0; j != effect->count; j++)
            Func_0814cd48(effect->actors[j], 7, 5, j, 8);
        }
        star->x += star->velocityX;
        star->y += star->velocityY;
        star->z += star->velocityZ;
      }
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
