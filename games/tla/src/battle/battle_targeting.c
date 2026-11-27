/* Applying an ability to each of its targets, the battle events queued on the
   way, and the wait for the player to advance a battle message. Part of
   battle.c. */

/* The target's checks, effect and messages */
/* A stat stage changes the stat by an eighth of it (gmUnitCalc), from half at
   the lowest to one and a half at the highest. */
#define STAT_STAGE_MIN		  -4
#define STAT_STAGE_MAX		  4
/* Zephyr's Agility stage, beyond the usual highest: twice the Agility. */
#define AGILITY_STAGE_DOUBLED 8

/* The turns a stat change or a status lasts; a status or a lowered stat may end
   sooner. */
#define STAT_CHANGE_TURNS	 7
#define AGILITY_CHANGE_TURNS 5
#define STATUS_TURNS		 7
#define REGENERATE_TURNS	 5
/* The Grim Reaper's countdown. */
#define CURSE_TURNS			 7

/* The rate gmBattleCalcPsynergyPowerStub and gmBattleCalcRecoveryStub scale by:
   256 is 1. */
#define FULL_RATE 256

/* A message operand that shows nothing: the event only waits for the player. */
#define MESSAGE_NONE -1

/* A summon's bonus damage counts the target's max HP up to this much. */
#define SUMMON_BONUS_HP_MAX 10000

/* Counts unit's stun among the statuses Restore cures, for
   COUNT_PARTIAL_CURES. */
/* BUG: Restore cures stun and never confusion or charm, as its description says
   ("Remove sleep, stun, and delusion.") and its cure does, but the count takes
   confusion and charm and not stun, so Cerberus, the Hydros Statue and the
   Refresh Ball pass over an ally who is only stunned. */
#if FIX_RESTORE_COUNTS_STUN
#define COUNT_STUN(unit, count) \
	if((unit)->stun != 0) \
		(count)++;
#else
#define COUNT_STUN(unit, count) \
	if((Uint8)(unit)->confusion != 0) \
		(count)++; \
	if((unit)->charm != 0) \
		(count)++;
#endif

/* Counts the statuses of unit that Restore cures: delusion, stun (COUNT_STUN),
   sleep, the Psynergy seal and the Grim Reaper. */
#define COUNT_PARTIAL_CURES(unit, count) \
	{ \
		if((unit)->delusion != 0) \
			(count)++; \
		COUNT_STUN((unit), (count)) \
		if((unit)->sleep != 0) \
			(count)++; \
		if((unit)->psySeal != 0) \
			(count)++; \
		if((unit)->grimReaper != 0) \
			(count)++; \
	}

/* Counts the statuses Tonic cures: those, the evil spirit and poison. */
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
		case ABILITY_EFFECT_CRYSTAL: \
		case ABILITY_EFFECT_SPRING: \
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
			if((unit)->poison <= 1) \
				(applies) = 1; \
			break; \
		case ABILITY_EFFECT_STUN: \
		case ABILITY_EFFECT_STUN_THREE_ROLLS: \
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
		case ABILITY_EFFECT_BALM: \
			if((unit)->hp == 0) \
				(applies) = 100; \
			break; \
		case ABILITY_EFFECT_TOTAL_DEFENSE: \
		case ABILITY_EFFECT_GUARD_AURA: \
			if((unit)->defendLevel <= 1) \
				(applies) = 1; \
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
			case ABILITY_FORMULA_DAMAGE - 1: \
			case ABILITY_FORMULA_ENEMY_DAMAGE - 1: \
			case ABILITY_FORMULA_SUMMON - 1: \
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
			case ABILITY_FORMULA_HEAL_PP - 1: \
				if((unit)->pp < (unit)->ppMax) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_EFFECT - 1: \
			case ABILITY_FORMULA_NO_POWER - 1: \
				if((action)->power != 0 && (unit)->hp != 0) \
					(applies)++; \
				break; \
			case ABILITY_FORMULA_EFFECT_ONLY - 1: \
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
	Uint16		  orderPositions[6];
	Sint32		  unitIds[6];
	Sint32		  targetPositions[6];
	BATTLE_STATE *turnOrder;
	UNIT		 *unit;
	UNIT		 *nextUnit;
	UNIT		 *thisUnit;
	Sint32		  candidateCount;
	Sint32		  targetIndex;
	Sint32		  targetCount;
	Sint32		  innerIndex;
	Sint32		  selected;
	Sint32		  unitId;
	Sint32		  value;
	Sint32		  nextValue;
	Sint32		  swap;
	Sint32		  ownSide;
	Sint32		  kindIndex;
	Sint32		  applies;
	Uint32		  roll;
	Sint16		 *slot;

	candidateCount = 0;
	turnOrder = Ram_HeapSlots->battleState;
	targetCount = 0;

	/* The candidates: every unit on the side the ability may target. */
	if(action->tgtMode != ABILITY_TARGET_ONE) {
		ownSide = 0;
		if(action->tgtMode == ABILITY_TARGET_ALLIES ||
			action->tgtMode == ABILITY_TARGET_SELF)
			ownSide = 1;

		if(((userId < 0 || userId > ALLY_MAX) && ownSide == 0) ||
			(userId >= 0 && userId <= ALLY_MAX && ownSide != 0)) {
			for(targetIndex = 0;
				turnOrder->partyUnits[targetIndex] != BATTLE_UNIT_LIST_END;
				targetIndex++) {
				slot = &turnOrder->partyUnits[targetIndex];
				unitId = *slot;
				if(unitId != BATTLE_UNIT_REMOVED) {
					if(action->tgtMode != ABILITY_TARGET_SELF ||
						unitId == userId) {
						unitIds[candidateCount] = unitId;
						orderPositions[candidateCount] = PLACE_PARTY;
						orderPositions[candidateCount] |= targetIndex;
						candidateCount++;
					}
				}
			}
		} else {
			for(targetIndex = 0;
				turnOrder->enemyIds[targetIndex] != BATTLE_UNIT_LIST_END;
				targetIndex++) {
				unitId = turnOrder->enemyIds[targetIndex];
				if(unitId != BATTLE_UNIT_REMOVED) {
					if(action->tgtMode != ABILITY_TARGET_SELF ||
						unitId == userId) {
						unitIds[candidateCount] = unitId;
						orderPositions[candidateCount] = PLACE_ENEMY;
						orderPositions[candidateCount] |= targetIndex;
						candidateCount++;
					}
				}
			}
		}
	}

	if(candidateCount == 0)
		return TARGET_PICK_SIDE_EMPTY;

	/* Keep the candidates the ability would affect, in place. */
	for(targetIndex = 0; targetIndex < candidateCount; targetIndex++) {
		unit = syUnitGet(unitIds[targetIndex]);
		CHECK_EFFECT_TARGET(unit, action, applies, kindIndex);
		if(applies != 0) {
			unitIds[targetCount] = unitIds[targetIndex];
			targetPositions[targetCount] = orderPositions[targetIndex];
			targetCount++;
		}
	}

	if(targetCount == 0)
		return TARGET_PICK_NONE;

	if(action->tgtMode == ABILITY_TARGET_FOES && action->range == 1 &&
		gmEnemyGetDefinitionStub(syUnitGet(userId)->enemyId)->tgtStrategy !=
			TARGET_STRATEGY_RANDOM &&
		(Uint32)((action->flg & ABILITY_FORMULA_MASK) -
			ABILITY_FORMULA_ATTACK) <=
			ABILITY_FORMULA_DAMAGE - ABILITY_FORMULA_ATTACK) {
		/* Sort the targets by HP, the most first. */
		/* BUG: each pass starts one place later, so HP 1, 2 and 3 end as 2, 3
		   and 1. */
		selected = -1;
		for(targetIndex = 0; targetIndex < targetCount; targetIndex++) {
			for(innerIndex = targetIndex; innerIndex < targetCount - 1;
				innerIndex++) {
				thisUnit = syUnitGet(unitIds[innerIndex]);
				nextUnit = syUnitGet(unitIds[innerIndex + 1]);
				if(gmEnemyGetDefinitionStub(syUnitGet(userId)->enemyId)
						->tgtStrategy == TARGET_STRATEGY_MOST_HP) {
					value = thisUnit->hp;
					nextValue = nextUnit->hp;
				} else {
					value = thisUnit->hpMax;
					nextValue = nextUnit->hpMax;
				}
				if(value < nextValue) {
					swap = unitIds[innerIndex];
					unitIds[innerIndex] = unitIds[innerIndex + 1];
					unitIds[innerIndex + 1] = swap;
					swap = targetPositions[innerIndex];
					targetPositions[innerIndex] =
						targetPositions[innerIndex + 1];
					targetPositions[innerIndex + 1] = swap;
				}
			}
		}

		/* Weighted toward the front: of a roll's outcomes the first target
		   takes 6, the second 5, the third 4 and the fourth 3; five or more
		   fall through to an even roll. */
		switch(targetCount) {
		case 1:
			selected = 0;
			break;
		case 2:
			roll = syRandomBelow(6 + 5);
			selected = 0;
			if(roll >= 6)
				selected = 1;
			break;
		case 3:
			selected = syRandomBelow(6 + 5 + 4);
			if(selected < 6)
				selected = 0;
			else if(selected < 6 + 5)
				selected = 1;
			else
				selected = 2;
			break;
		case 4:
			selected = syRandomBelow(6 + 5 + 4 + 3);
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
			return targetPositions[selected];
	}
	return targetPositions[syRandomBelow(targetCount)];
}

/* Clears every status and stat change of a unit that goes down and works out
   its stats again. */
void btUnitClearStatuses(Sint32 unitId)
{
	UNIT  *unit;
	Sint32 element;

	unit = syUnitGet(unitId);
	for(element = 0; element < ELEMENT_COUNT; element++)
		unit->summonBoost[element] = 0;

	unit->poison = 0;
	unit->attackTurns = 0;
	unit->attackStage = 0;
	unit->defenseTurns = 0;
	unit->defenseStage = 0;
	unit->resistanceTurns = 0;
	unit->resistanceStage = 0;
	unit->delusion = 0;
	unit->confusion = 0;
	unit->charm = 0;
	unit->stun = 0;
	unit->sleep = 0;
	unit->psySeal = 0;
	unit->regenerate = 0;
	unit->reflect = 0;
	unit->haunt = 0;
	unit->grimReaper = 0;
	unit->equipBonus[0] = 0;
	unit->equipBonus[1] = 0;
	unit->ready = 0;
	unit->immobile = 0;
	unit->agilityTurns = 0;
	unit->agilityStage = 0;
	unit->challenge = 0;

	gmUnitCalcStub(unitId);
	btUnitBuildStatFlags(unitId, btObjectGet(unitId));
}

/* Waits for the player to advance a battle message: once the window has
   finished printing, shows the bobbing arrow at its corner until B is held, or
   A, B, L or R is pressed (or held, after 16 frames), then plays the cursor
   sound. */
