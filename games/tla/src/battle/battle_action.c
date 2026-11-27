/* An action announced and built for its targets, btActionRunBuild, with whether
   a unit is in its side's list and how many Djinn a side has on standby. Part
   of battle.c. */

#include "battle/unit_is_listed.c"

/* Counts the standby Djinn on unitId's side, and each element's count into
   counts when it is not null. */
Sint32 btDjinnCountStandby(Uint32 unitId, Uint8 *cnts)
{
	Uint16				 unitIds[8];
	DJINN_RECOVERY_LIST *list;
	Sint32				 unitCnt;
	Sint32				 standbyCnt;
	Sint32				 recoveryIdx;
	Sint32				 i;
	Sint32				 sideMask;
	Sint32				 recoveryTbl;

	standbyCnt = 0;
	sideMask = BATTLE_SIDE_MASK_PARTY;
	if(unitId > ALLY_MAX)
		sideMask = BATTLE_SIDE_MASK_ENEMIES;
	unitCnt = btUnitListIds(sideMask, unitIds);
	recoveryTbl = DJINN_RECOVERY_PARTY;
	if(unitId > ALLY_MAX)
		recoveryTbl = DJINN_RECOVERY_OPPONENT;
	list = &gmDjinnGetRecoveryTableStub(recoveryTbl)->list;
	if(cnts != 0)
		for(i = 0; i < ELEMENT_COUNT; i++)
			cnts[i] = 0;
	for(recoveryIdx = 0; recoveryIdx != list->cnt; recoveryIdx++) {
		if(list->entries[recoveryIdx].turns == DJINN_RECOVERY_AFTER_BATTLE) {
			for(i = 0; i < unitCnt; i++)
				if(unitIds[i] == list->entries[recoveryIdx].unitId)
					break;
			if(i != unitCnt) {
				if(cnts != 0)
					cnts[list->entries[recoveryIdx].element]++;
				standbyCnt++;
			}
		}
	}
	return standbyCnt;
}

/* A range that reaches every place of a side from any of them: Dazzle's. */
#define RANGE_WHOLE_SIDE 11

/* The chances, out of 256, an action rolls against the battle's random byte. */
typedef enum ACTION_CHANCE {
	CHANCE_DELUDED_MISS = 153,	  /* a deluded attacker misses */
	CHANCE_BLOW_LIKELY = 153,	  /* a likely double or triple blow lands */
	CHANCE_BLOW_RARE = 32,		  /* a rare one */
	CHANCE_BLOW = 64,			  /* any other */
	CHANCE_VENGEANCE_EXTRA = 179, /* Vengeance strikes one more time or two */
	CHANCE_VENGEANCE_TWICE = 128  /* of those, two more */
} ACTION_CHANCE;

/* gmBattleRollEffectSuccess's successScale at full strength, in percent. */
#define EFFECT_CHANCE_FULL 100

/* Announces the action and builds it for its targets, paying its PP or Djinn.
   Returns 0 to stage it, ACTION_REPORTED when it failed with its message shown,
   or ACTION_NOTHING_TO_SHOW. */
