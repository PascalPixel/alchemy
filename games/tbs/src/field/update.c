#include "unit/field/update.h"
#include "unit/field/object_script.h"
#include "types.h"
#include "irq.h"
#include "iwram_call.h"
#include "mulq16.h"
#include "script_motion.h"
#include "macros.h"

#define OBJECT_SLOTS 8 ///< script-driven objects updated each frame
#define TARGET_UNSET ((s32)0x80000000) ///< no movement target
#define TURN_MAX 0x1000 ///< the most an object turns in one frame

/// Calls a resident IWRAM routine at its absolute address, which this code
/// still reaches when it runs from its copy in RAM.
#define IWRAM_ENTRY(type, offset) ((type)(IwramIrqMain + (offset)))
#define IwramSqrt IWRAM_ENTRY(s32 (*)(s32 value), Iwram_SqrtOffset)
#define IwramRatioMulQ14                                                       \
  IWRAM_ENTRY(s32 (*)(s32 divisor, s32 value), Iwram_RatioMulQ14Offset)

extern struct ScriptMotionObject *gObjectSlots;

// Far-call stubs copied to RAM together with this code (updfar.s).
s32 ArcTan2FarArm(s32 z, s32 x);
s32 IwramSqrtFarArm(s32 value);
s32 IwramRatioMulQ14FarArm(s32 divisor, s32 value);

