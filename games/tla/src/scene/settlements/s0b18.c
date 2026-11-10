#include "local.h"
#include "version.h"

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room9;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Place(s32 id, s32 x, s32 z) { Actor_SetHorizontalPositionWithTerrain(id, x, z); }

s32 Func_02000b18(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    if (Flag_Test(0x205)) {
      CopyCells2(9, 11, 1, 1, 11, 12);
      Map_CopyCellPictureIndicesRect(12, 32, 11, 15, 1, 1);
      Map_CopyCellPictureIndicesRect(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (GameFlag_Test(0x205))
      Map_CopyCellAttributeRect(9, 11, 1, 1, 11, 11);
#else
    if (GameFlag_Test(0x204)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Place(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room1) {
    if (GameFlag_Test(0xa60)) {
      Map_CopyCellPictureIndicesRect(5, 46, 5, 47, 1, 1);
      Map_CopyCellPictureIndicesRect(4, 25, 5, 17, 1, 1);
      CopyCells2(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}
