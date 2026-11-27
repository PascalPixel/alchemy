/* @file */
/* Battle sprites: every unit's sprite placed, enemy sprite slots and the */
/* party's weapon sprites. */
/* Part of battle.c. */

#include "field/stubs.h"

/* Readies every unit's sprites, destroying those of units no longer in the */
/* battle, and sets each unit's pose again. */
void btObjectPut(void) {
	s16 list[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, list);
	btActorSpawnObjectsForList(list, 1);
}

/* Readies every unit's sprites, destroying those of units no longer in the */
/* battle, and leaves the poses of units that have sprites as they are. */
void btObjectPutKeepingPoses(void) {
	s16 list[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, list);
	btActorSpawnObjectsForList(list, 0);
}

/* Returns the palette of a unit's battle object. */
s32 btMotionGetSlotPalette(s32 id) { return btObjectGet(id)->pal; }

/* The battle has six enemy sprite slots; a large enemy takes two side by */
/* side, a small one one. */
#define ENEMY_SPRITE_SLOTS 6

/* Returns 1 if an enemy of the class would find room among the enemy sprite */
/* slots, two side by side for a large one. */
s32 btEnemyHasSprRoom(s32 classId) {
	BATTLE_STATE *battle;
	s32 needsSingleSlot;
	s32 spriteSlot;

	needsSingleSlot = btSprIsSmall(classId);
	battle = Ram_HeapSlots->battleState;
	for(spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
		if(battle->sprSlots[spriteSlot] != 0)
			continue;
		if(needsSingleSlot != 0)
			break;
		if(spriteSlot < ENEMY_SPRITE_SLOTS - 1 &&
		   battle->sprSlots[spriteSlot + 1] == 0)
			break;
	}
	return spriteSlot != ENEMY_SPRITE_SLOTS;
}

/* Each enemy sprite slot owns 16 KB of the sprite buffers, ActorSpriteSlots. */
#define ENEMY_SPRITE_SLOT_SIZE 16384

/* Loads an enemy's battle sprite into the first free enemy sprite slots: one */
/* slot, or two neighbouring ones for a large enemy. A few sprites load a */
/* second part into the next free slots. Returns (slot << RESOURCE_SLOT_SHIFT)
 */
/* | sprite, or 0 when a load fails. */
s32 btEnemySprLoadIntoSlots(s32 unitId) {
	s32 spritePart;
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	UNIT *unit = syUnitGet(unitId);
	s32 needsSingleSlot = btSprIsSmall(unit->enemyId);
	s32 result;
	s32 sprite;
	u8 *spriteBuffers;

	result = spritePart = 0;
	sprite = btSprGetSpr(unit->enemyId);
	/* Once for the sprite, and again for its second part if it has one. */
	for(spritePart = 0; spritePart <= 1; spritePart++) {
		s32 slot;

		/* A unit with a class is no enemy: nothing is loaded and the result
		 * stays */
		/* 0. */
		if(unit->classId != CLASS_NONE)
			continue;

		/* The first free slot (and, for a large enemy, the one after it). */
		for(slot = 0; slot < ENEMY_SPRITE_SLOTS; slot++) {
			if(battle->sprSlots[slot] != 0)
				continue;
			if(needsSingleSlot)
				break;
			if(slot < ENEMY_SPRITE_SLOTS - 1 && battle->sprSlots[slot + 1] == 0)
				break;
		}

		if(slot == ENEMY_SPRITE_SLOTS)
			break;

		spriteBuffers = ActorSpriteSlots;
		{
			s32 palette = btSprGetPalette(unit->enemyId);
			u8 *spriteBuffer = spriteBuffers + slot * ENEMY_SPRITE_SLOT_SIZE;

			if(fdSprPictureCacheLoadStub(slot, (u32 *)spriteBuffer,
										 sprite + spritePart, palette) == 0)
				return 0;
		}

		if(spritePart == 0)
			result = (slot << RESOURCE_SLOT_SHIFT) | sprite;

		battle->sprSlots[slot] = unitId;
		if(!needsSingleSlot)
			battle->sprSlots[slot + 1] = unitId;

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

/* Frees every enemy sprite slot unit unitId holds, unloading its sprite. */
/* Clears those BATTLE_STATE.sprSlots. */
void btObjectReleaseSprSlots(s32 unitId) {
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	s32 spriteSlot;

	for(spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
		if(battle->sprSlots[spriteSlot] == unitId) {
			fdSprPictureCacheLoadStub(spriteSlot, 0, 0, 0);
			battle->sprSlots[spriteSlot] = 0;
		}
	}
}

/* Returns the kind (BATTLE_WEAPON_KIND) of the weapon whose item id is */
/* itemId, which picks a party member's weapon sprite. */
s32 btUnitGetWeaponKind(s32 itemId) {
	s32 row;
	u32 assignment;
	s32 weaponKind;

	for(row = 0;; row++) {
		if(itemId == (BattleUnit_WeaponKinds[row] & WEAPON_ITEM_MASK)) {
			assignment = BattleUnit_WeaponKinds[row];
			weaponKind = assignment >> WEAPON_KIND_SHIFT;
			break;
		}
		if((s16)BattleUnit_WeaponKinds[row] == -1) {
			weaponKind = WEAPON_KIND_NONE;
			break;
		}
	}
	return weaponKind;
}

/* Returns the battle sprite of a character holding its equipped weapon, from */
/* that character's table by the weapon's kind, or 0 with no weapon. */
s32 btUnitGetWeaponSpr(s32 owner) {
	UNIT *unit = syUnitGet(owner);
	s32 slot = gmInventoryFindEquippedStub(owner, EQUIP_WEAPON);
	s32 result = 0;

	if(slot >= 0) {
		s32 kind =
			btUnitGetWeaponKind(unit->inventory[slot] & INVENTORY_ITEM_ID_MASK);
		switch(unit->enemyId) {
		case ISAAC:
			result = BattleUnit_WeaponAnimationsIsaac[kind];
			break;
		case GARET:
			result = BattleUnit_WeaponAnimationsGaret[kind];
			break;
		case IVAN:
			result = BattleUnit_WeaponAnimationsIvan[kind];
			break;
		case MIA:
			result = BattleUnit_WeaponAnimationsMia[kind];
			break;
		case JENNA:
			result = BattleUnit_WeaponAnimationsJenna[kind];
			break;
		case FELIX:
			result = BattleUnit_WeaponAnimationsFelix[kind];
			break;
		case SHEBA:
			result = BattleUnit_WeaponAnimationsSheba[kind];
			break;
		case PIERS:
			result = BattleUnit_WeaponAnimationsPiers[kind];
			break;
		}
	}
	return result;
}
