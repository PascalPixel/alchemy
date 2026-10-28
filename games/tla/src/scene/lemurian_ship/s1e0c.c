#include "lemurian_ship.h"
#include "flag_call.h"

void Func_020013f0(void);
void Func_02001944(s32 id, s32 platform);
void Func_020019ac(void);
void Func_02001dc8(s32 id);

/// Places a platform object at its raised position.
#define RAISE(id, x, z)                                                        \
  {                                                                            \
    LemurianShip_ObjSetPos(Actor_GetById(id), x, -0x200000, z);               \
    Actor_GetById(id)->height = -0x200000;                                    \
  }

void Func_02001e0c(void) {
  Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 32;
  Func_02002dc8(gData_02003010);
  Actor_GetById(8)->priorityFlags |= 32;
  Actor_SetSpritePriority(8, 1);
  Actor_GetById(9)->priorityFlags |= 32;
  if (GameFlag_Test(0x932)) {
    struct LemurianShipObject *object = Actor_GetById(15);

    object->motionFlags = 0;
    LemurianShip_ObjSetPos(Actor_GetById(15), 206 << 18, -0x200000, 144 << 15);
    object->height = -0x200000;
    if (GameFlag_Test(0x212))
      LemurianShip_BlendStart(110, 1, 3, 1, 110, 4);
    else
      LemurianShip_BlendStart(110, 0, 3, 1, 110, 4);
    if (!GameFlag_Test(0x222))
      Func_02002cd0(9, 0, 0);
  }
  if (GameFlag_Test(0x983)) {
    if (GameFlag_Test(0x207)) {
      Func_02002cd0(15, 0, 0);
      Func_02002c30(68, 68, 50, 64, 3, 3);
      LemurianShip_BlendStart(102, 16, 3, 2, 110, 4);
      LemurianShip_BlendStart(50, 0, 3, 1, 50, 4);
    } else {
      RAISE(15, 206 << 18, 144 << 15)
      Func_02002c30(72, 68, 50, 64, 3, 3);
      LemurianShip_BlendStart(106, 16, 3, 2, 110, 4);
      LemurianShip_BlendStart(50, 1, 3, 1, 50, 4);
      if (GameFlag_Test(0x212))
        LemurianShip_BlendStart(110, 1, 3, 1, 110, 4);
      else
        LemurianShip_BlendStart(110, 0, 3, 1, 110, 4);
    }
  }
  if (GameFlag_Test(0x933)) {
    LemurianShip_BlendStart(42, 0, 1, 1, 46, 5);
    LemurianShip_BlendStart(43, 0, 1, 1, 45, 5);
    LemurianShip_BlendStart(43, 0, 1, 1, 47, 5);
    RAISE(16, 186 << 18, 176 << 15)
    if (!GameFlag_Test(0x223))
      Func_02002cd0(10, 0, 0);
  } else {
    Func_02001dc8(10);
  }
  if (GameFlag_Test(0x934)) {
    struct LemurianShipObject *block = Actor_GetById(8);

    LemurianShip_BlendStart(42, 0, 1, 1, 47, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 46, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 48, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 47, 10);
    LemurianShip_BlendStart(43, 0, 1, 1, 47, 8);
    RAISE(17, 190 << 18, 152 << 16)
    if (block->x >> 20 == 47 && block->z >> 20 == 8)
      Func_02002dc8(gData_02003010);
    if (!GameFlag_Test(0x224))
      Func_02002cd0(11, 0, 0);
  } else {
    Func_02001dc8(11);
  }
  if (GameFlag_Test(0x935)) {
    struct LemurianShipObject *block = Actor_GetById(8);
    s32 x = block->x >> 20;
    s32 z = block->z >> 20;

    LemurianShip_BlendStart(42, 0, 1, 1, 49, 7);
    LemurianShip_BlendStart(43, 0, 1, 1, 49, 6);
    LemurianShip_BlendStart(43, 0, 1, 1, 50, 7);
    LemurianShip_BlendStart(43, 0, 1, 1, 48, 7);
    LemurianShip_BlendStart(43, 0, 1, 1, 49, 8);
    RAISE(18, 198 << 18, 240 << 15)
    if ((x == 48 && z == 7) || (x == 49 && z == 8))
      Func_02002dc8(gData_02003010);
    if (!GameFlag_Test(0x225))
      Func_02002cd0(12, 0, 0);
  } else {
    Func_02001dc8(12);
  }
  if (GameFlag_Test(0x936)) {
    LemurianShip_BlendStart(42, 0, 1, 1, 51, 7);
    RAISE(19, 206 << 18, 240 << 15)
    if (!GameFlag_Test(0x226))
      Func_02002cd0(13, 0, 0);
  } else {
    Func_02001dc8(13);
  }
  if (GameFlag_Test(0x937)) {
    LemurianShip_BlendStart(42, 0, 1, 1, 51, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 51, 8);
    LemurianShip_BlendStart(43, 0, 1, 1, 50, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 52, 9);
    LemurianShip_BlendStart(43, 0, 1, 1, 51, 10);
    RAISE(20, 206 << 18, 152 << 16)
    if (!GameFlag_Test(0x227))
      Func_02002cd0(14, 0, 0);
  } else {
    Func_02001dc8(14);
  }
  Func_02002db8(gData_0200301e);
  Task_Add(Func_020013f0, 0x480);
  if (Flag_IsSet(0x222)) {
    Func_02001944(9, 15);
    Flag_Clear(0x222);
  } else if (Flag_IsSet(0x223)) {
    Func_02001944(10, 16);
    Flag_Clear(0x223);
  } else if (Flag_IsSet(0x224)) {
    Func_02001944(11, 17);
    Flag_Clear(0x224);
  } else if (Flag_IsSet(0x225)) {
    Func_02001944(12, 18);
    Flag_Clear(0x225);
  } else if (Flag_IsSet(0x226)) {
    Func_02001944(13, 19);
    Flag_Clear(0x226);
  } else if (Flag_IsSet(0x227)) {
    Func_02001944(14, 20);
    Flag_Clear(0x227);
  }
  if (gSequenceParentWork.entrance == 26 && !GameFlag_Test(0x109))
    Func_020019ac();
}
