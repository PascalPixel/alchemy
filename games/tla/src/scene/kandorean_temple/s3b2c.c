#include "local.h"
#include "event_inline.h"
#include "version.h"

void Func_02003ae4(s32 id, s32 base);

void Func_02003b2c(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x100;
  if (GameFlag_Test(0x842)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  } else {
    Func_020052c0(8);
  }
  if (GameFlag_Test(0x849)) {
    MapGrid_CopyCellAttributes(2, 0, 2, 3, 17, 24);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetPositionAndFacing(11, PIXELS(374), PIXELS(282), FACING_EAST);
    {
      /* FAKEMATCH: a temporary builds the angle ahead of the other arguments
       * (three reordered rows as a plain argument). */
      s32 angle = 0xd000;

      Actor_SetPositionAndFacing(12, PIXELS(272), PIXELS(296), angle);
    }
    {
      /* FAKEMATCH: a temporary builds the angle ahead of the other arguments
       * (three reordered rows as a plain argument). */
      s32 angle = 0x3000;

      Actor_SetPositionAndFacing(13, PIXELS(216), PIXELS(264), angle);
    }
  } else {
    Func_02003ae4(11, 0);
    Func_02003ae4(12, 240);
    Func_02003ae4(13, 480);
  }
  if (GameFlag_Test(0x212)) {
    Map_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
    MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4)
    GameFlag_ClearBit(0x12f);
#endif
}
