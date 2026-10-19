#ifndef ALCHEMY_GAME_STATE_H
#define ALCHEMY_GAME_STATE_H

/// @file
/// GameState, the game's state: play time, coins, the scene and the leader's
/// position, the active party and the options.

#include "types.h"
#include "static_assert.h"
#include "battle_unit.h"

/// The game's state: play time, coins, the scene and the leader's position,
/// the active party and the options.
struct GameState {
  u32 buildStamp;
  u32 playTime;
  u8 unk008[8];
  s32 coins;
  u8 unk014[0x104];
  s32 shopCredit;
  s8 shopGifts;
  s8 prizeWeights[0x0f];
  s8 wonPrizes[0x10];
  u8 unk13c[0x84];
  s16 scene;
  s16 entrance;
  s16 savedScene;
  s16 savedEntrance;
  s16 defaultScene;
  s16 defaultEntrance;
  s16 sceneGroup;
  s16 nextScene;
  s16 nextEntrance;
  s16 defeatScene;
  s16 defeatEntrance;
  s16 special;
  u8 unk1d8[2];
  s16 map;
  s32 x;       ///< 16.16 fixed-point pixels
  s32 y;       ///< 16.16 fixed-point pixels
  s32 z;       ///< 16.16 fixed-point pixels
  u32 heading; ///< 1/65536 turns
  u16 turn;
  s16 sceneCue;
  s16 cue;
  u8 movementMode;
  u8 unk1f3;
  s32 selectedActor;
  u8 activeOwners[8];
  u8 unk200[5];
  u8 paletteGlow[2];
  u8 unk207[3];
  u8 speech;
  u8 unk20b;
  u8 messageSpeed;
  u8 unk20d[2];
  u8 unk20f;
  u16 aKeys;
  u16 bKeys;
  u16 selectKeys;
  u16 startKeys;
  u16 firstShortcutKeys;
  u16 secondShortcutKeys;
  u16 dashKeys;
  s16 returnCue;
  u16 firstShortcut;
  u16 secondShortcut;
  u8 perspectiveScene;
  u8 unk225[5];
  u8 idleSleepEnabled;
  u8 battleStart;
  s16 hazardLimit;
  s16 hazardMode;
  s16 hazardDamage;
  s16 hazardSteps;
  s16 pendingDjinnEvent;
  s16 sceneChangeReason;
  s32 encounterSteps;
  u16 pendingItem;
  s16 sceneVariant;
  s16 retreatScene;
  s16 retreatEntrance;
  s32 avoidDistance;
  s8 avoidByItem;
  s8 pausedPose;
  s16 pausedObject;
  s16 cloaked;
  u8 unk24e[2];
  u32 pausedUpdate;
  u8 unk254[0x50];
  u16 linkTallies[8];
  u8 unk2b4[4];
  u32 startStamp;
  u8 unk2bc[4];
  struct BattleUnit owners[8];
};

STATIC_ASSERT(offsetof(struct GameState, buildStamp) == 0);
STATIC_ASSERT(offsetof(struct GameState, playTime) == 4);
STATIC_ASSERT(offsetof(struct GameState, idleSleepEnabled) == 0x22a);
STATIC_ASSERT(offsetof(struct GameState, coins) == 0x10);
STATIC_ASSERT(offsetof(struct GameState, shopCredit) == 0x118);
STATIC_ASSERT(offsetof(struct GameState, shopGifts) == 0x11c);
STATIC_ASSERT(offsetof(struct GameState, owners) == 0x2c0);
STATIC_ASSERT(sizeof(struct GameState) == 0xd20);
STATIC_ASSERT(offsetof(struct GameState, wonPrizes) == 0x12c);
STATIC_ASSERT(offsetof(struct GameState, sceneGroup) == 0x1cc);
STATIC_ASSERT(offsetof(struct GameState, dashKeys) == 0x21c);
STATIC_ASSERT(offsetof(struct GameState, avoidDistance) == 0x244);
STATIC_ASSERT(offsetof(struct GameState, entrance) == 0x1c2);
STATIC_ASSERT(offsetof(struct GameState, special) == 0x1d6);
STATIC_ASSERT(offsetof(struct GameState, x) == 0x1dc);
STATIC_ASSERT(offsetof(struct GameState, movementMode) == 0x1f2);
STATIC_ASSERT(offsetof(struct GameState, selectedActor) == 0x1f4);
STATIC_ASSERT(offsetof(struct GameState, activeOwners) == 0x1f8);
STATIC_ASSERT(offsetof(struct GameState, speech) == 0x20a);
STATIC_ASSERT(offsetof(struct GameState, messageSpeed) == 0x20c);
STATIC_ASSERT(offsetof(struct GameState, aKeys) == 0x210);
STATIC_ASSERT(offsetof(struct GameState, firstShortcutKeys) == 0x218);
STATIC_ASSERT(offsetof(struct GameState, secondShortcutKeys) == 0x21a);
STATIC_ASSERT(offsetof(struct GameState, firstShortcut) == 0x220);
STATIC_ASSERT(offsetof(struct GameState, retreatEntrance) == 0x242);
STATIC_ASSERT(offsetof(struct GameState, cloaked) == 0x24c);
STATIC_ASSERT(offsetof(struct GameState, linkTallies) == 0x2a4);
STATIC_ASSERT(offsetof(struct GameState, startStamp) == 0x2b8);

#ifndef OWN_gGameState
extern struct GameState gGameState;
#endif

/// Sets the battle start type.
// A statement macro (S2: a layer, 34 sites): written as one, its store stays in
// place at every use; as a plain assignment, tbs-en differs.
#define Battle_SetStart(mode)                                                  \
  do {                                                                         \
    gGameState.battleStart = (mode);                                           \
  } while (0)

#endif
