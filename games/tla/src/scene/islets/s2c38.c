#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002c38(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  map->mode = 1;
  Actor_SetSingleChildFlags(Actor_Get(11), 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_Get(10)->collisionFlags |= 0x80;
  if (Event_GetFlag(0xaa1)) {
    spawned = FieldActor_Create(489, 0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Actor_SetItemIcon(spawned, 454);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Actor_Get(11)->angle = FACING_NORTH + FACING_STEP;
  } else {
    Actor_SetAnimation(11, 5);
  }
  Task_AddFrame(Func_02002bd8, 0x480);
}
