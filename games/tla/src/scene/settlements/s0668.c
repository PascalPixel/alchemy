#include "types.h"
#include "adventure.h"

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room2;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room5;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room8;
extern char SceneId_Ov690Room9;
extern char SceneId_Ov690Room10;

extern u8 gData_0200289c[];
extern u8 gData_020028f0[];
extern u8 gData_02002974[];
extern u8 gData_020029a4[];
extern u8 gData_02002a70[];
extern u8 gData_02002ad0[];
extern u8 gData_02002b3c[];
extern u8 gData_02002b90[];
extern u8 gData_02002c2c[];

u8 *Func_02000668(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gData_020028f0;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gData_02002974;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gData_020029a4;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_02002a70;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gData_02002ad0;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gData_02002b3c;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gData_02002b90;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gData_02002c2c;
  return gData_0200289c;
}
