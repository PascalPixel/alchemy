#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_02003d30(void);
void Func_02003f78(void);

void Func_02004060(void) {
  if (GameFlag_Test(0x94c)) {
    MapGrid_CopyCellsTo(47, 19, 38, 19, 9, 11);
    MapGrid_CopyCellAttributes(1, 5, 9, 1, 6, 29);
    Actor_SetPartAttribute(((struct ShamanVillageObject *)Actor_GetById(14)), 6);
  } else if (!GameFlag_Test(0x109)) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(15, PIXELS(168), PIXELS(568), angle);
    Actor_SetPositionAndFacing(17, PIXELS(216), PIXELS(536), angle);
    Actor_SetHorizontalPositionAndAngleWithTerrain(16, PIXELS(120), PIXELS(536), angle);
    wait(1);
  }
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  if (!GameFlag_Test(0x109)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 11)
      Func_02003d30();
    else if (entrance == 9)
      Func_02003f78();
  }
  if (GameFlag_Test(0x9fe)) {
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    Map_CopyCellAttributeRect(10, 12, 3, 1, 10, 13);
    Map_CopyCellAttributeRect(15, 12, 3, 1, 15, 13);
    MapGrid_CopyCellAttributes(9, 20, 3, 1, 9, 21);
  }
}

void Func_020041b8(void) {
  struct ShamanVillageObject *object;

  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  ((struct ShamanVillageObject *)Actor_GetById(14))->motionFlags = 0;
  if (GameFlag_Test(0x94e)) {
    Actor_SetPositionAndResetMotion(((struct ShamanVillageObject *)Actor_GetById(14)), PIXELS(520), PIXELS(32),
                                     PIXELS(184));
    wait(1);
    MapGrid_CopyCellAttributes(30, 11, 1, 1, 32, 11);
    wait(10);
  }
  if (GameFlag_Test(0x9e1)) {
    Map_CopyCellPictureIndicesRect(64, 0, 76, 2, 3, 3);
    MapGrid_CopyCellAttributes(12, 5, 3, 1, 12, 6);
  }
  if (GameFlag_Test(0x94d))
    MapGrid_CopyCellsTo(48, 40, 101, 11, 1, 2);
  else
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  object = ((struct ShamanVillageObject *)Actor_GetById(16));
  object->collisionFlags = 8;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSingleChildFlags(((struct ShamanVillageObject *)Actor_GetById(16)), 0);
}
