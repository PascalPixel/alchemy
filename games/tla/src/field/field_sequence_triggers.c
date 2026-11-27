/* @file */
/* Finding and running a scene's triggers: talking, step triggers, examine */
/* triggers. */
/* Part of field_sequence.c. */

#include "field/actor_script.h"
#include "window/screens.h"

/* What an examine trigger is on (EXAMINE), the control bits 8..4 */
/* EXAMINE_TRIGGER() packs it into. */
#define EXAMINE_TARGET(kind) (((kind) >> 4) & 0x1f)
/* The PP a Psynergy Stone gives each party member, enough to fill it: "The */
/* party's PP is fully restored!" */
#define PSYNERGY_STONE_PP 1999
/* The Psynergy Stone sprite's animation once it is used: it breaks into */
/* pieces that scatter as sparkles, then is gone (psynergy_stone.png's */
/* pictures 7 to 10). */
#define ANIM_PSYNERGY_STONE_SHATTER 2

/* The encounter zone whose slots hold the Mimics' formations; Mimics past its
 */
/* ENCOUNTER_ZONE_SLOTS go on in the next zone. */
#define ENCOUNTER_ZONE_MIMIC 104
#define ENCOUNTER_ZONE_SLOTS 8

/* Finds actor id's row in the scene's four actor placement tables, or NULL: */
/* an id below ACTOR_FIRST_PLACED is the row of that sprite, a later one the */
/* row of that place among the placed actors' rows, counted in order. */
ACTION_DESCRIPTOR *scFindActorPlacement(s32 id) {
	ACTION_DESCRIPTOR *entry;
	s32 i;
	s32 group = ACTOR_FIRST_PLACED;

	for(i = 0; i < 4; i++) {
		entry = FieldStateWork->tbls[i];
		if(entry == NULL)
			continue;
		if(id < ACTOR_FIRST_PLACED) {
			for(; (s16)entry->spr != -1; entry++) {
				if((s16)entry->spr == id)
					goto found;
			}
		}
		else {
			for(; (s16)entry->spr != -1; entry++) {
				if((s16)entry->spr >= ACTOR_FIRST_PLACED) {
					if(group == id)
						goto found;
					group++;
				}
			}
		}
	}
found:
	if((s16)entry->spr == -1)
		return NULL;
	return entry;
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
		return syGameFlagGet(condition);
	}
	flagSet = syGameFlagGet(condition);
	return flagSet == 0;
}

/* Returns the first of the scene's triggers of kind for target whose */
/* condition holds and whose push (STEP_TRIGGER_FLAG_WALKING) and facing */
/* (TRIGGER_FLAG_FACING) requirements are met, or NULL. TRIGGER_PUSH and */
/* TRIGGER_PUSHED, the triggers fdRunPushCell runs for an actor pushed and */
/* for the cell pushed against, answer any target; the facing test alone is */
/* skipped while MainRoutineStateWork.movementMode is LEADER_MOVEMENT_KEYS. */
TRIGGER_VIEW *scFindTrigger(s32 kind, s32 target) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	TRIGGER_VIEW *trigger = state->triggers;
	s32 flags;
	u32 facing = fdActorLookup(MainRoutineStateWork.leaderActorId)->facing;

	flags = trigger->flg;
	while(flags != -1) {
		u8 *mode = &MainRoutineStateWork.movementMode;

		if((flags & TRIGGER_KIND_MASK) == kind &&
		   (trigger->value == target || kind == TRIGGER_PUSH ||
			kind == TRIGGER_PUSHED) &&
		   fdGameFlagIsConditionActive(trigger->condition)) {
			s32 matches = 0;
			s32 minimumPushFrames = FIELD_PUSH_FRAMES;
			s32 touchRequirements;

			flags = trigger->flg;
			if(flags & STEP_TRIGGER_FLAG_QUICK)
				minimumPushFrames = 2;
			touchRequirements =
				flags & (STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING);

			switch(touchRequirements) {
			case 0:
				matches = 1;
				break;
			case STEP_TRIGGER_FLAG_WALKING:
				if(state->blockedSteps > minimumPushFrames)
					matches = 1;
				break;
			case TRIGGER_FLAG_FACING:
				if(*mode != LEADER_MOVEMENT_KEYS) {
					/* Bits 15 to 12 are a facing in sixteenths of a turn; the
					 * leader's */
					/* facing must be within an eighth of a turn of it. */
					flags &= TRIGGER_FACING_MASK;
					matches = (u16)((flags - facing) + (TRIG_TURN / 8 - 1)) <=
							  TRIG_TURN / 4 - 2;
				}
				break;
			case STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING:
				if(state->blockedSteps > minimumPushFrames) {
					flags &= TRIGGER_FACING_MASK;
					matches = (u16)((flags - facing) + (TRIG_TURN / 8 - 1)) <=
							  TRIG_TURN / 4 - 2;
				}
				break;
			}
			if(matches)
				return trigger;
		}
		trigger++;
		if(trigger->flg == -1)
			break;
		flags = trigger->flg;
	}
	return NULL;
}

