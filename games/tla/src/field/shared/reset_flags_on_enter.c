#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void Func_080c9de0(s32 id, s32 variant);

#define SCENE_RECORD(id) (&gScenes[id])

/// The Broken Seal's Scene_ResetFlagsOnEnter with this game's flags, and
/// Func_080c9de0 in place of the group's visited flag.
void Func_080c9c38(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gMainRoutineWork.scene)->group;
  s32 flag;

  if (keep == 0) {
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBit(flag);
    if (group != gMainRoutineWork.sceneGroup) {
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBit(flag);
      GameFlag_SetBit(0x12f);
      gMainRoutineWork.encounterSteps = 0;
      gMainRoutineWork.hazardSteps = 0;
      GameFlag_ClearBit(0x110);
      GameFlag_ClearBit(0x111);
      GameFlag_ClearBit(0x112);
      GameFlag_ClearBit(0x113);
      gMainRoutineWork.retreatScene = gMainRoutineWork.scene;
      gMainRoutineWork.retreatEntrance = gMainRoutineWork.entrance;
    }
    for (flag = 0x80; flag <= 0xdf; flag++)
      GameFlag_ClearBit(flag);
    GameFlag_ClearBit(0x16c);
    GameFlag_ClearBit(0x144);
    GameFlag_ClearBit(0x161);
    GameFlag_ClearBit(0x123);
    GameFlag_ClearBit(0x11c);
    GameFlag_ClearBit(0x146);
    GameFlag_ClearBit(0x17c);
    GameFlag_ClearBit(0x17d);
    GameFlag_ClearBit(0x17e);
    if (gMainRoutineWork.ownerOverride != 0)
      GameFlag_SetBit(0x176);
    else
      GameFlag_ClearBit(0x176);
    gMainRoutineWork.returnCue = -1;
  }
  gMainRoutineWork.sceneGroup = group;
  /* FAKEMATCH(barrier): the scene group is stored before the scene's offset
   * is built; as plain C, 4 of 174 instructions differ in tla-en. */
  asm("");
  Func_080c9de0(gMainRoutineWork.scene, gMainRoutineWork.entrance);
  gMainRoutineWork.sceneVariant = SCENE_RECORD(gMainRoutineWork.scene)->variant;
  if (gMainRoutineWork.sceneVariant == 2)
    GameFlag_SetBit(0x123);
  GameFlag_RefreshLureCapFar();
}
