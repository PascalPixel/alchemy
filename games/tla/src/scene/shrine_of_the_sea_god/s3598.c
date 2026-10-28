#include "shrine_of_the_sea_god.h"
#include "call.h"

void Func_02002f88(s32 actor);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 flag, s32 b, s32 c);

/// The Shrine of the Sea God's prepare step: sets up actors 8 to 24 and,
/// for each of flags 0x231 and 0x232 that is set, copies its opened tiles
/// and places actor 15 or 16 where it rests.
void Func_02003598(void) {
  struct ShrineOfTheSeaGodObject *object;
  struct ShrineOfTheSeaGodObject *lastObject;

  ((struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork)->mode17 = 1;
  Call1((void (*)())Func_02002f88, 8);
  Call1((void (*)())Func_02002f88, 9);
  Call1((void (*)())Func_02002f88, 10);
  Call1((void (*)())Func_02002f88, 11);
  Call1((void (*)())Func_02002f88, 12);
  Call1((void (*)())Func_02002f88, 13);
  Call1((void (*)())Func_02002f88, 14);
  Call1((void (*)())Func_02002f88, 15);
  Call1((void (*)())Func_02002f88, 16);
  Call2((void (*)())Actor_SetModeById, 13, 0);
  Call2((void (*)())Actor_SetModeById, 14, 0);
  Call2((void (*)())Actor_SetModeById, 15, 2);
  Call2((void (*)())Actor_SetModeById, 16, 2);
  if (GameFlag_Test(0x231)) {
    Func_020039a8(0, 0, 3, 1, 19, 13);
    Func_020039a8(40, 0, 2, 1, 19, 33);
    Actor_SetPositionAndResetMotion(Actor_GetById(15), 0x1400000, -0x150000,
                                     0x2160000);
    Actor_SetSpritePriority(15, 3);
  }
  if (GameFlag_Test(0x232)) {
    Func_020039a8(0, 0, 3, 1, 24, 22);
    Func_020039a8(40, 0, 2, 1, 24, 24);
    Actor_SetPositionAndResetMotion(Actor_GetById(16), 0x1900000, -0x150000,
                                     0x1860000);
    Actor_SetSpritePriority(16, 3);
  }
  Func_02003b18();
  Call4((void (*)())Func_02003b20, 0, 0x203, 17, 18);
  Call4((void (*)())Func_02003b20, 1, 0x204, 19, 20);
  Call1((void (*)())Func_02003aa0, 21);
  Actor_GetById(21)->motionFlags = 0;
  object = Actor_GetById(22);
  Call1((void (*)())Func_02003aa0, 22);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Call1((void (*)())Func_02003aa0, 23);
  Actor_GetById(23)->motionFlags = 0;
  /* FAKEMATCH(temporary): actor 24 fetched into its own variable; as
   * object = Actor_GetById(24), 51 of 171 instructions differ in tla-en. */
  lastObject = Actor_GetById(24);
  object = lastObject;
  Call1((void (*)())Func_02003aa0, 24);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
}
