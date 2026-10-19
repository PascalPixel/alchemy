// Near miss, differ 52 of 202: ☀️'s Scene_ReturnFromBattle, without the
// childhood healing and ending a defeat with Func_080ca3f4. The native
// shares one store of the scene between "scene != -1" and the default
// (ldrh r2 of the default, then movs r4, #240 at the shared label); agscc
// schedules movs r4 before the ldrh, so the cross jump starts later and each
// branch keeps its own copy. Written with the default first, a ternary or the
// default as an assignment to scene, it differs 63 to 109; an empty asm after
// the default's load differs 268.
#include "types.h"
#include "battle_unit.h"
#include "party_state.h"

void *UnitGet(s32 owner);
void GameFlag_SetBit(s32 flag);
void Func_080ca3f4(s32 scene, s32 entrance);

void Scene_ReturnFromBattle(s32 result) {
  struct BattleUnit *unit;
  s16 scene;
  s16 entrance;

  gPartyState.unk256 = result;
  if (result == -1) {
    unit = UnitGet(gPartyState.currentOwner);
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
      gPartyState.entrance = gPartyState.secondary;
      GameFlag_SetBit(0x109);
    }
  }
}
