#include "ankohl_ruins.h"
#include "edition.h"

/// Copies the cells under the object with the given id in the rooms that have
/// them (rooms 1, 3, 4 and 5); in room 5 the object with id 9 also copies the
/// rows that flag 0x205 opens (always, in Japanese).
void Func_020006d0(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 4, 69);
      AnkohlRuins_Call2dfc(3, 68, 1, 1, 3, 70);
    }
  }
  /* FAKEMATCH(barrier): memory barriers before each later scene test reload
   * gMainRoutineWork's address, as the native does; as plain statements, 19 of
   * 168 instructions differ in tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 19) {
      AnkohlRuins_Call2dfc(35, 72, 1, 1, 38, 72);
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 37, 73);
    }
    if (id == 8)
      AnkohlRuins_Call2dfc(2, 113, 1, 1, 3, 113);
    if (id == 18)
      AnkohlRuins_Call2dfc(39, 93, 1, 1, 40, 93);
  }
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(32, 69, 1, 1, 32, 70);
      AnkohlRuins_Call2dfc(33, 69, 1, 1, 33, 71);
    }
  }
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(17, 85, 1, 1, 17, 83);
      AnkohlRuins_Call2dfc(19, 84, 1, 1, 18, 82);
      AnkohlRuins_Call2dfc(19, 84, 1, 1, 19, 83);
    }
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (id == 9)
#else
    if (id == 9 && GameFlag_Test(0x205))
#endif
      AnkohlRuins_Call2df4(35, 98, 3, 5, 25, 98);
  }
}
