#ifndef ALCHEMY_MAIN_ROUTINE_STATE_H
#define ALCHEMY_MAIN_ROUTINE_STATE_H

// The main routine's state: the transition data the main routine reads between modes and the state
// every mode shares, which a save file keeps.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle_unit.h"

// A scene's type (SCENE.sceneType, copied to MAIN_ROUTINE_STATE.sceneType).
typedef enum SCENE_TYPE
{
    SCENE_TYPE_OTHER = 1, // every other scene: dungeons, the world map
    // Towns and their rooms, where walking restores no PP, recovers no Djinn and does no poison
    // damage, Retreat is blocked, Avoid does not wear off and FLAG_IN_TOWN is set.
    SCENE_TYPE_TOWN  = 2
} SCENE_TYPE;

// MainRoutineStateWork.hazardMode: what advances the hazard count (fdUpdateWalking).
typedef enum HAZARD_MODE
{
    // The distance the leader walks on METATILE_BEHAVIOR_DESERT_SAND ground.
    HAZARD_COUNT_ON_METATILES = 0,
    // One each tick the field mode runs; no code in the game sets it.
    HAZARD_COUNT_TICKS        = 1
} HAZARD_MODE;

// MainRoutineStateWork.hazardDamage's bit 8: its low byte is a percentage of maximum HP rather than
// HP.
#define HAZARD_DAMAGE_PERCENT 0x100

// MainRoutineStateWork.battleType: how the next battle starts (SET_BATTLE_TYPE); it is set back to
// BATTLE_TYPE_NORMAL as the battle ends.
typedef enum BATTLE_TYPE
{
    BATTLE_TYPE_NORMAL    = 0,
    // A battle the party cannot run from: a Mimic's (field/field_mode_cues.c) and the travelers'
    // fight in Vale (scValePlazaFightTravelers, scene/vale_plaza.c).
    BATTLE_TYPE_NO_ESCAPE = 2,
    // The battles a script starts.
    BATTLE_TYPE_SCRIPTED  = 3,
    BATTLE_TYPE_LINK      = 4, // a link battle (btRun, scene/battle_arena.c)
    // The debug battle menu's mark (btDebugBattleRunMenu), which no code tests by value; it still
    // lets the party run.
    BATTLE_TYPE_DEBUG     = 5
} BATTLE_TYPE;

// The most coins the party carries, and the most shop credit.
#define COIN_MAX       999999

// The most Game Tickets counted in PARTY_HOLDINGS.shopGifts, the last of the Game Ticket's credit
// costs that is used.
#define SHOP_GIFTS_MAX 28

// What the party holds apart from its items: its summons, coins and Djinn, and what Tolbi's shops
// and games keep for it. gmDjinnGetRecoveryTable returns it as the party's DJINN_RECOVERY_TABLE,
// and Lucky Wheels writes the prizes it wins through it.
typedef struct PARTY_HOLDINGS
{
    // The summons the party can call, bit n for summon n: the DJINN_RECOVERY_TABLE's summonMask.
    Uint32 summonMask;
    Uint32 coins; // 0 to COIN_MAX (gmPartyAdjustCoins)
    // The party's Djinn recovery list (DJINN_RECOVERY_LIST), read through gmDjinnGetRecoveryTable.
    Uint8  djinnRecovery[260];
    Sint32 shopCredit; // spent on a Game Ticket
    Sint8  shopGifts;  // indexes the Game Ticket's credit cost
    // The spring game's prize weights, three prizes to each of 4, 8, 16, 32 and 64 on a new game.
    Sint8  prizeWeights[15];
    // Lucky Wheels: 0 to 7 the prize categories won, ended by -1 (-2 once awarded); 8 on the last
    // variant given of each category.
    Sint8  wonPrizes[16];
} PARTY_HOLDINGS;

