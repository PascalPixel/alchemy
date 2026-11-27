#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

/* Unit, a party member's or enemy's battle state, with the elements, abilities
   and actions it uses. */

#include "edition.h"
#include "static_assert.h"
#include "game/item.h"

/* The sizes the STATIC_ASSERTs below hold the structs to, and the action id's
   bits in an action list entry. */
enum {
	ABILITY_DEFINITION_SIZE = 16, /* bytes in a ABILITY_DEFINITION */
	/* Bytes in a UNIT, which the code clears and copies whole. */
	BATTLE_UNIT_SIZE = 332,
	/* PSYNERGY_ENTRY.encodedAction's action id */
	PSYNERGY_ABILITY_MASK = 0x3fff
};

/* An entry in a unit's action list: the action's id (PSYNERGY_ABILITY_MASK) and
   marks for an action its class or an equipped item gave. */
typedef struct PSYNERGY_ENTRY {
	/* bits: 15 from the class | 14 from an equipped item | 13..0 action id.
	   Entries with neither mark were taught for good; 0 is empty. */
	Uint16 encodedAction;
	Uint16 unused; /* no code names it */
} PSYNERGY_ENTRY;

/* A unit's elemental power and resistance in one element. */
typedef struct BATTLE_ELEMENT_STATS {
	Sint16 power;	   /* Elemental power in the element. */
	Sint16 resistance; /* Elemental resistance to it. */
} BATTLE_ELEMENT_STATS;

/* The elements, in the order of every table kept per element: a unit's element
   stats, the Djinn and the summons. ELEMENT_NONE marks an ability or a weapon
   that has no element. */
typedef enum ELEMENT {
	ELEMENT_VENUS,
	ELEMENT_MERCURY,
	ELEMENT_MARS,
	ELEMENT_JUPITER,
	ELEMENT_NONE,
	ELEMENT_COUNT = ELEMENT_NONE
} ELEMENT;

/* ABILITY_DEFINITION.range of an ability that reaches every unit on the side,
   each as if aimed at. */
#define ABILITY_RANGE_ALL 255
/* The range icon the Psynergy lists draw for ABILITY_RANGE_ALL, the last of the
   twelve; range n takes icon n - 1. */
#define RANGE_ICON_ALL	  11

/* The low four bits of ABILITY_DEFINITION.flags: the ability kind (enum
   ABILITY_FORMULA). */
#define ABILITY_FORMULA_MASK 15

/* An ability kind picks what the ability's power does to each target: the
   formula btAbilityApplyToTarget puts it through. ABILITY_FORMULA_BLANK,
   ABILITY_FORMULA_EFFECT_ONLY and ABILITY_FORMULA_NO_POWER apply no power. No
   row has a kind above ABILITY_FORMULA_HEAL_PP, which would apply none
   either. */
typedef enum ABILITY_FORMULA {
	ABILITY_FORMULA_BLANK = 0,	 /* only the blank rows no table uses */
	ABILITY_FORMULA_HEAL_HP = 1, /* Cure, Fizz, healing items */
	ABILITY_FORMULA_EFFECT = 2,	 /* damage only if the effect lands */
	ABILITY_FORMULA_ATTACK = 3,	 /* Attack vs Defense, plus the power */
	ABILITY_FORMULA_ATTACK_MULTIPLIED = 4, /* that attack times power / 10 */
	ABILITY_FORMULA_DAMAGE = 5, /* HP damage from the power (Psynergy) */
	/* HP damage from enemies' moves such as Fire Blessing and Deadly Gas,
	   falling off faster than Psynergy's (RangeFalloffEnemyDamage). Unless the
	   move has wording of its own, it's announced as `<unit> unleashes
	   <ability>!`. A weapon's unleash is something else:
	   ABILITY_FORMULA_ATTACK, or for Rapid Smash
	   ABILITY_FORMULA_ATTACK_MULTIPLIED. */
	ABILITY_FORMULA_ENEMY_DAMAGE = 6,
	/* Granite, Flash and Kite, whose effect is all they do. */
	ABILITY_FORMULA_EFFECT_ONLY = 7,
	/* Summon damage falls off faster than Psynergy's
	   (RangeFalloffSummonDamage). Called with the Summon command, a summon
	   also adds 3% of the target's maximum HP to its power for each Djinni it
	   spends; the Fusion Dragon's Outer Space and Dragon Driver go without that
	   bonus. */
	ABILITY_FORMULA_SUMMON = 8,
	/* The field Psynergy (Move, Catch...), the field-only stat items (Apple,
	   Power Bread...), Defend, Flee and the enemies' commands (Threaten, Ally
	   Search...). The enemies' targeting check (CHECK_EFFECT_TARGET) treats it
	   as a summon: worth it on a living unit only when the power is nonzero. */
	ABILITY_FORMULA_NO_POWER = 9,
	/* Crazy Voice, and Psy Drain when it lands */
	ABILITY_FORMULA_PP_DAMAGE = 10,
	ABILITY_FORMULA_HEAL_PP = 11 /* Ember, Psy Crystal, Adept Ring */
} ABILITY_FORMULA;

