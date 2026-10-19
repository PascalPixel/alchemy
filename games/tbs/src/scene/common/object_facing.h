#include "script.h"
#include "types.h"
#include "facing_object.h"

// Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
// Elemental Star room, and Vale's Kraden, Babi, home and interiors scenes.

// An object script call: an actor with a facing target turns towards it, at
// most a sixteenth of a turn a frame.
static s32
OverlayObject_UpdateFacingTowardTarget(struct ScriptInterpreter *interpreter) {
  struct FacingObject *obj = (struct FacingObject *)interpreter;
  s32 delta;
  u16 old;
  s32 angle;
  struct FacingObject *target;

  target = obj->facingTarget;
  if (target != 0) {
    obj->facingFlags = (0xFE & obj->facingFlags);
    angle = (u16)ArcTan2(target->positionZ - obj->positionZ,
                         target->positionX - obj->positionX);
    old = obj->facing;
    delta = (s16)(angle - old);
    if (delta != 0) {
      if (delta > 0x1000) {
        delta = 0x1000;
      }

      if (delta < -0x1000) {
        delta = -0x1000;
      }
      obj->facing = (old + delta);
    }
  }
  return 1;
}
