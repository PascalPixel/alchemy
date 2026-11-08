#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
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
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a8Room1 && GameFlag_Test(SPLIT_CONSTANT(0x900, 197)) &&
      !GameFlag_Test(0x9cc))
    return gData_02004b2c;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov6a8Room2)
    return gData_02004c4c;
  if (scene == (s32)&SceneId_Ov6a8Room3) {
    if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 61)))
      return gData_02004f04;
    return gData_02004d9c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room4) {
    if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 61))) {
      gData_0200521c[1].talkFacing = 3;
      gData_0200521c[4].talkFacing = 3;
      return gData_0200521c;
    }
    return gData_0200506c;
  }
  if (scene == (s32)&SceneId_Ov6a8Room5) {
    if (GameFlag_Test(SPLIT_CONSTANT(0xa00, 61)))
      return gData_020054a4;
    return gData_02005444;
  }
  if (scene == (s32)&SceneId_Ov6a8Room6)
    return gData_0200551c;
  return gData_02004b14;
}
