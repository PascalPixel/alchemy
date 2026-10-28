#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

extern const u8 gData_02002200[];
extern const u8 gData_02002218[];
extern const u8 gData_02002320[];
extern const u8 gData_02002458[];

const u8 *Func_02000360(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov679Room1)
    return gData_02002218;
  if (scene == (s32)&SceneId_Ov679Room3)
    return gData_02002320;
  if (scene == (s32)&SceneId_Ov679Room2)
    return gData_02002458;
  return gData_02002200;
}
