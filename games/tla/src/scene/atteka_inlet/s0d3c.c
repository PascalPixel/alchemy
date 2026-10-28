#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void Func_02000ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

s32 Func_02000d3c(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov693Room1 && GameFlag_Test(0x9d0)) {
    Func_02000ff0(54, 47, 4, 15, 10, 4);
    Func_02000ff0(54, 38, 73, 7, 10, 4);
    Func_02000ff0(54, 43, 21, 55, 10, 3);
    Func_02000ff0(65, 43, 21, 20, 10, 3);
    Func_02000ff0(4, 47, 4, 50, 10, 3);
  }
  return 0;
}
