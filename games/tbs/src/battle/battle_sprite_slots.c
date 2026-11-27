// Battle sprites: enemy sprite slots and the party's weapon sprites. Part of battle.c.

#include "battle_internal.h"

// Readies the sprite of every unit in the battle (releasing those of units that left) and sets each
// one's display mode and motion afresh.
void btObjectPut(void)
{
	Sint16 unitIds[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	btObjectSpawnObjectsForList(unitIds, 1);
}

// Readies the sprite of every unit in the battle (releasing those of units that left), keeping
// their modes and motions.
void btObjectPutKeepingPoses(void)
{
	Sint16 unitIds[UNIT_MAX];

	btUnitListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
	btObjectSpawnObjectsForList(unitIds, 0);
}

// Returns the palette of unit id's battle object.
Sint32 btObjectGetPalette(Sint32 id)
{
	return btObjectGet(id)->palette;
}

// Each enemy sprite slot owns 16 KB of RamEnemySpriteSlots.
#define ENEMY_SPRITE_SLOT_SIZE (16 * 1024)

// Returns 1 if the enemy would find room among the enemy sprite slots, two side by side for a large
// one.
Sint32 btEnemyHasSpriteRoom(Sint32 enemyId)
{
	BATTLE_STATE *battle;
	Sint32        needsSingleSlot;
	Sint32        spriteSlot;

	needsSingleSlot = btSpriteIsSmall(enemyId);
	battle          = BattleStateWork;
	for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++)
	{
		if (battle->spriteSlots[spriteSlot] != 0)
			continue;
		if (needsSingleSlot != 0)
			break;
		if (spriteSlot < ENEMY_SPRITE_SLOTS - 1 && battle->spriteSlots[spriteSlot + 1] == 0)
			break;
	}
	return spriteSlot != ENEMY_SPRITE_SLOTS;
}

// Loads an enemy's battle sprite into the first free enemy sprite slots, the Kraken's and the
// Fusion Dragon's second part too. Returns the sprite with its first slot (SPRITE_PICTURE_TAG), or
// 0.
Sint32 btEnemySpriteLoadIntoSlots(Sint32 unitId)
{
	Sint32        spritePart;
	BATTLE_STATE *battle          = BattleStateWork;
	UNIT         *unit            = gmUnitGetStub(unitId);
	Sint32        needsSingleSlot = btSpriteIsSmall(unit->id);
	Sint32        result;
	Sint32        sprite;

	result = spritePart = 0;
	sprite              = btSpriteGetSprite(unit->id);
	// Once for the sprite, and again for its second part if it has one.
	for (spritePart = 0; spritePart <= 1; spritePart++)
	{
		Sint32 slot;

		// A unit with a class is a character, no enemy: nothing is loaded and the result stays 0.
		if (unit->classId != CLASS_NONE)
			continue;

		// The first free slot (and, for a large enemy, the one after it).
		for (slot = 0; slot < ENEMY_SPRITE_SLOTS; slot++)
		{
			if (battle->spriteSlots[slot] != 0)
				continue;
			if (needsSingleSlot)
				break;
			if (slot < ENEMY_SPRITE_SLOTS - 1 && battle->spriteSlots[slot + 1] == 0)
				break;
		}

		if (slot == ENEMY_SPRITE_SLOTS)
			break;

		{
			Sint32 palette   = btSpriteGetPalette(unit->id);
			Uint8 *spriteBuf = RamEnemySpriteSlots + slot * ENEMY_SPRITE_SLOT_SIZE;

			if (fdSpritePictureCacheLoadStub(
					slot, (Uint32 *)spriteBuf, sprite + spritePart, palette)
			    == 0)
				return 0;
		}

		if (spritePart == 0)
			result = (slot << PICTURE_CACHE_TAG_SLOT_SHIFT) | sprite;

		battle->spriteSlots[slot] = unitId;
		if (!needsSingleSlot)
			battle->spriteSlots[slot + 1] = unitId;

		// Only the Kraken and the Fusion Dragon are drawn in two parts, the second the sprite after
		// the first.
		if (sprite != SPRITE_BATTLE_KRAKEN && sprite != SPRITE_BATTLE_FUSION_DRAGON)
			break;
	}

	return result;
}

// Frees every enemy sprite slot unit unitId holds, unloading its sprite. Clears those
// BattleStateWork->spriteSlots.
UNREAD_RESULT btObjectReleaseSpriteSlots(Sint32 unitId)
{
	BATTLE_STATE *battle = BattleStateWork;
	Sint32        spriteSlot;

	for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++)
	{
		if (battle->spriteSlots[spriteSlot] == unitId)
		{
			fdSpritePictureCacheLoadStub(spriteSlot, 0, 0, 0);
			battle->spriteSlots[spriteSlot] = 0;
		}
	}
}

// Returns the kind (BATTLE_WEAPON_KIND) of the weapon whose item id is itemId, which picks a party
// member's weapon sprite.
Sint32 btUnitGetWeaponKind(Sint32 itemId)
{
	Sint32 assignmentIndex;
	Uint32 assignment;
	Sint32 weaponKind;

	for (assignmentIndex = 0;; assignmentIndex++)
	{
		if (itemId == (BattleUnitWeaponKinds[assignmentIndex] & WEAPON_ITEM_MASK))
		{
			assignment = BattleUnitWeaponKinds[assignmentIndex];
			weaponKind = assignment >> WEAPON_KIND_SHIFT;
			break;
		}
		if ((Sint16)BattleUnitWeaponKinds[assignmentIndex] == -1)
		{
			weaponKind = WEAPON_KIND_NONE;
			break;
		}
	}
	return weaponKind;
}

// Returns the weapon sprite a party member's attack shows for the weapon they have equipped, or 0.
Sint32 btUnitGetWeaponSprite(Sint32 unitId)
{
	UNIT  *unit;
	Sint32 slot;
	Sint32 weaponSprite;

	unit         = gmUnitGetStub(unitId);
	slot         = gmInventoryFindEquippedStub(unitId, EQUIP_WEAPON);
	weaponSprite = 0;
	if (slot >= 0)
	{
		Sint32 weaponKind;

		weaponKind = btUnitGetWeaponKind(unit->inventory[slot] & WEAPON_ITEM_MASK);
		switch (unit->id)
		{
			case ISAAC: weaponSprite = BattleUnitWeaponAnimationsIsaac[weaponKind]; break;
			case GARET: weaponSprite = BattleUnitWeaponAnimationsGaret[weaponKind]; break;
			case IVAN : weaponSprite = BattleUnitWeaponAnimationsIvan[weaponKind]; break;
			case MIA  : weaponSprite = BattleUnitWeaponAnimationsMia[weaponKind]; break;
			case FELIX: break;
			case JENNA: weaponSprite = BattleUnitWeaponAnimationsJenna[weaponKind]; break;
		}
	}
	return weaponSprite;
}

#undef ENEMY_SPRITE_SLOT_SIZE