/* ABILITY_DEFINITION.effect: what an ability does besides its damage or
   healing, applied by btAbilityApplyToTarget unless noted. Each is named for
   the abilities that have it (ability_table.c, with their English names and
   descriptions). ABILITY_EFFECT_SIDESTEP and ABILITY_EFFECT_CRAZY_VOICE are
   read by no code here, and no row has ABILITY_EFFECT_RESTORE_HP or
   ABILITY_EFFECT_RESTORE_PP. A stat change whose name ends in `_SHARPLY` moves
   its stat two stages, one without moves it one. */
typedef enum ABILITY_EFFECT {
	ABILITY_EFFECT_NONE = 0,
	ABILITY_EFFECT_RESTORE_HP = 1,	/* restores HP (the item menu's use) */
	ABILITY_EFFECT_RESTORE_PP = 2,	/* restores PP (the item menu's use) */
	ABILITY_EFFECT_CURE_POISON = 3, /* Cure Poison, Antidote */
	ABILITY_EFFECT_RESTORE = 4,		/* Restore, Elixir */
	ABILITY_EFFECT_REVIVE = 5,		/* Revive, Water of Life: full HP */
	ABILITY_EFFECT_ATTACK_UP_SHARPLY = 6,		 /* Impact */
	ABILITY_EFFECT_ATTACK_UP = 7,				 /* High Impact */
	ABILITY_EFFECT_ATTACK_DOWN_SHARPLY = 8,		 /* Dull */
	ABILITY_EFFECT_ATTACK_DOWN = 9,				 /* Blunt */
	ABILITY_EFFECT_DEFENSE_UP_SHARPLY = 10,		 /* Guard */
	ABILITY_EFFECT_DEFENSE_UP = 11,				 /* Protect */
	ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY = 12,	 /* Impair */
	ABILITY_EFFECT_DEFENSE_DOWN = 13,			 /* Debilitate */
	ABILITY_EFFECT_RESISTANCE_UP_SHARPLY = 14,	 /* Ward */
	ABILITY_EFFECT_RESISTANCE_UP = 15,			 /* Resist */
	ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY = 16, /* Weaken */
	ABILITY_EFFECT_RESISTANCE_DOWN = 17,		 /* Enfeeble */
	ABILITY_EFFECT_POISON = 18,					 /* Taint */
	ABILITY_EFFECT_VENOM = 19,					 /* Poison */
	ABILITY_EFFECT_DELUSION = 20,				 /* Delude */
	ABILITY_EFFECT_CONFUSION = 21,				 /* Confuse */
	ABILITY_EFFECT_CHARM = 22,
	ABILITY_EFFECT_STUN = 23, /* Paralyze */
	ABILITY_EFFECT_SLEEP = 24,
	ABILITY_EFFECT_SEAL = 25, /* Bind: Psynergy sealed */
	ABILITY_EFFECT_HAUNT = 26,
	ABILITY_EFFECT_INSTANT_DEATH = 27, /* Condemn */
	ABILITY_EFFECT_CURSE = 28,		   /* Curse: the Grim Reaper's countdown */
	ABILITY_EFFECT_REGENERATE = 29,
	ABILITY_EFFECT_REFLECT = 30,
	ABILITY_EFFECT_DRAIN_HP = 31,
	ABILITY_EFFECT_DRAIN_PP = 32,	   /* Psy Drain */
	ABILITY_EFFECT_BREAK = 33,		   /* Break: raised stats undone */
	ABILITY_EFFECT_LEAVE_ONE_HP = 34,  /* Severe Blow, Thrash */
	ABILITY_EFFECT_HALVE_DEFENSE = 35, /* Torch */
	/* A ladder of chances of an ability's critical hit, rolled by
	   btActionRunBuild: A is 1 in 64, then each rung doubles the odds, to E's 1
	   in 4. Only an attack (ABILITY_FORMULA_ATTACK or
	   ABILITY_FORMULA_ATTACK_MULTIPLIED) applies it, and no ability has one. */
	ABILITY_EFFECT_CRITICAL_A = 36,
	ABILITY_EFFECT_CRITICAL_B = 37,
	ABILITY_EFFECT_CRITICAL_C = 38,
	ABILITY_EFFECT_CRITICAL_D = 39,
	ABILITY_EFFECT_CRITICAL_E = 40,
	/* A chance of a double or triple blow, rolled by btActionRunBuild:
	   the plain one 1 in 4, the rare one, which no ability has, 1 in 8, and the
	   likely one 153 in 256. */
	ABILITY_EFFECT_DOUBLE_BLOW_RARE = 41,
	/* Double Fang, Twin Beaks, Flying Attack */
	ABILITY_EFFECT_DOUBLE_BLOW = 42,
	ABILITY_EFFECT_TRIPLE_BLOW_RARE = 43,
	ABILITY_EFFECT_TRIPLE_BLOW = 44,   /* Swift Strike */
	ABILITY_EFFECT_THREATEN = 45,	   /* Contain, Threaten, Tremble */
	ABILITY_EFFECT_DEFEND = 46,		   /* Defend, Granite */
	ABILITY_EFFECT_TOTAL_DEFENSE = 47, /* Total Defense, Flash */
	ABILITY_EFFECT_SIDESTEP = 48,
	ABILITY_EFFECT_FLEE = 49,	   /* Flee: an enemy's becomes COMMAND_FLEE */
	ABILITY_EFFECT_CALL_ALLY = 50, /* Ally Search, Speed Surge */
	ABILITY_EFFECT_CALL_HELP = 51, /* Search for Help */
	ABILITY_EFFECT_CRAZY_VOICE = 52,
	ABILITY_EFFECT_GROUND = 53,		   /* Ground, War Cry, Wicked Howl */
	ABILITY_EFFECT_CHALLENGE = 54,	   /* no ability has it */
	ABILITY_EFFECT_SELF_DESTRUCT = 55, /* Dynamite, Mortal Blow */
	ABILITY_EFFECT_REVIVE_HALF = 56,   /* Quartz */
	ABILITY_EFFECT_DEW = 57,		   /* Dew: revives with 80% of max HP */
	ABILITY_EFFECT_AGILITY_DOWN = 58,  /* Vine */
	ABILITY_EFFECT_AGILITY_UP = 59,	   /* Zephyr */
	ABILITY_EFFECT_DRAIN_HALF_HP = 60, /* Sap */
	ABILITY_EFFECT_FIZZ = 61,		   /* Fizz: 60% of max HP */
	ABILITY_EFFECT_SPRITZ = 62,		   /* Spritz: 30% of max HP */
	ABILITY_EFFECT_EMBER = 63,		   /* Ember: 7% of max PP */
	ABILITY_EFFECT_CURE_ALL = 64,	   /* Tonic */
	ABILITY_EFFECT_DOUBLE_BLOW_LIKELY = 65, /* Gust, Rapid Smash, Slice */
	ABILITY_EFFECT_ACT_TWICE = 66,			/* Kite */
	ABILITY_EFFECT_SEAL_2_TURNS = 67,		/* Luff */
	ABILITY_EFFECT_TRIPLE_BLOW_LIKELY = 68, /* Sonic Smash */
	ABILITY_EFFECT_PSY_LEECH = 69
} ABILITY_EFFECT;

