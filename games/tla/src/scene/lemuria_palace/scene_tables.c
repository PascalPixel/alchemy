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
#include "call.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

struct Actor *Actor_GetById();
void Actor_SetSpeedParameters();
void FieldEvent_WaitFrames();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02001fb4();
void Func_02001fcc();
void Func_02001fd4();
void Func_020020b4();
void Func_020020cc();
void Func_02002124(s32);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gScenePlacements;
}

void Func_02000054(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02001fcc();
  Func_020020cc(0);
  Func_02002124(158);
  Func_02001fb4(step->script, step->x, step->z);
  Actor_GetById(gAdventure.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gAdventure.currentOwner, 2);
  Call3((void (*)())Actor_CenterOnColumn, gAdventure.currentOwner, 2,
        -8);
  FieldEvent_WaitFrames(10);
  Func_020020b4(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fd4();
}
