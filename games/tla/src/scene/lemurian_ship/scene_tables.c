#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "scene_ids.h"

extern u8 gData_02003054[];
extern u8 gData_02003084[];
extern u8 gData_0200312c[];
extern u8 gData_02003234[];
extern u8 gData_02003254[];

u8 *Func_02000038(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov656Room1)
    return gData_02003084;
  if (scene == (s32)&SceneId_Ov656Room2)
    return gData_0200312c;
  return gData_02003054;
}

u8 *Func_02000078(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov656Room1)
    return gData_02003234;
  if (scene == (s32)&SceneId_Ov656Room2)
    return gData_02003254;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }
