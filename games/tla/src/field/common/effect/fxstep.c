#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "effect_step.h"

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity) {
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;
  s32 vz = step->velocityZ;

  step->x += vx;
  step->y += vy;
  step->z += vz;
  step->velocityX = vx * damping / 64;
  step->velocityY = (vy + gravity) * damping / 64;
  step->velocityZ = vz * damping / 64;
}

void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity) {
  s32 x = step->x;
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;

  step->x = x + vx;
  step->y += vy;
  step->velocityX = vx * damping / 64;
  step->velocityY = (vy + gravity) * damping / 64;
}

s32 EffectPosition_ApplyBaseAndYOffset(const void *point,
                                       struct EffectPosition *position) {
  s32 result = Render_ProjectPoint(point, EffectPosition_Coordinates(position));
  position->y -= 16;
  return result;
}

void EffectPosition_ApplyAnimationAndYOffset(s32 id,
                                             struct EffectPosition *position) {
  BattleUnit_ProjectToScreen(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

void EffectPosition_ApplyStepAndYOffset(s32 id,
                                        struct EffectPosition *position) {
  BattleMotion_ProjectScaledPositionFar(id,
                                        EffectPosition_Coordinates(position));
  position->y -= 16;
}

void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position) {
  BattleMotion_ProjectPositionFar(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}
