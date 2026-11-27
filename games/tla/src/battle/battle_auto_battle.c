/* @file */
/* Auto battle: the actions it builds and plays without the command menu, the */
/* shoulder-key camera and the escape. */
/* Part of battle.c. */

/* Auto battle: gives every living unit a plain attack and sorts them into */
/* action order. Returns how many actions there are. */
s32 btAutoBattleBuildActions(BATTLE_ACTION_RECORD *entries) {
	BATTLE_ACTION_RECORD swap;
	u16 unitIds[UNIT_MAX];
	s32 partyCount;
	s32 count = 0;
	s32 enemyCount;
	s32 targetRange;
	s32 index;
	BATTLE_ACTION_RECORD *entry;
	UNIT *unit;

	/* The party's actions. */
	partyCount = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, unitIds);
	/* DEAD: syUnitGet's results are not used; the ROM keeps the calls. */
	for(index = 0; index != PARTY_BATTLE_SIZE; index++)
		syUnitGet(index);
	for(index = 0; index < partyCount; index++) {
		s32 unitId = unitIds[index];

		unit = syUnitGet(unitId);
		entry = &entries[index];
		entry->unitId = unitId;
		entry->pri = unit->agility;
		entry->command = COMMAND_ATTACK;
		entry->prm = 0;
		entry->tgt = FIRST_ENEMY_UNIT;
		count++;
	}
	/* The enemies attack a random party member, replaced when the action plays
	 * if */
	/* it is down. */
	enemyCount = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
	entry = &entries[partyCount];
	targetRange = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, NULL);
	for(index = 0; index < enemyCount; index++) {
		s32 unitId = unitIds[index];

		unit = syUnitGet(unitId);
		entry->unitId = unitId;
		entry->pri = unit->agility >> 1;
		if(entry->pri != 0)
			entry->pri += (u32)(syRandom() * unit->agility) >> 16;
		entry->command = COMMAND_ATTACK;
		entry->prm = 0;
		entry->tgt = syRandomBelow(targetRange);
		count++;
		entry++;
	}
	/* Bubble the faster units forward. */
	/* BUG: one pass short of a full sort; with two actions nothing is sorted.
	 */
	for(index = count - 2; index > 0; index--) {
		s32 swaps = 0;
		s32 pos;

		for(pos = count - 1; pos > 0; pos--) {
			if((s16)entries[pos].pri > (s16)entries[pos - 1].pri) {
				syDMAXCopy(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
				syDMAXCopy(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4),
						   REG_DMA3);
				syDMAXCopy(&swap, &entries[pos - 1], DMA_COPY_WORDS(4),
						   REG_DMA3);
				swaps++;
			}
		}
		if(swaps == 0)
			break;
	}
	return count;
}

/* Unused: turns the battle camera while L or R is held, a 128th of a turn */
/* each frame, and redraws the zoomed background unless a background holds it.
 */
UNUSED void btCameraAdjustByShoulderKeys(void) {
	BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;
	BATTLE_BACKGROUND_STATE *background = Ram_HeapSlots->battleBg;

	if((InputWork.held & KEY_L) != 0) {
		camera->yaw += TRIG_TURN / 128;
	}
	if((InputWork.held & KEY_R) != 0) {
		camera->yaw -= TRIG_TURN / 128;
	}
	if(background->keepRange == 0) {
		btCameraSetRange(toFIXED(120), toFIXED(120), 0, 0, FIXED_ONE);
	}
}

