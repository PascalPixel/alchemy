#include "mars_lighthouse_star_room.h"
#include "scene_ids.h"

extern char SceneId_Ov6ACRoom3;
void Func_02004294(s32 scene, s32 a1);

s32 Func_0200055c(void) {
  if (GameFlag_Test(0xa30))
    Func_02004294(gSequenceParentWork.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  if (gSequenceParentWork.entrance == 20)
    GameFlag_SetBit(0xa4b);
  return 0;
}
