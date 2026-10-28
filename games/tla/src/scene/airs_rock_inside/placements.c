#include "airs_rock_inside.h"

extern const u8 gData_02006238[];
extern const u8 gData_02006268[];

const u8 *Func_020025cc(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov664Room1)
    return gData_02006268;
  return gData_02006238;
}
