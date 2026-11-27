/* Finding and running a scene's cues: talking, step cues, examine cues. Part of
   field_mode.c. */

#include "field/actor_script.h"
#include "window/screens.h"

/* What an examine cue is on (EXAMINE), the control bits 8..4 */
/* EXAMINE_CUE() packs it into. */
#define EXAMINE_TARGET(kind)		(((kind) >> 4) & 0x1f)
/* The PP a Psynergy Stone gives each party member, enough to fill it: "The
   party's PP is fully restored!" */
#define PSYNERGY_STONE_PP			1999
/* The Psynergy Stone sprite's animation once it is used: it breaks into pieces
   that scatter as sparkles, then is gone (psynergy_stone.png's pictures 7 to
   10). */
#define ANIM_PSYNERGY_STONE_SHATTER 2

/* The encounter zone whose slots hold the Mimics' formations; Mimics past its
   ENCOUNTER_ZONE_SLOTS go on in the next zone. */
#define ENCOUNTER_ZONE_MIMIC 104
#define ENCOUNTER_ZONE_SLOTS 8

/* Finds actor id's row in the scene's four actor placement tables, or NULL: an
   id below ACTOR_FIRST_PLACED is the row of that sprite, a later one the row of
   that place among the placed actors' rows, counted in order. */
ACTION_DESCRIPTOR *fdFindActorPlacement(Sint32 id)
{
	ACTION_DESCRIPTOR *entry;
	Sint32			   i;
	Sint32			   group = ACTOR_FIRST_PLACED;

	for(i = 0; i < 4; i++) {
		entry = FieldStateWork->tbls[i];
		if(entry == NULL)
			continue;
		if(id < ACTOR_FIRST_PLACED) {
			for(; (Sint16)entry->spr != -1; entry++) {
				if((Sint16)entry->spr == id)
					goto found;
			}
		} else {
			for(; (Sint16)entry->spr != -1; entry++) {
				if((Sint16)entry->spr >= ACTOR_FIRST_PLACED) {
					if(group == id)
						goto found;
					group++;
				}
			}
		}
	}
found:
	if((Sint16)entry->spr == -1)
		return NULL;
	return entry;
}

/* Returns whether a script condition holds: -1 always holds; a condition with
   CONDITION_FLAG_SET holds while that flag is set, any other while its flag is
   clear. */
Sint32 fdGameFlagIsConditionActive(Sint32 condition)
{
	Sint32 flgSet;

	if(condition == -1) {
		return 1;
	}
	if(CONDITION_FLAG_SET & condition) {
		return syGameFlagGet(condition);
	}
	flgSet = syGameFlagGet(condition);
	return flgSet == 0;
}

/* Returns the first of the scene's cues of kind for target whose condition
   holds and whose push (STEP_CUE_FLAG_WALKING) and facing (CUE_FLAG_FACING)
   requirements are met, or NULL. CUE_PUSH and CUE_PUSHED, the cues
   fdRunPushMetatile runs for an actor pushed and for the metatile pushed
   against, answer any target; the facing test alone is skipped while
   MainRoutineStateWork.movementMode is LEADER_MOVEMENT_KEYS. */
