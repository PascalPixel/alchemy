#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"

void Func_020027f8(void);
void Func_02002830(void);
void Func_0200291c(void);
void Func_0200296c(void);
void Func_020029a4(void);
void Func_020029dc(void);
void Func_02002a2c(void);
void Func_02002b18(void);
void Func_02002b50(void);
void Func_02002c18(void);
void Func_02002c50(void);
void Func_02002c88(void);

void Func_02002d50(void) {
  struct ShrineOfTheSeaGodEventWork *work;
  struct ShrineOfTheSeaGodObject *object;

  if (!GameFlag_Test(SPLIT_CONSTANT(SPLIT_CONSTANT(0x800, 124))))
    return;
  if (GameFlag_Test(115))
    return;
  work = Ram_HeapSlots->eventWork;
  object = Actor_GetById(14);
  switch (work->exit) {
  case 13:
    if (object->state == 0)
      Func_020027f8();
    GameFlag_ClearBit(0x206);
    break;
  case 14:
    if (object->state == 0)
      Func_020029a4();
    break;
  case 15:
    {
      u8 current = object->state;

      if (current == 1)
        Func_02002830();
      else if (current == 4 && !GameFlag_Test(0x206))
        Func_02002b50();
    }
    break;
  case 16:
    {
      u8 current = object->state;

      if (current == 1)
        Func_02002b18();
      else if (current == 5)
        Func_02002c50();
    }
    break;
  case 17:
    {
      u8 current = object->state;

      if (current == 2)
        Func_0200291c();
      else if (current == 4)
        Func_02002c18();
    }
    break;
  case 18:
    {
      u8 current = object->state;

      if (current == 2)
        Func_02002a2c();
      else if (current == 5 && !GameFlag_Test(SPLIT_CONSTANT(0x206)))
        Func_02002c88();
    }
    break;
  case 19:
    if (object->state == 3)
      Func_0200296c();
    break;
  case 20:
    if (object->state == 3)
      Func_020029dc();
    GameFlag_ClearBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x206)));
    break;
  }
}