void btMessageWaitForAdvance(void)
{
	MENU_SPR *prompt;
	MENU_SPR  promptStorage;
	Sint32	  frame;
	Sint32	  tileSlot;
	Sint32	  pictureTiles;
	Sint32	  sheet;
	WINDOW	 *window;
	MESSAGE	 *message;

	while(!mnMessageAllTypedStub())
		sySynch(1);
	/* FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	 * members at its uses. */
	prompt = &promptStorage;
	sheet = (Sint32)BattleMessage_AdvanceArrowTiles;
	tileSlot = syVRAMCacheAllocEntry(128);
	for(frame = 0;; frame++) {
		/* Cycle the eight pictures, one every four frames. */
		pictureTiles = ((FrameCount >> 2) & 7) * 128 + sheet;
		window = Ram_HeapSlots->battleMsg->win;
		message = Ram_HeapSlots->battleMsg->msg;
		syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		syIOWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		/* Both attribute words are set whole: an 8 by 16 arrow of 256 colours,
		   blended with what is behind it. */
		MENU_SPR_INIT(*prompt,
			OBJ_SIZE_8X16 | OBJ_256_COLORS |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
		prompt->oam.f.tile = syVRAMCacheReload(tileSlot, pictureTiles);
		prompt->oam.f.x = window->x * 8 + (message->x >> 8) + 4;
		/* Bob up and down by up to 2 pixels, once every 16 frames. */
		prompt->oam.f.y =
			CURSOR_BOB_Y() + window->y * 8 + (message->y >> 8) + 6;
		syOAMAddSprAtDepth(prompt, OAM_DEPTH_WINDOW);
		/* A held advance key counts only after 16 frames, so one still held
		   from the previous message does not skip this one. */
		if((PerWork.data & KEY_B) || (PerWork.push & KEY_ADVANCE) ||
			(frame > 15 && (PerWork.data & KEY_ADVANCE)))
			break;
		sySynch(1);
	}
	sySndPlayStub(SE_MENU_CURSOR_MOVE);
	syVRAMCacheFreeEntry(tileSlot);
	sySynch(1);
}

/* Shows the cursor bobbing at (x, y) and waits for the player to go on with A,
   B, L or R; returns 1. */
Sint32 btMessageWaitForAdvanceAt(Sint32 x, Sint32 y)
{
	MENU_POINT position;
	Sint32	   pointerTiles;
	MENU_SPR   promptStorage;
	MENU_SPR  *prompt;
	Sint32	   tileSlot;

	pointerTiles = mnWindowSprGetPointerTilesStub(0);
	position.x = x;
	position.y = y;
	while(!mnMessageAllTypedStub())
		sySynch(1);
	/* FAKEMATCH(temporary): prompt holds &promptStorage; as promptStorage's
	 * members at its uses. */
	prompt = &promptStorage;
	tileSlot = syVRAMCacheAllocEntry(128);
	while(1) {
		syIOWriteQueueClearBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_BG2);
		syIOWriteQueueSetBits16((Uint32)&REG_WINOUT, WINOUT_WIN01_OBJ);
		syIOWrite16(BLEND_ALPHA(BLEND_WEIGHT_MAX, 0), &REG_BLDALPHA);
		MENU_SPR_INIT(*prompt, OBJ_SIZE_16X16);
		prompt->oam.f.tile = syVRAMCacheReload(tileSlot, pointerTiles);
		prompt->oam.f.x = position.x + ((FrameCount & 4) >> 1) - 4;
		prompt->oam.f.y = position.y - ((FrameCount & 4) >> 2) - 8;
		syOAMAddSprAtDepth(prompt, OAM_DEPTH_WINDOW);
		if(PerWork.push & KEY_ADVANCE)
			break;
		sySynch(1);
	}
	syVRAMCacheFreeEntry(tileSlot);
	sySynch(1);
	return 1;
}

/* Waits until the battle message has typed out, then marks the next message
   (BATTLE_MESSAGE_STATE.marked). */
void btMessageWaitForTyping(void)
{
	while(!mnMessageAllTypedStub())
		sySynch(1);
	Ram_HeapSlots->battleMsg->marked = 1;
}

/* Takes an enemy out of the battle at once, one that leaves rather than
   falls. */
Sint32 btUnitRemoveEnemy(Sint32 unitId)
{
	/* UB: declared Sint32 but returns no value; no caller reads it. */
	UNIT		  *creature;
	BATTLE_OBJECT *runtime;

	creature = syUnitGet(unitId);
	if(creature->side == BATTLE_SIDE_ENEMY) {
		gmUnitSetHpStub(creature, 0);
		btObjectRemoveFromLists(unitId);
		btObjectMarkForRelease(unitId);
		runtime = btObjectGet(unitId);
		fdActorReleaseStub((ACTOR *)runtime->obj);
		runtime->obj = 0;
		runtime->active = 0;
	}
}

/* Grants the Djinn bonus for the rest of the action. */
void btPlaybackGrantDjinnBonus(BATTLE_PLAYBACK_STATE *runtime, Uint32 operand)
{
	runtime->djinnBonus |= 1;
}

/* Runs the queued battle events in order, then resets the queue. */
Uint32 btPlaybackRunAll(void)
{
	BATTLE_STATE		  *battle;
	BATTLE_PLAYBACK_STATE *runtime;
	BATTLE_PLAYBACK_QUEUE *queue;
	Sint32				   i;

	battle = Ram_HeapSlots->battleState;
	runtime = &battle->events;
	queue = &runtime->queue;

	for(i = 0; i < queue->cnt; i++) {
		Uint8 opcode = queue->opcodes[i];
		if(opcode <= PLAYBACK_BLOW_AWAY)
			switch(opcode) {
			case PLAYBACK_DJINN_BONUS:
				btPlaybackGrantDjinnBonus(runtime, queue->operands[i]);
				break;
			case PLAYBACK_REMOVE_UNIT:
				btUnitRemoveEnemy(queue->operands[i]);
				break;
			case PLAYBACK_UNIT_NAME:
				mnMessageArgumentPushStub(
					queue->operands[i], MESSAGE_ARGUMENT_ACTOR);
				break;
			case PLAYBACK_NUMBER:
				mnMessageArgumentPushStub(
					queue->operands[i], MESSAGE_ARGUMENT_NUMBER);
				break;
			case PLAYBACK_ITEM_NAME:
				mnMessageArgumentPushStub(
					queue->operands[i] & INVENTORY_ITEM_ID_MASK,
					MESSAGE_ARGUMENT_ITEM);
				break;
			case PLAYBACK_ABILITY_NAME:
				mnMessageArgumentPushStub(
					queue->operands[i] & PSYNERGY_ABILITY_MASK,
					MESSAGE_ARGUMENT_ABILITY);
				break;
			case PLAYBACK_MARK:
				Ram_HeapSlots->battleMsg->marked = 1;
				break;
			case PLAYBACK_CLEAR_ARGUMENTS:
				mnMessageArgumentClearAllStub();
				break;
			case PLAYBACK_MESSAGE:
				if((Sint32)queue->operands[i] >= 0)
					mnBattleMessageShowAndWaitStub(queue->operands[i]);
				btMessageWaitForAdvance();
				mnMessageArgumentClearAllStub();
				break;
			case PLAYBACK_MESSAGE_NO_WAIT:
				if((Sint32)queue->operands[i] >= 0)
					mnBattleMessageShowAndWaitStub(queue->operands[i]);
				mnMessageArgumentClearAllStub();
				break;
			case PLAYBACK_HIT:
				if(runtime->pendingSnd > 0)
					sySndPlayStub(runtime->pendingSnd);
				btMotionFlashHit(queue->operands[i], 0, 0);
				break;
			case PLAYBACK_DEFEAT:
				btEnemyRecordDefeat(queue->operands[i], runtime->djinnBonus);
				btUnitClearStatuses(queue->operands[i]);
				btMotionShowDowned(queue->operands[i]);
				break;
			case PLAYBACK_DRAW_STATUS:
				mnPartyStatDrawStub(Ram_HeapSlots->battleState->partyStat);
				break;
			case PLAYBACK_REFRESH_UNIT:
				btUnitBuildStatFlags(
					queue->operands[i], btObjectGet(queue->operands[i]));
				btObjectUpdatePose(queue->operands[i]);
				break;
			case PLAYBACK_BLOW_AWAY:
				btObjectRemoveFromLists(queue->operands[i]);
				break;
			}
	}
	return btPlaybackReset();
}

/* Queues one playback command with its operand. Nothing checks the queue's
   capacity. */
void btPlaybackPush(Uint32 opcode, Uint32 operand)
{
	BATTLE_PLAYBACK_QUEUE *queue = &Ram_HeapSlots->battleState->events.queue;
	Uint32				   index = queue->cnt;

	queue->opcodes[index] = opcode;
	queue->operands[index] = operand;
	queue->cnt = index + 1;
}

/* Returns 1 for an effect that still applies when the ability's damage has just
   knocked its target out: the drains, which give the damage they dealt back to
   the user. */
Bool btAbilityEffectAppliesToDowned(Sint32 effect)
{
	switch(effect) {
	case ABILITY_EFFECT_DRAIN_HP:
	case ABILITY_EFFECT_DRAIN_PP:
	case ABILITY_EFFECT_DRAIN_HALF_HP:
	case ABILITY_EFFECT_PSY_LEECH:
		return TRUE;
	default:
		return FALSE;
	}
}

/* Returns 1 for a unit no blow may fell, which is left 1 HP: Agatio and Karst
   in their first battle. */
Bool btUnitKeepsOneHp(Sint32 unitId)
{
	UNIT *unit = syUnitGet(unitId);

	if(unit->enemyId == ENEMY_AGATIO || unit->enemyId == ENEMY_KARST)
		return TRUE;
	return FALSE;
}

/* Puts unit into a unit list at the removed place nearest the list's divider,
   or appends it: where a Star Magician's ball takes its place. */
void btPartyInsertUnitCentered(Uint16 *list, Sint32 unit)
{
	Sint32	counts[2] = {0, 0};
	Sint32	side;
	Sint32	slot;
	Sint32	divider;
	Sint32	i;
	Uint16 *entry;

	slot = -1;
	side = 0;
	divider = slot;
	i = 0;
	entry = list;
	do {
		Uint32 value = *entry++;

		if(value == BATTLE_UNIT_REMOVED) {
			if(divider >= 0 && slot >= 0) {
				if(ABS(i - divider) < ABS(slot - divider))
					slot = i;
			} else {
				slot = i;
			}
		} else {
			if(value == BATTLE_UNIT_LIST_END)
				break;
			if(value == FIRST_ENEMY_UNIT) {
				side ^= 1;
				divider = i;
			}
			counts[side]++;
		}
		i++;
	} while(i <= 5);
	if(slot >= 0) {
		list[slot] = unit;
	} else {
		list[i] = unit;
		list[i + 1] = BATTLE_UNIT_LIST_END;
	}
}

/* Party unit ids run to ALLY_MAX; enemy units start at FIRST_ENEMY_UNIT. */
#define IS_PARTY_UNIT(unitId) ((unitId) >= 0 && (unitId) <= ALLY_MAX)

/* Whether two units are on opposite sides: party unit ids are below
   FIRST_ENEMY_UNIT, enemy ids from it. */
#define ARE_OPPONENTS(unitId, otherId) \
	((Uint32)((unitId) ^ (otherId)) >> 7 != 0)

/* Holds a stat stage within STAT_STAGE_MIN to STAT_STAGE_MAX. */
#define CLAMP_STAT_STAGE(stage) \
	{ \
		if((stage) < STAT_STAGE_MIN) \
			(stage) = STAT_STAGE_MIN; \
		if((stage) > STAT_STAGE_MAX) \
			(stage) = STAT_STAGE_MAX; \
	}