/* ABILITY_DEFINITION.targetMode: whom an ability may target, from its user's
   view. The party's cursor (mnBattleTargetRunSelection) shows the enemies for
   every mode but ABILITY_TARGET_ALLIES and ABILITY_TARGET_SELF. No row of
   ability_table.c has a mode above ABILITY_TARGET_SELF; btActionRunSetTargets
   would walk such a mode's range as it does ABILITY_TARGET_FOES'. */
typedef enum ABILITY_TARGET {
	/* The ability takes one target, the aimed unit, whatever its range
	   (btActionRunSetTargets), and an enemy aims it at itself. The field
	   Psynergy and enemies' commands such as Threaten, Ally Search and Flee
	   have it. */
	ABILITY_TARGET_ONE = 0,
	ABILITY_TARGET_FOES = 1,
	ABILITY_TARGET_ALLIES = 2, /* the user's side, the user included */
	/* Unused: no ability has it. An enemy that draws it aims at itself and
	   picks again (btAIChooseAction); the party's cursor and
	   btActionRunSetTargets take it as ABILITY_TARGET_FOES. */
	ABILITY_TARGET_UNUSED = 3,
	ABILITY_TARGET_SELF = 4 /* the user alone, whatever the range */
} ABILITY_TARGET;

/* UNIT.curse bits, the curse. */
typedef enum CURSE_FLAG {
	CURSE_BOUND = 0x01,		 /* the unit is bound */
	CURSE_ITEM = 0x02,		 /* a cursed item is equipped */
	CURSE_KEEP_BOUND = 0x04, /* keeps CURSE_BOUND through gmUnitCalculate */
	/* EQUIP_EFFECT_CURSE_REMOVABLE: gmUnitCalculate clears it again with
	   CURSE_BOUND. */
	CURSE_REMOVABLE = 0x08
} CURSE_FLAG;

