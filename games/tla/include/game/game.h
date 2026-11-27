#ifndef ALCHEMY_GAME_H
#define ALCHEMY_GAME_H

// The item and summon tables and the game module's functions.

#include "types.h"
#include "edition.h"
#include "battle/battle_summon.h"
#include "battle/battle_unit.h"
#include "item.h"
#include "character_ids.h"
#include "character.h"
#include "battle/battle_type.h"
#include "game/class_djinn.h"
#include "flags.h"
#include "system.h"
#include "game/game_functions.h"

struct ITEM_DEFINITION;
struct SUMMON_DEFINITION;

// What a Psynergy does when cast on the field (an ability's fieldPsynergy), each named for the
// abilities that have it in the English release. The field effect it starts keeps it as its
// animation.
typedef enum FIELD_PSY
{
    FIELD_PSYNERGY_MOVE      = 1,
    FIELD_PSYNERGY_MIND_READ = 2,
    FIELD_PSYNERGY_PLY       = 3,
    FIELD_PSYNERGY_FORCE     = 4,
    FIELD_PSYNERGY_DOUSE     = 5,
    FIELD_PSYNERGY_FROST     = 6,
    FIELD_PSYNERGY_LIFT      = 7,
    FIELD_PSYNERGY_REVEAL    = 8,
    FIELD_PSYNERGY_HALT      = 9,
    FIELD_PSYNERGY_CLOAK     = 10,
    FIELD_PSYNERGY_CARRY     = 11,
    FIELD_PSYNERGY_GROWTH    = 12,
    FIELD_PSYNERGY_CATCH     = 13,
    // Whirlwind and Gale.
    FIELD_PSYNERGY_WHIRLWIND = 14,
    FIELD_PSYNERGY_RETREAT   = 15,
    FIELD_PSYNERGY_AVOID     = 16,
    FIELD_PSYNERGY_LASH      = 17,
    FIELD_PSYNERGY_POUND     = 18,
    FIELD_PSYNERGY_TREMOR    = 19,
    FIELD_PSYNERGY_SCOOP     = 20,
    FIELD_PSYNERGY_BURST     = 21,
    FIELD_PSYNERGY_PARCH     = 22,
    FIELD_PSYNERGY_SAND      = 23,
    FIELD_PSYNERGY_HOVER     = 24,
    FIELD_PSYNERGY_CYCLONE   = 25,
    FIELD_PSYNERGY_BLAZE     = 26,
    FIELD_PSYNERGY_GRIND     = 27,
    // The two abilities whose names are question marks.
    FIELD_PSYNERGY_RING      = 28,
    FIELD_PSYNERGY_ARROW     = 29,
    FIELD_PSYNERGY_TELEPORT  = 30
} FIELD_PSY;

// A step of the current field Psynergy (fdPsynergyRunHandler).
typedef enum FIELD_PSYNERGY_HANDLER
{
    // Casts it with no target (fdPsynergyRun).
    FIELD_PSYNERGY_HANDLER_UNTARGETED = 0,
    // Casts it on its target, secondActorId (fdPsynergyDispatch).
    FIELD_PSYNERGY_HANDLER_ON_TARGET  = 1,
    FIELD_PSYNERGY_HANDLER_FINISH     = 2, // finishes it
    FIELD_PSYNERGY_HANDLER_EXTRA      = 3  // runs its kind's extra handler
} FIELD_PSYNERGY_HANDLER;

// The elemental power and resistance of one element level, in ElementPowerResistByLevel.
typedef struct ELEMENT_LEVEL_BONUS
{
    Uint16 power;      // base power, before the tenths of a level
    Uint16 resistance; // base resistance, likewise
} ELEMENT_LEVEL_BONUS;

extern const ITEM_DEFINITION   ItemDefinitionTable[];
extern const SUMMON_DEFINITION SummonDefinitionTable[];

struct BATTLE_ELEMENT_STATS;

