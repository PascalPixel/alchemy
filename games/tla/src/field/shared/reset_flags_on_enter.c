#include "types.h"
#include "adventure.h"
#include "scene_tables.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void Func_080c9de0(s32 id, s32 variant);

#define SCENE_RECORD(id) (&gScenes[id])

/// The Broken Seal's Scene_ResetFlagsOnEnter with this game's flags, and
/// Func_080c9de0 in place of the group's visited flag.
void Func_080c9c38(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gAdventure.scene)->group;
  s32 flag;

  if (keep == 0) {
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBit(flag);
    if (group != gAdventure.sceneGroup) {
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBit(flag);
      GameFlag_SetBit(0x12f);
      gAdventure.encounterSteps = 0;
      gAdventure.hazardSteps = 0;
      GameFlag_ClearBit(0x110);
      GameFlag_ClearBit(0x111);
      GameFlag_ClearBit(0x112);
      GameFlag_ClearBit(0x113);
      gAdventure.retreatScene = gAdventure.scene;
      gAdventure.retreatEntrance = gAdventure.entrance;
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
    if (gAdventure.ownerOverride != 0)
      GameFlag_SetBit(0x176);
    else
      GameFlag_ClearBit(0x176);
    gAdventure.returnCue = -1;
  }
  gAdventure.sceneGroup = group;
  /* FAKEMATCH(barrier): the scene group is stored before the scene's offset
   * is built; as plain C, 4 of 174 instructions differ in tla-en. */
  asm("");
  Func_080c9de0(gAdventure.scene, gAdventure.entrance);
  gAdventure.sceneVariant = SCENE_RECORD(gAdventure.scene)->variant;
  if (gAdventure.sceneVariant == 2)
    GameFlag_SetBit(0x123);
  GameFlag_RefreshLureCapFar();
}
