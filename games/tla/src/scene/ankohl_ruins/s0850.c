#include "ankohl_ruins.h"

/// Copies the cells the object with the given id has pushed open in rooms 1,
/// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
/// track.
void Func_02000850(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (id == 8 && gSequenceParentWork.scene == (s32)&SceneId_Ov686Room1) {
    if (object->x >> 20 == 4) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 2, 69);
      GameFlag_SetBit(0x9c6);
    }
    AnkohlRuins_Call2dfc(3, 68, 1, 1, 4, 69);
    AnkohlRuins_Call2dfc(3, 69, 1, 1, 3, 70);
  }
  /* FAKEMATCH(barrier): memory barriers before the later scene tests reload
   * gSequenceParentWork's address, as the native does; as plain statements, 70 of
   * 235 instructions differ in tla-en. */
  if (id == 8 && ({
        __asm__("" : : : "memory");
        gSequenceParentWork.scene;
      }) == (s32)&SceneId_Ov686Room3) {
    Func_02002dfc(32, 71, 1, 1, 33, 71);
    AnkohlRuins_Call2dfc(31, 70, 1, 1, 32, 70);
    if (object->x >> 20 == 32) {
      Func_02002dfc(32, 69, 1, 1, 33, 69);
      GameFlag_SetBit(0x9c7);
    }
  }
  __asm__("" : : : "memory");
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(3, 114, 1, 1, 3, 113);
      if (object->x >> 20 == 2)
        GameFlag_SetBit(0x9c8);
    }
    if (id == 18) {
      AnkohlRuins_Call2dfc(40, 94, 1, 1, 40, 93);
      if (object->x >> 20 == 39)
        GameFlag_SetBit(0x9c9);
    }
    if (id == 19) {
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 38, 72);
      Func_02002dfc(37, 74, 1, 1, 37, 73);
      if (object->x >> 20 == 38) {
        Func_02002dfc(37, 70, 1, 1, 37, 71);
        GameFlag_SetBit(0x9ca);
      }
    }
  }
  __asm__("" : : : "memory");
  if (id == 8 && gSequenceParentWork.scene == (s32)&SceneId_Ov686Room5) {
    Func_02002dfc(16, 83, 1, 1, 17, 83);
    Func_02002dfc(19, 82, 1, 1, 18, 82);
    Func_02002dfc(19, 82, 1, 1, 19, 83);
    if (object->x >> 20 == 17) {
      Func_02002dfc(16, 84, 1, 1, 18, 84);
      GameFlag_SetBit(0x9cb);
    }
  }
}
