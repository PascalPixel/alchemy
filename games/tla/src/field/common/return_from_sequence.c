#include "types.h"
#include "battle_unit.h"
#include "party_state.h"

void *Unit_Get(s32 owner);
void GameFlag_SetBit(s32 flag);
void Func_080ca3f4(s32 scene, s32 entrance);

/// The Broken Seal's Scene_ReturnFromSequence, without the childhood healing,
/// ending a defeat with Func_080ca3f4.
void Scene_ReturnFromSequence(s32 result) {
  struct BattleUnit *unit;
  s16 scene;
  s16 entrance;

  gPartyState.sequenceResult = result;
  if (result == -1) {
    unit = Unit_Get(gPartyState.currentOwner);
    if (unit->hp == 0) {
      unit->hp = 1;
      Unit_UpdateGauges(unit);
    }
    scene = gPartyState.pair1f2[0];
    entrance = gPartyState.pair1f2[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gPartyState.scene = scene;
      else
        gPartyState.scene = gPartyState.primary;
      if (entrance != -1)
        gPartyState.entrance = entrance;
      else
        gPartyState.entrance = gPartyState.secondary;
    } else {
      gPartyState.scene = gPartyState.savedScene;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the
       * default's load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gPartyState.entrance = gPartyState.savedEntrance;
    }
    Func_080ca3f4(gPartyState.scene, gPartyState.entrance);
  } else {
    scene = gPartyState.pair1ee[0];
    entrance = gPartyState.pair1ee[1];
    if (scene != -1 || entrance != -1) {
      if (scene != -1)
        gPartyState.scene = scene;
      else
        gPartyState.scene = gPartyState.primary;
      if (entrance != -1)
        gPartyState.entrance = entrance;
      else
        gPartyState.entrance = gPartyState.secondary;
    } else {
      gPartyState.scene = gPartyState.primary;
      /* FAKEMATCH(barrier): the scene's store ends its block, so the win's
       * default load comes before the shared entrance offset; as plain C,
       * 2 of 202 instructions differ in tla-en. */
      asm("");
      gPartyState.entrance = gPartyState.secondary;
      GameFlag_SetBit(0x109);
    }
  }
}
