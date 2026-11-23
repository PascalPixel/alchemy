/// @file
/// An unused Mercury Djinni heal, and the camera turn Fire and Froth share.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define DJINN_HEAL_SPARKS 512 ///< sparks in PARTICLES, 64 for each target
#define DJINN_HEAL_SPARKS_PER_TARGET 64

#define DJINN_HEAL_SWIRL_FRAMES 98
#define DJINN_HEAL_SWIRL_MOTES 32 ///< motes swirled in
/// The motes set out to swirl, of which DJINN_HEAL_SWIRL_MOTES are drawn.
#define DJINN_HEAL_SWIRL_SLOTS 64
/// A swirling mote is drawn at size DJINN_HEAL_MOTE_SIZE_MAX at depth
/// DJINN_HEAL_MOTE_DEPTH_NEAR or nearer, one size smaller every
/// DJINN_HEAL_MOTE_DEPTH_STEP farther, its depth held below
/// DJINN_HEAL_MOTE_DEPTH_FAR.
#define DJINN_HEAL_MOTE_SIZE_MAX 9
#define DJINN_HEAL_MOTE_DEPTH_NEAR 250
#define DJINN_HEAL_MOTE_DEPTH_STEP 64
#define DJINN_HEAL_MOTE_DEPTH_FAR 634
/// The depths the sparkles' idle size by depth is worked out over, as the
/// healing sparkles' are: three sizes, DJINN_HEAL_SPARKLE_DEPTH_STEP apart.
#define DJINN_HEAL_SPARKLE_DEPTH_NEAR 314
#define DJINN_HEAL_SPARKLE_DEPTH_STEP 128
#define DJINN_HEAL_SPARKLE_DEPTH_FAR 634
/// The angle mote `i` turns each frame about its axis: each later mote faster.
#define DJINN_HEAL_MOTE_SPIN(i) ((i) * 32 + 256)

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
  FIXED target[3];
  s32 delta[3];
  s32 djinniPoint[3];
  struct ScreenPoint screen;
  s32 point[3];
  struct ScreenPoint view;
  BlitterPair draw;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  BattleEffect_FetchRectangleBlitters(work->args->side ^ 1, draw);
  {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;
    struct Actor *goal = GetBattleObjectSlotFar(work->args->targets[0])->actor;

    for (i = 0; i != DJINN_HEAL_SWIRL_SLOTS; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 heading = Random16() & TRIG_ANGLE_MASK;
      s32 speed = (Random16() & 0xff) + 128;

      SET_POSITION(spark, 0, INTtoFIXED((Random16() & 31) + 20), 0);
      SET_VELOCITY(spark, (Trig_Sin(heading) * speed) >> 5, 0,
                   (Trig_Cos(heading) * speed) >> 5);
      spark->variant = 0;
    }
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                          CANVAS_FADE_THREE_QUARTERS);

    // The swirl.
    SET_VECTOR(position, actor->x.fixed, 0, actor->z.fixed);
    SET_VECTOR(target, goal->x.fixed, toFIXED(90), 0);
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
    Matrix_TranslateVector(position);
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
        Matrix_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Matrix_RotateY(frame * DJINN_HEAL_MOTE_SPIN(i));
          break;
        case SPIN_PITCH:
          Matrix_RotateX(-frame * DJINN_HEAL_MOTE_SPIN(i));
          break;
        case SPIN_ROLL:
          Matrix_RotateZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
          break;
        case SPIN_PITCH_AND_ROLL:
          Matrix_RotateX(-frame * DJINN_HEAL_MOTE_SPIN(i));
          Matrix_RotateZ(-frame * DJINN_HEAL_MOTE_SPIN(i));
          break;
        }
        Projection_Point(spark, &screen);
        screen.x >>= 1;
        Matrix_Pop();
        CLAMP(screen.depth, DJINN_HEAL_MOTE_DEPTH_NEAR,
              DJINN_HEAL_MOTE_DEPTH_FAR);
        size = DJINN_HEAL_MOTE_SIZE_MAX -
               (screen.depth - DJINN_HEAL_MOTE_DEPTH_NEAR) /
                   DJINN_HEAL_MOTE_DEPTH_STEP;
        EFFECT_DRAW_MOTE(draw[0], canvas, sheet, screen.x, screen.y, size);
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
      SET_VECTOR(djinniPoint, 0, (Trig_Sin(frame * (TRIG_TURN / 64)) * 4), 0);
      Projection_Point(djinniPoint, &screen);
      screen.x >>= 1;
      draw[1](canvas, work->sheet, screen.x - SMALL_DJINNI_WIDTH / 2,
              screen.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
              MERCURY_SMALL_DJINNI_HEIGHT);
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  // The healing.
  for (i = 0; i != DJINN_HEAL_SPARKS; i++) {
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
      target[1] += toFIXED(4);
    SET_VECTOR(point, target[0], target[1],
               target[2] + Trig_Sin(frame * (TRIG_TURN / 32)) * 40);
    Projection_Point(point, &view);
    view.x >>= 1;
    draw[0](canvas, work->sheet, view.x - SMALL_DJINNI_WIDTH / 2,
            view.y - MERCURY_SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
            MERCURY_SMALL_DJINNI_HEIGHT);
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *object =
          GetBattleObjectSlotFar(work->args->targets[member])->actor;
      s32 start = member * 8;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(position, object->x.fixed, toFIXED(40), object->z.fixed);
      Matrix_TranslateVector(position);
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
        Matrix_RotateY((frame - start) << 9);
        for (i = 0; i != DJINN_HEAL_SPARKS_PER_TARGET; i++) {
          struct Particle *spark =
              &PARTICLES[member * DJINN_HEAL_SPARKS_PER_TARGET + i];

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
              CLAMP(view.depth, DJINN_HEAL_SPARKLE_DEPTH_NEAR,
                    DJINN_HEAL_SPARKLE_DEPTH_FAR);
              pictureIndex = 3 - (view.depth - DJINN_HEAL_SPARKLE_DEPTH_NEAR) /
                                     DJINN_HEAL_SPARKLE_DEPTH_STEP;
              pictureIndex = (i * 4 + frame) % 9;
              {
                u8 *picture =
                    sheet + gBattleEffect_SparklePictures[pictureIndex];
                u32 edge = gBattleEffect_SparkleSizes[pictureIndex];
                u32 half = edge >> 1;

                draw[1](canvas, picture, view.x - half, view.y - half, edge,
                        edge);
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

/// Turns the battle camera by the effect's delta each VBlank: the whole delta
/// on the VBlank after one of the effect's frames ends, half of it on the
/// others, so the camera keeps turning while a frame runs long.
void BattleCamera_ApplyPhasedDelta(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleCamera *camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  s32 *phase = &work->cameraPhase;

  // XXX (no evidence): whether the half turns are meant to keep the turn
  // smooth when an effect's frame runs long; the code shows the effect, not
  // the reason.
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
#undef DJINN_HEAL_SWIRL_SLOTS
#undef DJINN_HEAL_MOTE_SIZE_MAX
#undef DJINN_HEAL_MOTE_DEPTH_NEAR
#undef DJINN_HEAL_MOTE_DEPTH_STEP
#undef DJINN_HEAL_MOTE_DEPTH_FAR
#undef DJINN_HEAL_SPARKLE_DEPTH_NEAR
#undef DJINN_HEAL_SPARKLE_DEPTH_STEP
#undef DJINN_HEAL_SPARKLE_DEPTH_FAR
