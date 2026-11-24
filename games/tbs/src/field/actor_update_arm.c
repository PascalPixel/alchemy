/// @file
/// Unused: an ARM copy of the actors' update, made to run from RAM. Nothing
/// runs it; the field moves its actors with Actor_UpdateAllOnField.

#include "field/actor_update_arm.h"
#include "fixed_math.h"
#include "macros.h"
#include "types.h"
#include "field/actor_arm_view.h"
#include "field/actor_script.h"
#include "system/heap.h"
#include "system/irq.h"
#include "system/iwram_call.h"
#include "system/mul_fx.h"

#define ACTOR_SLOTS 8             ///< script-driven actors updated each frame
#define TURN_MAX (TRIG_TURN / 16) ///< the most an actor turns in one frame

/// Calls a resident IWRAM routine at its absolute address, which this code
/// still reaches when it runs from its copy in RAM.
#define IWRAM_ENTRY(type, offset) ((type)(IwramIrqMain + (offset)))
#define IwramSqrt IWRAM_ENTRY(s32 (*)(s32 value), Iwram_SqrtOffset)
#define IwramRatioMulQ14                                                       \
  IWRAM_ENTRY(s32 (*)(s32 divisor, s32 value), Iwram_RatioMulQ14Offset)

/// Far-call stubs copied to RAM together with this code (actor_update_far.s).
s32 Trig_AtanFarArm(s32 z, s32 x);
s32 Iwram_SqrtFarArm(s32 value);
s32 Iwram_RatioMulQ14FarArm(s32 divisor, s32 value);

