#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"
#include "event_inline.h"

struct LemurianShipWarp {
  u16 exit;
  s32 value;
  u16 x;
  u16 z;
};

extern struct LemurianShipWarp gData_02003904[];
extern struct LemurianShipWarp gData_0200397c[];

void Func_0200021c(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Func_02002c68();
  Event_ResolvePendingActions(0);
  found = 0;
  i = 8;
next:
  object = ((struct LemurianShipObject *)Actor_GetById(i));
  if (object != NULL)
    object->motionFlags = 0;
  i++;
  if (i < 64)
    goto next;
  entry = (u8 *)gData_02003904;
  i = 0;
  offset = 0;
next2:
  if (work->exit == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 10)
    goto next2;
  Sound_Play(158);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gData_02003904[found].x, gData_02003904[found].z, 1,
                      2);
  Map_PlayMapEdit(gData_02003904[found].value, gData_02003904[found].x,
                  gData_02003904[found].z + 60);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (found == 12)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  Event_Wait(4);
  Import_EventSetExitRequest(work->exit);
}
