#include "kibombo.h"

void Func_02002de0(void);
void Func_0200324c(void);

void Func_0200538c(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  if (!GameFlag_Test(0x8fb))
    Func_02002de0();
  Func_0200324c();
  if (!GameFlag_Test(0x869))
    GameFlag_SetBit(0x201);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->motionFlags = 0;
  if (GameFlag_Test(0x9ea)) {
    Kibombo_ObjSetPos(Actor_GetById(11), 208 << 15, 0, 148 << 17);
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 0x20;
  if (GameFlag_Test(0x86a))
    Func_020005e0();
  if (GameFlag_Test(0x204))
    Func_02000624();
  if (GameFlag_Test(0x205))
    Func_02000668();
  if (GameFlag_Test(0x206))
    Func_020006ac();
  if (GameFlag_Test(0x207))
    Func_020006f0();
  Func_02005b8c(12);
  Func_02005b8c(13);
  Func_02005b8c(14);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
}
