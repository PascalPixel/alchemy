#ifndef ALCHEMY_GAME_STATE_H
#define ALCHEMY_GAME_STATE_H

/// @file
/// GameState, the game's state: play time, coins, the map and the leader's
/// position, the active party and the options.

#include "types.h"
#include "static_assert.h"
#include "battle_unit.h"

/// A map's variant (MapRecord.variant, copied to GameState.mapType).
enum MapType {
  MAP_TYPE_FIELD = 1, ///< every other map: dungeons, the world map
  /// Towns and their interiors, where walking restores no PP, recovers no
  /// Djinn and does no poison damage, Retreat is blocked, Avoid does not wear
  /// off and FLAG_IN_TOWN is set.
  MAP_TYPE_TOWN = 2
};

/// gGameState.hazardMode: what advances the hazard count (Field_ProcessStep).
enum HazardMode {
  /// The distance the leader walks on MB_DESERT_SAND ground.
  HAZARD_COUNT_ON_CELLS = 0,
  /// One each frame the field loop runs; no ☀️ code sets it.
  HAZARD_COUNT_FRAMES = 1
};

/// gGameState.hazardDamage's bit 8: its low byte is a percentage of maximum HP
/// rather than HP.
#define HAZARD_DAMAGE_PERCENT 0x100

/// gGameState.battleStart: how the next battle starts (Battle_SetStart); it is
/// set back to BATTLE_START_NORMAL as the battle ends. Any start but that one
/// stops the opening's first-strike and surprise rolls (Battle_RunEncounter).
enum BattleStart {
  BATTLE_START_NORMAL = 0,
  /// A battle the party cannot run from: a Mimic's (object/object/events.c)
  /// and the travelers' fight in Vale (ValeGate_FightTravelers,
  /// map/vale_gate.c).
  BATTLE_START_NO_ESCAPE = 2,
  /// The battles a script starts. Here and in a link battle the command menu
  /// itself refuses to run, with MsgNoTimeToRun ("This is no time to be
  /// running away!"), as it does once the party is challenged
  /// (UiText_ShowLocalizedMessageAndWait, window/battle_command/link_timer.c).
  BATTLE_START_SCRIPTED = 3,
  BATTLE_START_LINK = 4, ///< a link battle (Battle_RunEncounter, menu/lobby.c)
  /// The debug battle menu's mark (DebugBattle_RunMenu), which no code tests
  /// by value; it still lets the party run.
  BATTLE_START_DEBUG = 5
};

