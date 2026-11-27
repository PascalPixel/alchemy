/* Battle objects made and placed on the battlefield. Part of battle.c. */

#include "battle_internal.h"

/* The fdActorCreate kind a battle object's actor is made with: one that makes
   no sprite, its battle sprites being attached afterwards. */
#define BATTLE_OBJECT_ACTOR_KIND 15

/* Creates the battle object of a unit at (x, y) on the battlefield: its sprite,
   palette, size and weapon, turned to face the centre. */
void btObjectMake(
	BATTLE_OBJECT *battleObject, Sint32 unitId, Sint32 x, Sint32 y)
{
	FIXED  fixedX;
	FIXED  fixedZ;
	Sint32 palette;
	Sint32 spr;
	Sint32 registeredSpr;
	UNIT  *unit;
	ACTOR *actor;
	Sint32 facingAng;
	Sint32 weaponSpr;
	Uint8  characterId;

	/* Positions are 16.16 fixed point; y is the battlefield's depth. */
	fixedX = toFIXED(x);
	fixedZ = toFIXED(y);
	actor = fdActorCreateStub(
		ACTOR_CREATE_KIND(BATTLE_OBJECT_ACTOR_KIND), fixedX, 0, fixedZ);
	unit = gmUnitGetStub(unitId);
	palette = 0;
	registeredSpr = btEnemySprLoadIntoSlots(unitId);

	/* The sprite: an enemy's from its battle sprite row, a party member's by
	   character; a character on the enemy side, a link opponent, takes palette
	   1. */
	if(unit->classId == CLASS_NONE) {
		spr = btSprGetSpr(unit->id);
		if(registeredSpr == 0)
			palette = btSprGetPalette(unit->id);
		else
			spr = registeredSpr;
	} else {
		switch(unit->id) {
		case GARET:
			spr = SPRITE_BATTLE_GARET;
			break;
		case MIA:
			spr = SPRITE_BATTLE_MIA;
			break;
		case IVAN:
			spr = SPRITE_BATTLE_IVAN;
			break;
		case JENNA:
			spr = SPRITE_BATTLE_JENNA;
			break;
		case ISAAC:
		default:
			spr = SPRITE_BATTLE_ISAAC;
			break;
		}
		if(unitId < 0 || unitId > ALLY_MAX)
			palette = 1;
	}

	/* The enemies drawn larger or smaller than their sprites, in 16.16. Bandit
	   and Orc Captain are set to 1.0 again. */
	battleObject->scale = toFIXED(1);
	switch(unit->id) {
	case ENEMY_KILLER_APE:
		battleObject->scale = toFIXED(8) / 5;
		break;
	case ENEMY_MANTICORE:
		battleObject->scale = toFIXED(31) / 20;
		break;
	case ENEMY_LIVING_STATUE:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_HYDROS_STATUE:
		battleObject->scale = toFIXED(8) / 5;
		break;
	case ENEMY_ANT_LION:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_TRET:
		battleObject->scale = toFIXED(9) / 5;
		break;
	case ENEMY_DEADBEARD:
		battleObject->scale = toFIXED(9) / 5;
		break;
	case ENEMY_BANDIT:
		battleObject->scale = toFIXED(1);
		break;
	case ENEMY_KRAKEN:
		battleObject->scale = toFIXED(17) / 10;
		break;
	case ENEMY_FUSION_DRAGON:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_MERCURY_LIGHTHOUSE:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_FUCHIN_FALLS_CAVE:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_ALTIN_PEAK:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_VAULT_CAVE:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_CROSSBONE_ISLE:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_ALTMILLER_CAVE:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_SUHALLA_DESERT:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_MIMIC_PIRATE_SHIP:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_TROLL:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_CAVE_TROLL:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_BRUTAL_TROLL:
		battleObject->scale = toFIXED(7) / 5;
		break;
	case ENEMY_ORC_CAPTAIN:
		battleObject->scale = toFIXED(1);
		break;
	case ENEMY_ORC_LORD:
		battleObject->scale = toFIXED(5) / 4;
		break;
	case ENEMY_LIZARD_FIGHTER:
		battleObject->scale = toFIXED(7) / 5;
		break;
	case ENEMY_LIZARD_KING:
		battleObject->scale = toFIXED(7) / 5;
		break;
	case ENEMY_HOBGOBLIN:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_GRYPHON:
		battleObject->scale = toFIXED(13) / 10;
		break;
	case ENEMY_CHIMERA:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_AZART_IRON_SHIELD:
		battleObject->scale = toFIXED(21) / 20;
		break;
	case ENEMY_AZART_PADDED_GLOVES:
		battleObject->scale = toFIXED(21) / 20;
		break;
	case ENEMY_SATRAGE_CHAIN_MAIL:
		battleObject->scale = toFIXED(11) / 10;
		break;
	case ENEMY_SATRAGE_COTTON_SHIRT:
		battleObject->scale = toFIXED(11) / 10;
		break;
	case ENEMY_NAVAMPA_CLAYMORE:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_NAVAMPA_HUNTERS_SWORD:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_TOADONPA:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_TORNADO_LIZARD:
		battleObject->scale = toFIXED(6) / 5;
		break;
	case ENEMY_STORM_LIZARD:
		battleObject->scale = toFIXED(3) / 2;
		break;
	case ENEMY_TEMPEST_LIZARD:
		battleObject->scale = toFIXED(31) / 20;
		break;
	case ENEMY_RECLUSE:
		battleObject->scale = toFIXED(5) / 4;
		break;
	case ENEMY_CEREBUS:
		battleObject->scale = toFIXED(5) / 4;
		break;
	case ENEMY_SLIME_SOL_SANCTUM:
	case ENEMY_SLIME_WORLD_MAP:
	case ENEMY_OOZE:
	case ENEMY_SLIME_BEAST:
	case ENEMY_THIEF:
	case ENEMY_SIREN:
	case ENEMY_SUCCUBUS:
	case ENEMY_NIGHTMARE:
	case ENEMY_KOBOLD:
	case ENEMY_MYSTERY_MAN:
	case ENEMY_SATUROS_MERCURY_LIGHTHOUSE:
	case ENEMY_SATUROS_VENUS_LIGHTHOUSE:
	case ENEMY_MYSTERY_WOMAN:
	case ENEMY_MENARDI:
		battleObject->scale = toFIXED(9) / 10;
		break;
	}

	battleObject->actor = actor;
	battleObject->anchorX = fixedX;
	battleObject->anchorZ = fixedZ;
	battleObject->pal = palette;
	battleObject->resource = spr;
	weaponSpr = btUnitGetWeaponSpr(unitId);
	battleObject->anm = 0;
	battleObject->anmEntry = 0;
	battleObject->shadowEntry = 0;
	battleObject->active = 0;
	battleObject->fading = 0;
	battleObject->shadow = SPRITE_BATTLE_SHADOW;
	characterId = unit->id;
	battleObject->overlay = weaponSpr;

	/* Isaac and Garet draw a Machete as part of their own sprite, with no
	   weapon overlay. */
	if(characterId <= GARET &&
		gmInventoryGetEquippedItemStub(unit, EQUIP_WEAPON) == ITEM_MACHETE) {
		if(unit->id == ISAAC) {
			spr = SPRITE_ISAAC_MACHETE;
			battleObject->resource = spr;
		} else {
			spr = SPRITE_GARET_MACHETE;
			battleObject->resource = spr;
		}
		battleObject->overlay = 0;
	}

	/* Face the centre of the battlefield; enemies are drawn at 1.3 times their
	   size. */
	facingAng = syAtan(y / 8, x) + DEGtoANG(180);
	actor->facing = facingAng;
	actor->collisionFlg = ACTOR_SOLID | ACTOR_PUSHABLE;
	actor->motionFlg = ACTOR_FALLS;
	if(unit->classId == CLASS_NONE) {
		actor->scaleX = toFIXED(13) / 10;
		actor->scaleY = toFIXED(13) / 10;
	} else {
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
	fdActorStartScriptStub(actor, BattleObjectEmptyScript);
}

#include "battle/placement_get_step_pair.c"

/* BATTLE_STATE.enemyIds of an enemy that left the battle, and of a place held
   for one: unit ids 254 and 255 in the list. */
#define IS_PLACEHOLDER_ID(id) ((Uint16)((id) - BATTLE_UNIT_REMOVED) <= 1)

/* The enemies' column, in pixels from the battlefield's centre, and the nearer
   place the Kraken and the Fusion Dragon would take. */
#define ENEMY_COLUMN_X			(-80)
#define ENEMY_COLUMN_NEAR_X		(-50)
/* Pixels of the column a large enemy, a small one and an empty place take. */
#define ENEMY_SPACING_LARGE		38
#define ENEMY_SPACING_SMALL		27
#define ENEMY_SPACING_EMPTY		25
/* The spacing the column's length is first estimated from for up to four
   enemies; five or six are estimated at a small enemy's. */
#define ESTIMATED_ENEMY_SPACING 30

/* Lays out count enemies in a column left of the centre, spaced by their size,
   from a rough estimate of the column's length. */
void btPlacementLayoutEnemies(
	Uint16 *unitIds, Sint32 cnt, Sint32 *xPositions, Sint32 *zPositions)
{
	Sint32 totalSpacing =
		cnt <= 4 ? ESTIMATED_ENEMY_SPACING : ENEMY_SPACING_SMALL;
	Sint32 z = ((cnt - 1) * totalSpacing) / 2;
	Sint32 idx;

	/* z starts half a line's estimated length up and steps down by half of each
	   enemy's spacing before it (none for the first) and half after it. */
	for(idx = 0; idx != cnt; idx++) {
		Sint32 spacing = 0;

		xPositions[idx] = ENEMY_COLUMN_X;
		if(idx != 0) {
			UNIT *unit;

			spacing = ENEMY_SPACING_EMPTY;
			if(!IS_PLACEHOLDER_ID(unitIds[idx])) {
				unit = gmUnitGetStub(unitIds[idx]);
				spacing = btSprIsSmall(unit->id) ? ENEMY_SPACING_SMALL
												 : ENEMY_SPACING_LARGE;
				/* The first place keeps ENEMY_COLUMN_X whatever its size. */
				if(unit->id == ENEMY_FUSION_DRAGON ||
					unit->id == ENEMY_KRAKEN) {
					xPositions[idx] = ENEMY_COLUMN_NEAR_X;
				}
			}
		}
		z -= spacing / 2;
		zPositions[idx] = z;
		spacing = ENEMY_SPACING_EMPTY;
		if(!IS_PLACEHOLDER_ID(unitIds[idx])) {
			UNIT *unit = gmUnitGetStub(unitIds[idx]);

			spacing = btSprIsSmall(unit->id) ? ENEMY_SPACING_SMALL
											 : ENEMY_SPACING_LARGE;
		}
		z -= spacing / 2;
	}
}

/* Returns the id of the first free enemy unit, or -1 when all six are taken. */
Sint32 btEnemyGetFreeUnitId(void)
{
	Sint32 enemySlot;
	Sint32 unitId;

	for(enemySlot = 0; enemySlot < ENEMY_BATTLE_SIZE; enemySlot++) {
		unitId = enemySlot + FIRST_ENEMY_UNIT;
		if(gmUnitGetStub(unitId)->side == BATTLE_SIDE_NONE)
			break;
	}
	if(enemySlot == ENEMY_BATTLE_SIZE)
		return -1;
	return unitId;
}

/* Lays the enemies out again and moves each one's battle object anchor to its
   new place. */
UNREAD_RESULT btPlacementRelayoutEnemies(void)
{
	BATTLE_STATE *battle = BattleStateWork;
	Uint16		  unitIds[UNIT_MAX];
	Sint32		  xPositions[ENEMY_BATTLE_SIZE];
	Sint32		  zPositions[ENEMY_BATTLE_SIZE];
	Sint32		  enemyCnt;
	Sint32		  enemyIdx;

	for(enemyIdx = 0; enemyIdx < ENEMY_BATTLE_SIZE &&
		battle->enemyIds[enemyIdx] != BATTLE_UNIT_LIST_END;
		enemyIdx++)
		unitIds[enemyIdx] = battle->enemyIds[enemyIdx];
	enemyCnt = enemyIdx;
	btPlacementLayoutEnemies(unitIds, enemyCnt, xPositions, zPositions);
	for(enemyIdx = 0; enemyIdx < enemyCnt; enemyIdx++) {
		Sint32 unitId = battle->enemyIds[enemyIdx];

		if(unitId != BATTLE_UNIT_REMOVED) {
			BATTLE_OBJECT *battleObject = btObjectGet(unitId);

			battleObject->anchorX = toFIXED(xPositions[enemyIdx]);
			battleObject->anchorZ = toFIXED(zPositions[enemyIdx]);
		}
	}
}

/* Places every unit at the start of a battle and creates its battle object at
   its spot. */
void btUnitRefreshPlacement(void)
{
	BATTLE_STATE *battle;
	Uint16		  unitIds[UNIT_MAX];
	Sint32		  xPositions[ENEMY_BATTLE_SIZE];
	Sint32		  zPositions[ENEMY_BATTLE_SIZE];
	Sint32		  cnt;
	Sint32		  i;
	Sint32		  partyPlace = 0;
	Sint32		  enemyPlace = 0;
	Sint32		  num;
	Sint32		  enemy;
	Uint8		 *place;

	battle = BattleStateWork;
	cnt = btPartyPrepareMembers(unitIds);
	/* objIdx[0..7] is each party unit id's place and objIdx[8..13] the enemy
	   places, numbered 8 to 13. */
	num = BATTLE_OBJECT_NONE;
	for(i = 0; i < UNIT_MAX; i++)
		battle->objIdx[i] = num;
	place = &battle->objIdx[UNIT_MAX - 1];
	for(enemy = ENEMY_BATTLE_SIZE - 1, num = UNIT_MAX - 1; enemy >= 0;
		enemy--, num--)
		*place-- = num;

	/* The party, at its fixed spots in pixels: x 80, z 48, 16, -16 and -48
	   (BattlePlacementSpots, battle_tables.c). */
	for(i = 0; i < cnt; partyPlace++, i++) {
		Sint32 unitId = unitIds[i];

		battle->objIdx[unitId] = partyPlace;
		btObjectMake(btObjectGet(unitId), unitId,
			BattlePlacementSpots[partyPlace * 2],
			BattlePlacementSpots[partyPlace * 2 + 1]);
	}

	/* The enemies, laid out by size. */
	for(i = 0;
		i < ENEMY_BATTLE_SIZE && battle->enemyIds[i] != BATTLE_UNIT_LIST_END;
		i++)
		unitIds[i] = battle->enemyIds[i];
	cnt = i;
	btPlacementLayoutEnemies(unitIds, cnt, xPositions, zPositions);
	for(i = 0; i < cnt; i++, enemyPlace++) {
		Sint32 unitId = battle->enemyIds[i];

		if(unitId != BATTLE_UNIT_REMOVED)
			btObjectMake(btObjectGet(unitId), unitId, xPositions[enemyPlace],
				zPositions[enemyPlace]);
	}
}

#include "battle/placement_contains_id.c"
