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
///
/// It is the one gGameState in EWRAM, right after the 0x200 bytes of game
/// flags; a save file's checksum covers the 968 words from gGameFlagBytes to
/// the end of this struct (SaveState_BuildSummaryHeader). A new game sets its
/// defaults (GameState_InitDefaults), and the field, battle, menus and scenes
/// read and write it directly.
struct GameState {
  /// Runtime_GetBuildStampTime's value when the game was last saved; its low
  /// half goes into the file's summary.
  u32 buildStamp;
  u32 playTime; ///< frames, copied from gPlayTimeFrames when saving
  /// Not named by TBS code. The party's struct DjinnRecoveryTable
  /// (Djinn_GetRecoveryTable) starts at byte 4, its first 8 bytes running
  /// over coins.
  u8 unk008[8];
  s32 coins; ///< 0 to 999999 (Party_AdjustCoins)
  /// The party's Djinn recovery list (struct DjinnRecoveryList), read through
  /// Djinn_GetRecoveryTable; not named by TBS code.
  u8 unk014[0x104];
  s32 shopCredit; ///< spent on a Game Ticket; not written by name in TBS
  s8 shopGifts;   ///< indexes the Game Ticket's credit cost
  /// The slot machine's prize weights, three prizes to each of 4, 8, 16, 32
  /// and 64 on a new game; not read by name in TBS.
  s8 prizeWeights[0x0f];
  /// Lucky Wheels: 0 to 7 the prize categories won, ended by -1 (-2 once
  /// awarded); 8 on the last variant given of each category.
  s8 wonPrizes[0x10];
  u8 unk13c[0x84]; ///< not named by TBS code
  /// The scene being played: its index in gField_SceneTable (a SceneId_
  /// symbol), or SCENE_BATTLE or another number above the field scenes.
  s16 scene;
  s16 entrance;        ///< the entrance taken; in SCENE_BATTLE, the enemy group
  s16 savedScene;      ///< where a defeat returns to without a defeat point
  s16 savedEntrance;   ///< where a defeat returns to without a defeat point
  s16 defaultScene;    ///< the scene being run, kept as it starts
  s16 defaultEntrance; ///< the entrance it was entered by
  s16 sceneGroup;   ///< the current scene's group; a new group resets its flags
  s16 nextScene;    ///< where the event sends the party next; -1 for none
  s16 nextEntrance; ///< where the event sends the party next; -1 for none
  s16 defeatScene;  ///< where a defeat returns to; -1 for none
  s16 defeatEntrance; ///< where a defeat returns to; -1 for none
  s16 special;        ///< the battle backdrop chosen for the scene or terrain
  u8 unk1d8[2];       ///< not named by TBS code
  /// The scene record's map (mapIndex); the world map's is 1, which the
  /// code compares as (s32)&SceneId_Clear.
  s16 map;
  s32 x;              ///< 16.16 fixed-point pixels
  s32 y;              ///< 16.16 fixed-point pixels
  s32 z;              ///< 16.16 fixed-point pixels
  u32 heading;        ///< 1/65536 turns
  u16 turn;           ///< the leader's terrainId, kept with its position
  s16 sceneCue;       ///< the battle music chosen for the encounter
  s16 cue;            ///< the current scene's field music
  u8 movementMode;    ///< 1 while the leader is climbing, 0 otherwise
  u8 unk1f3;          ///< not named by TBS code
  s32 selectedActor;  ///< the leader: its unit and its actor
  u8 activeOwners[8]; ///< the party in order
  u8 unk200[5];       ///< not named by TBS code
  u8 paletteGlow[2];  ///< window hue (0 to 23), brightness (0 to 14)
  u8 unk207[3];       ///< not named by TBS code
  /// The voice sound option: 1, as on a new game, plays each speaker's voice
  /// sound as their message types (BattleFx_GetResourceId).
  u8 speech;
  u8 unk20b;       ///< 1 on a new game; not read by name in TBS
  u8 messageSpeed; ///< 0 to 2; 1 on a new game
  u8 unk20d[2];    ///< not named by TBS code
  /// Set on saving at the end of the game; copied to the file's summary as
  /// sendFlag.
  u8 unk20f;
  /// The buttons for each field command, as KEY_ masks; read with gKeysPressed
  /// except dashKeys, held.
  ///@{
  u16 aKeys;              ///< examine; KEY_A
  u16 bKeys;              ///< KEY_B; not read by name in TBS
  u16 selectKeys;         ///< the menu; KEY_SELECT
  u16 startKeys;          ///< KEY_START
  u16 firstShortcutKeys;  ///< firstShortcut; KEY_L
  u16 secondShortcutKeys; ///< secondShortcut; KEY_R
  u16 dashKeys;           ///< KEY_B
  ///@}
  s16 returnCue; ///< music playing when the field left; -1 for none
  /// The shortcuts' assignments. bits: 15..14 kind (0 Psynergy, 1 an item) |
  /// 13..0 which one.
  ///@{
  u16 firstShortcut;
  u16 secondShortcut;
  ///@}
  u8 perspectiveScene; ///< set to 1 on the world map; not read by name in TBS
  u8 unk225[5];        ///< not named by TBS code
  u8 idleSleepEnabled; ///< the option, saved from gIdleSleepEnabled
  u8 battleStart;      ///< how the next battle starts (Battle_SetStart)
  /// Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches
  /// hazardLimit the party takes hazardDamage and the count starts again. A
  /// fresh entry into a scene, not a return from battle, resets them to 1 HP
  /// every 16 pixels walked on it; the desert sets 25 percent every 600
  /// (Field_RunScene, LamakanDesert_Initialize).
  ///@{
  s16 hazardLimit; ///< pixels walked on hazard cells (mode 0), or frames (1)
  s16 hazardMode;  ///< 0 counts on hazard cells only; 1 counts every frame
  /// bits: 8 the amount is a percentage of maximum HP | 7..0 the amount
  s16 hazardDamage;
  s16 hazardSteps; ///< reset on entering a new scene group
  ///@}
  /// The Djinni battle pending (DJINNI_0 plus its number, flags in bits 15 to
  /// 11), resolved on returning to the field.
  s16 pendingDjinnEvent;
  s16 sceneChangeReason; ///< the last battle's result; -1 for a defeat
  s32 encounterSteps;    ///< the encounter count, reset with a new group
  u16 pendingItem;  ///< given on returning to the field if the party has room
  s16 sceneVariant; ///< the current scene's variant from its record
  s16 retreatScene; ///< where Retreat goes; the group's first entrance
  s16 retreatEntrance; ///< where Retreat goes; the group's first entrance
  s32 avoidDistance;   ///< distance left of Avoid, 16.16 pixels; 0 when off
  s8 avoidByItem;      ///< Avoid came from an item (the Sacred Feather)
  s8 pausedPose;       ///< Halt: the stopped object's first animation param
  s16 pausedObject;    ///< Halt: the stopped object; -1 for none
  s16 cloaked;         ///< 1 under Cloak, which prevents battles
  u8 unk24e[2];        ///< not named by TBS code
  u32 pausedUpdate;    ///< Halt: the stopped object's update; not read by name
  u8 unk254[0x50];     ///< not named by TBS code
  /// The link lobby's tallies: 4 the total wins and 7 the consecutive wins it
  /// reports, 3 cleared on choosing a monster battle; the others not named by
  /// TBS code.
  u16 linkTallies[8];
  u8 unk2b4[4];   ///< not named by TBS code
  u32 startStamp; ///< Runtime_GetBuildStampTime's value at a new game
  u8 unk2bc[4];   ///< not named by TBS code
  struct BattleUnit owners[8]; ///< the characters
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
/// The game's state (system/ewram.s).
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