/* Pushes one message for a unit of the party and another for an enemy. */
#define PUSH_SIDE_MESSAGE(unitId, partyMessage, enemyMessage) \
	{ \
		if(IS_PARTY_UNIT(unitId)) \
			PUSH_MESSAGE(partyMessage); \
		else \
			PUSH_MESSAGE(enemyMessage); \
	}

/* A guarding unit takes half the damage, two fifths behind Shade, or a tenth
   behind Total Defense. */
#define REDUCE_BY_GUARD(damage, unit) \
	{ \
		guard = (unit)->defendLevel; \
		if(guard != 0) { \
			if(guard == DEFEND_NORMAL) \
				(damage) /= 2; \
			else if(guard == DEFEND_SHIELD) \
				(damage) = (damage) * 2 / 5; \
			else \
				(damage) /= 10; \
		} \
	}

/* defenseCut: how much of the target's Defense an attack counts. */
#define DEFENSE_HALVED	1 /* Torch: half */
#define DEFENSE_IGNORED 2 /* Raiden's Wrath and Trident: none */

/* The element power an item, or an ability with no element, acts with. */
#define ITEM_ELEMENT_POWER 100

/* An unleashed Djinni gives the Djinn bonus (more coins and experience, and a
   likelier item, from the enemies that fall) against a target weakest to its
   element, with a chance in percent of the user's power less the target's
   resistance, plus this many. */
#define DJINN_BONUS_CHANCE 30

/* Applies the action's ability to one of its targets, slot, in the seven steps
   of README.md's "How an ability resolves", queueing what playback will show.
   The Lost Age adds the shields that turn an ability away, the abilities that
   call reinforcements, and the Djinn and summon effects of its own. */
