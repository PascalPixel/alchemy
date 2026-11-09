#include "yampi_desert_cave.h"
#include "actor.h"
#include "event_inline.h"

/// Func_02001120's counterpart: the leader's terrain becomes 2, and in rooms 1
/// and 3 the same calls run with their first and last pairs swapped, each
/// followed by more.
void Func_02001040(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    MapGrid_CopyCellAttributes(55, 71, 4, 1, 43, 70);
    MapGrid_CopyCellAttributes(43, 71, 4, 1, 55, 71);
  }
  /* FAKEMATCH: the native reads the scene again here, as Func_02001120 does;
   * without the barrier the read is moved up both paths */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    Map_CopyCellAttributeRect(110, 45, 5, 2, 90, 50);
    Map_CopyCellAttributeRect(113, 47, 2, 1, 90, 52);
    Map_CopyCellAttributeRect(107, 48, 3, 2, 90, 53);
    MapGrid_CopyCellAttributes(97, 58, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(97, 58, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(97, 58, 3, 2, 107, 48);
  }
}

void Func_02001120(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1)
    MapGrid_CopyCellAttributes(43, 70, 4, 1, 55, 71);
  /* FAKEMATCH: the native reads the scene again here; without the barrier the
   * read is moved up both paths (differ 18) */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    MapGrid_CopyCellAttributes(90, 50, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(90, 52, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(90, 53, 3, 2, 107, 48);
  }
}
