#include "types.h"
#include "frame.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

void Func_02001860(s32 x, s32 y, s32 z);

/* FAKEMATCH: a volatile read keeps each test's own load of the frame counter;
 * plain, a test that follows a skipped call reuses the value the test before it
 * loaded (differ 32). */
#define FRAME_COUNT (*(volatile u32 *)&gFrameCount)

void Func_020018e0(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room2) {
    if ((FRAME_COUNT & 63) == 0)
      Func_02001860(536 << 16, 40 << 16, 232 << 16);
    if (((FRAME_COUNT + 60) & 63) == 0)
      Func_02001860(168 << 16, 40 << 16, 296 << 16);
    if (((FRAME_COUNT + 20) & 63) == 0) {
      Func_02001860(280 << 16, 42 << 16, 184 << 16);
      Func_02001860(152 << 16, 48 << 16, 424 << 16);
    }
    if (((FRAME_COUNT + 40) & 63) == 0)
      Func_02001860(336 << 16, 42 << 16, 362 << 16);
  } else if (scene == (s32)&SceneId_Ov6a8Room3) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(568 << 16, 104 << 16, 296 << 16);
  }
}