Sint32 btActionRunBuild(BATTLE_ACTION_RECORD *request, BATTLE_PLAN *plan)
{
	UNIT		 *actor = syUnitGet(request->unitId);
	BATTLE_STATE *battle = (BATTLE_STATE *)RamHeapSlots->battleState;
	Sint32		  selectedTgtId;
	Sint32		  abilityId;

	/* Fills the action's targets: the aimed unit, or the places within its
	   range, every place for Dazzle; with none, the unit defends. */
	Sint32 btActionRunSetTargets(Sint32 abilityId)
	{
		ABILITY_DEFINITION *ability = gmAbilityGetDirectStub(abilityId);
		Sint32				mode = ability->tgtMode;
		Bool				includeDowned = FALSE;
		Sint32				targetCount;
		Sint32				partyCount;
		Sint32				enemyCount;
		Sint32				aimedPlace;
		Sint32				range;
		Sint32				place;
		Sint32				targetId;
		Sint32				pass;

		if(gmBattleIsReviveEffectStub(ability->eff))
			includeDowned = TRUE;
		switch(mode) {
		case ABILITY_TARGET_ONE:
			plan->tgtCnt = 1;
			plan->tgtOfs[0] = mode;
			plan->tgtIds[0] = selectedTgtId;
			plan->tgtMultiplier[0] = 1;
			break;
		case ABILITY_TARGET_SELF:
			plan->tgtOfs[0] = 0;
			plan->tgtCnt = 1;
			plan->tgtIds[0] = selectedTgtId;
			plan->tgtMultiplier[0] = 1;
			break;
		default:
			/* The places within range of the aimed one, on its side. */
			targetCount = 0;
			range = request->range;
			for(pass = 0; pass <= 1; pass++) {
				for(place = 0;
					battle->partyUnits[place] != BATTLE_UNIT_LIST_END; place++)
					;
				partyCount = place;
				for(place = 0; battle->enemyIds[place] != BATTLE_UNIT_LIST_END;
					place++)
					;
				enemyCount = place;
				aimedPlace = request->tgt & PLACE_INDEX_MASK;
				{
					Sint32 first = aimedPlace - range + 1;
					Sint32 last = aimedPlace + range - 1;

					for(place = first; place <= last; place++) {
						if(place < 0)
							continue;
						if(request->tgt & PLACE_ENEMY_SIDE) {
							if(place >= enemyCount)
								continue;
							targetId = battle->enemyIds[place];
							if(targetId == BATTLE_UNIT_REMOVED)
								continue;
							if(!includeDowned && syUnitGet(targetId)->hp == 0)
								continue;
							plan->tgtMultiplier[targetCount] = 1;
							plan->tgtOfs[targetCount] = place - aimedPlace;
							plan->tgtIds[targetCount] = targetId;
							targetCount++;
						} else {
							if(place >= partyCount)
								continue;
							targetId = battle->partyUnits[place];
							if(targetId == BATTLE_UNIT_REMOVED)
								continue;
							if(!includeDowned && syUnitGet(targetId)->hp == 0)
								continue;
							plan->tgtMultiplier[targetCount] = 1;
							plan->tgtOfs[targetCount] = place - aimedPlace;
							plan->tgtIds[targetCount] = targetId;
							targetCount++;
						}
					}
				}
				plan->tgtCnt = targetCount;
				if(targetCount <= 0) {
					mnMessageArgumentPushStub(
						request->unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub((Sint32)&MsgActorDefends);
					if(actor->defendLevel == 0)
						actor->defendLevel = DEFEND_NORMAL;
					return ACTION_REPORTED;
				}
				if(ability->eff != ABILITY_EFFECT_DAZZLE)
					break;
				range = RANGE_WHOLE_SIDE;
			}
			break;
		}
		return 0;
	}

	/* A fresh action run, and the debug defeat. */
	selectedTgtId = btGetUnitAtPlace(request->tgt);
	btPlaybackReset();
	/* The word after the plan in BATTLE_STATE is skipPlayback. */
	*(Sint32 *)(plan + 1) = 0;
	plan->hauntDamage = 0;
	plan->unitId = request->unitId;
	plan->tgtCnt = 0;
	plan->battleEff = 0;
	plan->failure = ACTION_FAILURE_NONE;
	plan->partnerId = request->unitId;
	plan->element = ELEMENT_NONE;
	mnMessageArgumentClearAllStub();
	if(actor->hp == 0)
		return ACTION_NOTHING_TO_SHOW;

	/* Debug: hold R to defeat all enemies, or R and Select the whole party. */
	if(DebugMode && syGameFlagGet(FLAG_DEBUG_BATTLE_KEYS) &&
		(PerWork.data & KEY_R)) {
		Bool   enemy = TRUE;
		Sint32 idx;
		Sint32 id;

		if(PerWork.data & KEY_SELECT)
			enemy = FALSE;
		for(idx = 0;; idx++) {
			if(enemy)
				id = battle->enemyIds[idx];
			else
				id = battle->partyUnits[idx];
			if(id == BATTLE_UNIT_LIST_END)
				break;
			if(id == BATTLE_UNIT_REMOVED)
				continue;
			if(gmUnitAddHpStub(id, DEFEAT_HP_CHANGE) == 0) {
				btPlaybackPush(PLAYBACK_HIT, id);
				btPlaybackPush(PLAYBACK_DEFEAT, id);
			}
		}
		btPlaybackRunAll();
		return ACTION_NOTHING_TO_SHOW;
	}

	/* What stops the unit, unless it strikes back with the Djinn Counter:
	   Ground, sleep, stun, a bound unit's bad luck. */
	mnMessageArgumentClearAllStub();
	if(request->command != COMMAND_DJINN_COUNTER) {
		if(actor->immobile) {
			actor->immobile = 0;
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgUnableToMove);
			return ACTION_REPORTED;
		}
		if(actor->sleep) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgIsAsleep);
			return ACTION_REPORTED;
		}
		if(actor->stun) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgIsParalyzed);
			return ACTION_REPORTED;
		}
		/* A bound unit loses one action in four, except Defend. */
		if((actor->curse & CURSE_BOUND) && request->command != COMMAND_DEFEND &&
			(gmBattleRandomStub() & 3) == 0) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgIsBound);
			return ACTION_REPORTED;
		}
	}
	if(request->command == COMMAND_NONE)
		return ACTION_NOTHING_TO_SHOW;
	abilityId = ABILITY_ATTACK;
	{
		Sint32 idx;

		for(idx = 0; idx < BATTLE_TARGET_CAPACITY; idx++)
			plan->tgtCritical[idx] = 0;
		for(idx = 0; idx < BATTLE_TARGET_CAPACITY; idx++)
			plan->tgtHit[idx] = HIT_NOT_ROLLED;
	}

	/* The announcement, the ability id and the targets, by command. */
	switch(request->command) {
	case COMMAND_FLEE:
		if((Uint16)request->unitId <= ALLY_MAX) {
			mnBattleMessageShowAndWaitStub((Sint32)&MsgPartyFlees);
		} else {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorRuns);
		}
		btMessageWaitForAdvance();
		plan->staging = STAGING_FLEE;
		return 0;

	case COMMAND_ATTACK:
		/* A weapon unleash replaces the action with its ability. */
		abilityId = gmEquipmentRollUnleashStub(actor);
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		if(abilityId != ABILITY_ATTACK) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(
				gmInventoryGetEquippedItemStub(actor, EQUIP_WEAPON),
				MESSAGE_ARGUMENT_ITEM);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgWeaponHowls);
			btMessageWaitForTyping();
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			mnBattleMessageShowAndWaitStub((Sint32)(MsgWeaponHowls + 1));
		}
		break;

	case COMMAND_DJINN_COUNTER:
		abilityId = ABILITY_DJINN_COUNTER;
		/* Poseidon answers with a plain attack. */
		if((Uint16)(actor->enemyId - ENEMY_POSEIDON) <=
			ENEMY_POSEIDON_3 - ENEMY_POSEIDON)
			abilityId = ABILITY_ATTACK;
		if(selectedTgtId == BATTLE_UNIT_LIST_END)
			return ACTION_NOTHING_TO_SHOW;
		plan->tgtCnt = 1;
		plan->tgtOfs[0] = 0;
		plan->tgtIds[0] = selectedTgtId;
		plan->tgtMultiplier[0] = 1;
		break;

	case COMMAND_PSYNERGY: {
		ABILITY_DEFINITION *ability;
		Bool				canCast = TRUE;

		abilityId = request->prm;
		ability = gmAbilityGetDirectStub(abilityId);
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
		switch(abilityId) {
		case ABILITY_WILD_WOLF ... ABILITY_PHOENIX:
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorCastsSpecial);
			break;
		case ABILITY_FLAME_CARD ... ABILITY_DEATH_CARD:
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorCastsDjinni);
			break;
		default:
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorCasts);
			break;
		}
		/* Too little PP or a seal fails the cast; only a cast that goes ahead
		   pays. */
		if(actor->pp < ability->ppCost) {
			plan->failure = ACTION_FAILURE_NO_PP;
			canCast = FALSE;
		}
		if(actor->psySeal) {
			plan->failure = ACTION_FAILURE_SEALED;
			canCast = FALSE;
		}
		if(canCast) {
			plan->failure = ACTION_FAILURE_NONE;
			actor->pp -= ability->ppCost;
			gmUnitRecalculateGaugesStub(request->unitId);
			if(actor->pp < 0)
				actor->pp = 0;
			if(actor->pp > actor->ppMax)
				actor->pp = actor->ppMax;
		}
	} break;

	case COMMAND_ITEM: {
		ITEM_DEFINITION *item;

		/* A slot of -1: an earlier command this turn used the item up
		   (btStagingRunItem). */
		if(request->prm < 0) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgItemAlreadyUsed);
			return ACTION_REPORTED;
		}
		/* An item with no ability, or a broken one, and the user defends. */
		item = gmItemGetDirectStub(actor->inventory[request->prm]);
		abilityId = item->abilityId;
		if(abilityId == 0 ||
			(actor->inventory[request->prm] & INVENTORY_BROKEN)) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorDefends);
			if(actor->defendLevel == 0)
				actor->defendLevel = DEFEND_NORMAL;
			return ACTION_REPORTED;
		}
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(
			actor->inventory[request->prm], MESSAGE_ARGUMENT_ITEM);
		/* An item that is kept is raised ("raises"); anything else is used. */
		if(item->useType == ITEM_USE_MAY_BREAK ||
			item->useType == ITEM_USE_FREE) {
			switch(item->type) {
			case ITEM_TYPE_WEAPON:
			case ITEM_TYPE_SHIELD:
			case ITEM_TYPE_PSYNERGY:
			case ITEM_TYPE_TRIDENT:
			case ITEM_TYPE_RING:
				mnBattleMessageShowAndWaitStub((Sint32)&MsgActorRaisesItem);
				break;
			default:
				mnBattleMessageShowAndWaitStub((Sint32)&MsgActorUsesBattleItem);
				break;
			}
		} else {
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorUsesBattleItem);
		}
	} break;

	case COMMAND_DEFEND:
	case COMMAND_TOTAL_DEFENSE:
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((Sint32)&MsgActorDefends);
		return ACTION_REPORTED;

	case COMMAND_NONE:
		return ACTION_NOTHING_TO_SHOW;

	case COMMAND_ABILITY: {
		Sint32 msg;

		abilityId = request->prm;
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
		if((gmAbilityGetDirectStub(abilityId)->flg & ABILITY_FORMULA_MASK) ==
			ABILITY_FORMULA_ENEMY_DAMAGE)
			msg = (Sint32)&MsgActorUnleashesAbility;
		else
			msg = (Sint32)&MsgActorUsesAbility;
		/* Some enemy abilities have their own wording. */
		switch(abilityId) {
		case ABILITY_SONIC_WAVE:
		case ABILITY_SONIC_WAVE_STRONG:
			msg = (Sint32)&MsgEmitsUltrasonicWaves;
			break;
		case ABILITY_HEAT_MIRAGE:
			msg = (Sint32)&MsgActorCasts;
			break;
		case ABILITY_THREATEN:
			msg = (Sint32)&MsgGlowersFerociously;
			break;
		case ABILITY_TREMBLE:
			msg = (Sint32)&MsgIsIntimidated;
			break;
		case ABILITY_CONTAIN:
			msg = (Sint32)&MsgBalefulGaze;
			break;
		case ABILITY_WORMS:
			msg = (Sint32)&MsgEatsWorms;
			break;
		case ABILITY_SHRIEK:
		case ABILITY_BANSHEE_HOWL:
		case ABILITY_CRAZY_VOICE:
		case ABILITY_WAR_CRY:
		case ABILITY_WICKED_HOWL:
			msg = (Sint32)&MsgLetsOutAbility;
			break;
		case ABILITY_WING_BEAT:
		case ABILITY_WING_FLUTTER:
		case ABILITY_WING_STROKE:
			msg = (Sint32)&MsgActorUsesAbility;
			break;
		case ABILITY_ROTTEN_BLOOD:
			msg = (Sint32)&MsgEnemyUnleashesAbility;
			break;
		case ABILITY_HEARTRENDER:
			msg = (Sint32)&MsgGlowersMiserably;
			break;
		case ABILITY_DECOMPOSE:
			msg = (Sint32)&MsgSmellOfDecay;
			break;
		case ABILITY_BERSERK:
			msg = (Sint32)&MsgFuriousRage;
			break;
		case ABILITY_SPEED_SURGE:
			msg = (Sint32)&MsgAttemptsToDivide;
			break;
		case ABILITY_ALLY_SEARCH:
			msg = (Sint32)&MsgLooksForAllies;
			break;
		case ABILITY_BATTLE_CRY:
			msg = (Sint32)&MsgLooksForHelp;
			break;
		case ABILITY_PRESSURE:
			msg = (Sint32)&MsgCannotMove;
			break;
		}
		mnBattleMessageShowAndWaitStub(msg);
	} break;

	case COMMAND_DJINN:
		/* A Djinni on standby that has recovered is set again, and nothing else
		   happens; one still recovering says so. A set Djinni is unleashed. */
		abilityId = gmDjinnGetAbilityIdStub(DJINN_COMMAND_ELEMENT(request->prm),
			DJINN_COMMAND_INDEX(request->prm));
		if(!gmDjinnIsSetStub(request->unitId,
			   DJINN_COMMAND_ELEMENT(request->prm),
			   DJINN_COMMAND_INDEX(request->prm))) {
			if(gmDjinnCanSetStub(request->unitId,
				   DJINN_COMMAND_ELEMENT(request->prm),
				   DJINN_COMMAND_INDEX(request->prm))) {
				gmAbilityGetDirectStub(abilityId);
				btHighlightSetModes(0, HIGHLIGHT_OFF);
				gmDjinnSetStub(request->unitId,
					DJINN_COMMAND_ELEMENT(request->prm),
					DJINN_COMMAND_INDEX(request->prm));
				gmDjinnRecoveryRemoveStub(request->unitId,
					DJINN_COMMAND_ELEMENT(request->prm),
					DJINN_COMMAND_INDEX(request->prm));
				gmUnitCalcStub(request->unitId);
				btPlaybackReset();
				btPlaybackSchedulePhase(30);
				btPlaybackPush(PLAYBACK_UNIT_NAME, request->unitId);
				btPlaybackPush(PLAYBACK_ABILITY_NAME,
					DJINN_COMMAND_ELEMENT(request->prm) * DJINN_PER_ELEMENT +
						DJINN_COMMAND_INDEX(request->prm) +
						ABILITY_DJINNI_FIRST);
				btPlaybackPush(PLAYBACK_SOUND, SE_NOTE_HIGH);
				btPlaybackPush(PLAYBACK_DRAW_STATUS, 0);
				btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)&MsgDjinnIsSet);
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, request->unitId);
				sySndPlayStub(SE_POWER_UP);
				fdActorSelectAnimationStub(
					btObjectGet(request->unitId)->obj, POSE_CAST);
				fdActorSetAnimationSpeedStub(
					(ACTOR *)btObjectGet(request->unitId)->obj,
					ANIM_STEP_NORMAL * 2);
				btElementGlowPlay(request->unitId,
					DJINN_COMMAND_ELEMENT(request->prm), GLOW_BURST, 0);
				btPlaybackWaitForReady();
				return ACTION_NOTHING_TO_SHOW;
			}
			/* Recovering. */
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			sySndPlayStub(SE_MENU_ERROR);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgDjinnInRecovery);
			sySynch(60);
			return ACTION_REPORTED;
		}
		/* Unleash the set Djinni; it goes on standby. */
		{
			ABILITY_DEFINITION *ability;

			if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
				return ACTION_REPORTED;
			gmDjinnRecoveryAddStandbyStub(request->unitId,
				DJINN_COMMAND_ELEMENT(request->prm),
				DJINN_COMMAND_INDEX(request->prm));
			ability = gmAbilityGetDirectStub(abilityId);
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgActorUnleashesDjinn);
			plan->element = ability->element;
		}
		break;

	case COMMAND_SUMMON: {
		const SUMMON_DEFINITION *summon;
		DJINN_RECOVERY_LIST		*list;
		Sint32					 side;
		Uint8					 cnts[ELEMENT_COUNT];
		Sint32					 idx;

		/* Enough Djinn of each element on standby? A link battle counts the
		   summoner's side; any other counts the party's. */
		summon = gmSummonDefinitionGetNearStub(request->prm);
		btDjinnCountStandby(((BATTLE_STATE *)RamHeapSlots->battleState)->isLink
				? request->unitId
				: 0,
			cnts);
		side = DJINN_RECOVERY_PARTY;
		if((Uint16)request->unitId > ALLY_MAX &&
			((BATTLE_STATE *)RamHeapSlots->battleState)->isLink)
			side = DJINN_RECOVERY_OPPONENT;
		list = &gmDjinnGetRecoveryTableStub(side)->list;
		for(idx = 0;
			idx < ELEMENT_COUNT && cnts[idx] >= summon->djinnRequired[idx];
			idx++)
			cnts[idx] = summon->djinnRequired[idx];
		abilityId = summon->abilityId;
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		if(idx != ELEMENT_COUNT) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			mnBattleMessageShowAndWaitStub((Sint32)&MsgSummonLacksDjinn);
			return ACTION_REPORTED;
		}
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
		mnBattleMessageShowAndWaitStub((Sint32)&MsgActorSummons);
		/* Mark the standby Djinn being used DJINN_RECOVERY_SUMMONED, the first
		   found of each element, from units in the formation. */
		for(idx = 0; idx != list->cnt; idx++) {
			DJINN_RECOVERY_ENTRY *entry = &list->entries[idx];

			if(entry->turns == DJINN_RECOVERY_AFTER_BATTLE &&
				btUnitIsListed(entry->unitId) && cnts[entry->element] != 0) {
				entry->turns = DJINN_RECOVERY_SUMMONED;
				cnts[entry->element]--;
			}
		}
		/* Daedalus strikes again at the turn's end (btStagingPlanDaedalus). */
		if(abilityId == ABILITY_DAEDALUS) {
			if((Uint16)request->unitId > ALLY_MAX)
				battle->nextDaedalusFromEnemies |= 1
					<< battle->nextDaedalusStrikes;
			battle->nextDaedalusStrikes++;
		}
	} break;
	}

	if(abilityId == ABILITY_ATTACK) {
		UNIT *tgt = syUnitGet(plan->tgtIds[0]);

		plan->abilityId = ABILITY_ATTACK;
		plan->element = gmUnitGetAttackElementStub(request->unitId);
		plan->staging = STAGING_ATTACK;
		/* An enemy's attack effect comes from its class; a party member's from
		   who it is. */
		if(actor->classId == CLASS_NONE) {
			plan->battleEff =
				btSprGetAttackEffect(actor->enemyId) | EFFECT_BY_ID_RANGE;
		} else {
			plan->battleEff = 0;
			plan->battleEff = btUnitGetPartyAttackEffect(actor->enemyId);
		}
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((Sint32)&MsgActorAttacks);
		/* A deluded attacker misses CHANCE_DELUDED_MISS times in 256, and any
		   attack 1 in 32; a target that cannot move is never missed, and in the
		   Djinn tutorial the attack always misses. */
		do {
			if(tgt->hp == 0 || tgt->sleep != 0 || tgt->stun != 0 ||
				tgt->immobile != 0 || tgt->charm != 0)
				break;
			if(actor->delusion != 0 &&
				(gmBattleRandomStub() & 255) < CHANCE_DELUDED_MISS)
				plan->tgtMultiplier[0] = 0;
			if((gmBattleRandomStub() & 31) == 0)
				plan->tgtMultiplier[0] = 0;
		} while(0);
		if(syGameFlagGet(FLAG_DJINN_LESSON))
			plan->tgtMultiplier[0] = 0;
		/* Critical hit: one chance in 32, or else the equipment's unleash bonus
		   in 200ths, against a 16-bit roll. */
		if(tgt->hp != 0) {
			if((gmBattleRandomStub() & 31) == 0)
				plan->tgtCritical[0] = CRITICAL_HIT;
			else if(toFIXED(1) * gmEquipmentGetUnleashRateBonusStub(actor) /
					200 >
				(gmBattleRandomStub() & 0xffff))
				plan->tgtCritical[0] = CRITICAL_HIT;
		}
	} else {
		ABILITY_DEFINITION *ability = gmAbilityGetDirectStub(abilityId);
		Sint32				chance;
		Sint32				amount;

		plan->abilityId = abilityId;
		plan->element = ability->element;
		plan->battleEff = 0;
		if(ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
			ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
			ability->eff == ABILITY_EFFECT_DOUBLE_BLOW ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_RARE ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_LIKELY ||
			ability->eff == ABILITY_EFFECT_VENGEANCE) {
			Sint32 idx;

			/* Vengeance strikes one or two more times, a double or triple blow
			   one or two, each at its chance: the multiplier on every target
			   rises. */
			if(ability->eff == ABILITY_EFFECT_VENGEANCE) {
				Sint32 roll = gmBattleRandomStub() & 255;

				if(roll < CHANCE_VENGEANCE_EXTRA) {
					amount = 1;
					if(roll < CHANCE_VENGEANCE_TWICE)
						amount = 2;
					for(idx = 0; idx < plan->tgtCnt; idx++)
						plan->tgtMultiplier[idx] += amount;
				}
			} else {
				if(ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
					ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_LIKELY)
					chance = CHANCE_BLOW_LIKELY;
				else if(ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
					ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_RARE)
					chance = CHANCE_BLOW_RARE;
				else
					chance = CHANCE_BLOW;
				if(ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
					ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
					ability->eff == ABILITY_EFFECT_DOUBLE_BLOW)
					amount = 1;
				else
					amount = 2;
				if((gmBattleRandomStub() & 255) < chance) {
					for(idx = 0; idx < plan->tgtCnt; idx++)
						plan->tgtMultiplier[idx] += amount;
				}
			}
		} else if(ability->eff >= ABILITY_EFFECT_CRITICAL_A &&
			ability->eff <= ABILITY_EFFECT_CRITICAL_E) {
			/* An ability's critical hit on every target, 1 in 64 to 1 in 4. */
			Sint32 mask;

			switch(ability->eff) {
			case ABILITY_EFFECT_CRITICAL_A:
				mask = 64 - 1;
				break;
			case ABILITY_EFFECT_CRITICAL_B:
				mask = 32 - 1;
				break;
			case ABILITY_EFFECT_CRITICAL_C:
				mask = 16 - 1;
				break;
			case ABILITY_EFFECT_CRITICAL_D:
				mask = 8 - 1;
				break;
			case ABILITY_EFFECT_CRITICAL_E:
			default:
				mask = 4 - 1;
				break;
			}
			if((gmBattleRandomStub() & mask) == 0) {
				Sint32 idx;

				for(idx = 0; idx < plan->tgtCnt; idx++)
					plan->tgtCritical[idx] = CRITICAL_STRONG;
			}
		} else if(abilityId == ABILITY_ANNIHILATION ||
			abilityId == ABILITY_WHORL ||
			ability->eff == ABILITY_EFFECT_BLOW_AWAY) {
			/* Annihilation, Whorl and Gale roll their hit now, so that the
			   effect can show it (the variant below): against foes at the
			   falloff for each target's distance, at full rate otherwise. */
			Sint32 idx;

			for(idx = 0; idx < plan->tgtCnt; idx++) {
				Sint32 rate;

				if(ability->tgtMode == ABILITY_TARGET_FOES) {
					rate =
						ActionRunEffectChance[__builtin_abs(plan->tgtOfs[idx])];
				} else {
					rate = EFFECT_CHANCE_FULL;
				}
				plan->tgtHit[idx] = gmBattleRollEffectSuccessStub(
					request->unitId, plan->tgtIds[idx], ability->element,
					ability->eff, rate);
			}
		}
		if(abilityId == ABILITY_MELD) {
			plan->partnerId = btUnitFindStrongestAlly(plan->unitId);
		} else if(abilityId == ABILITY_MOLD) {
			Sint32 tries = 0;

			plan->partnerId = plan->tgtIds[0];
			do {
				plan->tgtIds[0] = btGetUnitAtPlace(
					btAIChooseRandomTarget(plan->partnerId <= ALLY_MAX));
				if(plan->tgtIds[0] != plan->partnerId)
					break;
			} while(++tries <= 9);
		}
		/* How it is staged: the ability's battle effect, stronger for a double
		   or triple blow, and its staging. */
		if(abilityId >= 0 && abilityId <= ABILITY_PSY_BOOST) {
			plan->battleEff = AbilityBattleEffects[abilityId];
			if(plan->tgtMultiplier[0] > 1)
				plan->battleEff = AbilityBattleEffects[abilityId] +
					(plan->tgtMultiplier[0] << EFFECT_VARIANT_SHIFT) -
					(1 << EFFECT_VARIANT_SHIFT);
		}
		if((abilityId >= 0 && abilityId <= ABILITY_PSY_BOOST) &&
			AbilityStaging[abilityId] != 0)
			plan->staging = AbilityStaging[abilityId];
		else if(btAbilityIsPsynergy(abilityId))
			plan->staging = STAGING_CAST;
		else if(plan->battleEff != 0) {
			if(actor->classId == CLASS_NONE)
				plan->staging = STAGING_ENEMY_CAST;
			else
				plan->staging = STAGING_CAST;
		} else
			plan->staging = STAGING_MESSAGES;
		if(gmBattleIsReviveEffectStub(ability->eff))
			plan->battleEff |= EFFECT_ON_DOWNED;
		/* When the hit rolled ahead lands on the first target, the effect plays
		   its next variant. */
		if((abilityId == ABILITY_ANNIHILATION || abilityId == ABILITY_WHORL ||
			   ability->eff == ABILITY_EFFECT_BLOW_AWAY) &&
			plan->tgtHit[0] != 0)
			plan->battleEff |= 1 << EFFECT_VARIANT_SHIFT;
	}
	/* An item is staged as an item, unless its ability takes the whole
	   screen. */
	if(request->command == COMMAND_ITEM && plan->staging != STAGING_SUMMON &&
		plan->staging != STAGING_FULL_SCREEN)
		plan->staging = STAGING_ITEM;
	plan->command = request->command;
	return 0;
}
