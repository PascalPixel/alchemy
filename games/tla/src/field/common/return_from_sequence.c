#include "types.h"
#include "battle_unit.h"
#include "adventure.h"

void *Unit_Get(s32 owner);
void GameFlag_SetBit(s32 flag);
void Func_080ca3f4(s32 scene, s32 entrance);

/// The Broken Seal's Scene_ReturnFromSequence, without the childhood healing,
/// ending a defeat with Func_080ca3f4.
void Scene_ReturnFromSequence(s32 result) {
  struct BattleUnit *unit;
  s16 scene;
  s16 entrance;

  gAdventure.sequenceResult = result;
  if (result == -1) {
    unit = Unit_Get(gAdventure.currentOwner);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    scene = gAdventure.pair1f2[0];
    entrance = gAdventure.pair1f2[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gAdventure.scene = scene;
      else
        gAdventure.scene = gAdventure.primary;
      if (entrance != -1)
        gAdventure.entrance = entrance;
      else
        gAdventure.entrance = gAdventure.secondary;
    } else {
      gAdventure.scene = gAdventure.savedScene;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the
       * default's load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gAdventure.entrance = gAdventure.savedEntrance;
    }
    Func_080ca3f4(gAdventure.scene, gAdventure.entrance);
  } else {
    scene = gAdventure.pair1ee[0];
    entrance = gAdventure.pair1ee[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gAdventure.scene = scene;
      else
        gAdventure.scene = gAdventure.primary;
      if (entrance != -1)
        gAdventure.entrance = entrance;
      else
        gAdventure.entrance = gAdventure.secondary;
    } else {
      gAdventure.scene = gAdventure.primary;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the win's
       * default load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gAdventure.entrance = gAdventure.secondary;
      GameFlag_SetBit(0x109);
    }
  }
}
