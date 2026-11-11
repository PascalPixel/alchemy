#include "scene.h"
#include "scene_ids.h"

extern u32 gData_0200375c[];
extern u32 gData_020037a4[];
extern u32 gData_0200381c[];
extern u32 gData_02003864[];
extern u32 gData_020038ac[];
extern u32 gData_0200390c[];
extern u32 gData_02003984[];
extern u32 gData_020039cc[];
extern u32 gData_02003aec[];
extern u32 gData_02003b94[];
extern u32 gData_02003c0c[];
extern u32 gData_02003c84[];

u32 *Func_02000070(void) {
  s32 scene;

  if (GameFlag_Test(0x97e)) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Ov685Room1)
      return gData_02003aec;
    if (scene == (s32)&SceneId_Ov685Room2)
      return gData_02003b94;
    if (scene == (s32)&SceneId_Ov685Room3)
      return gData_02003c0c;
    if (scene == (s32)&SceneId_Ov685Room4)
      return gData_02003c84;
  } else {
    if (GameFlag_Test(0x97f)) {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_020038ac;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_0200390c;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_02003984;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_020039cc;
    } else {
      scene = gMainRoutineWork.scene;
      if (scene == (s32)&SceneId_Ov685Room1)
        return gData_0200375c;
      if (scene == (s32)&SceneId_Ov685Room2)
        return gData_020037a4;
      if (scene == (s32)&SceneId_Ov685Room3)
        return gData_0200381c;
      if (scene == (s32)&SceneId_Ov685Room4)
        return gData_02003864;
    }
  }
  return gData_0200375c;
}
