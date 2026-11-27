/* @file */
/* The end of an action and of a turn: counters, Djinn recovery, poison and */
/* the Grim Reaper. */
/* Part of battle.c. */

#include "battle_internal.h"

/* How much likelier, in percent, each lowered stat and status is to end early
 */
/* (btUnitRollCounterEarlyEnd's bias). */
typedef enum EARLY_END_BIAS {
	EARLY_END_ATTACK_DOWN = 30,
	EARLY_END_DEFENSE_DOWN = 20,
	EARLY_END_RESISTANCE_DOWN = 20,
	EARLY_END_DELUSION = 30,
	EARLY_END_CONFUSION = 60,
	EARLY_END_CHARM = 70,
	EARLY_END_STUN = 40,
	EARLY_END_SLEEP = 50,
	EARLY_END_BIND = 30 /* Bind's seal, once Luff's is gone */
} EARLY_END_BIAS;

/* Rolls whether a stat change or status with five turns or fewer left ends */
/* early, at `luck * 3 - turns * 5 + bias` percent, and returns 1 when it does.
 */
s32 btUnitRollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
	UNIT *unit = gmUnitGetStub(unitId);

	if(turns <= 5) {
		/* 65536 / 100, rounded down, scales the percentage to the 16-bit random
		 */
		/* range. */
		s32 threshold = ((unit->luck * 3 - turns * 5) + bias) * (65536 / 100);
		if(threshold >= (btRandomStub() & 0xffff))
			return 1;
	}
	return 0;
}

/* Counts down an Attack change and returns 1 when it ends; a lowered Attack */
/* may recover early. */
s32 btUnitCountDownAttackStage(s32 unitId) {
	UNIT *unit = gmUnitGetStub(unitId);

	if(unit->attackTurns != 0) {
		unit->attackTurns--;
		if(unit->attackTurns == 0) {
			unit->attackStage = 0;
			return 1;
		}
		if(unit->attackStage < 0) {
			if(btUnitRollCounterEarlyEnd(unitId, unit->attackTurns,
										 EARLY_END_ATTACK_DOWN) != 0) {
				unit->attackStage = 0;
				unit->attackTurns = 0;
				return 1;
			}
		}
	}
	return 0;
}

/* Defines a stat change's turn-end countdown, as */
/* btUnitCountDownAttackStage is, a lowered stat ending early at bias. */
#define DEFINE_COUNT_DOWN_MODIFIER(name, counter, level, bias)                 \
	s32 name(s32 unitId) {                                                     \
		UNIT *unit = gmUnitGetStub(unitId);                                    \
                                                                               \
		if(unit->counter != 0) {                                               \
			unit->counter--;                                                   \
			if(unit->counter == 0) {                                           \
				unit->level = 0;                                               \
				return 1;                                                      \
			}                                                                  \
			if(unit->level < 0 &&                                              \
			   btUnitRollCounterEarlyEnd(unitId, unit->counter, bias) != 0) {  \
				unit->level = 0;                                               \
				unit->counter = 0;                                             \
				return 1;                                                      \
			}                                                                  \
		}                                                                      \
		return 0;                                                              \
	}

/* Counts down a Defense change; btProcessTurnEnd shows `MsgDefNormal` */
/* when it returns 1. */
DEFINE_COUNT_DOWN_MODIFIER(btUnitCountDownDefenseStage, defenseTurns,
						   defenseStage, EARLY_END_DEFENSE_DOWN)
/* Counts down a Resistance change; btProcessTurnEnd shows */
/* `MsgResNormal` when it returns 1. */
DEFINE_COUNT_DOWN_MODIFIER(btUnitCountDownResistanceStage, resistanceTurns,
						   resistanceStage, EARLY_END_RESISTANCE_DOWN)

/* Defines a status's turn-end countdown: returns 1 when `counter` runs out, or
 */
