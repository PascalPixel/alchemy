// Matches (tla-en, 2026-11-07).
// 7 November: the last actor's zero as a block-local constant (from the permuter) gives
// the native's registers there (20 to 0); no device.
// Before: differ 20 of 134 (tla-en, 2026-11-05), no device; left: the last actor, native r1 with r0 walked to its 0x62 byte.
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
