#include "types.h"

#include "party_state.h"
#include "scene_ids.h"

void Func_02001950(s32 show, s32 first, u32 count);

void Func_020018ec(s32 show) {
  if (gPartyState.scene == (s32)&SceneId_Ov671Room1)
    Func_02001950(show, 8, 1);
  else if (gPartyState.scene == (s32)&SceneId_Ov671Room2)
    Func_02001950(show, 29, 2);
}
