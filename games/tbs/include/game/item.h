#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

/* Items: their definitions, types, flags and effects. */

#include "edition.h"
#include "static_assert.h"
#include "stubs.h"

struct UNIT;

/* An inventory entry's parts, and a unit's slot count. bits: 15..11 quantity |
   10 broken | 9 equipped | 8..0 item id */
typedef enum INVENTORY_ENCODING {
	INVENTORY_ITEM_ID_MASK = 0x1ff,
	INVENTORY_EQUIPPED = 0x200,
	INVENTORY_BROKEN = 0x400,
	/* The item id with its broken and equipped bits, all but the quantity. */
	INVENTORY_ENTRY_MASK = 0x7ff,
	/* The quantity field holds the count less one; a stack (ITEM_STACKABLE)
	   grows to INVENTORY_STACK_MAX (gmInventoryAddItem). */
	INVENTORY_QUANTITY_SHIFT = 11,
	/* One more in the stack. */
	INVENTORY_QUANTITY_STEP = 1 << INVENTORY_QUANTITY_SHIFT,
	INVENTORY_QUANTITY_MASK = 0xf800,
	INVENTORY_SLOTS = 15,	 /* UNIT.inventory's entries */
	INVENTORY_STACK_MAX = 30 /* the most items a stack holds */
} INVENTORY_ENCODING;

/* What gmInventoryRemove and gmInventoryDiscard return. */
typedef enum INVENTORY_REMOVE_RESULT {
	INVENTORY_REMOVE_EMPTY_SLOT = -1, /* nothing was there to take */
	INVENTORY_REMOVE_ONE = 1,		  /* the stack is one smaller */
	/* The last one left its slot, and the slots after it moved up. */
	INVENTORY_REMOVE_LAST = 2
} INVENTORY_REMOVE_RESULT;

/* What gmInventoryEquip returns. */
typedef enum INVENTORY_EQUIP_RESULT {
	INVENTORY_EQUIP_DONE = 0,	  /* equipped, or it was already */
	INVENTORY_EQUIP_REFUSED = -1, /* the character cannot equip it */
	/* The item it would replace is cursed on (ITEM_CANNOT_UNEQUIP). */
	INVENTORY_EQUIP_STUCK = -2
} INVENTORY_EQUIP_RESULT;

/* What gmInventoryCheckDiscard returns: whether an item may be dropped or sold,
   and why not. */
typedef enum INVENTORY_DISCARD_CHECK {
	INVENTORY_DISCARD_ALLOWED = 0,
	INVENTORY_DISCARD_EMPTY_SLOT = -1,
	/* Equipped and cursed on: the shop shows "Can't remove." */
	INVENTORY_DISCARD_STUCK = -3,
	/* An important item (ITEM_INDISPENSABLE): the shop shows "Can't drop." */
	INVENTORY_DISCARD_INDISPENSABLE = -4
} INVENTORY_DISCARD_CHECK;

/* The bits of ITEM_DEFINITION.flg. */
typedef enum ITEM_FLAGS {
	ITEM_CURSED = 1, /* equipping it curses the wearer (UNIT.curse) */
	/* Once equipped it cannot be taken off, or dropped while worn. */
	ITEM_CANNOT_UNEQUIP = 2,
	/* A rare item: "A rare item" on the Item screen, the shop's
	   MsgRareSellOffer, and second rank among the spoils (btSpoilsRankItem). */
	ITEM_RARE = 4,
	/* "An important item" on the Item screen: never dropped, worth nothing to a
	   shop (mnShopSalePrice), and first among a battle's drops kept. */
	ITEM_INDISPENSABLE = 8,
	ITEM_STACKABLE = 16 /* kept in one inventory entry with a quantity */
} ITEM_FLAGS;

/* What using an item costs (ITEM_DEFINITION's useType). */
#define ITEM_USE_FREE	   0 /* nothing: it is kept, as equipment is */
#define ITEM_USE_CONSUMED  1 /* one is used up */
#define ITEM_USE_MAY_BREAK 2 /* breaks one time in eight */
/* Not used up: equipped, it teaches its abilityId as Psynergy. */
#define ITEM_USE_TEACHES   3
#define ITEM_USE_EMPTIES   4 /* Hermes' Water leaves an Empty Bottle */

/* What kind of thing an item is (gmItemGetEquipmentGroup), which picks the
   comparison a shop shows. */
typedef enum EQUIPMENT_GROUP {
	/* anything not a weapon or armor, rings included */
	EQUIPMENT_GROUP_NONE = 0,
	EQUIPMENT_GROUP_WEAPON = 1,
	/* Armor, shields, helms, boots and shirts. */
	EQUIPMENT_GROUP_ARMOR = 2
} EQUIPMENT_GROUP;

/* An item's type (ITEM_DEFINITION's type, item_table.c's type field). A unit
   equips one item of each type, except Psynergy items. */
typedef enum ITEM_TYPE {
	/* Not equipment: Herb, Game Ticket, the keys, unused rows. */
	ITEM_TYPE_NONE = 0,
	ITEM_TYPE_WEAPON = 1, /* swords, axes, maces, rods and staffs */
	ITEM_TYPE_ARMOR = 2,  /* armor, clothes and robes */
	ITEM_TYPE_SHIELD = 3, /* shields, gloves and armlets */
	ITEM_TYPE_HELM = 4,	  /* helms, caps, crowns and circlets */
	ITEM_TYPE_BOOTS = 5,  /* Hyper Boots to Turtle Boots */
	/* The items that teach Psynergy while held: Orb of Force to Catch Beads. */
	ITEM_TYPE_PSYNERGY = 6,
	/* no row of item_table.c has it; listed as equipment */
	ITEM_TYPE_UNUSED = 7,
	ITEM_TYPE_RING = 8, /* Adept Ring to Cleric's Ring */
	ITEM_TYPE_SHIRT = 9 /* Mythril Shirt, Silk Shirt, Running Shirt */
} ITEM_TYPE;