Sint32 btAbilityApplyToTarget(BATTLE_PLAN *plan, Sint32 slot)
{
	ABILITY_DEFINITION *ability;
	Sint32				abilityId;
	UNIT			   *actor;
	Sint32				unitId;
	Sint32				element;
	Sint32				elementBonus; /* elemental power less resistance */
	BATTLE_STATE	   *battle;
	Sint32				distance; /* steps from the aimed position, 0 to 5 */
	Sint32 defenseCut;	/* DEFENSE_HALVED or DEFENSE_IGNORED, else 0 */
	Sint32 multiplier;	/* 0 on a miss, 2 or 3 for a double or triple blow */
	Sint32 damageDealt; /* HP or PP the target lost (negative if healed) */
	Sint32 leaveOneHp;	/* the blow leaves the target 1 HP at most */
	Sint32 hit;			/* the ability's effect lands */
	Sint32 critical;	/* BATTLE_CRITICAL */
	Sint32 skipDamage;	/* no damage step: instant death replaces it */
	Sint32 kind;		/* the ability kind: ABILITY_FORMULA */
	Sint32 affinity;	/* -1 a resisted element, 1 a weakness, else 0 */
	/* A copy of the target as it was, to measure each stat's change; a heap
	   block freed at the end. */
	UNIT  *savedTarget;
	Sint32 elementPower; /* the user's power in the ability's element */
	Sint32 hp;
	Sint32 drained;
	Sint32 defense;
	Sint32 pass;
	Uint8  targetId;
	Sint8 *resistance;
	UNIT  *target;
	Uint32 actionIndex;
	Sint32 value;
	Sint32 amount; /* the damage, or the HP or PP restored */
	Sint32 power;
	Sint8 *agility;
	Sint32 guard;
	Sint16 enemyList[8];
	Sint32 enemyIndex;
	Sint32 restored;
	Sint32 unitSize;

	/* 1. The target as it was, its distance and the element. */
	elementBonus = 0;
	battle = Ram_HeapSlots->battleState;
	defenseCut = 0;
	damageDealt = 0;
	leaveOneHp = 0;
	skipDamage = 0;
	affinity = 0;
	unitSize = sizeof(UNIT);
	savedTarget = syHeapAlloc(unitSize);

	/* Meld and Mold strike with the stats of the unit that joins the user. */
	/* BUG: abilityId is tested before it is read from the plan, so this reads a
	   local not yet set. */
	if(abilityId == ABILITY_MELD || abilityId == ABILITY_MOLD)
		unitId = plan->partnerId;
	else
		unitId = plan->unitId;
	targetId = plan->tgtIds[slot];
	abilityId = plan->abilityId;
	element = plan->element;
	multiplier = plan->tgtMultiplier[slot];
	critical = plan->tgtCritical[slot];

	ability = gmAbilityGetDirectStub(abilityId);
	actor = syUnitGet(unitId);
	target = syUnitGet(targetId);
	syWordsCopyEntry(savedTarget, target, unitSize);

	/* What turns an opponent's ability away untouched: a Sentinel dissipates
	   Psynergy, Poseidon's force field stands until the Trident breaks it, and
	   Haze leaves nothing to hit. */
	switch(target->enemyId) {
	case ENEMY_SENTINEL:
		if(plan->command == COMMAND_PSYNERGY &&
			ARE_OPPONENTS(unitId, targetId)) {
			btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			PUSH_MESSAGE(MsgDissipated);
			goto done;
		}
		break;
	case ENEMY_POSEIDON:
	case ENEMY_POSEIDON_SECOND_ACTION:
	case ENEMY_POSEIDON_3:
		if(abilityId != ABILITY_TRIDENT && battle->forceField != 0 &&
			ARE_OPPONENTS(unitId, targetId)) {
			btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			PUSH_MESSAGE(MsgFieldProtects);
			goto done;
		}
		break;
	}
	if(target->defendLevel == DEFEND_VANISHED &&
		ARE_OPPONENTS(unitId, targetId)) {
		btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
		btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
		PUSH_MESSAGE(MsgNoEffect);
		goto done;
	}

	/* An ability that reaches the whole side treats every target as aimed
	   at. */
	if(ability->range != ABILITY_RANGE_ALL) {
		distance = plan->tgtOfs[slot];
		if(distance < 0)
			distance = -distance;
	} else {
		distance = 0;
	}

	/* The affinity picks the message: "damage.", "damage!" or "damage!!!". */
	if(element != ELEMENT_NONE) {
		Sint32 i;

		value = target->baseElement[element].resistance;
		for(i = 0;
			i < ELEMENT_COUNT && value >= target->baseElement[i].resistance;
			i++)
			;
		if(i == ELEMENT_COUNT)
			affinity = -1;

		for(i = 0;
			i < ELEMENT_COUNT && value <= target->baseElement[i].resistance;
			i++)
			;
		if(i == ELEMENT_COUNT)
			affinity = 1;
	}

	/* The user's power in the ability's element; items always use 100. */
	if(plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER &&
		plan->command != COMMAND_ITEM)
		elementPower = actor->element[plan->element].power;
	else
		elementPower = ITEM_ELEMENT_POWER;

	/* 2. The Djinn bonus, and whether the effect lands. */
	if(plan->command == COMMAND_DJINN &&
		(plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER) &&
		affinity > 0) {
		Sint32 chance;

		chance = elementPower - target->element[plan->element].resistance;
		chance += DJINN_BONUS_CHANCE;
		/* Percent to a fraction of the 16-bit random range. */
		chance *= toFIXED(1) / 100;
		if(chance > (gmBattleRandomStub() & 0xffff))
			btPlaybackPush(PLAYBACK_DJINN_BONUS, 5);
	}

	/* Abilities that knock the target's Djinn to standby. */
	if(abilityId == ABILITY_DJINNFEST || abilityId == ABILITY_FUGUE)
		btUnitDrainDjinn(targetId, 0, 0);
	if(abilityId == ABILITY_DJINN_BLAST || abilityId == ABILITY_DJINN_STORM)
		btUnitDrainDjinn(targetId, 1, 0);
	if(abilityId == ABILITY_DJINN_STUN)
		btUnitDrainDjinn(targetId, 1, 1);

	/* Whether the effect lands: a roll made when the action was built, or one
	   made now, less likely the farther the target is. */
	kind = ability->flg & ABILITY_FORMULA_MASK;
	{
		Sint32 plannedHit;

		plannedHit = plan->tgtHit[slot];
		if(plannedHit == HIT_NOT_ROLLED)
			hit = gmBattleRollEffectSuccessStub(unitId, targetId, element,
				ability->eff, RangeFalloff_EffectChance[distance]);
		else
			hit = plannedHit;
	}

	/* 3. Calling an ally; Speed Surge is an enemy splitting in two. A Star
	   Magician calls one of its four balls, any other caller a Sea Fighter, and
	   Regen Dance the next enemy waiting in the queue. */
	if(ability->eff == ABILITY_EFFECT_CALL_ALLY ||
		ability->eff == ABILITY_EFFECT_CALL_HELP ||
		ability->eff == ABILITY_EFFECT_CALL_REINFORCEMENT ||
		ability->eff == ABILITY_EFFECT_CALL_QUEUED) {
		Sint32 classId;
		Sint32 newUnitId;
		Sint32 suffix;

		classId = actor->enemyId;
		suffix = -1;
		newUnitId = btEnemyGetFreeUnitId();
		if(ability->eff == ABILITY_EFFECT_CALL_HELP) {
			classId = btFormationGetRandomMember(battle->formation);
		} else if(ability->eff == ABILITY_EFFECT_CALL_REINFORCEMENT) {
			if(actor->enemyId == ENEMY_STAR_MAGICIAN)
				classId = (gmBattleRandomStub() & 3) + ENEMY_REFRESH_BALL;
			else
				classId = ENEMY_SEA_FIGHTER;
		} else if(ability->eff == ABILITY_EFFECT_CALL_QUEUED) {
			if(battle->spoils.queueCnt != 0) {
				classId =
					battle->spoils.queuedSummons[battle->spoils.queueHead];
				suffix = battle->spoils.queuedLetters[battle->spoils.queueHead];
			} else
				hit = 0;
		}
		if(hit != 0 && btEnemyHasSprRoom(classId) != 0 && newUnitId >= 0) {
			/* The newcomer's name letter; on the first duplicate the enemy
			   already there takes a letter too. */
			if(suffix == -1) {
				suffix = btEnemyClaimNameLetter(classId, 1);
				if(suffix & ENEMY_LETTER_FIRST_TOO)
					btEnemyLetterFirstOfKind(classId);
			}
			gmBattleUnitSetStub(newUnitId, classId, suffix & ENEMY_LETTER_MASK);
			if(ability->eff == ABILITY_EFFECT_CALL_QUEUED) {
				battle->spoils.queueCnt--;
				battle->spoils.queueHead = (battle->spoils.queueHead + 1) % 2;
			}
			/* A Star Magician's ball takes the place nearest the middle; any
			   other newcomer fills a removed enemy's place, or is appended. */
			if(actor->enemyId == ENEMY_STAR_MAGICIAN)
				btPartyInsertUnitCentered(battle->enemyIds, newUnitId);
			else {
				Sint32 i;

				for(i = 0; i < ENEMY_BATTLE_SIZE; i++) {
					if(battle->enemyIds[i] == BATTLE_UNIT_REMOVED) {
						battle->enemyIds[i] = newUnitId;
						break;
					}
					if(battle->enemyIds[i] == BATTLE_UNIT_LIST_END) {
						battle->enemyIds[i] = newUnitId;
						battle->enemyIds[i + 1] = BATTLE_UNIT_LIST_END;
						break;
					}
				}
			}
			/* Lay the enemies out again and put the newcomer's sprite in place
			   (the anchors are 16.16 fixed point). */
			btPlacementRelayoutEnemies();
			{
				BATTLE_OBJECT *battleObject;

				battleObject = btObjectGet(newUnitId);
				btObjectMake((BATTLE_OBJECT *)battleObject, newUnitId,
					battleObject->anchorX / toFIXED(1),
					battleObject->anchorZ / toFIXED(1));
			}
			btObjectPut();
			{
				Sint32 enemyCount;

				enemyCount = btEnemyList(enemyList);
				for(enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++)
					btMotionReturnToSpot((Uint16)enemyList[enemyIndex]);
			}
			btPlaybackPush(PLAYBACK_UNIT_NAME, newUnitId);
			if(ability->eff == ABILITY_EFFECT_CALL_QUEUED)
				PUSH_MESSAGE(MsgUpToFight);
			else if(abilityId != ABILITY_SPEED_SURGE)
				PUSH_MESSAGE(MsgAppears);
			else
				PUSH_MESSAGE(MsgSplitOff);
		} else if(abilityId == ABILITY_SPEED_SURGE) {
			PUSH_MESSAGE(MsgSplitFailed);
		} else {
			PUSH_MESSAGE(MsgNoOneCame);
		}
	}

	/* 4. Effects that change how the damage step works. */
	if(hit != 0) {
		if(ability->eff == ABILITY_EFFECT_GROUND ||
			ability->eff == ABILITY_EFFECT_ULYSSES) {
			/* Ground holds only a target that has not yet acted this turn. */
			hit = 0;
			for(actionIndex = 0; actionIndex < BATTLE_ACTION_CAPACITY;
				actionIndex++) {
				if(battle->actions[actionIndex].unitId == targetId) {
					hit = 1;
					break;
				}
			}
		} else if(ability->eff == ABILITY_EFFECT_HALVE_DEFENSE) {
			defenseCut = DEFENSE_HALVED;
		} else if(ability->eff == ABILITY_EFFECT_LEAVE_ONE_HP) {
			leaveOneHp = 1;
		} else if(ability->eff == ABILITY_EFFECT_INSTANT_DEATH) {
			skipDamage = 1;
		} else if(ability->eff == ABILITY_EFFECT_SELF_DESTRUCT) {
			/* The user leaves the battle (btUnitRemoveEnemy). */
			if(actor->hp != 0)
				btPlaybackPush(PLAYBACK_REMOVE_UNIT, unitId);
		} else if(ability->eff == ABILITY_EFFECT_DRAIN_PP) {
			/* Psy Drain does PP damage, and misses a target with no PP. */
			if(target->pp != 0)
				kind = ABILITY_FORMULA_PP_DAMAGE;
			else
				hit = 0;
		} else if(ability->eff == ABILITY_EFFECT_SLAY_DRAGON) {
			defenseCut = DEFENSE_IGNORED;
		} else if(ability->eff == ABILITY_EFFECT_SLAY_SEA_CREATURE) {
			defenseCut = DEFENSE_IGNORED;
		}
	}

	/* 5. Damage and healing; a downed target is skipped unless the ability
	   revives, and Shine's damage falls on its first target alone. */
	if((ability->eff != ABILITY_EFFECT_DAZZLE || slot == 0) &&
		skipDamage == 0 &&
		(target->hp != 0 || gmBattleIsReviveEffectStub(ability->eff) != 0)) {
		Sint32 pp;
		Sint32 newHp;

		switch(kind) {
		/* An attack: the user's Attack against the target's Defense. Meld and
		   Mold strike with the Attack of the two units that join in them. */
		case ABILITY_FORMULA_ATTACK:
		case ABILITY_FORMULA_ATTACK_MULTIPLIED: {
			Sint32 baseDefense;

			if(target->defendLevel == DEFEND_VANISHED)
				break;
			hp = target->hp;
			baseDefense = target->defense;
			defense = baseDefense;
			if(defenseCut != 0) {
				if(defenseCut == DEFENSE_IGNORED)
					defense = 0;
				else
					defense = baseDefense / 2;
			}
			pass = 1;
			do {
				if(element != ELEMENT_NONE)
					elementBonus =
						elementPower - target->element[element].resistance;
				if(pass == 0)
					elementBonus = 0;
				power = ability->power;
				if(kind == ABILITY_FORMULA_ATTACK_MULTIPLIED)
					amount = gmBattleCalcAttackDamageStub(
								 actor->attack, defense, 0, elementBonus) *
						power / 10;
				else {
					Sint32 attack;

					attack = actor->attack;
					if(abilityId == ABILITY_MELD || abilityId == ABILITY_MOLD) {
						attack = syUnitGet(plan->unitId)->attack;
						if(abilityId == ABILITY_MELD)
							attack += syUnitGet(plan->partnerId)->attack / 4;
					}
					amount = gmBattleCalcAttackDamageStub(
						attack, defense, power, elementBonus);
				}
				amount *= multiplier;
				if(critical != CRITICAL_NONE) {
					if(critical == CRITICAL_HIT)
						amount = amount * 5 / 4;
					else
						amount = amount * 3 / 2;
					amount += (target->level / 5) + 6;
					if(pass == 0) {
						btPlaybackPush(PLAYBACK_MARK, 0);
						{
							Sint32 message;

							message = (Sint32)MsgCritical;
							if(IS_PARTY_UNIT(targetId))
								message += 1;
							btPlaybackPush(PLAYBACK_MESSAGE_NO_WAIT, message);
						}
					}
				}
				amount += gmBattleRandomStub() & 3;
				REDUCE_BY_GUARD(amount, target);
				if(amount <= 0)
					amount = 1;
				if(leaveOneHp != 0) {
					if(amount < hp - 1) {
						amount = hp - 1;
						if(amount <= 0)
							amount = 1;
					}
				}
				if(syGameFlagGet(FLAG_DJINN_LESSON) != 0 &&
					plan->command == COMMAND_DJINN && hp <= amount) {
					amount = hp - 1;
				}
				pass++;
			} while(pass <= 1);
			hp -= amount;
			btPlaybackPush(PLAYBACK_HIT, targetId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			{
				Sint32 message;

				if(IS_PARTY_UNIT(targetId))
					message = (Sint32)MsgDmgEmphP + affinity;
				else
					message = (Sint32)MsgDmgEmphE + affinity;
				btPlaybackPush(PLAYBACK_MESSAGE, message);
			}

			if(hp <= 0 && btUnitKeepsOneHp(targetId) != 0)
				hp = 1;
			if(hp <= 0) {
				hp = 0;
				btPlaybackPush(PLAYBACK_DEFEAT, targetId);
				if(target->enemyId != ENEMY_DOOM_DRAGON_A &&
					target->enemyId != ENEMY_DOOM_DRAGON_E) {
					btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
					PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
				}
			} else
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			damageDealt = target->hp - hp;
			target->hp = hp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		case ABILITY_FORMULA_PP_DAMAGE: {
			if(target->defendLevel == DEFEND_VANISHED)
				break;
			if(ability->power == 0)
				break;
			pp = target->pp;
			if(element != ELEMENT_NONE)
				elementBonus =
					elementPower - target->element[element].resistance;
			power = ability->power;
			amount =
				gmBattleCalcPsynergyPowerStub(power, elementBonus, FULL_RATE);
			amount = amount * RangeFalloff_PpLoss[distance] / 100;
			amount *= multiplier;
			REDUCE_BY_GUARD(amount, target);
			if(ability->eff == ABILITY_EFFECT_DRAIN_PP && amount > pp)
				amount = pp;
			if(amount > pp)
				amount = pp;

			btPlaybackPush(PLAYBACK_HIT, targetId);
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			{
				Sint32 message;

				if(IS_PARTY_UNIT(targetId))
					message = (Sint32)MsgPpLossP;
				else
					message = (Sint32)MsgPpLossE;
				btPlaybackPush(PLAYBACK_MESSAGE, message);
				pp -= amount;
			}
			if(pp <= 0)
				pp = 0;
			btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			damageDealt = target->pp - pp;
			target->pp = pp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		case ABILITY_FORMULA_HEAL_HP: {
			if(ability->power == 0)
				break;
			newHp = target->hp;
			if(newHp == 0)
				break;

			power = ability->power;
			amount = gmBattleCalcRecoveryStub(power,
				element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower,
				FULL_RATE);
			amount = amount * RangeFalloff_HealHp[distance] / 100;
			amount *= multiplier;
			amount += gmBattleRandomStub() & 3;
			newHp += amount;
			if(newHp > target->hpMax) {
				newHp = target->hpMax;
				amount = newHp - target->hp;
			}
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			if(newHp == target->hpMax)
				PUSH_MESSAGE(MsgHpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, amount);
				PUSH_MESSAGE(MsgHpRecover);
			}
			damageDealt = target->hp - newHp;
			target->hp = newHp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		/* Unused: no formula is -1. PP damage again, by the PP-damage falloff
		   and with the plain damage message. */
		case -1: {
			if(target->defendLevel == DEFEND_VANISHED)
				break;
			if(ability->power == 0)
				break;
			pp = target->pp;
			if(element != ELEMENT_NONE)
				elementBonus =
					elementPower - target->element[element].resistance;
			power = ability->power;
			amount =
				gmBattleCalcPsynergyPowerStub(power, elementBonus, FULL_RATE);
			amount = amount * RangeFalloff_PpDamage[distance] / 100;
			amount *= multiplier;
			REDUCE_BY_GUARD(amount, target);
			btPlaybackPush(PLAYBACK_HIT, targetId);
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			{
				Sint32 message;

				if(IS_PARTY_UNIT(targetId))
					message = (Sint32)MsgDmgP;
				else
					message = (Sint32)MsgDmgE;
				btPlaybackPush(PLAYBACK_MESSAGE, message);
				pp -= amount;
			}
			if(pp <= 0)
				pp = 0;
			btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			target->pp = pp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		case ABILITY_FORMULA_DAMAGE:
		case ABILITY_FORMULA_ENEMY_DAMAGE:
		case ABILITY_FORMULA_SUMMON: {
			Sint32 pass;

			if(target->defendLevel == DEFEND_VANISHED)
				break;
			if(ability->power == 0)
				break;
			newHp = target->hp;
			pass = 1;
			do {
				if(element != ELEMENT_NONE)
					elementBonus =
						elementPower - target->element[element].resistance;
				if(pass == 0)
					elementBonus = 0;
				power = ability->power;
				/* A summon, and Meteor Blow, Star Mine and Cruel Ruin, add a
				   share of the target's max HP, counted up to
				   SUMMON_BONUS_HP_MAX. */
				if(abilityId == ABILITY_METEOR_BLOW ||
					abilityId == ABILITY_STAR_MINE ||
					abilityId == ABILITY_CRUEL_RUIN ||
					plan->command == COMMAND_SUMMON ||
					plan->command == COMMAND_DAEDALUS) {
					Sint32 maxHpPercent;

					maxHpPercent = 0;
					/* The Lost Age's new summons keep arms of their own beside
					   The Broken Seal's groups of four, also where they share a
					   group's percentage. */
					switch(abilityId) {
					default:
					case ABILITY_COATLICUE:
						break;
					case ABILITY_JUPITER:
					case ABILITY_MERCURY:
					case ABILITY_MARS:
					case ABILITY_VENUS:
						maxHpPercent = 3;
						break;
					case ABILITY_THOR:
					case ABILITY_METEOR:
					case ABILITY_BOREAS:
					case ABILITY_JUDGMENT:
						maxHpPercent = 12;
						break;
					case ABILITY_ULYSSES:
						maxHpPercent = 12;
						break;
					case ABILITY_METEOR_BLOW:
						maxHpPercent = 35;
						break;
					case ABILITY_ATALANTA:
					case ABILITY_KIRIN:
					case ABILITY_NEREID:
					case ABILITY_RAMSES:
						maxHpPercent = 6;
						break;
					case ABILITY_MEGAERA:
					case ABILITY_ZAGAN:
						maxHpPercent = 6;
						break;
					case ABILITY_TIAMAT:
					case ABILITY_NEPTUNE:
					case ABILITY_PROCNE:
					case ABILITY_CYBELE:
						maxHpPercent = 9;
						break;
					case ABILITY_MOLOCH:
					case ABILITY_FLORA:
						maxHpPercent = 9;
						break;
					case ABILITY_DAEDALUS:
						maxHpPercent = 7;
						break;
					case ABILITY_ECLIPSE:
					case ABILITY_DAEDALUS_STRIKE:
					case ABILITY_HAURES:
						maxHpPercent = 15;
						break;
					case ABILITY_AZUL:
						maxHpPercent = 21;
						break;
					case ABILITY_CATASTROPHE:
						maxHpPercent = 24;
						break;
					case ABILITY_STAR_MINE:
					case ABILITY_CHARON:
						maxHpPercent = 30;
						break;
					case ABILITY_CRUEL_RUIN:
					case ABILITY_IRIS:
						maxHpPercent = 40;
						break;
					}
					value = target->hpMax;
					if(value > SUMMON_BONUS_HP_MAX)
						value = SUMMON_BONUS_HP_MAX;
					power += value * maxHpPercent / 100;
				}
				amount = gmBattleCalcPsynergyPowerStub(
					power, elementBonus, FULL_RATE);
				amount *= multiplier;
				/* FAKEMATCH(dead): the ability kind masked again. */
				switch(kind & ABILITY_FORMULA_MASK) {
				case ABILITY_FORMULA_DAMAGE:
					amount =
						RangeFalloff_PsynergyDamage[distance] * amount / 100;
					break;
				case ABILITY_FORMULA_SUMMON:
					amount = amount * RangeFalloff_SummonDamage[distance] / 100;
					break;
				case ABILITY_FORMULA_ENEMY_DAMAGE:
					amount = amount * RangeFalloff_EnemyDamage[distance] / 100;
					break;
				}
				amount += gmBattleRandomStub() & 3;
				REDUCE_BY_GUARD(amount, target);
				if((MainRoutineStateWork.battleType == BATTLE_TYPE_LOST ||
					   (syGameFlagGet(FLAG_DJINN_LESSON) != 0 &&
						   plan->command == COMMAND_SUMMON)) &&
					newHp > amount) {
					amount = newHp;
				}
				pass++;
			} while(pass <= 1);
			btPlaybackPush(PLAYBACK_HIT, targetId);
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			{
				Sint32 message;

				if(IS_PARTY_UNIT(targetId))
					message = (Sint32)MsgDmgEmphP + affinity;
				else
					message = (Sint32)MsgDmgEmphE + affinity;
				btPlaybackPush(PLAYBACK_MESSAGE, message);
				newHp -= amount;
			}

			if(newHp <= 0 && btUnitKeepsOneHp(targetId) != 0)
				newHp = 1;
			if(MainRoutineStateWork.battleType == BATTLE_TYPE_LOST)
				newHp = 0;
			if(newHp <= 0) {
				btPlaybackPush(PLAYBACK_DEFEAT, targetId);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				{
					Sint32 message;

					newHp = 0;
					if(IS_PARTY_UNIT(targetId))
						message = (Sint32)MsgGoesDown;
					else
						message = (Sint32)MsgFelled;
					btPlaybackPush(PLAYBACK_MESSAGE, message);
				}
			} else
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			damageDealt = target->hp - newHp;
			target->hp = newHp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		case ABILITY_FORMULA_HEAL_PP: {
			if(ability->power == 0)
				break;
			pp = target->pp;
			power = ability->power;
			amount = gmBattleCalcRecoveryStub(power,
				element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower,
				FULL_RATE);
			amount = amount * RangeFalloff_HealPp[distance] / 100;
			amount *= multiplier;
			pp += amount;
			if(pp > target->ppMax) {
				pp = target->ppMax;
				amount = pp - target->pp;
			}
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			if(pp == target->ppMax)
				PUSH_MESSAGE(MsgPpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, amount);
				PUSH_MESSAGE(MsgPpRecover);
			}
			target->pp = pp;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		case ABILITY_FORMULA_BLANK:
		case ABILITY_FORMULA_EFFECT_ONLY:
		case ABILITY_FORMULA_NO_POWER:
			break;

		case ABILITY_FORMULA_EFFECT:
			if(target->defendLevel == DEFEND_VANISHED)
				break;
			if(hit != 0) {
				if(ability->power == 0)
					break;
				newHp = target->hp;
				if(element != ELEMENT_NONE)
					elementBonus =
						elementPower - target->element[element].resistance;
				power = ability->power;
				amount = gmBattleCalcPsynergyPowerStub(
					power, elementBonus, FULL_RATE);
				amount *= multiplier;
				amount = amount * RangeFalloff_EffectDamage[distance] / 100;
				REDUCE_BY_GUARD(amount, target);
				btPlaybackPush(PLAYBACK_HIT, targetId);
				btPlaybackPush(PLAYBACK_NUMBER, amount);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				{
					Sint32 message;

					if(IS_PARTY_UNIT(targetId))
						message = (Sint32)MsgDmgP;
					else
						message = (Sint32)MsgDmgE;
					btPlaybackPush(PLAYBACK_MESSAGE, message);
					newHp -= amount;
				}

				if(newHp <= 0 && btUnitKeepsOneHp(targetId) != 0)
					newHp = 1;
				if(newHp <= 0) {
					btPlaybackPush(PLAYBACK_DEFEAT, targetId);
					newHp = 0;
					if(target->enemyId != ENEMY_DOOM_DRAGON_A &&
						target->enemyId != ENEMY_DOOM_DRAGON_E) {
						btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
						PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
					}
				} else
					btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
				damageDealt = target->hp - newHp;
				target->hp = newHp;
				gmUnitRecalculateGaugesStub(targetId);
				break;
			}
			btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			PUSH_MESSAGE(MsgNoEffect);
			break;
		}
	}

	/* The Trident breaks Poseidon's force field. */
	if(battle->forceField != 0 && abilityId == ABILITY_TRIDENT) {
		btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
		PUSH_MESSAGE(MsgFieldBroken);
		battle->forceField = 0;
	}
	/* 6. The added effect; each message after the first pushes the name again.
	   Shine's falls on the targets after its first. */
	btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
	if((ability->eff != ABILITY_EFFECT_DAZZLE || slot != 0) &&
		(gmBattleIsReviveEffectStub(ability->eff) != 0 || target->hp != 0 ||
			btAbilityEffectAppliesToDowned(ability->eff) != 0) &&
		hit != 0) {
		switch(ability->eff) {

		/* Tonic cures every ailment. */
		case ABILITY_EFFECT_CURE_ALL:
			if(target->delusion != 0) {
				target->delusion = 0;
				PUSH_MESSAGE(MsgCureDelusion);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			}
			if(target->stun != 0) {
				target->stun = 0;
				btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgCureStun);
			}
			if(target->sleep != 0) {
				target->sleep = 0;
				PUSH_MESSAGE(MsgWakes);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			}
			if(target->psySeal != 0) {
				target->psySeal = 0;
				PUSH_MESSAGE(MsgCureSeal);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			}
			if(target->grimReaper != 0) {
				target->grimReaper = 0;
				PUSH_MESSAGE(MsgCureCurse);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			}
			if(target->haunt != 0) {
				target->haunt = 0;
				PUSH_MESSAGE(MsgCureSpirit);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			}
			if(target->poison != 0) {
				PUSH_MESSAGE(MsgCurePoison);
				target->poison = 0;
			}
			btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
			break;

		/* Restore cures what Tonic does, but not the evil spirit or poison. */
		case ABILITY_EFFECT_RESTORE:
			if(target->delusion != 0) {
				target->delusion = 0;
				btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgCureDelusion);
			}
			if(target->stun != 0) {
				target->stun = 0;
				btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgCureStun);
			}
			if(target->sleep != 0) {
				target->sleep = 0;
				btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgWakes);
			}
			if(target->psySeal != 0) {
				target->psySeal = 0;
				btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgCureSeal);
			}
			if(target->grimReaper == 0)
				break;
			target->grimReaper = 0;
			btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
			btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
			PUSH_MESSAGE(MsgCureCurse);
			break;

		/* Healing by a share of the target's max HP: "HP fully restored" at the
		   maximum, else the amount restored. Nothing shows if no HP came back,
		   unless the ability heals. */
		case ABILITY_EFFECT_FIZZ:
		case ABILITY_EFFECT_SPRITZ:
		case ABILITY_EFFECT_CRYSTAL:
		case ABILITY_EFFECT_SPRING:
		case ABILITY_EFFECT_BREATH: {
			Sint32 before;
			Sint32 limit;
			Sint32 maximum;
			Sint32 heal;
#if !FIX_POINTER_OUTSIDE_OBJECT
			Uint16 *stat;
#endif

			/* UB: a pointer walks from hp down past it to hpMax, reading each
			   stat twice, unsigned and signed; the member reads behind UBFIX
			   share one load each. */
#if FIX_POINTER_OUTSIDE_OBJECT
			before = (Uint16)target->hp;
			heal = target->hp;
#else
			stat = (Uint16 *)&target->hp;
			before = *stat--;
			heal = *(Sint16 *)(stat + 1);
#endif
			if(ability->eff == ABILITY_EFFECT_BREATH) {
				Sint32 fullHp;

#if FIX_POINTER_OUTSIDE_OBJECT
				limit = (Uint16)target->hpMax;
				fullHp = target->hpMax;
#else
				stat--;
				limit = *stat--;
				fullHp = *(Sint16 *)(stat + 1);
#endif
				heal += fullHp * 2 / 5;
			} else if(ability->eff == ABILITY_EFFECT_SPRING) {
#if FIX_POINTER_OUTSIDE_OBJECT
				limit = (Uint16)target->hpMax;
				maximum = target->hpMax;
#else
				stat--;
				limit = *stat--;
				maximum = *(Sint16 *)(stat + 1);
#endif
				heal += maximum * 7 / 10;
			} else if(ability->eff == ABILITY_EFFECT_CRYSTAL) {
				limit = (Uint16)target->hpMax;
				heal += (Sint16)limit / 2;
				/* FAKEMATCH(dead): updates of limit that cancel out. */
				limit++;
				limit--;
			} else {
#if FIX_POINTER_OUTSIDE_OBJECT
				limit = (Uint16)target->hpMax;
				maximum = target->hpMax;
#else
				stat--;
				limit = *stat--;
				maximum = *(Sint16 *)(stat + 1);
#endif
				if(ability->eff == ABILITY_EFFECT_FIZZ)
					heal += maximum * 60 / 100;
				else
					heal += maximum * 30 / 100;
			}
			if(heal > (Sint16)limit)
				heal = (Sint16)limit;
			restored = heal - (Sint16)before;
			if(restored == 0 && kind != ABILITY_FORMULA_HEAL_HP)
				break;
			if(heal == (Sint16)limit)
				PUSH_MESSAGE(MsgHpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, restored);
				PUSH_MESSAGE(MsgHpRecover);
			}
			target->hp = heal;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		/* The same by a share of max PP. */
		case ABILITY_EFFECT_EMBER:
		case ABILITY_EFFECT_AROMA:
		case ABILITY_EFFECT_ETHER: {
			Sint32 before;
			Sint32 maximum;
			Sint32 heal;
			Sint32 limit;
#if !FIX_POINTER_OUTSIDE_OBJECT
			Uint16 *stat;
#endif

			/* UB: a pointer walks from pp down past it to ppMax, reading each
			   stat twice, unsigned and signed; the member reads behind UBFIX
			   share one load each. */
#if FIX_POINTER_OUTSIDE_OBJECT
			before = (Uint16)target->pp;
			heal = target->pp;
#else
			stat = (Uint16 *)&target->pp;
			before = *stat--;
			heal = *(Sint16 *)(stat + 1);
#endif
			if(ability->eff == ABILITY_EFFECT_AROMA) {
				maximum = target->ppMax;
				limit = (Uint16)target->ppMax;
				heal += (Sint16)maximum / 10;
			} else if(ability->eff == ABILITY_EFFECT_ETHER) {
#if FIX_POINTER_OUTSIDE_OBJECT
				limit = (Uint16)target->ppMax;
				maximum = target->ppMax;
#else
				stat--;
				limit = *stat--;
				maximum = *(Sint16 *)(stat + 1);
#endif
				heal += maximum * 3 / 10;
			} else {
#if FIX_POINTER_OUTSIDE_OBJECT
				limit = (Uint16)target->ppMax;
				maximum = target->ppMax;
#else
				stat--;
				limit = *stat--;
				maximum = *(Sint16 *)(stat + 1);
#endif
				heal += maximum * 7 / 100;
			}
			if(heal > (Sint16)limit)
				heal = (Sint16)limit;
			restored = heal - (Sint16)before;
			if(restored == 0 && kind != ABILITY_FORMULA_HEAL_PP)
				break;
			if(heal == (Sint16)limit)
				PUSH_MESSAGE(MsgPpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, restored);
				PUSH_MESSAGE(MsgPpRecover);
			}
			target->pp = heal;
			gmUnitRecalculateGaugesStub(targetId);
			break;
		}

		/* The stat changes show how far the stat itself moved, 0 at its limit;
		   Zephyr and Vine set Agility's stage outright. */
		case ABILITY_EFFECT_AGILITY_UP:
			target->agilityStage = AGILITY_STAGE_DOUBLED;
			target->agilityTurns = AGILITY_CHANGE_TURNS;
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, target->agility - savedTarget->agility);
			PUSH_MESSAGE(MsgAgiUp);
			break;

		case ABILITY_EFFECT_AGILITY_DOWN:
			agility = &target->agilityStage;
			{
				Uint8 stage;

				stage = STAT_STAGE_MIN;
				*agility = stage;
			}
			target->agilityTurns = AGILITY_CHANGE_TURNS;
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, savedTarget->agility - target->agility);
			PUSH_MESSAGE(MsgAgiDown);
			break;

		case ABILITY_EFFECT_ATTACK_DOWN:
			target->attackStage -= 1;
			CLAMP_STAT_STAGE(target->attackStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, savedTarget->attack - target->attack);
			PUSH_MESSAGE(MsgAtkDown);
			target->attackTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:
			target->attackStage -= 2;
			CLAMP_STAT_STAGE(target->attackStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, savedTarget->attack - target->attack);
			PUSH_MESSAGE(MsgAtkDown);
			target->attackTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_ATTACK_UP:
			target->attackStage += 1;
			CLAMP_STAT_STAGE(target->attackStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, target->attack - savedTarget->attack);
			PUSH_MESSAGE(MsgAtkUp);
			target->attackTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_ATTACK_UP_SHARPLY:
			target->attackStage += 2;
			CLAMP_STAT_STAGE(target->attackStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, target->attack - savedTarget->attack);
			PUSH_MESSAGE(MsgAtkUp);
			target->attackTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_DEFENSE_DOWN:
			target->defenseStage -= 1;
			CLAMP_STAT_STAGE(target->defenseStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, savedTarget->defense - target->defense);
			PUSH_MESSAGE(MsgDefDown);
			target->defenseTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
			target->defenseStage -= 2;
			CLAMP_STAT_STAGE(target->defenseStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, savedTarget->defense - target->defense);
			PUSH_MESSAGE(MsgDefDown);
			target->defenseTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_DEFENSE_UP:
			target->defenseStage += 1;
			CLAMP_STAT_STAGE(target->defenseStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, target->defense - savedTarget->defense);
			PUSH_MESSAGE(MsgDefUp);
			target->defenseTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_DEFENSE_UP_SHARPLY:
			target->defenseStage += 2;
			CLAMP_STAT_STAGE(target->defenseStage);
			gmUnitCalcStub(targetId);
			btPlaybackPush(
				PLAYBACK_NUMBER, target->defense - savedTarget->defense);
			PUSH_MESSAGE(MsgDefUp);
			target->defenseTurns = STAT_CHANGE_TURNS;
			break;

		/* The revivals bring back a downed target with all, half, 80% or 60% of
		   its HP. */
		case ABILITY_EFFECT_REVIVE:
			if(target->hp != 0)
				break;
			PUSH_MESSAGE(MsgRevived);
			target->hp = target->hpMax;
			gmUnitRecalculateGaugesStub(targetId);
			break;

		case ABILITY_EFFECT_REVIVE_HALF:
			if(target->hp != 0)
				break;
			PUSH_MESSAGE(MsgRevived);
			target->hp = target->hpMax / 2;
			gmUnitRecalculateGaugesStub(targetId);
			break;

		case ABILITY_EFFECT_DEW:
			if(target->hp != 0)
				break;
			PUSH_MESSAGE(MsgRevived);
			target->hp = target->hpMax * 8 / 10;
			gmUnitRecalculateGaugesStub(targetId);
			break;

		case ABILITY_EFFECT_BALM:
			if(target->hp != 0)
				break;
			PUSH_MESSAGE(MsgRevived);
			target->hp = (Sint16)(target->hpMax * 6 / 10);
			gmUnitRecalculateGaugesStub(targetId);
			break;

		case ABILITY_EFFECT_CURE_POISON:
			if(target->poison != 0)
				PUSH_MESSAGE(MsgCurePoison);
			target->poison = 0;
			break;

		/* A Resistance change needs no recalculation to show: it is the change
		   in stages times RESISTANCE_PER_STAGE. */
		case ABILITY_EFFECT_RESISTANCE_DOWN:
			target->resistanceStage--;
			CLAMP_STAT_STAGE(target->resistanceStage);
			btPlaybackPush(PLAYBACK_NUMBER,
				(savedTarget->resistanceStage - target->resistanceStage) *
					RESISTANCE_PER_STAGE);
			PUSH_MESSAGE(MsgResDown);
			target->resistanceTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY: {
			target->resistanceStage -= 2;
			if(STAT_STAGE_MIN > target->resistanceStage)
				target->resistanceStage = STAT_STAGE_MIN;
			if(target->resistanceStage > STAT_STAGE_MAX)
				target->resistanceStage = STAT_STAGE_MAX;
			btPlaybackPush(PLAYBACK_NUMBER,
				(savedTarget->resistanceStage - target->resistanceStage) *
					RESISTANCE_PER_STAGE);
			PUSH_MESSAGE(MsgResDown);
			target->resistanceTurns = STAT_CHANGE_TURNS;
		} break;

		case ABILITY_EFFECT_RESISTANCE_UP:
			target->resistanceStage++;
			CLAMP_STAT_STAGE(target->resistanceStage);
			btPlaybackPush(PLAYBACK_NUMBER,
				(target->resistanceStage - savedTarget->resistanceStage) *
					RESISTANCE_PER_STAGE);
			PUSH_MESSAGE(MsgResUp);
			target->resistanceTurns = STAT_CHANGE_TURNS;
			break;

		case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY: {
			target->resistanceStage += 2;
			if(*(resistance = &target->resistanceStage) < STAT_STAGE_MIN)
				target->resistanceStage = STAT_STAGE_MIN;
			if(target->resistanceStage > STAT_STAGE_MAX)
				target->resistanceStage = STAT_STAGE_MAX;
			btPlaybackPush(PLAYBACK_NUMBER,
				(target->resistanceStage - savedTarget->resistanceStage) *
					RESISTANCE_PER_STAGE);
			PUSH_MESSAGE(MsgResUp);
			target->resistanceTurns = STAT_CHANGE_TURNS;
		} break;

		/* Poison does nothing to a poisoned target, deadly poison nothing to
		   one that already has it. */
		case ABILITY_EFFECT_POISON:
			if(target->poison != 0)
				break;
			PUSH_MESSAGE(MsgPoisoned);
			target->poison = POISON_NORMAL;
			break;

		case ABILITY_EFFECT_VENOM:
			if(target->poison > POISON_NORMAL)
				break;
			PUSH_MESSAGE(MsgVenom);
			target->poison = POISON_VENOM;
			break;

		case ABILITY_EFFECT_DELUSION:
		case ABILITY_EFFECT_DAZZLE:
			PUSH_MESSAGE(MsgDelusion);
			target->delusion = STATUS_TURNS;
			break;

		case ABILITY_EFFECT_CONFUSION:
			PUSH_MESSAGE(MsgConfused);
			target->confusion = STATUS_TURNS;
			break;

		case ABILITY_EFFECT_CHARM:
			PUSH_MESSAGE(MsgCharmed);
			target->charm = STATUS_TURNS;
			break;

		case ABILITY_EFFECT_STUN:
		case ABILITY_EFFECT_STUN_THREE_ROLLS:
			PUSH_MESSAGE(MsgStunned);
			target->stun = STATUS_TURNS;
			break;

		case ABILITY_EFFECT_SLEEP:
			PUSH_MESSAGE(MsgAsleep);
			target->sleep = STATUS_TURNS;
			break;

		case ABILITY_EFFECT_SEAL:
			PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
			target->psySeal |= PSY_SEAL_BIND;
			break;

		case ABILITY_EFFECT_SEAL_2_TURNS:
			PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
			target->psySeal |= PSY_SEAL_LUFF;
			break;

		/* Instant death: "X goes down!" on the party, Drown's "X suffocates!"
		   or "X's spirit has been drained!" on an enemy. */
		case ABILITY_EFFECT_INSTANT_DEATH:
			if(btUnitKeepsOneHp(targetId) != 0)
				break;
			btPlaybackPush(PLAYBACK_DEFEAT, targetId);
			if(target->side == BATTLE_SIDE_PARTY)
				PUSH_MESSAGE(MsgKoDown);
			else if(abilityId == ABILITY_DROWN)
				PUSH_MESSAGE(MsgSuffocate);
			else
				PUSH_MESSAGE(MsgSpiritDrain);
			target->hp = 0;
			gmUnitRecalculateGaugesStub(targetId);
			break;

		case ABILITY_EFFECT_REGENERATE:
			PUSH_MESSAGE(MsgHpRegen);
			target->regenerate = REGENERATE_TURNS;
			break;

		case ABILITY_EFFECT_REFLECT:
			PUSH_MESSAGE(MsgReflect);
			target->reflect = STATUS_TURNS;
			break;

		/* The drains give the damage dealt back to the user: Drain all of it as
		   HP, Sap half. */
		case ABILITY_EFFECT_DRAIN_HP:
		case ABILITY_EFFECT_DRAIN_HALF_HP:
			amount = actor->hp;
			power = damageDealt;
			if(ability->eff == ABILITY_EFFECT_DRAIN_HALF_HP)
				power /= 2;
			drained = power;
			amount += drained;
			if(amount > actor->hpMax) {
				amount = actor->hpMax;
				drained = amount - actor->hp;
			}
			btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			if(amount == actor->hpMax)
				PUSH_MESSAGE(MsgHpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, drained);
				PUSH_MESSAGE(MsgHpRecover);
			}
			actor->hp = amount;
			gmUnitRecalculateGaugesStub(unitId);
			break;

		/* Psy Drain gives the PP taken. */
		case ABILITY_EFFECT_DRAIN_PP: {
			Sint32 heal;
			Sint32 drainedPp;

			heal = actor->pp;
			drainedPp = damageDealt;
			heal += drainedPp;
			if(heal > actor->ppMax) {
				heal = actor->ppMax;
				drainedPp = heal - actor->pp;
			}
			btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			if(heal == actor->ppMax)
				PUSH_MESSAGE(MsgPpFull);
			else {
				btPlaybackPush(PLAYBACK_NUMBER, drainedPp);
				PUSH_MESSAGE(MsgPpRecover);
			}
			actor->pp = heal;
			gmUnitRecalculateGaugesStub(unitId);
			break;
		}

		/* Psy Leech: a tenth of the damage as PP, as far as the target has it
		   and the user has room. */
		case ABILITY_EFFECT_PSY_LEECH:
			amount = damageDealt / 10;
			if(target->pp < amount)
				amount = target->pp;
			if(actor->pp + amount > actor->ppMax)
				amount = actor->ppMax - actor->pp;
			if(amount == 0)
				break;
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			PUSH_SIDE_MESSAGE(targetId, MsgLeechTake, MsgLeechGain);
			gmUnitAddPpStub(unitId, amount);
			break;

		/* Catastrophe also takes a tenth of the target's max PP, as far as it
		   has it. */
		case ABILITY_EFFECT_CATASTROPHE:
			amount = (Sint16)(target->ppMax / 10);
			if(target->pp < amount)
				amount = target->pp;
			if(amount == 0)
				break;
			btPlaybackPush(PLAYBACK_NUMBER, amount);
			PUSH_SIDE_MESSAGE(targetId, MsgMorePpLossP, MsgMorePpLossE);
			gmUnitAddPpStub(targetId, -amount);
			break;

		/* Break returns raised Attack, Defense, Resistance and Agility stages
		   to 0, leaving lowered ones, and takes away a summon's boost. */
		case ABILITY_EFFECT_BREAK:
			if(target->attackStage > 0) {
				target->attackStage = 0;
				target->attackTurns = 0;
			}
			if(target->defenseStage > 0) {
				target->defenseStage = 0;
				target->defenseTurns = 0;
			}
			if(target->resistanceStage > 0) {
				target->resistanceStage = 0;
				target->resistanceTurns = 0;
			}
			if(target->agilityStage > 0)
				target->agilityStage = 0;
			target->summonBoost[ELEMENT_VENUS] = 0;
			target->summonBoost[ELEMENT_MERCURY] = 0;
			target->summonBoost[ELEMENT_MARS] = 0;
			target->summonBoost[ELEMENT_JUPITER] = 0;
			PUSH_MESSAGE(MsgBuffsReset);
			break;

		case ABILITY_EFFECT_HAUNT:
			PUSH_MESSAGE(MsgEvilSpirit);
			target->haunt = 1;
			break;

		case ABILITY_EFFECT_CURSE:
		case ABILITY_EFFECT_CURSE_THREE_ROLLS:
			if(target->grimReaper == 0) {
				PUSH_MESSAGE(MsgDeathCurse);
				target->grimReaper = CURSE_TURNS;
				break;
			}
			if(target->grimReaper <= 1)
				break;
			target->grimReaper -= 1;
			btPlaybackPush(PLAYBACK_NUMBER, target->grimReaper);
			PUSH_MESSAGE(MsgDeathCount);
			break;

		/* Kite: the target acts twice in the next turn (the count drops at each
		   turn's end). */
		case ABILITY_EFFECT_ACT_TWICE:
			PUSH_MESSAGE(MsgReadies);
			target->ready = 2;
			break;

		/* The target rises to the challenge and won't flee; on the party, no
		   one may run ("No time to run!"). */
		case ABILITY_EFFECT_CHALLENGE:
			PUSH_MESSAGE(MsgChallenge);
			target->challenge = 1;
			if(IS_PARTY_UNIT(targetId))
				battle->challengeFlg |= 2;
			break;

		/* The target loses its next action (btActionRunBuild). */
		case ABILITY_EFFECT_GROUND:
		case ABILITY_EFFECT_ULYSSES:

			PUSH_MESSAGE(MsgImmobile);
			target->immobile = 1;
			break;

		/* The guards: the message shows either way, but a guard is never
		   lowered. */
		case ABILITY_EFFECT_DEFEND:

			PUSH_MESSAGE(MsgDefendAura);
			if(target->defendLevel > 0)
				break;
			target->defendLevel = DEFEND_NORMAL;
			break;

		case ABILITY_EFFECT_SHIELD:
			PUSH_MESSAGE(MsgDefendAura);
			if((Uint32)target->defendLevel > DEFEND_NORMAL)
				break;
			target->defendLevel = DEFEND_SHIELD;
			break;

		case ABILITY_EFFECT_TOTAL_DEFENSE:
		case ABILITY_EFFECT_GUARD_AURA:
			PUSH_MESSAGE(MsgTotalDefenseAura);
			if(target->defendLevel > DEFEND_SHIELD)
				break;
			target->defendLevel = DEFEND_TOTAL;
			break;

		case ABILITY_EFFECT_VANISH:
			PUSH_MESSAGE(MsgVanished);
			if((Uint32)target->defendLevel > DEFEND_TOTAL)
				break;
			target->defendLevel = DEFEND_VANISHED;
			break;

		/* Contain, Threaten and Tremble do nothing; the player only reads their
		   message. */
		case ABILITY_EFFECT_THREATEN:
			btPlaybackPush(PLAYBACK_MESSAGE, MESSAGE_NONE);
			break;

		case ABILITY_EFFECT_CEASE_FIRE:
			PUSH_MESSAGE(MsgAllResting);
			battle->ceaseFire = 1;
			break;

		/* Gale blows the target out of the battle, as one that flees. */
		case ABILITY_EFFECT_BLOW_AWAY:
			PUSH_MESSAGE(MsgBlownAway);
			btPlaybackPush(PLAYBACK_BLOW_AWAY, targetId);
			break;

		/* The second equipment total doubles as the readiness to
		   counterattack. */
		case ABILITY_EFFECT_COUNTER:
			PUSH_MESSAGE(MsgCounterReady);
			target->equipBonus[1] = 1;
			break;

		default:
			break;
		}
	}

	/* 7. Waking from damage, and the stats worked out again. */
