// Draft, differ 35 of 37: the native reads the scene again with ldrsh before
// each of the three tests and builds each flag base in r2; this form reads the
// second and third with ldrh before the branch and extends them. A forwarding
// inline for the scene test is worse (differ 75).
#include "types.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

void GameFlag_SetBit(s32 flag);

void Func_02001264(s32 a, s32 id) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov686Room1)
    GameFlag_SetBit(id + 0x97f);
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov686Room2)
    GameFlag_SetBit(id + 0x995);
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov686Room4)
    GameFlag_SetBit(id + 0x996);
}
