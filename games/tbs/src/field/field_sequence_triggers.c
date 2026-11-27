/* @file */
/* Finding and running a scene's triggers: talking, step triggers, examine */
/* triggers. */
/* Part of field_sequence.c. */

#include "field_sequence_internal.h"

/* What an examine trigger is on (EXAMINE), the control bits 8..4 */
/* EXAMINE_TRIGGER() packs it into. */
#define EXAMINE_TARGET(kind) (((kind) >> 4) & 0x1f)
/* The item a FIND_ITEM search result holds. */
#define FIND_ITEM_MASK 0xfff
/* The PP a Psynergy Stone gives each party member, enough to fill it: "The */
/* party's PP is fully restored!" */
#define PSYNERGY_STONE_PP 999

/* Returns the actor placement of actor id in the scene's */
/* actor placement tables, or NULL: for a party character (0 to 7) the row with
 */
/* its sprite, and for a placed actor (8 on) the row it was created */
/* from, counting the placed rows in order. */
ACTOR_PLACE *scFindActorPlacement(s32 id) {
	ACTOR_PLACE *placement;
	s32 tableIndex;
	s32 placedActorId = ACTOR_FIRST_PLACED;

	for(tableIndex = 0; tableIndex < (s32)ARRAY_COUNT(FieldStateWork->tbls);
		tableIndex++) {
		placement = FieldStateWork->tbls[tableIndex];
		if(placement == NULL)
			continue;
		if(id < ACTOR_FIRST_PLACED) {
			for(; (s16)placement->spr != -1; placement++) {
				if((s16)placement->spr == id) {
#ifdef UBFIX
					return placement;
#else
					goto found;
#endif
				}
			}
		}
		else {
			for(; (s16)placement->spr != -1; placement++) {
				if((s16)placement->spr >= ACTOR_FIRST_PLACED) {
					if(placedActorId == id) {
#ifdef UBFIX
						return placement;
#else
						goto found;
#endif
					}
					placedActorId++;
				}
			}
		}
	}
#ifdef UBFIX
	return NULL;
#else
found:
	/* UB: with the last table absent and no row matched, placement is NULL here
	 */
	/* and is read all the same. */
	if((s16)placement->spr == -1)
		return NULL;
	return placement;
#endif
}

/* Returns whether an event condition holds: -1 always holds; a condition with
 */
/* CONDITION_FLAG_SET holds while that flag is set, any other while its flag is
 */
/* clear. */
s32 fdGameFlagIsConditionActive(s32 condition) {
	s32 flagSet;

	if(condition == -1) {
		return 1;
	}
	if(CONDITION_FLAG_SET & condition) {
		return gmFlagGetStub(condition);
	}
	flagSet = gmFlagGetStub(condition);
	return flagSet == 0;
}

/* Returns 1 for an examine trigger (TRIGGER_EXAMINE) that holds whatever its */
/* condition: one with more flags than its kind, unless its result is a mimic */
/* battle (FIND_MIMIC). */
u32 scExamineTriggerIgnoresCondition(s32 flags, s32 result) {
	if((flags & TRIGGER_KIND_MASK) != TRIGGER_EXAMINE)
		return 0;
	if((flags & EXAMINE_TRIGGER_MASK) == TRIGGER_EXAMINE)
		return 0;
	if((result & FIND_TYPE_MASK) != FIND_MIMIC)
		return 1;
	return 0;
}

/* Returns the first of the scene's triggers of kind for target whose condition
 */
