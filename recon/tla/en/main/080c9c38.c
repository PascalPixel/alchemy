// Near miss, differ 2 of 174 (14 with no device): ☀️'s
// Scene_ResetFlagsOnEnter with ⚓️'s flags and Func_080c9de0 in place of the
// group's visited flag. Only order differs. Four one-pass blocks and one
// empty asm (to be tagged FAKEMATCH on adoption) give the native's order:
// gPartyState loaded before the ownerOverride and returnCue offsets,
// sceneGroup stored before the scene's offset, the record's address before
// the variant's offset, and the scene's ldrh before retreatScene's offset
// (the asm holds it and clobbers r1 and r3; without r3 it lands in r3,
// differ 8). Left: the native issues the variant's load (movs r2, #3;
// ldrsb) before shifting its offset (lsls r1); an asm on the record
// clobbering r1 and r2 still shifts first (differ 2), clobbering r2 alone
// moves movs r1 above the record's add (differ 4), and writing through a
// pointer to sceneVariant differs 27.
#include "types.h"
#include "party_state.h"
#include "field_scene.h"

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void Func_080c9de0(s32 id, s32 variant);

#define SCENE_RECORD(id) (&gField_SceneTable[id])

void Scene_ResetFlagsOnEnter(s32 unused, s32 keep) {
  s32 group = SCENE_RECORD(gPartyState.scene)->group;
  s32 flag;

  if (keep == 0) {
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBit(flag);
    if (group != gPartyState.sceneGroup) {
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBit(flag);
      GameFlag_SetBit(0x12f);
      gPartyState.encounterSteps = 0;
      gPartyState.hazardSteps = 0;
      GameFlag_ClearBit(0x110);
      GameFlag_ClearBit(0x111);
      GameFlag_ClearBit(0x112);
      GameFlag_ClearBit(0x113);
      {
        u32 scene = *(u16 *)&gPartyState.scene;

        asm("" : "+l"(scene) : : "r1", "r3");
        gPartyState.retreatScene = scene;
      }
      gPartyState.retreatEntrance = gPartyState.entrance;
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
    {
      struct PartyState *party = &gPartyState;

      do {
      } while (0);
      if (party->ownerOverride != 0)
        GameFlag_SetBit(0x176);
      else
        GameFlag_ClearBit(0x176);
    }
    {
      struct PartyState *party = &gPartyState;

      do {
      } while (0);
      party->returnCue = -1;
    }
  }
  gPartyState.sceneGroup = group;
  do {
  } while (0);
  Func_080c9de0(gPartyState.scene, gPartyState.entrance);
  {
    const struct SceneRecord *record = SCENE_RECORD(gPartyState.scene);

    do {
    } while (0);
    gPartyState.sceneVariant = record->variant;
  }
  if (gPartyState.sceneVariant == 2)
    GameFlag_SetBit(0x123);
  GameFlag_RefreshLureCapFar();
}