/// The game's state: play time, coins, the map and the leader's position,
/// the active party and the options.
///
/// It is the one gGameState in EWRAM, right after the 0x200 bytes of game
/// flags; a save file's checksum covers the 968 words from gGameFlagBytes to
/// the end of this struct (SaveState_BuildSummaryHeader). A new game sets its
/// defaults (GameState_InitializeDefaults), and the field, battle, menus and
/// maps read and write it directly.
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
  /// The map being played: its index in gField_MapTable (a MapId_
  /// symbol), or RUN_BATTLE or another number above the field maps.
  s16 mapId;
  s16 warpDest; ///< the warp destination taken; in RUN_BATTLE, the enemy group
  s16 savedMap; ///< where a defeat returns to without a defeat point
  s16 savedWarpDest;   ///< where a defeat returns to without a defeat point
  s16 defaultMap;      ///< the map being run, kept as it starts
  s16 defaultWarpDest; ///< the warp destination it was entered by
  s16 mapSec;  ///< the current map's map section; a new one resets its flags
  s16 nextMap; ///< where the script sends the party next; -1 for none
  s16 nextWarpDest;   ///< where the script sends the party next; -1 for none
  s16 defeatMap;      ///< where a defeat returns to; -1 for none
  s16 defeatWarpDest; ///< where a defeat returns to; -1 for none
  s16 special;        ///< the battle backdrop chosen for the map or terrain
  u8 unk1d8[2];       ///< not named by TBS code
  /// The map record's layout (layoutIndex); the world map's is 1, which the
  /// code compares as (s32)&MapId_Clear.
  s16 layoutId;
  s32 x;               ///< 16.16 fixed-point pixels
  s32 y;               ///< 16.16 fixed-point pixels
  s32 z;               ///< 16.16 fixed-point pixels
  u32 heading;         ///< 1/65536 turns
  u16 turn;            ///< the leader's terrainId, kept with its position
  s16 battleCue;       ///< the battle music chosen for the encounter
  s16 cue;             ///< the current map's field music
  u8 movementMode;     ///< 1 while the leader is climbing, 0 otherwise
  u8 unk1f3;           ///< not named by TBS code
  s32 selectedLocalId; ///< the leader: its unit and its object event
  u8 activeOwners[8];  ///< the party in order
  u8 unk200[5];        ///< not named by TBS code
  u8 paletteGlow[2];   ///< window hue (0 to 23), brightness (0 to 14)
  u8 unk207[3];        ///< not named by TBS code
  /// The voice sound option: 1, as on a new game, plays each speaker's voice
  /// sound as their message types (FieldEffect_GetResourceId).
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
  u8 perspectiveMap;   ///< set to 1 on the world map; not read by name in TBS
  u8 unk225[5];        ///< not named by TBS code
  u8 idleSleepEnabled; ///< the option, saved from gIdleSleepEnabled
  u8 battleStart;      ///< an enum BattleStart
  /// Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches
  /// hazardLimit the party takes hazardDamage and the count starts again. A
  /// fresh entry into a map, not a return from battle, resets them to 1 HP
  /// every 16 pixels walked on it; the desert sets 25 percent every 600
  /// (Field_RunMap, LamakanDesert_Initialize).
  ///@{
  s16 hazardLimit; ///< pixels walked on hazard cells, or frames, by hazardMode
  s16 hazardMode;  ///< an enum HazardMode
  /// bits: 8 the amount is a percentage of maximum HP | 7..0 the amount
  s16 hazardDamage;
  s16 hazardSteps; ///< reset on entering a new map section
  ///@}
  /// The Djinni battle pending (DJINNI_EVENT_0 plus its number, flags in bits
  /// 15 to 11), resolved on returning to the field.
  s16 pendingDjinnEvent;
  s16 mapChangeReason; ///< the last battle's result; -1 for a defeat
  s32 encounterSteps;  ///< the encounter count, reset with a new map section
  u16 pendingItem; ///< given on returning to the field if the party has room
  s16 mapType;     ///< the current map's variant from its record
  s16 retreatMap;  ///< where Retreat goes; the map section's first warp
                   ///< destination
  s16 retreatWarpDest; ///< where Retreat goes; the map section's first warp
                       ///< destination
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
STATIC_ASSERT(offsetof(struct GameState, mapSec) == 0x1cc);
STATIC_ASSERT(offsetof(struct GameState, dashKeys) == 0x21c);
STATIC_ASSERT(offsetof(struct GameState, avoidDistance) == 0x244);
STATIC_ASSERT(offsetof(struct GameState, warpDest) == 0x1c2);
STATIC_ASSERT(offsetof(struct GameState, special) == 0x1d6);
STATIC_ASSERT(offsetof(struct GameState, x) == 0x1dc);
STATIC_ASSERT(offsetof(struct GameState, movementMode) == 0x1f2);
STATIC_ASSERT(offsetof(struct GameState, selectedLocalId) == 0x1f4);
STATIC_ASSERT(offsetof(struct GameState, activeOwners) == 0x1f8);
STATIC_ASSERT(offsetof(struct GameState, speech) == 0x20a);
STATIC_ASSERT(offsetof(struct GameState, messageSpeed) == 0x20c);
STATIC_ASSERT(offsetof(struct GameState, aKeys) == 0x210);
STATIC_ASSERT(offsetof(struct GameState, firstShortcutKeys) == 0x218);
STATIC_ASSERT(offsetof(struct GameState, secondShortcutKeys) == 0x21a);
STATIC_ASSERT(offsetof(struct GameState, firstShortcut) == 0x220);
STATIC_ASSERT(offsetof(struct GameState, retreatWarpDest) == 0x242);
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