/* How many effects an item has while equipped (ITEM_DEFINITION.effs). */
#define EQUIP_EFFECT_SLOTS 4
/* A multiplier of 10 tenths, which keeps a stat as it is: the classes' stats
   (CLASS_DEFINITION) and the EQUIP_EFFECT_SCALE_ effects are in tenths. */
#define STAT_SCALE_ONE	   10

/* What an effect of an equipped item does (EQUIP_EFFECT.kind); the Item screen
   names each by MsgEquipEffectName + kind ("Max HP", "Luck"). */
typedef enum EQUIP_EFFECT_KIND {
	EQUIP_EFFECT_NONE,
	EQUIP_EFFECT_ADD_HP,
	EQUIP_EFFECT_ADD_HP_REGEN,
	EQUIP_EFFECT_ADD_PP,
	EQUIP_EFFECT_ADD_PP_REGEN,
	EQUIP_EFFECT_ADD_AGILITY,
	EQUIP_EFFECT_ADD_LUCK,
	EQUIP_EFFECT_SCALE_HP,
	EQUIP_EFFECT_SCALE_HP_REGEN,
	EQUIP_EFFECT_SCALE_PP,
	EQUIP_EFFECT_SCALE_PP_REGEN,
	EQUIP_EFFECT_SCALE_ATTACK,
	EQUIP_EFFECT_SCALE_DEFENSE,
	EQUIP_EFFECT_SCALE_AGILITY,
	EQUIP_EFFECT_SCALE_LUCK,
	/* Element power and resistance, in the element order Venus, Mercury, Mars,
	   Jupiter. */
	EQUIP_EFFECT_ADD_VENUS_POWER,
	EQUIP_EFFECT_ADD_MERCURY_POWER,
	EQUIP_EFFECT_ADD_MARS_POWER,
	EQUIP_EFFECT_ADD_JUPITER_POWER,
	EQUIP_EFFECT_ADD_VENUS_RESIST,
	EQUIP_EFFECT_ADD_MERCURY_RESIST,
	EQUIP_EFFECT_ADD_MARS_RESIST,
	EQUIP_EFFECT_ADD_JUPITER_RESIST,
	/* "Critical Hits increase": each point adds a percent to the weapon's
	   unleash chance and half a percent to a plain attack's chance of a
	   critical hit. */
	EQUIP_EFFECT_ADD_UNLEASH_CHANCE,
	/* Summed by gmUnitCalc into equipBonus[1]; no item of item_table.c has it,
	   the item menu draws no line for it, and nothing reads the total. */
	EQUIP_EFFECT_ADD_COUNTERSTRIKE_RATE,
	/* "The curse can be removed": sets CURSE_REMOVABLE, which lifts
	   CURSE_BOUND. */
	EQUIP_EFFECT_CURSE_REMOVABLE,
	EQUIP_EFFECT_ADD_ACTIONS, /* more actions each turn */
	/* sets FLAG_LURE_CAP while worn: encounters come faster */
	EQUIP_EFFECT_LURE
} EQUIP_EFFECT_KIND;

/* One effect of an item while equipped: its kind (EQUIP_EFFECT_KIND) and
   amount. */
typedef struct EQUIP_EFFECT {
	Uint8 kind;	  /* EQUIP_EFFECT_KIND; 0 for none */
	Sint8 amount; /* what the kind adds or scales by */
	Uint8 padding[2];
} EQUIP_EFFECT;

STATIC_ASSERT(sizeof(EQUIP_EFFECT) == 4);
STATIC_ASSERT(offsetof(EQUIP_EFFECT, kind) == 0);
STATIC_ASSERT(offsetof(EQUIP_EFFECT, amount) == 1);

/* The equipMask bit of `character` (ISAAC to PIERS): gmItemCanUnitEquipDirect
   reads bit unit->id of the mask. */
#define EQUIP_CHARACTER(character) (1 << (character))
/* Every one of the eight characters' bits. */
#define EQUIP_ALL_CHARACTERS	   0xff

/* An item's definition: its price, type, who can equip it, its bonuses and
   unleash, its effects and the action it is used for. */
typedef struct ITEM_DEFINITION {
	Sint16 price;	  /* coins */
	Uint8  type;	  /* ITEM_TYPE; 0 for an item that is not equipment */
	Uint8  flg;		  /* ITEM_FLAGS */
	Uint16 equipMask; /* one bit per character that can equip it, bit 0 Isaac */
	Uint16 icon;	  /* indexes ItemIcons */
	Sint16 attack;	  /* Attack added while equipped */
	Sint8  defense;	  /* Defense added while equipped */
	Uint8  unleashChance;  /* in steps of 5 percent */
	Uint8  useType;		   /* what using it costs (an ITEM_USE_ value) */
	Uint16 unleashAbility; /* the action a weapon unleashes, or 0 */
	Uint8  padding[4];
	long   element; /* a weapon's attack element; ELEMENT_NONE for none */
	/* What it does while equipped. */
	EQUIP_EFFECT effs[EQUIP_EFFECT_SLOTS];
	Uint16		 abilityId; /* the ability used, or taught as Psynergy */
} ITEM_DEFINITION;

STATIC_ASSERT(sizeof(ITEM_DEFINITION) == 44);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, price) == 0);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, type) == 2);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, flg) == 3);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, equipMask) == 4);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, icon) == 6);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, attack) == 8);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, defense) == 10);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, unleashChance) == 11);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, useType) == 12);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, unleashAbility) == 14);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, element) == 20);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, effs) == 24);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, abilityId) == 40);

#endif
