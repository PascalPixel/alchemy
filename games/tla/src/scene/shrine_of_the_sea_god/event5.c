#include "shrine_of_the_sea_god.h"

void Task_RemoveFar(void (*callback)(void));
void Func_02001340(struct ShrineOfTheSeaGodObject *object,
                   struct ShrineOfTheSeaGodObject *target);
void Func_02001658(struct ShrineOfTheSeaGodObject *object, s32 base);
void Func_020018ac(void);

void Func_020018ac(void) { Func_0200142c(Actor_GetById(16)); }

void Func_020018bc(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gAdventure.currentOwner);
  struct ShrineOfTheSeaGodObject *pillar = Actor_GetById(16);

  Func_020039d8();
  Func_02003b08(0);
  ShrineOfTheSeaGod_ObjWalk(gAdventure.currentOwner, 408, 360);
  Func_02001340(leader, pillar);
  ShrineOfTheSeaGod_BlendStart(0, 0, 3, 1, 24, 22);
  ShrineOfTheSeaGod_BlendStart(40, 0, 2, 1, 24, 24);
  WaitFrames(1);
  leader->motionFlags = 3;
  Actor_SetSpritePriority(16, 3);
  pillar->priorityFlags |= 2;
  Task_Add(Func_020018ac, 0x480);
  Func_02001658(leader, 2);
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     1);
  ShrineOfTheSeaGod_ObjSpeed(gAdventure.currentOwner, 0x9999, 0x1999);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 406, 360);
  ShrineOfTheSeaGod_ObjCommit(gAdventure.currentOwner, 400, 392);
  Func_02003b48(161);
  Func_02003a30(23, 0, 0);
  Func_02003a30(24, 0, 0);
  Task_RemoveFar(Func_020018ac);
  WaitFrames(1);
  Func_020039e0();
  GameFlag_SetBit(0x232);
}
