// Near miss of Func_0200021c (listing 115 instructions, draft 117, 56 differ):
// the native keeps the found row in r7 and the event work in r9, the draft the
// reverse, and the native builds the speed pair after loading the owner. The
// two scans are goto loops (nothing is moved out of them) and the second walks
// an explicit byte offset, as in Func_02000324.
#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"

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

static inline void LemurianShip_ObjOffset(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_OffsetPositionAndResetMotion(actor, a, b);
}

static inline void LemurianShip_ObjCenter(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_CenterOnColumn(actor, a, b);
}

void Func_0200021c(void) {
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
  Func_02002de0(158);
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  LemurianShip_MapCopy(0, 0, gData_02003904[found].x, gData_02003904[found].z,
                       1, 2);
  Func_02002c28(gData_02003904[found].value, gData_02003904[found].x,
                gData_02003904[found].z + 60);
  Object_SetModeById(gPartyState.currentOwner, 2);
  LemurianShip_ObjSpeed(gPartyState.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gPartyState.currentOwner;

    if (found == 12)
      LemurianShip_ObjOffset(owner, 0, -4);
    else
      LemurianShip_ObjCenter(owner, 2, -4);
  }
  FieldEvent_WaitTicks(4);
  Func_02002d70(work->exit);
}
