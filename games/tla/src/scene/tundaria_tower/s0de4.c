#include "tundaria_tower.h"
#include "scene_tables.h"

void Func_02002fc8(void);

void Func_02000de4(void) {
  Func_02002fc8();
  GameFlag_ClearBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Map_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
    Call6((void (*)())Map_CopyCellAttributeRect, 43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle;
      s32 owner = gMainRoutineWork.currentOwner;

      if (angle < 0)
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(600));
      else
        Actor_SetHorizontalPositionWithTerrain(owner, PIXELS(872), PIXELS(632));
    }
  }
}