/* An ability's definition, a row of AbilityDefinitions: its targeting, ability
   kind, element, effect, range, PP cost and power. */
typedef struct ABILITY_DEFINITION {
	Uint8 targetMode; /* ABILITY_TARGET */
	/* bits: 7 usable in battle | 6 usable in the field | 5..4 clear in every
	   row | 3..0 ability kind (ABILITY_FORMULA_MASK). */
	Uint8 flags;
	Uint8 element;				/* ELEMENT */
	Uint8 effect;				/* ABILITY_EFFECT */
	Uint8 icon;					/* the icon the menus draw */
	Uint8 reservedAfterIcon[3]; /* 0 in every row */
	Uint8
		/* the aimed unit and range - 1 either side, or ABILITY_RANGE_ALL */
		  range;
	Uint8 ppCost; /* PP the ability costs */
	/* The ability's power, as its ability kind reads it: for damage, about what
	   the aimed target loses when the user's Power in the element equals the
	   target's Resist (a summon adds to it, see ABILITY_FORMULA_SUMMON); for
	   healing, about what the aimed target regains from a healer with 100 Power
	   in the element; for an attack, what it adds to the blow at an even match,
	   and for a multiplied one the blow's multiple in tenths. The battle
	   README's "How an ability resolves" gives the formulas. */
	Uint16 power;
	/* The field Psynergy the ability works as, 1 (Move) to 16 (Avoid); 0 for
	   none. The field effects branch on it, and the Status screen counts it as
	   usable in the field. */
	Uint8 fieldPsynergy;
	Uint8 reservedAfterType[3]; /* 0 in every row */
} ABILITY_DEFINITION;

