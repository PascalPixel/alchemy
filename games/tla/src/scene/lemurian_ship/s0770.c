#include "lemurian_ship.h"
#include "event_inline.h"

void Func_02000770(void) { Func_02002dd0(gData_02003010); }

void Func_02000780(void) { Func_02002dd8(); }

void Func_02000788(void) { Func_02002dd0(gData_02003010); }

void Func_02000798(void) {
  struct LemurianShipObject *object;
  s32 x;
  u8 *flags;
  s32 moving;

  Func_02002dd8();
  object = ((struct LemurianShipObject *)Actor_GetById(8));
  x = object->x >> 20;
  if (x == 16) {
    Func_02002c68();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(16, 47, 1, 1, x, 46);
    wait(1);
    flags = &object->motionFlags;
    *flags = 3;
    do {
      wait(1);
      moving = object->velocityY;
    } while (moving != 0);
    Sound_Play(188);
    wait(10);
    *flags = moving;
    GameFlag_SetBit(0x200);
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
    Func_02002c70();
  }
}

void Func_02000818(void) { Func_02002dc0(gData_02003014); }

void Func_02000828(void) { Func_02002dc0(gData_0200301e); }

void Func_02000838(void) {
  MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
  GameFlag_SetBit(0x212);
}

void Func_02000860(void) { Func_02002dc0(gData_02003034); }
