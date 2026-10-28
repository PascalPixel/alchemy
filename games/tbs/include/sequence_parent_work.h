#ifndef ALCHEMY_SEQUENCE_PARENT_WORK_H
#define ALCHEMY_SEQUENCE_PARENT_WORK_H

/// @file
/// The sequence parent's work: the transition data the sequence parent reads
/// between sequences and the state every sequence shares, which a save file
/// keeps.

#include "types.h"
#include "static_assert.h"
#include "battle_unit.h"

/// A scene's type (Scene.sceneType, copied to SequenceParentWork.sceneType).
enum SceneType {
  SCENE_TYPE_OTHER = 1, ///< every other scene: dungeons, the world map
  /// Towns and their interiors, where walking restores no PP, recovers no
  /// Djinn and does no poison damage, Retreat is blocked, Avoid does not wear
  /// off and FLAG_IN_TOWN is set.
  SCENE_TYPE_TOWN = 2
};

/// gSequenceParentWork.hazardMode: what advances the hazard count
/// (Field_UpdateWalking).
enum HazardMode {
  /// The distance the leader walks on CELL_BEHAVIOR_DESERT_SAND ground.
  HAZARD_COUNT_ON_CELLS = 0,
  /// One each frame the field sequence runs; no ☀️ code sets it.
  HAZARD_COUNT_FRAMES = 1
};

/// gSequenceParentWork.hazardDamage's bit 8: its low byte is a percentage of
/// maximum HP rather than HP.
#define HAZARD_DAMAGE_PERCENT 0x100

/// gSequenceParentWork.battleType: how the next battle starts (Battle_SetType);
/// it is set back to BATTLE_TYPE_NORMAL as the battle ends. Any start but that
/// one stops the opening's first-strike and surprise rolls (Battle_Run).
enum BattleType {
  BATTLE_TYPE_NORMAL = 0,
  /// A battle the party cannot run from: a Mimic's
  /// (overworld/overworld/triggers.c) and the travelers' fight in Vale
  /// (ValePlaza_FightTravelers, scene/vale_plaza.c).
  BATTLE_TYPE_NO_ESCAPE = 2,
  /// The battles an event starts. Here and in a link battle the command menu
  /// itself refuses to run, with MsgNoTimeToRun ("This is no time to be
  /// running away!"), as it does once the party is challenged
  /// (Message_ShowLocalizedAndWait, window/screens/link_timer.c).
  BATTLE_TYPE_SCRIPTED = 3,
  BATTLE_TYPE_LINK = 4, ///< a link battle (Battle_Run, menu/battle_arena.c)
  /// The debug battle menu's mark (DebugBattle_RunMenu), which no code tests
  /// by value; it still lets the party run.
  BATTLE_TYPE_DEBUG = 5
};

