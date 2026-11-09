#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02000c5c(void);

s32 Func_02000250(void) {
  struct Actor *object;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  if (gMainRoutineWork.entrance == 99) {
    GameFlag_SetBit(0x8aa);
    GameFlag_SetBit(0x8ab);
    gMainRoutineWork.secondary = 3;
    gMainRoutineWork.entrance = 3;
    Func_02000c5c();
  }
  if (GameFlag_Test(0x8a7)) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  } else if (GameFlag_Test(0x8ab)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    if (!GameFlag_Test(0x8bd)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  }
  if (GameFlag_Test(0x900))
    Actor_SetPosition(13, 0x1880000, 0x2880000);
  Actor_GetById(13)->priorityFlags |= 32;
  Func_080d3b28(15, 457);
  if (!GameFlag_Test(0x901)) {
    if (!GameFlag_Test(0x109)) {
      Actor_GetById(14)->priorityFlags |= 128;
      object = Actor_GetById(15);
      object->priorityFlags |= 64;
      object->y += 0x100000;
      object->terrainHeight += 0x100000;
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(14, 0x880000, 0x1db0000);
    if (!GameFlag_Test(0x902))
      Actor_SetPosition(15, 0x780000, 0x1d80000);
    else
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  }
  return 0;
}
