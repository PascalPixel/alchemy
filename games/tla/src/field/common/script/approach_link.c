#include "types.h"
#include "object_runtime.h"
#include "iwram_call.h"

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectDispatch_SelectAnimation(struct ObjectRuntime *object,
                                            s32 argument);

// SC_APPROACH_LINK, as ☀️'s function of the same name: follows the linked
// object at its speed. More than 16 pixels away it walks to 16 pixels short
// of it (animation 2) and runs on; within 16 it stands (animation 1) and waits
// for the next frame.
s32 Object_ApproachLinkedObject(struct ObjectRuntime *object) {
  s32 distancePixels;
  s32 moveZ;
  s32 dx;
  s32 dz;
  s32 deltaZPixels;
  s32 deltaXPixels;
  s32 remainingDistance;
  s32 moveX;
  /* FAKEMATCH(pin): the linked object held in the r2 the native keeps it in;
   * as a plain local, 15 of 65 instructions differ in tla-en. */
  register struct ObjectRuntime *linkedObject asm("r2");

  linkedObject = object->linkedObject;
  object->speedLimit = linkedObject->speedLimit;
  object->acceleration = linkedObject->acceleration;
  {
    /* FAKEMATCH(temporary): the object's x in its own variable, which ⚓️
     * loads before the linked object's; as object->x in the subtraction
     * with no barrier, 2 of 65 instructions differ in tla-en. */
    s32 objectX = object->x;

    /* FAKEMATCH(barrier): the empty asm that holds the temporary's load
     * ahead; as the temporary alone, 9 of 65 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(objectX));
    dx = linkedObject->x - objectX;
  }
  dz = linkedObject->z - object->z;
  deltaXPixels = dx >> 0x10;
  deltaZPixels = dz >> 0x10;
  distancePixels =
      Iwram_Sqrt((deltaXPixels * deltaXPixels) + (deltaZPixels * deltaZPixels));
  if (distancePixels > 0x10) {
    remainingDistance = distancePixels - 0x10;
    moveX = dx * remainingDistance / distancePixels;
    moveZ = dz * remainingDistance / distancePixels;
    Object_SetMoveTarget(object, object->x + moveX, object->y,
                         object->z + moveZ);
    ObjectDispatch_SelectAnimation(object, 2);
    object->step = (object->step + 1);
    return 1;
  }
  ObjectDispatch_SelectAnimation(object, 1);
  return 0;
}
