/* @file */
/* Battle objects made and placed on the battlefield. */
/* Part of battle.c. */

/* The fdActorCreate kind a battle object's actor is made with: one that makes
 */
/* no sprite, its battle sprites being attached afterwards. */
#define BATTLE_OBJECT_ACTOR_KIND 15

/* Resets a battle object, making its actor when it has none: nine tenths */
/* size, the anchors at the centre unless keep is set, no sprite, animation, */
/* shadow or status, turned half a turn, visible, then the empty script. */
void btObjectReset(BATTLE_OBJECT *battleObject, s32 keep) {
	MOTION_OBJECT *object;
	s32 facingAngle;

	if(battleObject->obj == 0)
		object = (MOTION_OBJECT *)fdActorCreateStub(
			ACTOR_CREATE_KIND(BATTLE_OBJECT_ACTOR_KIND), 0, 0, 0);
	else
		object = battleObject->obj;
	/* FAKEMATCH(dead): a once-only block placing centre's declaration; as
	   centre declared with the routine's locals. */
	{
		FIXED centre = 0;

		battleObject->scale = FIXED_ONE * 9 / 10;
		battleObject->obj = object;
		if(keep == 0) {
			battleObject->anchorZ = centre;
			battleObject->anchorX = centre;
			object->x = battleObject->anchorX;
			object->z = battleObject->anchorZ;
		}
	}
	battleObject->pal = 0;
	battleObject->resource = 0;
	battleObject->overlay = 0;
	battleObject->anm = 0;
	battleObject->shadow = 0;
	battleObject->anmEntry = 0;
	battleObject->activeStatuses = 0;
	battleObject->shadowEntry = 0;
	battleObject->active = 0;
	battleObject->selectedCondition = 0;
	battleObject->fading = 0;
	facingAngle = syAtan(0, 0) + DEGtoANG(180);
	object->ang = facingAngle;
	object->collisionFlg = ACTOR_SOLID | ACTOR_PUSHABLE;
	object->motionFlg = ACTOR_FALLS;
	battleObject->visible = 1;
	fdActorStartScriptStub((ACTOR *)object, BattleObject_EmptyScript);
}

