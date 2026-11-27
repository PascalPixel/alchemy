#ifndef ALCHEMY_GAME_H
#define ALCHEMY_GAME_H

/* @file */
/* The item and summon tables and the game module's functions. */

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

/* What a Psynergy does when cast on the field (an ability's fieldPsynergy), */
/* each named for the abilities that have it in the English release. The */
/* field effect it starts keeps it as its animation. */
typedef enum FIELD_PSY {
	FIELD_PSYNERGY_MOVE = 1,
	FIELD_PSYNERGY_MIND_READ = 2,
	FIELD_PSYNERGY_PLY = 3,
	FIELD_PSYNERGY_FORCE = 4,
	FIELD_PSYNERGY_DOUSE = 5,
	FIELD_PSYNERGY_FROST = 6,
	FIELD_PSYNERGY_LIFT = 7,
	FIELD_PSYNERGY_REVEAL = 8,
	FIELD_PSYNERGY_HALT = 9,
	FIELD_PSYNERGY_CLOAK = 10,
	FIELD_PSYNERGY_CARRY = 11,
	FIELD_PSYNERGY_GROWTH = 12,
	FIELD_PSYNERGY_CATCH = 13,
	/* Whirlwind and Gale. */
	FIELD_PSYNERGY_WHIRLWIND = 14,
	FIELD_PSYNERGY_RETREAT = 15,
	FIELD_PSYNERGY_AVOID = 16,
	FIELD_PSYNERGY_LASH = 17,
	FIELD_PSYNERGY_POUND = 18,
	FIELD_PSYNERGY_TREMOR = 19,
	FIELD_PSYNERGY_SCOOP = 20,
	FIELD_PSYNERGY_BURST = 21,
	FIELD_PSYNERGY_PARCH = 22,
	FIELD_PSYNERGY_SAND = 23,
	FIELD_PSYNERGY_HOVER = 24,
	FIELD_PSYNERGY_CYCLONE = 25,
	FIELD_PSYNERGY_BLAZE = 26,
	FIELD_PSYNERGY_GRIND = 27,
	/* The two abilities whose names are question marks. */
	FIELD_PSYNERGY_RING = 28,
	FIELD_PSYNERGY_ARROW = 29,
	FIELD_PSYNERGY_TELEPORT = 30
} FIELD_PSY;

/* A step of the current field Psynergy (fdPsynergyRunHandler). */
typedef enum FIELD_PSYNERGY_HANDLER {
	/* Casts it with no target (fdPsynergyRun). */
	FIELD_PSYNERGY_HANDLER_UNTARGETED = 0,
	/* Casts it on its target, secondActorId (fdPsynergyDispatch). */
	FIELD_PSYNERGY_HANDLER_ON_TARGET = 1,
	FIELD_PSYNERGY_HANDLER_FINISH = 2, /* finishes it */
	FIELD_PSYNERGY_HANDLER_EXTRA = 3   /* runs its kind's extra handler */
} FIELD_PSYNERGY_HANDLER;

/* The elemental power and resistance of one element level, in */
/* Element_PowerResistByLevel. */
typedef struct ELEMENT_LEVEL_BONUS {
	u16 power;		/* base power, before the tenths of a level */
	u16 resistance; /* base resistance, likewise */
} ELEMENT_LEVEL_BONUS;

extern const ITEM_DEFINITION Item_DefinitionTable[];
extern const SUMMON_DEFINITION Summon_DefinitionTable[];

struct BATTLE_ELEMENT_STATS;

/* How many enemies Enemy_DragonIds and Enemy_SeaCreatureIds list. */
#define DRAGON_ENEMY_COUNT		 47
#define SEA_CREATURE_ENEMY_COUNT 29
extern const u16 Enemy_DragonIds[DRAGON_ENEMY_COUNT];
extern const u16 Enemy_SeaCreatureIds[SEA_CREATURE_ENEMY_COUNT];
extern const ABILITY_DEFINITION AbilityDefinitions[];
extern const DJINN_DEFINITION Djinn_DefinitionTable[];
extern const ENEMY_DEFINITION Enemy_Definitions[];
/* Counts the Djinn recovery down twenty times, setting every recovering Djinni
 */
