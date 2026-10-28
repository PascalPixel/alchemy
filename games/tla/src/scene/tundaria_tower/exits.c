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

extern const u8 gData_02003618[];
extern const u8 gData_02003648[];
extern const u8 gData_02003660[];
extern const u8 gData_02003690[];
extern const u8 gData_02003708[];
extern const u8 gData_020037c8[];

const struct Trigger *Func_020004c8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom2)
    return (const struct Trigger *)gData_02003618;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom3)
    return (const struct Trigger *)gData_02003648;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom4)
    return (const struct Trigger *)gData_02003660;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom5)
    return (const struct Trigger *)gData_02003690;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom6)
    return (const struct Trigger *)gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov68fRoom7)
    return (const struct Trigger *)gData_020037c8;
  return (const struct Trigger *)gData_02003618;
}
