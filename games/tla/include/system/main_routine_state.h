#ifndef ALCHEMY_MAIN_ROUTINE_STATE_H
#define ALCHEMY_MAIN_ROUTINE_STATE_H

/// @file
/// gMainRoutineState, the game's state a save file keeps, and the values its
/// fields hold.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/// A scene's type (Scene.sceneType, copied to MainRoutineState.sceneType).
enum SceneType {
  SCENE_TYPE_OTHER = 1, ///< every other scene: dungeons, the world map
  /// Towns and their rooms, and the prologue's Venus Lighthouse scenes:
  /// FLAG_IN_TOWN is set there (Scene_Arrive).
  SCENE_TYPE_TOWN = 2
};

/// gMainRoutineState.battleType: how the next battle starts; it is set back to
/// BATTLE_TYPE_NORMAL as the battle ends.
enum BattleType {
  BATTLE_TYPE_NORMAL = 0,
  /// A battle the party cannot run from: a Mimic's, a hidden Mad Plant's and
  /// the bosses'.
  BATTLE_TYPE_NO_ESCAPE = 2,
  BATTLE_TYPE_LINK = 4,  ///< a link battle, from the lobby
  BATTLE_TYPE_DEBUG = 5, ///< the debug battle menu's mark
  /// A battle the party is meant to lose: it is caught off guard and every
  /// hit downs its target, as when Karst and Agatio fight Isaac and Ivan.
  BATTLE_TYPE_LOST = 6
};

/// Battle_Run's results in gMainRoutineState.sequenceResult besides the number
/// of enemies defeated.
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999

/// The most coins the party carries, and the most shop credit.
#define COIN_MAX 999999

/// gMainRoutineState.hazardMode: what advances the hazard count
/// (Field_UpdateWalking).
enum HazardMode {
  /// The distance the leader walks on MAP_CELL_HAZARD ground.
  HAZARD_COUNT_ON_CELLS = 0,
  /// One each frame the field sequence runs; no code in the game sets it.
  HAZARD_COUNT_FRAMES = 1
};

/// gMainRoutineState.hazardDamage's bit 8: its low byte is a percentage of
/// maximum HP rather than HP.
#define HAZARD_DAMAGE_PERCENT 0x100

/// The most Game Tickets counted in MainRoutineState.shopGifts, the last of
/// gShop_GameTicketCreditCosts' prices that is used.
#define SHOP_GIFTS_MAX 28

/// The first pending Djinni battle event: DJINNI_BATTLE_EVENT_FIRST plus the
/// Djinni's number (Field_RequestDjinnBattle).
#define DJINNI_BATTLE_EVENT_FIRST 300

/// gMainRoutineState.difficulty: the level chosen on starting again from a
/// cleared game.
enum Difficulty {
  DIFFICULTY_NORMAL = 0, ///< "Play on Normal level? (Default difficulty)"
  /// "Play on EASY level? (Characters start at Lost Age Clear Data levels)"
  DIFFICULTY_EASY = 1,
  DIFFICULTY_HARD = 2 ///< "Play on HARD level? (Monsters are stronger)"
};

