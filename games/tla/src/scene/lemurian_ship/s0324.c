#include "lemurian_ship.h"

struct LemurianShipWarp {
  u16 exit;
  s32 value;
  u16 x;
  u16 z;
};

extern struct LemurianShipWarp gData_02003904[];
extern struct LemurianShipWarp gData_0200397c[];
void Func_02002c28(s32 value, s32 x, s32 z);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 a, s32 b);
void ObjectMotion_CenterOnColumn(s32 actor, s32 a, s32 b);

void Func_02000324(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object;
  u8 *entry;
  s32 offset;
  u32 found;
  u32 i;

  Func_02002c68();
  Func_02002da8(0);
  found = 0;
  i = 8;
next:
  object = Object_GetById(i);
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
  Func_02002de0(158);
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  LemurianShip_MapCopy(0, 0, gData_0200397c[found].x, gData_0200397c[found].z,
                       1, 2);
  Func_02002c28(gData_0200397c[found].value, gData_0200397c[found].x,
                gData_0200397c[found].z + 60);
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  ObjectMotion_CenterOnColumn(gPartyState.currentOwner, 2, -4);
  FieldEvent_WaitTicks(4);
  Func_02002d70(work->exit);
}
