#include "types.h"
#include "actor.h"
#include "iwram_call.h"

void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_SelectAnimation(struct Actor *object,
                                            s32 argument);

/// SC_APPROACH_WATCHED: follows the watched object at its speed. More than 16
/// pixels away it walks to 16 pixels short of it (animation 2) and runs on;
/// within 16 it stands (animation 1) and waits for the next frame.
// As ☀️'s function of the same name.
s32 Actor_ApproachWatchedActor(struct Actor *object) {
  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  /* FAKEMATCH(pin): the watched object held in the r2 the native keeps it in;
   * as a plain local, 15 of 65 instructions differ in tla-en. */
  register struct Actor *watchedObject asm("r2");

  watchedObject = object->watchedObject;
  object->speedLimit = watchedObject->speedLimit;
  object->acceleration = watchedObject->acceleration;
  {
    /* FAKEMATCH(temporary): the object's x in its own variable, which ⚓️
     * loads before the watched object's; as object->x in the subtraction
     * with no barrier, 2 of 65 instructions differ in tla-en. */
    s32 objectX = object->x;

    /* FAKEMATCH(barrier): the empty asm that holds the temporary's load
     * ahead; as the temporary alone, 9 of 65 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(objectX));
    dx = watchedObject->x - objectX;
  }
  dz = watchedObject->z - object->z;
  deltaXPixels = dx >> 0x10;
  deltaZPixels = dz >> 0x10;
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  if (distancePixels > 0x10) {
    remainingDistance = distancePixels - 0x10;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Actor_SetMoveTarget(object, object->x + moveX, object->y,
                         object->z + moveZ);
    Actor_SelectAnimation(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  Actor_SelectAnimation(object, 1);
  return 0;
}