/* Creates the battle object of a unit at (x, y) on the battlefield: its */
/* sprite, palette, size and weapon, turned to face the centre. */
void btObjectMake(BATTLE_OBJECT *battleObject, s32 unitId, s32 x, s32 y) {
	FIXED fixedX;
	FIXED fixedZ;
	s32 palette;
	s32 sprite;
	s32 registeredSprite;
	UNIT *unit;
	MOTION_OBJECT *object;
	s32 facingAngle;
	s32 weaponSprite;

	fixedX = toFIXED(x);
	fixedZ = toFIXED(y);
	object = fdActorCreateStub(ACTOR_CREATE_KIND(BATTLE_OBJECT_ACTOR_KIND),
							   fixedX, 0, fixedZ);
	unit = syUnitGet(unitId);
	palette = 0;
	registeredSprite = btEnemySprLoadIntoSlots(unitId);

	/* The sprite: an enemy's from its class, a party member's by character; a
	 */
	/* character on the enemy side, a link opponent, takes palette 1. */
	if(unit->classId == CLASS_NONE) {
		sprite = btSprGetSpr(unit->enemyId);
		if(registeredSprite == 0)
			palette = btSprGetPalette(unit->enemyId);
		else
			sprite = registeredSprite;
	}
	else {
		switch(unit->enemyId) {
		case ISAAC:
			sprite = SPRITE_BATTLE_ISAAC;
			break;
		case GARET:
			sprite = SPRITE_BATTLE_GARET;
			break;
		case MIA:
			sprite = SPRITE_BATTLE_MIA;
			break;
		case IVAN:
			sprite = SPRITE_BATTLE_IVAN;
			break;
		case FELIX:
			sprite = SPRITE_BATTLE_FELIX;
			break;
		case SHEBA:
			sprite = SPRITE_BATTLE_SHEBA;
			break;
		case PIERS:
			sprite = SPRITE_BATTLE_PIERS;
			break;
		case JENNA:
		default:
			sprite = SPRITE_BATTLE_JENNA;
			break;
		}
		if((u32)unitId > ALLY_MAX)
			palette = 1;
	}

	/* The units drawn larger or smaller than their sprites, in 16.16. */
	battleObject->scale = FIXED_ONE;
	battleObject->shadow = SPRITE_BATTLE_SHADOW;

	switch(unit->enemyId) {
	case ISAAC:
		battleObject->scale = FIXED_ONE;
		break;
	case GARET:
		battleObject->scale = FIXED_ONE;
		break;
	case MIA:
		battleObject->scale = FIXED_ONE;
		break;
	case IVAN:
		battleObject->scale = FIXED_ONE;
		break;
	case PIERS:
		battleObject->scale = FIXED_ONE * 21 / 20;
		break;
	case JENNA:
		battleObject->scale = FIXED_ONE * 19 / 20;
		break;
	case FELIX:
		battleObject->scale = FIXED_ONE;
		break;
	case SHEBA:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_SLIME_SOL_SANCTUM:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_SLIME_WORLD_MAP:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_OOZE:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_SLIME_BEAST:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_KILLER_APE:
		battleObject->scale = FIXED_ONE * 8 / 5;
		break;
	case ENEMY_MANTICORE:
		battleObject->scale = FIXED_ONE * 31 / 20;
		break;
	case ENEMY_LIVING_STATUE:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_HYDROS_STATUE:
		battleObject->scale = FIXED_ONE * 8 / 5;
		break;
	case ENEMY_ANT_LION:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_TRET:
		battleObject->scale = FIXED_ONE * 9 / 5;
		break;
	case ENEMY_DEADBEARD:
		battleObject->scale = FIXED_ONE * 9 / 5;
		break;
	case ENEMY_BANDIT:
		battleObject->scale = FIXED_ONE;
		break;
	case ENEMY_THIEF:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_BANDIT_WORLD_MAP:
		battleObject->scale = FIXED_ONE;
		break;
	case ENEMY_THIEF_WORLD_MAP:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_KRAKEN:
		battleObject->scale = FIXED_ONE * 17 / 10;
		break;
	case ENEMY_FUSION_DRAGON:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_KANDOREAN_TEMPLE:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_AIRS_ROCK:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_GABOMBA_STATUE:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_AQUA_ROCK:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_GAIA_ROCK:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_TREASURE_ISLE:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_JUPITER_LIGHTHOUSE:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_MAGMA_ROCK:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_MIMIC_MARS_LIGHTHOUSE:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_TROLL:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_CAVE_TROLL:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_ORC_CAPTAIN:
		battleObject->scale = FIXED_ONE;
		break;
	case ENEMY_ORC_LORD:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_KOBOLD_UNUSED:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_HOBGOBLIN:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_GRYPHON:
		battleObject->scale = FIXED_ONE * 13 / 10;
		break;
	case ENEMY_CHIMERA:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_AZART_IRON_SHIELD:
		battleObject->scale = FIXED_ONE * 21 / 20;
		break;
	case ENEMY_AZART_PADDED_GLOVES:
		battleObject->scale = FIXED_ONE * 21 / 20;
		break;
	case ENEMY_SATRAGE_CHAIN_MAIL:
		battleObject->scale = FIXED_ONE * 11 / 10;
		break;
	case ENEMY_SATRAGE_COTTON_SHIRT:
		battleObject->scale = FIXED_ONE * 11 / 10;
		break;
	case ENEMY_NAVAMPA_CLAYMORE:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_NAVAMPA_HUNTERS_SWORD:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_TOADONPA:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_TORNADO_LIZARD:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_STORM_LIZARD:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_TEMPEST_LIZARD:
		battleObject->scale = FIXED_ONE * 31 / 20;
		break;
	case ENEMY_RECLUSE:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_CERBERUS:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_SIREN:
	case ENEMY_SUCCUBUS:
	case ENEMY_NIGHTMARE:
	case ENEMY_MYSTERY_MAN:
	case ENEMY_SATUROS_MERCURY_LIGHTHOUSE:
	case ENEMY_SATUROS_VENUS_LIGHTHOUSE:
	case ENEMY_MYSTERY_WOMAN:
	case ENEMY_MENARDI:
		battleObject->scale = FIXED_ONE * 9 / 10;
		break;
	case ENEMY_RUFFIAN:
	case ENEMY_RUFFIAN_2:
	case ENEMY_RUFFIAN_3:
		battleObject->scale = FIXED_ONE * 19 / 20;
		break;
	case ENEMY_DULLAHAN:
	case ENEMY_DULLAHAN_B:
	case ENEMY_DULLAHAN_C:
	case ENEMY_SENTINEL:
	case ENEMY_SENTINEL_UNUSED_A:
	case ENEMY_SENTINEL_UNUSED_B:
		battleObject->scale = FIXED_ONE * 13 / 10;
		break;
	case ENEMY_VALUKAR:
	case ENEMY_VALUKAR_UNUSED_A:
	case ENEMY_VALUKAR_UNUSED_B:
		battleObject->scale = FIXED_ONE * 29 / 20;
		break;
	case ENEMY_SERPENT:
	case ENEMY_SERPENT_ONE_BEAM:
	case ENEMY_SERPENT_TWO_BEAMS:
	case ENEMY_SERPENT_THREE_BEAMS:
	case ENEMY_SERPENT_FOUR_BEAMS:
	case ENEMY_SERPENT_6:
	case ENEMY_SERPENT_7:
		battleObject->scale = FIXED_ONE * 11 / 10;
		break;
	case ENEMY_POSEIDON:
	case ENEMY_POSEIDON_SECOND_ACTION:
	case ENEMY_POSEIDON_3:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_AQUA_HYDRA:
		battleObject->scale = FIXED_ONE * 3 / 2;
		break;
	case ENEMY_HYDRA:
	case ENEMY_PYRODRA:
		battleObject->scale = FIXED_ONE * 11 / 10;
		break;
	case ENEMY_WILD_WOLF:
	case ENEMY_DINO:
		battleObject->scale = FIXED_ONE * 23 / 20;
		break;
	case ENEMY_DIRE_WOLF:
	case ENEMY_DINOX:
		battleObject->scale = FIXED_ONE * 6 / 5;
		break;
	case ENEMY_WHITE_WOLF:
	case ENEMY_DINOSAURUS:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_SEA_DRAGON:
	case ENEMY_TURTLE_DRAGON:
	case ENEMY_OCEAN_DRAGON:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_DEVIL_SCORPION:
	case ENEMY_SAND_SCORPION:
		battleObject->scale = FIXED_ONE * 3 / 4;
		break;
	case ENEMY_MOAPA:
	case ENEMY_MOAPA_B:
	case ENEMY_MOAPA_C:
		battleObject->shadow = SPRITE_MOAPA_SHADOW;
		battleObject->scale = FIXED_ONE * 23 / 20;
		break;
	case ENEMY_KNIGHT:
	case ENEMY_KNIGHT_B:
	case ENEMY_KNIGHT_3:
		battleObject->scale = FIXED_ONE * 101 / 100;
		break;
	case ENEMY_BRIGGS:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_SEA_FIGHTER:
	case ENEMY_CHAMPA_2:
	case ENEMY_CHAMPA_3:
	case ENEMY_MINOTAURUS:
	case ENEMY_MINOS_WARRIOR:
	case ENEMY_MINOS_KNIGHT:
	case ENEMY_AKA_MANAH:
	case ENEMY_DRUJ:
	case ENEMY_AESHMA:
		battleObject->scale = FIXED_ONE * 11 / 10;
		break;
	case ENEMY_FLAME_DRAGON_AGATIO:
		battleObject->scale = FIXED_ONE * 5 / 4;
		break;
	case ENEMY_FLAME_DRAGON_KARST:
		battleObject->scale = FIXED_ONE;
		break;
	case ENEMY_PIXIE:
	case ENEMY_FAERY:
	case ENEMY_WEIRD_NYPMH:
		battleObject->scale = FIXED_ONE * 4 / 5;
		break;
	case ENEMY_REFRESH_BALL:
	case ENEMY_THUNDER_BALL:
	case ENEMY_ANGER_BALL:
	case ENEMY_GUARDIAN_BALL:
		battleObject->scale = FIXED_ONE * 4 / 5;
		break;
	case ENEMY_CHESTBEATER:
	case ENEMY_WILD_GORILLA:
	case ENEMY_CRAZY_GORILLA:
	case ENEMY_AVIMANDER:
	case ENEMY_BRUTAL_TROLL:
	case ENEMY_LIZARD_FIGHTER:
	case ENEMY_LIZARD_KING:
	case ENEMY_DOOM_DRAGON_A:
	case ENEMY_DOOM_DRAGON_E:
	case ENEMY_DOOM_DRAGON_H:
		battleObject->scale = FIXED_ONE * 7 / 5;
		break;
	}

	battleObject->obj = object;
	battleObject->anchorX = fixedX;
	battleObject->anchorZ = fixedZ;
	battleObject->pal = palette;
	battleObject->resource = sprite;
	weaponSprite = btUnitGetWeaponSpr(unitId);
	battleObject->anm = 0;
	battleObject->anmEntry = 0;
	battleObject->shadowEntry = 0;
	battleObject->active = 0;
	battleObject->overlay = weaponSprite;
	battleObject->fading = 0;
	battleObject->visible = 0;
	/* Isaac and Garet draw a Machete as part of their own sprite, with no */
	/* weapon overlay. */
	if(unit->enemyId <= GARET &&
	   gmInventoryGetEquippedItemStub(unit, EQUIP_WEAPON) == ITEM_MACHETE) {
		if(unit->enemyId == ISAAC) {
			sprite = SPRITE_ISAAC_MACHETE;
			battleObject->resource = sprite;
		}
		else {
			sprite = SPRITE_GARET_MACHETE;
			battleObject->resource = sprite;
		}
		battleObject->overlay = 0;
	}

	/* Face the centre of the battlefield; enemies are drawn at 1.3 times their
	 */
	/* size. */
	facingAngle = syAtan(y / 8, x) + DEGtoANG(180);
	object->ang = facingAngle;
	object->collisionFlg = ACTOR_SOLID | ACTOR_PUSHABLE;
	object->motionFlg = ACTOR_FALLS;
	if(unit->classId == CLASS_NONE) {
		object->scaleX = FIXED_ONE * 13 / 10;
		object->scaleY = FIXED_ONE * 13 / 10;
	}
	else {
		object->scaleX = FIXED_ONE;
		object->scaleY = FIXED_ONE;
	}
	fdActorStartScriptStub((ACTOR *)object, BattleObject_EmptyScript);
}

