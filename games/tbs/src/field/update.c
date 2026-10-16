#include "unit/field/update.h"
#include "types.h"
#include "irq.h"
#include "iwram_call.h"
#include "mulq16.h"
#include "script_motion.h"

#define OBJECT_SLOTS 8 // script-driven objects updated each frame
#define TARGET_UNSET ((s32)0x80000000) // no movement target
#define TURN_MAX 0x1000                // the most an object turns in one frame

// Calls a resident IWRAM routine at its absolute address, which this code
// still reaches when it runs from its copy in RAM.
#define IWRAM_ENTRY(type, offset) ((type)(IwramIrqMain + (offset)))
#define IwramSqrt IWRAM_ENTRY(s32 (*)(s32 value), Iwram_SqrtOffset)
#define IwramRatioMulQ14                                                       \
  IWRAM_ENTRY(s32 (*)(s32 divisor, s32 value), Iwram_RatioMulQ14Offset)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern struct ScriptMotionObject *gObjectSlots;
extern ScriptCommandFn gScriptObject_CommandTable[];

// Far-call stubs copied to RAM together with this code (updfar.s).
s32 ArcTan2FarArm(s32 z, s32 x);
s32 IwramSqrtFarArm(s32 value);
s32 IwramRatioMulQ14FarArm(s32 divisor, s32 value);

// Advances the eight script-driven objects by one frame. For each live one:
// run its hook, then its script (unless it waits or is paused); steer it
// towards its target, or brake it when it has none; apply gravity and
// bounce off the ground; detect arrival at the target; and turn it to face
// the way it moves. Copied to RAM and run from there.
void Object_UpdateAll(void) {
  struct ScriptMotionObject *obj;
  s32 y;
  s32 arrived;
  struct ScriptMotionObject *nextObj;
  s32 nextIndex;
  s32 i;
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s32 vx;
  s32 dist;
  s32 ratio;
  s32 ground;
  s32 turn;
  u16 angle;

  for (i = 0, obj = gObjectSlots; i < OBJECT_SLOTS;
       i = nextIndex, obj = nextObj) {
    nextIndex = i + 1;
    nextObj = obj + 1;
    if (obj->script == NULL)
      continue;
    if (obj->hook != NULL)
      obj->hook((union FieldObject *)obj);
    if (obj->script == NULL)
      continue;
    if (obj->paused != 0)
      continue;

    // Count down a wait, or run commands until one yields for this frame.
    // Words above 63 are operands, which the cursor steps over.
    if (obj->wait != 0) {
      obj->wait--;
    } else {
      for (;;) {
        s32 cursor = (s16)obj->scriptCursor;
#ifdef UBFIX
        u32 command = obj->script[cursor];
#else
        u32 command = obj->script->word[cursor];
#endif

        if (command <= 63) {
          if (gScriptObject_CommandTable[command](obj) == 0)
            break;
        } else {
          obj->scriptCursor++;
        }
      }
      if (obj->script == NULL)
        continue;
    }

    x = obj->x;
    y = obj->y;
    z = obj->z;
    if (obj->frozen == 0) {
      arrived = 0;
      if (obj->targetX != TARGET_UNSET) {
        // Accelerate towards the target. The first distance is in whole
        // pixels, to stay in range; under 256 pixels it is measured again in
        // 16.16.
        dx = (obj->targetX - x) / 65536;
        dz = (obj->targetZ - z) / 65536;
        dist = IwramSqrt(dx * dx + dz * dz) << 16;
        if (dist <= 0xffffff) {
          dx = obj->targetX - x;
          dz = obj->targetZ - z;
          dist = IwramSqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
        }
        if (dist == 0) {
          x = obj->targetX;
          z = obj->targetZ;
        } else {
          ratio = IwramRatioMulQ14(dist, obj->acceleration);
          vx = obj->velocityX + MulQ16(dx, ratio);
          obj->velocityX = vx;
          // dx now carries the new z velocity.
          dx = obj->velocityZ + MulQ16(dz, ratio);
          obj->velocityZ = dx;
          dist = IwramSqrt(MulQ16(vx, vx) + MulQ16(dx, dx)) << 8;
          if (dist > obj->speedLimit) {
            // Too fast: scale the velocity back to the speed limit.
            ratio = IwramRatioMulQ14(dist, obj->speedLimit);
            obj->velocityX = MulQ16(vx, ratio);
            obj->velocityZ = MulQ16(dx, ratio);
          }
        }
      } else {
        // No target: brake by the acceleration until the object stops.
        dx = obj->velocityX;
        dz = obj->velocityZ;
        dist = IwramSqrtFarArm(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
        if (dist != 0) {
          if (dist - obj->acceleration < 0) {
            obj->velocityX = 0;
            obj->velocityZ = 0;
          } else {
            ratio = IwramRatioMulQ14FarArm(dist, dist - obj->acceleration);
            obj->velocityX = MulQ16(dx, ratio);
            obj->velocityZ = MulQ16(dz, ratio);
          }
        } else {
          obj->velocityX = 0;
          obj->velocityZ = 0;
        }
      }

      // Gravity, and a bounce that loses energy on landing.
      if (obj->motionFlags & 2) {
        ground = obj->terrainHeight;
        if (y > ground)
          obj->velocityY -= obj->gravity;
        if (y <= ground && obj->velocityY < 0) {
          y = ground;
          obj->velocityY = -MulQ16(obj->velocityY, obj->vertical.bounce);
          if ((obj->velocityY < 0 ? -obj->velocityY : obj->velocityY) <=
              obj->gravity)
            obj->velocityY = 0;
        }
      }
    }

    x += obj->velocityX;
    y += obj->velocityY;
    z += obj->velocityZ;

    // Arrived: the watched axis reached the target, or crossed it this frame
    // (the sign of the offset changed).
    switch (obj->arrivalAxis) {
    case ARRIVAL_AXIS_X:
      if (x == obj->targetX ||
          ((obj->x - obj->targetX) ^ (x - obj->targetX)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Y:
      if (y == obj->targetY ||
          ((obj->y - obj->targetY) ^ (y - obj->targetY)) < 0)
        arrived = 1;
      break;
    case ARRIVAL_AXIS_Z:
      if (z == obj->targetZ ||
          ((obj->z - obj->targetZ) ^ (z - obj->targetZ)) < 0)
        arrived = 1;
      break;
    }
    if (arrived) {
      if (obj->snapToTarget) {
        x = obj->targetX;
        obj->velocityX = 0;
        z = obj->targetZ;
        obj->velocityZ = 0;
        if (obj->motionFlags == 0) {
          y = obj->targetY;
          obj->velocityY = 0;
        }
      }
      obj->targetX = TARGET_UNSET;
      obj->targetY = TARGET_UNSET;
      obj->targetZ = TARGET_UNSET;
      obj->arrivalAxis = 0;
    }
    obj->x = x;
    obj->y = y;
    obj->z = z;

    // Turn to face the direction of travel.
    if (obj->steeringFlags & 1) {
      x = obj->velocityX;
      z = obj->velocityZ;
      if (x != 0 || z != 0) {
        angle = ArcTan2FarArm(z, x);
        turn = (s16)(angle - obj->facing);
        if (turn > TURN_MAX)
          turn = TURN_MAX;
        if (turn < -TURN_MAX)
          turn = -TURN_MAX;
        obj->facing += turn;
      }
    }
  }
}