done:
	btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
	/* A unit ready to counterattack remembers who struck it. */
	if(plan->command != COMMAND_DJINN_COUNTER &&
		(Sint8)target->equipBonus[1] != 0 &&
		ability->tgtMode == ABILITY_TARGET_FOES && target->hp != 0) {
		battle->counterUnitId = targetId;
		battle->counterTgtId = unitId;
	}

	/* Damage wakes a sleeping target one time in four, unless it fell asleep
	   this turn. */
	if(target->hp != 0) {
		if(target->sleep != 0)
			if(target->sleep < STATUS_TURNS && damageDealt > 0 &&
				(3 & gmBattleRandomStub()) == 0) {
				target->sleep = 0;
				btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
				PUSH_MESSAGE(MsgWakes);
			}
	}
	syHeapRelease(savedTarget);
	gmUnitCalcStub(targetId);
	mnPartyStatDrawStub(Ram_HeapSlots->battleState->partyStat);
	if(target->hp != 0)
		btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
	/* A haunted unit pays, one time in four, a quarter of what it took, at the
	   end of its action; Daedalus's second strike does not count. */
	if(abilityId != ABILITY_DAEDALUS_STRIKE && actor->haunt != 0 &&
		(gmBattleRandomStub() & 3) == 0 && damageDealt > 0) {
		Sint32 share;

		share = damageDealt >> 2;
		if(share == 0)
			share = 1;
		plan->hauntDamage += share;
	}
}

