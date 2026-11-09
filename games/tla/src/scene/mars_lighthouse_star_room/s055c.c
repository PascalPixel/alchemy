#include "mars_lighthouse_star_room.h"
#include "scene_ids.h"

extern char SceneId_Ov6ACRoom3;
void Func_02004294(s32 scene, s32 a1);

s32 Func_0200055c(void) {
  if (Event_GetFlag(0xa30))
    Func_02004294(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  if (gMainRoutineWork.entrance == 20)
    Event_SetFlag(0xa4b);
  return 0;
}