/// The sequence parent's work: the transition data the sequence parent reads
/// between sequences (the scene and entrance it runs next, sequenceResult,
/// the return point, the after-battle and defeat destinations and the restart
/// point), and what every sequence shares (the party, coins, Djinn, play time
/// and options). A save file keeps it.
///
/// It is a fixed struct, never freed and in no heap slot: the one
/// gSequenceParentWork in EWRAM, right after the 0x200 bytes of game
/// flags. Saving writes it whole in a file's first part, with the flags
/// before it, and a file's checksum covers the 968 words from gGameFlagBytes
/// to the end of this struct (Save_BuildFileSummary, Save_WriteCurrentFile). A
/// new game sets its defaults (SequenceParentWork_InitializeDefaults), and the
/// field, battle, menus and scenes read and write it directly.
struct SequenceParentWork {
  /// BuildStamp_GetTime's value when the game was last saved; its low
  /// half goes into the file's summary.
  u32 buildStamp;
  u32 playTime; ///< frames, copied from gPlayTimeFrames when saving
  u8 unused[4]; ///< no code names it
  /// The summons the party can call, bit n for summon n: the first word of
  /// the party's struct DjinnRecoveryTable (BattleSummonState.availableMask),
  /// which Djinn_GetRecoveryTable returns from here, its next word running
  /// over coins.
  u32 summonMask;
  s32 coins; ///< 0 to 999999 (Party_AdjustCoins)
  /// The party's Djinn recovery list (struct DjinnRecoveryList), read through
  /// Djinn_GetRecoveryTable; not named by TBS code.
  u8 djinnRecovery[0x104];
  s32 shopCredit; ///< spent on a Game Ticket; not written by name in TBS
  s8 shopGifts;   ///< indexes the Game Ticket's credit cost
  /// Lucky Wheels' prize weights, three prizes to each of 4, 8, 16, 32
  /// and 64 on a new game; not read by name in TBS.
  s8 prizeWeights[0x0f];
  /// Lucky Wheels: 0 to 7 the prize categories won, ended by -1 (-2 once
  /// awarded); 8 on the last variant given of each category.
  s8 wonPrizes[0x10];
  u8 unusedAfterPrizes[0x84]; ///< no code names it
  /// The destination: the sequence the sequence parent runs next, a
  /// SceneId_ row of gScenes or a SEQUENCE_ value above the table's rows.
  /// During a scene it is the scene running. An exit-table lookup overwrites
  /// it, and EXIT_KEEP_DESTINATION keeps it.
  s16 sceneId;
  /// The destination's entrance; in SEQUENCE_BATTLE, the formation to fight.
  /// Not a union with a formation member: alchemy-gcc pads a union to a word,
  /// which would move every member after it.
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
  s16 defeatEntrance;       ///< its entrance; -1 for none
  s16 special;              ///< the battle background chosen for the scene or
                            ///< battle environment
  u8 unusedAfterSpecial[2]; ///< no code names it
  /// The scene's map (Scene.mapIndex); the World Map's is MAP_INDEX_WORLD.
  s16 mapId;
  s32 x;                   ///< 16.16 fixed-point pixels
  s32 y;                   ///< 16.16 fixed-point pixels
  s32 z;                   ///< 16.16 fixed-point pixels
  u32 heading;             ///< 1/65536 turns
  u16 layer;               ///< the leader's layer, kept with its position
  s16 battleMusic;         ///< the battle music chosen for the encounter
  s16 sceneMusic;          ///< the current scene's field music
  u8 movementMode;         ///< 1 while the leader is climbing, 0 otherwise
  u8 paddingAfterMovement; ///< up to the word leaderActorId starts on
  s32 leaderActorId;       ///< the leader: its unit and its actor
  u8 party[8];             ///< the party in order
  u8 unusedAfterOwners[5]; ///< no code names it
  u8 windowColor[2];       ///< window hue (0 to 23), brightness (0 to 14)
  u8 unusedAfterGlow[3];   ///< no code names it
  /// The voice sound option: 1, as on a new game, plays each speaker's voice
  /// sound as their message types (Speaker_GetVoice).
  u8 speech;
  u8 unusedAfterSpeech;          ///< 1 on a new game; nothing reads it
  u8 messageSpeed;               ///< 0 to 2; 1 on a new game
  u8 unusedAfterMessageSpeed[2]; ///< no code names it
  /// Set on saving at the end of the game, marking the file cleared: its data
  /// can be sent. Copied to the file's summary as sendFlag.
  u8 sendFlag;
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
  s16 returnMusic; ///< music playing when the field left; -1 for none
  /// The shortcuts' assignments. bits: 15..14 kind (0 Psynergy, 1 an item) |
  /// 13..0 which one.
  ///@{
  u16 firstShortcut;
  u16 secondShortcut;
  ///@}
  u8 perspectiveMap; ///< set to 1 on the world map; not read by name in TBS
  u8 unusedAfterPerspective[5]; ///< no code names it
  u8 idleSleepEnabled;          ///< the option, saved from gIdleSleepEnabled
  u8 battleType;                ///< an enum BattleType
  /// Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches
  /// hazardLimit the party takes hazardDamage and the count starts again. A
  /// fresh entry into a scene, not a return from battle, resets them to 1 HP
  /// every 16 pixels walked on it; the desert sets 25 percent every 600
  /// (FieldSequence_Run, LamakanDesert_Initialize).
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
  s16 retreatScene;   ///< where Retreat goes; the area's first
                      ///< entrance
  s16 retreatEntrance;     ///< where Retreat goes; the area's first
                           ///< entrance
  s32 avoidDistance;       ///< distance left of Avoid, 16.16 pixels; 0 when off
  s8 avoidByItem;          ///< Avoid came from an item (the Sacred Feather)
  s8 pausedPose;           ///< Halt: the stopped actor's first animation param
  s16 haltedActor;         ///< Halt: the stopped actor; -1 for none
  s16 cloaked;             ///< 1 under Cloak, which prevents battles
  u8 paddingAfterCloak[2]; ///< up to the word pausedUpdate starts on
  u32 pausedUpdate; ///< Halt: the stopped actor's update; not read by name
  u8 unusedAfterHalt[0x50]; ///< no code names it
  /// The Battle Arena's tallies: 4 the total wins and 7 the consecutive wins it
  /// reports, 3 cleared on choosing a monster battle; the others not named by
  /// TBS code.
  u16 linkTallies[8];
  u8 unusedAfterTallies[4];    ///< no code names it
  u32 startStamp;              ///< BuildStamp_GetTime's value at a new game
  u8 unusedAfterStartStamp[4]; ///< no code names it
  struct Unit units[8];        ///< the characters
};