/* holds, or NULL. */
const TRIGGER *scFindTrigger(s32 kind, s32 target) {
	FIELD_STATE *state = FieldStateWork;
	const TRIGGER *trigger = state->triggers;
	s32 flags;
	u32 facing =
		((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;

	flags = trigger->kind;
	while(flags != -1) {
		/* DEAD: flags already holds trigger->kind; the ROM keeps the second
		 * load of */
		/* it once the condition holds. */
		if((flags & TRIGGER_KIND_MASK) == kind &&
		   (u8)trigger->subject == target &&
		   (scExamineTriggerIgnoresCondition(flags, trigger->value) != 0 ||
			(fdGameFlagIsConditionActive(trigger->condition) != 0 &&
			 (flags = trigger->kind, 1)))) {
			s32 matches = 0;
			s32 minimumPushFrames = FIELD_PUSH_FRAMES;
			s32 touchRequirements;

			if((flags & STEP_TRIGGER_FLAG_QUICK) != 0)
				minimumPushFrames = 2;
			touchRequirements =
				flags & (STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING);

			switch(touchRequirements) {
			case 0:
				matches = 1;
				break;
			case STEP_TRIGGER_FLAG_WALKING:
				if(state->blockedSteps > minimumPushFrames) {
					matches = 1;
				}
				break;
			case TRIGGER_FLAG_FACING:
				/* Bits 15 to 12 are a facing in sixteenths of a turn; the
				 * leader's */
				/* facing must be within an eighth of a turn of it. */
				flags &= TRIGGER_FACING_MASK;
				matches = (u16)((flags - facing) + (DEGtoANG(45) - 1)) <=
						  DEGtoANG(90) - 2;
				break;
			case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
				if(state->blockedSteps > minimumPushFrames) {
					flags &= TRIGGER_FACING_MASK;
					matches = (u16)((flags - facing) + (DEGtoANG(45) - 1)) <=
							  DEGtoANG(90) - 2;
				}
				break;
			}
			if(matches) {
				return trigger;
			}
		}
		trigger++;
		if(trigger->kind == -1)
			break;
		flags = trigger->kind;
	}
	return 0;
}

/* Unused: runs the loaded overlay's initialize entry. */
UNUSED void fdOverlayCallInit(void) { OverlayArea.initialize(); }

/* Returns the talk trigger of actor id, or 0: for the actor stopped with Halt
 */
/* (MainRoutineStateWork.haltedActor), its TRIGGER_TALK_HALTED event if it has
 */
/* one, the event for talking to it while it is stopped (Crossbone Isle's Bane
 */
/* has one). */
const TRIGGER *scFindActorTalkTrigger(s32 id) {
	const TRIGGER *talkTrigger = scFindTrigger(TRIGGER_TALK, id);
	s32 haltedActorId = MainRoutineStateWork.haltedActor;

	if(haltedActorId == id) {
		const TRIGGER *haltedTrigger =
			scFindTrigger(TRIGGER_TALK_HALTED, haltedActorId);

		if(haltedTrigger != 0) {
			return haltedTrigger;
		}
	}
	return talkTrigger;
}

/* Talks to actor id: shows its event's message or calls its event function, */
/* with the actor stopped and turned as its placement says. The actor Halt has
 */
/* stopped answers its TRIGGER_TALK_HALTED event if it has one; else it says */
/* its talk message in halting speech, or, in place of a talk function, one of
 */
/* its sprite's two Halted lines. */
s32 scTalkToActor(s32 id) {
	ACTOR_PLACE *placement = scFindActorPlacement(id);
	s32 result = -1;
	ACTOR *actor = fdActorGetById(id);
	const TRIGGER *trigger;
	s32 actorHalted = 0;
	u32 savedAngle;
	s32 decodeFlags = 0;

	if(MainRoutineStateWork.haltedActor == id) {
		actorHalted = 1;
		trigger = scFindTrigger(TRIGGER_TALK_HALTED, id);
		if(trigger == 0) {
			trigger = scFindTrigger(TRIGGER_TALK, id);
			decodeFlags = MESSAGE_DECODE_HALTING_SPEECH;
			if(trigger == 0) {
				return -1;
			}
			/* A function talk trigger is not run for a stopped actor: it says
			 */
			/* one of the two lines of its sprite's Halted pair, at random. */
			if(trigger->value >= TRIGGER_RESULT_MESSAGE_END) {
				s32 haltedLines = fdSpeakerGetHaltedLines(id);
				u32 random = syRandom();
				s32 message =
					MsgPonderEllipsis + haltedLines * 2 + (random * 2 >> 16);
				fdEventStart();
				fdEventSetMessageId(message);
				fdEventShowMessageAndWait(id, 0);
				fdEventEnd();
				goto finish;
			}
		}
	}
	else {
		trigger = scFindTrigger(TRIGGER_TALK, id);
	}

	if(trigger != 0 && trigger->value != 0) {
		if(!actorHalted) {
			/* Stop the actor; with any talk facing but TALK_FACING_KEEP, also
			 * stop */
			/* the leader and make each watch the other. */
			actor->paused = 1;
			fdActorSetAnimationSpeedStub(actor, 0);
			savedAngle = actor->facing;
			if((u8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK ||
			   (s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
				ACTOR *leader =
					fdActorGetById(MainRoutineStateWork.leaderActorId);

				leader->tgtX = leader->x.fixed;
				leader->tgtY = leader->y.fixed;
				leader->tgtZ = leader->z.fixed;
				SET_VELOCITY(leader, 0, 0, 0);
				fdActorTurnToFaceEachOther(
					id, MainRoutineStateWork.leaderActorId, 0);
			}
		}
		/* A result below TRIGGER_RESULT_MESSAGE_END is a message; above, a
		 * function */
		/* to call. */
		if(trigger->value < TRIGGER_RESULT_MESSAGE_END) {
			mnMessageSetDecodeFlagsStub(decodeFlags);
			fdEventStart();
			fdEventSetMessageId(trigger->value);
			fdEventShowMessageAndWait(id, 0);
			fdEventEnd();
		}
		else {
			fdLeaderStopMoving();
			((void (*)(s32))trigger->value)(id);
		}
		if(!actorHalted) {
			/* An actor whose script has ended goes back to what its talk facing
			 */
			/* says: watching the leader or its old angle. */
			if(actor->script[actor->scriptCursor] == SCRIPT_COMMAND_END) {
				if((s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
					actor->watchedActor =
						(union ACTOR_OR_EFFECT *)fdActorGetById(
							MainRoutineStateWork.leaderActorId);
					actor->steeringFlg |= ACTOR_FACES_MOTION;
					fdActorSetMovementType(actor, (s32)Actor_FaceWatchedScript);
				}
				else if((s8)placement->talkFacing ==
						TALK_FACING_PARTY_AND_BACK) {
					actor->tmr = savedAngle;
					fdActorStartScriptStub(actor,
										   (const s32 *)Actor_StepAngleScript);
				}
			}
			actor->paused = 0;
			fdActorSetAnimationSpeedStub(actor, ANIM_STEP_NORMAL);
		}
		result = 0;
	}
finish:
	if(actorHalted) {
		fdPsynergyResumeHaltedActor(MainRoutineStateWork.haltedActor);
		MainRoutineStateWork.haltedActor = -1;
	}
	return result;
}

/* Runs the scene's raised trigger (TRIGGER_RAISED) for value: its message or */
/* its function. Returns 0 when an event ran, -1 when there was none. */
s32 scRunRaisedTrigger(s32 value) {
	s32 result;
	s32 handled;
	const TRIGGER *trigger;

	trigger = scFindTrigger(TRIGGER_RAISED, value);
	handled = -1;
	if(trigger != NULL) {
		result = trigger->value;
		if(result != 0) {
			if(result < TRIGGER_RESULT_MESSAGE_END) {
				fdEventStart();
				fdEventSetMessageId(trigger->value);
				fdEventShowMessageAndWait(-1, 0);
				handled = 0;
				fdEventEnd();
			}
			else {
				((void (*)(s32))result)(value);
				handled = 0;
			}
		}
	}
	return handled;
}

/* Runs the step trigger (TRIGGER_STEP) of what the leader touched, or else */
/* takes its exit (TRIGGER_EXIT): plays the exit's sound (and effect) and */
/* records where it leads. */
s32 scRunStepTriggerOrExit(s32 value) {
	const TRIGGER *touch;
	const TRIGGER *exit;
	s32 exitId;
	s32 handled;
	FIELD_STATE *state;

	touch = scFindTrigger(TRIGGER_STEP, value);
	handled = -1;
	state = FieldStateWork;
	if((touch != 0) && (touch->value != 0)) {
		if(touch->kind & STEP_TRIGGER_FLAG_WALKING) {
			state->blockedSteps = 0;
		}
		if(touch->value < TRIGGER_RESULT_MESSAGE_END) {
			fdEventStart();
			fdEventSetMessageId(touch->value);
			fdEventShowMessageAndWait(-1, 0);
			handled = 0;
			fdEventEnd();
		}
		else {
			((void (*)(s32))touch->value)(value);
			handled = 0;
		}
	}
	else {
		exit = scFindTrigger(TRIGGER_EXIT, value);
		if(exit != 0) {
			/* An EXIT_STYLE chooses how the leader leaves; the animated
			 * styles' */
			/* flags make the next scene play the matching arrival. */
			exitId = exit->kind & EXIT_STYLE_MASK;
			switch(exitId) {
			case EXIT_STYLE_NORMAL:
				sySndPlayStub(SE_MAP_EXIT);
				break;
			case EXIT_STYLE_STAIRS_UP:
				sySndPlayStub(SE_STAIRS_UP);
				fdExitStartStairsUp();
				break;
			case EXIT_STYLE_STAIRS_DOWN:
				sySndPlayStub(SE_STAIRS_DOWN);
				fdExitStartStairsDown();
				break;
			}
			state->exit = exit->value;
			handled = 0;
		}
	}
	return handled;
}

/* Runs the event of a cell the leader pushes against (CELL_BEHAVIOR_LEDGE to */
/* CELL_BEHAVIOR_JUMP_FORWARD, and CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED), once
 */
/* it has pushed for more than FIELD_PUSH_FRAMES frames; the count then */
/* restarts. */
s32 fdRunPushCell(s32 behavior) {
	FIELD_STATE *state = FieldStateWork;
	s32 leader = MainRoutineStateWork.leaderActorId;

	switch(behavior) {
	case CELL_BEHAVIOR_LEDGE:
		if(state->blockedSteps > FIELD_PUSH_FRAMES && (KeysHeld & KEY_DOWN)) {
			fdActorJumpDownLedge(leader, ANIM_LEADER_SPRING, 0);
			state->blockedSteps = 0;
		}
		break;
	case CELL_BEHAVIOR_JUMP_FORWARD_UNREACHED:
	case CELL_BEHAVIOR_JUMP_FORWARD:
		if(state->blockedSteps > FIELD_PUSH_FRAMES) {
			fdTryJumpForward();
			state->blockedSteps = 0;
		}
		break;
	case CELL_BEHAVIOR_CLIMB:
		if(state->blockedSteps > FIELD_PUSH_FRAMES) {
			if(KeysHeld & KEY_DOWN) {
				fdClimbDown();
			}
			else if(KeysHeld & KEY_UP) {
				fdClimbUp();
			}
			state->blockedSteps = 0;
		}
		break;
	}
	return 0;
}

/* Searches the cell ahead with the given cell behavior. The fixed search */
/* spots, CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL, show their two messages. */
s32 fdExamineCell(s32 tile) {
	const TRIGGER *trigger;
	s32 leader;
	u32 messageOffset;
	s16 condition;
	u32 searchResult;
	s32 rewardType;
	s32 canOpen;

	leader = MainRoutineStateWork.leaderActorId;
	/* Each fixed spot has its own pair of lines, chosen through */
	/* Field_ExamineTriggerMessageOffsets; the unsigned compare also rejects */
	/* codes below CELL_BEHAVIOR_JAR. */
	messageOffset = tile - CELL_BEHAVIOR_JAR;
	if(messageOffset <= CELL_BEHAVIOR_WELL - CELL_BEHAVIOR_JAR) {
		fdLeaderStopMoving();
		tile = Field_ExamineTriggerMessageOffsets[messageOffset];
		mnFieldMessageShowStub(MsgTileChecked + tile, FIELD_MESSAGE_ZOOM);
		mnFieldMessageShowStub(MsgTileResult + tile, FIELD_MESSAGE_ZOOM);
	}
	else {
		trigger = scFindTrigger(TRIGGER_EXAMINE, tile);
		if(trigger != NULL) {
			s32 flags = trigger->kind;

			/* Bits 8 to 4, the search target (EXAMINE), choose what
			 * the */
			/* leader checks ("checked the chest...") unless */
			/* TRIGGER_SUBJECT_OWN_EFFECT says the event has its own message; if
			 * the */
			/* event leaves FLAG_NOTHING_HAPPENS set, the matching result line
			 * follows */
			/* ("but the chest was empty."). */
			condition = trigger->condition;
			messageOffset = EXAMINE_TARGET(flags);
			if((trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT) == 0 &&
			   messageOffset != 0) {
				fdLeaderStopMoving();
				mnFieldMessageShowStub(MsgTileChecked + messageOffset,
									   FIELD_MESSAGE_ZOOM);
				gmFlagSetStub(FLAG_NOTHING_HAPPENS);
			}
			else {
				gmFlagClearStub(FLAG_NOTHING_HAPPENS);
			}
			/* The value is a function address (bits 27 to 24 set) or a FIND_
			 * value */
			/* whose top 12 bits give its type: an item, coins, a Psynergy
			 * Stone, a */
			/* message or a mimic. */
			searchResult = trigger->value;
			if((searchResult & FIND_FUNCTION_BITS) != 0 ||
			   (searchResult & FIND_TYPE_MASK) == FIND_MESSAGE(0)) {
				if((searchResult & FIND_FUNCTION_BITS) != 0) {
					/* A function. */
					if(fdGameFlagIsConditionActive(condition) != 0)
						((void (*)(s32))trigger->value)(
							MainRoutineStateWork.leaderActorId);
					if(gmFlagGetStub(FLAG_NOTHING_HAPPENS) != 0)
						mnFieldMessageShowStub(MsgTileResult + messageOffset,
											   FIELD_MESSAGE_ZOOM);
				}
				else {
					if(fdGameFlagIsConditionActive(condition) != 0)
						mnFieldMessageShowStub((u16)trigger->value,
											   FIELD_MESSAGE_ZOOM);
					else
						/* "but found nothing." */
						mnFieldMessageShowStub(MsgButFoundNothing,
											   FIELD_MESSAGE_ZOOM);
				}
			}
			else {
				/* A chest; its condition flag is set once it has been opened.
				 */
				fdEventStart();
				if(fdGameFlagIsConditionActive(condition) != 0) {
					/* FIND_CHEST_LOCKED marks a chest the party's own
					 * characters */
					/* cannot open ("Can't open it"). */
					searchResult = trigger->value;
					canOpen = 1;
					if((searchResult & FIND_CHEST_LOCK_MASK) ==
						   FIND_CHEST_LOCKED &&
					   leader <= ALLY_MAX)
						canOpen = 0;
					if(canOpen != 0) {
						/* A EXAMINE_TRIGGER(EXAMINE_CHEST) event is a chest the
						 * map shows */
						/* as a treasure spot. */
						if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
						   EXAMINE_TRIGGER(EXAMINE_CHEST))
							fdTreasureSpotOpenChest(tile);
						rewardType = trigger->value & FIND_TYPE_MASK;
						if(rewardType == FIND_PSYNERGY_STONE) {
							/* A Psynergy Stone: restores all PP. */
							void *object;

							if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
							   EXAMINE_TRIGGER(EXAMINE_CHEST))
								fdTreasureSpotShowOpened(tile);
							object = fdTreasureSpotGetActor(tile);
							fdItemDispRaiseAboveLeader(object);
							sySndPlayStub(MUS_TREASURE_FOUND);
							mnMessageArgumentPushStub((u16)trigger->value,
													  MESSAGE_ARGUMENT_NUMBER);
							/* "Isaac got a Psynergy Stone." */
							mnFieldMessageShowStub(
								MsgGotPsynergyStone,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
							fdPartyAddPp(PSYNERGY_STONE_PP, 0);
							mnPartyStatCreateWindowStub(PARTY_STATUS_SHOW_PP);
							sySndPlayStub(SE_RECOVERY);
							/* "The party's PP is fully restored!" */
							mnFieldMessageShowStub(MsgGotPsynergyStone + 1,
												   FIELD_MESSAGE_ZOOM);
							mnPartyStatCloseStub();
							fdActorSelectAnimationStub(object, 2);
							sySndPlayStub(SE_GLITTER);
							fdEventWait(30);
							/* "The Psynergy Stone disappeared..." */
							mnFieldMessageShowStub(MsgGotPsynergyStone + 2,
												   FIELD_MESSAGE_ZOOM);
							fdTreasureSpotHide(tile);
							if(condition != -1)
								gmFlagSetStub(condition);
						}
						else if(rewardType == FIND_MIMIC) {
							/* A mimic: a battle with its slot of
							 * ENCOUNTER_ZONE_MIMIC. */
							FIELD_STATE *state = FieldStateWork;

							if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
							   EXAMINE_TRIGGER(EXAMINE_CHEST))
								fdTreasureSpotLaunchMimic(tile);
							/* The chest's flag (with CONDITION_FLAG_SET) waits
							 * in */
							/* pendingDjinni, which fdSequenceRun resolves when
							 * the scene */
							/* is entered again after the battle. */
							if(condition != -1) {
								MainRoutineStateWork.pendingDjinni =
									condition | CONDITION_FLAG_SET;
							}
							/* BATTLE_TYPE_NO_ESCAPE (battle/battle_turn.c): no
							 * first strike */
							/* or surprise, and every attempt to run fails; the
							 * command menu */
							/* still offers it, refusing outright only for */
							/* BATTLE_TYPE_SCRIPTED and BATTLE_TYPE_LINK. */
							state->encounter = fdEncounterGetFormationAtSlot(
								ENCOUNTER_ZONE_MIMIC, (u16)trigger->value);
							btSetType(BATTLE_TYPE_NO_ESCAPE);
							fdEncounterSelectBattleMusic(ENCOUNTER_ZONE_MIMIC,
														 (u16)trigger->value);
							sySndPlayStub(MainRoutineStateWork.battleMusic);
							/* "The treasure chest was really a Mimic!" */
							mnFieldMessageShowStub(MsgChestWasMimic,
												   FIELD_MESSAGE_ZOOM);
						}
						else if(rewardType == FIND_COINS(0)) {
							/* Coins. */
							void *object;

							object = fdItemDispShowItemInFront(
								MainRoutineStateWork.leaderActorId, 0);
							sySynch(30);
							if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
							   EXAMINE_TRIGGER(EXAMINE_CHEST))
								fdTreasureSpotShowOpened(tile);
							fdItemDispRaiseAboveLeader(object);
							sySndPlayStub(MUS_TREASURE_FOUND);
							mnMessageArgumentPushStub((u16)trigger->value,
													  MESSAGE_ARGUMENT_NUMBER);
							/* "Isaac got [number] coins." */
							mnFieldMessageShowStub(
								MsgGotCoins,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
							gmPartyAdjustCoinsStub((u16)trigger->value);
							if(condition != -1)
								gmFlagSetStub(condition);
							fdActorReleaseStub(object);
						}
						else {
							/* An item, which stays in the chest when nobody can
							 * carry it. */
							void *object;
							s32 recipient;

							object = fdItemDispShowItemInFront(
								MainRoutineStateWork.leaderActorId,
								trigger->value & FIND_ITEM_MASK);
							sySynch(30);
							recipient =
								gmPartyInventoryAddStub((u16)trigger->value);
							/* messageOffset now masks the inventory entry the
							 * chest gave. */
							messageOffset =
								INVENTORY_QUANTITY_MASK | INVENTORY_ENTRY_MASK;
							if(recipient == -1) {
								mnMessageArgumentPushStub(
									trigger->value & FIND_ITEM_MASK,
									MESSAGE_ARGUMENT_ITEM);
								/* "Isaac found [item]." */
								mnFieldMessageShowStub(MsgFoundItem,
													   FIELD_MESSAGE_ZOOM);
								/* "But Isaac's party can't carry any more, so
								 * they left it */
								/* behind." */
								mnFieldMessageShowStub(MsgFoundItem + 4,
													   FIELD_MESSAGE_ZOOM);
								fdActorReleaseIfPresent(object);
								if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
								   EXAMINE_TRIGGER(EXAMINE_CHEST))
									fdTreasureSpotCloseChest(tile);
							}
							else {
								if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
								   EXAMINE_TRIGGER(EXAMINE_CHEST))
									fdTreasureSpotShowOpened(tile);
								fdItemDispRaiseAboveLeader(object);
								sySndPlayStub(MUS_TREASURE_FOUND);
								mnMessageArgumentPushStub(
									trigger->value & messageOffset,
									MESSAGE_ARGUMENT_ITEM);
								if(recipient ==
								   MainRoutineStateWork.leaderActorId) {
									/* "Isaac got [item]." */
									mnFieldMessageShowStub(
										MsgGotItem,
										FIELD_MESSAGE_ZOOM |
											FIELD_MESSAGE_WAITING_FOR_SOUND);
								}
								else {
									mnMessageArgumentPushStub(
										recipient, MESSAGE_ARGUMENT_ACTOR);
									/* "Isaac gave [item] to [member]." */
									mnFieldMessageShowStub(
										MsgGaveItemToMember,
										FIELD_MESSAGE_ZOOM |
											FIELD_MESSAGE_WAITING_FOR_SOUND);
								}
								if(condition != -1)
									gmFlagSetStub(condition);
								fdActorReleaseStub(object);
							}
						}
					}
					else {
						/* "but Isaac can't open it..." */
						mnFieldMessageShowStub(MsgCantOpenIt,
											   FIELD_MESSAGE_ZOOM);
					}
				}
				else {
					/* Already opened: the result line for the check message. */
					mnFieldMessageShowStub(MsgTileResult + messageOffset,
										   FIELD_MESSAGE_ZOOM);
				}
				fdEventEnd();
				scPlayQueuedMusic();
			}
		}
		else {
			/* "Isaac checked on the ground..." */
			mnFieldMessageShowStub(MsgCheckedGround, FIELD_MESSAGE_ZOOM);
			/* "but didn't find anything." */
			mnFieldMessageShowStub(MsgGroundDidntFindAnything,
								   FIELD_MESSAGE_ZOOM);
		}
		gmFlagClearStub(FLAG_NOTHING_HAPPENS);
	}
	return 0;
}

#undef EXAMINE_TARGET
#undef FIND_ITEM_MASK
#undef PSYNERGY_STONE_PP
