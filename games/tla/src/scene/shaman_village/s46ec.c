#include "shaman_village.h"

void Func_020046bc(void);

void Func_020046ec(void) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 521;
  if (GameFlag_Test(0x9fe)) {
    Task_Add(Func_020046bc, 0x480);
  } else {
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  }
}
