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
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_ids.h"
#include "edition.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02000228[];
extern const u8 gData_020003a8[];
extern const u8 gData_020003d8[];
extern const u8 gData_020005ac[];
extern const u8 gData_02000780[];

void Func_0200014c(void);
void Func_02000178(s32 a, s32 b);
void Func_02000180(s32 a, s32 b);

const struct ScenePlacement *Func_0200004c(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov683 &&
      (u16)(gAdventure.entrance - 8) <= 1)
    return (const struct ScenePlacement *)gData_020003a8;
  return (const struct ScenePlacement *)gData_02000228;
}

void Func_0200008c(s32 value) { Func_02000178(25, value); }

void Func_02000098(s32 value) { Func_02000180(10, value); }

const struct SceneEvent *Func_020000a4(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov683 &&
      (u16)(gAdventure.entrance - 8) <= 1)
    return (const struct SceneEvent *)gData_02000780;
  if (GameFlag_Test(0x97f))
    return (const struct SceneEvent *)gData_020005ac;
  return (const struct SceneEvent *)gData_020003d8;
}

s32 Func_020000fc(void) {
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  s32 value = 0x201;
#else
  s32 value = 0x209;
#endif

  /* FAKEMATCH(barrier): an empty asm after the address and the value, so the
   * adventure's address loads just before the store; as one assignment, 2
   * of 32 instructions differ in tla-en. */
  __asm__("");
  *effect = value;
  if (gAdventure.scene == (s32)&SceneId_Ov683 &&
      (u16)(gAdventure.entrance - 8) <= 1)
    Func_0200014c();
  return 0;
}
