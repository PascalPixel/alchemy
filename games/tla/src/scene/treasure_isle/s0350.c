#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

extern const u8 gData_02001330[];
extern const u8 gData_02001390[];
extern const u8 gData_02001480[];
extern const u8 gData_02001504[];
extern const u8 gData_0200169c[];

const u8 *Func_02000350(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov6afRoom2)
    return gData_02001390;
  if (scene == (s32)&SceneId_Ov6afRoom3)
    return gData_02001480;
  if (scene == (s32)&SceneId_Ov6afRoom4)
    return gData_02001504;
  if (scene == (s32)&SceneId_Ov6afRoom5)
    return gData_0200169c;
  return gData_02001330;
}