STATIC_ASSERT(offsetof(struct SequenceParentWork, buildStamp) == 0);
STATIC_ASSERT(offsetof(struct SequenceParentWork, playTime) == 4);
STATIC_ASSERT(offsetof(struct SequenceParentWork, idleSleepEnabled) == 0x22a);
STATIC_ASSERT(offsetof(struct SequenceParentWork, coins) == 0x10);
STATIC_ASSERT(offsetof(struct SequenceParentWork, shopCredit) == 0x118);
STATIC_ASSERT(offsetof(struct SequenceParentWork, shopGifts) == 0x11c);
STATIC_ASSERT(offsetof(struct SequenceParentWork, units) == 0x2c0);
STATIC_ASSERT(sizeof(struct SequenceParentWork) == 0xd20);
STATIC_ASSERT(offsetof(struct SequenceParentWork, wonPrizes) == 0x12c);
STATIC_ASSERT(offsetof(struct SequenceParentWork, area) == 0x1cc);
STATIC_ASSERT(offsetof(struct SequenceParentWork, dashKeys) == 0x21c);
STATIC_ASSERT(offsetof(struct SequenceParentWork, avoidDistance) == 0x244);
STATIC_ASSERT(offsetof(struct SequenceParentWork, entrance) == 0x1c2);
STATIC_ASSERT(offsetof(struct SequenceParentWork, special) == 0x1d6);
STATIC_ASSERT(offsetof(struct SequenceParentWork, x) == 0x1dc);
STATIC_ASSERT(offsetof(struct SequenceParentWork, movementMode) == 0x1f2);
STATIC_ASSERT(offsetof(struct SequenceParentWork, leaderActorId) == 0x1f4);
STATIC_ASSERT(offsetof(struct SequenceParentWork, party) == 0x1f8);
STATIC_ASSERT(offsetof(struct SequenceParentWork, speech) == 0x20a);
STATIC_ASSERT(offsetof(struct SequenceParentWork, messageSpeed) == 0x20c);
STATIC_ASSERT(offsetof(struct SequenceParentWork, aKeys) == 0x210);
STATIC_ASSERT(offsetof(struct SequenceParentWork, firstShortcutKeys) == 0x218);
STATIC_ASSERT(offsetof(struct SequenceParentWork, secondShortcutKeys) == 0x21a);
STATIC_ASSERT(offsetof(struct SequenceParentWork, firstShortcut) == 0x220);
STATIC_ASSERT(offsetof(struct SequenceParentWork, retreatEntrance) == 0x242);
STATIC_ASSERT(offsetof(struct SequenceParentWork, cloaked) == 0x24c);
STATIC_ASSERT(offsetof(struct SequenceParentWork, linkTallies) == 0x2a4);
STATIC_ASSERT(offsetof(struct SequenceParentWork, startStamp) == 0x2b8);

#ifndef OWN_gSequenceParentWork
/// The game's state (cartridge/ewram.s).
extern struct SequenceParentWork gSequenceParentWork;
#endif

/// Sets the battle type.
// A statement macro (34 sites): written as one, its store stays in
// place at every use; as a plain assignment, tbs-en differs.
#define Battle_SetType(mode)                                                   \
  do {                                                                         \
    gSequenceParentWork.battleType = (mode);                                   \
  } while (0)

#endif
