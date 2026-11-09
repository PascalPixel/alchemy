#include "ankohl_ruins.h"

/// Copies the cells the object with the given id has pushed open in rooms 1,
/// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
/// track.
void Func_02000850(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (object->x >> 20 == 4) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 2, 69);
      Event_SetFlag(0x9c6);
    }
    AnkohlRuins_Call2dfc(3, 68, 1, 1, 4, 69);
    AnkohlRuins_Call2dfc(3, 69, 1, 1, 3, 70);
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    AnkohlRuins_Call2dfc(31, 70, 1, 1, 32, 70);
    if (object->x >> 20 == 32) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(0x9c7);
    }
  }
  /* FAKEMATCH(barrier): a memory barrier makes the Room4 test reload
   * gMainRoutineWork's address; without it, 70 of 235 instructions differ in
   * tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(3, 114, 1, 1, 3, 113);
      if (object->x >> 20 == 2)
        Event_SetFlag(0x9c8);
    }
    if (id == 18) {
      AnkohlRuins_Call2dfc(40, 94, 1, 1, 40, 93);
      if (object->x >> 20 == 39)
        Event_SetFlag(0x9c9);
    }
    if (id == 19) {
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (object->x >> 20 == 38) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(0x9ca);
      }
    }
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (object->x >> 20 == 17) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(0x9cb);
    }
  }
}
