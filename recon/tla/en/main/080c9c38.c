// Draft, differ 117 of 174: resets scene state on entering a scene. Unless
// keepFlags, it clears the scene flags 0x200 to 0x2ff; on a change of scene
// group (gField_SceneTable's group against PartyState.sceneGroup) also the
// area flags 0x300 to 0x3ff, sets flag 0x12f, zeroes the encounter steps and
// the halfword at 0x252, clears flags 0x110 to 0x113 and records the scene
// and entrance at 0x260; then clears flags 0x80 to 0xdf and nine single
// flags, sets or clears flag 0x176 by ownerOverride and writes 0xffff at
// 0x23e. Always stores the group, runs Func_080c9de0(scene, entrance), and
// stores the scene's variant at 0x25e (variant 2 sets flag 0x123). The first
// argument is unused. Left: mostly register choice of the rebuilt offsets
// (the native rotates r1, r2 and r3 the other way and steps 0x258 to 0x252
// and 0x1ec to 0x1e2 with subs), and the 0xffff store, built with
// movs/lsls/adds in the native but a halfword pool load in the draft.
// The fields past the header's names go through a view of gPartyState
// (as byte-offset casts the constants fold into pool words, differ 141).
// The draft also takes r0, free as the unused first argument, where the
// native avoids it. Tried: variant stored and compared from one local
// (from 154).
#include "types.h"
#include "party_state.h"
#include "field_scene.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_RefreshLureCapFar(void);
void Func_080c9de0(s32 scene, s32 entrance);

// PartyState fields this function reaches past the header's names.
struct PartySceneView {
  u8 unk000[0x23e];
  u16 unk23e;
  u8 unk240[0x12];
  u16 unk252;
  u8 unk254[0xa];
  s16 unk25e;
  u16 unk260;
  u16 unk262;
};

#define gPartyScene (*(struct PartySceneView *)&gPartyState)

void Func_080c9c38(s32 unused, s32 keepFlags) {
  s32 group = gField_SceneTable[gPartyState.scene].group;
  s32 flag;
  s32 variant;

  if (keepFlags == 0) {
    for (flag = 0x200; flag <= 0x2ff; flag++)
      GameFlag_ClearBit(flag);
    if (group != gPartyState.sceneGroup) {
      for (flag = 0x300; flag <= 0x3ff; flag++)
        GameFlag_ClearBit(flag);
      GameFlag_SetBit(0x12f);
      gPartyState.encounterSteps = 0;
      gPartyScene.unk252 = 0;
      GameFlag_ClearBit(0x110);
      GameFlag_ClearBit(0x111);
      GameFlag_ClearBit(0x112);
      GameFlag_ClearBit(0x113);
      gPartyScene.unk260 = gPartyState.scene;
      gPartyScene.unk262 = gPartyState.entrance;
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
    if (gPartyState.ownerOverride != 0)
      GameFlag_SetBit(0x176);
    else
      GameFlag_ClearBit(0x176);
    gPartyScene.unk23e = 0xffff;
  }
  gPartyState.sceneGroup = group;
  Func_080c9de0(gPartyState.scene, gPartyState.entrance);
  variant = gField_SceneTable[gPartyState.scene].variant;
  gPartyScene.unk25e = variant;
  if (variant == 2)
    GameFlag_SetBit(0x123);
  GameFlag_RefreshLureCapFar();
}