/* Auto battle: plays one action, a defeated target replaced by a living unit */
/* of its side. Returns 1 when the party got away, -1 when the unit is down. */
s32 btAutoBattleRunAction(BATTLE_ACTION_RECORD *action) {
	BATTLE_BACKGROUND_STATE *background;
	s32 unitId;
	s32 yaw;
	UNIT *actor;

	unitId = action->unitId;
	actor = syUnitGet(unitId);
	if(actor->hp == 0)
		return -1;

	action->tgt = btAbilityReplaceDefeated(action);
	background = Ram_HeapSlots->battleBg;
	/* Here any unit id above Felix's counts as an enemy, Jenna, Sheba and Piers
	 */
	/* included. */
	if(action->unitId > FELIX)
		yaw = YAW_ENEMY_ACTION;
	else
		yaw = YAW_PARTY_ACTION;
	background->tgtYaw = yaw;
	background->frames = 60;
	mnMessageArgumentClearAllStub();

	switch(action->command) {
	case COMMAND_FLEE:
		mnBattleMessageShowAndWaitStub((s32)&MsgPartyFlees);
		if(btFleePlayRun(action) != 0)
			return 1;
		break;
	case COMMAND_DEFEND:
		sySynch(45);
		btAutoBattleShowDefend((s16 *)action);
		break;
	case COMMAND_ITEM:
		sySynch(45);
		btAutoBattleRunAttackOrItem(action);
		break;
	case COMMAND_ATTACK:
	default: {
		BATTLE_BACKGROUND_STATE *view = Ram_HeapSlots->battleBg;
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
s32 btObjectIsValidId(u32 unitId) {
	if(unitId <= ALLY_MAX) {
		return 0;
	}
	if(unitId - FIRST_ENEMY_UNIT < ENEMY_BATTLE_SIZE) {
		return 0;
	}
	return -1;
}

/* Auto battle: the party tries to run, and gets away 15 times in 16. Returns 1
 */
/* when it got away. */
s32 btFleePlayRun(BATTLE_ACTION_RECORD *action) {
	s16 partyMembers[UNIT_MAX];
	s32 partySize;
	s32 memberSlot;

	(void)action;
	/* Flee unless a roll below 16 comes up 0, 15 in 16. */
	if(syRandomBelow(16) != 0) {
		partySize = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, partyMembers);
		for(memberSlot = 0; memberSlot != partySize; memberSlot++) {
			btMotionSetupEscapeObject(partyMembers[memberSlot]);
			sySynch(8);
		}
		sySynch(22);
		return 1;
	}
	mnBattleMessageShowAndWaitStub(MsgCannotEscape);
	return 0;
}

/* Auto battle: shows that a standing unit is defending, "<unit> is */
/* defending!". `action` is the action record, read for its unit id. Returns -1
 */
/* for an invalid unit id, 0 otherwise. */
s32 btAutoBattleShowDefend(s16 *action) {
	s32 objectId;
	s32 result;
	UNIT *unit;

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
	mnBattleMessageShowAndWaitStub((s32)MsgActorDefends);
	return 0;
}

/* Auto battle: casts Psynergy, the caster's side fading out while its effect */
/* plays on the target side. Returns -1 for an invalid unit or target. */
s32 btAutoBattleRunPsynergy(BATTLE_ACTION_RECORD *action) {
	u16 visibleUnits[UNIT_MAX];
	BATTLE_PRESENTATION_STATE context;
	s32 actorId;
	s32 targetId;
	u32 sideStart;
	u32 sideCount;
	u32 livingCount;
	MOTION_OBJECT *actorRecord;
	u32 index;

	livingCount = 0;
	actorId = action->unitId;
	if(btObjectIsValidId(actorId) < 0)
		return -1;
	targetId = action->tgt;
	if(btObjectIsValidId(targetId) < 0)
		return -1;

	{
		BATTLE_BACKGROUND_STATE *background = Ram_HeapSlots->battleBg;
		/* An enemy caster turns the camera to YAW_BEHIND_ENEMIES, not */
		/* YAW_ENEMY_ACTION. */
		background->tgtYaw =
			action->unitId > FELIX ? YAW_BEHIND_ENEMIES : YAW_PARTY_ACTION;
		background->frames = 60;
	}
	sySynch(10);
	/* A roll nothing uses, which still advances the random number generator. */
	syRandom();
	actorRecord = btObjectGet(actorId)->obj;

	/* The caster's side, taken as the side the target is not on, and the */
	/* caster's POSE_CAST when its id is among that side's first sideCount ids.
	 */
	if(targetId >= 0 && targetId <= ALLY_MAX) {
		sideCount = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
		sideStart = FIRST_ENEMY_UNIT;
	}
	else {
		sideCount = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
		sideStart = 0;
	}

	for(index = 0; index != sideCount; index++) {
		if(index + sideStart == actorId)
			fdActorSelectAnimationStub((ACTOR *)actorRecord, POSE_CAST);
	}

	/* Fade the caster's side out over 16 frames, and close the party status */
	/* window. */
	sySynch(30);
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(index = 0; index != sideCount; index++)
		btHighlightSetSprMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
	for(index = 0; index != BLEND_WEIGHT_MAX; index++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - index, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	mnPartyStatDrawStub(PARTY_STATUS_CLOSED);

	/* Ready only the target side's living units, and play the effect. */
	if(targetId >= FIRST_ENEMY_UNIT) {
		u32 count = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, visibleUnits);
		for(index = 0; livingCount != count; index++) {
			u32 unit = index + FIRST_ENEMY_UNIT;
			if(syUnitGet(unit)->hp > 0)
				visibleUnits[livingCount++] = unit;
		}
	}
	else {
		u32 count = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, visibleUnits);
		for(index = 0; livingCount != count; index++) {
			if(syUnitGet(index)->hp > 0)
				visibleUnits[livingCount++] = index;
		}
	}
	visibleUnits[livingCount] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForList((s16 *)visibleUnits, 0);

	context.kind = action->prm;
	context.actor = actorId;
	for(index = 0; index != livingCount; index++)
		context.actors[index] = visibleUnits[index];
	context.cnt = livingCount;
	if(targetId >= 0 && targetId <= ALLY_MAX)
		context.side = EFFECT_SIDE_PARTY;
	else
		context.side = EFFECT_SIDE_ENEMIES;
	btEffectRunSummonStub((s32 *)&context);

	/* Bring every unit back and fade the caster's side in. */
	sySynch(10);
	btObjectPut();
	REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
	for(index = 0; index != sideCount; index++)
		btHighlightSetSprMode(index + sideStart, SPRITE_SEMI_TRANSPARENT);
	for(index = 0; index != BLEND_WEIGHT_MAX; index++) {
		REG_BLDALPHA = BLEND_ALPHA(index, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	for(index = 0; index != sideCount; index++)
		btHighlightSetSprMode(index + sideStart, SPRITE_OPAQUE);
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
	sySynch(3);
	return 0;
}

/* Unused auto battle attack: "<unit> attacks!", the attacker leaps up to its */
/* target, the blow, and both return. Returns -1 when either id is not a */
/* battle object. */
UNUSED s32 btAutoBattleRunApproachAttack(BATTLE_ACTION_RECORD *action) {
	BATTLE_PRESENTATION_STATE state;

	/* A short wait when the camera already faces the party's side. */
	if(Ram_HeapSlots->battleBg->tgtYaw == YAW_PARTY_ACTION) {
		Ram_HeapSlots->battleBg->tgtYaw = YAW_PARTY_ACTION;
		sySynch(10);
	}
	else {
		Ram_HeapSlots->battleBg->tgtYaw = YAW_PARTY_ACTION;
		sySynch(30);
	}

	state.actor = action->unitId;
	if(btObjectIsValidId(state.actor) < 0)
		return -1;

	state.actors[0] = action->tgt;
	if(btObjectIsValidId(state.actors[0]) < 0)
		return -1;

	/* DEAD: syUnitGet's results are not used; the ROM keeps the calls. The roll
	 */
	/* only advances the random number generator. */
	syUnitGet(state.actor);
	syUnitGet(state.actors[0]);
	syRandom();
	mnMessageArgumentPushStub(state.actor, MESSAGE_ARGUMENT_ACTOR);
	mnBattleMessageShowAndWaitStub((s32)MsgActorAttacks);
	btMotionApproachTarget(state.actor, state.actors[0], 13, 0);
	fdActorSetAnimationSpeedStub((ACTOR *)btObjectGet(state.actor)->obj,
								 ANIM_STEP_NORMAL);
	btObjectGet(state.actors[0]);

	state.cnt = 1;
	if((u16)state.actors[0] <= ALLY_MAX)
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

/* Auto battle: an attack or an item, played on the first living unit of the */
/* target's side as each element's pillar in turn. Returns -1 for an invalid */
/* unit. */
s32 btAutoBattleRunAttackOrItem(BATTLE_ACTION_RECORD *action) {
	BATTLE_PRESENTATION_STATE state;
	BATTLE_BACKGROUND_STATE *background = Ram_HeapSlots->battleBg;
	MOTION_OBJECT *object;
	s32 targetYaw;
	s32 i;

	if(action->unitId > FELIX)
		targetYaw = YAW_ENEMY_ACTION;
	else
		targetYaw = YAW_PARTY_ACTION;
	if(background->tgtYaw == targetYaw) {
		background->frames = 40;
		sySynch(40);
	}
	else {
		background->tgtYaw = targetYaw;
		background->frames = 40;
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
			btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)state.actors);
	else
		state.cnt =
			btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)state.actors);
	/* The status window without its PP row (see PARTY_STATUS_SUMMONS). */
	mnPartyStatDrawStub(Ram_HeapSlots->battleState->partyStat &
						~PARTY_STATUS_SHOW_PP);
	/* The unit raises its hand (POSE_CAST); then only the first living unit of
	 */
	/* the target's side takes the effect. */
	object = btObjectGet(state.actor)->obj;
	fdActorSelectAnimationStub((ACTOR *)object, POSE_CAST);
	fdActorSetAnimationSpeedStub((ACTOR *)object, ANIM_STEP_NORMAL);
	if((u16)action->tgt <= ALLY_MAX) {
		state.side = EFFECT_SIDE_PARTY;
		btUnitListLiving(BATTLE_SIDE_MASK_PARTY, (u16 *)state.actors);
		state.cnt = 1;
	}
	else {
		state.side = EFFECT_SIDE_ENEMIES;
		btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, (u16 *)state.actors);
		state.cnt = 1;
	}
	/* Keep each target's sprite entries' parameters for the effect. */
	for(i = 0; i != state.cnt; i++) {
		SPR *animation = (SPR *)btObjectGet(state.actors[i])->obj->records;
		s32 j;

		for(j = 0; j != animation->cnt - 1; j++)
			state.params[i][j] = animation->entries[j]->colMode;
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
	/* The blow goes through the dispatch on its kind, which carries no */
	/* EFFECT_BY_ID_RANGE: it runs from the handler table. */
	BATTLE_EFFECT_RUN(&state, state.kind);
	fdActorSelectAnimationStub((ACTOR *)object, POSE_STANDING);
	for(i = 0; i != state.cnt; i++)
		btMotionReturnToSpot(state.actors[i]);
	btMotionReturnToSpot(state.actor);
	return 0;
}

/* Unused auto battle damage: takes damage HP from a unit and shows it, with "A
 */
/* bitter blow!" or "A critical hit!" first when showMessage is set. */
UNUSED void btAutoBattleApplyDamage(u32 unitId, s32 damage, s32 showMessage,
									u8 *context) {
	u8 localContext[4];
	UNIT *character;
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
		if(showMessage != 0)
			mnBattleMessageShowAndWaitStub((s32)MsgBitterBlow);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((s32)MsgDmgP);
	}
	else {
		if(showMessage != 0)
			mnBattleMessageShowAndWaitStub((s32)MsgCritical);
		mnMessageArgumentPushStub(damage, MESSAGE_ARGUMENT_NUMBER);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((s32)MsgDmgE);
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	}

	btMotionPlayFall(unitId);
	if(unitId <= ALLY_MAX) {
		if(character->hp <= 0) {
			mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
			mnBattleMessageShowAndWaitStub((s32)MsgGoesDown);
		}
	}
	else if(character->hp <= 0) {
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
		mnBattleMessageShowAndWaitStub((s32)MsgDowned);
	}

	slot = btObjectGet(unitId);
	fdActorSelectAnimationStub((ACTOR *)slot->obj, POSE_STANDING);
}

