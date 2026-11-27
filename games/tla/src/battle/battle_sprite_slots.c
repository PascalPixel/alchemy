/* Battle sprites: every unit's sprite placed, enemy sprite slots and the
   party's weapon sprites. Part of battle.c. */

#include "field/stubs.h"

/* Readies every unit's sprites, destroying those of units no longer in the
   battle, and sets each unit's pose again. */
void btObjectPut(void)
{
	Sint16 list[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, list);
	btActorSpawnObjectsForList(list, 1);
}

/* Readies every unit's sprites, destroying those of units no longer in the
   battle, and leaves the poses of units that have sprites as they are. */
void btObjectPutKeepingPoses(void)
{
	Sint16 list[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, list);
	btActorSpawnObjectsForList(list, 0);
}

/* Returns the palette of a unit's battle object. */
Sint32 btMotionGetSlotPalette(Sint32 id)
{
	return btObjectGet(id)->palette;
}

/* The battle has six enemy sprite slots; a large enemy takes two side by side,
   a small one one. */
#define ENEMY_SPRITE_SLOTS 6

/* Returns 1 if an enemy of the class would find room among the enemy sprite
   slots, two side by side for a large one. */
Sint32 btEnemyHasSpriteRoom(Sint32 classId)
{
	BATTLE_STATE *battle;
	Sint32		  needsSingleSlot;
	Sint32		  spriteSlot;

	needsSingleSlot = btSpriteIsSmall(classId);
	battle = RamHeapSlots->battleState;
	for(spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
		if(battle->spriteSlots[spriteSlot] != 0)
			continue;
		if(needsSingleSlot != 0)
			break;
		if(spriteSlot < ENEMY_SPRITE_SLOTS - 1 &&
			battle->spriteSlots[spriteSlot + 1] == 0)
			break;
	}
	return spriteSlot != ENEMY_SPRITE_SLOTS;
}

/* Each enemy sprite slot owns 16 KB of the sprite buffers, ActorSpriteSlots. */
#define ENEMY_SPRITE_SLOT_SIZE 16384

/* Loads an enemy's battle sprite into the first free enemy sprite slots: one
   slot, or two neighbouring ones for a large enemy. A few sprites load a second
   part into the next free slots. Returns (slot << RESOURCE_SLOT_SHIFT) |
   sprite, or 0 when a load fails. */
Sint32 btEnemySpriteLoadIntoSlots(Sint32 unitId)
{
	Sint32		  spritePart;
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	UNIT		 *unit = syUnitGet(unitId);
	Sint32		  needsSingleSlot = btSpriteIsSmall(unit->enemyId);
	Sint32		  result;
	Sint32		  sprite;
	Uint8		 *spriteBuffers;

	result = spritePart = 0;
	sprite = btSpriteGetSprite(unit->enemyId);
	/* Once for the sprite, and again for its second part if it has one. */
	for(spritePart = 0; spritePart <= 1; spritePart++) {
		Sint32 slot;

		/* A unit with a class is no enemy: nothing is loaded and the result
		   stays 0. */
		if(unit->classId != CLASS_NONE)
			continue;

		/* The first free slot (and, for a large enemy, the one after it). */
		for(slot = 0; slot < ENEMY_SPRITE_SLOTS; slot++) {
			if(battle->spriteSlots[slot] != 0)
				continue;
			if(needsSingleSlot)
				break;
			if(slot < ENEMY_SPRITE_SLOTS - 1 &&
				battle->spriteSlots[slot + 1] == 0)
				break;
		}

		if(slot == ENEMY_SPRITE_SLOTS)
			break;

		spriteBuffers = ActorSpriteSlots;
		{
			Sint32 palette = btSpriteGetPalette(unit->enemyId);
			Uint8 *spriteBuf = spriteBuffers + slot * ENEMY_SPRITE_SLOT_SIZE;

			if(fdSpritePictureCacheLoadStub(slot, (Uint32 *)spriteBuf,
				   sprite + spritePart, palette) == 0)
				return 0;
		}

		if(spritePart == 0)
			result = (slot << RESOURCE_SLOT_SHIFT) | sprite;

		battle->spriteSlots[slot] = unitId;
		if(!needsSingleSlot)
			battle->spriteSlots[slot + 1] = unitId;

		if(sprite != SPRITE_BATTLE_SERPENT && sprite != SPRITE_BATTLE_KRAKEN &&
			sprite != SPRITE_BATTLE_FUSION_DRAGON &&
			sprite != SPRITE_BATTLE_POSEIDON &&
			sprite != SPRITE_BATTLE_DOOM_DRAGON_A &&
			sprite != SPRITE_BATTLE_DOOM_DRAGON_E &&
			sprite != SPRITE_BATTLE_DOOM_DRAGON_H)
			break;
	}

	return result;
}

/* Frees every enemy sprite slot unit unitId holds, unloading its sprite. Clears
   those BATTLE_STATE.spriteSlots. */
void btObjectReleaseSpriteSlots(Sint32 unitId)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  spriteSlot;

	for(spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
		if(battle->spriteSlots[spriteSlot] == unitId) {
			fdSpritePictureCacheLoadStub(spriteSlot, 0, 0, 0);
			battle->spriteSlots[spriteSlot] = 0;
		}
	}
}

/* Returns the kind (BATTLE_WEAPON_KIND) of the weapon whose item id is itemId,
   which picks a party member's weapon sprite. */
Sint32 btUnitGetWeaponKind(Sint32 itemId)
{
	Sint32 row;
	Uint32 assignment;
	Sint32 weaponKind;

	for(row = 0;; row++) {
		if(itemId == (BattleUnitWeaponKinds[row] & WEAPON_ITEM_MASK)) {
			assignment = BattleUnitWeaponKinds[row];
			weaponKind = assignment >> WEAPON_KIND_SHIFT;
			break;
		}
		if((Sint16)BattleUnitWeaponKinds[row] == -1) {
			weaponKind = WEAPON_KIND_NONE;
			break;
		}
	}
	return weaponKind;
}

/* Returns the battle sprite of a character holding its equipped weapon, from
   that character's table by the weapon's kind, or 0 with no weapon. */
Sint32 btUnitGetWeaponSprite(Sint32 owner)
{
	UNIT  *unit = syUnitGet(owner);
	Sint32 slot = gmInventoryFindEquippedStub(owner, EQUIP_WEAPON);
	Sint32 result = 0;

	if(slot >= 0) {
		Sint32 kind =
			btUnitGetWeaponKind(unit->inventory[slot] & INVENTORY_ITEM_ID_MASK);
		switch(unit->enemyId) {
		case ISAAC:
			result = BattleUnitWeaponAnimationsIsaac[kind];
			break;
		case GARET:
			result = BattleUnitWeaponAnimationsGaret[kind];
			break;
		case IVAN:
			result = BattleUnitWeaponAnimationsIvan[kind];
			break;
		case MIA:
			result = BattleUnitWeaponAnimationsMia[kind];
			break;
		case JENNA:
			result = BattleUnitWeaponAnimationsJenna[kind];
			break;
		case FELIX:
			result = BattleUnitWeaponAnimationsFelix[kind];
			break;
		case SHEBA:
			result = BattleUnitWeaponAnimationsSheba[kind];
			break;
		case PIERS:
			result = BattleUnitWeaponAnimationsPiers[kind];
			break;
		}
	}
	return result;
}
