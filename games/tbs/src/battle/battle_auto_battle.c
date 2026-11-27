/* Auto battle: the actions it builds and plays, without the command menu. Part
   of battle.c. */

#include "battle_internal.h"

/* Auto battle: gives every living unit a plain attack and sorts them into
   action order. Returns how many actions there are. */
Sint32 btAutoBattleBuildActions(BATTLE_ACTION *entries)
{
	BATTLE_ACTION  swap;
	Uint16		   unitIds[UNIT_MAX];
	Sint32		   partyCnt;
	Sint32		   cnt = 0;
	Sint32		   enemyCnt;
	Sint32		   tgtRange;
	Sint32		   idx;
	BATTLE_ACTION *entry;
	UNIT		  *unit;

	/* The party's actions. The four lookups' results are never used. */
	partyCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
	for(idx = 0; idx != PARTY_BATTLE_SIZE; idx++)
		gmUnitGetStub(idx);
	for(idx = 0; idx < partyCnt; idx++) {
		Sint32 unitId = unitIds[idx];

		unit = gmUnitGetStub(unitId);
		entry = &entries[idx];
		entry->unitId = unitId;
		entry->pri = unit->agility;
		entry->command = COMMAND_ATTACK;
		entry->prm = 0;
		entry->tgt = FIRST_ENEMY_UNIT;
		cnt++;
	}
	/* The enemies attack a random party member, replaced when the action plays
	   if it is down. */
	enemyCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
	entry = &entries[partyCnt];
	tgtRange = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, NULL);
	for(idx = 0; idx < enemyCnt; idx++) {
		Sint32 unitId = unitIds[idx];

		unit = gmUnitGetStub(unitId);
		entry->unitId = unitId;
		entry->pri = unit->agility >> 1;
		if(entry->pri != 0)
			entry->pri += syRandom16Below(unit->agility);
		entry->command = COMMAND_ATTACK;
		entry->prm = 0;
		entry->tgt = syRandom16Below(tgtRange);
		cnt++;
		entry++;
	}

	/* Bubble the faster units forward. */
	/* BUG: one pass short of a full sort; with two actions nothing is
	   sorted. */
	for(idx = cnt - 2; idx > 0; idx--) {
		Sint32 swaps = 0;
		Sint32 pos;

		for(pos = cnt - 1; pos > 0; pos--) {
			if(entries[pos].pri > entries[pos - 1].pri) {
				syDMAXCopy(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
				syDMAXCopy(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4),
					REG_DMA3);
				syDMAXCopy(
					&swap, &entries[pos - 1], DMA_COPY_WORDS(4), REG_DMA3);
				swaps++;
			}
		}
		if(swaps == 0)
			break;
	}
	return cnt;
}

/* Unused: turns the camera while L or R is held. */
DEFINE_ADJUST_CAMERA(btCameraAdjustByShoulderKeys)

/* Auto battle: plays one action, a defeated target replaced by a living unit of
   its side. Returns 1 when the party got away, -1 when the unit is down. */
Sint32 btAutoBattleRunAction(BATTLE_ACTION *action)
{
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 unitId;
	Sint32					 yaw;
	UNIT					*actor;

	unitId = action->unitId;
	actor = gmUnitGetStub(unitId);
	if(actor->hp == 0)
		return -1;

	action->tgt = btAbilityReplaceDefeated(action);
	view = BattleBackgroundStateWork;
	/* Here any unit id above Felix's counts as an enemy, Jenna's included. */
	if(action->unitId > FELIX)
		yaw = YAW_ENEMY_ACTION;
	else
		yaw = YAW_PARTY_ACTION;
	view->tgtYaw = yaw;
	view->ticks = 60;
	mnMessageArgumentClearAllStub();

	switch(action->command) {
	case COMMAND_FLEE:
		mnBattleMessageShowAndWaitStub(MsgPartyFlees);
		if(btFleePlayRun(action) != 0)
			return 1;
		break;
	case COMMAND_DEFEND:
		sySynch(45);
		btAutoBattleShowDefend(action);
		break;
	case COMMAND_ITEM:
		sySynch(45);
		btAutoBattleRunAttackOrItem(action);
		break;
	case COMMAND_ATTACK:
	default: {
		BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;
		view->keepRange = 0;
		btAutoBattleRunAttackOrItem(action);
		view->keepRange = 0;
		break;
	}
	case COMMAND_PSYNERGY:
		btAutoBattleRunPsynergy(action);
		break;
	}

	mnMessageResetIdleStub();
	return 0;
}