/// gMainRoutineState, the state a save file keeps: play time, coins, the scene
/// and the leader's position, the active party and the options.
struct MainRoutineState {
  /// BuildStamp_GetTime's value when the game was last saved; 0 on a new
  /// game.
  u32 buildStamp;
  u32 playTime; ///< frames
  u8 unused[4];
  /// The summons the party can call, bit n for summon n: the first word of
  /// the party's DjinnRecoveryTable, which Djinn_GetRecoveryTable returns from
  /// here, its next word running over coins.
  u32 summonMask;
  s32 coins; ///< at most COIN_MAX (Party_AdjustCoins)
  /// The party's Djinn recovery list (DjinnRecoveryTable from its offers on),
  /// read through Djinn_GetRecoveryTable.
  u8 djinnRecovery[292];
  s32 shopCredit; ///< shop credit, at most COIN_MAX (Party_AdjustShopCredit)
  s8 shopGifts;   ///< shop gifts, 0 to SHOP_GIFTS_MAX (Party_AdjustShopGifts)
  /// The weights of the spring game's 15 prizes: three
  /// each of 4, 8, 16, 32 and 64 on a new game; Lemuria's spring halves a
  /// prize's weight each time it is won.
  u8 prizeWeights[15];
  /// The icons matched on the Lucky Wheels, a prize each, ended by -1, for
  /// Contigo's attendant to hand out (-1 first: no prize, -2: handed out
  /// already).
  s8 wheelsPrizes[8];
  /// For each icon, which of its three prizes it gave last
  /// (Contigo_SelectLuckyWheelsPrize).
  s8 wheelsPrizeCursors[6];
  u8 unusedAfterWheelsPrizes[134]; ///< no code names it
  s16 next;
  s16 entrance;
  u16 restartScene;    ///< where a defeat returns to without a defeat point
  u16 restartEntrance; ///< where a defeat returns to without a defeat point
  s16 returnScene;     ///< the default scene: the fallback of both destinations
  s16 returnEntrance;  ///< the default entrance: the fallback of both
                       ///< destinations
  s16 area; ///< the current scene's group; a new group resets its state
  /// The after-battle destination, -1 for none: read by
  /// Scene_ReturnFromSequence when the party was not defeated, before the
  /// default scene and entrance.
  u16 afterBattleScene;
  u16 afterBattleEntrance; ///< its entrance; -1 for none
  /// The defeat destination, -1 for none: read by Scene_ReturnFromSequence
  /// after a defeat.
  u16 defeatScene;
  u16 defeatEntrance;   ///< its entrance; -1 for none
  s16 battleBackground; ///< the battle background chosen for the scene or
                        ///< terrain
  u8 unusedAfterBackground[2];
  s16 mapId;
  s32 x; ///< the party's position on entering
  s32 y;
  s32 z;
  s32 heading;
  u16 layer;              ///< 0 on taking an entrance
  s16 battleMusic;        ///< the battle music chosen for the encounter
  s16 sceneMusic;         ///< the current scene's field music
  u8 movementMode;        ///< how the leader moves, an enum LeaderMovementMode
  s32 leaderActorId;      ///< the leader
  u8 party[8];            ///< the party in order
  u8 unusedAfterParty[5]; ///< no code names it
  u8 windowHue;           ///< the "Window color" setting (option 0)
  u8 windowBrightness;    ///< the "Window brightness" setting (option 1)
  u8 unusedAfterBrightness[3];
  u8 speech;            ///< option 3, on or off; 1 on a new game
  u8 unusedAfterSpeech; ///< 1 on a new game; nothing reads it
  /// Option 2, the message speed, 0 to 2: 1 on a new game; some scenes set 2
  /// and restore it after.
  u8 messageSpeed;
  u8 unusedAfterMessageSpeed[2];
  /// Set on saving at the end of the game, marking the file cleared: its data
  /// can be sent. Copied to the file's summary as sendFlag.
  u8 sendFlag;
  /// The buttons for each field command, as KEY_ masks, set on a new game;
  /// read with gInput.pressed except dashKeys, held.
  ///@{
  u16 aKeys;              ///< examine; KEY_A
  u16 bKeys;              ///< KEY_B; held with aKeys, it makes flight climb
  u16 selectKeys;         ///< the menu; KEY_SELECT
  u16 startKeys;          ///< KEY_START
  u16 firstShortcutKeys;  ///< KEY_L
  u16 secondShortcutKeys; ///< KEY_R
  u16 dashKeys;           ///< KEY_B
  ///@}
  s16 returnMusic; ///< the music to play on returning, -1 for the scene's own
  /// The shortcuts' assignments, run by firstShortcutKeys and
  /// secondShortcutKeys (PsynergyMenu_SetShortcut: kind << 10 | id).
  u16 firstShortcut;
  u16 secondShortcut;

