#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003198();
void Func_02003298(s32 actor, s32 value);
void Func_02000c5c(void);

static __inline__ void EasternAlhafraShip_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): EasternAlhafraShip_ObjPlace forwarded inline; as direct
   * calls to Func_02003198, 4 of 170 instructions differ in tla-en. */
  Func_02003198(actor, x, z);
}

s32 Func_02000250(void) {
  struct Actor *object;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  if (gMainRoutineWork.entrance == 99) {
    GameFlag_SetBit(SPLIT_CONSTANT(0x800, 170));
    GameFlag_SetBit(SPLIT_CONSTANT(0x800, 171));
    gMainRoutineWork.secondary = 3;
    gMainRoutineWork.entrance = 3;
    Func_02000c5c();
  }
  if (GameFlag_Test(SPLIT_CONSTANT(0x7a8, 255))) {
    Func_02003198(10, 0, 0);
    Func_02003198(11, 0, 0);
    Func_02003198(8, 0, 0);
    Func_02003198(9, 0, 0);
  } else if (GameFlag_Test(SPLIT_CONSTANT(0x800, 171))) {
    Func_02003198(8, 0, 0);
    Func_02003198(9, 0, 0);
    if (!GameFlag_Test(SPLIT_CONSTANT(0x800, 189))) {
      Func_02003198(10, 0, 0);
      Func_02003198(11, 0, 0);
    }
  } else {
    Func_02003198(10, 0, 0);
    Func_02003198(11, 0, 0);
  }
  if (GameFlag_Test(0x900))
    EasternAlhafraShip_ObjPlace(13, 0x1880000, 0x2880000);
  Actor_GetById(13)->priorityFlags |= 32;
  Func_02003298(15, 457);
  if (!GameFlag_Test(0x901)) {
    if (!GameFlag_Test(0x109)) {
      Actor_GetById(14)->priorityFlags |= 128;
      object = Actor_GetById(15);
      object->priorityFlags |= 64;
      object->y += 0x100000;
      object->terrainHeight += 0x100000;
    }
  } else {
    Func_02003198(14, 0x880000, 0x1db0000);
    if (!GameFlag_Test(0x902))
      EasternAlhafraShip_ObjPlace(15, 0x780000, 0x1d80000);
    else
      Func_02003198(15, 0, 0);
  }
  return 0;
}
