#include "types.h"

extern s32 gData_02003204, gData_02003208, gData_0200320c, gData_02003210;
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

/// Unless flag 0x104 is set, runs four 240-frame timers (gData_02003204 to
/// gData_02003210) that hold flags 0x200 to 0x203 set for their last 90
/// frames; in their first 150 the first two follow flags 0x320 and 0x321 and
/// the last two are clear.
void Func_02000740(void) {
  if (GameFlag_Test(0x104))
    return;
  if (gData_02003204 <= 149) {
    if (GameFlag_Test(0x320))
      GameFlag_SetBit(0x200);
    else
      GameFlag_ClearBit(0x200);
  } else {
    GameFlag_SetBit(0x200);
  }
  if (++gData_02003204 > 239)
    gData_02003204 = 0;
  if (gData_02003208 <= 149) {
    if (GameFlag_Test(0x321))
      GameFlag_SetBit(0x201);
    else
      GameFlag_ClearBit(0x201);
  } else {
    GameFlag_SetBit(0x201);
  }
  if (++gData_02003208 > 239)
    gData_02003208 = 0;
  if (gData_0200320c <= 149)
    GameFlag_ClearBit(0x202);
  else
    GameFlag_SetBit(0x202);
  if (++gData_0200320c > 239)
    gData_0200320c = 0;
  if (gData_02003210 <= 149)
    GameFlag_ClearBit(0x203);
  else
    GameFlag_SetBit(0x203);
  if (++gData_02003210 > 239)
    gData_02003210 = 0;
}
