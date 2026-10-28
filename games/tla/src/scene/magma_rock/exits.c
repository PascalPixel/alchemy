#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "scene_ids.h"

extern const u8 gData_0200403c[];
extern const u8 gData_020041a4[];
extern const u8 gData_0200427c[];
extern const u8 gData_020043e4[];
extern const u8 gData_020044a4[];
extern const u8 gData_020045c4[];

const struct Trigger *Func_020008d4(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov691Room2) {
    if (gAdventure.entrance <= 3)
      return (const struct Trigger *)gData_020041a4;
    if (gAdventure.entrance <= 8)
      return (const struct Trigger *)gData_0200427c;
    if (gAdventure.entrance <= 12)
      return (const struct Trigger *)gData_020043e4;
    return (const struct Trigger *)gData_020044a4;
  }
  if (gAdventure.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gData_020045c4;
  return (const struct Trigger *)gData_0200403c;
}
