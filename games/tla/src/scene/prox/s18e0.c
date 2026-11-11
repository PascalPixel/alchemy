#include "types.h"
#include "frame.h"
#include "main_routine_work.h"
#include "scene_ids.h"

void Func_02001860(s32 x, s32 y, s32 z);


void Func_020018e0(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room2) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(536 << 16, 40 << 16, 232 << 16);
    if (((gFrameCount + 60) & 63) == 0)
      Func_02001860(168 << 16, 40 << 16, 296 << 16);
    if (((gFrameCount + 20) & 63) == 0) {
      Func_02001860(280 << 16, 42 << 16, 184 << 16);
      Func_02001860(152 << 16, 48 << 16, 424 << 16);
    }
    if (((gFrameCount + 40) & 63) == 0)
      Func_02001860(336 << 16, 42 << 16, 362 << 16);
  } else if (scene == (s32)&SceneId_Ov6a8Room3) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(568 << 16, 104 << 16, 296 << 16);
  }
}