/// Advances the eight script-driven actors by one frame.
void Actor_UpdateAll(void) {
  struct ActorArmView *actor;
  s32 y;
  s32 arrived;
  struct ActorArmView *nextActor;
  s32 nextSlot;
  s32 slot;
  s32 x;
  s32 z;
  s32 deltaX;
  s32 deltaZ;
  FIXED velocityX;
  FIXED velocityZ;
  s32 distance;
  s32 speedRatio;
  s32 groundY;
  s32 turnDelta;
  u16 travelAngle;

  for (slot = 0, actor = HEAP_SLOT_BLOCK(HEAP_SLOT_ACTORS); slot < ACTOR_SLOTS;
       slot = nextSlot, actor = nextActor) {
    nextSlot = slot + 1;
    nextActor = actor + 1;
    if (actor->script == NULL)
      continue;
    if (actor->update != NULL)
      actor->update((struct Actor *)actor);
    if (actor->script == NULL)
      continue;
    if (actor->paused != 0)
      continue;

    // Count down a wait, or run commands until one waits for this frame.
    if (actor->wait != 0) {
      actor->wait--;
    } else {
      for (;;) {
        s32 cursor = (s16)actor->scriptCursor;
#ifdef UBFIX
        u32 command = actor->script[cursor];
#else
        u32 command = actor->script->word[cursor];
#endif

        if (command <= SCRIPT_COMMAND_MAX) {
          if (gActorScript_CommandTable[command]((struct Actor *)actor) == 0)
            break;
        } else {
          actor->scriptCursor++;
        }
      }
      if (actor->script == NULL)
        continue;
    }

    x = actor->x;
    y = actor->y;
    z = actor->z;
// UB: frozen actors skip the initialization below, but still test arrival.
#ifdef UBFIX
    arrived = 0;
#endif
    if (actor->frozen == 0) {
      arrived = 0;
      if (actor->targetX != ACTOR_NO_TARGET) {
        // Accelerate towards the target.
        deltaX = (actor->targetX - x) / FIXED_ONE;
        deltaZ = (actor->targetZ - z) / FIXED_ONE;
        distance = toFIXED(IwramSqrt(deltaX * deltaX + deltaZ * deltaZ));
        if (distance <= toFIXED(256) - 1) {
          deltaX = actor->targetX - x;
          deltaZ = actor->targetZ - z;
          distance = IwramSqrt(MulFX(deltaX, deltaX) + MulFX(deltaZ, deltaZ))
                     << 8;
        }
        if (distance == 0) {
          x = actor->targetX;
          z = actor->targetZ;
        } else {
          // speedRatio is acceleration / distance in 16.16.
          speedRatio = IwramRatioMulQ14(distance, actor->acceleration);
          velocityX = actor->velocityX + MulFX(deltaX, speedRatio);
          actor->velocityX = velocityX;
          // Limit the combined horizontal velocity after accelerating both
          // axes.
          velocityZ = actor->velocityZ + MulFX(deltaZ, speedRatio);
          actor->velocityZ = velocityZ;
          distance = IwramSqrt(MulFX(velocityX, velocityX) +
                               MulFX(velocityZ, velocityZ))
                     << 8;
          if (distance > actor->speedLimit) {
            // Too fast: scale the velocity back to the speed limit.
            speedRatio = IwramRatioMulQ14(distance, actor->speedLimit);
            actor->velocityX = MulFX(velocityX, speedRatio);
            actor->velocityZ = MulFX(velocityZ, speedRatio);
          }
        }
      } else {
        // No target: brake by the acceleration until the actor stops.
        deltaX = actor->velocityX;
        deltaZ = actor->velocityZ;
        distance =
            Iwram_SqrtFarArm(MulFX(deltaX, deltaX) + MulFX(deltaZ, deltaZ))
            << 8;
        if (distance != 0) {
          if (distance - actor->acceleration < 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
          } else {
            speedRatio = Iwram_RatioMulQ14FarArm(
                distance, distance - actor->acceleration);
            actor->velocityX = MulFX(deltaX, speedRatio);
            actor->velocityZ = MulFX(deltaZ, speedRatio);
          }
        } else {
          actor->velocityX = 0;
          actor->velocityZ = 0;
        }
      }

      // Gravity for an actor that falls, and a bounce that loses energy on
      // landing.
      if (actor->motionFlags & ACTOR_FALLS) {
        groundY = actor->groundHeight;
        if (y > groundY)
          actor->velocityY -= actor->gravity;
        if (y <= groundY && actor->velocityY < 0) {
          y = groundY;
          actor->velocityY = -MulFX(actor->velocityY, actor->vertical.bounce);
          if ((actor->velocityY < 0 ? -actor->velocityY : actor->velocityY) <=
              actor->gravity)
            actor->velocityY = 0;
        }
      }
    }

    // Integrate velocity even when the acceleration and gravity step is frozen.
    x += actor->velocityX;
    y += actor->velocityY;
    z += actor->velocityZ;

    // Arrived: the watched axis reached the target, or crossed it this frame
    // (the sign of the offset changed).
    switch (actor->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == actor->targetX ||
          ((actor->x - actor->targetX) ^ (x - actor->targetX)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == actor->targetY ||
          ((actor->y - actor->targetY) ^ (y - actor->targetY)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == actor->targetZ ||
          ((actor->z - actor->targetZ) ^ (z - actor->targetZ)) < 0)
        arrived = 1;
      break;
    }
    // Snapping stops the actor on the target; it takes the target's height
    // too only when its motion flags are all clear.
    if (arrived) {
      if (actor->snapToTarget) {
        x = actor->targetX;
        actor->velocityX = 0;
        z = actor->targetZ;
        actor->velocityZ = 0;
        if (actor->motionFlags == 0) {
          y = actor->targetY;
          actor->velocityY = 0;
        }
      }
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      actor->arrivalAxis = ARRIVAL_AXIS_NONE;
    }
    SET_POSITION(actor, x, y, z);

    // An actor that faces its motion turns toward its direction of travel.
    if (actor->steeringFlags & ACTOR_FACES_MOTION) {
      x = actor->velocityX;
      z = actor->velocityZ;
      if (x != 0 || z != 0) {
        travelAngle = Trig_AtanFarArm(z, x);
        // As a signed halfword the difference is the shorter way round.
        turnDelta = (s16)(travelAngle - actor->facing);
        if (turnDelta > TURN_MAX)
          turnDelta = TURN_MAX;
        if (turnDelta < -TURN_MAX)
          turnDelta = -TURN_MAX;
        actor->facing += turnDelta;
      }
    }
  }
}
