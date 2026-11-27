#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

/* Items: their definitions, types, flags and effects. */

#include "static_assert.h"
#include "edition.h"

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

/* The equipMask bit of character `no` (ISAAC to PIERS):
   gmItemCanUnitEquipDirect reads the bit of the unit's character. */
#define EQUIP_CHARACTER(no)	 (1 << (no))
/* Every one of the eight characters' bits. */
#define EQUIP_ALL_CHARACTERS 0xff

/* The bits of ITEM_DEFINITION.flags. */
typedef enum ITEM_FLAGS {
	ITEM_CURSED = 0x01, /* equipping it curses the wearer (UNIT.curse) */
	/* Once equipped it cannot be taken off, or dropped while worn. */
	ITEM_CANNOT_UNEQUIP = 0x02,
	/* A rare item: the rare weapons and armour, keys and the other
	   one-of-a-kind items carry it. */
	ITEM_RARE = 0x04,
	/* An important item: never dropped, and worth nothing to a shop. */
	ITEM_INDISPENSABLE = 0x08,
	ITEM_STACKABLE = 0x10, /* kept in one inventory entry with a quantity */
	/* Left behind when The Broken Seal's data is transferred
	   (gmDataTransferApply): the Elemental Stars and their bags, and that
	   game's errands, from the Healing Fungus to the Lil' Turtle. */
	ITEM_NOT_TRANSFERRED = 0x20
} ITEM_FLAGS;

/* What using an item costs (ITEM_DEFINITION's useType). */
#define ITEM_USE_FREE	   0 /* nothing: it is kept, as equipment is */
#define ITEM_USE_CONSUMED  1 /* one is used up */
#define ITEM_USE_MAY_BREAK 2 /* may break when used */
/* Not used up: equipped, it teaches its actionId as Psynergy. */
#define ITEM_USE_TEACHES   3
#define ITEM_USE_EMPTIES   4 /* Hermes' Water leaves an Empty Bottle */

/* What gmItemGetEquipmentGroup sorts an item into. */
typedef enum EQUIPMENT_GROUP {
	/* anything not a weapon or armor, rings included */
	EQUIPMENT_GROUP_NONE = 0,
	EQUIPMENT_GROUP_WEAPON = 1,
	EQUIPMENT_GROUP_ARMOR = 2
} EQUIPMENT_GROUP;

/* Why gmInventoryCheckDiscard or gmInventoryCheckUnequip refuses an item, as
   bits; 0 when it may go. */
typedef enum INVENTORY_REFUSAL {
	/* Equipped and ITEM_CANNOT_UNEQUIP: "You can't remove it!" */
	INVENTORY_REFUSE_CANNOT_UNEQUIP = 1,
	/* ITEM_INDISPENSABLE: "You can't drop it!" */
	INVENTORY_REFUSE_INDISPENSABLE = 2,
	INVENTORY_REFUSE_EMPTY = 8 /* the slot holds no item */
} INVENTORY_REFUSAL;

/* An item's type (ITEM_DEFINITION's type, item_table.c's type field). A unit
   equips one item of each type, except Psynergy items. */
typedef enum ITEM_TYPE {
	/* Not equipment: Herb, the keys, unused rows. */
	ITEM_TYPE_NONE = 0,
	ITEM_TYPE_WEAPON = 1, /* swords, axes, maces, rods and staffs */
	ITEM_TYPE_ARMOR = 2,  /* armor, clothes and robes */
	ITEM_TYPE_SHIELD = 3, /* shields, gloves and armlets */
	ITEM_TYPE_HELM = 4,	  /* helms, caps, crowns and circlets */
	ITEM_TYPE_BOOTS = 5,  /* Hyper Boots to Turtle Boots and the greaves */
	/* The items that teach Psynergy while held: Orb of Force to Teleport
	   Lapis. */
	ITEM_TYPE_PSYNERGY = 6,
	/* The Trident alone: "A weapon with a unique purpose". */
	ITEM_TYPE_TRIDENT = 7,
	ITEM_TYPE_RING = 8,	 /* Adept Ring to War Ring */
	ITEM_TYPE_SHIRT = 9, /* Mythril Shirt to Divine Camisole */
	/* Mysterious Card, Trainer's Whip and Tomegathericon: "Equip to change your
	   class". */
	ITEM_TYPE_CLASS_CHANGE = 10,
	/* The four Elemental Stars and the Mythril Bags that hold them; the item
	   menu treats them as it does items that are not equipment. */
	ITEM_TYPE_ELEMENTAL_STAR = 11
} ITEM_TYPE;

