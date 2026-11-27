/* @file */
/* The end of a turn: each unit's modifiers and conditions counted down, the */
/* Djinn recovering, and the extras an action applies. */
/* Part of battle.c. */

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
	UNIT *unit = syUnitGet(unitId);

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
	UNIT *unit = syUnitGet(unitId);

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

/* Counts down a Defense change and returns 1 when it ends; a lowered Defense */
/* may recover early. */
s32 btUnitCountDownDefenseStage(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);

	if(unit->defenseTurns != 0) {
		unit->defenseTurns--;
		if(unit->defenseTurns == 0) {
			unit->defenseStage = 0;
			return 1;
		}
		if(unit->defenseStage < 0 &&
		   btUnitRollCounterEarlyEnd(unitId, unit->defenseTurns,
									 EARLY_END_DEFENSE_DOWN) != 0) {
			unit->defenseStage = 0;
			unit->defenseTurns = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down a Resistance change and returns 1 when it ends; a lowered */
/* Resistance may recover early. */
s32 btUnitCountDownResistanceStage(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);

	if(unit->resistanceTurns != 0) {
		unit->resistanceTurns--;
		if(unit->resistanceTurns == 0) {
			unit->resistanceStage = 0;
			return 1;
		}
		if(unit->resistanceStage < 0 &&
		   btUnitRollCounterEarlyEnd(unitId, unit->resistanceTurns,
									 EARLY_END_RESISTANCE_DOWN) != 0) {
			unit->resistanceStage = 0;
			unit->resistanceTurns = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down delusion and returns 1 when it ends, early or not. */
s32 btUnitCountDownDelusion(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);
	if(unit->delusion != 0) {
		unit->delusion--;
		if(unit->delusion == 0)
			return 1;
		if(btUnitRollCounterEarlyEnd(unitId, unit->delusion,
									 EARLY_END_DELUSION) != 0) {
			unit->delusion = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down confusion and returns 1 when it ends, early or not. */
s32 btUnitCountDownConfusion(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);
	if((u8)unit->confusion != 0) {
		(*(u8 *)&unit->confusion)--;
		if(unit->confusion == 0)
			return 1;
		if(btUnitRollCounterEarlyEnd(unitId, (u8)unit->confusion,
									 EARLY_END_CONFUSION) != 0) {
			unit->confusion = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down charm and returns 1 when it ends, early or not. */
s32 btUnitCountDownCharm(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);
	if(unit->charm != 0) {
		unit->charm--;
		if(unit->charm == 0)
			return 1;
		if(btUnitRollCounterEarlyEnd(unitId, unit->charm, EARLY_END_CHARM) !=
		   0) {
			unit->charm = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down a stun and returns 1 when it ends, early or not. */
s32 btUnitCountDownStun(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);
	if(unit->stun != 0) {
		unit->stun--;
		if(unit->stun == 0)
			return 1;
		if(btUnitRollCounterEarlyEnd(unitId, unit->stun, EARLY_END_STUN) != 0) {
			unit->stun = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down sleep and returns 1 when it ends, early or not. */
s32 btUnitCountDownSleep(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);
	if(unit->sleep != 0) {
		unit->sleep--;
		if(unit->sleep == 0)
			return 1;
		if(btUnitRollCounterEarlyEnd(unitId, unit->sleep, EARLY_END_SLEEP) !=
		   0) {
			unit->sleep = 0;
			return 1;
		}
	}
	return 0;
}

/* Counts down both seals in UNIT.psySeal and returns 1 when the last */
/* ends; only Bind's, once Luff's is gone, may end early. */
s32 btUnitCountDownPsynergySeal(s32 unitId) {
	UNIT *unit = syUnitGet(unitId);

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

/* While Regenerate lasts, heals the unit by a tenth of its max HP for each */
/* turn it has left and one more, shown in playback; then counts it down and */
/* returns 1 when it runs out. */
s32 btUnitCountDownRegenerate(s32 id) {
	UNIT *unit = syUnitGet(id);
	u8 *count = &unit->regenerate;

	if(*count != 0) {
		s32 max;
		s32 before;
		s32 hp;
		s32 gain;

		btPlaybackReset();
		hp = unit->hp;
		max = unit->hpMax;
		before = hp;
		hp += max * (*count + 1) / 10;
		if(hp > max)
			hp = max;
		gain = hp - before;
		if(gain != 0) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, id);
			if(hp == unit->hpMax) {
				btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgHpFull);
			}
			else {
				btPlaybackPush(PLAYBACK_NUMBER, gain);
				btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgHpRecover);
			}
			unit->hp = hp;
			gmUnitRecalculateGaugesStub(id);
		}
		btPlaybackRunAll();
		if(--unit->regenerate == 0)
			return 1;
	}
	return 0;
}

/* Counts down Reflect and returns 1 when it runs out; it never ends early. */
s32 btUnitCountDownReflect(s32 unitId) {
	u8 *turns = &syUnitGet(unitId)->reflect;
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
	UNIT *unit = syUnitGet(unitId);
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

		if(entry->turns > 0 && syUnitGet(entry->unitId)->hp != 0)
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

/* Does nothing; btRun calls it at the end of each turn and of the battle. */
void btTurnEndNoOp(void) {}

/* Counts down one side's Djinn recovery at the turn's end: an owner must be in
 */
/* the battle; each Djinni that comes back is shown before it is Set: its */
/* owner raises a hand and glows in its element to a high note, with "... is */
/* set to ...!" Returns 1 when a count fell, 2 when one was Set, else 0. */
s32 btDjinnRecoveryCountDownSide(s32 side) {
	DJINN_RECOVERY_LIST *list;
	DJINN_RECOVERY_ENTRY *timedEntry;
	DJINN_RECOVERY_ENTRY *entry;
	s32 result = 0;
	s32 i;
	s32 count;

	list = &gmDjinnGetRecoveryTableStub(side)->list;
	i = 0;
	if(i < list->cnt) {
		timedEntry = list->entries;
		do {
			if(timedEntry->turns > 0 && btUnitIsListed(timedEntry->unitId) &&
			   syUnitGet(timedEntry->unitId)->hp != 0) {
				timedEntry->turns--;
				result = 1;
			}
			count = list->cnt;

			i++;
			timedEntry++;
		} while(i < count);
	}
	i = 0;
	if(i < list->cnt) {
		entry = list->entries;
		do {
			if(entry->turns == 0) {
				u8 id = entry->unitId;

				if(btUnitIsListed(id)) {
					u8 element;

					result = 2;
					btPlaybackReset();
					btPlaybackSchedulePhase(30);
					btPlaybackPush(PLAYBACK_UNIT_NAME, id);
					btPlaybackPush(PLAYBACK_ABILITY_NAME,
								   entry->element * DJINN_PER_ELEMENT +
									   entry->idx + ABILITY_DJINNI_FIRST);
					btPlaybackPush(PLAYBACK_SOUND, SE_NOTE_HIGH);
					btPlaybackPush(PLAYBACK_DRAW_STATUS, 0);
					btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
					btPlaybackPush(PLAYBACK_REFRESH_UNIT, id);
					if(btUnitIsListed(id)) {
						fdActorSelectAnimationStub(
							(ACTOR *)btObjectGet(id)->obj, POSE_CAST);
						fdActorSetAnimationSpeedStub(
							(ACTOR *)btObjectGet(id)->obj,
							ANIM_STEP_NORMAL * 2);
					}
					element = entry->element;
					gmDjinnSetStub(id, element, entry->idx);
					btDjinnRecoveryRemoveStub(id, entry->element, entry->idx);
					gmUnitCalcStub(id);
					if(btUnitIsListed(id))
						btElementGlowPlay(id, element, GLOW_BURST, 0);
					btPlaybackWaitForReady();
				}
			}
			else {
				entry++;
				i++;
			}
		} while(i < list->cnt);
	}
	return result;
}

/* The Japanese text holds these messages in the same order two places later, */
/* but its labels for several of them belong to their neighbours, so the */
/* Japanese release reaches each of those by its place beside a message whose */
/* label fits its text. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* ねむりからさめた! ("wakes from slumber"), three before MsgResNormal. */
#define WAKES_MESSAGE ((s32)&MsgResNormal - 3)
/* すばやさが もどった! ("Agility returns to normal"), after MsgDefNormal. */
#define AGI_NORMAL_MESSAGE ((s32)&MsgDefNormal + 1)
/* われにかえった! ("regains consciousness"), two after MsgDefNormal. */
#define CONSCIOUS_MESSAGE ((s32)&MsgDefNormal + 2)
/* まぼろしを うちやぶった! ("sees clearly once again"), three before */
/* MsgCharmConscious. */
#define CURE_DELUSION_MESSAGE ((s32)&MsgCharmConscious - 3)
/* エナジーふうじがとけた! ("Psynergy seal is gone"), two before */
/* MsgCharmConscious. */
#define CURE_SEAL_MESSAGE ((s32)&MsgCharmConscious - 2)
/* しびれがきえた! ("no longer stunned"), the message before MsgCharmConscious.
 */
#define CURE_STUN_MESSAGE ((s32)&MsgCharmConscious - 1)
/* リフレクトのこうかがきえた! ("Reflect no longer affects"), after */
/* MsgRefrainEnd. */
#define REFLECT_END_MESSAGE ((s32)&MsgRefrainEnd + 1)
#else
#define WAKES_MESSAGE		  ((s32)&MsgWakes)
#define AGI_NORMAL_MESSAGE	  ((s32)&MsgAgiNormal)
#define CONSCIOUS_MESSAGE	  ((s32)&MsgConfusionConscious)
#define CURE_DELUSION_MESSAGE ((s32)&MsgCureDelusion)
#define CURE_SEAL_MESSAGE	  ((s32)&MsgCureSeal)
#define CURE_STUN_MESSAGE	  ((s32)&MsgCureStun)
#define REFLECT_END_MESSAGE	  ((s32)&MsgReflectEnd)
#endif

/* When `test` finds the unit's status over, shows its end, `message`, with the
 */
/* unit's name, and waits for the player. */
#define ANNOUNCE_END(test, message)                                            \
	if(test(unitId) != 0) {                                                    \
		btUnitBuildStatFlags(unitId, btObjectGet(unitId));                     \
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);             \
		mnBattleMessageShowAndWaitStub(message);                               \
		btMessageWaitForAdvance();                                             \
	}

/* As ANNOUNCE_END, turning the unit back to its usual pose before the message.
 */
#define ANNOUNCE_END_POSE(test, message)                                       \
	if(test(unitId) != 0) {                                                    \
		btUnitBuildStatFlags(unitId, btObjectGet(unitId));                     \
		mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);             \
		btObjectUpdatePose(unitId);                                            \
		mnBattleMessageShowAndWaitStub(message);                               \
		btMessageWaitForAdvance();                                             \
	}

/* The bosses whose regeneration is multiplied, the Serpents and Dullahan, */
/* regenerate BOSS_REGEN_MULTIPLE times their hpRegen each turn. */
#define BOSS_REGEN_MULTIPLE 10

/* Ends a turn: counts down the recovering Djinn and sets those recovered, then
 */
/* gives each unit its regeneration and counts down its stat changes and */
/* statuses, showing each that ends. */
void btProcessTurnEnd(void) {
	BATTLE_STATE *battle;
	s32 side;
	s32 sideCount;
	s32 count;
	s32 recoveryIndex;
	DJINN_RECOVERY_LIST *list;
	DJINN_RECOVERY_ENTRY *timedEntry;
	DJINN_RECOVERY_ENTRY *expiredEntry;

	battle = Ram_HeapSlots->battleState;
	sideCount = (battle->isLink != 0) + 1;
	for(side = 0; side < sideCount; side++) {
		list = &gmDjinnGetRecoveryTableStub(side)->list;
		recoveryIndex = 0;
		timedEntry = list->entries;
		if(recoveryIndex < list->cnt) {
			do {
				if(timedEntry->turns > 0 &&
				   btObjectGet(timedEntry->unitId) != 0 &&
				   syUnitGet(timedEntry->unitId)->hp != 0) {
					timedEntry->turns--;
				}
				recoveryIndex++;
				timedEntry++;
			} while(recoveryIndex < list->cnt);
		}
		recoveryIndex = 0;
		if(recoveryIndex < list->cnt) {
			expiredEntry = list->entries;
			do {
				if(expiredEntry->turns == 0) {
					u8 unitId = expiredEntry->unitId;
					u8 element;

					if(btObjectGet(unitId) != 0) {
						btPlaybackReset();
						btPlaybackSchedulePhase(30);
						btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
						btPlaybackPush(
							PLAYBACK_ABILITY_NAME,
							expiredEntry->element * DJINN_PER_ELEMENT +
								expiredEntry->idx + ABILITY_DJINNI_FIRST);
						btPlaybackPush(PLAYBACK_SOUND, SE_NOTE_HIGH);
						btPlaybackPush(PLAYBACK_DRAW_STATUS, 0);
						btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
						btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
						sySndPlayStub(SE_POWER_UP);
						if(btUnitIsListed(unitId)) {
							fdActorSelectAnimationStub(btObjectGet(unitId)->obj,
													   POSE_CAST);
							fdActorSetAnimationSpeedStub(
								(ACTOR *)btObjectGet(unitId)->obj,
								ANIM_STEP_NORMAL * 2);
						}
						element = expiredEntry->element;
						gmDjinnSetStub(unitId, element, expiredEntry->idx);
						btDjinnRecoveryRemoveStub(unitId, expiredEntry->element,
												  expiredEntry->idx);
						gmUnitCalcStub(unitId);
						if(btUnitIsListed(unitId))
							btElementGlowPlay(unitId, element, GLOW_BURST, 0);
						btPlaybackWaitForReady();
					}
				}
				else {
					expiredEntry++;
					recoveryIndex++;
				}
			} while(recoveryIndex < list->cnt);
		}
	}
	btBackgroundSetMode(BATTLE_BACKGROUND_ZOOMED,
						Ram_HeapSlots->battleState->bg, 0);
	{
		s32 *groupList;
		BATTLE_SIDE_ORDER order = Battle_TurnEndSideOrder;
		u16 unitIds[16];
		s32 group;
		s32 unitIndex;

		if(battle->isLink != 0 && battle->linkSide != 0) {
			order.group[0] = BATTLE_SIDE_MASK_ENEMIES;
			order.group[1] = BATTLE_SIDE_MASK_PARTY;
		}
		groupList = order.group;
		for(group = 0; group <= 1; group++) {
			count = btUnitListIds(groupList[group], unitIds);
			if(groupList[group] == BATTLE_SIDE_MASK_PARTY)
				count += btPartyPrepareReserveOwners(unitIds + count);
			for(unitIndex = 0; unitIndex < count; unitIndex++) {
				s32 hpAmount;
				s32 ppAmount;
				s32 unitId = unitIds[unitIndex];
				UNIT *unit = syUnitGet(unitId);

				/* A readiness to counterattack lasts the one turn. */
				unit->equipBonus[1] = 0;
				if(unit->ready != 0) {
					unit->ready--;
				}
				if(unit->hp != 0) {
					if(unit->hpRegen != 0 && unit->hp != unit->hpMax) {
						hpAmount = unit->hpRegen;
						switch(unit->enemyId) {
						case ENEMY_SERPENT:
						case ENEMY_SERPENT_ONE_BEAM:
						case ENEMY_SERPENT_TWO_BEAMS:
						case ENEMY_SERPENT_THREE_BEAMS:
						case ENEMY_SERPENT_FOUR_BEAMS:
						case ENEMY_SERPENT_6:
						case ENEMY_SERPENT_7:
						case ENEMY_DULLAHAN:
							hpAmount *= BOSS_REGEN_MULTIPLE;
							break;
						}
						if(unit->hp + hpAmount > unit->hpMax) {
							hpAmount = unit->hpMax - unit->hp;
						}
						gmUnitAddHpStub(unitId, hpAmount);
						mnMessageArgumentPushStub(unitId,
												  MESSAGE_ARGUMENT_ACTOR);
						mnMessageArgumentPushStub(hpAmount,
												  MESSAGE_ARGUMENT_NUMBER);
						if(unit->hp == unit->hpMax) {
							mnBattleMessageShowAndWaitStub((s32)&MsgHpFull);
						}
						else {
							mnBattleMessageShowAndWaitStub((s32)&MsgHpRecover);
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
							mnBattleMessageShowAndWaitStub((s32)&MsgPpFull);
						}
						else {
							mnBattleMessageShowAndWaitStub((s32)&MsgPpRecover);
						}
						sySndPlayStub(SE_NOTE_HIGH);
						btMessageWaitForAdvance();
					}
				}
				ANNOUNCE_END(btUnitCountDownAgilityStage, AGI_NORMAL_MESSAGE);
				ANNOUNCE_END(btUnitCountDownAttackStage, (s32)&MsgAtkNormal);
				ANNOUNCE_END(btUnitCountDownDefenseStage, (s32)&MsgDefNormal);
				ANNOUNCE_END(btUnitCountDownResistanceStage,
							 (s32)&MsgResNormal);
				ANNOUNCE_END(btUnitCountDownDelusion, CURE_DELUSION_MESSAGE);
				ANNOUNCE_END(btUnitCountDownConfusion, CONSCIOUS_MESSAGE);
				ANNOUNCE_END(btUnitCountDownCharm, (s32)&MsgCharmConscious);
				ANNOUNCE_END_POSE(btUnitCountDownStun, CURE_STUN_MESSAGE);
				ANNOUNCE_END_POSE(btUnitCountDownSleep, WAKES_MESSAGE);
				ANNOUNCE_END(btUnitCountDownPsynergySeal, CURE_SEAL_MESSAGE);
				ANNOUNCE_END(btUnitCountDownRegenerate, (s32)&MsgRefrainEnd);
				ANNOUNCE_END(btUnitCountDownReflect, REFLECT_END_MESSAGE);
			}
		}
	}
}

/* The most Djinn btUnitDrainDjinn gathers to choose from. */
#define DRAIN_CANDIDATE_MAX 10

/* Djinnfest, Fugue, Djinn Blast, Djinn Storm and Djinn Stun: puts one of the */
/* unit's set Djinn at random on standby (every one, when all is set), or, when
 */
/* none is set, drains one that could be set so it must recover. Djinn Stun's */
/* announce shows MsgDjinnOnStandby; the others show MsgDjinnDrained or */
/* MsgAllDjinnDrained. */
void btUnitDrainDjinn(s32 unitId, s32 all, s32 announce) {
	u8 elements[DRAIN_CANDIDATE_MAX];
	u8 indices[DRAIN_CANDIDATE_MAX];
	s32 shown;
	s32 count;
	s32 active;
	s32 element;
	s32 djinni;
	s32 i;
	s32 owner;
	s32 recoveryTurn;
	DJINN_RECOVERY_LIST *list;
	DJINN_RECOVERY_LIST *pending;

	shown = 0;
again: {
	count = 0;
	active = 0;
	for(element = 0; element < ELEMENT_COUNT; element++) {
		for(djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
			if(gmDjinnIsSetStub(unitId, element, djinni)) {
				elements[count] = element;
				indices[count] = djinni;
				count++;
				active = 1;
				if(count == DRAIN_CANDIDATE_MAX)
					goto picked;
			}
		}
	}
	if(count == 0) {
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinni = 0; djinni < DJINN_PER_ELEMENT; djinni++) {
				if(gmDjinnCanSetStub(unitId, element, djinni)) {
					elements[count] = element;
					indices[count] = djinni;
					count++;
					if(count == DRAIN_CANDIDATE_MAX)
						goto picked;
				}
			}
		}
	}
picked:
	if(count == 0)
		return;

	count = ((u32)btRandomStub() * count) >> 16;
	element = elements[count];
	djinni = indices[count];
	if(active) {
		btDjinnRecoveryAddStandbyStub(unitId, element, djinni);
		gmDjinnPutOnStandbyStub(unitId, element, djinni);
		if(announce) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_ABILITY_NAME, element * DJINN_PER_ELEMENT +
													  djinni +
													  ABILITY_DJINNI_FIRST);
			btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgDjinnOnStandby);
			return;
		}
	}
	else if(announce) {
		return;
	}

	list = &gmDjinnGetRecoveryTableStub(unitId < 0 || unitId > ALLY_MAX)->list;
	for(i = 0; i < list->cnt; i++) {
		if(list->entries[i].element == element &&
		   list->entries[i].idx == djinni &&
		   list->entries[i].turns == DJINN_RECOVERY_AFTER_BATTLE)
			list->entries[i].turns = DJINN_RECOVERY_SUMMONED;
	}

	pending =
		&gmDjinnGetRecoveryTableStub(unitId < 0 || unitId > ALLY_MAX)->list;
	for(;;) {
		owner = -1;
		for(i = 0; i < pending->cnt; i++) {
			if(pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
				owner = pending->entries[i].unitId;
				break;
			}
		}
		if(owner == -1)
			break;
		recoveryTurn = -1;
		for(i = 0; i < pending->cnt; i++) {
			if(pending->entries[i].unitId == owner &&
			   pending->entries[i].turns > recoveryTurn)
				recoveryTurn = pending->entries[i].turns;
		}
		recoveryTurn++;
		if(recoveryTurn <= 1)
			recoveryTurn = 2;
		for(i = 0; i < pending->cnt; i++) {
			if(pending->entries[i].unitId == owner &&
			   pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
				pending->entries[i].turns = recoveryTurn;
				recoveryTurn++;
			}
		}
	}

	if(!shown) {
		shown = 1;
		btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
		if(!all) {
			s32 message;

			btPlaybackPush(PLAYBACK_ABILITY_NAME, element * DJINN_PER_ELEMENT +
													  djinni +
													  ABILITY_DJINNI_FIRST);
			message = (u32)&MsgDjinnDrained;
			if(unitId < 0 || unitId > ALLY_MAX)
				message++;
			btPlaybackPush(PLAYBACK_MESSAGE, message);
		}
		else {
			btPlaybackPush(PLAYBACK_MESSAGE, (u32)&MsgAllDjinnDrained);
		}
	}
	if(all)
		goto again;
}
}

/* Ends one unit's action: its Djinn on standby are taken off its set, the */
/* Djinn a summon used begin to recover, and the unit takes its summon's power
 */
/* boost, the haunt's damage, poison and the Grim Reaper. */
void btUnitProcessActionEnd(BATTLE_PLAN *plan) {
	u32 unitId;
	UNIT *unit;
	s32 battleContinues;
	s32 counts[ELEMENT_COUNT];
	DJINN_RECOVERY_LIST *list;
	s32 entryIndex;
	s32 recoveryTurn;

	unitId = plan->unitId;
	battleContinues = 0;
	unit = syUnitGet(unitId);
	if(unitId <= ALLY_MAX ||
	   ((BATTLE_STATE *)Ram_HeapSlots->battleState)->isLink != 0) {
		list = &gmDjinnGetRecoveryTableStub(unitId > ALLY_MAX)->list;
		for(entryIndex = 0; entryIndex < list->cnt; entryIndex++) {
			if(list->entries[entryIndex].unitId == unitId &&
			   list->entries[entryIndex].turns == DJINN_RECOVERY_AFTER_BATTLE) {
				gmDjinnPutOnStandbyStub(unitId,
										list->entries[entryIndex].element,
										list->entries[entryIndex].idx);
			}
		}
	}
	if(btUnitListLiving(BATTLE_SIDE_MASK_PARTY, 0) != 0 &&
	   btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) != 0) {
		battleContinues = 1;
	}
	{
		s32 i;
		s32 owner;
		DJINN_RECOVERY_LIST *pending;

		pending =
			&gmDjinnGetRecoveryTableStub(
				 unitId > ALLY_MAX &&
				 ((BATTLE_STATE *)Ram_HeapSlots->battleState)->isLink != 0)
				 ->list;
		for(i = 0; i < ELEMENT_COUNT; i++) {
			counts[i] = 0;
		}
		for(;;) {
			owner = -1;
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
					owner = pending->entries[i].unitId;
					break;
				}
			}
			if(owner == -1) {
				break;
			}
			recoveryTurn = -1;
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].unitId == owner &&
				   pending->entries[i].turns > recoveryTurn) {
					recoveryTurn = pending->entries[i].turns;
				}
			}
			recoveryTurn++;
			if(recoveryTurn <= 1) {
				recoveryTurn = 2;
			}
			for(i = 0; i < pending->cnt; i++) {
				if(pending->entries[i].unitId == owner &&
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
		s32 i;

		before = syHeapAllocEwram(sizeof(UNIT));
		syWordsCopyEntry(before, unit, sizeof(UNIT));
		for(i = 0; i < ELEMENT_COUNT; i++) {
			/* A summon's boost to an element is how many of its Djinn the
			 * summons */
			/* used, four at most, and never falls during the battle. */
			if(counts[i] > 4)
				counts[i] = 4;
			if(unit->summonBoost[i] < counts[i])
				unit->summonBoost[i] = counts[i];
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
				/* "...'s Earth Power rises by ...!", then Water, Fire and Wind.
				 */
				btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgEarthPowerUp + i);
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
				sySndPlayStub(SE_POWER_UP);
				fdActorSelectAnimationStub(btObjectGet(unitId)->obj, POSE_CAST);
				fdActorSetAnimationSpeedStub((ACTOR *)btObjectGet(unitId)->obj,
											 ANIM_STEP_NORMAL * 2);
				btElementGlowPlay(unitId, i, GLOW_PILLAR, -1);
				btPlaybackWaitForReady();
			}
		}
		syHeapRelease(before);
	}
	if(battleContinues != 0 && btObjectGet(unitId)->obj != 0) {
		s8 *poison;
		u8 *reaper;

		btPlaybackReset();
		if(plan->hauntDamage != 0) {
			btPlaybackPush(PLAYBACK_HIT, unitId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_NUMBER, plan->hauntDamage);
			btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgCurseDamage);
			if(gmUnitAddHpStub(unitId, -plan->hauntDamage) == 0) {
				if(btUnitKeepsOneHp(unitId)) {
					gmUnitAddHpStub(unitId, 1);
					btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
				}
				else {
					btPlaybackPush(PLAYBACK_DEFEAT, unitId);
					btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
					btPlaybackPush(PLAYBACK_MESSAGE,
								   unitId <= ALLY_MAX
									   ? (u32)MsgGoesDown
									   : (u32)MsgStrengthExhausted);
				}
			}
			else {
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
			}
		}
		btPlaybackRunAll();
		btPlaybackReset();
		poison = &unit->poison;
		if(*poison != 0) {
			s32 damage = unit->hpMax * *poison / 10;
			BATTLE_STATE *battle = Ram_HeapSlots->battleState;

			btPlaybackPush(PLAYBACK_HIT, unitId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_NUMBER, damage);
			btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgPoisonDamage);
			/* DEAD: *poison is nonzero here, so the hit's sound is always */
			/* SE_HEAVY_IMPACT; the ROM keeps the reread of *poison, the compare
			 * and */
			/* both stores. */
			if(*poison != 0) {
				battle->events.pendingSnd = SE_HEAVY_IMPACT;
			}
			else {
				battle->events.pendingSnd = SE_IMPACT;
			}
			if(gmUnitAddHpStub(unitId, -damage) == 0) {
				if(btUnitKeepsOneHp(unitId)) {
					gmUnitAddHpStub(unitId, 1);
					btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
				}
				else {
					btPlaybackPush(PLAYBACK_DEFEAT, unitId);
					btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
					btPlaybackPush(PLAYBACK_MESSAGE,
								   unitId <= ALLY_MAX
									   ? (u32)MsgGoesDown
									   : (u32)MsgStrengthExhausted);
				}
			}
			else {
				btPlaybackPush(PLAYBACK_REFRESH_UNIT, unitId);
			}
		}
		btPlaybackRunAll();
		btPlaybackReset();
		/* At 0 the Reaper takes far more HP than any unit has. */
		reaper = &unit->grimReaper;
		if(*reaper != 0 && !btUnitKeepsOneHp(unitId) && --*reaper == 0 &&
		   gmUnitAddHpStub(unitId, DEFEAT_HP_CHANGE) == 0) {
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgReaperCalls);
			btPlaybackPush(PLAYBACK_HIT, unitId);
			btPlaybackPush(PLAYBACK_DEFEAT, unitId);
			btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
			/* MsgGoesDown, "... goes down...", or MsgStrengthExhausted, "...'s
			 */
			/* strength is exhausted...", counted from MsgReaperCalls as the ROM
			 */
			/* counts them. */
			if(unitId <= ALLY_MAX) {
				btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgReaperCalls - 3);
			}
			else {
				btPlaybackPush(PLAYBACK_MESSAGE, (u32)MsgReaperCalls + 3);
			}
		}
		btPlaybackRunAll();
	}
	if(btObjectGet(unitId)->obj != 0)
		gmUnitCalcStub(unitId);
}