/* ABILITY_DEFINITION.flags: where an ability, or the ability an item calls, can
   be used, as the Status screen's Psynergy list says ("Can be used in
   Towns/Dungeons", "Can be used in Battle"). */
typedef enum ABILITY_USE {
	ABILITY_USABLE_IN_FIELD = 0x40, /* can be used outside battle */
	/* can be used in battle; else greyed there */
	ABILITY_USABLE_IN_BATTLE = 0x80
} ABILITY_USE;

/* UNIT.classId of an enemy, which has no class of its own. */
#define CLASS_NONE 0

/* Unit ids from 128 are the enemies'. */
#define FIRST_ENEMY_UNIT 128

/* The last party unit id: ids 0 to 7, one for each character, ISAAC to
   PIERS. */
#define ALLY_MAX		 7
#define PARTY_UNIT_COUNT (ALLY_MAX + 1)

/* The party fights four at a time, three in a link battle; six enemies at most,
   three in a link battle. */
#define PARTY_BATTLE_SIZE 4
#define ENEMY_BATTLE_SIZE 6

/* The most units in a battle, every party unit and the most enemies: the length
   of a unit list such as btUnitListIds fills. */
#define UNIT_MAX (PARTY_UNIT_COUNT + ENEMY_BATTLE_SIZE)

/* A full HP or PP gauge (UNIT.hpGauge, UNIT.ppGauge): the gauge is the current
   value over the maximum in 2.14 fixed point, as gmUnitCalculate multiplies a
   maximum by it and divides by this, and the cap gmUnitClampGauge puts on hp /
   hpMax shifted up UNIT_GAUGE_SHIFT bits. */
#define UNIT_GAUGE_SHIFT 14
#define UNIT_GAUGE_FULL	 (1 << UNIT_GAUGE_SHIFT)

/* The bits of UNIT.baseActionCount that count the unit's actions a turn. */
#define UNIT_ACTION_COUNT_MASK 0xf

/* What a Resistance stage adds to the resistance to every element
   (gmUnitCalculate). */
#define RESISTANCE_PER_STAGE 20

/* UNIT.poison: the poison takes a tenth of the maximum HP at the end of each of
   the unit's actions, deadly poison a fifth. */
#define POISON_NORMAL 1
#define POISON_VENOM  2

/* UNIT.defendLevel, 0 for no Defend: Defend and Granite halve the damage, Total
   Defense and Flash cut it to a tenth (REDUCE_BY_GUARD, battle_ability.c). It
   lasts the turn: btUnitResetGuardLevels clears it as the next one's commands
   are gathered. */
#define DEFEND_NORMAL 1
#define DEFEND_TOTAL  2

/* The caps on a unit's level and stats. */
#define UNIT_LEVEL_MAX		  99
#define UNIT_HP_MAX			  9999 /* maximum HP and PP, an enemy's */
#define UNIT_CHARACTER_HP_MAX 1999 /* maximum HP and PP, a character's */
#define UNIT_STAT_MAX		  999  /* Attack, Defense and Agility */
#define UNIT_LUCK_MAX		  99
#define UNIT_ELEMENT_MAX	  200 /* elemental power and resistance */
#define UNIT_HP_REGEN_MAX	  10000
#define UNIT_PP_REGEN_MAX	  200

/* How many actions a unit's list holds (UNIT.psynergy). */
#define PSYNERGY_SLOTS 32

/* An enemy's memory of its action choice (btAIChooseAction). */
typedef struct BATTLE_AI_STATE {
	/* ACTION_STRATEGY_RANDOM_START has drawn its start. */
	unsigned initialized : 1;
	unsigned selection : 3;	 /* the place in its action list, 0 to 7 */
	unsigned remaining : 28; /* not used here */
} BATTLE_AI_STATE;

/* A unit's name buffer: 14 characters and the 0 that ends them. */
#define UNIT_NAME_SIZE 15

