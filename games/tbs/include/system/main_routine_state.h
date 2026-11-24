#ifndef ALCHEMY_MAIN_ROUTINE_STATE_H
#define ALCHEMY_MAIN_ROUTINE_STATE_H

/// @file
/// The main routine's state: the transition data the main routine reads
/// between sequences and the state every sequence shares, which a save file
/// keeps.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_unit.h"

/// A scene's type (Scene.sceneType, copied to MainRoutineState.sceneType).
enum SceneType {
  SCENE_TYPE_OTHER = 1, ///< every other scene: dungeons, the world map
  /// Towns and their rooms, where walking restores no PP, recovers no
  /// Djinn and does no poison damage, Retreat is blocked, Avoid does not wear
  /// off and FLAG_IN_TOWN is set.
  SCENE_TYPE_TOWN = 2
};

/// gMainRoutineState.hazardMode: what advances the hazard count
/// (Field_UpdateWalking).
enum HazardMode {
  /// The distance the leader walks on CELL_BEHAVIOR_DESERT_SAND ground.
  HAZARD_COUNT_ON_CELLS = 0,
  /// One each frame the field sequence runs; no code in the game sets it.
  HAZARD_COUNT_FRAMES = 1
};

/// gMainRoutineState.hazardDamage's bit 8: its low byte is a percentage of
/// maximum HP rather than HP.
#define HAZARD_DAMAGE_PERCENT 0x100

/// gMainRoutineState.battleType: how the next battle starts (Battle_SetType);
/// it is set back to BATTLE_TYPE_NORMAL as the battle ends.
enum BattleType {
  BATTLE_TYPE_NORMAL = 0,
  /// A battle the party cannot run from: a Mimic's
  /// (field/field_sequence_triggers.c) and the travelers' fight in Vale
  /// (ValePlaza_FightTravelers, scene/vale_plaza.c).
  BATTLE_TYPE_NO_ESCAPE = 2,
  /// The battles an event starts.
  BATTLE_TYPE_SCRIPTED = 3,
  BATTLE_TYPE_LINK = 4, ///< a link battle (Battle_Run, scene/battle_arena.c)
  /// The debug battle menu's mark (DebugBattle_RunMenu), which no code tests
  /// by value; it still lets the party run.
  BATTLE_TYPE_DEBUG = 5
};

/// The most coins the party carries, and the most shop credit.
#define COIN_MAX 999999

/// The most Game Tickets counted in PartyHoldings.shopGifts, the last of the
/// Game Ticket's credit costs that is used.
#define SHOP_GIFTS_MAX 28

/// What the party holds apart from its items: its summons, coins and Djinn,
/// and what Tolbi's shops and games keep for it. Djinn_GetRecoveryTable
/// returns it as the party's struct DjinnRecoveryTable, and Lucky Wheels
/// writes the prizes it wins through it.
struct PartyHoldings {
  /// The summons the party can call, bit n for summon n: the
  /// DjinnRecoveryTable's summonMask.
  u32 summonMask;
  s32 coins; ///< 0 to COIN_MAX (Party_AdjustCoins)
  /// The party's Djinn recovery list (struct DjinnRecoveryList), read through
  /// Djinn_GetRecoveryTable.
  u8 djinnRecovery[260];
  s32 shopCredit; ///< spent on a Game Ticket
  s8 shopGifts;   ///< indexes the Game Ticket's credit cost
  /// The spring game's prize weights, three prizes to each of 4, 8, 16, 32
  /// and 64 on a new game.
  s8 prizeWeights[15];
  /// Lucky Wheels: 0 to 7 the prize categories won, ended by -1 (-2 once
  /// awarded); 8 on the last variant given of each category.
  s8 wonPrizes[16];
};

