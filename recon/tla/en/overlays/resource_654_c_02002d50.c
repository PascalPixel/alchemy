// Near miss, differ 6 of 106: the touch dispatch of the wall room (☀️'s
// EV13_TOUCH shape): by the exit touched, 13 to 20, it runs one of the
// room's events when object 14's state allows. Left: the native loads the
// event work before it sets the Object_GetById argument (one row), and it
// forms the state pointer (adds r0, #98) in the jump-table block after the
// range check; formed before the switch it comes ahead of the check, and
// formed in each case it stays there (differ 58).
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
  u8 *state;

  if (!GameFlag_Test(0x87c))
    return;
  if (GameFlag_Test(115))
    return;
  work = Ram_HeapSlots->eventWork;
  state = &Object_GetById(14)->state;
  switch (work->exit) {
  case 13:
    if (*state == 0)
      Func_020027f8();
    GameFlag_ClearBit(0x206);
    break;
  case 14:
    if (*state == 0)
      Func_020029a4();
    break;
  case 15:
    if (*state == 1)
      Func_02002830();
    else if (*state == 4 && !GameFlag_Test(0x206))
      Func_02002b50();
    break;
  case 16:
    if (*state == 1)
      Func_02002b18();
    else if (*state == 5)
      Func_02002c50();
    break;
  case 17:
    if (*state == 2)
      Func_0200291c();
    else if (*state == 4)
      Func_02002c18();
    break;
  case 18:
    if (*state == 2)
      Func_02002a2c();
    else if (*state == 5 && !GameFlag_Test(0x206))
      Func_02002c88();
    break;
  case 19:
    if (*state == 3)
      Func_0200296c();
    break;
  case 20:
    if (*state == 3)
      Func_020029dc();
    GameFlag_ClearBit(0x206);
    break;
  }
}
