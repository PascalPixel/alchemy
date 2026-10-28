// Draft of overlay 674's Func_02003164, differ 91 of 134 (13 more
// instructions): a room's setup written from the listing. The native keeps the
// map work in r5, a zero for the motion flags in r6 and object 10 in r7, and
// reuses the flag result as the zero in the else branch; this draft keeps the
// zero in r8. alchemy permute (5 minutes) did not close it.
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02003f4c(s32 actor);
void Actor_SetSpritePriority();

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

void Func_02003164(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(10);
  struct Actor *first;
  struct Actor *second;
  struct Actor *last;
  s32 set;

  map->value190 = 0;
  map->value194 = 0;
  if (gSequenceParentWork.entrance == 11)
    GameFlag_ClearBit(0x12f);
  if (!GameFlag_Test(0x109)) {
    object->motionFlags = 0;
    Actor_SetSpritePriority(10, 1);
  }
  Func_02003edc(11, 0, 0);
  Func_02003f4c(12);
  Func_02003f4c(13);
  set = GameFlag_Test(0x212);
  if (set) {
    first = Actor_GetById(12);
    second = Actor_GetById(13);
    first->motionFlags = 0;
    second->motionFlags = 0;
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
    Call6((void (*)())Func_02003e24, 7, 3, 1, 1, 7, 29);
  } else {
    first = Actor_GetById(12);
    second = Actor_GetById(13);
    first->motionFlags = 0;
    second->motionFlags = 0;
    Func_020020ec(first->x, first->z, 3, 255);
    Func_020020ec(second->x, second->z, 3, 255);
  }
  Func_02003f4c(14);
  last = Actor_GetById(14);
  ((u8 *)last)[0x62] = 1;
  last->motionFlags = 0;
  Func_020020ec(last->x, last->z, 1, 255);
  if (GameFlag_Test(0x213))
    Call6((void (*)())Func_02003e24, 5, 2, 1, 2, 5, 28);
}
