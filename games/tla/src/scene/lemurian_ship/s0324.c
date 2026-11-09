#include "lemurian_ship.h"
#include "event_inline.h"

struct LemurianShipWarp {
  u16 exit;
  s32 value;
  u16 x;
  u16 z;
};

extern struct LemurianShipWarp gData_02003904[];
extern struct LemurianShipWarp gData_0200397c[];

void Func_02000324(void) {
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
  entry = (u8 *)gData_0200397c;
  i = 0;
  offset = 0;
next2:
  if (work->exit == *(u16 *)(entry + offset))
    found = i;
  i++;
  offset += 12;
  if (i < 4)
    goto next2;
  if (found < 2 && GameFlag_Test(0x93f) && GameFlag_Test(0x9e4) == 0)
    found += 4;
  Sound_Play(158);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  MapGrid_CopyCellsTo(0, 0, gData_0200397c[found].x, gData_0200397c[found].z, 1,
                      2);
  Map_PlayMapEdit(gData_0200397c[found].value, gData_0200397c[found].x,
                  gData_0200397c[found].z + 60);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_CenterOnColumn(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(4);
  Import_EventSetExitRequest(work->exit);
}
