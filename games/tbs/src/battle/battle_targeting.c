/* The targets an enemy's or a confused unit's ability picks. Part of
   battle.c. */

#include "battle_internal.h"

#if !FIX_POINTER_OUTSIDE_OBJECT
/* BATTLE_STATE's unit lists, read as one array of halfwords. */
typedef struct BATTLE_STATE_UNIT_LIST_VIEW {
	/* BATTLE_STATE from its start; each list at its offset / 2 */
	Sint16 items[64];
} BATTLE_STATE_UNIT_LIST_VIEW;
#endif

/* Picks a random place: a party member still standing with requireLivingUnit,
   otherwise any enemy place in use. Returns 0 when there is none. */
Sint32 btAIChooseRandomTarget(Sint32 requireLivingUnit)
{
	Uint16 places[6];
	Sint32 cnt;

	/* UB: the view treats distinct BATTLE_STATE members as one array. */
#if FIX_POINTER_OUTSIDE_OBJECT
	Sint16 *unitIds;
	Sint32	unitId;
	Sint32	place;
	Sint32	sideTag;

	cnt = 0;
	unitIds = requireLivingUnit ? BattleStateWork->partyUnits
								: BattleStateWork->enemyIds;
	sideTag = requireLivingUnit ? PLACE_PARTY : PLACE_ENEMY;
	for(place = 0; unitIds[place] != BATTLE_UNIT_LIST_END; place++) {
		unitId = unitIds[place];
		if(unitId == BATTLE_UNIT_REMOVED)
			continue;
		if(requireLivingUnit && gmUnitGetStub(unitId)->hp == 0)
			continue;
		places[cnt++] = place | sideTag;
	}
#else
	struct BATTLE_STATE_UNIT_LIST_VIEW *lists;
	Sint16							   *entry;
	Uint16							   *cursor;
	Sint32								listedUnit;
	Sint32								place;
	Sint32								slot;
	Sint32								tail;
	Sint32								ofs;

	cnt = 0;
	lists = (struct BATTLE_STATE_UNIT_LIST_VIEW *)BattleStateWork;

	/* The party's standing places, else the enemies' places in use. */
	if(requireLivingUnit != 0) {
		place = 0;
		slot = offsetof(BATTLE_STATE, partyUnits) / sizeof(Sint16);
		if(lists->items[slot] != BATTLE_UNIT_LIST_END) {
			entry = lists->items;
			for(;;) {
				listedUnit = entry[slot];
				if(listedUnit != BATTLE_UNIT_REMOVED) {
					if(gmUnitGetStub(listedUnit)->hp != 0) {
						places[cnt] = place | PLACE_PARTY;
						cnt++;
					}
				}
				slot++;
				place++;
				if(entry[slot] == BATTLE_UNIT_LIST_END)
					break;
			}
		}
	} else {
		place = 0;
		slot = offsetof(BATTLE_STATE, enemyIds) / sizeof(Sint16) - 1;
		tail = offsetof(BATTLE_STATE, enemyIds) / sizeof(Sint16) - 1;
		ofs = tail * 2;
		entry = (lists->items + 1);
		if(*(Sint16 *)((char *)entry + ofs) != BATTLE_UNIT_LIST_END) {
			cursor = entry;
			do {
				if((Sint16)cursor[slot] != BATTLE_UNIT_REMOVED) {
					places[cnt] = place | PLACE_ENEMY;
					cnt++;
				}
				slot++;
				tail++;
				place++;
			} while(entry[tail] != BATTLE_UNIT_LIST_END);
		}
	}
#endif

	if(cnt == 0)
		return 0;
	return places[syRandom16Below(cnt)];
}

/* BUG: Restore cures stun and never confusion or charm, as its description says
   ("Remove sleep, stun, and delusion.") and its cure does, but the count takes
   confusion and charm and not stun, so Cerebus and the Hydros Statue pass over
   an ally who is only stunned. COUNT_STUN_SLOT is the count's place for
   stun. */
#if FIX_RESTORE_COUNTS_STUN
#define COUNT_STUN_SLOT(unit, count) \
	if((unit)->stun != 0) \
		(count)++;
