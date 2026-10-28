#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

extern const u8 gData_020016e4[];
extern const u8 gData_02001780[];
extern const u8 gData_0200184c[];
extern const u8 gData_020018f4[];
extern const u8 gData_02001a5c[];

const u8 *Func_02000b3c(void) {
  s32 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001780;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_0200184c;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_020018f4;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_02001a5c;
  return gData_020016e4;
}