/* Unused: reads the battlefield position of a party place. */
void btPlacementGetStepPair(s32 partyPlace, s32 *x, s32 *y) {
	partyPlace *= 2;
	*x = BattlePlacement_Spots[partyPlace];
	*y = BattlePlacement_Spots[partyPlace + 1];
}

/* BATTLE_STATE.enemyIds of an enemy that left the battle, and of a place */
/* held for one: unit ids 254 and 255 in the list. */
#define IS_PLACEHOLDER_ID(id) ((u16)((id) - BATTLE_UNIT_REMOVED) <= 1)

/* The enemies' column, in pixels from the battlefield's centre, and the */
/* nearer place the Kraken, the Fusion Dragon and the Serpent take. */
#define ENEMY_COLUMN_X		(-80)
#define ENEMY_COLUMN_NEAR_X (-50)
/* Pixels of the column a large enemy, a small one and an empty place take. */
#define ENEMY_SPACING_LARGE 38
#define ENEMY_SPACING_SMALL 27
#define ENEMY_SPACING_EMPTY 25
/* The spacing the column's length is first estimated from for up to four */
/* enemies; five or six are estimated at a small enemy's. */
#define ESTIMATED_ENEMY_SPACING 30

/* Lays out count enemies in a column left of the centre, spaced by their size,
 */