/// Advances the eight script-driven objects by one frame. For each live one:
/// run its hook, then its script (unless it waits or is paused); steer it
/// towards its target, or brake it when it has none; apply gravity and
/// bounce off the ground; detect arrival at the target; and turn it to face
/// the way it moves. Copied to RAM and run from there. Writes each object's
/// position, velocity and facing, and unsets its target on arrival. Only
/// Object_RunUpdateAllFromHeap copies and runs it, and nothing calls that;
/// the field runs Object_UpdateAllThumb instead.
void Object_UpdateAll(void) {
  struct ScriptMotionObject *object;
  s32 y;
  s32 arrived;
  struct ScriptMotionObject *nextObject;
  s32 nextSlot;
  s32 slot;
  s32 x;
  s32 z;
  s32 deltaX;
  s32 deltaZ;
  s32 velocityX;
  s32 velocityZ;
  s32 distance;
  s32 speedRatio;
  s32 groundY;
  s32 turnDelta;
  u16 travelAngle;

  for (slot = 0, object = gObjectSlots; slot < OBJECT_SLOTS;
       slot = nextSlot, object = nextObject) {
    nextSlot = slot + 1;
    nextObject = object + 1;
    if (object->script == NULL)
      continue;
    if (object->hook != NULL)
      object->hook((union FieldObject *)object);
    if (object->script == NULL)
      continue;
    if (object->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    // Words above 63 are labels (SC_LABEL), which the cursor steps over; a
    // command moves the cursor past its own operands.
    if (object->wait != 0) {
      object->wait--;
    } else {
      for (;;) {
        s32 cursor = (s16)object->scriptCursor;
#ifdef UBFIX
        u32 command = object->script[cursor];
#else
        u32 command = object->script->word[cursor];
#endif

        if (command <= 63) {
          if (gScriptObject_CommandTable[command](
                  (struct ScriptInterpreter *)object) == 0)
            break;
        } else {
          object->scriptCursor++;
        }
      }
      if (object->script == NULL)
        continue;
    }

    x = object->x;
    y = object->y;
    z = object->z;
// UB: frozen objects skip the initialization below, but still test arrival.
#ifdef UBFIX
    arrived = 0;
#endif
    if (object->frozen == 0) {
      arrived = 0;
      if (object->targetX != TARGET_UNSET) {
        // Accelerate towards the target. The first distance is in whole
        // pixels, to stay in range; under 256 pixels it is measured again in
        // 16.16, where the root of a 16.16 square is 8.8 and << 8 makes it
        // 16.16 again.
        deltaX = (object->targetX - x) / 65536;
        deltaZ = (object->targetZ - z) / 65536;
        distance = IwramSqrt(deltaX * deltaX + deltaZ * deltaZ) << 16;
        if (distance <= 0xffffff) {
          deltaX = object->targetX - x;
          deltaZ = object->targetZ - z;
          distance = IwramSqrt(MulQ16(deltaX, deltaX) + MulQ16(deltaZ, deltaZ))
                     << 8;
        }
        if (distance == 0) {
          x = object->targetX;
          z = object->targetZ;
        } else {
          // speedRatio is acceleration / distance in 16.16. Under 256 pixels
          // deltaX and deltaZ are 16.16 too, so each axis gains its share of
          // the acceleration; further away they are whole pixels, and MulQ16
          // makes the gain 65536 times smaller, which rounds to almost
          // nothing.
          // XXX: likely a bug, because Object_UpdateAllThumb's free flight
          // multiplies its whole-pixel offsets by the ratio without MulQ16.
          speedRatio = IwramRatioMulQ14(distance, object->acceleration);
          velocityX = object->velocityX + MulQ16(deltaX, speedRatio);
          object->velocityX = velocityX;
          // Limit the combined horizontal velocity after accelerating both
          // axes.
          velocityZ = object->velocityZ + MulQ16(deltaZ, speedRatio);
          object->velocityZ = velocityZ;
          distance = IwramSqrt(MulQ16(velocityX, velocityX) +
                               MulQ16(velocityZ, velocityZ))
                     << 8;
          if (distance > object->speedLimit) {
            // Too fast: scale the velocity back to the speed limit.
            speedRatio = IwramRatioMulQ14(distance, object->speedLimit);
            object->velocityX = MulQ16(velocityX, speedRatio);
            object->velocityZ = MulQ16(velocityZ, speedRatio);
          }
        }
      } else {
        // No target: brake by the acceleration until the object stops.
        // XXX: this branch calls the far-call stubs where the steering above
        // calls the IWRAM routines by address; both reach the same routines,
        // and only the bytes show the difference, not why.
        deltaX = object->velocityX;
        deltaZ = object->velocityZ;
        distance =
            IwramSqrtFarArm(MulQ16(deltaX, deltaX) + MulQ16(deltaZ, deltaZ))
            << 8;
        if (distance != 0) {
          if (distance - object->acceleration < 0) {
            object->velocityX = 0;
            object->velocityZ = 0;
          } else {
            speedRatio = IwramRatioMulQ14FarArm(
                distance, distance - object->acceleration);
            object->velocityX = MulQ16(deltaX, speedRatio);
            object->velocityZ = MulQ16(deltaZ, speedRatio);
          }
        } else {
          object->velocityX = 0;
          object->velocityZ = 0;
        }
      }

      // Gravity, and a bounce that loses energy on landing.
      if (object->motionFlags & 2) {
        groundY = object->terrainHeight;
        if (y > groundY)
          object->velocityY -= object->gravity;
        if (y <= groundY && object->velocityY < 0) {
          y = groundY;
          object->velocityY =
              -MulQ16(object->velocityY, object->vertical.bounce);
          if ((object->velocityY < 0 ? -object->velocityY
                                     : object->velocityY) <= object->gravity)
            object->velocityY = 0;
        }
      }
    }

    // Integrate velocity even when the acceleration and gravity step is frozen.
    x += object->velocityX;
    y += object->velocityY;
    z += object->velocityZ;

    // Arrived: the watched axis reached the target, or crossed it this frame
    // (the sign of the offset changed).
    switch (object->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == object->targetX ||
          ((object->x - object->targetX) ^ (x - object->targetX)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == object->targetY ||
          ((object->y - object->targetY) ^ (y - object->targetY)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == object->targetZ ||
          ((object->z - object->targetZ) ^ (z - object->targetZ)) < 0)
        arrived = 1;
      break;
    }
    // Snapping stops the object on the target; it takes the target's height
    // too only when its motion flags are all clear.
    if (arrived) {
      if (object->snapToTarget) {
        x = object->targetX;
        object->velocityX = 0;
        z = object->targetZ;
        object->velocityZ = 0;
        if (object->motionFlags == 0) {
          y = object->targetY;
          object->velocityY = 0;
        }
      }
      object->targetX = TARGET_UNSET;
      object->targetY = TARGET_UNSET;
      object->targetZ = TARGET_UNSET;
      object->arrivalAxis = 0;
    }
    SET_POSITION(object, x, y, z);

    // Turn to face the direction of travel.
    if (object->steeringFlags & 1) {
      x = object->velocityX;
      z = object->velocityZ;
      if (x != 0 || z != 0) {
        travelAngle = ArcTan2FarArm(z, x);
        // As a signed halfword the difference is the shorter way round.
        turnDelta = (s16)(travelAngle - object->facing);
        if (turnDelta > TURN_MAX)
          turnDelta = TURN_MAX;
        if (turnDelta < -TURN_MAX)
          turnDelta = -TURN_MAX;
        object->facing += turnDelta;
      }
    }
  }
}
