#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Func_02000d3c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1 && GameFlag_Test(0x9d0)) {
    Map_CopyCellPictureIndicesRect(54, 47, 4, 15, 10, 4);
    Map_CopyCellPictureIndicesRect(54, 38, 73, 7, 10, 4);
    Map_CopyCellPictureIndicesRect(54, 43, 21, 55, 10, 3);
    Map_CopyCellPictureIndicesRect(65, 43, 21, 20, 10, 3);
    Map_CopyCellPictureIndicesRect(4, 47, 4, 50, 10, 3);
  }
  return 0;
}
