#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_02005dfc[];
extern const u8 gData_02005e2c[];
extern const u8 gData_02005e4c[];
extern const u8 gData_02005e8c[];
extern const u8 gData_02005ecc[];
extern const u8 gData_02005eec[];

const u8 *Func_020002dc(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov671Room7)
    return gData_02005dfc;
  if (scene == (s32)&SceneId_Ov671Room1)
    return gData_02005e2c;
  if (scene == (s32)&SceneId_Ov671Room2)
    return gData_02005e4c;
  if (scene == (s32)&SceneId_Ov671Room3)
    return gData_02005e8c;
  if (scene == (s32)&SceneId_Ov671Room4)
    return gData_02005ecc;
  if (scene == (s32)&SceneId_Ov671Room5)
    return gData_02005eec;
  return NULL;
}
