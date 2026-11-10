#include "yampi_desert_cave.h"
#include "actor.h"
#include "event_inline.h"

void Func_02000e68(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = 12;

    Map_CopyCellAttributeRect(98, 3, 3, 3, 97, row);
    Map_CopyCellAttributeRect(33, 12, 1, 1, 34, row);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      MapGrid_CopyCellAttributes(102, 15, 1, 1, 103, 15);
      MapGrid_CopyCellAttributes(43, 14, 1, 1, 39, 16);
    }
    if (id == 13) {
      struct Actor *object = Actor_GetById(13);

      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      Map_CopyCellAttributeRect(7, 10, 1, 1, column, row);
    }
  }
}

/// Rooms 2 and 3: by where object id stands, sets flags 0x303 to 0x305 and
/// makes the rooms' Map_CopyCellAttributeRect calls.
void Func_02000f10(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = object->z >> 20;

    if (row == 13) {
      object->motionFlags = 0;
      GameFlag_SetBit(0x303);
      Map_CopyCellAttributeRect(32, 13, 1, 1, 34, row);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      Map_CopyCellAttributeRect(92, 3, 3, 3, 97, row);
      Map_CopyCellAttributeRect(32, 13, 1, 1, 34, row);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      s32 column;

      MapGrid_CopyCellAttributes(103, 16, 1, 1, 103, 15);
      column = object->x >> 20;
      if (column == 38) {
        GameFlag_SetBit(0x305);
        Map_CopyCellAttributeRect(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == 13) {
      if (object->x >> 20 == 10 && object->z >> 20 == 11)
        GameFlag_SetBit(0x304);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, object->x >> 20,
                                 object->z >> 20);
    }
  }
}