/* Returns 1 for abilities that cost PP, and for Psy Drain whatever its cost. */
Uint32 btAbilityIsPsynergy(Sint32 abilityId)
{
	Uint8 pp;

	if(abilityId == ABILITY_PSY_DRAIN)
		return 1;
	pp = gmAbilityGetDirectStub(abilityId)->ppCost;
	return pp != 0;
}

/* The battle's state, as the forms of Poseidon and the Doom Dragon read it
   before btAIChooseAction sets its own battle further on. */
#define AI_BATTLE ((BATTLE_STATE *)Ram_HeapSlots->battleState)

/* The Doom Dragon's poses for its later actions in a turn, beside POSE_CAST, a
   head thrust forward: one head lunging low, and one reared high
   (battle_doom_dragon_a.png's pictures 6 and 8, and 14 and 16). */
#define DOOM_DRAGON_POSE_LOW  6
#define DOOM_DRAGON_POSE_HIGH 7

/* The choices an enemy may fail to carry out before it defends instead. */
#define AI_MAX_FAILED_CHOICES 16

/* Chooses an enemy's action: one of the eight in its definition, picked by its
   action strategy, with an item instead where its item mask says so, and a
   target worth the ability. Called with retry 0 for each enemy as the turn is
   built and with retry 1 when it acts (btStagingDispatchAction). */