/* from a rough estimate of the column's length. */
void btPlacementLayoutEnemies(u16 *actorIds, s32 count, s32 *xPositions,
							  s32 *zPositions) {
	s32 totalSpacing =
		count <= 4 ? ESTIMATED_ENEMY_SPACING : ENEMY_SPACING_SMALL;
	s32 z = ((count - 1) * totalSpacing) / 2;
	s32 index;

	/* z starts half a line's estimated length up and steps down by half of */
	/* each enemy's spacing before it (none for the first) and half after it. */
	for(index = 0; index != count; index++) {
		u32 spacing = 0;

		xPositions[index] = ENEMY_COLUMN_X;
		if(index != 0) {
			UNIT *actor;
			u32 enemy;

			spacing = ENEMY_SPACING_EMPTY;
			if(!IS_PLACEHOLDER_ID(actorIds[index])) {
				actor = syUnitGet(actorIds[index]);
				spacing = btSprIsSmall(actor->enemyId) ? ENEMY_SPACING_SMALL
													   : ENEMY_SPACING_LARGE;
				enemy = actor->enemyId;
				/* The first place keeps ENEMY_COLUMN_X whatever its enemy. */
				if(enemy == ENEMY_FUSION_DRAGON || enemy == ENEMY_KRAKEN ||
				   enemy == ENEMY_SERPENT || enemy == ENEMY_SERPENT_ONE_BEAM ||
				   enemy == ENEMY_SERPENT_TWO_BEAMS ||
				   enemy == ENEMY_SERPENT_THREE_BEAMS ||
				   enemy == ENEMY_SERPENT_FOUR_BEAMS ||
				   enemy == ENEMY_SERPENT_6 || enemy == ENEMY_SERPENT_7) {
					xPositions[index] = ENEMY_COLUMN_NEAR_X;
				}
			}
		}
		z -= spacing / 2;
		zPositions[index] = z;
		spacing = ENEMY_SPACING_EMPTY;
		if(!IS_PLACEHOLDER_ID(actorIds[index])) {
			UNIT *actor = syUnitGet(actorIds[index]);

			spacing = btSprIsSmall(actor->enemyId) ? ENEMY_SPACING_SMALL
												   : ENEMY_SPACING_LARGE;
		}
		z -= spacing / 2;
	}
}

