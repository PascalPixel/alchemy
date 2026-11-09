#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_020017f4(void);
void Func_02001c44(s32 a);

void Func_02003880(void) {
  Func_02001c44(18);
  if (GameFlag_Test(0xa21) != 0) {
    MapGrid_CopyCellAttributes(44, 40, 8, 3, 44, 36);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(30, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(31, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(32, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(33, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    if (GameFlag_Test(0x9b2) == 0) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    } else {
      Actor_GetById(10)->angle = 0xd000;
      wait(1);
    }
  } else {
    Func_020017f4();
  }
}
