/// @file
/// A room's setup.

#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02003f4c(s32 actor);
void Actor_SetSpritePriority();

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

/// Sets up the room's actors and map work.
void Func_02003164(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(10);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->value190 = zero;
  map->value194 = zero;
  if (gMainRoutineWork.entrance == 11)
    GameFlag_ClearBit(0x12f);
  if (!GameFlag_Test(0x109)) {
    object->motionFlags = zero;
    Actor_SetSpritePriority(10, 1);
  }
  Func_02003edc(11, 0, 0);
  Func_02003f4c(12);
  Func_02003f4c(13);
  set = GameFlag_Test(0x212);
  if (set) {
    struct Actor *first = Actor_GetById(12);
    struct Actor *second = Actor_GetById(13);

    first->motionFlags = zero;
    second->motionFlags = zero;
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
    {
      s32 held = 29;

      Call6((void (*)())Func_02003e24, 7, 3, 1, 1, 7, held);
    }
  } else {
    struct Actor *first = Actor_GetById(12);
    struct Actor *second = Actor_GetById(13);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Func_020020ec(first->x, first->z, 3, 255);
    Func_020020ec(second->x, second->z, 3, 255);
  }
  Func_02003f4c(14);
  last = Actor_GetById(14);
  {
    // A local that holds 0, as the native keeps its own register for it.
    s32 still = 0;

    ((u8 *)last)[0x62] = 1;
    last->motionFlags = still;
  }
  Func_020020ec(last->x, last->z, 1, 255);
  if (GameFlag_Test(0x213))
    Call6((void (*)())Func_02003e24, 5, 2, 1, 2, 5, 28);
}