#include "battle/object_is_valid_id.c"

/* Auto battle: the party tries to run, and gets away 15 times in 16. Returns 1
   when it got away. */
Bool btFleePlayRun(BATTLE_ACTION *action)
{
	Sint16 partyMembers[UNIT_MAX];
	Sint32 partySize;
	Sint32 memberSlot;

	(void)action;
	/* Flee unless a roll below 16 comes up 0, 15 in 16. */
	if(syRandom16Below(16) != 0) {
		partySize = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, partyMembers);
		for(memberSlot = 0; memberSlot != partySize; memberSlot++) {
			btMotionSetupEscapeObject(partyMembers[memberSlot]);
			sySynch(8);
		}
		sySynch(22);
		return TRUE;
	}
	mnBattleMessageShowAndWaitStub(MsgCannotEscape);
	return FALSE;
}

/* Auto battle: shows that a standing unit is defending. The message is `<unit>
   is defending`. Returns -1 for an invalid unit id, 0 otherwise. */
Sint32 btAutoBattleShowDefend(const BATTLE_ACTION *action)
{
	Sint32 unitId;
	Sint32 result = 0;
	UNIT  *unit;

	unitId = action->unitId;
	unit = gmUnitGetStub(unitId);
	if(btObjectIsValidId(unitId) < 0) {
		return -1;
	}
	if(unit->hp <= 0) {
		return result;
	}
	mnMessageArgumentClearAllStub();
	mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	mnBattleMessageShowAndWaitStub(MsgActorDefends);
	return 0;
}

/* Auto battle: casts Psynergy, the caster's side fading out while its effect
   plays on the target side. */
