/// @file
/// An unused Mercury Djinni heal, and the camera turn Fire and Froth share.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define DJINN_HEAL_SWIRL_FRAMES 98
#define DJINN_HEAL_SWIRL_MOTES 32 ///< motes swirled in

/// A small Mercury Djinni heals the party: motes swirl up from the user, the
/// Djinni appears where they gather, and sparkles spiral in on each target in
/// turn. Nothing reaches it.
UNUSED void BattleEffect_RunDjinnHeal(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 member;
  u8 *sheet;
  s32 frame;
  s32 i;
  s32 position[3];
  s32 target[3];
  s32 delta[3];
  s32 djinniPoint[3];
  struct ScreenPoint screen;
  s32 point[3];
  struct ScreenPoint view;
  BlitterPair callbacks;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side ^ 1, callbacks);
  {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;
    struct Actor *goal = GetBattleObjectSlotFar(work->args->targets[0])->actor;

    for (i = 0; i != 64; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 heading = Random16() & TRIG_ANGLE_MASK;
      s32 speed = (Random16() & 0xff) + 128;

      SET_POSITION(spark, 0, ((Random16() & 31) + 20) << 16, 0);
      SET_VELOCITY(spark, (Trig_Sin(heading) * speed) >> 5, 0,
                   (Trig_Cos(heading) * speed) >> 5);
      spark->variant = 0;
    }
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);

    // The swirl.
    SET_VECTOR(position, actor->x.fixed, 0, actor->z.fixed);
    SET_VECTOR(target, goal->x.fixed, 180 << 15, 0);
  }
  SET_VECTOR(delta, (target[0] - position[0]) / 40,
             (target[1] - position[1]) / 40, (target[2] - position[2]) / 40);
  for (frame = 0; frame != DJINN_HEAL_SWIRL_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;
    if (frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 80)
      Sound_PlayFar(SE_SWIRL);
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame >= 30 && frame < 70) {
      position[0] += delta[0];
      position[1] += delta[1];
      position[2] += delta[2];
    }
    Transform_ApplyPosition(position);
    if (frame == 0)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                               TINT_KEEP, 0);
    if (frame == 24)
      BattleEffect_TintBattler(work->args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                               TINT_KEEP, 0);
    for (i = 0; i != DJINN_HEAL_SWIRL_MOTES; i++) {
      struct Particle *spark = &PARTICLES[i];
      if (frame > i && spark->variant == 0) {
        s32 size;
        Transform_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Transform_ApplyYaw(frame * (i * 32 + 256));
          break;
        case SPIN_PITCH:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          break;
        case SPIN_ROLL:
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        case SPIN_PITCH_AND_ROLL:
          Transform_ApplyPitch(-frame * (i * 32 + 256));
          Transform_ApplyRoll(-frame * (i * 32 + 256));
          break;
        }
        Projection_Point(spark, &screen);
        screen.x >>= 1;
        Transform_Pop();
        CLAMP(screen.depth, 250, 634);
        size = 9 - (screen.depth - 250) / 64;
        EFFECT_DRAW_MOTE(callbacks[0], canvas, sheet, screen.x, screen.y, size);
        Particle_AdvanceWithGravity3D(spark, 60, 0);
        if (frame > i + 30) {
          s32 dx = -spark->x >> 8;
          s32 dy = -spark->y >> 8;
          s32 dz = -spark->z >> 8;

          spark->velocityX += dx;
          spark->velocityY += dy;
          spark->velocityZ += dz;
        }
      }
    }
    // The Djinni appears and bobs.
    if (frame > 82) {
      SET_VECTOR(djinniPoint, 0, (Trig_Sin(frame << 10) * 4), 0);
      Projection_Point(djinniPoint, &screen);
      screen.x >>= 1;
      callbacks[1](canvas, work->sheet, screen.x - 10, screen.y - 17, 20, 34);
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  // The healing.
  for (i = 0; i != 512; i++) {
    struct Particle *spark = &PARTICLES[i];
    SET_POSITION(spark, ((Random16() & 0xff) - 127) << 15,
                 ((Random16() & 0x7f) + 64) << 15,
                 ((Random16() & 0xff) - 127) << 15);
    spark->variant = 0;
  }
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS, sheet, 0, 0);
  for (frame = 0; frame != work->args->targetCount * 8 + 72; frame++) {
    struct BattleCamera *camera = gBattleCamera;
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The Djinni sways, then flies off.
    if (frame >= work->args->targetCount * 8 + 40)
      target[1] += px(4);
    SET_VECTOR(point, target[0], target[1],
               target[2] + Trig_Sin(frame << 11) * 40);
    Projection_Point(point, &view);
    view.x >>= 1;
    callbacks[0](canvas, work->sheet, view.x - 10, view.y - 17, 20, 34);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *object =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;
      s32 start = member * 8;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(position, object->x.fixed, 160 << 14, object->z.fixed);
      Transform_ApplyPosition(position);
      // Each target glows with the recovery sound.
      if (frame == start + 30)
        Sound_PlayFar(SE_RECOVERY);
      if (frame == start + 40)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_WHITE, TINT_KEEP, TINT_KEEP, 0);
      if (frame == start + 64)
        BattleEffect_TintBattler(work->args->targets[member],
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      if (frame > start) {
        Transform_ApplyYaw((frame - start) << 9);
        for (i = 0; i != 64; i++) {
          struct Particle *spark = &PARTICLES[member * 64 + i];

          if (frame > start + i / 2) {
            s32 xx = (spark->x >> 8) * (spark->x >> 8);
            s32 yy = (spark->y >> 8) * (spark->y >> 8);
            s32 zz = (spark->z >> 8) * (spark->z >> 8);
            s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;
            if (distance != 0) {
              s32 pictureIndex;

              Projection_Point(spark, &view);
              view.x >>= 1;
              /* LEFTOVER(template): the clamp and size by depth, idle here. */
              CLAMP(view.depth, 314, 634);
              pictureIndex = 3 - (view.depth - 314) / 128;
              pictureIndex = (i * 4 + frame) % 9;
              {
                u8 *picture =
                    sheet + gBattleEffect_SparklePictures[pictureIndex];
                u32 edge = gBattleEffect_SparkleSizes[pictureIndex];
                u32 half = edge >> 1;

                callbacks[1](canvas, picture, view.x - half, view.y - half,
                             edge, edge);
              }
              spark->x -= spark->x / distance;
              spark->y -= spark->y / distance;
              spark->z -= spark->z / distance;
            }
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

// Every VBlank turns the camera at least half the delta, so it keeps turning on
// VBlanks that end no frame of the effect.
// XXX (no evidence): whether that is meant to keep the turn smooth when an
// effect's frame runs long; the code shows the effect, not the reason.
/// Turns the battle camera's yaw by the effect's delta.
void BattleCamera_ApplyPhasedDelta(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  s32 *phase = &work->cameraPhase;

  if (*phase == CAMERA_PHASE_FRAME_DONE) {
    camera->yaw += work->cameraDelta;
    *phase = CAMERA_PHASE_IDLE;
  } else {
    camera->yaw += work->cameraDelta / 2;
    if (*phase == CAMERA_PHASE_HALF_TURNED)
      *phase = CAMERA_PHASE_IDLE;
    else
      *phase = CAMERA_PHASE_HALF_TURNED;
  }
}

#undef DJINN_HEAL_SWIRL_FRAMES
#undef DJINN_HEAL_SWIRL_MOTES