// How many enemies EnemyDragonIds and EnemySeaCreatureIds list.
#define DRAGON_ENEMY_COUNT       47
#define SEA_CREATURE_ENEMY_COUNT 29
extern const Uint16             EnemyDragonIds[DRAGON_ENEMY_COUNT];
extern const Uint16             EnemySeaCreatureIds[SEA_CREATURE_ENEMY_COUNT];
extern const ABILITY_DEFINITION AbilityDefinitions[];
extern const DJINN_DEFINITION   DjinnDefinitionTable[];
extern const ENEMY_DEFINITION   EnemyDefinitions[];
// Counts the Djinn recovery down twenty times, setting every recovering Djinni of a party member
// with HP (btDjinnRecoveryCountDownTwenty).
void                            btDjinnRecoveryCountDownTwentyStub(void);
// Gives a Djinni to a unit.
Sint32                          gmDjinnGiveStub(Sint32 unitId, Sint32 element, Sint32 djinnIndex);
// Clears each button shortcut whose Psynergy can no longer be cast (fdPartyClearUnusableShortcuts).
void                            fdPartyClearUnusableShortcutsStub(void);
// Gives a unit one item, joining its first matching stack (up to INVENTORY_STACK_MAX) or taking the
// first free slot.
Sint32                          gmInventoryAddItemStub(Sint32 unitId, Sint32 itemId);
// Takes one item out of slot, closing up the inventory when it was the last.
Sint32                          gmInventoryRemoveStub(Sint32 owner, Sint32 slot);
void   gmUnitCalculateBaseElements(Sint32 unitId, BATTLE_ELEMENT_STATS *dest);
Sint32 gmClassDetermine(Sint32 character, Uint32 *djinn, Sint32 item);
Sint32 gmUnitGetElementLevels(Sint32 enemyId, const Uint8 *src, Sint32 *output);
Sint32 gmUnitRebuildPsynergyList(Sint32 owner);
// Decodes message into text with no length limit (mnMessageDecodeRawUnbounded).
void   mnMessageDecodeRawUnboundedStub(Sint32 msg, Uint16 *text);

// How many summons there are (summon_table.c), and the number that ends a list of them
// (gmSummonListLearned).
#define SUMMON_COUNT 29

// The summons by summon id, each named for its ability, in the order of SummonAbilities
// (summon_table.c): the elements' own four, The Broken Seal's twelve, then this game's thirteen.
typedef enum SUMMON_ID
{
    SUMMON_VENUS,
    SUMMON_MERCURY,
    SUMMON_MARS,
    SUMMON_JUPITER,
    SUMMON_RAMSES,
    SUMMON_NEREID,
    SUMMON_KIRIN,
    SUMMON_ATALANTA,
    SUMMON_CYBELE,
    SUMMON_NEPTUNE,
    SUMMON_TIAMAT,
    SUMMON_PROCNE,
    SUMMON_JUDGMENT,
    SUMMON_BOREAS,
    SUMMON_METEOR,
    SUMMON_THOR,
    SUMMON_ZAGAN,
    SUMMON_MEGAERA,
    SUMMON_FLORA,
    SUMMON_MOLOCH,
    SUMMON_ULYSSES,
    SUMMON_HAURES,
    SUMMON_ECLIPSE,
    SUMMON_COATLICUE,
    SUMMON_DAEDALUS,
    SUMMON_AZUL,
    SUMMON_CATASTROPHE,
    SUMMON_CHARON,
    SUMMON_IRIS
} SUMMON_ID;

// Ends the summon lists, and marks a summon already moved to the front.
#define SUMMON_END 32
// The summons in the order the Summon command lists them (summon_table.c).
extern const Uint8                SummonOrderList[SUMMON_COUNT];
// The experience each character needs for each level (experience_table.c).
extern const Uint32               CharacterLevelExpTable[];

extern const Sint16               ElementPowerResistCurve[5][2];
extern const ENEMY_ELEMENT_PRESET EnemyElementPresetTable[];
extern const CHARACTER_DEFINITION CharacterDefinitionTable[];
const CHARACTER_DEFINITION       *gmCharacterGetDefinition(Sint32 character);
extern const Sint32               CharacterNewGameIds[];
extern const Uint16               ItemRareItemCounters[512];

extern const Uint8                CharacterElementGroupTable[];

// The Djinni sprite of each element, SPRITE_VENUS_DJINNI to SPRITE_JUPITER_DJINNI, whose picture
// mnDjinnShowJoinedMessage shows as the portrait.
extern const Sint16               DjinnElementSprites[];
// Each party member's sprite, by character; each is the character's own id.
extern const Sint16               CharacterSpriteTable[];

struct LEVEL_UP_RESULT;

extern const Sint32 ClassSeriesByElements[4][4];
extern const Uint8  NewGameSceneStateTable[4][5];
extern const Uint16 DataTransferFlags[TRANSFERRED_FLAG_COUNT];
extern const Sint16 DataTransferDjinnChances[ELEMENT_COUNT][TRANSFERRED_DJINN_PER_ELEMENT];
void                gmPartyBalanceDjinn(void);

// Where InventorySnapshot's spare space begins, past the party's equipment gmInventorySnapshotSave
// keeps and the Trial Road's state: the scenes keep their own state there (scene/flame_throwers.h's
// FLAME_THROWERS_STATE).
#define INVENTORY_SNAPSHOT_SPARE 392

#endif