/* A unit in battle: a party member's or an enemy's stats, Psynergy, items,
   Djinn and statuses. */
typedef struct UNIT {
	/* The name, one byte a character, ending in 0. An enemy's is its message
	   name cut to 14 characters, with the suffix letter gmBattleUnitSet is
	   given (a digit in Japanese); its last byte is always 0. */
	Uint8  name[UNIT_NAME_SIZE];
	Uint8  level;  /* 1 to 99 for a character */
	Sint16 baseHp; /* before equipment, Djinn and class (gmUnitCalculate) */
	Sint16 basePp; /* before equipment, Djinn and class (gmUnitCalculate) */
	/* hp / hpMax in 0..UNIT_GAUGE_FULL; 1 while any HP remains */
	Sint16 hpGauge;
	/* pp / ppMax in 0..UNIT_GAUGE_FULL; 1 while any PP remains */
	Sint16 ppGauge;
	Uint16 baseAttack;	/* before equipment, Djinn, class and stages */
	Uint16 baseDefense; /* before equipment, Djinn, class and stages */
	Uint16 baseAgility; /* before equipment, Djinn, class and stages */
	Uint8  baseLuck;	/* before equipment, Djinn and class */
	/* Actions a turn before equipment: the low four bits are read. An enemy's
	   comes from its definition's actionCount; a level up sets a character's to
	   1. */
	Uint8 baseActionCount;
	/* HP regained each turn before equipment; 0 for a * character */
	Uint8 baseHpRegen;
	/* PP regained each turn before equipment and lighthouses */
	Uint8 basePpRegen;
	Uint8 baseUnused[2]; /* no code names it */
	/* Power and resistance before equipment and stages: an enemy's from its
	   element preset, a character's from its element levels
	   (gmUnitCalculateBaseElements). */
	BATTLE_ELEMENT_STATS baseElement[ELEMENT_COUNT];
	/* 0 to 1999 for a character, 9999 for an enemy (gmUnitCalculate) */
	Sint16 hpMax;
	/* 0 to 1999 for a character, 9999 for an enemy (gmUnitCalculate) */
	Sint16 ppMax;
	Sint16 hp;		/* current HP, 0 when downed */
	Sint16 pp;		/* current PP */
	Uint16 attack;	/* Attack, 0 to 999 (gmUnitCalculate) */
	Uint16 defense; /* Defense, 0 to 999 (gmUnitCalculate) */
	Uint16 agility; /* Agility, 0 to 999: the order of the turn's actions */
	Uint8  luck;	/* 0 to 99 (gmUnitCalculate) */
	/* Actions each turn, 0 to 2: baseActionCount, one more while ready, and the
	   equipment's extra actions (gmUnitCalculate). Each gives the unit a place
	   in the turn's queue. */
	Uint8 actionsPerTurn;
	Uint8 hpRegen; /* HP regained at each turn's end (gmUnitCalculate) */
	/* PP regained at each turn's end, 0 to 200 (gmUnitCalculate): 4 more once
	   the lighthouse of the character's element is lit. */
	Uint8 ppRegen;
	Uint8 unused[2]; /* no code names it, as baseUnused */
	/* Power and resistance in each element, 0 to 200, after equipment, a
	   summon's boost and Resistance stages (gmUnitCalculate). */
	BATTLE_ELEMENT_STATS element[ELEMENT_COUNT];
	/* The action list: Psynergy taught for good, then the class's and the
	   equipment's (gmUnitRefreshPsynergy). An empty entry is 0. */
	PSYNERGY_ENTRY psynergy[PSYNERGY_SLOTS];
	/* Items held, one entry each: bits 15..11 quantity | 10 broken | 9 equipped
	   | 8..0 item id (INVENTORY_*). An empty entry is 0. */
	Uint16 inventory[INVENTORY_SLOTS];
	Uint8  paddingBeforeDjinn[2]; /* up to the word djinnHeld starts on */
	/* The Djinn held, one bit per Djinni (by its index) in each element's
	   word. */
	Uint32 djinnHeld[ELEMENT_COUNT];
	/* The Djinn set, one bit per Djinni; only these add their stat bonuses. */
	Uint32 djinnSet[ELEMENT_COUNT];
	/* the Djinn held in each element, at most 10 */
	Uint8 djinnHeldCounts[ELEMENT_COUNT];
	Uint8
		djinnSetCounts[ELEMENT_COUNT]; /* the Djinn set in each element; the */
	/* class's element levels are read from these Copied from the enemy's
	   definition, where it is 0 in every row. During a battle the enemy's
	   action choice keeps its place in its action list here
	   (btAIChooseAction). */
	BATTLE_AI_STATE aiState;
	Uint32			experience; /* a character's experience points */
	/* Who the unit is: a character's id (ISAAC to PIERS, below 8) or an enemy's
	   id (gmBattleUnitSet); BattleSprites and the enemy tables are read by
	   it. */
	Uint8 id;
	/* The character's class, worked out from its Djinn (gmUnitUpdateClass);
	   CLASS_NONE marks an enemy, and code throughout tests it to tell the two
	   apart. */
	Uint8 classId;
	/* BATTLE_SIDE. The Djinn the party fights are enemies put on
	   BATTLE_SIDE_PARTY (gmBattleUnitSet). */
	Uint8 side;
	/* DEFEND_NORMAL or DEFEND_TOTAL while the unit guards, else 0. */
	Sint8 defendLevel;
	/* A summon's boost to its summoner's power in each element: at the end of
	   the summoner's action, the element the summon spent the most Djinn of
	   rises to that many (btUnitProcessActionEnd), and n adds (n * n + n) * 5
	   power (gmUnitCalculate). Cleared at the battle's start, on going down and
	   by Break. */
	Sint8 summonBoost[ELEMENT_COUNT];
	/* The curse: CURSE_FLAG bits. gmUnitCalculate rebuilds CURSE_BOUND and
	   CURSE_ITEM from the equipment. A bound unit loses one action in four,
	   except Defend (btActionRunBuild); any bit set shows the curse in the
	   Status screens and lets the sanctum remove it. */
	Sint8 curse;
	Sint8 poison; /* POISON_NORMAL or POISON_VENOM, 0 for none */
	/* Each stat change: the turns it has left, and its stage (gmUnitCalculate:
	   an eighth of Attack, Defense or Agility, or 20 Resistance, a stage).
	   Stages run -4 to 4 and last 7 turns; a lowered one may recover sooner at
	   the end of a turn. */
	Uint8 attackTurns;
	Sint8 attackStage;
	Uint8 defenseTurns;
	Sint8 defenseStage;
	Uint8 resistanceTurns;
	Sint8 resistanceStage;
	/* Delusion to sleep: the turns each status has left, 7 when it lands; each
	   may end sooner at the end of a turn. Sleep, confusion, charm and stun
	   cost the unit its actions. */
	Uint8 delusion;
	Uint8 confusion;
	Uint8 charm;
	Uint8 stun;
	Uint8 sleep;
	/* bits: 7..3 Luff (16 when cast, minus 8 a turn) | 2..0 Bind's turns (7
	   when cast). Psynergy is sealed while any bit is set. */
	Uint8 psySeal;
	Uint8 regenerate; /* Regenerate's turns left, 7 when cast */
	Uint8 reflect;	  /* Reflect's turns left, 7 when cast */
	Uint8 haunt;	  /* set while an evil spirit grips the unit */
	/* The Grim Reaper's countdown: 7 when cursed, down one at the end of each
	   of the unit's own actions (btUnitProcessActionEnd); at 0 it falls. */
	Uint8 grimReaper;
	/* The totals of EQUIP_EFFECT_ADD_UNLEASH_CHANCE and
	   EQUIP_EFFECT_ADD_COUNTERSTRIKE_RATE that gmUnitCalculate sums; nothing
	   reads them back. */
	Uint8 equipBonus[2];
	/* Kite's extra action: 2 when cast, down one at each turn's end; while
	   nonzero gmUnitCalculate gives the unit one more action a turn. */
	Uint8 ready;
	Uint8 immobile; /* set by ABILITY_EFFECT_GROUND: the next action is lost */
	Uint8 agilityTurns; /* the Agility change's turns left, 5 when cast */
	/* The Agility stage: -4 (Vine) to 8 (Zephyr, twice the Agility). */
	Sint8 agilityStage;
	/* Set by the challenge effect: an enemy with it never chooses to flee. */
	Sint8 challenge;
	Uint8 paddingBeforeEnemyId; /* up to the halfword enemyId starts on */
	/* Not named by any TBS code; TLA reads it as the unit's id. */
	Uint16 enemyId;
} UNIT;

