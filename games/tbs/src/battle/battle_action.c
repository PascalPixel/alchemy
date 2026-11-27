/* An action announced and built for its targets, btActionRunBuild. Part of
   battle.c. */

#include "battle_internal.h"

/* The chances, out of 256, an action rolls against the battle's random byte. */
typedef enum ACTION_CHANCE {
	CHANCE_DELUDED_MISS = 153, /* a deluded attacker misses */
	CHANCE_BLOW_LIKELY = 153,  /* a likely double or triple blow lands */
	CHANCE_BLOW_RARE = 32,	   /* a rare one */
	CHANCE_BLOW = 64		   /* any other */
} ACTION_CHANCE;

/* A party member's plain attack, by the beam it plays (BEAM_KIND,
   battle_effect_beams.c): one blue slash for Isaac, Garet and Jenna, three
   short blue arcs for Ivan and Mia. */
#define ATTACK_BEAM_SLASH 1
#define ATTACK_BEAM_WAVE  4

/* gmBattleRollEffectSuccess's successScale at full strength, in percent. */
#define EFFECT_CHANCE_FULL 100

/* Returns 1 when the unit is in its side's list of units in the battle
   (btUnitListIds), 0 otherwise. */
Uint32 btUnitIsListed(Uint32 unitId)
{
	Uint16 list[8];
	Sint32 side;
	Sint32 unitCnt;
	Sint32 unitIdx;

	side = BATTLE_SIDE_MASK_PARTY;
	if(unitId > ALLY_MAX)
		side = BATTLE_SIDE_MASK_ENEMIES;
	unitCnt = btUnitListIds(side, list);

	for(unitIdx = 0; unitIdx < unitCnt; unitIdx++) {
		if(list[unitIdx] == unitId)
			break;
	}

	return unitIdx != unitCnt;
}

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
	Sint32				 enemySide;

	standbyCnt = 0;
	sideMask = BATTLE_SIDE_MASK_PARTY;
	if(unitId > ALLY_MAX)
		sideMask = BATTLE_SIDE_MASK_ENEMIES;
	unitCnt = btUnitListIds(sideMask, unitIds);
	enemySide = DJINN_RECOVERY_PARTY;
	if(unitId > ALLY_MAX)
		enemySide = DJINN_RECOVERY_OPPONENT;
	list = &gmDjinnGetRecoveryTableStub(enemySide)->list;
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

/* Announces the action and builds it for its targets, paying its PP or Djinn.
   Returns 0 to stage it, ACTION_REPORTED when it failed with its message shown,
   or ACTION_NOTHING_TO_SHOW. */