// The main routine's state: the transition data the main routine reads between modes (the scene and
// entrance it runs next, modeResult, the return point, the after-battle and defeat destinations and
// the restart point), and what every mode shares (the party, coins, Djinn, play time and options).
typedef struct MAIN_ROUTINE_STATE
{
    // gmBuildStampGetTime's value when the game was last saved; its low half goes into the file's
    // summary.
    Uint32         buildStamp;
    Uint32         playTime; // ticks, copied from PlayTimeTicks when saving
    Uint8          unused[4];
    PARTY_HOLDINGS holdings;
    Uint8          unusedAfterPrizes[132];
    // What the main routine runs next: a scene (a SceneId row of ScenesWork), which means the field
    // mode running that scene, or a MODE_ value above the table's rows for another mode.
    Sint16         next;
    // The destination's entrance; in MODE_BATTLE, the formation to fight.
    Sint16         entrance;
    // The restart point, usually the last Sanctum: read after a defeat with no defeat destination,
    // and by the File Screen.
    Sint16         restartScene;
    Sint16         restartEntrance; // the restart point's entrance
    // The return point: the scene that was running when a battle or minigame started, where
    // gmReturnFromMode returns unless an after-battle destination is set.
    Sint16         returnScene;
    Sint16         returnEntrance; // the return point's entrance
    Sint16         area;           // the current scene's area; a new one resets its flags
    // The after-battle destination, -1 for none: read by gmReturnFromMode when the party was not
    // defeated, before the return point (a minigame's return reads it too), and cleared as each
    // scene starts.
    Sint16         afterBattleScene;
    Sint16         afterBattleEntrance; // its entrance; -1 for none
    // The defeat destination, -1 for none: read by gmReturnFromMode after a defeat, before the
    // restart point.
    Sint16         defeatScene;
    Sint16         defeatEntrance; // its entrance; -1 for none
    // the battle background chosen for the scene or battle environment
    Sint16         battleBg;
    Uint8          unusedAfterBg[2];
    // The scene's map (SCENE.mapIndex); the World Map's is MAP_INDEX_WORLD.
    Sint16         mapId;
    FIXED          x;                // 16.16 fixed-point pixels
    FIXED          y;                // 16.16 fixed-point pixels
    FIXED          z;                // 16.16 fixed-point pixels
    Uint32         heading;          // 1/65536 turns
    Uint16         layer;            // the leader's layer, kept with its position
    Sint16         battleMusic;      // the battle music chosen for the encounter
    Sint16         sceneMusic;       // the current scene's field music
    Uint8          movementMode;     // 1 while the leader is climbing, 0 otherwise
    Sint32         leaderActorId;    // the leader: its unit and its actor
    Uint8          party[8];         // the party in order
    Uint8          unusedAfterParty[5];
    Uint8          windowHue;        // the "Window color" setting, 0 to 23
    Uint8          windowBrightness; // the "Window brightness" setting, 0 to 14
    Uint8          unusedAfterBrightness[3];
    // The voice sound option: 1, as on a new game, plays each speaker's voice sound as their
    // message types (fdSpeakerGetVoice).
    Uint8          speech;
    Uint8          unusedAfterSpeech; // 1 on a new game; nothing reads it
    Uint8          msgSpeed;          // 0 to 2; 1 on a new game
    Uint8          unusedAfterMsgSpeed[2];
    // Set on saving at the end of the game, marking the file cleared: its data can be sent. Copied
    // to the file's summary as sendFlag.
    Uint8          sendFlag;
    // The buttons for each field command, as KEY_ masks; read with PerPush except dashKeys, held.
    Uint16         aKeys;      // examine; KEY_A
    Uint16         bKeys;
    Uint16         selectKeys; // the menu; KEY_SELECT
    Uint16         startKeys;
    Uint16         firstShortcutKeys;
    Uint16         secondShortcutKeys;
    Uint16         dashKeys;
    Sint16         returnMusic; // music playing when the field left; -1 for none
    // The shortcuts' assignments. bits: 15..14 kind (0 Psynergy, 1 an item) | 13..0 which one.
    Uint16         firstShortcut;
    Uint16         secondShortcut;
    Uint8          perspectiveMap;   // set to 1 on the world map
    Uint8          unusedAfterPerspective[5];
    Uint8          idleSleepEnabled; // the option, saved from IdleSleepEnabled
    Uint8          battleType;       // an BATTLE_TYPE
    // Hazardous ground (the Lamakan Desert's heat): when hazardSteps reaches hazardLimit the party
    // takes hazardDamage and the count starts again. pixels walked on hazard metatiles, or ticks,
    // by hazardMode
    Sint16         hazardLimit;
    Sint16         hazardMode; // an HAZARD_MODE
    // bits: 8 the amount is a percentage of maximum HP | 7..0 the amount
    Sint16         hazardDamage;
    Sint16         hazardSteps; // reset on entering a new area
    // The Djinni battle pending (DJINNI_BATTLE_SCRIPT_FIRST plus its number, flags in bits 15 to
    // 11), resolved on returning to the field.
    Sint16         pendingDjinni;
    Sint16         modeResult;     // the last battle's result; -1 for a defeat
    Sint32         encounterSteps; // the encounter count, reset with a new area
    // given on returning to the field if the party has room
    Uint16         pendingItem;
    // the current scene's variant from its row in ScenesWork
    Sint16         sceneType;
    // Where Retreat goes: the scene and entrance the party came into the area by.
    Sint16         retreatScene;
    Sint16         retreatEntrance;
    FIXED          avoidDistance; // distance left of Avoid, 16.16 pixels; 0 when off
    Sint8          avoidByItem;   // Avoid came from an item (the Sacred Feather)
    // Halt: the stopped actor's sprite colour mode, which Halt flickers and puts back when the
    // actor resumes.
    Sint8          haltedColorMode;
    Sint16         haltedActor; // Halt: the stopped actor; -1 for none
    Sint16         cloaked;     // 1 under Cloak, which prevents battles
    // Halt: the stopped actor's per-tick update, put back when it resumes.
    Uint32         haltedUpdate;
    Uint8          unusedAfterHalt[80];
    // The Battle Arena's tallies: 4 the total wins and 7 the consecutive wins it reports, 3 cleared
    // on choosing a monster battle; the others not named by the game.
    Uint16         linkTallies[8];
    Uint8          unusedAfterTallies[4];
    Uint32         startStamp; // gmBuildStampGetTime's value at a new game
    Uint8          unusedAfterStartStamp[4];
} MAIN_ROUTINE_STATE;

STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, buildStamp) == 0);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, playTime) == 4);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, idleSleepEnabled) == 554);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.coins) == 16);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.shopCredit) == 280);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.shopGifts) == 284);
STATIC_ASSERT(sizeof(MAIN_ROUTINE_STATE) == 704);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, holdings.wonPrizes) == 300);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, area) == 460);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, dashKeys) == 540);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, avoidDistance) == 580);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, entrance) == 450);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, battleBg) == 470);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, x) == 476);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, movementMode) == 498);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, leaderActorId) == 500);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, party) == 504);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, speech) == 522);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, msgSpeed) == 524);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, aKeys) == 528);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, firstShortcutKeys) == 536);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, secondShortcutKeys) == 538);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, firstShortcut) == 544);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, retreatEntrance) == 578);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, cloaked) == 588);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, linkTallies) == 676);
STATIC_ASSERT(offsetof(MAIN_ROUTINE_STATE, startStamp) == 696);

// The game's state (system/ewram.s).
extern MAIN_ROUTINE_STATE MainRoutineStateWork;
// The eight characters' units, which follow it (gmUnitGet).
extern UNIT               CharacterUnits[8];

// Says what kind of battle comes next (a BATTLE_TYPE_), as a script or a battle cue does before
// starting one.
#define SET_BATTLE_TYPE(mode)                                                                      \
    do                                                                                             \
    {                                                                                              \
        MainRoutineStateWork.battleType = (mode);                                                  \
    } while (0)

#endif
