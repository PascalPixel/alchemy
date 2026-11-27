/* Auto battle: the actions it builds and plays without the command menu, the
   shoulder-key camera and the escape. Part of battle.c. */

/* Auto battle: gives every living unit a plain attack and sorts them into
   action order. Returns how many actions there are. */
Sint32 btAutoBattleBuildActions(BATTLE_ACTION_RECORD *entries)
{
	BATTLE_ACTION_RECORD  swap;
	Uint16				  unitIds[UNIT_MAX];
	Sint32				  partyCnt;
	Sint32				  cnt = 0;
	Sint32				  enemyCnt;
	Sint32				  tgtRange;
	Sint32				  idx;
	BATTLE_ACTION_RECORD *entry;
	UNIT				 *unit;

	/* The party's actions. */
	partyCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
	/* DEAD: syUnitGet's results are not used; the ROM keeps the calls. */
	for(idx = 0; idx != PARTY_BATTLE_SIZE; idx++)
		syUnitGet(idx);
	for(idx = 0; idx < partyCnt; idx++) {
		Sint32 unitId = unitIds[idx];

		unit = syUnitGet(unitId);
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

		unit = syUnitGet(unitId);
		entry->unitId = unitId;
		entry->pri = unit->agility >> 1;
		if(entry->pri != 0)
			entry->pri += (Uint32)(syRandom() * unit->agility) >> 16;
		entry->command = COMMAND_ATTACK;
		entry->prm = 0;
		entry->tgt = syRandomBelow(tgtRange);
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
			if((Sint16)entries[pos].pri > (Sint16)entries[pos - 1].pri) {
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

/* Unused: turns the battle camera while L or R is held, a 128th of a turn each
   tick, and redraws the zoomed background unless a background holds it. */
UNUSED void btCameraAdjustByShoulderKeys(void)
{
	BATTLE_CAMERA			*camera = RamHeapSlots->cameraState;
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;

	if((PerWork.data & KEY_L) != 0) {
		camera->yaw += TRIG_TURN / 128;
	}
	if((PerWork.data & KEY_R) != 0) {
		camera->yaw -= TRIG_TURN / 128;
	}
	if(background->keepRange == 0) {
		btCameraSetRange(toFIXED(120), toFIXED(120), 0, 0, toFIXED(1));
	}
}

/* Auto battle: plays one action, a defeated target replaced by a living unit of
   its side. Returns 1 when the party got away, -1 when the unit is down. */
Sint32 btAutoBattleRunAction(BATTLE_ACTION_RECORD *action)
{
	BATTLE_BACKGROUND_STATE *background;
	Sint32					 unitId;
	Sint32					 yaw;
	UNIT					*actor;

	unitId = action->unitId;
	actor = syUnitGet(unitId);
	if(actor->hp == 0)
		return -1;

	action->tgt = btAbilityReplaceDefeated(action);
	background = RamHeapSlots->battleBg;
	/* Here any unit id above Felix's counts as an enemy, Jenna, Sheba and Piers
	   included. */
	if(action->unitId > FELIX)
		yaw = YAW_ENEMY_ACTION;
	else
		yaw = YAW_PARTY_ACTION;
	background->tgtYaw = yaw;
	background->ticks = 60;
	mnMessageArgumentClearAllStub();

	switch(action->command) {
	case COMMAND_FLEE:
		mnBattleMessageShowAndWaitStub((Sint32)&MsgPartyFlees);
		if(btFleePlayRun(action) != 0)
			return 1;
		break;
	case COMMAND_DEFEND:
		sySynch(45);
		btAutoBattleShowDefend((Sint16 *)action);
		break;
	case COMMAND_ITEM:
		sySynch(45);
		btAutoBattleRunAttackOrItem(action);
		break;
	case COMMAND_ATTACK:
	default: {
		BATTLE_BACKGROUND_STATE *view = RamHeapSlots->battleBg;
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

/* Returns 0 for a party member's or an enemy's unit id, -1 for any other. */
Sint32 btObjectIsValidId(Uint32 unitId)
{
	if(unitId <= ALLY_MAX) {
		return 0;
	}
	if(unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
		return 0;
	}
	return -1;
}

/* Auto battle: the party tries to run, and gets away 15 times in 16. Returns 1
   when it got away. */
Bool btFleePlayRun(BATTLE_ACTION_RECORD *action)
{
	Sint16 partyMembers[UNIT_MAX];
	Sint32 partySize;
	Sint32 memberSlot;

	(void)action;
	/* Flee unless a roll below 16 comes up 0, 15 in 16. */
	if(syRandomBelow(16) != 0) {
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

/* Auto battle: shows that a standing unit is defending, "<unit> is defending!".
   `action` is the action record, read for its unit id. Returns -1 for an
   invalid unit id, 0 otherwise. */
Sint32 btAutoBattleShowDefend(Sint16 *action)
{
	Sint32 objectId;
	Sint32 result;
	UNIT  *unit;

	objectId = *action;
	unit = syUnitGet(objectId);
	if(btObjectIsValidId(objectId) < 0) {
		return -1;
	}
	result = 0;
	if(unit->hp <= 0) {
		return result;
	}
	mnMessageArgumentClearAllStub();
	mnMessageArgumentPushStub(objectId, MESSAGE_ARGUMENT_ACTOR);
	mnBattleMessageShowAndWaitStub((Sint32)MsgActorDefends);
	return 0;
}

/* Auto battle: casts Psynergy, the caster's side fading out while its effect
   plays on the target side. Returns -1 for an invalid unit or target. */
Sint32 btAutoBattleRunPsynergy(BATTLE_ACTION_RECORD *action)
{
	Uint16					  visibleUnits[UNIT_MAX];
	BATTLE_PRESENTATION_STATE context;
	Sint32					  actorId;
	Sint32					  tgtId;
	Uint32					  sideStart;
	Uint32					  sideCnt;
	Uint32					  livingCnt;
	MOTION_OBJECT			 *actorRecord;
	Uint32					  idx;

	livingCnt = 0;
	actorId = action->unitId;
	if(btObjectIsValidId(actorId) < 0)
		return -1;
	tgtId = action->tgt;
	if(btObjectIsValidId(tgtId) < 0)
		return -1;

	{
		BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;
		/* An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not
		   YAW_ENEMY_ACTION. */
		background->tgtYaw =
			action->unitId > FELIX ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
		background->ticks = 60;
	}
	sySynch(10);
	/* A roll nothing uses, which still advances the random number generator. */
	syRandom();
	actorRecord = btObjectGet(actorId)->obj;

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
		if(idx + sideStart == actorId)
			fdActorSelectAnimationStub((ACTOR *)actorRecord, POSE_CAST);
	}

	/* Fade the caster's side out over 16 ticks, and close the party status
	   window. */
	sySynch(30);
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(idx = 0; idx != sideCnt; idx++)
		btHighlightSetSprMode(idx + sideStart, SPRITE_SEMI_TRANSPARENT);
	for(idx = 0; idx != BLEND_WEIGHT_MAX; idx++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - idx, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	mnPartyStatDrawStub(PARTY_STATUS_CLOSED);

	/* Ready only the target side's living units, and play the effect. */
	if(tgtId >= FIRST_ENEMY_UNIT) {
		Uint32 cnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
		for(idx = 0; livingCnt != cnt; idx++) {
			Uint32 unit = idx + FIRST_ENEMY_UNIT;
			if(syUnitGet(unit)->hp > 0)
				visibleUnits[livingCnt++] = unit;
		}
	} else {
		Uint32 cnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
		for(idx = 0; livingCnt != cnt; idx++) {
			if(syUnitGet(idx)->hp > 0)
				visibleUnits[livingCnt++] = idx;
		}
	}
	visibleUnits[livingCnt] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForList((Sint16 *)visibleUnits, 0);

	context.kind = action->prm;
	context.actor = actorId;
	for(idx = 0; idx != livingCnt; idx++)
		context.actors[idx] = visibleUnits[idx];
	context.cnt = livingCnt;
	if(tgtId >= 0 && tgtId <= ALLY_MAX)
		context.side = EFFECT_SIDE_PARTY;
	else
		context.side = EFFECT_SIDE_ENEMIES;
	btEffectRunSummonStub((Sint32 *)&context);

	/* Bring every unit back and fade the caster's side in. */
	sySynch(10);
	btObjectPut();
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(idx = 0; idx != sideCnt; idx++)
		btHighlightSetSprMode(idx + sideStart, SPRITE_SEMI_TRANSPARENT);
	for(idx = 0; idx != BLEND_WEIGHT_MAX; idx++) {
		REG_BLDALPHA = BLEND_ALPHA(idx, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	for(idx = 0; idx != sideCnt; idx++)
		btHighlightSetSprMode(idx + sideStart, SPRITE_OPAQUE);
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
	sySynch(3);
	return 0;
}

/* Unused auto battle attack: "<unit> attacks!", the attacker leaps up to its
   target, the blow, and both return. Returns -1 when either id is not a battle
   object. */
UNUSED Sint32 btAutoBattleRunApproachAttack(BATTLE_ACTION_RECORD *action)
{
	BATTLE_PRESENTATION_STATE state;

	/* A short wait when the camera already faces the party's side. */
	if(RamHeapSlots->battleBg->tgtYaw == YAW_PARTY_ACTION) {
		RamHeapSlots->battleBg->tgtYaw = YAW_PARTY_ACTION;
		sySynch(10);
	} else {
		RamHeapSlots->battleBg->tgtYaw = YAW_PARTY_ACTION;
		sySynch(30);
	}

	state.actor = action->unitId;
	if(btObjectIsValidId(state.actor) < 0)
		return -1;

	state.actors[0] = action->tgt;
	if(btObjectIsValidId(state.actors[0]) < 0)
		return -1;

	/* DEAD: syUnitGet's results are not used; the ROM keeps the calls. The roll
	   only advances the random number generator. */
	syUnitGet(state.actor);
	syUnitGet(state.actors[0]);
	syRandom();
	mnMessageArgumentPushStub(state.actor, MESSAGE_ARGUMENT_ACTOR);
	mnBattleMessageShowAndWaitStub((Sint32)MsgActorAttacks);
	btMotionApproachTarget(state.actor, state.actors[0], 13, 0);
	fdActorSetAnimationSpeedStub(
		(ACTOR *)btObjectGet(state.actor)->obj, ANIM_STEP_NORMAL);
	btObjectGet(state.actors[0]);

	state.cnt = 1;
	if((Uint16)state.actors[0] <= ALLY_MAX)
		state.side = EFFECT_SIDE_PARTY;
	else
		state.side = EFFECT_SIDE_ENEMIES;
	state.prelude = 0;

	sySynch(4);
	btEffectRunByIdRangeStub(&state);
	btMotionReturnToSpot(state.actors[0]);
	btMotionReturnToSpot(state.actor);
	return 0;
}

/* Auto battle: an attack or an item, played on the first living unit of the
   target's side as each element's pillar in turn. Returns -1 for an invalid
   unit. */
Sint32 btAutoBattleRunAttackOrItem(BATTLE_ACTION_RECORD *action)
{
	BATTLE_PRESENTATION_STATE state;
	BATTLE_BACKGROUND_STATE	 *background = RamHeapSlots->battleBg;
	MOTION_OBJECT			 *object;
	Sint32					  tgtYaw;
	Sint32					  i;

	if(action->unitId > FELIX)
		tgtYaw = YAW_ENEMY_ACTION;
	else
		tgtYaw = YAW_PARTY_ACTION;
	if(background->tgtYaw == tgtYaw) {
		background->ticks = 40;
		sySynch(40);
	} else {
		background->tgtYaw = tgtYaw;
		background->ticks = 40;
		sySynch(40);
	}
	state.kind = action->prm;
	state.range = action->range;
	state.actor = action->unitId;
	state.firstTgt = action->tgt;
	if(btObjectIsValidId(state.actor) < 0)
		return -1;
	if(state.firstTgt >= FIRST_ENEMY_UNIT)
		state.cnt =
			btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, (Uint16 *)state.actors);
	else
		state.cnt =
			btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (Uint16 *)state.actors);
	/* The status window without its PP row (see PARTY_STATUS_SUMMONS). */
	mnPartyStatDrawStub(
		RamHeapSlots->battleState->partyStat & ~PARTY_STATUS_SHOW_PP);
	/* The unit raises its hand (POSE_CAST); then only the first living unit of
	   the target's side takes the effect. */
	object = btObjectGet(state.actor)->obj;
	fdActorSelectAnimationStub((ACTOR *)object, POSE_CAST);
	fdActorSetAnimationSpeedStub((ACTOR *)object, ANIM_STEP_NORMAL);
	if((Uint16)action->tgt <= ALLY_MAX) {
		state.side = EFFECT_SIDE_PARTY;
		btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (Uint16 *)state.actors);
		state.cnt = 1;
	} else {
		state.side = EFFECT_SIDE_ENEMIES;
		btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, (Uint16 *)state.actors);
		state.cnt = 1;
	}
	/* Keep each target's sprite entries' parameters for the effect. */
	for(i = 0; i != state.cnt; i++) {
		SPR	  *anm = (SPR *)btObjectGet(state.actors[i])->obj->records;
		Sint32 j;

		for(j = 0; j != anm->cnt - 1; j++)
			state.params[i][j] = anm->entries[j]->colMode;
	}
	state.kind = ELEMENT_VENUS;
	state.variant = 0;
	btEffectRunElementPillarStub(&state);
	state.kind = ELEMENT_MERCURY;
	btEffectRunElementPillarStub(&state);
	state.kind = ELEMENT_MARS;
	btEffectRunElementPillarStub(&state);
	state.kind = ELEMENT_JUPITER;
	btEffectRunElementPillarStub(&state);
	state.kind = 0;
	/* The blow goes through the dispatch on its kind, which carries no
	   EFFECT_BY_ID_RANGE: it runs from the handler table. */
	BATTLE_EFFECT_RUN(&state, state.kind);
	fdActorSelectAnimationStub((ACTOR *)object, POSE_STANDING);
	for(i = 0; i != state.cnt; i++)
		btMotionReturnToSpot(state.actors[i]);
	btMotionReturnToSpot(state.actor);
	return 0;
}

/* Unused auto battle damage: takes damage HP from a unit and shows it, with "A
   bitter blow!" or "A critical hit!" first when showMsg is set. */
UNUSED void btAutoBattleApplyDamage(
	Uint32 unitId, Sint32 damage, Sint32 showMsg, Uint8 *context)
{
	Uint8		   localContext[4];
	UNIT		  *character;
	BATTLE_OBJECT *slot;

	if(context == 0) {
		context = localContext;
		context[0] = 0;
		context[1] = 0;
		context[2] = 0;
		context[3] = 0;
	}

	character = syUnitGet(unitId);
	character->hp -= damage;
	if(character->hp < 0)
		character->hp = 0;

	slot = btObjectGet(unitId);
	fdActorSelectAnimationStub((ACTOR *)slot->obj, POSE_DAMAGE);
	mnPartyStatDrawStub(PARTY_STATUS_HP_ONLY);
	mnMessageArgumentClearAllStub();

	if(unitId <= ALLY_MAX) {
		if(showMsg != 0)
			mnBattleMessageShowAndWaitStub((Sint32)MsgBitterBlow);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((Sint32)MsgDmgP);
	} else {
		if(showMsg != 0)
			mnBattleMessageShowAndWaitStub((Sint32)MsgCritical);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((Sint32)MsgDmgE);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	}

	btMotionPlayFall(unitId);
	if(unitId <= ALLY_MAX) {
		if(character->hp <= 0) {
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((Sint32)MsgGoesDown);
		}
	} else if(character->hp <= 0) {
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((Sint32)MsgDowned);
	}

	slot = btObjectGet(unitId);
	fdActorSelectAnimationStub((ACTOR *)slot->obj, POSE_STANDING);
}

/* Unused, as its one caller, btAutoBattleApplyDamage, is: for a unit whose HP
   has run out, plays its falling effect to the last tick and marks its battle
   object for release (btObjectMarkForRelease). */
void btMotionPlayFall(Sint32 unitId)
{
	SPR			  *anm;
	SPR_ANIMATION *entry;

	if(syUnitGet(unitId)->hp <= 0) {
		anm = btObjectGet(unitId)->obj->records;
		fdSprStartAnimationStub(anm, POSE_DAMAGE);
		entry = anm->entries[0];
		entry->colMode = SPRITE_COLOR_GREY;
		entry->picture = SPRITE_PICTURE_NONE;
		sySynch(4);
		fdSprDissolveRunStub(anm);
		btObjectMarkForRelease(unitId);
	}
}

/* btAbilityReplaceDefeated's result when the target's side is all down, which
   the battle's end comes before. */
#define TARGET_SIDE_DEFEATED 256

/* Returns the action's target, or a random living unit of its side when the
   target is down; TARGET_SIDE_DEFEATED when none is left. */
Sint32 btAbilityReplaceDefeated(const BATTLE_ACTION_RECORD *action)
{
	Sint16 livingUnits[UNIT_MAX];
	Sint32 tgtId;
	Sint32 livingCnt;

	tgtId = action->tgt;
	if(syUnitGet(tgtId)->hp != 0) {
		return tgtId;
	}

	if(tgtId >= FIRST_ENEMY_UNIT) {
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
	} else {
		livingCnt = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
	}

	if(livingCnt == 0) {
		return TARGET_SIDE_DEFEATED;
	}

	return livingUnits[(Uint32)(syRandom() * livingCnt) >> 16];
}