Sint32 btActionRunBuild(BATTLE_ACTION *request, BATTLE_ACTION_RUN *plan)
{
	UNIT		 *actor = gmUnitGetStub(request->unitId);
	BATTLE_STATE *battle = BattleStateWork;
	Sint32		  selectedTgtId;
	Sint32		  abilityId;

	/* Fills the action's targets: the aimed unit, or the places within its
	   range; with none, the unit defends. */
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

		switch(ability->eff) {
		case ABILITY_EFFECT_REVIVE:
		case ABILITY_EFFECT_REVIVE_HALF:
		case ABILITY_EFFECT_DEW:
			includeDowned = TRUE;
			break;
		}
		/* No target mode, and the user alone: the aimed unit, as one target. */
		switch(mode) {
		case ABILITY_TARGET_ONE:
			plan->tgtOfs[0] = mode;
			plan->tgtCnt = 1;
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
			for(place = 0; battle->partyUnits[place] != BATTLE_UNIT_LIST_END;
				place++)
				;
			partyCount = place;
			for(place = 0; battle->enemyIds[place] != BATTLE_UNIT_LIST_END;
				place++)
				;
			enemyCount = place;
			aimedPlace = request->tgt & PLACE_INDEX_MASK;
			range = request->range;
			{
				Sint32 first = aimedPlace - range + 1;
				Sint32 last = aimedPlace + range - 1;

				for(place = first; place <= last; place++) {
					if(place < 0)
						continue;
					/* The enemies' side. */
					if(request->tgt & PLACE_ENEMY_SIDE) {
						if(place >= enemyCount)
							continue;
						targetId = battle->enemyIds[place];
						if(targetId == BATTLE_UNIT_REMOVED)
							continue;
						if(!includeDowned && gmUnitGetStub(targetId)->hp == 0)
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
						if(!includeDowned && gmUnitGetStub(targetId)->hp == 0)
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
				mnBattleMessageShowAndWaitStub(MsgActorDefends);
				if(actor->defendLevel == 0)
					actor->defendLevel = DEFEND_NORMAL;
				return ACTION_REPORTED;
			}
		}
#if FIX_FUNCTION_SIGNATURES
		/* Callers test only -1. */
		return plan->tgtCnt;
#endif
	}

	/* A fresh action run, and the debug defeat. */
	selectedTgtId = btGetUnitAtPlace(request->tgt);
	btPlaybackReset();
	plan->hauntDamage = 0;
	plan->unitId = request->unitId;
	plan->tgtCnt = 0;
	plan->battleEff = 0;
	plan->failure = ACTION_FAILURE_NONE;
	plan->element = ELEMENT_NONE;
	mnMessageArgumentClearAllStub();
	if(actor->hp == 0)
		return ACTION_NOTHING_TO_SHOW;

	/* Debug, with FLAG_DEBUG_BATTLE_KEYS set: hold R to defeat all enemies, or
	   R and Select the whole party. */
	if(DebugMode && gmFlagGetStub(FLAG_DEBUG_BATTLE_KEYS) &&
		(PerData & KEY_R)) {
		Bool   enemy = TRUE;
		Sint32 idx;
		Sint32 id;

		if(PerData & KEY_SELECT)
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

	/* What stops the unit: Ground, sleep, stun, a bound unit's bad luck. */
	mnMessageArgumentClearAllStub();
	if(actor->immobile) {
		actor->immobile = 0;
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgUnableToMove);
		return ACTION_REPORTED;
	}
	if(actor->sleep) {
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgIsAsleep);
		return ACTION_REPORTED;
	}
	if(actor->stun) {
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgIsParalyzed);
		return ACTION_REPORTED;
	}
	/* A bound unit (curse bit 0) loses one action in four, except Defend. */
	if((actor->curse & CURSE_BOUND) && request->command != COMMAND_DEFEND &&
		(gmBattleRandomStub() & 3) == 0) {
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgIsBound);
		return ACTION_REPORTED;
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
			mnBattleMessageShowAndWaitStub(MsgPartyFlees);
		} else {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub(MsgActorRuns);
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
			mnBattleMessageShowAndWaitStub(MsgWeaponHowls);
			btPlaybackMarkMessage();
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			mnBattleMessageShowAndWaitStub(MsgWeaponHowls + 1);
		}
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
		mnBattleMessageShowAndWaitStub(MsgActorCasts);
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
			CLAMP(actor->pp, 0, actor->ppMax);
		}
	} break;

	case COMMAND_ITEM: {
		ITEM_DEFINITION *item;

		/* A slot of -1: an earlier command this turn used the item up
		   (btStagingRunItem). */
		if(request->prm < 0) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub(MsgItemAlreadyUsed);
			return ACTION_REPORTED;
		}
		/* An item with no ability, or a broken one, and the user defends. */
		item = gmItemGetDirectStub(actor->inventory[request->prm]);
		abilityId = item->abilityId;
		if(abilityId == 0 ||
			(actor->inventory[request->prm] & INVENTORY_BROKEN)) {
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub(MsgActorDefends);
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
			case ITEM_TYPE_UNUSED:
			case ITEM_TYPE_RING:
				mnBattleMessageShowAndWaitStub(MsgActorRaisesItem);
				break;
			default:
				mnBattleMessageShowAndWaitStub(MsgActorUsesBattleItem);
				break;
			}
		} else {
			mnBattleMessageShowAndWaitStub(MsgActorUsesBattleItem);
		}
	} break;

	case COMMAND_DEFEND:
	case COMMAND_TOTAL_DEFENSE:
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgActorDefends);
		return ACTION_REPORTED;

	case COMMAND_NONE:
		return ACTION_NOTHING_TO_SHOW;

	case COMMAND_ENEMY_ABILITY: {
		Sint32 msg;

		abilityId = request->prm;
		if(btActionRunSetTargets(abilityId) == ACTION_REPORTED)
			return ACTION_REPORTED;
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
		if((gmAbilityGetDirectStub(abilityId)->flg & ABILITY_FORMULA_MASK) ==
			ABILITY_FORMULA_ENEMY_DAMAGE)
			msg = MsgActorUnleashesAbility;
		else
			msg = MsgActorUsesAbility;
		/* Some enemy abilities have their own wording. */
		switch(abilityId) {
		case ABILITY_SONIC_WAVE:
		case ABILITY_SONIC_WAVE_STRONG:
			msg = MsgEmitsUltrasonicWaves;
			break;
		case ABILITY_HEAT_MIRAGE:
			msg = MsgActorCasts;
			break;
		case ABILITY_THREATEN:
			msg = MsgGlowersFerociously;
			break;
		case ABILITY_TREMBLE:
			msg = MsgIsIntimidated;
			break;
		case ABILITY_CONTAIN:
			msg = MsgBalefulGaze;
			break;
		case ABILITY_WORMS:
			msg = MsgEatsWorms;
			break;
		case ABILITY_SHRIEK:
		case ABILITY_BANSHEE_HOWL:
		case ABILITY_CRAZY_VOICE:
		case ABILITY_WAR_CRY:
		case ABILITY_WICKED_HOWL:
			msg = MsgLetsOutAbility;
			break;
		case ABILITY_WING_BEAT:
		case ABILITY_WING_FLUTTER:
		case ABILITY_WING_STROKE:
			msg = MsgActorUsesAbility;
			break;
		case ABILITY_ROTTEN_BLOOD:
			msg = MsgEnemyUnleashesAbility;
			break;
		case ABILITY_HEARTRENDER:
			msg = MsgGlowersMiserably;
			break;
		case ABILITY_DECOMPOSE:
			msg = MsgSmellOfDecay;
			break;
		case ABILITY_BERSERK:
			msg = MsgFuriousRage;
			break;
		case ABILITY_SPEED_SURGE:
			msg = MsgAttemptsToDivide;
			break;
		case ABILITY_ALLY_SEARCH:
			msg = MsgLooksForAllies;
			break;
		case ABILITY_SEARCH_FOR_HELP:
			msg = MsgLooksForHelp;
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
				btPlaybackPush(PLAYBACK_MESSAGE, MsgDjinnSet);
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, request->unitId);
				sySndPlayStub(SE_POWER_UP);
				fdActorSelectAnimationStub(
					btObjectGet(request->unitId)->actor, POSE_CAST);
				fdActorSetAnimationSpeedStub(
					btObjectGet(request->unitId)->actor, ANIM_STEP_NORMAL * 2);
				btElementGlowPlay(request->unitId,
					DJINN_COMMAND_ELEMENT(request->prm), GLOW_BURST, 0);
				btPlaybackWaitForReady();
				return ACTION_NOTHING_TO_SHOW;
			}
			/* Recovering. */
			mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
			sySndPlayStub(SE_MENU_ERROR);
			mnBattleMessageShowAndWaitStub(MsgDjinnInRecovery);
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
			mnBattleMessageShowAndWaitStub(MsgActorUnleashesDjinn);
			plan->element = ability->element;
		}
		break;

	case COMMAND_SUMMON: {
		const SUMMON_DEFINITION *summon;
		DJINN_RECOVERY_LIST		*list;
		Sint32					 side;
		Uint8					 cnts[ELEMENT_COUNT];
		/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
		 * touches. */
		UNUSED Uint8 unused[8];
		Sint32		 idx;

		/* Enough Djinn of each element on standby? */
		summon = gmSummonDefinitionGetNearStub(request->prm);
		btDjinnCountStandby(request->unitId, cnts);
		side = DJINN_RECOVERY_PARTY;
		if((Uint16)request->unitId > ALLY_MAX)
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
			mnBattleMessageShowAndWaitStub(MsgSummonLacksDjinn);
			return ACTION_REPORTED;
		}
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPushStub(abilityId, MESSAGE_ARGUMENT_ABILITY);
		mnBattleMessageShowAndWaitStub(MsgActorSummons);
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
	} break;
	}

	/* A normal attack. */
	if(abilityId == ABILITY_ATTACK) {
		UNIT *tgt = gmUnitGetStub(plan->tgtIds[0]);

		plan->abilityId = ABILITY_ATTACK;
		plan->element = gmUnitGetAttackElementStub(request->unitId);
		plan->staging = STAGING_ATTACK;
		/* An enemy's attack effect comes from its battle sprite's row; a party
		   member's from who it is. */
		if(actor->classId == CLASS_NONE) {
			plan->battleEff =
				btSprGetAttackEffect(actor->id) | EFFECT_BY_ID_RANGE;
		} else {
			plan->battleEff = 0;
			switch(actor->id) {
			case ISAAC:
				plan->battleEff = EFFECT_BY_ID_RANGE | ATTACK_BEAM_SLASH;
				break;
			case GARET:
				plan->battleEff = EFFECT_BY_ID_RANGE | ATTACK_BEAM_SLASH;
				break;
			case IVAN:
				plan->battleEff = EFFECT_BY_ID_RANGE | ATTACK_BEAM_WAVE;
				break;
			case MIA:
				plan->battleEff = EFFECT_BY_ID_RANGE | ATTACK_BEAM_WAVE;
				break;
			case JENNA:
				plan->battleEff = EFFECT_BY_ID_RANGE | ATTACK_BEAM_SLASH;
				break;
			}
		}
		mnMessageArgumentPushStub(request->unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgActorAttacks);
		/* A deluded attacker misses 153 times in 256, and any attack 1 in 32; a
		   target that cannot move is never missed, and in the Djinn tutorial
		   the attack always misses. */
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
		if(gmFlagGetStub(FLAG_DJINN_LESSON))
			plan->tgtMultiplier[0] = 0;
		/* Critical hit: one chance in 32, or else the equipment's unleash bonus
		   in 200ths, against a 16-bit roll. */
		if(tgt->hp != 0) {
			if((gmBattleRandomStub() & 31) == 0)
				plan->tgtCritical[0] = CRITICAL_HIT;
			else if(toFIXED(gmEquipmentGetUnleashRateBonusStub(actor)) / 200 >
				(gmBattleRandomStub() & 0xffff))
				plan->tgtCritical[0] = CRITICAL_HIT;
		}
	}

	/* An ability. */
	else {
		ABILITY_DEFINITION *ability = gmAbilityGetDirectStub(abilityId);

		plan->element = ability->element;
		plan->battleEff = 0;
		plan->abilityId = abilityId;
		if(ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_LIKELY ||
			ability->eff == ABILITY_EFFECT_DOUBLE_BLOW_RARE ||
			ability->eff == ABILITY_EFFECT_DOUBLE_BLOW ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_RARE ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW ||
			ability->eff == ABILITY_EFFECT_TRIPLE_BLOW_LIKELY) {
			/* A double or triple blow on every target at its chance, the
			   multiplier rising by 1 or 2. */
			Sint32 chance;
			Sint32 amount;

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
				Sint32 idx;

				for(idx = 0; idx < plan->tgtCnt; idx++)
					plan->tgtMultiplier[idx] += amount;
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
		} else if(abilityId == ABILITY_ANNIHILATION) {
			/* Annihilation rolls its hit now, at full rate whatever the
			   distance, so that the effect can show it (the variant below). */
			Sint32 idx;

			for(idx = 0; idx < plan->tgtCnt; idx++)
				plan->tgtHit[idx] = gmBattleRollEffectSuccessStub(
					request->unitId, plan->tgtIds[idx], ability->element,
					ability->eff, EFFECT_CHANCE_FULL);
		}
		/* How it is staged: the ability's battle effect, stronger for a double
		   or triple blow, and its staging. */
		if(abilityId >= 0 && abilityId <= ABILITY_THRASH) {
			plan->battleEff = AbilityBattleEffects[abilityId];
			if(plan->tgtMultiplier[0] > 1)
				plan->battleEff = AbilityBattleEffects[abilityId] +
					(plan->tgtMultiplier[0] << EFFECT_VARIANT_SHIFT) -
					(1 << EFFECT_VARIANT_SHIFT);
		}
		if((abilityId >= 0 && abilityId <= ABILITY_SEVERE_BLOW) &&
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
		/* When Annihilation's instant death lands on its first target, the
		   burst shows its second picture. */
		if(abilityId == ABILITY_ANNIHILATION && plan->tgtHit[0] != 0)
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
