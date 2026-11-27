#ifndef ALCHEMY_MAIN_ROUTINE_STATE_H
#define ALCHEMY_MAIN_ROUTINE_STATE_H

// The values MainRoutineStateWork's fields hold. The structure, the game's state a save file keeps,
// is the system library's (system/definitions.h), as the variable is.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "system.h"

// A scene's type (SCENE.sceneType, copied to MAIN_ROUTINE_STATE.sceneType).
typedef enum SCENE_TYPE
{
    SCENE_TYPE_OTHER = 1, // every other scene: dungeons, the world map
    // Towns and their rooms, and the prologue's Venus Lighthouse scenes: FLAG_IN_TOWN is set there
    // (gmArrive).
    SCENE_TYPE_TOWN  = 2
} SCENE_TYPE;

// MainRoutineStateWork.battleType: how the next battle starts; it is set back to BATTLE_TYPE_NORMAL
// as the battle ends.
typedef enum BATTLE_TYPE
{
    BATTLE_TYPE_NORMAL    = 0,
    // A battle the party cannot run from: a Mimic's, a hidden Mad Plant's and the bosses'.
    BATTLE_TYPE_NO_ESCAPE = 2,
    BATTLE_TYPE_LINK      = 4, // a link battle, from the lobby
    BATTLE_TYPE_DEBUG     = 5, // the debug battle menu's mark
    // A battle the party is meant to lose: it is caught off guard and every hit downs its target,
    // as when Karst and Agatio fight Isaac and Ivan.
    BATTLE_TYPE_LOST      = 6
} BATTLE_TYPE;

// btRun's results in MainRoutineStateWork.modeResult besides the number of enemies defeated.
#define BATTLE_OUTCOME_LOST -1
#define BATTLE_OUTCOME_FLED 999

// The most coins the party carries, and the most shop credit.
#define COIN_MAX            999999

// MainRoutineStateWork.hazardMode: what advances the hazard count (fdUpdateWalking).
typedef enum HAZARD_MODE
{
    // The distance the leader walks on MAP_METATILE_HAZARD ground.
    HAZARD_COUNT_ON_METATILES = 0,
    // One each tick the field mode runs; no code in the game sets it.
    HAZARD_COUNT_TICKS        = 1
} HAZARD_MODE;

// MainRoutineStateWork.hazardDamage's bit 8: its low byte is a percentage of maximum HP rather than
// HP.
#define HAZARD_DAMAGE_PERCENT      0x100

// The most Game Tickets counted in MAIN_ROUTINE_STATE.shopGifts, the last of
// ShopGameTicketCreditCosts' prices that is used.
#define SHOP_GIFTS_MAX             28

// The first pending Djinni battle script: DJINNI_BATTLE_SCRIPT_FIRST plus the Djinni's number
// (fdRequestDjinnBattle).
#define DJINNI_BATTLE_SCRIPT_FIRST 300

// MainRoutineStateWork.difficulty: the level chosen on starting again from a cleared game.
typedef enum DIFFICULTY
{
    DIFFICULTY_NORMAL = 0, // "Play on Normal level? (Default difficulty)"
    // "Play on EASY level? (Characters start at Lost Age Clear Data levels)"
    DIFFICULTY_EASY   = 1,
    DIFFICULTY_HARD   = 2 // "Play on HARD level? (Monsters are stronger)"
} DIFFICULTY;

// What MAIN_ROUTINE_STATE.wheelsPrizes holds in place of a first prize, as Lucky Wheels and its
// attendant leave it: no icon matched (also the list's end), or none to hand out, the player having
// quit while betting or the prizes having been given.
typedef enum LUCKY_WHEELS_PRIZES_MARK
{
    WHEELS_PRIZES_END              = -1,
    WHEELS_PRIZES_NONE_TO_HAND_OUT = -2
} LUCKY_WHEELS_PRIZES_MARK;

// Teleport's steps (MAIN_ROUTINE_STATE.teleportState): a place was chosen and the scene it lands in
// is loading, then the party arrives in it.
typedef enum TELEPORT_STATE
{
    TELEPORT_NONE,
    TELEPORT_CHOSEN,
    TELEPORT_ARRIVING
} TELEPORT_STATE;

// MAIN_ROUTINE_STATE.linkTallies, as the Link Lobby keeps them.
typedef enum LINK_TALLY
{
    LINK_TALLY_MONSTER_WINS        = 0,
    LINK_TALLY_MONSTER_LOSSES      = 1,
    LINK_TALLY_MONSTER_BEST_STREAK = 2, // the most won in a row
    LINK_TALLY_MONSTER_STREAK      = 3, // those won in the current series
    LINK_TALLY_LINKED_WINS         = 4,
    LINK_TALLY_LINKED_LOSSES       = 5,
    // The most linked battles won in a row, on the wall board.
    LINK_TALLY_LINKED_BEST_STREAK  = 6,
    LINK_TALLY_LINKED_STREAK       = 7 // those won in a row now
} LINK_TALLY;

// Sets how the next battle starts (an BATTLE_TYPE).
#define SET_BATTLE_TYPE(mode)                                                                      \
    do                                                                                             \
    {                                                                                              \
        MainRoutineStateWork.battleType = (mode);                                                  \
    } while (0)

#endif