/* Unused: runs the loaded overlay's initialize entry; nothing calls it. */
UNUSED void fdOverlayCallInit(void) { SCENE_ENTRIES->initialize(); }

/* Returns the talk trigger of actor id, or NULL: for the actor Halt has */
/* stopped (MainRoutineStateWork.haltedActor), its TRIGGER_TALK_HALTED trigger
 */
/* if it has one, the event for talking to it while it is stopped. */
void *scFindActorTalkTrigger(s32 id) {
	void *found = scFindTrigger(TRIGGER_TALK, id);
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	void *event;
	s32 stopped;

	stopped = party->haltedActor;
	event = found;
	if(stopped == id) {
		void *alt = scFindTrigger(TRIGGER_TALK_HALTED, stopped);

		if(alt != NULL)
			return alt;
	}
	return event;
}

s32 fdSpeakerGetHaltedLines(s32 id);

/* Talks to actor id. A container (actors from FIELD_TREASURE_ACTOR, and the */
/* Scoop spot, SCOOPED_ITEM_ACTOR) is checked instead: a chest gives what it */
/* holds, a Mimic starts its battle, a Psynergy Stone restores the party's PP */
/* and a stone tablet teaches its summon. Any other actor shows its event's */
/* message or calls its event function, stopped and turned as its placement */
/* says; the actor Halt has stopped answers its TRIGGER_TALK_HALTED trigger */
/* first. */
s32 scTalkToActor(s32 id) {
	ACTOR_PLACE *placement;
	s32 result;
	ACTOR *talker;
	s32 savedAngle;
	s32 paused;
	s32 decodeFlags;
	s32 silent;
	FIELD_STATE *field;
	ACTOR *leader;
	ACTOR *shown;
	s16 *row;
	TRIGGER_VIEW *talk;
	s32 point;
	u8 *busy;
	s32 found;
	s32 type;
	s32 flag;
	s32 contents;
	s32 index;
	s32 slot;
	FIELD_STATE *state;

	placement = scFindActorPlacement(id);
	result = -1;
	savedAngle = 0;
	paused = 0;
	decodeFlags = 0;
	silent = id & FIELD_TALK_SILENT;
	field = Ram_HeapSlots->fieldState;
	id &= FIELD_TALK_ACTOR_MASK;
	found = 0;
	talker = fdActorGetById(id);
	if(id >= FIELD_TREASURE_ACTOR) {
		if(id != SCOOPED_ITEM_ACTOR) {
			row = fdSceneItemGetRows();
			if(row != NULL) {
				/* The actor's row (CONTAINER_ROW): its type, point, flag
				 * and */
				/* contents. */
				row += (id - FIELD_TREASURE_ACTOR) * 4;
				type = *row++;
				index = id;
				found = 1;
				point = *row++;
				flag = *row++;
				contents = *row;
			}
		}
		else {
			type = CONTAINER_GROUND;
			point = result;
			flag = result;
			contents = field->scoopedItem;
			index = SCOOPED_ITEM_ACTOR;
			found = 1;
		}
		if(found) {
			/* found now holds what the search reports (EXAMINE). */
			found = type;
			if((u16)(type - CONTAINER_CHEST) <=
			   CONTAINER_MIMIC - CONTAINER_CHEST)
				found = EXAMINE_CHEST;
			if((u16)(type - CONTAINER_PSYNERGY_STONE) <=
			   CONTAINER_STONE_TABLET - CONTAINER_PSYNERGY_STONE)
				found = EXAMINE_GROUND;
			if(type == CONTAINER_STONE_TABLET)
				found = EXAMINE_STONE_TABLET;
			if(type == CONTAINER_POND)
				found = EXAMINE_POND;
			fdEventStart();
			busy = &talker->paused;
			*busy = 1;
			if(silent == 0)
				fdMessageShowStub(found + (s32)&MsgTileChecked,
								  FIELD_MESSAGE_ZOOM);
			if(MainRoutineStateWork.next == SCENE_VENUS_LIGHTHOUSE &&
			   point == 0 && !syGameFlagGet(flag)) {
				mnMessageArgumentPushStub(contents, MESSAGE_ARGUMENT_ITEM);
				fdMessageShowStub((s32)&MsgChestAlreadyOpen,
								  FIELD_MESSAGE_ZOOM);
			}
			if(!fdGameFlagIsConditionActive(flag))
				goto empty;
			if(type == CONTAINER_CHEST) {
				if(contents == 0)
					fdTreasureSpotOpenEmptyChest(index);
				else
					fdTreasureSpotOpenChest(index);
			}
			if(type == CONTAINER_MIMIC)
				fdTreasureSpotOpenChest(index);
			if(type == CONTAINER_PSYNERGY_STONE) {
				ACTOR *stone = fdTreasureSpotGetActor(index);

				stone->update = fdItemDispEmitSparkles;
				fdItemDispRaiseAboveLeader(stone);
				sySndPlayStub(MUS_TREASURE_FOUND);
				mnMessageArgumentPushStub(contents, MESSAGE_ARGUMENT_NUMBER);
				fdMessageShowStub((s32)&MsgGotPsynergyStone,
								  FIELD_MESSAGE_ZOOM |
									  FIELD_MESSAGE_WAITING_FOR_SOUND);
				fdPartyAddPp(PSYNERGY_STONE_PP, 0);
				mnPartyStatCreateWindowStub(PARTY_STATUS_SHOW_PP);
				sySndPlayStub(SE_RECOVERY);
				/* "<leader>'s PP are maxed out!" alone, else "The party's PP is
				 */
				/* fully restored!" */
				if(gmPartyCountMembersStub() == 1)
					fdMessageShowStub((s32)&MsgGotPsynergyStone + 6,
									  FIELD_MESSAGE_ZOOM);
				else
					fdMessageShowStub((s32)&MsgGotPsynergyStone + 1,
									  FIELD_MESSAGE_ZOOM);
				mnPartyStatCloseStub();
				stone->update = NULL;
				fdActorSelectAnimationStub(stone, ANIM_PSYNERGY_STONE_SHATTER);
				sySndPlayStub(SE_GLITTER);
				fdEventWait(30);
				fdMessageShowStub((s32)&MsgPsynergyStoneDisappeared,
								  FIELD_MESSAGE_ZOOM);
				fdTreasureSpotRemove(index);
				if(flag != -1)
					syGameFlagSet(flag);
			}
			else if(type == CONTAINER_STONE_TABLET) {
				fdEventStart();
				fdEventEndLastingPsynergy(0);
				fdSummonTabletPlayRunes(index);
				gmSummonShowLearnedMessageStub(contents);
				gmSummonLearnStub(contents);
				fdTreasureSpotRemove(index);
				if(flag != -1)
					syGameFlagSet(flag);
				fdEventEnd();
			}
			else if(type == CONTAINER_MIMIC) {
				state = Ram_HeapSlots->fieldState;
				fdTreasureSpotLaunchMimic(index);
				if(flag != -1)
					MainRoutineStateWork.pendingDjinni =
						flag | CONDITION_FLAG_SET;
				if(contents > ENCOUNTER_ZONE_SLOTS - 1)
					state->encounter = fdEncounterGetFormationAtSlot(
						ENCOUNTER_ZONE_MIMIC + 1,
						contents - ENCOUNTER_ZONE_SLOTS);
				else
					state->encounter = fdEncounterGetFormationAtSlot(
						ENCOUNTER_ZONE_MIMIC, contents);
				MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
				fdEncounterSelectBattleMusic(ENCOUNTER_ZONE_MIMIC, contents);
				sySndPlayStub(MainRoutineStateWork.battleMusic);
				fdMessageShowStub((s32)&MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
			}
			else {
				if(type == CONTAINER_POND)
					fdMessageShowStub((s32)&MsgSomethingInTheShallows,
									  FIELD_MESSAGE_ZOOM);
				if(fdActorGetSprShape(index)) {
					fdMessageShowStub((s32)&MsgSomethingBuriedHere,
									  FIELD_MESSAGE_ZOOM);
					if(gmPartyFindOwnerWithActionStub(ABILITY_SCOOP) != -1)
						FieldStateWork->fieldMenu = 1;
				}
				else if(contents & CONTAINER_COINS) {
					if(type == CONTAINER_GROUND) {
						shown = fdTreasureSpotGetActor(index);
						shown->update = fdItemDispEmitSparkles;
					}
					else {
						shown = fdItemDispShowItemInFront(index, 0);
						sySynch(30);
						if(type == CONTAINER_CHEST)
							fdTreasureSpotShowOpened(index);
					}
					fdItemDispRaiseAboveLeader(shown);
					sySndPlayStub(MUS_TREASURE_FOUND);
					contents &= CONTAINER_COINS - 1;
					mnMessageArgumentPushStub(contents,
											  MESSAGE_ARGUMENT_NUMBER);
					fdMessageShowStub((s32)&MsgCoinsReceived,
									  FIELD_MESSAGE_ZOOM |
										  FIELD_MESSAGE_WAITING_FOR_SOUND);
					gmPartyAdjustCoinsStub(contents);
					if(flag != -1)
						syGameFlagSet(flag);
					if(type == CONTAINER_GROUND) {
						fdActorSetPosAndResetMotionStub(shown, 0, 0, 0);
						shown->update = NULL;
					}
					else {
						fdActorReleaseStub(shown);
					}
				}
				else if(contents == 0) {
					if(flag != -1)
						syGameFlagSet(flag);
					goto empty;
				}
				else {
					if(type == CONTAINER_GROUND) {
						shown = fdTreasureSpotGetActor(index);
						shown->update = fdItemDispEmitSparkles;
					}
					else {
						shown = fdItemDispShowItemInFront(index, contents);
						sySynch(30);
					}
					slot = gmPartyInventoryAddStub(contents);
					if(slot == -1) {
						mnMessageArgumentPushStub(contents,
												  MESSAGE_ARGUMENT_ITEM);
						fdMessageShowStub((s32)&MsgFoundItem,
										  FIELD_MESSAGE_ZOOM);
						/* "But <leader>'s party can't carry any more, so they
						 * left it */
						/* behind." */
						fdMessageShowStub((s32)&MsgFoundItem + 4,
										  FIELD_MESSAGE_ZOOM);
						if(type != CONTAINER_GROUND)
							fdActorReleaseIfPresent(shown);
						if(type == CONTAINER_CHEST)
							fdTreasureSpotCloseChest(index);
					}
					else {
						if(type == CONTAINER_CHEST)
							fdTreasureSpotShowOpened(index);
						fdItemDispRaiseAboveLeader(shown);
						sySndPlayStub(MUS_TREASURE_FOUND);
						mnMessageArgumentPushStub(contents,
												  MESSAGE_ARGUMENT_ITEM);
						if(slot == MainRoutineStateWork.leaderActorId) {
							fdMessageShowStub(
								(s32)&MsgItemReceived,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
						}
						else {
							mnMessageArgumentPushStub(slot,
													  MESSAGE_ARGUMENT_ACTOR);
							fdMessageShowStub(
								(s32)&MsgItemGiven,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
						}
						if(flag != -1)
							syGameFlagSet(flag);
						if(type == CONTAINER_GROUND) {
							fdActorSetPosAndResetMotionStub(shown, 0, 0, 0);
							shown->update = NULL;
						}
						else {
							fdActorReleaseStub(shown);
						}
						if(type == CONTAINER_POND)
							fdTreasureSpotRemove(index);
						result = 0;
					}
				}
			}
			goto done;
		empty:
			fdMessageShowStub(found + (s32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
		done:
			*busy = 0;
			fdEventEnd();
			if(result != 0)
				return result;
		}
	}

	if(MainRoutineStateWork.haltedActor == id) {
		paused = 1;
		talk = scFindTrigger(TRIGGER_TALK_HALTED, id);
		if(talk == NULL) {
			/* With no halted line of its own, a halted actor's plain talk comes
			 * out */
			/* as halting speech, broken off after its first word. */
			talk = scFindTrigger(TRIGGER_TALK, id);
			decodeFlags = MESSAGE_DECODE_HALTING_SPEECH;
			if(talk == NULL)
				return result;
			/* A function talk trigger is not run for a stopped actor: it says
			 * one */
			/* of the two lines of its sprite's Halted pair, at random. */
			if(talk->result >= TRIGGER_RESULT_MESSAGE_END) {
				fdEventRunMessage(id, fdSpeakerGetHaltedLines(id) * 2 +
										  syRandomBelow(2) +
										  (s32)&MsgPonderEllipsis);
				goto resume;
			}
		}
	}
	else {
		talk = scFindTrigger(TRIGGER_TALK, id);
	}
	if(talk != NULL && talk->result != 0) {
		if(!paused) {
			talker->paused = 1;
			fdActorSetAnimationSpeedStub(talker, 0);
			savedAngle = talker->facing;
			if((u8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK ||
			   (s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
				leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
				fdActorSetPosAndResetMotionStub(
					leader, leader->x.fixed, leader->y.fixed, leader->z.fixed);
				fdActorTurnToFaceEachOther(
					id, MainRoutineStateWork.leaderActorId, 0);
			}
		}
		if(talk->result < TRIGGER_RESULT_MESSAGE_END) {
			mnMessageSetDecodeFlagsStub(decodeFlags);
			fdEventRunMessage(id, talk->result);
		}
		else {
			fdLeaderStopMoving();
			((void (*)(s32))talk->result)(id);
		}
		if(!paused) {
			if(talker->script[talker->scriptCursor] == SCRIPT_COMMAND_END) {
				if((s8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
					talker->watchedActor = (union FIELD_OBJECT *)fdActorGetById(
						MainRoutineStateWork.leaderActorId);
					talker->steeringFlg |= ACTOR_FACES_MOTION;
					fdActorSetMovementType(talker,
										   (s32)Actor_FaceWatchedScript);
				}
				else if((s8)placement->talkFacing ==
						TALK_FACING_PARTY_AND_BACK) {
					talker->tmr = savedAngle;
					fdActorStartScriptStub(talker, Actor_StepAngleScript);
				}
			}
			talker->paused = 0;
			fdActorSetAnimationSpeedStub(talker, ANIM_STEP_NORMAL);
		}
		result = 0;
	}
resume:
	if(paused) {
		fdPsynergyResumeHaltedActor(MainRoutineStateWork.haltedActor);
		MainRoutineStateWork.haltedActor = -1;
	}
	return result;
}

/* Runs the scene's raised trigger (TRIGGER_RAISED) for id: its message or */
/* its function. Returns 0 when an event ran, -1 when there was none. */
s32 scRunRaisedTrigger(s32 id) {
	TRIGGER_VIEW *trigger = scFindTrigger(TRIGGER_RAISED, id);
	s32 result = -1;

	if(trigger != NULL) {
		s32 handler = trigger->result;

		if(handler != 0) {
			if(handler < TRIGGER_RESULT_MESSAGE_END)
				fdEventRunMessage(-1, handler);
			else
				((void (*)(s32))handler)(id);
			result = 0;
		}
	}
	return result;
}

/* Runs the step trigger of what the leader touched, or else takes its exit: */
/* plays the exit's sound (and effect) and records where it leads. Returns 0 */
/* when something ran, -1 when there was nothing. */
s32 scRunStepTriggerOrExit(s32 value) {
	TRIGGER_VIEW *event;
	s32 handled;
	FIELD_STATE *state;

	event = scFindTrigger(TRIGGER_STEP, value);
	handled = -1;
	state = FieldStateWork;
	if(event != NULL && event->result != 0) {
		if(event->flg & STEP_TRIGGER_FLAG_WALKING)
			state->blockedSteps = 0;
		if(event->result < TRIGGER_RESULT_MESSAGE_END) {
			fdEventRunMessage(handled, event->result);
			handled = 0;
		}
		else {
			((void (*)(s32))event->result)(value);
			handled = 0;
		}
	}
	else {
		event = scFindTrigger(TRIGGER_EXIT, value);
		if(event != NULL) {
			switch(event->flg & EXIT_STYLE_MASK) {
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
			case EXIT_STYLE_HIDDEN_LAYER:
				sySndPlayStub(SE_MAP_EXIT);
				fdExitAddHiddenLeaderLayer();
				break;
			}
			state->exit = event->result;
			handled = 0;
		}
	}
	return handled;
}

/* Runs the push cell the leader pushes against (attributes, its cell's */
/* attribute bits) once it has pushed for more than FIELD_PUSH_FRAMES, idle */
/* while MainRoutineStateWork.sandActive is set: a MAP_CELL_LEDGE with KEY_DOWN
 */
/* held jumps down it, a MAP_CELL_JUMP jumps forward, a MAP_CELL_CLIMBABLE */
/* climbs down or up by the key held, and a MAP_CELL_PUSH pushes the actor */
/* ahead, running the scene's TRIGGER_PUSH trigger for it and TRIGGER_PUSHED */
/* for the cell. Always returns 0. */
s32 fdRunPushCell(s32 attributes) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 owner = MainRoutineStateWork.leaderActorId;
	TRIGGER_VIEW *event;
	s32 target;
	s32 result;

	if(MainRoutineStateWork.sandActive != 0)
		return 0;
	if((attributes & MAP_CELL_LEDGE) &&
	   state->blockedSteps > FIELD_PUSH_FRAMES &&
	   (state->heldKeys & KEY_DOWN)) {
		fdActorJumpDownLedge(owner, ANIM_LEADER_SPRING, 0);
		state->blockedSteps = 0;
	}
	if((attributes & MAP_CELL_JUMP) &&
	   state->blockedSteps > FIELD_PUSH_FRAMES && !fdTryJumpForward())
		state->blockedSteps = 0;
	if((attributes & MAP_CELL_CLIMBABLE) &&
	   state->blockedSteps > FIELD_PUSH_FRAMES) {
		s32 kind = state->heldKeys & (KEY_RIGHT | KEY_LEFT | KEY_UP | KEY_DOWN);
		s32 done = -1;

		if(kind == KEY_DOWN)
			done = fdClimbDown();
		else if(kind == KEY_UP)
			done = fdClimbUp();
		if(done == 0) {
			state->blockedSteps = 0;
			state->fieldMenu = 0;
			state->pauseMenu = 0;
			state->aButton = 0;
			state->encounter = 0;
			state->psynergy = 0;
		}
	}
	if((attributes & MAP_CELL_PUSH) &&
	   state->blockedSteps > FIELD_PUSH_FRAMES &&
	   fdLeaderCanPushObjectAhead()) {
		target = fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
		if(target != -1) {
			event = scFindTrigger(TRIGGER_PUSH, target);
			if(event != NULL && (result = event->result) != 0) {
				if(result < TRIGGER_RESULT_MESSAGE_END)
					fdEventRunMessage(-1, result);
				else
					((void (*)(s32, s32))result)(attributes, target);
			}
			fdLeaderPushObjectAhead();
			event = scFindTrigger(TRIGGER_PUSHED, attributes);
			if(event != NULL && (result = event->result) != 0) {
				if(result < TRIGGER_RESULT_MESSAGE_END)
					fdEventRunMessage(-1, result);
				else
					((void (*)(s32, s32))result)(attributes, target);
			}
			state->blockedSteps = 0;
		}
	}
	return 0;
}

/* The search command. CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL show their pair
 */
/* of messages from Field_ExamineTriggerMessageOffsets; otherwise the examine */
/* trigger of the cell runs: its "checked the ..." message, then its message or
 */
/* callback, or the matching "found nothing" message. MsgTileChecked is the */
/* base of the "X checked the Y..." lines, MsgTileResult the base of the "found
 */
/* nothing" lines. */
s32 fdExamineCell(s32 id) {
	TRIGGER *event;
	s32 index;
	s32 kind;
	s32 condition;
	u32 result;

	/* The unsigned compare also rejects codes below CELL_BEHAVIOR_JAR. */
	index = id - CELL_BEHAVIOR_JAR;
	if((u32)index <= CELL_BEHAVIOR_WELL - CELL_BEHAVIOR_JAR) {
		fdLeaderStopMoving();
		/* id, no longer needed, holds the pair's message number. */
		id = Field_ExamineTriggerMessageOffsets[index];
		fdMessageShowStub(id + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
		fdMessageShowStub(id + (s32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
		return 0;
	}
	event = scFindTrigger(TRIGGER_EXAMINE, id);
	if(event != NULL) {
		s32 flags = event->ctrl;

		condition = event->condition;
		kind = EXAMINE_TARGET(flags);
		if(!(event->subject & TRIGGER_SUBJECT_OWN_EFFECT) && kind != 0) {
			fdLeaderStopMoving();
			fdMessageShowStub(kind + (s32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
			syGameFlagSet(FLAG_NOTHING_HAPPENS);
		}
		else {
			syGameFlagClear(FLAG_NOTHING_HAPPENS);
		}
		/* The value is a function's address (FIND_FUNCTION_BITS set), which is
		 */
		/* called, or a FIND_ value: a FIND_MESSAGE is shown and any other type
		 */
		/* does nothing here. */
		result = event->value;
		if((result & FIND_FUNCTION_BITS) != 0 ||
		   (result & FIND_TYPE_MASK) == FIND_MESSAGE(0)) {
			if((result & FIND_FUNCTION_BITS) != 0) {
				if(fdGameFlagIsConditionActive(condition))
					((void (*)(s32))event->value)(
						MainRoutineStateWork.leaderActorId);
				if(syGameFlagGet(FLAG_NOTHING_HAPPENS))
					fdMessageShowStub(kind + (s32)&MsgTileResult,
									  FIELD_MESSAGE_ZOOM);
			}
			else {
				if(fdGameFlagIsConditionActive(condition))
					fdMessageShowStub((u16)event->value, FIELD_MESSAGE_ZOOM);
				else
					fdMessageShowStub((s32)&MsgButFoundNothing,
									  FIELD_MESSAGE_ZOOM);
			}
		}
	}
	else {
		fdMessageShowStub((s32)&MsgTileChecked + EXAMINE_GROUND,
						  FIELD_MESSAGE_ZOOM);
		fdMessageShowStub((s32)&MsgTileResult + EXAMINE_GROUND,
						  FIELD_MESSAGE_ZOOM);
	}
	syGameFlagClear(FLAG_NOTHING_HAPPENS);
	return 0;
}