/* of a party member with HP (btDjinnRecoveryCountDownTwenty). */
void btDjinnRecoveryCountDownTwentyStub(void);
/* Gives a Djinni to a unit. */
s32 gmDjinnGiveStub(s32 unitId, s32 element, s32 djinnIndex);
/* Clears each button shortcut whose Psynergy can no longer be cast */
/* (fdPartyClearUnusableShortcuts). */
void gmPartyClearUnusableShortcutsStub(void);
/* Gives a unit one item, joining its first matching stack (up to */
/* INVENTORY_STACK_MAX) or taking the first free slot. */
s32 gmInventoryAddItemStub(s32 unitId, s32 itemId);
/* Takes one item out of slot, closing up the inventory when it was the last. */
s32 gmInventoryRemoveStub(s32 owner, s32 slot);
void gmUnitCalcBaseElements(s32 unitId, BATTLE_ELEMENT_STATS *destination);
s32 gmClassDetermine(s32 character, u32 *djinn, s32 item);
s32 gmUnitGetElementLevels(s32 enemyId, const u8 *source, s32 *output);
s32 gmUnitRebuildPsynergyList(s32 owner);
/* Decodes message into text with no length limit */
/* (mnMessageDecodeRawUnbounded). */
void mnMessageDecodeRawUnboundedStub(s32 message, u16 *text);

/* How many summons there are (summon_table.c), and the number that ends a */
/* list of them (gmSummonListLearned). */
#define SUMMON_COUNT 29
/* The summons by summon id, each named for its ability, in the order of */
/* Summon_Abilities (summon_table.c): the elements' own four, The Broken */
/* Seal's twelve, then this game's thirteen. */
typedef enum SUMMON_ID {
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
/* Ends the summon lists, and marks a summon already moved to the front. */
#define SUMMON_END 32
/* The summons in the order the Summon command lists them (summon_table.c). */
extern const u8 Summon_OrderList[SUMMON_COUNT];
/* The experience each character needs for each level (experience_table.c). */
extern const u32 Character_LevelExpTable[];

extern const s16 Element_PowerResistCurve[5][2];
extern const ENEMY_ELEMENT_PRESET Enemy_ElementPresetTable[];
extern const CHARACTER_DEFINITION Character_DefinitionTable[];
const CHARACTER_DEFINITION *gmCharacterGetDefinition(s32 character);
extern const s32 Character_NewGameIds[];
extern const u16 Item_RareItemCounters[512];

extern const u8 Character_ElementGroupTable[];

/* The Djinni sprite of each element, SPRITE_VENUS_DJINNI to */
/* SPRITE_JUPITER_DJINNI, whose picture mnDjinnShowJoinedMessage shows as the */
/* portrait. */
extern const s16 Djinn_ElementSprites[];
/* Each party member's sprite, by character; each is the character's own id. */
extern const s16 Character_SpriteTable[];

struct LEVEL_UP_RESULT;

extern const s32 Class_SeriesByElements[4][4];
extern const u8 NewGame_SceneStateTable[4][5];
extern const u16 DataTransfer_Flags[TRANSFERRED_FLAG_COUNT];
extern const s16 DataTransfer_DjinnChances[ELEMENT_COUNT]
										  [TRANSFERRED_DJINN_PER_ELEMENT];
void gmPartyBalanceDjinn(void);

/* Where InventorySnapshot's spare space begins, past the party's equipment */
/* gmInventorySnapshotSave keeps and the Trial Road's state: the scenes keep */
/* their own state there (scene/flame_throwers.h's FLAME_THROWERS_STATE). */
#define INVENTORY_SNAPSHOT_SPARE 392

#endif