/* Unused, as its one caller, btAutoBattleApplyDamage, is: for a unit whose HP
 */
/* has run out, plays its falling effect to the last frame and marks its battle
 */
/* object for release (btObjectMarkForRelease). */
void btMotionPlayFall(s32 unitId) {
	SPR *animation;
	SPR_ANIMATION *entry;

	if(syUnitGet(unitId)->hp <= 0) {
		animation = btObjectGet(unitId)->obj->records;
		fdSprStartAnimationStub(animation, POSE_DAMAGE);
		entry = animation->entries[0];
		entry->colMode = SPRITE_COLOR_GREY;
		entry->picture = SPRITE_PICTURE_NONE;
		sySynch(4);
		fdSprDissolveRunStub(animation);
		btObjectMarkForRelease(unitId);
	}
}

/* btAbilityReplaceDefeated's result when the target's side is all down, */
/* which the battle's end comes before. */
#define TARGET_SIDE_DEFEATED 256

/* Returns the action's target, or a random living unit of its side when the */
/* target is down; TARGET_SIDE_DEFEATED when none is left. */
s32 btAbilityReplaceDefeated(const BATTLE_ACTION_RECORD *action) {
	s16 livingUnits[UNIT_MAX];
	s32 targetId;
	s32 livingCount;

	targetId = action->tgt;
	if(syUnitGet(targetId)->hp != 0) {
		return targetId;
	}

	if(targetId >= FIRST_ENEMY_UNIT) {
		livingCount = btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
	}
	else {
		livingCount = btUnitListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
	}

	if(livingCount == 0) {
		return TARGET_SIDE_DEFEATED;
	}

	return livingUnits[(u32)(syRandom() * livingCount) >> 16];
}