#else
#define COUNT_STUN_SLOT(unit, count) \
	if((Uint8)(unit)->confusion != 0) \
		(count)++; \
	if((unit)->charm != 0) \
		(count)++;
#endif

/* Counts the statuses of unit that Restore's count takes: delusion,
   COUNT_STUN_SLOT's (confusion and charm where Restore cures stun, the BUG
   above), sleep, the Psynergy seal and the Grim Reaper. */
#define COUNT_PARTIAL_CURES(unit, count) \
	{ \
		if((unit)->delusion != 0) \
			(count)++; \
		COUNT_STUN_SLOT((unit), (count)) \
		if((unit)->sleep != 0) \
			(count)++; \
		if((unit)->psySeal != 0) \
			(count)++; \
		if((unit)->grimReaper != 0) \
			(count)++; \
	}

/* Counts the statuses Tonic cures: those, the evil spirit and poison. No enemy
   has Tonic. */
#define COUNT_ALL_CURES(unit, count) \
	{ \
		COUNT_PARTIAL_CURES((unit), (count)); \
		if((unit)->haunt != 0) \
			(count)++; \
		if((unit)->poison != 0) \
			(count)++; \
	}

/* Counts what Break would undo on unit; a raised Agility is not counted. */
#define COUNT_POSITIVE_MODIFIERS(unit, count) \
	{ \
		if((unit)->attackStage > 0) \
			(count)++; \
		if((unit)->defenseStage > 0) \
			(count)++; \
		if((unit)->resistanceStage > 0) \
			(count)++; \
		if((unit)->summonBoost[ELEMENT_VENUS] > 0) \
			(count)++; \
		if((unit)->summonBoost[ELEMENT_MERCURY] > 0) \
			(count)++; \
		if((unit)->summonBoost[ELEMENT_MARS] > 0) \
			(count)++; \
		if((unit)->summonBoost[ELEMENT_JUPITER] > 0) \
			(count)++; \
	}

/* How worthwhile an enemy's ability is against unit, 0 for pointless: a stat
   change that can still move, a cure or heal it needs, a status it lacks, a
   downed unit to revive. */