/// The main routine's state: the transition data the main routine reads between
/// sequences (the scene and entrance it runs next, sequenceResult, the return
/// point, the after-battle and defeat destinations and the restart point), and
/// what every sequence shares (the party, coins, Djinn, play time and options).
struct MainRoutineState {
  /// BuildStamp_GetTime's value when the game was last saved; its low
  /// half goes into the file's summary.
  u32 buildStamp;
  u32 playTime; ///< frames, copied from gPlayTimeFrames when saving
  u8 unused[4];
  struct PartyHoldings holdings;
  u8 unusedAfterPrizes[132];
  /// What the main routine runs next: a scene (a SceneId_ row of gScenes),
  /// which means the field sequence running that scene, or a SEQUENCE_ value
  /// above the table's rows for another sequence.
  s16 next;
  /// The destination's entrance; in SEQUENCE_BATTLE, the formation to fight.
  s16 entrance;
  /// The restart point, usually the last Sanctum: read after a defeat with no
  /// defeat destination, and by the File Screen.
  s16 restartScene;
  s16 restartEntrance; ///< the restart point's entrance
  /// The return point: the scene that was running when a battle or minigame
  /// started, where Scene_ReturnFromSequence returns unless an after-battle
  /// destination is set.
  s16 returnScene;
  s16 returnEntrance; ///< the return point's entrance
  s16 area;           ///< the current scene's area; a new one resets its flags
  /// The after-battle destination, -1 for none: read by
  /// Scene_ReturnFromSequence when the party was not defeated, before the
  /// return point (a minigame's return reads it too), and cleared as each
  /// scene starts.
  s16 afterBattleScene;
  s16 afterBattleEntrance; ///< its entrance; -1 for none
  /// The defeat destination, -1 for none: read by Scene_ReturnFromSequence
  /// after a defeat, before the restart point.
  s16 defeatScene;
  s16 defeatEntrance;   ///< its entrance; -1 for none
  s16 battleBackground; ///< the battle background chosen for the scene or
                        ///< battle environment
  u8 unusedAfterBackground[2];
  /// The scene's map (Scene.mapIndex); the World Map's is MAP_INDEX_WORLD.
  s16 mapId;
  FIXED x;           ///< 16.16 fixed-point pixels
  FIXED y;           ///< 16.16 fixed-point pixels
  FIXED z;           ///< 16.16 fixed-point pixels
  u32 heading;       ///< 1/65536 turns
  u16 layer;         ///< the leader's layer, kept with its position
  s16 battleMusic;   ///< the battle music chosen for the encounter
  s16 sceneMusic;    ///< the current scene's field music
  u8 movementMode;   ///< 1 while the leader is climbing, 0 otherwise
  s32 leaderActorId; ///< the leader: its unit and its actor
  u8 party[8];       ///< the party in order
  u8 unusedAfterParty[5];
  u8 windowHue;        ///< the "Window color" setting, 0 to 23
  u8 windowBrightness; ///< the "Window brightness" setting, 0 to 14
  u8 unusedAfterBrightness[3];
  /// The voice sound option: 1, as on a new game, plays each speaker's voice
  /// sound as their message types (Speaker_GetVoice).
  u8 speech;
  u8 unusedAfterSpeech; ///< 1 on a new game; nothing reads it
  u8 messageSpeed;      ///< 0 to 2; 1 on a new game
  u8 unusedAfterMessageSpeed[2];
  /// Set on saving at the end of the game, marking the file cleared: its data
  /// can be sent. Copied to the file's summary as sendFlag.
  u8 sendFlag;
  /// The buttons for each field command, as KEY_ masks; read with gKeysPressed
  /// except dashKeys, held.
  ///@{
  u16 aKeys; ///< examine; KEY_A
  u16 bKeys;
  u16 selectKeys; ///< the menu; KEY_SELECT
  u16 startKeys;
  u16 firstShortcutKeys;
  u16 secondShortcutKeys;
  u16 dashKeys;
  ///@}
  s16 returnMusic; ///< music playing when the field left; -1 for none
  /// The shortcuts' assignments. bits: 15..14 kind (0 Psynergy, 1 an item) |
  /// 13..0 which one.
  ///@{
  u16 firstShortcut;
  u16 secondShortcut;
  ///@}
  u8 perspectiveMap; ///< set to 1 on the world map
  u8 unusedAfterPerspective[5];
  u8 idleSleepEnabled; ///< the option, saved from gIdleSleepEnabled
  u8 battleType;       ///< an enum BattleType
  /// Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches
  /// hazardLimit the party takes hazardDamage and the count starts again.
  ///@{
  s16 hazardLimit; ///< pixels walked on hazard cells, or frames, by hazardMode
  s16 hazardMode;  ///< an enum HazardMode
  /// bits: 8 the amount is a percentage of maximum HP | 7..0 the amount
  s16 hazardDamage;
  s16 hazardSteps; ///< reset on entering a new area
  ///@}
  /// The Djinni battle pending (DJINNI_BATTLE_EVENT_FIRST plus its number,
  /// flags in bits 15 to 11), resolved on returning to the field.
  s16 pendingDjinni;
  s16 sequenceResult; ///< the last battle's result; -1 for a defeat
  s32 encounterSteps; ///< the encounter count, reset with a new area
  u16 pendingItem;    ///< given on returning to the field if the party has room
  s16 sceneType;      ///< the current scene's variant from its row in gScenes
  /// Where Retreat goes: the scene and entrance the party came into the area
  /// by.
  ///@{
  s16 retreatScene;
  s16 retreatEntrance;
  ///@}
  FIXED avoidDistance; ///< distance left of Avoid, 16.16 pixels; 0 when off
  s8 avoidByItem;      ///< Avoid came from an item (the Sacred Feather)
  /// Halt: the stopped actor's sprite colour mode, which Halt flickers and
  /// puts back when the actor resumes.
  s8 pausedColorMode;
  s16 haltedActor; ///< Halt: the stopped actor; -1 for none
  s16 cloaked;     ///< 1 under Cloak, which prevents battles
  /// Halt: the stopped actor's per-frame update, put back when it resumes.
  u32 pausedUpdate;
  u8 unusedAfterHalt[80];
  /// The Battle Arena's tallies: 4 the total wins and 7 the consecutive wins it
  /// reports, 3 cleared on choosing a monster battle; the others not named by
  /// the game.
  u16 linkTallies[8];
  u8 unusedAfterTallies[4];
  u32 startStamp; ///< BuildStamp_GetTime's value at a new game
  u8 unusedAfterStartStamp[4];
};

