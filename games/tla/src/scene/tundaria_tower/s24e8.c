#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Func_02002f80(struct Actor *object, s32 mode);
void Func_020031a8(s32 cue);
void Func_020029d0(s32 a);
void Func_02002b04(void);
void wait(s32 frames);

void Func_020024e8(u16 *row, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;
  s32 nextFlag;

  if (*row == 0xffff)
    return;
loop:
  if (*row != id) {
    row += 2;
    goto test;
  }
  flag = row[1];
  Func_02003048();
  nextFlag = flag + 1;
  if (!GameFlag_Test(nextFlag)) {
    Func_020031a8(185);
    TundariaTower_SetWork(0x10000, 0x40000, 0x10000);
    Func_020029d0(0);
    wait(8);
    Func_02002f80(object, 2);
    Func_02002c1c(-4, object->terrainId, id);
    Func_020029d0(1);
    wait(16);
    TundariaTower_SetWork(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Func_020031a8(188);
    Func_02002b04();
    wait(20);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(nextFlag);
  }
  Func_02003050();
  return;
test:
  if (*row != 0xffff)
    goto loop;
}
