// Draft of Scene_GetPlacements, differ 19 of 87: GCSE (PRE) keeps gAdventure's
// address in r5 across the flag tests and reuses it for the second scene read;
// the native loads it again from the pool after the join. Tried: one local read
// twice, nested ifs, one-pass blocks around either read, the flag inline layer
// (all differ 19), three minutes of permute (16, by wrapping the reread in a
// dead condition).
#include "types.h"
#include "scene_tables.h"
#include "adventure.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);

extern struct ActorPlacement gData_02004b14[];
extern struct ActorPlacement gData_02004b2c[];
extern struct ActorPlacement gData_02004c4c[];
extern struct ActorPlacement gData_02004d9c[];
extern struct ActorPlacement gData_02004f04[];
extern struct ActorPlacement gData_0200506c[];
extern struct ActorPlacement gData_0200521c[];
extern struct ActorPlacement gData_02005444[];
extern struct ActorPlacement gData_020054a4[];
extern struct ActorPlacement gData_0200551c[];

const struct ActorPlacement *Func_0200048c(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov6a8Room1 && GameFlag_Test(0x9c5) &&
      !GameFlag_Test(0x9cc))
    return gData_02004b2c;
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov6a8Room2)
    return gData_02004c4c;
  if (scene == (s32)&SceneId_Ov6a8Room3) {
    if (GameFlag_Test(0xa3d))
      return gData_02004f04;
    return gData_02004d9c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room4) {
    if (GameFlag_Test(0xa3d)) {
      gData_0200521c[1].talkFacing = 3;
      gData_0200521c[4].talkFacing = 3;
      return gData_0200521c;
    }
    return gData_0200506c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room5) {
    if (GameFlag_Test(0xa3d))
      return gData_020054a4;
    return gData_02005444;
  }
  if (scene == (s32)&SceneId_Ov6a8Room6)
    return gData_0200551c;
  return gData_02004b14;
}