/* How many effects an item has while equipped (ITEM_DEFINITION.effects). */
#define EQUIP_EFFECT_SLOTS 4
/* A multiplier of 10 tenths, which keeps a stat as it is: the classes' stats
   (CLASS_DEFINITION) and the EQUIP_EFFECT_SCALE_ effects are in tenths. */
#define STAT_SCALE_ONE	   10

/* What an equipped item does for its wearer (ItemEquipEffect.kind): adds to a
   stat or to an element's power or resistance, scales a stat in tenths, or one
   of a few effects of its own. */
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
	EQUIP_EFFECT_ADD_UNLEASH_CHANCE, /* percent added to the weapon's unleash */
	/* Nothing sums or reads it: gmUnitCalculate has no case for it. */
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
	Uint8 kind;		  /* EQUIP_EFFECT_KIND; 0 for none */
	Sint8 amount;	  /* what the kind adds or scales by */
	Uint8 padding[2]; /* 0 in every row */
} EQUIP_EFFECT;

STATIC_ASSERT(sizeof(EQUIP_EFFECT) == 4);
STATIC_ASSERT(offsetof(EQUIP_EFFECT, kind) == 0);
STATIC_ASSERT(offsetof(EQUIP_EFFECT, amount) == 1);

/* An item's definition: its price, type, who can equip it, its bonuses and
   unleash, its effects and the action it is used for. */
typedef struct ITEM_DEFINITION {
	Uint16 price;	  /* coins */
	Uint8  type;	  /* ITEM_TYPE; 0 for an item that is not equipment */
	Uint8  flags;	  /* ITEM_FLAGS */
	Uint16 equipMask; /* one bit per character that can equip it, bit 0 Isaac */
	Uint16 icon;	  /* indexes ItemIcons */
	Sint16 attack;	  /* Attack added while equipped */
	Sint8  defense;	  /* Defense added while equipped */
	Uint8  unleashChance;  /* in steps of 5 percent */
	Uint8  useType;		   /* what using it costs (an ITEM_USE_ value) */
	Uint16 unleashAbility; /* the action a weapon unleashes, or 0 */
	Uint8  padding[4];	   /* 0 in every row */
	long   element; /* a weapon's attack element; ELEMENT_NONE for none */
	EQUIP_EFFECT
	effects[EQUIP_EFFECT_SLOTS]; /* what it does while equipped */
	Uint16 abilityId;			 /* the ability used, or taught as Psynergy */
} ITEM_DEFINITION;

STATIC_ASSERT(sizeof(ITEM_DEFINITION) == 44);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, price) == 0);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, type) == 2);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, flags) == 3);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, equipMask) == 4);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, icon) == 6);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, attack) == 8);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, defense) == 10);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, unleashChance) == 11);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, useType) == 12);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, unleashAbility) == 14);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, element) == 20);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, effects) == 24);
STATIC_ASSERT(offsetof(ITEM_DEFINITION, abilityId) == 40);

/* Returns an item's definition. */
ITEM_DEFINITION *gmItemGetDirectStub(Sint32 item);

/* Rolls whether a character's weapon unleashes, 5 percent a point of its
   chance. */
Sint32 gmEquipmentRollUnleashStub(struct UNIT *owner);
/* Returns whether the character's class may equip the item, 0 for any unit past
   the eight characters (gmItemCanUnitEquipDirect). */
Sint32 gmItemCanUnitEquipDirectStub(Sint32 unit, Sint32 item);
Sint32 gmRareItemStockAdjust(Sint32 counter, Sint32 delta);

#endif