Sint32 btAIChooseAction(BATTLE_ACTION_RECORD *request, Sint32 retry)
{
	UNIT					*unit;
	Uint32					 selection;
	BATTLE_AI_STATE			*selectionMemory;
	const ENEMY_DEFINITION	*enemy;
	Bool					 itemsAllowed;
	Sint32					 failedChoices;
	Bool					 needsChoice;
	const AI_ENEMY_REST		*halfwords;
	const Sint8				*actionStrategy;
	const Uint8				*itemActionMask;
	Sint16					 command;
	Sint32					 allyTarget;
	Sint32					 opponentTarget;
	Bool					 allyIsParty;
	Bool					 opponentIsParty;
	BATTLE_STATE			*battle;
	ITEM_DEFINITION			*item;
	Sint32					 useItem;
	Sint8					 strategyKind;
	ABILITY_DEFINITION		*ability;
	Sint32					 abilityId;
	Uint8					 counts[4];
	Uint8					 summons[36];
	Sint32					 summonIndex;
	Sint32					 element;
	const SUMMON_DEFINITION *summon;

	/* Picks a place in the action list from a 0-255 roll and eight weights: the
	   first place whose running total passes the roll, else place 0. */
	Sint32 btEnemySelectWeightedAction(Uint8 *weights)
	{
		Sint32 roll;
		Sint32 total;
		Sint32 selectedAction;
		Sint32 actionIndex;

		roll = gmBattleRandomStub() & 0xff;
		total = weights[0];
		selectedAction = 0;
		actionIndex = 0;
		if(roll >= total) {
			while(++actionIndex <= 7) {
				total += weights[actionIndex];
				if(roll < total) {
					selectedAction = actionIndex;
					break;
				}
			}
		}
		return selectedAction;
	}

	unit = syUnitGet(request->unitId);
	itemsAllowed = TRUE;
	failedChoices = 0;
	needsChoice = TRUE;
	selection = -1U;
	if(unit->classId != CLASS_NONE)
		return -1;

	selectionMemory = &unit->aiState;
	enemy = gmEnemyGetDefinitionStub(unit->enemyId);
	if(unit->enemyId == ENEMY_POSEIDON) {
		selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
		if(request->turnAction == 1)
			enemy = gmEnemyGetDefinitionStub(ENEMY_POSEIDON_SECOND_ACTION);
	} else if(unit->enemyId == ENEMY_DOOM_DRAGON_A) {
		/* Each later action of the turn strikes with another pose of its
		   heads. */
		selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
		switch(request->turnAction) {
		case 1:
			AI_BATTLE->actingAnm = DOOM_DRAGON_POSE_LOW;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_C);
			break;
		case 2:
			AI_BATTLE->actingAnm = POSE_CAST;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_B);
			break;
		case 3:
			AI_BATTLE->actingAnm = DOOM_DRAGON_POSE_HIGH;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_D);
			break;
		}
	} else if(unit->enemyId == ENEMY_DOOM_DRAGON_E) {
		selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
		switch(request->turnAction) {
		case 1:
			AI_BATTLE->actingAnm = DOOM_DRAGON_POSE_LOW;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_G);
			break;
		case 2:
			AI_BATTLE->actingAnm = POSE_CAST;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_F);
			break;
		}
	} else if(unit->enemyId == ENEMY_DOOM_DRAGON_H) {
		selectionMemory = &AI_BATTLE->aiStates[request->turnAction];
		if(request->turnAction == 1) {
			AI_BATTLE->actingAnm = POSE_CAST;
			enemy = gmEnemyGetDefinitionStub(ENEMY_DOOM_DRAGON_I);
		}
	} else if(unit->enemyId == ENEMY_DULLAHAN) {
		if(selectionMemory->initialized == 0)
			selectionMemory->form = syRandomBelow(3);
		enemy = gmEnemyGetDefinitionStub(
			BattleAI_DullahanForms[selectionMemory->form]);
	}

	if(retry != 0 && request->command != COMMAND_ABILITY) {
		switch(enemy->actionStrategy) {
		case ACTION_STRATEGY_RANDOM_START:
		case ACTION_STRATEGY_IN_ORDER:
			selectionMemory->selection = selectionMemory->selection + 1;
			break;
		}
		goto done;
	}

	actionStrategy = &enemy->actionStrategy;
	itemActionMask = &enemy->itemActions;
	halfwords = (const AI_ENEMY_REST *)((const Uint8 *)enemy + 2);

