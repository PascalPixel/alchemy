/// @file
/// Astral Blast and Shining Star.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"

#define BOLT_COUNT 3
#define RING_POINTS 10
#define RING_STEPS 16 ///< dots drawn between two ring points
#define FAR_DEPTH 399999

#define ACTOR(id) ((struct Actor *)BattleObject_GetFar(id)->object)

#define ASTRAL_BLAST_FRAMES 60

/// Three pink stars of light fly from above the user at the first target,
/// growing and spinning, while the camera swings round; each knocks the target
/// back.
///
/// Abilities: Astral Blast.
void BattleEffect_RunAstralBlast(struct BattleEffectParams *args) {
  struct ScreenPoint pos;
  struct ScreenPoint base;
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
  struct Particle *point;
  struct Particle *bolt;
  s32 start;
  s32 nearest;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *ring;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;
  caster = ACTOR(args->user);

  // Set up: the rune palette, the dot sheet, the caster's casting pose, one
  // max blitter and the transfer.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_RuneSheet),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Actor_SelectAnimationFar(caster, 2);
  Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL * 3);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The rings leave from above the caster and reach the target in twelve
  // frames; the second and third aim past it.
  source = ACTOR(args->user);
  target = ACTOR(args->targets[0]);
  for (i = 0, point = work->particles; i != BOLT_COUNT; i++) {
    SET_POSITION(point, source->x.fixed, source->y.fixed + px(40),
                 source->z.fixed);
    if (i == 0)
      point->velocityX = (target->x.fixed - point->x) / 12;
    else
      point->velocityX = (target->x.fixed * 2 - point->x) / 12;
    point->velocityY = (target->y.fixed - point->y + px(40)) / 12;
    point->velocityZ = (target->z.fixed - point->z) / 12;
    point->variant = 0;
    point++;
  }

  ring = (struct Particle *)gMapGridCellBuffer;
  for (frame = 0; frame != ASTRAL_BLAST_FRAMES; frame++) {
    // The camera turns toward the caster's side, slowing from frame 40.
    if (frame <= 47) {
      struct BattleCamera *view = (struct BattleCamera *)camera;
      s32 speed;

      if (frame <= 39)
        speed = 128;
      else
        speed = 0x300 - frame * 16;
      if (args->side == EFFECT_SIDE_ENEMIES)
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
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        Transform_ApplyPosition(bolt);
        nearest = 0;
        for (j = 0; j != RING_POINTS; j++) {
          s32 depth;

          Transform_Push();
          Transform_ApplyRoll((frame - start) << 10);
          Transform_ApplyYaw(TRIG_TURN / 4);
          scale[0] = ((frame - start) << 12) + Q16_ONE / 16;
          if (scale[0] > Q16_ONE)
            scale[0] = Q16_ONE;
          scale[1] = scale[0];
          scale[2] = scale[0];
          Transform_ApplyScale(scale);
          Transform_ApplyRoll(j * 0x199a);
          depth = EffectPosition_ProjectPoint(&gStar_Points[(j & 1) * 3], &pos);
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
            struct Particle *from =
                &((struct Particle *)gMapGridCellBuffer)[j + i * 10];
            struct Particle *to =
                &((struct Particle *)gMapGridCellBuffer)[(j + 1) % 10 + i * 10];

            for (k = 0; k != RING_STEPS; k++) {
              s32 x =
                  from->velocityX + (to->velocityX - from->velocityX) * k / 16;
              s32 y =
                  from->velocityY + (to->velocityY - from->velocityY) * k / 16;

              draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                      x - size / 2, y - size, size, size * 2);
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
          target->acceleration = px(2);
          target->speedLimit = px(8);
          target->velocityY = px(5);
          target->gravity = 0xab85;
          target->steeringFlags = 0;
          Actor_StartIdleFar(target);
          if (target->x.fixed < 0)
            Actor_SetMoveTargetFar(target, target->x.fixed - px(40), 0,
                                   target->z.fixed);
          else
            Actor_SetMoveTargetFar(target, target->x.fixed + px(40), 0,
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

#undef RING_STEPS

// Shining Star's rings of stars.

#define STAR_COUNT 8

#define RING_STEPS 12 // dots drawn between two ring points
#define DOT_SIZE 2
#define GROUND_Y 0x1dffff

#define SHINING_STAR_FRAMES 128

/// Eight rings of stars fly from the caster to the target, one every eight
/// frames and spinning as they go; each bounces where it meets the ground,
/// shaking the screen and flashing the targets.
void BattleEffect_RunShiningStar(struct BattleEffectParams *args) {
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
  struct Particle *point;
  struct Particle *star;
  struct ScreenPoint pos;
  s32 size;
  s32 j;
  s32 k;
  struct Particle *ring;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the rune palette, the dot sheet, one max blitter and the transfer.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_RuneSheet),
                       SHEET_PALETTE_BYTES);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  // Each star leaves from above the caster on its own course to the target,
  // the later ones after a longer delay (held in variant).
  source = BattleObject_GetFar(args->user)->object;
  target = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0, point = work->particles; i != STAR_COUNT; i++) {
    s32 delay = i * 8;

    SET_POSITION(point, source->x / 2, source->y + px(120), source->z);
    point->velocityX =
        (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
    point->velocityY = (target->y - point->y + px(20)) / 12;
    point->velocityZ = (target->z - point->z) / 12;
    point->variant = (Random16() & 15) + delay;
    point++;
  }

  size = DOT_SIZE;
  ring = (struct Particle *)gMapGridCellBuffer;
  for (frame = 0; frame != SHINING_STAR_FRAMES; frame++) {
    BattleEffect_UpdatePhasePalette(frame, TRIG_TURN - TRIG_TURN / 3,
                                    TRIG_TURN / 3, 0);
    if (frame == 96)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    for (i = 0; i != STAR_COUNT; i++) {
      star = &work->particles[i];
      if (frame >= star->variant) {
        // Project the star and, while it is on screen, lay out its ring of
        // ten points (the velocity fields of the map buffer's steps), then
        // join each point to the next with a row of dots.
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        if (pos.x >= -8 && pos.x <= 127) {
          if (pos.y <= 127) {
            if (pos.y >= -8) {
              for (j = 0; j != RING_POINTS; j++) {
                ring[i * 10 + j].velocityX =
                    pos.x +
                    (Q_16_16_TO_INT((Trig_Sin(j * 0x199a -
                                              ((frame - star->variant) << 11)) *
                                     gStar_Radii[j & 1]) /
                                    2));
                ring[i * 10 + j].velocityY =
                    pos.y -
                    (Q_16_16_TO_INT(
                        Trig_Cos(j * 0x199a - ((frame - star->variant) << 11)) *
                        gStar_Radii[j & 1]));
              }
              for (j = 0; j != RING_POINTS; j++) {
                struct Particle *from = &ring[j + i * 10];
                struct Particle *to = &ring[i * 10 + (j + 1) % 10];

                for (k = 0; k != RING_STEPS; k++) {
                  s32 x = from->velocityX +
                          (to->velocityX - from->velocityX) * k / 12;
                  s32 y = from->velocityY +
                          (to->velocityY - from->velocityY) * k / 12;

                  draw[0](canvas,
                          graphics + gBattleEffect_MotePictures[size - 1],
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
          Sound_PlayFar(SE_HEAVY_IMPACT);
          for (j = 0; j != args->targetCount; j++)
            BattleEffect_TintBattler(args->targets[j], SPRITE_COLOR_WHITE,
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
#undef BOLT_COUNT
#undef STAR_COUNT
