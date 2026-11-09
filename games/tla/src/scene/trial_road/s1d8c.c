#include "trial_road.h"
#include "event_inline.h"

void Func_02005a08(void);
void Func_02005a10(s32 a, s32 b, s32 c);

void Func_02001d8c(void) {
  Func_02005a08();
  Func_02005a10(0, 15, 16);
  if (!GameFlag_Test(0x374))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
}

void Func_02001dd0(void) {}