select_again:
	strategyKind = *actionStrategy;
	switch(strategyKind) {
	case ACTION_STRATEGY_EVEN:
		selection = btEnemySelectWeightedAction(
			BattleAI_ActionWeights[ACTION_STRATEGY_EVEN]);
		break;
	case ACTION_STRATEGY_FALLING:
		selection = btEnemySelectWeightedAction(
			BattleAI_ActionWeights[ACTION_STRATEGY_FALLING]);
		break;
	case ACTION_STRATEGY_FIRST_FAVOURED:
		selection = btEnemySelectWeightedAction(
			BattleAI_ActionWeights[ACTION_STRATEGY_FIRST_FAVOURED]);
		break;
	case ACTION_STRATEGY_RANDOM_START:
		if(selectionMemory->initialized == 0) {
			selectionMemory->selection = gmBattleRandomStub();
			selectionMemory->initialized = 1;
		}
		/* fall through */
	case ACTION_STRATEGY_IN_ORDER:
		selection = selectionMemory->selection;
		if(retry != 0)
			selectionMemory->selection = selection + 1;
		break;
	case ACTION_STRATEGY_NEXT_PICK:
		selection++;
		break;
	case ACTION_STRATEGY_SAME:
		break;
	}

	useItem = (*itemActionMask >> selection) & 1;
	useItem &= itemsAllowed;
	abilityId = halfwords->abilities[selection];
	request->command = COMMAND_ABILITY;
	if(useItem != 0 && retry != 0) {
		if(!(INVENTORY_ITEM_ID_MASK & unit->inventory[0])) {
			useItem = 0;
			if(enemy->tgtStrategy == TARGET_STRATEGY_MOST_HP) {
				request->command = COMMAND_ITEM;
				{
					Sint32 cantUse = ABILITY_CANT_USE;

					request->prm = cantUse;
				}
				goto done;
			}
		}
		if(useItem != 0) {
			item = gmItemGetDirectStub(unit->inventory[0]);
			if(item->useType != ITEM_USE_CONSUMED) {
				useItem = 0;
			} else {
				gmAbilityGetDirectStub(item->abilityId);
				abilityId = item->abilityId;
				request->command = COMMAND_ITEM;
				request->prm = 0;
			}
		}
		if(useItem == 0)
			itemsAllowed = FALSE;
	}

	if(needsChoice != 0) {
		ability = gmAbilityGetDirectStub(abilityId);
		switch(ability->eff) {
		case ABILITY_EFFECT_DEFEND:
			request->command = COMMAND_DEFEND;
			request->tgt = btFindUnitPlace(request->unitId);
			break;
		case ABILITY_EFFECT_TOTAL_DEFENSE:
			request->command = COMMAND_TOTAL_DEFENSE;
			request->tgt = btFindUnitPlace(request->unitId);
			break;
		case ABILITY_EFFECT_FLEE:
			request->command = COMMAND_FLEE;
			request->tgt = btFindUnitPlace(request->unitId);
			break;
		case ABILITY_EFFECT_COUNTER:
		case ABILITY_EFFECT_GUARD_AURA:
			if(retry == 0)
				request->pri += PRIORITY_FIRST_IN_TURN;
			break;
		}

		if(retry == 0) {
			switch(request->command) {
			case COMMAND_DEFEND:
			case COMMAND_TOTAL_DEFENSE:
				break;
			default:
				goto done;
			}
		}

		if(request->command != COMMAND_ITEM &&
			btAbilityIsPsynergy(abilityId) != 0) {
			request->command = COMMAND_PSYNERGY;
			request->prm = abilityId;
			if(ability->ppCost > unit->pp &&
				enemy->tgtStrategy != TARGET_STRATEGY_MOST_HP)
				goto next_candidate;
			if(unit->psySeal != 0 &&
				enemy->tgtStrategy == TARGET_STRATEGY_RANDOM)
				goto next_candidate;
		}
		command = request->command;
		if(command == COMMAND_FLEE && (Uint8)unit->challenge != 0)
			goto next_candidate;
		if(retry != 0 &&
			(command == COMMAND_DEFEND || command == COMMAND_TOTAL_DEFENSE))
			goto next_candidate;

		if(command == COMMAND_ABILITY) {
			request->prm = abilityId;
			if(abilityId == ABILITY_ATTACK)
				request->command = COMMAND_ATTACK;
			if(abilityId == ABILITY_CRUCIBLE) {
				request->command = COMMAND_SUMMON;
				btDjinnCountStandby(0, counts);
				summonIndex = gmSummonListLearnedStub(summons);
				for(summonIndex--; summonIndex >= 0; summonIndex--) {
					summon =
						gmSummonDefinitionGetNearStub(summons[summonIndex]);
					for(element = 0; element <= ELEMENT_JUPITER &&
						summon->djinnRequired[element] <= counts[element];
						element++)
						;
					if(element == ELEMENT_COUNT)
						break;
				}
				if(summonIndex < 0)
					goto next_candidate;
				request->prm = summons[summonIndex];
				abilityId = gmSummonDefinitionGetNearStub(summons[summonIndex])
								->abilityId;
				ability = gmAbilityGetDirectStub(abilityId);
			}
		}

		request->range = ability->range;
		switch(ability->tgtMode) {
		case ABILITY_TARGET_ALLIES:
		case ABILITY_TARGET_SELF:
			if(abilityId == ABILITY_GUARD_AURA ||
				abilityId == ABILITY_EARNEST_PLY)
				allyTarget = btAIChooseTarget(FIRST_ENEMY_UNIT, ability);
			else
				allyTarget = btAIChooseTarget(request->unitId, ability);
			if(allyTarget == TARGET_PICK_SIDE_EMPTY) {
				allyIsParty = FALSE;
				if((Uint16)request->unitId <= ALLY_MAX)
					allyIsParty = TRUE;
				allyTarget = btAIChooseRandomTarget(allyIsParty);
			}
			if(allyTarget != TARGET_PICK_NONE) {
				request->tgt = allyTarget;
				needsChoice = FALSE;
			}
			break;
		case ABILITY_TARGET_FOES:
			opponentTarget = btAIChooseTarget(request->unitId, ability);
			if(opponentTarget == TARGET_PICK_SIDE_EMPTY) {
				opponentIsParty = FALSE;
				if((Uint16)request->unitId <= ALLY_MAX)
					opponentIsParty = TRUE;
				opponentTarget = btAIChooseRandomTarget(opponentIsParty);
			}
			if(opponentTarget != TARGET_PICK_NONE) {
				request->tgt = opponentTarget;
				needsChoice = FALSE;
			}
			break;
		case 3: /* Unused: no ability's target mode is 3. */
			request->tgt = btFindUnitPlace(request->unitId);
			break;
		default:
			if(ability->eff == ABILITY_EFFECT_CALL_REINFORCEMENT &&
				!btEnemyHasSprRoom(unit->enemyId == ENEMY_STAR_MAGICIAN
						? ENEMY_REFRESH_BALL
						: ENEMY_SEA_FIGHTER))
				break;
			battle = (BATTLE_STATE *)Ram_HeapSlots->battleState;
			if(ability->eff == ABILITY_EFFECT_CALL_QUEUED) {
				if(battle->spoils.queueCnt == 0)
					break;
				if(!btEnemyHasSprRoom(
					   battle->spoils.queuedSummons[battle->spoils.queueHead]))
					break;
			}
			request->tgt = btFindUnitPlace(request->unitId);
			needsChoice = FALSE;
			break;
		}
	}
	if(retry == 0)
		needsChoice = FALSE;
next_candidate:
	if(needsChoice != 0 && failedChoices > AI_MAX_FAILED_CHOICES) {
		request->command = COMMAND_DEFEND;
		needsChoice = FALSE;
	}
	failedChoices++;
	if(needsChoice != 0)
		goto select_again;
done:
	if(unit->enemyId == ENEMY_DULLAHAN && retry == 1 &&
		selectionMemory->selection == 0)
		selectionMemory->form = (selectionMemory->form + 1) % 3;
	return 0;
}

/* Unused: makes three empty DMA3 transfers after a multiplication whose result
   it drops. */
void btDMAStopDMA3(void)
{
	syMathUnsignedMulHigh(5, 0);
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
	syDMAXCopy(0, 0, DMA_COPY_WORDS(0), REG_DMA3);
}
