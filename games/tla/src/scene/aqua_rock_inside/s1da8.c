#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

extern const u8 gData_02004c70[];
extern const u8 gData_02004c7c[];
extern const u8 gData_02004d88[];
extern const u8 gData_02004e48[];
extern const u8 gData_02004ed8[];
extern const u8 gData_02004fb0[];
extern const u8 gData_0200507c[];
extern const u8 gData_020051b4[];
extern const u8 gData_020051fc[];
extern const u8 gData_02005268[];
extern const u8 gData_020052f8[];

const u8 *Func_02001da8(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gData_02004c7c;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gData_02004d88;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gData_02004e48;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gData_02004ed8;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gData_02004fb0;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gData_0200507c;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gData_020051b4;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gData_020051fc;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gData_02005268;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gData_020052f8;
  return gData_02004c70;
}