Sint32 btAutoBattleRunPsynergy(BATTLE_ACTION *action)
{
	Uint16				 visibleUnits[UNIT_MAX];
	BATTLE_STAGING_STATE context;
	Sint32				 unitId;
	Sint32				 tgtId;
	Uint32				 sideStart;
	Uint32				 sideCnt;
	Uint32				 livingCnt;
	ACTOR				*casterActor;
	Uint32				 idx;

	livingCnt = 0;
	unitId = action->unitId;
	if(btObjectIsValidId(unitId) < 0) {
		return -1;
	}
	tgtId = action->tgt;
	if(btObjectIsValidId(tgtId) < 0) {
		return -1;
	}

	{
		BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;
		/* An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
		   YAW_ENEMY_ACTION. */
		view->tgtYaw =
			action->unitId > FELIX ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
		view->ticks = 60;
	}
	sySynch(10);
	/* A roll nothing uses, which still advances the random number generator. */
	syRandom();
	casterActor = btObjectGet(unitId)->actor;

	/* The caster's side, taken as the side the target is not on, and the
	   caster's POSE_CAST when its id is among that side's first sideCnt
	   ids. */
	if(tgtId >= 0 && tgtId <= ALLY_MAX) {
		sideCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
		sideStart = FIRST_ENEMY_UNIT;
	} else {
		sideCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
		sideStart = 0;
	}

	for(idx = 0; idx != sideCnt; idx++) {
		if(idx + sideStart == unitId) {
			fdActorSelectAnimationStub(casterActor, POSE_CAST);
		}
	}

	/* Fade the caster's side out over 16 ticks, and close the party status
	   window. */
	sySynch(30);
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(idx = 0; idx != sideCnt; idx++) {
		btHighlightSetSprMode(idx + sideStart, SPRITE_SEMI_TRANSPARENT);
	}
	for(idx = 0; idx != BLEND_WEIGHT_MAX; idx++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - idx, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	mnPartyStatDrawStub(PARTY_STATUS_CLOSED);

	/* Ready only the target side's living units, and play the effect. */
	if(tgtId > FIRST_ENEMY_UNIT - 1) {
		Uint32 cnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
		for(idx = 0; livingCnt != cnt; idx++) {
			Uint32 unit = idx + FIRST_ENEMY_UNIT;
			if(gmUnitGetStub(unit)->hp > 0) {
				visibleUnits[livingCnt++] = unit;
			}
		}
	} else {
		Uint32 cnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
		for(idx = 0; livingCnt != cnt; idx++) {
			if(gmUnitGetStub(idx)->hp > 0) {
				visibleUnits[livingCnt++] = idx;
			}
		}
	}
	visibleUnits[livingCnt] = BATTLE_UNIT_LIST_END;
	btObjectSpawnObjectsForList(visibleUnits, 0);

	context.eff.kind = action->prm;
	context.eff.user = unitId;
	for(idx = 0; idx != livingCnt; idx++) {
		context.eff.tgts[idx] = visibleUnits[idx];
	}
	context.eff.tgtCnt = livingCnt;
	if(tgtId >= 0 && tgtId <= ALLY_MAX) {
		context.eff.side = EFFECT_SIDE_PARTY;
	} else {
		context.eff.side = EFFECT_SIDE_ENEMIES;
	}
	btEffectRunSummonStub(&context.eff);

	/* Bring every unit back and fade the caster's side in. */
	sySynch(10);
	btObjectPut();
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(idx = 0; idx != sideCnt; idx++) {
		btHighlightSetSprMode(idx + sideStart, SPRITE_SEMI_TRANSPARENT);
	}
	for(idx = 0; idx != BLEND_WEIGHT_MAX; idx++) {
		REG_BLDALPHA = BLEND_ALPHA(idx, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	for(idx = 0; idx != sideCnt; idx++) {
		btHighlightSetSprMode(idx + sideStart, SPRITE_OPAQUE);
	}
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
	sySynch(3);
	return 0;
}

/* Unused auto battle attack: "<unit> attacks!", the attacker runs up to its
   target, the blow, and both return. */
Sint32 btAutoBattleRunApproachAttack(BATTLE_ACTION *input)
{
	BATTLE_STAGING_STATE state;

	/* A short wait when the camera already faces the party's side. */
	if(BattleBackgroundStateWork->tgtYaw == YAW_PARTY_ACTION) {
		BattleBackgroundStateWork->tgtYaw = YAW_PARTY_ACTION;
		sySynch(10);
	} else {
		BattleBackgroundStateWork->tgtYaw = YAW_PARTY_ACTION;
		sySynch(30);
	}

	state.eff.user = input->unitId;
	if(btObjectIsValidId(state.eff.user) < 0)
		return -1;

	state.eff.tgts[0] = input->tgt;
	if(btObjectIsValidId(state.eff.tgts[0]) < 0)
		return -1;

	/* Lookups whose results are unused, and a roll that only advances the
	   random number generator. */
	gmUnitGetStub(state.eff.user);
	gmUnitGetStub(state.eff.tgts[0]);
	syRandom();
	mnMessageArgumentPushStub(state.eff.user, MESSAGE_ARGUMENT_ACTOR);
	mnBattleMessageShowAndWaitStub(MsgActorAttacks);
	btMotionApproachTarget(state.eff.user, state.eff.tgts[0], 13, 0);
	fdActorSetAnimationSpeedStub(
		btObjectGet(state.eff.user)->actor, ANIM_STEP_NORMAL);
	btObjectGet(state.eff.tgts[0]);

	state.eff.tgtCnt = 1;
	if((Uint16)state.eff.tgts[0] <= ALLY_MAX)
		state.eff.side = EFFECT_SIDE_PARTY;
	else
		state.eff.side = EFFECT_SIDE_ENEMIES;
	state.eff.prelude = 0;

	sySynch(4);
	btEffectRunByIdRangeStub(&state.eff);
	btMotionReturnToSpot(state.eff.tgts[0]);
	btMotionReturnToSpot(state.eff.user);
	return 0;
}

/* Auto battle: an attack or an item, played on the first living unit of the
   target's side. */
Sint32 btAutoBattleRunAttackOrItem(BATTLE_ACTION *action)
{
	BATTLE_STAGING_STATE	 state;
	BATTLE_BACKGROUND_STATE *view = BattleBackgroundStateWork;
	ACTOR					*userActor;
	Sint32					 tgtYaw;
	Sint32					 tgtIdx;

	/* As in btAutoBattleRunAction, Jenna counts as an enemy here. */
	if(action->unitId > FELIX)
		tgtYaw = YAW_ENEMY_ACTION;
	else
		tgtYaw = YAW_PARTY_ACTION;
	if(view->tgtYaw == tgtYaw) {
		view->ticks = 40;
		sySynch(40);
	} else {
		view->tgtYaw = tgtYaw;
		view->ticks = 40;
		sySynch(40);
	}
	state.eff.kind = action->prm;
	state.eff.range = action->range;
	state.eff.user = action->unitId;
	state.eff.firstTgt = action->tgt;
	if(btObjectIsValidId(state.eff.user) < 0)
		return -1;
	if(state.eff.firstTgt > FIRST_ENEMY_UNIT - 1)
		state.eff.tgtCnt =
			btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, state.eff.tgts);
	else
		state.eff.tgtCnt =
			btUnitListLiving(BATTLE_SIDE_MASK_PARTY, state.eff.tgts);
	/* The status window without its PP row (see PARTY_STATUS_SUMMONS). */
	mnPartyStatDrawStub(BattleStateWork->partyStat & ~PARTY_STATUS_SHOW_PP);
	/* The unit raises its hand (POSE_CAST); then only the first living unit of
	   the target's side takes the effect. */
	userActor = btObjectGet(state.eff.user)->actor;
	fdActorSelectAnimationStub(userActor, POSE_CAST);
	fdActorSetAnimationSpeedStub(userActor, ANIM_STEP_NORMAL);
	if((Uint16)action->tgt <= ALLY_MAX) {
		state.eff.side = EFFECT_SIDE_PARTY;
		btUnitListLiving(BATTLE_SIDE_MASK_PARTY, state.eff.tgts);
		state.eff.tgtCnt = 1;
	} else {
		state.eff.side = EFFECT_SIDE_ENEMIES;
		btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, state.eff.tgts);
		state.eff.tgtCnt = 1;
	}

	/* Keep each target's sprite entries' parameters for the effect. */
	for(tgtIdx = 0; tgtIdx != state.eff.tgtCnt; tgtIdx++) {
		SPR	  *spr = btObjectGet(state.eff.tgts[tgtIdx])->actor->spr;
		Sint32 entryIdx;

		for(entryIdx = 0; entryIdx != spr->cnt - 1; entryIdx++)
			state.savedAnmPrms[tgtIdx][entryIdx] =
				spr->entries[entryIdx]->colMode;
	}
	/* The four elements' pillars in turn, then the blow. */
	state.eff.kind = ELEMENT_VENUS;
	state.eff.variant = 0;
	btEffectRunElementPillarStub(&state.eff);
	state.eff.kind = ELEMENT_MERCURY;
	btEffectRunElementPillarStub(&state.eff);
	state.eff.kind = ELEMENT_MARS;
	btEffectRunElementPillarStub(&state.eff);
	state.eff.kind = ELEMENT_JUPITER;
	btEffectRunElementPillarStub(&state.eff);
	state.eff.kind = 0;
	/* The blow goes through the dispatch on its kind, which carries no
	   EFFECT_BY_ID_RANGE: it runs from the handler table. */
	BATTLE_EFFECT_RUN(&state.eff, state.eff.kind);
	fdActorSelectAnimationStub(userActor, POSE_STANDING);
	for(tgtIdx = 0; tgtIdx != state.eff.tgtCnt; tgtIdx++)
		btMotionReturnToSpot(state.eff.tgts[tgtIdx]);
	btMotionReturnToSpot(state.eff.user);
	return 0;
}

/* Unused auto battle damage: takes damage HP from a unit and shows it, with "A
   bitter blow!" or "A critical hit!" first when showMsg is set. */
void btAutoBattleApplyDamage(
	Uint32 unitId, Sint32 damage, Sint32 showMsg, Uint8 *context)
{
	Uint8		   localContext[4];
	UNIT		  *unit;
	BATTLE_OBJECT *battleObject;

	if(context == 0) {
		context = localContext;
		context[0] = 0;
		context[1] = 0;
		context[2] = 0;
		context[3] = 0;
	}

	unit = gmUnitGetStub(unitId);
	unit->hp -= damage;
	if(unit->hp < 0)
		unit->hp = 0;

	/* The damage pose, and the status window with names and HP only. */
	battleObject = btObjectGet(unitId);
	fdActorSelectAnimationStub(battleObject->actor, POSE_DAMAGE);
	mnPartyStatDrawStub(PARTY_STATUS_HP_ONLY);
	mnMessageArgumentClearAllStub();

	if(unitId <= ALLY_MAX) {
		if(showMsg != 0)
			mnBattleMessageShowAndWaitStub(MsgBitterBlow);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgDmgP);
	} else {
		if(showMsg != 0)
			mnBattleMessageShowAndWaitStub(MsgCritical);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgDmgE);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	}

	btMotionPlayFall(unitId);
	if(unitId <= ALLY_MAX) {
		if(unit->hp <= 0) {
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub(MsgGoesDown);
		}
	} else if(unit->hp <= 0) {
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub(MsgDowned);
	}

	battleObject = btObjectGet(unitId);
	fdActorSelectAnimationStub(battleObject->actor, POSE_STANDING);
}

