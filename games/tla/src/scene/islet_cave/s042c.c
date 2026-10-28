#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

extern const u8 gData_02001548[];
extern const u8 gData_02001668[];
extern const u8 gData_02001740[];
extern const u8 gData_02001788[];
extern const u8 gData_02001914[];
extern const u8 gData_020019b0[];

const u8 *Func_0200042c(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov675Room1)
    return gData_02001668;
  if (scene == (s32)&SceneId_Ov675Room2)
    return gData_02001740;
  return gData_02001548;
}

const u8 *Func_0200046c(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov675Room2)
    return gData_02001914;
  if (scene == (s32)&SceneId_Ov675Room3)
    return gData_020019b0;
  return gData_02001788;
}
