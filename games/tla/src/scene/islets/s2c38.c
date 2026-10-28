#include "islets.h"

void Func_02002c38(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct Actor *spawned;
  struct Adventure *party = &gAdventure;

  /* FAKEMATCH: the listing loads the adventure's address before it builds the
   * entrance offset */
  asm volatile("" : "+l"(party));
  if (party->entrance == 2)
    GameFlag_ClearBit(0x12f);
  map->mode = 1;
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
  Actor_GetById(10)->collisionFlags |= 0x80;
  if (GameFlag_Test(0xaa1)) {
    spawned = (struct Actor *)Value4((s32 (*)())Func_02003df4, 489,
                                             0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Func_02003e3c(spawned, 454);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Actor_GetById(11)->angle = 0xd000;
  } else {
    Actor_SetModeById(11, 5);
  }
  Value2((s32 (*)())Task_Add, (s32)Func_02002bd8, 0x480);
}