/* clears it and returns 1 when the status ends early at `bias`. */
#define DEFINE_COUNT_DOWN_STATUS(name, counter, bias)                          \
	s32 name(s32 unitId) {                                                     \
		UNIT *unit = gmUnitGetStub(unitId);                                    \
		if(unit->counter != 0) {                                               \
			unit->counter--;                                                   \
			if(unit->counter == 0)                                             \
				return 1;                                                      \
			if(btUnitRollCounterEarlyEnd(unitId, unit->counter, bias) != 0) {  \
				unit->counter = 0;                                             \
				return 1;                                                      \
			}                                                                  \
		}                                                                      \
		return 0;                                                              \
	}

/* Counts down delusion, which may end early; btProcessTurnEnd shows */
/* `MsgCureDelusion` when it returns 1. */
DEFINE_COUNT_DOWN_STATUS(btUnitCountDownDelusion, delusion, EARLY_END_DELUSION)

/* Counts down confusion, which may end early; btProcessTurnEnd shows */
/* `MsgConfusionConscious` when it returns 1. */
DEFINE_COUNT_DOWN_STATUS(btUnitCountDownConfusion, confusion,
						 EARLY_END_CONFUSION)

/* Counts down charm, which may end early; btProcessTurnEnd shows */
/* `MsgCharmConscious` when it returns 1. */
DEFINE_COUNT_DOWN_STATUS(btUnitCountDownCharm, charm, EARLY_END_CHARM)
/* Counts down stun, which may end early; btProcessTurnEnd shows */
/* `MsgCureStun` when it returns 1. */
DEFINE_COUNT_DOWN_STATUS(btUnitCountDownStun, stun, EARLY_END_STUN)
/* Counts down sleep, which may end early; btProcessTurnEnd shows */
/* `MsgWakes` when it returns 1. */
DEFINE_COUNT_DOWN_STATUS(btUnitCountDownSleep, sleep, EARLY_END_SLEEP)

