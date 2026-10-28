#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);

extern u8 gData_02001194[];
extern u8 gData_020011ac[];
extern u8 gData_020012cc[];
extern u8 gData_0200147c[];
extern u8 gData_0200159c[];
extern u8 gData_0200165c[];

void *Func_02000064(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1) {
    if (GameFlag_Test(0x9b2))
      return gData_0200147c;
    if (GameFlag_Test(0x9d0))
      return gData_020012cc;
    return gData_020011ac;
  }
  if (scene == (s32)&SceneId_Ov693Room2) {
    if (GameFlag_Test(0x9d0))
      return gData_0200159c;
    return gData_0200165c;
  }
  return gData_02001194;
}
