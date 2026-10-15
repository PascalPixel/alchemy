#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_02001294[];
extern const u8 gData_020015f0[];
extern const u8 gData_020018c8[];

const u8 *Func_020000ac(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov66fRoom1)
    return gData_020015f0;
  if (scene == (s32)&SceneId_Ov66fRoom2)
    return gData_020018c8;
  return gData_02001294;
}