/* Counts down both seals in UNIT.psySeal and returns 1 when the last ends; */
/* only Bind's, once Luff's is gone, may end early. */
s32 btUnitCountDownPsynergySeal(s32 unitId) {
	UNIT *unit = gmUnitGetStub(unitId);

	if(unit->psySeal != 0) {
		if(unit->psySeal > PSY_SEAL_BIND_MASK)
			unit->psySeal -= PSY_SEAL_LUFF_TURN;
		if(unit->psySeal & PSY_SEAL_BIND_MASK)
			unit->psySeal--;
		if(unit->psySeal == 0)
			return 1;
		if(unit->psySeal <= PSY_SEAL_BIND_MASK &&
		   btUnitRollCounterEarlyEnd(unitId, unit->psySeal, EARLY_END_BIND) !=
			   0) {
			unit->psySeal = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down UNIT.regenerate and returns 1 when it runs out; it never */
/* ends early. btProcessTurnEnd then shows `MsgRefrainEnd`. */
s32 btUnitCountDownRegenerate(s32 unitId) {
	u8 *turns = &gmUnitGetStub(unitId)->regenerate;
	if(*turns != 0) {
		(*turns)--;
		if(*turns == 0) {
			return 1;
		}
	}
	return 0;
}

/* Counts down Reflect and returns 1 when it runs out; it never ends early. */
/* btProcessTurnEnd then shows `MsgReflectEnd`. */
s32 btUnitCountDownReflect(s32 unitId) {
	u8 *turns = &gmUnitGetStub(unitId)->reflect;
	if(*turns != 0) {
		(*turns)--;
		if(*turns == 0) {
			return 1;
		}
	}
	return 0;
}

/* Counts down an Agility change and returns 1 when it runs out; it never ends
 */
/* early. */
s32 btUnitCountDownAgilityStage(s32 unitId) {
	UNIT *unit = gmUnitGetStub(unitId);
	u8 *turns = &unit->agilityTurns;
	if(*turns != 0) {
		(*turns)--;
		if(*turns == 0) {
			unit->agilityStage = 0;
			return 1;
		}
	}
	return 0;
}

/* Brings the party's recovering Djinn one turn nearer, setting each that is */
/* ready, and returns 1 when any was set. */
s32 btDjinnRecoveryCountDown(void) {
	DJINN_RECOVERY_LIST *list =
		&gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->list;
	s32 removed = 0;
	s32 i;

	for(i = 0; i < list->cnt; i++) {
		DJINN_RECOVERY_ENTRY *entry = &list->entries[i];

		if(entry->turns > 0 && gmUnitGetStub(entry->unitId)->hp != 0)
			entry->turns--;
	}
	/* Removing an entry moves the next one into its place, so i advances only
	 */
	/* past entries that stay. */
	for(i = 0; i < list->cnt;) {
		DJINN_RECOVERY_ENTRY *entry = &list->entries[i];

		if(entry->turns == 0) {
			u8 unitId = entry->unitId;

			gmDjinnSetStub(unitId, entry->element, entry->idx);
			btDjinnRecoveryRemoveStub(unitId, entry->element, entry->idx);
			gmUnitCalcStub(unitId);
			removed = 1;
		}
		else {
			i++;
		}
	}
	return removed;
}

/* Runs btDjinnRecoveryCountDown twenty times, which sets every recovering */
/* Djinni of a party member with HP; those on standby stay so. Returns 0. */
s32 btDjinnRecoveryCountDownTwenty(void) {
	s32 turn;

	for(turn = 0; turn < 20; turn++)
		btDjinnRecoveryCountDown();
	return 0;
}

/* Does nothing; btRun calls it at the end of each turn and of */
/* the battle. */
void btTurnEndNoOp(void) {}

/* Ends a turn: sets recovered Djinn, then gives each unit its regeneration and
 */
/* counts down its stat changes and statuses, showing each that ends. */
UNREAD_RESULT btProcessTurnEnd(void) {
	BATTLE_STATE *battle;
	s32 side;
	s32 sideCount;
	s32 count;
	s32 i;
	DJINN_RECOVERY_LIST *list;

	/* Djinn recovery: the party's list, and in a link battle the opponent's */
	/* too. Only a Djinni whose unit is on the field with HP counts down. */
	battle = BattleStateWork;
	sideCount = (battle->isLink != 0) + 1;
	for(side = 0; side < sideCount; side++) {
		list = &gmDjinnGetRecoveryTableStub(side)->list;
		for(i = 0; i < list->cnt; i++) {
			DJINN_RECOVERY_ENTRY *entry = &list->entries[i];

			if(entry->turns > 0 && btObjectGet(entry->unitId) != 0 &&
			   gmUnitGetStub(entry->unitId)->hp != 0)
				entry->turns--;
		}
		/* Set each recovered Djinni, with its message and its element's effect.
		 */
		for(i = 0; i < list->cnt;) {
			DJINN_RECOVERY_ENTRY *entry = &list->entries[i];

			if(entry->turns == 0) {
				u8 unitId = entry->unitId;
				s32 element;

				if(btObjectGet(unitId) != 0) {
					btPlaybackReset();
					btPlaybackSchedulePhase(30);
					btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
					btPlaybackPush(PLAYBACK_ABILITY_NAME,
								   entry->element * DJINN_PER_ELEMENT +
									   entry->idx + ABILITY_DJINNI_FIRST);
					btPlaybackPush(PLAYBACK_SOUND, SE_NOTE_HIGH);
					btPlaybackPush(PLAYBACK_DRAW_STATUS, 0);
					PUSH_MESSAGE(MsgDjinnSet);
					btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
					sySndPlayStub(SE_POWER_UP);
					fdActorSelectAnimationStub(btObjectGet(unitId)->actor,
											   POSE_CAST);
					fdActorSetAnimationSpeedStub(btObjectGet(unitId)->actor,
												 ANIM_STEP_NORMAL * 2);
					element = entry->element;
					gmDjinnSetStub(unitId, element, entry->idx);
					btDjinnRecoveryRemoveStub(unitId, entry->element,
											  entry->idx);
					gmUnitCalcStub(unitId);
					btElementGlowPlay(unitId, element, GLOW_BURST, 0);
					btPlaybackWaitForReady();
				}
			}
			else {
				i++;
			}
		}
	}
	/* Fade 0 leaves the palette as it is: this only puts the zoomed */
	/* background back and rebuilds its map. */
	btBackgroundSetMode(BATTLE_BACKGROUND_ZOOMED, BattleStateWork->bg, 0);
	{
		s32 *groupList;
		BATTLE_SIDE_ORDER order = Battle_TurnEndSideOrder;
		u16 unitIds[UNIT_MAX];
		s32 group;
		s32 unitIndex;

		/* Each unit's regeneration and counters, the party's first; the link
		 * child */
		/* takes the enemies first, so both consoles show the same order. */
		if(battle->isLink != 0 && battle->linkSide != 0) {
			order.group[0] = BATTLE_SIDE_MASK_ENEMIES;
			order.group[1] = BATTLE_SIDE_MASK_PARTY;
		}
		groupList = order.group;
		for(group = 0; group <= 1; group++) {
			count = btUnitListIds(groupList[group], unitIds);
			for(unitIndex = 0; unitIndex < count; unitIndex++) {
				s32 hpAmount;
				s32 ppAmount;
				s32 unitId = unitIds[unitIndex];
				UNIT *unit = gmUnitGetStub(unitId);

				if(unit->ready != 0) {
					unit->ready--;
				}
				/* Regeneration, capped at the maximum, for a unit still
				 * standing. */
				if(unit->hp != 0) {
					if(unit->hpRegen != 0 && unit->hp != unit->hpMax) {
						hpAmount = unit->hpRegen;
						if(unit->hp + hpAmount > unit->hpMax) {
							hpAmount = unit->hpMax - unit->hp;
						}
						gmUnitAddHpStub(unitId, hpAmount);
						mnMessageArgumentPushStub(unitId,
												  MESSAGE_ARGUMENT_ACTOR);
						mnMessageArgumentPushStub(hpAmount,
												  MESSAGE_ARGUMENT_NUMBER);
						if(unit->hp == unit->hpMax) {
							mnBattleMessageShowAndWaitStub(MsgHpFull);
						}
						else {
							mnBattleMessageShowAndWaitStub(MsgHpRecover);
						}
						sySndPlayStub(SE_NOTE_HIGH);
						btMessageWaitForAdvance();
					}
					if(unit->ppRegen != 0 && unit->pp != unit->ppMax) {
						ppAmount = unit->ppRegen;
						if(unit->pp + ppAmount > unit->ppMax) {
							ppAmount = unit->ppMax - unit->pp;
						}
						gmUnitAddPpStub(unitId, ppAmount);
						mnMessageArgumentPushStub(unitId,
												  MESSAGE_ARGUMENT_ACTOR);
						mnMessageArgumentPushStub(ppAmount,
												  MESSAGE_ARGUMENT_NUMBER);
						if(unit->pp == unit->ppMax) {
							mnBattleMessageShowAndWaitStub(MsgPpFull);
						}
						else {
							mnBattleMessageShowAndWaitStub(MsgPpRecover);
						}
						sySndPlayStub(SE_NOTE_HIGH);
						btMessageWaitForAdvance();
					}
				}
				/* Each counter that ends rebuilds the unit's status icons and
				 * shows */
				/* its message; the end of stun or sleep also sets the unit
				 * standing. */
				if(btUnitCountDownAgilityStage(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgAgiNormal);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownAttackStage(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgAtkNormal);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownDefenseStage(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgDefNormal);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownResistanceStage(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgResNormal);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownDelusion(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgCureDelusion);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownConfusion(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgConfusionConscious);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownCharm(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgCharmConscious);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownStun(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					btObjectUpdatePose(unitId);
					mnBattleMessageShowAndWaitStub(MsgCureStun);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownSleep(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					btObjectUpdatePose(unitId);
					mnBattleMessageShowAndWaitStub(MsgWakes);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownPsynergySeal(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgCureSeal);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownRegenerate(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgRefrainEnd);
					btMessageWaitForAdvance();
				}
				if(btUnitCountDownReflect(unitId) != 0) {
					btUnitBuildStatFlags(unitId, btObjectGet(unitId));
					mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(MsgReflectEnd);
					btMessageWaitForAdvance();
				}
			}
		}
	}
}

/* Ends one unit's action: its Djinn on standby are taken off its set, the */
/* Djinn a summon used begin to recover, and the unit takes its summon's power
 */
/* boost, the haunt's damage, poison and the Grim Reaper. */
UNREAD_RESULT btUnitProcessActionEnd(BATTLE_ACTION_RUN *plan) {
	s32 unitId;
	UNIT *unit;
	s32 battleContinues;
	s32 counts[ELEMENT_COUNT];
	DJINN_RECOVERY_LIST *list;
	/* An index: a recovery list entry's, then the strongest element's. */
	s32 index;
	s32 recoveryTurn;

	/* The unit's standby Djinn come off its set. */
	unitId = plan->unitId;
	battleContinues = 0;
	unit = gmUnitGetStub(unitId);
	list = &gmDjinnGetRecoveryTableStub(unitId < 0 || unitId > ALLY_MAX)->list;
	for(index = 0; index < list->cnt; index++) {
		if(list->entries[index].unitId == unitId &&
		   list->entries[index].turns == DJINN_RECOVERY_AFTER_BATTLE) {
			gmDjinnPutOnStandbyStub(unitId, list->entries[index].element,
									list->entries[index].idx);
		}
	}
	if(btUnitListLiving(BATTLE_SIDE_MASK_PARTY, 0) != 0 &&
	   btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) != 0) {
		battleContinues = 1;
	}
	{
		s32 i;
		s32 holderId;
		DJINN_RECOVERY_LIST *pending;
		pending =
			&gmDjinnGetRecoveryTableStub(unitId < 0 || unitId > ALLY_MAX)->list;
		/* The Djinn a summon used recover one a turn, after those already */
		/* recovering and no sooner than in 2 turns. */
		for(i = 0; i < ELEMENT_COUNT; i++) {
			counts[i] = 0;
		}
		for(;;) {
			holderId = -1;
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
					holderId = pending->entries[i].unitId;
					break;
				}
			}
			if(holderId == -1) {
				break;
			}
			recoveryTurn = -1;
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].unitId == holderId &&
				   pending->entries[i].turns > recoveryTurn) {
					recoveryTurn = pending->entries[i].turns;
				}
			}
			recoveryTurn++;
			if(recoveryTurn <= 1) {
				recoveryTurn = 2;
			}
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].unitId == holderId &&
				   pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
					pending->entries[i].turns = recoveryTurn;
					counts[pending->entries[i].element]++;
					recoveryTurn++;
				}
			}
		}
	}
	if(battleContinues != 0) {
		UNIT *before;
		s32 mostOfOneElement;
		s32 i;
		/* The summon's boost: the element it used most Djinn of rises that many
		 */
		/* levels. */
		mostOfOneElement = 0;
		before = syHeapAllocEwram(sizeof(UNIT));
		syWordsCopy(before, unit, sizeof(UNIT));
		index = -1;
		for(i = 0; i < ELEMENT_COUNT; i++) {
			if(counts[i] > mostOfOneElement) {
				mostOfOneElement = counts[i];
				index = i;
			}
		}
		if(index >= 0 && unit->summonBoost[index] < mostOfOneElement) {
			unit->summonBoost[index] = mostOfOneElement;
		}
		gmUnitCalcStub(unitId);
		for(i = 0; i < ELEMENT_COUNT; i++) {
			s32 gain = unit->element[i].power - before->element[i].power;

			if(gain > 0) {
				btPlaybackReset();
				btPlaybackSchedulePhase(25);
				btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
				btPlaybackPush(PLAYBACK_NUMBER, gain);
				btPlaybackPush(PLAYBACK_SOUND, SE_NOTE_HIGH);
				/* MsgEarthPowerUp and the next three, in ELEMENT order. */
				PUSH_MESSAGE(MsgEarthPowerUp + i);
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
				sySndPlayStub(SE_POWER_UP);
				fdActorSelectAnimationStub(btObjectGet(unitId)->actor,
										   POSE_CAST);
				fdActorSetAnimationSpeedStub(btObjectGet(unitId)->actor,
											 ANIM_STEP_NORMAL * 2);
				/* The pillar of each element whose power rose, its strength the
				 * most */
				/* Djinn of one element the summon used, less one. */
				btElementGlowPlay(unitId, i, GLOW_PILLAR, mostOfOneElement - 1);
				btPlaybackWaitForReady();
			}
		}
		syHeapRelease(before);
	}
	/* Damage at the action's end: the haunting spirit's, poison's, then the
	 * Grim */
	/* Reaper's countdown. */
	if(battleContinues != 0) {
		s8 *poison;
		btPlaybackReset();
		if(plan->hauntDamage != 0) {
			btPlaybackPush(PLAYBACK_HIT, unitId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_NUMBER, plan->hauntDamage);
			PUSH_MESSAGE(MsgCurseDamage);
			if(gmUnitAddHpStub(unitId, -plan->hauntDamage) == 0) {
				s32 message;
				btPlaybackPush(PLAYBACK_DEFEAT, unitId);
				btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
				if(unitId >= 0 && unitId <= ALLY_MAX) {
					message = MsgGoesDown;
				}
				else {
					message = MsgStrengthExhausted;
				}
				PUSH_MESSAGE(message);
			}
			else {
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
			}
		}
		btPlaybackRunAll();
		btPlaybackReset();
		poison = &unit->poison;
		/* A tenth of the maximum HP, a fifth for deadly poison (POISON_VENOM).
		 */
		if(*poison != 0) {
			s32 damage = unit->hpMax * *poison / 10;
			BATTLE_STATE *battle = BattleStateWork;
			btPlaybackPush(PLAYBACK_HIT, unitId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_NUMBER, damage);
			PUSH_MESSAGE(MsgPoisonDamage);
			/* DEAD: *poison is nonzero here, so the hit's sound is always */
			/* SE_HEAVY_IMPACT; the ROM keeps the reread of *poison, the compare
			 * and */
			/* both stores. */
			if(*poison != 0) {
				battle->playback.pendingSnd = SE_HEAVY_IMPACT;
			}
			else {
				battle->playback.pendingSnd = SE_IMPACT;
			}
			if(gmUnitAddHpStub(unitId, -damage) == 0) {
				s32 message;
				btPlaybackPush(PLAYBACK_DEFEAT, unitId);
				btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
				if(unitId >= 0 && unitId <= ALLY_MAX) {
					message = MsgGoesDown;
				}
				else {
					message = MsgStrengthExhausted;
				}
				PUSH_MESSAGE(message);
			}
			else {
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
			}
		}
		btPlaybackRunAll();
		btPlaybackReset();
		/* At 0, "The Grim Reaper calls out to ...!" and the unit falls. */
		if(unit->grimReaper != 0) {
			if(--unit->grimReaper == 0 &&
			   gmUnitAddHpStub(unitId, DEFEAT_HP_CHANGE) == 0) {
				btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
				PUSH_MESSAGE(MsgReaperCalls);
				btPlaybackPush(PLAYBACK_HIT, unitId);
				btPlaybackPush(PLAYBACK_DEFEAT, unitId);
				btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
				/* MsgGoesDown, "... goes down...", or MsgStrengthExhausted,
				 * "...'s */
				/* strength is exhausted...", counted from MsgReaperCalls. */
				if(unitId >= 0 && unitId <= ALLY_MAX) {
					PUSH_MESSAGE(MsgReaperCalls - 3);
				}
				else {
					PUSH_MESSAGE(MsgReaperCalls + 3);
				}
			}
		}
		btPlaybackRunAll();
	}
	gmUnitCalcStub(unitId);
}
