#include "types.h"
#include "adventure.h"

extern char SceneId_Ov692Room1;
extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern char SceneId_Ov692Room5;
extern char SceneId_Ov692Room6;
extern char SceneId_Ov692Room7;
extern char SceneId_Ov692Room8;

extern u8 gData_02005f60[];
extern u8 gData_02005fe4[];
extern u8 gData_020060bc[];
extern u8 gData_020061c4[];
extern u8 gData_020063a4[];
extern u8 gData_02006458[];
extern u8 gData_0200659c[];
extern u8 gData_02006714[];

u8 *Func_02003418(void) {
  s16 scene = gAdventure.scene;

  if (scene != (s32)&SceneId_Ov692Room1) {
    if (scene == (s32)&SceneId_Ov692Room2)
      return gData_02005fe4;
    if (scene == (s32)&SceneId_Ov692Room3)
      return gData_020060bc;
    if (scene == (s32)&SceneId_Ov692Room4)
      return gData_020061c4;
    if (scene == (s32)&SceneId_Ov692Room5)
      return gData_020063a4;
    if (scene == (s32)&SceneId_Ov692Room6)
      return gData_02006458;
    if (scene == (s32)&SceneId_Ov692Room7)
      return gData_0200659c;
    if (scene == (s32)&SceneId_Ov692Room8)
      return gData_02006714;
  }
  return gData_02005f60;
}
