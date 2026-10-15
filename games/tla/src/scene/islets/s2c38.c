#include "islets.h"

void Func_02002c38(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *spawned;
  struct PartyState *party = &gPartyState;

  /* FAKEMATCH: the listing loads the party state address before it builds the
   * entrance offset */
  asm volatile("" : "+l"(party));
  if (party->entrance == 2)
    GameFlag_ClearBit(0x12f);
  map->mode = 1;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Object_GetById(10)->collisionFlags |= 0x80;
  if (GameFlag_Test(0xaa1)) {
    spawned = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003df4, 489,
                                             0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Func_02003e3c(spawned, 454);
      ObjectDispatch_SetSingleChildFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Object_GetById(11)->angle = 0xd000;
  } else {
    Object_SetModeById(11, 5);
  }
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02002bd8, 0x480);
}
