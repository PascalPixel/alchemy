#include "jupiter_lighthouse_room_7.h"

void Scene_SetArrivalFlags(s32 flags);
void Func_02001224(s32 mode);
s32 Func_02000290(void);
void Func_020002f0(void);
s32 Func_02002b5c(s32 x, s32 z, s32 layer);
void Func_02002b54(s32 x, s32 z, s32 layer, s32 value);
void Func_02002c4c(s32 x, s32 z, s32 layer, s32 value);

s32 Func_02000fc0(void) {
  struct Actor *object;
  s32 value;

  Scene_SetArrivalFlags(0);
  Func_02001224(1);
  if (gMainRoutineWork.entrance == 12) {
    /* FAKEMATCH: the placements go through the call layer; called directly,
     * they share 0xa80000 and the z values in saved registers (differ 46). */
    Call3((void (*)())Func_02002ba4, 10, 0xa80000, 0x1080000);
    Call3((void (*)())Func_02002ba4, 11, 0xc80000, 0x1080000);
    Call3((void (*)())Func_02002ba4, 12, 0xe80000, 0x1080000);
    Call3((void (*)())Func_02002ba4, 13, 0xa80000, 0x1280000);
    Call3((void (*)())Func_02002ba4, 14, 0x880000, 0x1480000);
    Call3((void (*)())Func_02002ba4, 15, 0xa80000, 0x1480000);
  }
  if (!GameFlag_Test(0x109) && Func_02000290() != 2) {
    GameFlag_ClearBit(0xa10);
    GameFlag_ClearBit(0xa12);
    GameFlag_ClearBit(0xa14);
    GameFlag_ClearBit(0xa11);
    GameFlag_ClearBit(0xa13);
    GameFlag_ClearBit(0xa15);
  }
  if (Func_02000290() == 2)
    GameFlag_ClearBit(0xa16);
  else
    GameFlag_SetBit(0xa16);
  if (GameFlag_Test(0xa17))
    GameFlag_ClearBit(0xa1a);
  else
    GameFlag_SetBit(0xa1a);
  Actor_GetById(18)->motionFlags = 0;
  Actor_GetById(19)->motionFlags = 0;
  Actor_GetById(20)->motionFlags = 0;
  Func_020002f0();
  if (!GameFlag_Test(0xa1a)) {
    object = Actor_GetById(20);
    value = Func_02002b5c(object->x, object->z, 0);
    Func_02002b54(object->x, object->z, 0, value + 2);
    Func_02002c4c(object->x, object->z, 0, 0x80);
  }
  Func_020005e8(0);
  return 0;
}
