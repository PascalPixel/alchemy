#include "types.h"

#include "sequence_parent_work.h"
#include "scene_ids.h"

void Func_02001950(s32 show, s32 first, u32 count);

void Func_020018ec(s32 show) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov671Room1)
    Func_02001950(show, 8, 1);
  else if (gSequenceParentWork.scene == (s32)&SceneId_Ov671Room2)
    Func_02001950(show, 29, 2);
}