CUE_VIEW *fdFindCue(Sint32 kind, Sint32 tgt)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	CUE_VIEW	*cue = state->cues;
	Sint32		 flg;
	Uint32 facing = fdActorLookup(MainRoutineStateWork.leaderActorId)->facing;

	flg = cue->flg;
	while(flg != -1) {
		Uint8 *mode = &MainRoutineStateWork.movementMode;

		if((flg & CUE_KIND_MASK) == kind &&
			(cue->value == tgt || kind == CUE_PUSH || kind == CUE_PUSHED) &&
			fdGameFlagIsConditionActive(cue->condition)) {
			Sint32 matches = 0;
			Sint32 minimumPushTicks = FIELD_PUSH_TICKS;
			Sint32 touchRequirements;

			flg = cue->flg;
			if(flg & STEP_CUE_FLAG_QUICK)
				minimumPushTicks = 2;
			touchRequirements = flg & (STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING);

			switch(touchRequirements) {
			case 0:
				matches = 1;
				break;
			case STEP_CUE_FLAG_WALKING:
				if(state->blockedSteps > minimumPushTicks)
					matches = 1;
				break;
			case CUE_FLAG_FACING:
				if(*mode != LEADER_MOVEMENT_KEYS) {
					/* Bits 15 to 12 are a facing in sixteenths of a turn; the
					   leader's facing must be within an eighth of a turn of
					   it. */
					flg &= CUE_FACING_MASK;
					matches = (Uint16)((flg - facing) + (TRIG_TURN / 8 - 1)) <=
						TRIG_TURN / 4 - 2;
				}
				break;
			case STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING:
				if(state->blockedSteps > minimumPushTicks) {
					flg &= CUE_FACING_MASK;
					matches = (Uint16)((flg - facing) + (TRIG_TURN / 8 - 1)) <=
						TRIG_TURN / 4 - 2;
				}
				break;
			}
			if(matches)
				return cue;
		}
		cue++;
		if(cue->flg == -1)
			break;
		flg = cue->flg;
	}
	return NULL;
}

/* Unused: runs the loaded overlay's initialize entry; nothing calls it. */
UNUSED void fdOverlayCallInit(void)
{
	SCENE_ENTRIES->initialize();
}

/* Returns the talk cue of actor id, or NULL: for the actor Halt has stopped
   (MainRoutineStateWork.haltedActor), its CUE_TALK_HALTED cue if it has one,
   the script for talking to it while it is stopped. */
void *fdFindActorTalkCue(Sint32 id)
{
	void			   *found = fdFindCue(CUE_TALK, id);
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	void			   *event;
	Sint32				stopped;

	stopped = party->haltedActor;
	event = found;
	if(stopped == id) {
		void *alt = fdFindCue(CUE_TALK_HALTED, stopped);

		if(alt != NULL)
			return alt;
	}
	return event;
}

Sint32 fdSpeakerGetHaltedLines(Sint32 id);

/* Talks to actor id. A container (actors from FIELD_TREASURE_ACTOR, and the
   Scoop spot, SCOOPED_ITEM_ACTOR) is checked instead: a chest gives what it
   holds, a Mimic starts its battle, a Psynergy Stone restores the party's PP
   and a stone tablet teaches its summon. Any other actor shows its script's
   message or calls its script function, stopped and turned as its placement
   says; the actor Halt has stopped answers its CUE_TALK_HALTED cue first. */