/* Returns the id of the first free enemy unit, or -1 when all six are taken. */
s32 btEnemyGetFreeUnitId(void) {
	s32 enemySlot;
	s32 unitId;

	for(enemySlot = 0; enemySlot < ENEMY_BATTLE_SIZE; enemySlot++) {
		unitId = enemySlot + FIRST_ENEMY_UNIT;
		if(syUnitGet(unitId)->side == BATTLE_SIDE_NONE)
			break;
	}
	if(enemySlot == ENEMY_BATTLE_SIZE)
		return -1;
	return unitId;
}

/* Lays the enemies out again and moves each one's battle object anchor to its
 */
/* new place, skipping the removed places before the first enemy and laying */
/* out up to the last one still there. */
void btPlacementRelayoutEnemies(void) {
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	u16 unitIds[UNIT_MAX];
	s32 x[ENEMY_BATTLE_SIZE];
	s32 z[ENEMY_BATTLE_SIZE];
	s32 last = -1;
	s32 count = 0;
	s32 i;
	s32 total;

	for(i = 0;
		i < ENEMY_BATTLE_SIZE && battle->enemyIds[i] != BATTLE_UNIT_LIST_END;
		i++) {
		if(battle->enemyIds[i] == BATTLE_UNIT_REMOVED) {
			if(count == 0)
				continue;
		}
		else {
			last = count;
		}
		unitIds[count] = battle->enemyIds[i];
		count++;
	}
	total = last + 1;
	btPlacementLayoutEnemies(unitIds, total, x, z);
	for(i = 0; i < total; i++) {
		s32 id = unitIds[i];

		if(id != BATTLE_UNIT_REMOVED) {
			BATTLE_OBJECT *battleObject = btObjectGet(id);

			battleObject->anchorX = toFIXED(x[i]);
			battleObject->anchorZ = toFIXED(z[i]);
		}
	}
}

/* Places every unit at the start of a battle and creates its battle object at
 */
/* its spot, the reserve party members too. */
void btUnitRefreshPlacement(void) {
	u16 ids[UNIT_MAX];
	s32 x[ENEMY_BATTLE_SIZE];
	s32 z[ENEMY_BATTLE_SIZE];
	s32 count;
	s32 i;
	s32 place;
	s32 stand;
	s32 n;
	s32 value;
	u8 *p;
	BATTLE_STATE *battle;

	place = 0;
	stand = 0;
	battle = Ram_HeapSlots->battleState;
	count = btPartyPrepareMembers(ids);
	count += btPartyPrepareReserveOwners(ids + count);
	/* placement[0..7] is each party unit id's place and placement[8..13] the */
	/* enemy places, numbered 8 to 13. */
	value = BATTLE_OBJECT_NONE;
	for(i = 0; i < UNIT_MAX; i++)
		battle->placement[i] = value;
	for(n = ENEMY_BATTLE_SIZE - 1, p = &battle->placement[UNIT_MAX - 1],
	value = UNIT_MAX - 1;
		n >= 0; n--, value--)
		*p-- = value;

	/* The party, at its fixed spots in pixels (BattlePlacement_Spots). */
	for(i = 0; i < count; place++, i++) {
		s32 id = ids[i];

		battle->placement[id] = place;
		btObjectMake(btObjectGet(id), id, BattlePlacement_Spots[place * 2],
					 BattlePlacement_Spots[place * 2 + 1]);
	}
	/* The enemies, laid out by size. */
	for(i = 0;
		i < ENEMY_BATTLE_SIZE && battle->enemyIds[i] != BATTLE_UNIT_LIST_END;
		i++)
		ids[i] = battle->enemyIds[i];
	count = i;
	btPlacementLayoutEnemies(ids, count, x, z);
	for(i = 0; i < count; i++, stand++) {
		s32 id = battle->enemyIds[i];

		if(id != BATTLE_UNIT_REMOVED)
			btObjectMake(btObjectGet(id), id, x[stand], z[stand]);
	}
}

/* Returns 1 if the unit at place is in list. */
s32 btPlacementContainsId(s16 *list, s32 place) {
	u32 i;

	if(place > ALLY_MAX)
		place += FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
	i = 0;
	do {
		u16 unitId = *list++;

		if(unitId == BATTLE_UNIT_LIST_END)
			break;
		if(unitId == place)
			return 1;
	} while(++i < UNIT_MAX);
	return 0;
}