STATIC_ASSERT(sizeof(ABILITY_DEFINITION) == ABILITY_DEFINITION_SIZE);
STATIC_ASSERT(offsetof(ABILITY_DEFINITION, effect) == 3);
STATIC_ASSERT(offsetof(ABILITY_DEFINITION, power) == 10);
STATIC_ASSERT(offsetof(ABILITY_DEFINITION, fieldPsynergy) == 12);

STATIC_ASSERT(sizeof(PSYNERGY_ENTRY) == 4);
STATIC_ASSERT(sizeof(BATTLE_ELEMENT_STATS) == 4);
STATIC_ASSERT(sizeof(UNIT) == BATTLE_UNIT_SIZE);
STATIC_ASSERT(offsetof(UNIT, baseHp) == 16);
STATIC_ASSERT(offsetof(UNIT, baseAttack) == 24);
STATIC_ASSERT(offsetof(UNIT, psynergy) == 88);
STATIC_ASSERT(offsetof(UNIT, djinnHeld) == 248);
STATIC_ASSERT(offsetof(UNIT, djinnSet) == 264);
STATIC_ASSERT(offsetof(UNIT, aiState) == 288);
STATIC_ASSERT(offsetof(UNIT, hp) == 56);
STATIC_ASSERT(offsetof(UNIT, baseElement) == 36);
STATIC_ASSERT(offsetof(UNIT, level) == 15);
STATIC_ASSERT(offsetof(UNIT, element) == 72);
STATIC_ASSERT(offsetof(UNIT, ready) == 324);
STATIC_ASSERT(offsetof(UNIT, inventory) == 216);
STATIC_ASSERT(offsetof(UNIT, djinnHeldCounts) == 280);
STATIC_ASSERT(offsetof(UNIT, djinnSetCounts) == 284);
STATIC_ASSERT(offsetof(UNIT, defendLevel) == 299);
STATIC_ASSERT(offsetof(UNIT, charm) == 314);
STATIC_ASSERT(offsetof(UNIT, haunt) == 320);

STATIC_ASSERT(offsetof(UNIT, enemyId) == 330);

/* Clamps a gauge ratio to 0..UNIT_GAUGE_FULL. */
static __inline__ Sint32 gmUnitClampGauge(Sint32 ratio)
{
	Sint32 clamped;

	if(ratio > UNIT_GAUGE_FULL)
		clamped = UNIT_GAUGE_FULL;
	else if(ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

/* Recalculates HP and PP gauges, keeping at least one when any points
   remain. */
#define UNIT_UPDATE_GAUGES(unit) \
	do { \
		(unit)->hpGauge = gmUnitClampGauge( \
			((unit)->hp << UNIT_GAUGE_SHIFT) / (unit)->hpMax); \
		if((unit)->hpGauge == 0 && (unit)->hp != 0) \
			(unit)->hpGauge = 1; \
		(unit)->ppGauge = gmUnitClampGauge( \
			((unit)->pp << UNIT_GAUGE_SHIFT) / (unit)->ppMax); \
		if((unit)->ppGauge == 0 && (unit)->pp != 0) \
			(unit)->ppGauge = 1; \
	} while(0)

#endif
