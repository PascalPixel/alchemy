/// @file
/// The stump Move pushes off a ledge into the water, in Goma Cave Entrance and
/// Goma Cave.
///
/// The including scene defines where it lands (STUMP_LANDING_X and _Z), where
/// it bounces on to and sinks (STUMP_RESTING_X and _Z), and the height of the
/// ground it comes down to (STUMP_FLOOR_HEIGHT, 16.16).

/// Spins the leaping stump a thirty-second of a turn each frame.
static void Scene_SpinStump(struct Actor *actor) {
  struct Sprite *sprite = actor->sprite;

  sprite->rotation -= TRIG_TURN / 32;
}

#include "actor_wait_below_height.c"

/// The stump tips over the ledge, leaps spinning into the water, lands with a
/// shake and a ring of dust, bounces once and sinks with a splash.
static void Scene_LeapStump(s32 stumpId) {
  struct Actor *stump;
  u32 i;
  struct Vector3 dir;
  struct EffectOptions options;

  stump = Actor_Get(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i < STUMP_TIP_FRAMES; i++) {
    wait(1);
    stump->sprite->rotation -= TRIG_TURN / 256;
    stump->x.fixed -= Trig_Cos(stump->sprite->rotation) / 2;
    stump->targetX = ACTOR_NO_TARGET;
  }
  stump->update = Scene_SpinStump;
  Actor_SetSpeed(stumpId, 3);
  Actor_SetDestination(stumpId, STUMP_LANDING_X, STUMP_LANDING_Z);
  stump->gravity = toFIXED(4) / 5;
  stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  stump->layer = 0;
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, STUMP_FLOOR_HEIGHT);
  Sound_Play(SE_THUMP);
  MapGrid_SetShake(5, 5, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  // The puffs of dust, one in each of the sixteen facings.
  for (i = 0; i < STUMP_LANDING_PUFFS; i++) {
    dir.x = Trig_Cos(i * FACING_STEP);
    dir.y = 0;
    dir.z = Trig_Sin(i * FACING_STEP);
    dir.x -= dir.x / 4;
    dir.z -= dir.z / 2;
    FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, dir.x,
                      dir.y, dir.z, 0, NULL);
  }
  stump->velocityY = toFIXED(5);
  Actor_SetDestination(stumpId, STUMP_RESTING_X, STUMP_RESTING_Z);
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, STUMP_FLOOR_HEIGHT);
  stump->update = NULL;
  stump->sprite->rotation = TRIG_TURN / 16;
  options.type = SPRITE_SPLASH;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = 4 * FIXED_ONE / 5;
  options.targetScaleX = 3 * FIXED_ONE / 2;
  options.targetScaleY = 6 * FIXED_ONE / 5;
  FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, 0, 0, 0,
                    EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                        EFFECT_SCALE_TO_TARGET,
                    &options);
  Sound_Play(SE_CAST);
  Actor_SetAnimation(stumpId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
}
