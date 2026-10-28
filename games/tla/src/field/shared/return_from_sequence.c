#include "types.h"
#include "battle_unit.h"
#include "sequence_parent_work.h"

void *Unit_Get(s32 owner);
void GameFlag_SetBit(s32 flag);
void Func_080ca3f4(s32 scene, s32 entrance);

/// The Broken Seal's Scene_ReturnFromSequence, without the childhood healing,
/// ending a defeat with Func_080ca3f4.
void Scene_ReturnFromSequence(s32 result) {
  struct BattleUnit *unit;
  s16 scene;
  s16 entrance;

  gSequenceParentWork.sequenceResult = result;
  if (result == -1) {
    unit = Unit_Get(gSequenceParentWork.currentOwner);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    scene = gSequenceParentWork.pair1f2[0];
    entrance = gSequenceParentWork.pair1f2[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gSequenceParentWork.scene = scene;
      else
        gSequenceParentWork.scene = gSequenceParentWork.primary;
      if (entrance != -1)
        gSequenceParentWork.entrance = entrance;
      else
        gSequenceParentWork.entrance = gSequenceParentWork.secondary;
    } else {
      gSequenceParentWork.scene = gSequenceParentWork.savedScene;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the
       * default's load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gSequenceParentWork.entrance = gSequenceParentWork.savedEntrance;
    }
    Func_080ca3f4(gSequenceParentWork.scene, gSequenceParentWork.entrance);
  } else {
    scene = gSequenceParentWork.pair1ee[0];
    entrance = gSequenceParentWork.pair1ee[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gSequenceParentWork.scene = scene;
      else
        gSequenceParentWork.scene = gSequenceParentWork.primary;
      if (entrance != -1)
        gSequenceParentWork.entrance = entrance;
      else
        gSequenceParentWork.entrance = gSequenceParentWork.secondary;
    } else {
      gSequenceParentWork.scene = gSequenceParentWork.primary;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the win's
       * default load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gSequenceParentWork.entrance = gSequenceParentWork.secondary;
      GameFlag_SetBit(0x109);
    }
  }
}
