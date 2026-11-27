#ifndef ALCHEMY_WEAPON_KIND_H
#define ALCHEMY_WEAPON_KIND_H
/* @file */
/* The kinds of weapon a party member's attack shows, and how */
/* BattleUnit_WeaponKinds packs one with its item. */

/* BattleUnit_WeaponKinds: a weapon's item id with its kind above it. */
#define WEAPON_ITEM_MASK  0x1ff
#define WEAPON_KIND_SHIFT 9

/* The kinds of weapon a party member's attack shows, by the rows of */
/* BattleUnit_WeaponKinds lists for each. */
typedef enum BATTLE_WEAPON_KIND {
	WEAPON_KIND_LONG_SWORD,	 /* Long Sword to Muramasa */
	WEAPON_KIND_LIGHT_BLADE, /* Machete to Bandit's Sword */
	WEAPON_KIND_AXE,		 /* Battle Axe to Demon Axe */
	WEAPON_KIND_MACE,		 /* Mace to Wicked Mace */
	/* Wooden Stick to Shaman's Rod, but the two ankhs among them. */
	WEAPON_KIND_STAFF,
	WEAPON_KIND_ANKH, /* the Blessed Ankh and Angelic Ankh */
	WEAPON_KIND_NONE  /* an item the list lacks */
} BATTLE_WEAPON_KIND;

/* A row of BattleUnit_WeaponKinds: the item and its kind. */
#define WEAPON(item, kind) ((item) | (kind) << WEAPON_KIND_SHIFT)

#endif