  u8 perspectiveMap; ///< set to 1 on the world map
  u8 unusedAfterPerspective[5];
  /// Option 4, Auto-Sleep, saved from gIdleSleepEnabled.
  u8 idleSleepEnabled;
  u8 battleType; ///< an enum BattleType, set back to BATTLE_TYPE_NORMAL at the
                 ///< end
  /// Hazardous ground: when hazardSteps reaches hazardLimit the party takes
  /// hazardDamage and the count starts again.
  ///@{
  s16 hazardLimit;
  s16 hazardMode;
  s16 hazardDamage;
  u16 hazardSteps; ///< reset on entering a new scene group
  ///@}
  /// The Djinni battle pending (DJINNI_BATTLE_EVENT_FIRST plus its number,
  /// flags in bits 15 to 11), or a Mimic's flag with CONDITION_FLAG_SET,
  /// resolved on returning to the field.
  s16 pendingDjinni;
  /// The last battle's result: the enemies defeated, BATTLE_OUTCOME_FLED or
  /// BATTLE_OUTCOME_LOST.
  s16 sequenceResult;
  s32 encounterSteps; ///< reset on entering a new scene group
  /// Cleared with the battle's spoils; no code reads it by name.
  u16 pendingItem;
  s16 sceneType;       ///< the current scene's enum SceneType
  s16 retreatScene;    ///< where Retreat goes; the group's first entrance
  s16 retreatEntrance; ///< where Retreat goes; the group's first entrance
  FIXED avoidDistance; ///< distance left of Avoid, 16.16 pixels; 0 when off
  s8 avoidByItem;      ///< Avoid came from an item (FLAG_PSYNERGY_FROM_ITEM)
  u8 pausedPose;       ///< the paused object's first animation param
  s16 haltedActor;     ///< the object field Psynergy has stopped, -1 for none
  /// Nonzero while the leader is sand (FieldPsynergy_Sand): ending the
  /// pending actions ends Sand too, and the field's push and button handler
  /// idles.
  s8 sandActive;
  s8 cloaked; ///< Cloak: the leader is hidden
  u8 unusedAfterCloaked[2];
  void *pausedUpdate; ///< the paused object's update, restored on resuming
  /// The weird answers Felix gives, counted up by Event_CountAnswer from 0 on a
  /// new game: telling Kraden on Idejima that the Western Sea is not where
  /// they go, agreeing with Alhafra's mayor that the party owes him, and
  /// others. Past four, as Felix answers Ancient Lemuria's guards' "Do we have
  /// your word?", Kraden says "Sometimes, you can say the weirdest things..."
  /// to a yes and "Are you INSANE!?" to a no.
  u8 weirdAnswers;
  /// Nonzero makes actor 8, not leaderActorId, the one events control.
  s16 ownerOverride;
  s32 shipX;      ///< the ship's x on the world map, where it is moored
  s32 shipZ;      ///< the ship's z on the world map
  s32 shipFacing; ///< the ship's facing on the world map
  u8 unusedAfterShipFacing[2]; ///< no code names it
  /// gAnyFileCleared as the new game began
  /// (MainRoutineState_InitializeDefaults): 1 when a file on the cartridge had
  /// been cleared. Nothing reads it back.
  u8 clearedFileAtStart;
  u8 difficulty; ///< the level chosen from a cleared game (Difficulty)
  u8 unusedAfterDifficulty[4]; ///< no code names it
  /// The Broken Seal's coins, kept from the data transfer (DataTransfer_Apply)
  /// until Isaac's party joins and Party_AddIsaacsParty adds them.
  s32 transferredCoins;
  u8 unusedAfterTransferredCoins[44]; ///< no code names it
  u16 forgeItem; ///< the item left at the forge (Forge_SetItem)
  /// Teleport's destination: SCENE_WORLD_MAP once a place is chosen on the
  /// map screen (MapScreen_Show), -1 after cancelling.
  s16 teleportScene;
  /// The chosen place's flag, which the world map takes as its entrance; -1
  /// after cancelling.
  s16 teleportEntrance;
  /// 1 once a place is chosen and 2 as the party arrives, when the scene it
  /// lands in plays the arrival and clears it.
  s8 teleportState;
  /// Set while the leader stands on the upper level of the Alhafran Cave,
  /// where Sand ends; cleared on entering a scene.
  s8 onUpperLevel;
  /// The link lobby's tallies: 3 cleared on choosing a monster
  /// battle, 0 and 3 counted up together, 6 read by the lobby.
  u16 linkTallies[8];
  u8 unusedAfterTallies[4];
  u32 startStamp; ///< BuildStamp_GetTime's value at a new game
  u8 unusedAfterStartStamp[4];
};

STATIC_ASSERT(offsetof(struct MainRoutineState, coins) == 16);
STATIC_ASSERT(offsetof(struct MainRoutineState, next) == 480);
STATIC_ASSERT(offsetof(struct MainRoutineState, leaderActorId) == 532);
STATIC_ASSERT(offsetof(struct MainRoutineState, battleType) == 587);
STATIC_ASSERT(offsetof(struct MainRoutineState, ownerOverride) == 630);
STATIC_ASSERT(offsetof(struct MainRoutineState, linkTallies) == 708);
STATIC_ASSERT(sizeof(struct MainRoutineState) == 736);

/// The game's state (system/ewram.s).
extern struct MainRoutineState gMainRoutineState;

/// Sets how the next battle starts (an enum BattleType).
#define Battle_SetType(mode)                                                   \
  do {                                                                         \
    gMainRoutineState.battleType = (mode);                                     \
  } while (0)

s32 Party_CountMembers(void);
s32 Party_ListMembers(s16 *owners);
void Unit_LevelUpTo(s32 owner, s32 level);

#endif