STATIC_ASSERT(offsetof(struct MainRoutineState, buildStamp) == 0);
STATIC_ASSERT(offsetof(struct MainRoutineState, playTime) == 4);
STATIC_ASSERT(offsetof(struct MainRoutineState, idleSleepEnabled) == 554);
STATIC_ASSERT(offsetof(struct MainRoutineState, holdings.coins) == 16);
STATIC_ASSERT(offsetof(struct MainRoutineState, holdings.shopCredit) == 280);
STATIC_ASSERT(offsetof(struct MainRoutineState, holdings.shopGifts) == 284);
STATIC_ASSERT(sizeof(struct MainRoutineState) == 704);
STATIC_ASSERT(offsetof(struct MainRoutineState, holdings.wonPrizes) == 300);
STATIC_ASSERT(offsetof(struct MainRoutineState, area) == 460);
STATIC_ASSERT(offsetof(struct MainRoutineState, dashKeys) == 540);
STATIC_ASSERT(offsetof(struct MainRoutineState, avoidDistance) == 580);
STATIC_ASSERT(offsetof(struct MainRoutineState, entrance) == 450);
STATIC_ASSERT(offsetof(struct MainRoutineState, battleBackground) == 470);
STATIC_ASSERT(offsetof(struct MainRoutineState, x) == 476);
STATIC_ASSERT(offsetof(struct MainRoutineState, movementMode) == 498);
STATIC_ASSERT(offsetof(struct MainRoutineState, leaderActorId) == 500);
STATIC_ASSERT(offsetof(struct MainRoutineState, party) == 504);
STATIC_ASSERT(offsetof(struct MainRoutineState, speech) == 522);
STATIC_ASSERT(offsetof(struct MainRoutineState, messageSpeed) == 524);
STATIC_ASSERT(offsetof(struct MainRoutineState, aKeys) == 528);
STATIC_ASSERT(offsetof(struct MainRoutineState, firstShortcutKeys) == 536);
STATIC_ASSERT(offsetof(struct MainRoutineState, secondShortcutKeys) == 538);
STATIC_ASSERT(offsetof(struct MainRoutineState, firstShortcut) == 544);
STATIC_ASSERT(offsetof(struct MainRoutineState, retreatEntrance) == 578);
STATIC_ASSERT(offsetof(struct MainRoutineState, cloaked) == 588);
STATIC_ASSERT(offsetof(struct MainRoutineState, linkTallies) == 676);
STATIC_ASSERT(offsetof(struct MainRoutineState, startStamp) == 696);

/// The game's state (system/ewram.s).
extern struct MainRoutineState gMainRoutineState;
/// The eight characters' units, which follow it (Unit_Get).
extern struct Unit gCharacterUnits[8];

/// Says what kind of battle comes next (a BATTLE_TYPE_), as an event or a
/// battle trigger does before starting one.
#define Battle_SetType(mode)                                                   \
  do {                                                                         \
    gMainRoutineState.battleType = (mode);                                     \
  } while (0)

#endif