Sint32 fdTalkToActor(Sint32 id)
{
	ACTOR_PLACE *placement;
	Sint32		 result;
	ACTOR		*talker;
	Sint32		 savedAng;
	Sint32		 paused;
	Sint32		 decodeFlg;
	Sint32		 silent;
	FIELD_STATE *field;
	ACTOR		*leader;
	ACTOR		*shown;
	Sint16		*row;
	CUE_VIEW	*talk;
	Sint32		 point;
	Uint8		*busy;
	Sint32		 found;
	Sint32		 type;
	Sint32		 flg;
	Sint32		 contents;
	Sint32		 idx;
	Sint32		 slot;
	FIELD_STATE *state;

	placement = fdFindActorPlacement(id);
	result = -1;
	savedAng = 0;
	paused = 0;
	decodeFlg = 0;
	silent = id & FIELD_TALK_SILENT;
	field = RamHeapSlots->fieldState;
	id &= FIELD_TALK_ACTOR_MASK;
	found = 0;
	talker = fdActorGetById(id);
	if(id >= FIELD_TREASURE_ACTOR) {
		if(id != SCOOPED_ITEM_ACTOR) {
			row = fdSceneItemGetRows();
			if(row != NULL) {
				/* The actor's row (CONTAINER_ROW): its type, point, flag and
				   contents. */
				row += (id - FIELD_TREASURE_ACTOR) * 4;
				type = *row++;
				idx = id;
				found = 1;
				point = *row++;
				flg = *row++;
				contents = *row;
			}
		} else {
			type = CONTAINER_GROUND;
			point = result;
			flg = result;
			contents = field->scoopedItem;
			idx = SCOOPED_ITEM_ACTOR;
			found = 1;
		}
		if(found) {
			/* found now holds what the search reports (EXAMINE). */
			found = type;
			if((Uint16)(type - CONTAINER_CHEST) <=
				CONTAINER_MIMIC - CONTAINER_CHEST)
				found = EXAMINE_CHEST;
			if((Uint16)(type - CONTAINER_PSYNERGY_STONE) <=
				CONTAINER_STONE_TABLET - CONTAINER_PSYNERGY_STONE)
				found = EXAMINE_GROUND;
			if(type == CONTAINER_STONE_TABLET)
				found = EXAMINE_STONE_TABLET;
			if(type == CONTAINER_POND)
				found = EXAMINE_POND;
			fdScriptStart();
			busy = &talker->paused;
			*busy = 1;
			if(silent == 0)
				mnFieldMessageShowStub(
					found + (Sint32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
			if(MainRoutineStateWork.next == SCENE_VENUS_LIGHTHOUSE &&
				point == 0 && !syGameFlagGet(flg)) {
				mnMessageArgumentPushStub(contents, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub(
					(Sint32)&MsgChestAlreadyOpen, FIELD_MESSAGE_ZOOM);
			}
			if(!fdGameFlagIsConditionActive(flg))
				goto empty;
			if(type == CONTAINER_CHEST) {
				if(contents == 0)
					fdTreasureSpotOpenEmptyChest(idx);
				else
					fdTreasureSpotOpenChest(idx);
			}
			if(type == CONTAINER_MIMIC)
				fdTreasureSpotOpenChest(idx);
			if(type == CONTAINER_PSYNERGY_STONE) {
				ACTOR *stone = fdTreasureSpotGetActor(idx);

				stone->update = fdItemDispEmitSparkles;
				fdItemDispRaiseAboveLeader(stone);
				sySndPlayStub(MUS_TREASURE_FOUND);
				mnMessageArgumentPushStub(contents, MESSAGE_ARGUMENT_NUMBER);
				mnFieldMessageShowStub((Sint32)&MsgGotPsynergyStone,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
				fdPartyAddPp(PSYNERGY_STONE_PP, 0);
				mnPartyStatCreateWindowStub(PARTY_STATUS_SHOW_PP);
				sySndPlayStub(SE_RECOVERY);
				/* "<leader>'s PP are maxed out!" alone, else "The party's PP is
				   fully restored!" */
				if(gmPartyCountMembersStub() == 1)
					mnFieldMessageShowStub(
						(Sint32)&MsgGotPsynergyStone + 6, FIELD_MESSAGE_ZOOM);
				else
					mnFieldMessageShowStub(
						(Sint32)&MsgGotPsynergyStone + 1, FIELD_MESSAGE_ZOOM);
				mnPartyStatCloseStub();
				stone->update = NULL;
				fdActorSelectAnimationStub(stone, ANIM_PSYNERGY_STONE_SHATTER);
				sySndPlayStub(SE_GLITTER);
				fdScriptWait(30);
				mnFieldMessageShowStub(
					(Sint32)&MsgPsynergyStoneDisappeared, FIELD_MESSAGE_ZOOM);
				fdTreasureSpotRemove(idx);
				if(flg != -1)
					syGameFlagSet(flg);
			} else if(type == CONTAINER_STONE_TABLET) {
				fdScriptStart();
				fdScriptEndLastingPsynergy(0);
				fdSummonTabletPlayRunes(idx);
				mnSummonShowLearnedMessageStub(contents);
				gmSummonLearnStub(contents);
				fdTreasureSpotRemove(idx);
				if(flg != -1)
					syGameFlagSet(flg);
				fdScriptEnd();
			} else if(type == CONTAINER_MIMIC) {
				state = RamHeapSlots->fieldState;
				fdTreasureSpotLaunchMimic(idx);
				if(flg != -1)
					MainRoutineStateWork.pendingDjinni =
						flg | CONDITION_FLAG_SET;
				if(contents > ENCOUNTER_ZONE_SLOTS - 1)
					state->encounter =
						fdEncounterGetFormationAtSlot(ENCOUNTER_ZONE_MIMIC + 1,
							contents - ENCOUNTER_ZONE_SLOTS);
				else
					state->encounter = fdEncounterGetFormationAtSlot(
						ENCOUNTER_ZONE_MIMIC, contents);
				MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
				fdEncounterSelectBattleMusic(ENCOUNTER_ZONE_MIMIC, contents);
				sySndPlayStub(MainRoutineStateWork.battleMusic);
				mnFieldMessageShowStub(
					(Sint32)&MsgChestWasMimic, FIELD_MESSAGE_ZOOM);
			} else {
				if(type == CONTAINER_POND)
					mnFieldMessageShowStub(
						(Sint32)&MsgSomethingInTheShallows, FIELD_MESSAGE_ZOOM);
				if(fdActorGetSprShape(idx)) {
					mnFieldMessageShowStub(
						(Sint32)&MsgSomethingBuriedHere, FIELD_MESSAGE_ZOOM);
					if(gmPartyFindOwnerWithActionStub(ABILITY_SCOOP) != -1)
						FieldStateWork->fieldMenu = 1;
				} else if(contents & CONTAINER_COINS) {
					if(type == CONTAINER_GROUND) {
						shown = fdTreasureSpotGetActor(idx);
						shown->update = fdItemDispEmitSparkles;
					} else {
						shown = fdItemDispShowItemInFront(idx, 0);
						sySynch(30);
						if(type == CONTAINER_CHEST)
							fdTreasureSpotShowOpened(idx);
					}
					fdItemDispRaiseAboveLeader(shown);
					sySndPlayStub(MUS_TREASURE_FOUND);
					contents &= CONTAINER_COINS - 1;
					mnMessageArgumentPushStub(
						contents, MESSAGE_ARGUMENT_NUMBER);
					mnFieldMessageShowStub((Sint32)&MsgCoinsReceived,
						FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
					gmPartyAdjustCoinsStub(contents);
					if(flg != -1)
						syGameFlagSet(flg);
					if(type == CONTAINER_GROUND) {
						fdActorSetPosAndResetMotionStub(shown, 0, 0, 0);
						shown->update = NULL;
					} else {
						fdActorReleaseStub(shown);
					}
				} else if(contents == 0) {
					if(flg != -1)
						syGameFlagSet(flg);
					goto empty;
				} else {
					if(type == CONTAINER_GROUND) {
						shown = fdTreasureSpotGetActor(idx);
						shown->update = fdItemDispEmitSparkles;
					} else {
						shown = fdItemDispShowItemInFront(idx, contents);
						sySynch(30);
					}
					slot = gmPartyInventoryAddStub(contents);
					if(slot == -1) {
						mnMessageArgumentPushStub(
							contents, MESSAGE_ARGUMENT_ITEM);
						mnFieldMessageShowStub(
							(Sint32)&MsgFoundItem, FIELD_MESSAGE_ZOOM);
						/* "But <leader>'s party can't carry any more, so they
						   left it behind." */
						mnFieldMessageShowStub(
							(Sint32)&MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
						if(type != CONTAINER_GROUND)
							fdActorReleaseIfPresent(shown);
						if(type == CONTAINER_CHEST)
							fdTreasureSpotCloseChest(idx);
					} else {
						if(type == CONTAINER_CHEST)
							fdTreasureSpotShowOpened(idx);
						fdItemDispRaiseAboveLeader(shown);
						sySndPlayStub(MUS_TREASURE_FOUND);
						mnMessageArgumentPushStub(
							contents, MESSAGE_ARGUMENT_ITEM);
						if(slot == MainRoutineStateWork.leaderActorId) {
							mnFieldMessageShowStub((Sint32)&MsgItemReceived,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
						} else {
							mnMessageArgumentPushStub(
								slot, MESSAGE_ARGUMENT_ACTOR);
							mnFieldMessageShowStub((Sint32)&MsgItemGiven,
								FIELD_MESSAGE_ZOOM |
									FIELD_MESSAGE_WAITING_FOR_SOUND);
						}
						if(flg != -1)
							syGameFlagSet(flg);
						if(type == CONTAINER_GROUND) {
							fdActorSetPosAndResetMotionStub(shown, 0, 0, 0);
							shown->update = NULL;
						} else {
							fdActorReleaseStub(shown);
						}
						if(type == CONTAINER_POND)
							fdTreasureSpotRemove(idx);
						result = 0;
					}
				}
			}
			goto done;
		empty:
			mnFieldMessageShowStub(
				found + (Sint32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
		done:
			*busy = 0;
			fdScriptEnd();
			if(result != 0)
				return result;
		}
	}

	if(MainRoutineStateWork.haltedActor == id) {
		paused = 1;
		talk = fdFindCue(CUE_TALK_HALTED, id);
		if(talk == NULL) {
			/* With no halted line of its own, a halted actor's plain talk comes
			   out as halting speech, broken off after its first word. */
			talk = fdFindCue(CUE_TALK, id);
			decodeFlg = MESSAGE_DECODE_HALTING_SPEECH;
			if(talk == NULL)
				return result;
			/* A function talk cue is not run for a stopped actor: it says one
			   of the two lines of its sprite's Halted pair, at random. */
			if(talk->result >= CUE_RESULT_MESSAGE_END) {
				fdScriptRunMessage(id,
					fdSpeakerGetHaltedLines(id) * 2 + syRandomBelow(2) +
						(Sint32)&MsgPonderEllipsis);
				goto resume;
			}
		}
	} else {
		talk = fdFindCue(CUE_TALK, id);
	}
	if(talk != NULL && talk->result != 0) {
		if(!paused) {
			talker->paused = 1;
			fdActorSetAnimationSpeedStub(talker, 0);
			savedAng = talker->facing;
			if((Uint8)placement->talkFacing <= TALK_FACING_PARTY_AND_BACK ||
				(Sint8)placement->talkFacing == TALK_FACING_PARTY_THEN_WATCH) {
				leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
				fdActorSetPosAndResetMotionStub(
					leader, leader->x.fixed, leader->y.fixed, leader->z.fixed);
				fdActorTurnToFaceEachOther(
					id, MainRoutineStateWork.leaderActorId, 0);
			}
		}
		if(talk->result < CUE_RESULT_MESSAGE_END) {
			mnMessageSetDecodeFlagsStub(decodeFlg);
			fdScriptRunMessage(id, talk->result);
		} else {
			fdLeaderStopMoving();
			((void (*)(Sint32))talk->result)(id);
		}
		if(!paused) {
			if(talker->script[talker->scriptCursor] == SCRIPT_COMMAND_END) {
				if((Sint8)placement->talkFacing ==
					TALK_FACING_PARTY_THEN_WATCH) {
					talker->watchedActor = (union FIELD_OBJECT *)fdActorGetById(
						MainRoutineStateWork.leaderActorId);
					talker->steeringFlg |= ACTOR_FACES_MOTION;
					fdActorSetMovementType(
						talker, (Sint32)ActorFaceWatchedScript);
				} else if((Sint8)placement->talkFacing ==
					TALK_FACING_PARTY_AND_BACK) {
					talker->tmr = savedAng;
					fdActorStartScriptStub(talker, ActorStepAngleScript);
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

/* Runs the scene's raised cue (CUE_RAISED) for id: its message or its function.
   Returns 0 when a script ran, -1 when there was none. */
Sint32 fdRunRaisedCue(Sint32 id)
{
	CUE_VIEW *cue = fdFindCue(CUE_RAISED, id);
	Sint32	  result = -1;

	if(cue != NULL) {
		Sint32 handler = cue->result;

		if(handler != 0) {
			if(handler < CUE_RESULT_MESSAGE_END)
				fdScriptRunMessage(-1, handler);
			else
				((void (*)(Sint32))handler)(id);
			result = 0;
		}
	}
	return result;
}

/* Runs the step cue of what the leader touched, or else takes its exit: plays
   the exit's sound (and effect) and records where it leads. Returns 0 when
   something ran, -1 when there was nothing. */
Sint32 fdRunStepCueOrExit(Sint32 value)
{
	CUE_VIEW	*event;
	Sint32		 handled;
	FIELD_STATE *state;

	event = fdFindCue(CUE_STEP, value);
	handled = -1;
	state = FieldStateWork;
	if(event != NULL && event->result != 0) {
		if(event->flg & STEP_CUE_FLAG_WALKING)
			state->blockedSteps = 0;
		if(event->result < CUE_RESULT_MESSAGE_END) {
			fdScriptRunMessage(handled, event->result);
			handled = 0;
		} else {
			((void (*)(Sint32))event->result)(value);
			handled = 0;
		}
	} else {
		event = fdFindCue(CUE_EXIT, value);
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

/* Runs the push metatile the leader pushes against (attributes, its metatile's
   attribute bits) once it has pushed for more than FIELD_PUSH_TICKS, idle
   while MainRoutineStateWork.sandActive is set: a MAP_METATILE_LEDGE with
   KEY_DOWN held jumps down it, a MAP_METATILE_JUMP jumps forward, a
   MAP_METATILE_CLIMBABLE climbs down or up by the key held, and a
   MAP_METATILE_PUSH pushes the actor ahead, running the scene's CUE_PUSH cue
   for it and CUE_PUSHED for the metatile. Always returns 0. */
Sint32 fdRunPushMetatile(Sint32 attributes)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 owner = MainRoutineStateWork.leaderActorId;
	CUE_VIEW	*event;
	Sint32		 tgt;
	Sint32		 result;

	if(MainRoutineStateWork.sandActive != 0)
		return 0;
	if((attributes & MAP_METATILE_LEDGE) &&
		state->blockedSteps > FIELD_PUSH_TICKS &&
		(state->heldKeys & KEY_DOWN)) {
		fdActorJumpDownLedge(owner, ANIM_LEADER_SPRING, 0);
		state->blockedSteps = 0;
	}
	if((attributes & MAP_METATILE_JUMP) &&
		state->blockedSteps > FIELD_PUSH_TICKS && !fdTryJumpForward())
		state->blockedSteps = 0;
	if((attributes & MAP_METATILE_CLIMBABLE) &&
		state->blockedSteps > FIELD_PUSH_TICKS) {
		Sint32 kind =
			state->heldKeys & (KEY_RIGHT | KEY_LEFT | KEY_UP | KEY_DOWN);
		Sint32 done = -1;

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
	if((attributes & MAP_METATILE_PUSH) &&
		state->blockedSteps > FIELD_PUSH_TICKS &&
		fdLeaderCanPushObjectAhead()) {
		tgt = fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
		if(tgt != -1) {
			event = fdFindCue(CUE_PUSH, tgt);
			if(event != NULL && (result = event->result) != 0) {
				if(result < CUE_RESULT_MESSAGE_END)
					fdScriptRunMessage(-1, result);
				else
					((void (*)(Sint32, Sint32))result)(attributes, tgt);
			}
			fdLeaderPushObjectAhead();
			event = fdFindCue(CUE_PUSHED, attributes);
			if(event != NULL && (result = event->result) != 0) {
				if(result < CUE_RESULT_MESSAGE_END)
					fdScriptRunMessage(-1, result);
				else
					((void (*)(Sint32, Sint32))result)(attributes, tgt);
			}
			state->blockedSteps = 0;
		}
	}
	return 0;
}

/* The search command. METATILE_BEHAVIOR_JAR to METATILE_BEHAVIOR_WELL show
   their pair of messages from FieldExamineCueMessageOffsets; otherwise the
   examine cue of the metatile runs: its "checked the ..." message, then its
   message or callback, or the matching "found nothing" message. MsgTileChecked
   is the base of the "X checked the Y..." lines, MsgTileResult the base of the
   "found nothing" lines. */
Sint32 fdExamineMetatile(Sint32 id)
{
	CUE	  *event;
	Sint32 idx;
	Sint32 kind;
	Sint32 condition;
	Uint32 result;

	/* The unsigned compare also rejects codes below METATILE_BEHAVIOR_JAR. */
	idx = id - METATILE_BEHAVIOR_JAR;
	if((Uint32)idx <= METATILE_BEHAVIOR_WELL - METATILE_BEHAVIOR_JAR) {
		fdLeaderStopMoving();
		/* id, no longer needed, holds the pair's message number. */
		id = FieldExamineCueMessageOffsets[idx];
		mnFieldMessageShowStub(
			id + (Sint32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
		mnFieldMessageShowStub(id + (Sint32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
		return 0;
	}
	event = fdFindCue(CUE_EXAMINE, id);
	if(event != NULL) {
		Sint32 flg = event->ctrl;

		condition = event->condition;
		kind = EXAMINE_TARGET(flg);
		if(!(event->subject & CUE_SUBJECT_OWN_EFFECT) && kind != 0) {
			fdLeaderStopMoving();
			mnFieldMessageShowStub(
				kind + (Sint32)&MsgTileChecked, FIELD_MESSAGE_ZOOM);
			syGameFlagSet(FLAG_NOTHING_HAPPENS);
		} else {
			syGameFlagClear(FLAG_NOTHING_HAPPENS);
		}
		/* The value is a function's address (FIND_FUNCTION_BITS set), which is
		   called, or a FIND_ value: a FIND_MESSAGE is shown and any other type
		   does nothing here. */
		result = event->value;
		if((result & FIND_FUNCTION_BITS) != 0 ||
			(result & FIND_TYPE_MASK) == FIND_MESSAGE(0)) {
			if((result & FIND_FUNCTION_BITS) != 0) {
				if(fdGameFlagIsConditionActive(condition))
					((void (*)(Sint32))event->value)(
						MainRoutineStateWork.leaderActorId);
				if(syGameFlagGet(FLAG_NOTHING_HAPPENS))
					mnFieldMessageShowStub(
						kind + (Sint32)&MsgTileResult, FIELD_MESSAGE_ZOOM);
			} else {
				if(fdGameFlagIsConditionActive(condition))
					mnFieldMessageShowStub(
						(Uint16)event->value, FIELD_MESSAGE_ZOOM);
				else
					mnFieldMessageShowStub(
						(Sint32)&MsgButFoundNothing, FIELD_MESSAGE_ZOOM);
			}
		}
	} else {
		mnFieldMessageShowStub(
			(Sint32)&MsgTileChecked + EXAMINE_GROUND, FIELD_MESSAGE_ZOOM);
		mnFieldMessageShowStub(
			(Sint32)&MsgTileResult + EXAMINE_GROUND, FIELD_MESSAGE_ZOOM);
	}
	syGameFlagClear(FLAG_NOTHING_HAPPENS);
	return 0;
}
