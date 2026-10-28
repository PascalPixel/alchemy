#include "shrine_of_the_sea_god.h"

void Task_RemoveFar(void (*func)(void));
void Func_02001340(struct ShrineOfTheSeaGodObject *object,
                   struct ShrineOfTheSeaGodObject *target);
void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base);
void Func_020016c8(void);

void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base) {
  do {
    wait(1);
  } while (object->speed != 0);
  Func_02003b48(240);
  Actor_SetSpritePriority(base + 21, 3);
  Actor_SetSpritePriority(base + 22, 3);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 21), object->x, 0,
                                   object->z);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 22), object->x, 0,
                                   object->z + -0x100000);
  Actor_SetModeById(base + 21, 1);
  Actor_SetModeById(base + 22, 1);
  wait(20);
}

void Func_020016c8(void) { Func_0200142c(Actor_GetById(15)); }

void Func_020016d8(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gAdventure.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(15);

  Func_020039d8();
  Func_02003b08(0);
  ShrineOfTheSeaGod_ObjWalk(gAdventure.currentOwner, 328, 216);
  Func_02001340(leader, pillar);
  ShrineOfTheSeaGod_BlendStart(0, 0, 3, 1, 19, 13);
  ShrineOfTheSeaGod_BlendStart(40, 0, 2, 1, 19, 33);
  wait(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(15, 3);
  pillar->priorityFlags |= 2;
  Task_Add(Func_020016c8, 0x480);
  Func_02001658(leader, 0);
  Actor_SetSpritePriority(9, 1);
  Actor_GetById(9)->priorityFlags &= ~2;
  Actor_SetSpritePriority(11, 1);
  Actor_GetById(11)->priorityFlags &= ~2;
  leader->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     1);
  ShrineOfTheSeaGod_ObjSpeed(gAdventure.currentOwner, 0x9999, 0x1999);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 328, 224);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 321, 256);
  Func_02003a30(21, 0, 0);
  Func_02003a30(22, 0, 0);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 321, 320);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 321, 400);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 321, 480);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 321, 536);
  Func_02003b48(161);
  Actor_SetSpritePriority(9, 2);
  Actor_GetById(9)->priorityFlags |= 2;
  Actor_SetSpritePriority(11, 2);
  Actor_GetById(11)->priorityFlags |= 2;
  leader->motionFlags = 3;
  Task_RemoveFar(Func_020016c8);
  wait(1);
  Func_020039e0();
  GameFlag_SetBit(0x231);
}