/* For a unit whose HP has run out: plays its falling effect to the last tick
   and marks its battle object for release (btObjectMarkForRelease). */
void btMotionPlayFall(Sint32 unitId)
{
	SPR_ANIMATION *entry;
	SPR			  *spr;
	if(gmUnitGetStub(unitId)->hp <= 0) {
		/* POSE_DAMAGE in grey, its first entry showing SPRITE_PICTURE_NONE. */
		spr = btObjectGet(unitId)->actor->spr;
		fdSprStartAnimationStub(spr, POSE_DAMAGE);
		entry = spr->entries[0];
		entry->colMode = SPRITE_COLOR_GREY;
		entry->picture = SPRITE_PICTURE_NONE;
		sySynch(4);
		fdSprDissolveRunStub((Uint8 *)spr);
		btObjectMarkForRelease(unitId);
	}
}

/* btAbilityReplaceDefeated's result when the target's side is all down, which
   the battle's end comes before. */
#define TARGET_SIDE_DEFEATED 256

/* Returns the action's target, or a random living unit of its side when the
   target is down; TARGET_SIDE_DEFEATED when none is left. */
Sint32 btAbilityReplaceDefeated(const BATTLE_ACTION *action)
{
	Uint16 livingUnits[UNIT_MAX];
	Sint32 tgtId;
	Sint32 livingCnt;

	tgtId = action->tgt;
	if(gmUnitGetStub(tgtId)->hp != 0) {
		return tgtId;
	}

	if(tgtId > FIRST_ENEMY_UNIT - 1) {
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
	} else {
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
	}

	if(livingCnt == 0) {
		return TARGET_SIDE_DEFEATED;
	}

	return (Sint16)livingUnits[syRandom16Below(livingCnt)];
}
