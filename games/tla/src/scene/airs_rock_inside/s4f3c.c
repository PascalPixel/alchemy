#include "types.h"
#include "actor.h"

extern s32 gData_020073d8;

s32 Func_02005f84(void);
struct Actor *Actor_GetById(u32 id);
void Func_02005dac(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);

void Func_02004f3c(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 level;

  actor->priorityFlags |= 32;
  if (actor->y + 0x200000 >= 0x1380000)
    level = 2;
  else if (actor->y + 0x200000 >= 0xd80000)
    level = 1;
  else
    level = 0;
  if (gData_020073d8 == level)
    return;
  switch (level) {
  case 2: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 64, 6, 4, a4, a5);
  }
    Func_02005dac(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 68, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 a4 = 7, a5 = 39;
    Func_02005dac(64, 72, 6, 4, a4, a5);
  }
    Func_02005dac(70, 68, 6, 4, 26, 39);
    break;
  }
  gData_020073d8 = level;
}
