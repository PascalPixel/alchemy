#include "islets.h"
#include "event_inline.h"

struct Actor *Func_02000da0(struct Actor *object, s32 value) {
  struct Actor *spawned;

  spawned =
      FieldActor_Create(489, object->x, object->y + 0x180000, object->z);
  if (spawned != 0) {
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
    spawned->motionFlags = 0;
    Actor_SetItemIcon(spawned, value);
    Actor_SetSingleChildFlags(spawned, 0);
    spawned->unk5c[0] = 1;
  }
  return spawned;
}

void Func_02000df8(struct Actor *object) {
  struct Actor *spawned;

  if ((gFrameCount & 3) == 0) {
    spawned = FieldActor_Create(14, object->x, object->y, object->z);
    if (spawned != 0) {
      Actor_SetSingleChildFlags(spawned, 0);
      Func_02003de4(spawned, 1);
      Func_02003dec(spawned, gData_020044f0);
    }
  }
}
