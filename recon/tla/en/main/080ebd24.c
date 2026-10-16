// Near miss, differ 2 of 156 (2026-10-15). The native loads targetX, then
// starts EFFECT_NO_TARGET (ldr r0, [r7, #12]; movs r2, #128; lsls; sub sp, #4);
// this starts the constant first, its movs having two dependents to the load's
// one. Measured: an empty asm taking the load as input and the constant's first
// byte as in/out orders those rows, but flips the tie in the multiply block and
// three later constants' registers (differ 30); a barrier after the load moves
// sub sp ahead of it (differ 2).
#include "types.h"
#include "effect_slot.h"
#include "iwram_call.h"
#include "fixed_math.h"

void EffectSlot_UpdateMotion(struct EffectSlot *effect) {
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ix;
  s32 iz;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  dx = effect->targetX - effect->x;
  dz = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    ix = dx / 0x10000;
    iz = dz / 0x10000;
    distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    if (distance <= 0x80000) {
      EffectSlot_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(dz, dx);
  if (effect->flag42 != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = (s16)facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}
