#include "caverns.h"

void Func_02000848(struct FieldActor *object) {
  struct FieldActor *leader =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
  s32 leaderZ = leader->z;
  s32 z = object->z;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
    /* FAKEMATCH(temporary): the mask in a local set first, so that it takes
     * r3 and the flags byte r2; as *flags |= 4, differ 4. */
    s32 value = 4;

    value |= *flags;
    *flags = value;
  } else {
    *flags &= ~6;
  }
}