#define CHECK_EFFECT_TARGET(unit, action, applies, kindIndex) \
	{ \
		(applies) = 0; \
		switch((action)->eff) { \
		case ABILITY_EFFECT_ATTACK_UP_SHARPLY: \
		case ABILITY_EFFECT_ATTACK_UP: \
			if((unit)->attackStage + 1 <= STAT_STAGE_MAX) \
				(applies) = 1; \
			if((unit)->attackTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY: \
		case ABILITY_EFFECT_ATTACK_DOWN: \
			if((unit)->attackStage - 1 >= STAT_STAGE_MIN) \
				(applies) = 1; \
			if((unit)->attackTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_DEFENSE_UP_SHARPLY: \
		case ABILITY_EFFECT_DEFENSE_UP: \
			if((unit)->defenseStage + 1 <= STAT_STAGE_MAX) \
				(applies) = 1; \
			if((unit)->defenseTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY: \
		case ABILITY_EFFECT_DEFENSE_DOWN: \
			if((unit)->defenseStage - 1 >= STAT_STAGE_MIN) \
				(applies) = 1; \
			if((unit)->defenseTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY: \
		case ABILITY_EFFECT_RESISTANCE_UP: \
			if((unit)->resistanceStage + 1 <= STAT_STAGE_MAX) \
				(applies) = 1; \
			if((unit)->resistanceTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY: \
		case ABILITY_EFFECT_RESISTANCE_DOWN: \
			if((unit)->resistanceStage - 1 >= STAT_STAGE_MIN) \
				(applies) = 1; \
			if((unit)->resistanceTurns == 1) \
				(applies)++; \
			break; \
		case ABILITY_EFFECT_CURE_POISON: \
			if((unit)->poison != 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_RESTORE: \
			COUNT_PARTIAL_CURES((unit), (applies)); \
			break; \
		case ABILITY_EFFECT_FIZZ: \
		case ABILITY_EFFECT_SPRITZ: \
			if((unit)->hp < (unit)->hpMax) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_BREAK: \
			COUNT_POSITIVE_MODIFIERS((unit), (applies)); \
			break; \
		case ABILITY_EFFECT_CURE_ALL: \
			COUNT_ALL_CURES((unit), (applies)); \
			break; \
		case ABILITY_EFFECT_CURSE: \
			if((unit)->grimReaper == 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_POISON: \
			if((unit)->poison == 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_VENOM: \
			if((unit)->poison <= POISON_NORMAL) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_STUN: \
			if((unit)->stun == 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_SLEEP: \
			if((unit)->sleep == 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_HAUNT: \
			if((unit)->haunt == 0) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_REVIVE: \
		case ABILITY_EFFECT_REVIVE_HALF: \
		case ABILITY_EFFECT_DEW: \
			if((unit)->hp == 0) \
				(applies) = 100; \
			break; \
		case ABILITY_EFFECT_NONE: \
		case ABILITY_EFFECT_RESTORE_HP: \
		case ABILITY_EFFECT_RESTORE_PP: \
			break; \
		default: \
			(applies) = 1; \
			break; \
		} \
		if((unit)->hp == 0 && gmBattleIsReviveEffectStub((action)->eff) == 0) \
			(applies) = 0; \
		if((applies) == 0) { \
			(kindIndex) = ((action)->flg & ABILITY_FORMULA_MASK) - 1; \
			switch(kindIndex) { \
			case ABILITY_FORMULA_ATTACK - 1: \
			case ABILITY_FORMULA_ATTACK_MULTIPLIED - 1: \
				if((unit)->hp != 0) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_SUMMON - 1: \
			case ABILITY_FORMULA_NO_POWER - 1: \
				if((action)->power != 0 && (unit)->hp != 0) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_PP_DAMAGE - 1: \
				if((unit)->pp != 0) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_HEAL_HP - 1: \
				if((unit)->hp != 0 && (unit)->hp < (unit)->hpMax) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_EFFECT - 1: \
			case ABILITY_FORMULA_DAMAGE - 1: \
			case ABILITY_FORMULA_ENEMY_DAMAGE - 1: \
				if((action)->power != 0 && (unit)->hp != 0) \
					(applies)++; \
				break; \
			} \
		} \
	}

/* Picks the target of an enemy's ability, or a confused unit's, at random among
   those it would affect; a single-target attack by an enemy with a strategy
   favours the strongest. */
Sint32 btAIChooseTarget(Sint32 userId, ABILITY_DEFINITION *action)
{
	Uint16		  candidatePlaces[6];
	Sint32		  unitIds[6];
	Sint32		  tgtPlaces[6];
	BATTLE_STATE *battle;
	UNIT		 *unit;
	UNIT		 *nextUnit;
	UNIT		 *thisUnit;
	Sint32		  candidateCnt;
	Sint32		  tgtIdx;
	Sint32		  tgtCnt;
	Sint32		  innerIdx;
	Sint32		  selected;
	Sint32		  unitId;
	Sint32		  value;
	Sint32		  nextValue;
	Sint32		  temp;
	Sint32		  ownSide;
	Sint32		  kindIdx;
	Sint32		  applies;
	Uint32		  roll;
	Sint16		 *slot;

	battle = BattleStateWork;
	tgtCnt = 0;
	candidateCnt = 0;

	/* The candidates: every unit on the side the ability may target. */
	if(action->tgtMode != ABILITY_TARGET_ONE) {
		ownSide = 0;
		if(action->tgtMode == ABILITY_TARGET_ALLIES ||
			action->tgtMode == ABILITY_TARGET_SELF)
			ownSide = 1;

		if(((userId < 0 || userId > ALLY_MAX) && ownSide == 0) ||
			(userId >= 0 && userId <= ALLY_MAX && ownSide != 0)) {
			for(tgtIdx = 0; battle->partyUnits[tgtIdx] != BATTLE_UNIT_LIST_END;
				tgtIdx++) {
				slot = &battle->partyUnits[tgtIdx];
				unitId = *slot;
				if(unitId != BATTLE_UNIT_REMOVED) {
					if(action->tgtMode != ABILITY_TARGET_SELF ||
						unitId == userId) {
						unitIds[candidateCnt] = unitId;
						candidatePlaces[candidateCnt] = PLACE_PARTY;
						candidatePlaces[candidateCnt] |= tgtIdx;
						candidateCnt++;
					}
				}
			}
		} else {
			for(tgtIdx = 0; battle->enemyIds[tgtIdx] != BATTLE_UNIT_LIST_END;
				tgtIdx++) {
				unitId = battle->enemyIds[tgtIdx];
				if(unitId != BATTLE_UNIT_REMOVED) {
					if(action->tgtMode != ABILITY_TARGET_SELF ||
						unitId == userId) {
						unitIds[candidateCnt] = unitId;
						candidatePlaces[candidateCnt] = PLACE_ENEMY;
						candidatePlaces[candidateCnt] |= tgtIdx;
						candidateCnt++;
					}
				}
			}
		}
	}

	if(candidateCnt == 0)
		return TARGET_PICK_SIDE_EMPTY;

	/* Keep the candidates the ability would affect, in place. */
	for(tgtIdx = 0; tgtIdx < candidateCnt; tgtIdx++) {
		unit = gmUnitGetStub(unitIds[tgtIdx]);
		CHECK_EFFECT_TARGET(unit, action, applies, kindIdx);
		if(applies != 0) {
			unitIds[tgtCnt] = unitIds[tgtIdx];
			tgtPlaces[tgtCnt] = candidatePlaces[tgtIdx];
			tgtCnt++;
		}
	}

	if(tgtCnt == 0)
		return TARGET_PICK_NONE;

	if(action->tgtMode == ABILITY_TARGET_FOES && action->range == 1 &&
		gmEnemyGetDefinitionStub(gmUnitGetStub(userId)->id)->tgtStrategy !=
			TARGET_STRATEGY_RANDOM &&
		(Uint32)((action->flg & ABILITY_FORMULA_MASK) -
			ABILITY_FORMULA_ATTACK) <=
			ABILITY_FORMULA_DAMAGE - ABILITY_FORMULA_ATTACK) {
		/* Sort the targets by HP, the most first. */
		/* BUG: each pass starts one place later, so HP 1, 2 and 3 end as 2, 3
		   and 1. */
		selected = -1;
		for(tgtIdx = 0; tgtIdx < tgtCnt; tgtIdx++) {
			for(innerIdx = tgtIdx; innerIdx < tgtCnt - 1; innerIdx++) {
				thisUnit = gmUnitGetStub(unitIds[innerIdx]);
				nextUnit = gmUnitGetStub(unitIds[innerIdx + 1]);
				if(gmEnemyGetDefinitionStub(gmUnitGetStub(userId)->id)
						->tgtStrategy == TARGET_STRATEGY_MOST_HP) {
					value = thisUnit->hp;
					nextValue = nextUnit->hp;
				} else {
					value = thisUnit->hpMax;
					nextValue = nextUnit->hpMax;
				}
				if(value < nextValue) {
					temp = unitIds[innerIdx];
					unitIds[innerIdx] = unitIds[innerIdx + 1];
					unitIds[innerIdx + 1] = temp;
					temp = tgtPlaces[innerIdx];
					tgtPlaces[innerIdx] = tgtPlaces[innerIdx + 1];
					tgtPlaces[innerIdx + 1] = temp;
				}
			}
		}

		/* Weighted toward the front: of a roll's outcomes the first target
		   takes 6, the second 5, the third 4 and the fourth 3; five or more
		   fall through to an even roll. */
		switch(tgtCnt) {
		case 1:
			selected = 0;
			break;
		case 2:
			roll = syRandom16Below(6 + 5);
			selected = 0;
			if(roll >= 6)
				selected = 1;
			break;
		case 3:
			selected = syRandom16Below(6 + 5 + 4);
			if(selected < 6)
				selected = 0;
			else if(selected < 6 + 5)
				selected = 1;
			else
				selected = 2;
			break;
		case 4:
			selected = syRandom16Below(6 + 5 + 4 + 3);
			if(selected < 6)
				selected = 0;
			else if(selected < 6 + 5)
				selected = 1;
			else if(selected < 6 + 5 + 4)
				selected = 2;
			else
				selected = 3;
			break;
		}

		if(selected >= 0)
			return tgtPlaces[selected];
	}
	return tgtPlaces[syRandom16Below(tgtCnt)];
}
