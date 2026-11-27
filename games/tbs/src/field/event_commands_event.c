/* @file */
/* Event commands: waits, received items, questions and destinations. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* When a gift goes to another party member, `<leader> gave <item> to */
/* <member>.` follows `<leader> got <item>.`; it is MsgGaveItemToMember, the */
/* next message, in every edition. */
#if LANGUAGE == LANGUAGE_GERMAN
#define GIFT_RECEIVED_MESSAGE MsgGaveItemToMember
#else
#define GIFT_RECEIVED_MESSAGE (MsgGotItem + 1)
#endif

/* Japanese and English show it zooming open and waiting for the jingle; from */
/* patch 2 it only zooms open. */
#if TBS_PATCH_2
#define GIFT_RECEIVED_MESSAGE_MODE FIELD_MESSAGE_ZOOM
#else
#define GIFT_RECEIVED_MESSAGE_MODE                                             \
	(FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND)
#endif

/* Shows the message for an item the party has just received. */
static inline void fdItemGiftShowGotItem(s32 item, s32 flags) {
#if LANGUAGE == LANGUAGE_GERMAN
	if(item == ITEM_BLACK_ORB) {
		mnFieldMessageShowStub(MsgGotBlackOrb, flags);
		return;
	}
#endif
	mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
	mnFieldMessageShowStub(MsgGotItem, flags);
}

/* Debug only: while debug mode is on, R makes every event wait (fdEventWait) */
/* end at once and L restores normal waits. */
void fdEventUpdateDebugSkip(void) {
	FIELD_STATE *fieldState = FieldStateWork;

	if(DebugMode != 0) {
		if(KeysPressed & KEY_L) {
			fieldState->fastForward = 0;
		}
		if(KeysPressed & KEY_R) {
			fieldState->fastForward = -1;
		}
	}
}

/* Waits the requested frames through sySynch(), unless debug fast-forward */
/* (fdEventUpdateDebugSkip) is on: the wait events use. */
void fdEventWait(s32 frames) {
	if(FieldStateWork->fastForward == 0 && frames != 0) {
		sySynch(frames);
	}
}

/* Stops the leader where it stands: no horizontal velocity, the normal walking
 */
/* speed (1.0) and acceleration (0.5), no destination, and the standing pose, */
/* or the clinging pose while the leader is on a climbable wall. */
void fdLeaderStopMoving(void) {
	ACTOR *actor;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	actor->speedLimit = FIXED_ONE;
	actor->acceleration = toFIXED(1) / 2;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->velX = 0;
	actor->velZ = 0;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_CLIMB) {
		fdActorSelectAnimationStub(actor, ANIM_LEADER_CLING);
		return;
	}
	fdActorSelectAnimationStub(actor, ANIM_STAND);
}

/* Begins an event: closes any area name window, stops the leader, resets the */
/* message and timing state, and starts the debug fast-forward task with the */
/* leader as the message speaker. */
void fdEventStart(void) {
	FIELD_STATE *fieldState = FieldStateWork;

	mnAreaNameWindowHideStub();
	fdLeaderStopMoving();
	/* A raised or step trigger still marked running is ended first. */
	if((s16)fieldState->busy != 0) {
		fdEndBusyAndReveal();
	}
	/* Every event starts with no name window and normal waits. */
	fieldState->choiceMsg = 0;
	fieldState->choiceWidth = 0;
	fieldState->transitionFrm = MAP_TRANSITION_FRAMES;
	fieldState->fastForward = 0;
	fieldState->unusedAfterMsg = -1;
	fieldState->unusedBeforeCamera[0] = -1;
	fieldState->unusedBeforeCamera[1] = -1;
	sySetEvent(fdEventUpdateDebugSkip, TASK_PASS_FRAME);
	/* XXX (no evidence): what flag 0x132 means; every event start in both games
	 */
	/* clears it, nothing in either game's source names it or reads it by its */
	/* number, and no other code of tbs-en's build loads that number for a flag.
	 */
	gmFlagClearStub(0x132);
	fieldState->speaker = MainRoutineStateWork.leaderActorId;
	fieldState->msgWin = 0;
}

/* Ends an event: stops the debug fast-forward task, gives the camera back to */
/* the leader and refreshes FLAG_LURE_CAP (gmFlagRefreshLureCap). */
void fdEventEnd(void) {
	syCloseEvent(fdEventUpdateDebugSkip);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	gmFlagRefreshLureCapStub();
}

UNUSED void fdEventNoOpUpdateA(void) {}

/* An empty hook; it does nothing in the released game. Only */
/* fdEventRunActorUpdateAndWait calls it. */
void fdEventNoOpUpdate(void) {}

/* Respawns the scene's actors from an actor placement table and */
/* waits one frame for them to appear. */
void fdEventRunActorUpdateAndWait(ACTOR_PLACE *table) {
	fdEventNoOpUpdate();
	fdActorResetForLeader(table);
	sySynch(1);
	fdActorLookup(MainRoutineStateWork.leaderActorId);
}

/* Spawns the actors of an actor placement table after the last actor in use. */
void fdEventAppendActorPlacements(ACTOR_PLACE *table) {
	fdEventSpawnActorPlacements(table, fdActorFindLastActiveId());
}

/* Sets the scene's trigger table, which the field checks as the leader moves:
 */
/* Altin Peak's event calls it. */
void fdEventSetTriggers(const TRIGGER *triggers) {
	FIELD_STATE *state = FieldStateWork;

	state->triggers = triggers;
}

/* Adds unit to the party. With announce nonzero it also shows `<name> joined */
/* your party.` (mnPartyShowJoinedMessage, which receives announce too). */
void fdEventAddPartyMember(s32 unit, s32 announce) {
	gmUnitGetStub(unit);
	gmPartyAddMemberStub(unit);
	if(announce != 0) {
		gmPartyShowJoinedMessageStub(unit, announce);
	}
}

/* Adds two units to the party and announces them together. Vale's */
/* event calls it twice. */
void fdEventAddPartyMemberPair(s32 first, s32 second) {
	gmPartyAddMemberStub(first);
	gmPartyAddMemberStub(second);
	gmPartyShowPairJoinedMessageStub(first, second);
}

/* Checks an L or R button shortcut, packed as a field Psynergy use is: the */
/* party member (ACT_UNIT) and the Psynergy (ABILITY_ID). Returns 0 when it can
 */
/* be cast, -1 for no such character, -2 when they are out of the party and -3
 */
/* when they do not know the Psynergy. */
s32 fdPartyCheckShortcut(u32 shortcut) {
	s32 member = ACT_UNIT(shortcut);
	s32 ability = ABILITY_ID(shortcut);

	if(member > ALLY_MAX)
		return -1;
	if(gmFlagGetStub(FLAG_IN_PARTY(member)) == 0)
		return -2;
	if(gmUnitKnowsPsynergyStub(member, ability) == 0)
		return -3;
	return 0;
}

/* Clears each button shortcut whose Psynergy can no longer be cast: zeroes */
/* MainRoutineStateWork.firstShortcut or secondShortcut. */
void fdPartyClearUnusableShortcuts(void) {
	if(fdPartyCheckShortcut(MainRoutineStateWork.firstShortcut) != 0)
		MainRoutineStateWork.firstShortcut = 0;
	if(fdPartyCheckShortcut(MainRoutineStateWork.secondShortcut) != 0)
		MainRoutineStateWork.secondShortcut = 0;
}

/* Takes unit out of the party, fully restoring its HP and PP and curing its */
/* poison. */
void fdPartyRemoveMemberRestored(s32 unit) {
	UNIT *vitals;
	s32 partyIndex;
	s32 partyCount;
	s32 survivingMembers;

	gmPartyRemoveMemberStub(unit);
	fdPartyClearUnusableShortcuts();
	vitals = gmUnitGetStub(unit);
	vitals->hp = vitals->hpMax;
	vitals->pp = vitals->ppMax;
	gmUnitUpdateGauges(vitals);
	vitals->poison = 0;
	/* Count the party members who remain and still have HP. */
	survivingMembers = 0;
	partyCount = gmPartyCountMembersStub();
	for(partyIndex = 0; partyIndex < partyCount; partyIndex++) {
		vitals = gmUnitGetStub(MainRoutineStateWork.party[partyIndex]);
		if(vitals->hp != 0)
			survivingMembers++;
	}
	if(survivingMembers == 0) {
		vitals = gmUnitGetStub(MainRoutineStateWork.leaderActorId);
		vitals->hp = 1;
		gmUnitUpdateGauges(vitals);
	}
}

/* Checks whether the party can carry one more of item: each party member */
/* carries at most INVENTORY_STACK_MAX of one item. */
s32 fdPartyCheckItemLimit(s32 item) {
	s32 partyCount;
	s32 heldQuantity = 0;
	s32 partyIndex;

	partyCount = gmPartyCountMembersStub();
	for(partyIndex = 0; partyIndex < partyCount; partyIndex++)
		heldQuantity += gmInventoryCountItemStub(
			MainRoutineStateWork.party[partyIndex], item);

	if(heldQuantity >= partyCount * INVENTORY_STACK_MAX) {
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Isaac's party cannot carry any more <item>s..." */
		mnFieldMessageShowStub(MsgCannotCarryMore, FIELD_MESSAGE_ZOOM);
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Isaac threw the <item> as far away as possible." */
		mnFieldMessageShowStub(MsgCannotCarryMore + 1, FIELD_MESSAGE_ZOOM);
		return -1;
	}
	return 0;
}

/* Gives item to the first party member with room for it and says so; when */
/* every bag is full the player picks something to drop. Returns who got it, */
/* or -1. */
s32 fdPartyInventoryGiveItem(s32 item, s32 unused) {
	FIELD_STATE *state;
	s32 savedMessage;
	s32 owner;
	s32 result;
	s32 discardQuantity;
	s32 discardIndex;
	s32 member;
	s32 slot;

	state = FieldStateWork;
	/* The messages below move the event's message on; every return puts it */
	/* back, so the event continues where it was. */
	savedMessage = state->msg;
	owner = gmPartyInventoryAddStub(item);
	if(owner == -1) {
		/* Every bag is full: "But you can't carry any more items." */
		fdItemGiftShowGotItem(item, FIELD_MESSAGE_ZOOM);
		mnFieldMessageShowStub(MsgCannotCarryAnyMore, FIELD_MESSAGE_ZOOM);
	retry:
		mnFieldMessageShowStub(MsgWhatWillYouDrop, FIELD_MESSAGE_ZOOM);
		result = mnItemPickerRunStub(&member, &slot);
		if(result == -1) {
			/* Cancelled: drop the new item instead, unless it is indispensable
			 */
			/* ("That <item>'s valuable! You must get it!"). */
			if(gmItemGetDirectStub(item)->flg & ITEM_INDISPENSABLE) {
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub(MsgWhatWillYouDrop + 4,
									   FIELD_MESSAGE_ZOOM);
				goto retry;
			}
			/* "Drop the <item>?" */
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(MsgWhatWillYouDrop + 1,
								   FIELD_MESSAGE_ZOOM |
									   FIELD_MESSAGE_KEEP_OPEN);
			result = fdYesNoAsk(1);
			mnMessageCloseFinishedWindowsStub();
			if(result != 0)
				goto retry;
			/* "Isaac drops the <item>." */
			gmItemAdjustRareItemStockStub(item, 1);
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(MsgWhatWillYouDrop + 2, FIELD_MESSAGE_ZOOM);
			state->msg = savedMessage;
			return owner;
		}
		else {
			/* A carried item was picked: drop it (the whole stack) and take the
			 * new */
			/* one. */
			{
				UNIT *unit = gmUnitGetStub(member);
				/* FAKEMATCH(dead): unit and the picked item read through it. */
				UNUSED s32 pickedItem = unit->inventory[slot];

				discardQuantity = gmInventoryGetQuantityStub(member, slot);
			}
			/* A party member who already carries INVENTORY_STACK_MAX of the new
			 * item */
			/* cannot take it: say so and ask again. */
#if !(TBS_PATCH_1 && !TBS_PATCH_2)
			if(gmInventoryCountItemStub(member, item) >= INVENTORY_STACK_MAX) {
				mnMessageArgumentPushStub(member, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub(MsgWhatWillYouDrop + 7,
									   FIELD_MESSAGE_ZOOM);
				goto retry;
			}
#endif
			for(discardIndex = 0; discardIndex < discardQuantity;
				discardIndex++)
				gmInventoryDiscardStub(member, slot);
			owner = gmPartyInventoryAddStub(item);
			sySndPlayStub(MUS_TREASURE_FOUND);
			if(owner == MainRoutineStateWork.leaderActorId) {
				fdItemGiftShowGotItem(
					item, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
			}
			else {
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
				mnFieldMessageShowStub(MsgGaveItemToMember,
									   FIELD_MESSAGE_ZOOM |
										   FIELD_MESSAGE_WAITING_FOR_SOUND);
			}
			state->msg = savedMessage;
			return owner;
		}
	}
	else {
		/* There was room: "Isaac got <item>.", and "Isaac gave <item> to */
		/* <member>." when someone other than the leader carries it. */
		sySndPlayStub(MUS_TREASURE_FOUND);
		fdItemGiftShowGotItem(item, FIELD_MESSAGE_ZOOM |
										FIELD_MESSAGE_WAITING_FOR_SOUND);
		if(owner != MainRoutineStateWork.leaderActorId) {
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
			mnFieldMessageShowStub(GIFT_RECEIVED_MESSAGE,
								   GIFT_RECEIVED_MESSAGE_MODE);
		}
		state->msg = savedMessage;
	}
	return owner;
}

/* Gives item to one party member. Returns unit, or -1 if its bag is full. */
s32 fdInventoryTryAddAndReturnMember(s32 item, s32 unused, s32 unit) {
	if(gmInventoryAddItemStub(unit, item) >= 0)
		return unit;
	return -1;
}

/* An empty function; nothing in The Broken Seal calls it. */
void fdInventoryNoOpCallback(void) {}

/* An empty function; nothing in The Broken Seal calls it. */
void fdInventoryReservedNoOp(void) {}

/* Waits, at most 90 frames, for an actor's animation to stop being animation:
 */
/* the actor has finished a one-off animation such as a nod. */
void fdActorWaitForAnimationEnd(s32 actorId, s32 animation) {
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		s32 frames = 0;
		u8 *animationNumber = &actor->spr->lastNo;

		for(; frames < 90; frames++) {
			sySynch(1);
			if(animation != *animationNumber) {
				break;
			}
		}
	}
}

/* The last tile row the message windows may reach with a yes-or-no question */
/* still asked below them. */
#define YES_NO_BELOW_LAST_ROW 15

/* Once the message on screen has been read, asks yes or no below it, or above
 */
/* it when the message windows reach past YES_NO_BELOW_LAST_ROW, unless */
/* forceBottom is set; the actor then nods for yes or shakes its head for no. */
/* Returns 0 for yes. */
s32 fdEventAskYesNoWithActorAnimation(s32 actorId, s32 forceBottom) {
	FIELD_STATE *state = FieldStateWork;
	s32 sprite = scFindActorPlacement(state->speaker)->spr;
	WINDOW *window = (WINDOW *)state->msgWin;
	WINDOW *side = (WINDOW *)state->sideWin;
	s32 useBottom = 1;
	s32 answer;

	/* Wait for every button to be released and the message to finish. */
	while(KeysPressed != 0)
		sySynch(1);

	while(mnMessageAllTypedStub() == 0)
		sySynch(1);

	sySynch(3);

	/* The question goes above the message windows when they reach too low, so
	 * it */
	/* does not cover them. */
	if(forceBottom == 0) {
		s32 bottom = window->y + window->height;

		if(side != 0) {
			s32 sideBottom = side->y + side->height;
			if(bottom < sideBottom)
				bottom = sideBottom;
		}

		if(bottom > YES_NO_BELOW_LAST_ROW)
			useBottom = 0;
	}

	answer =
		mnYesNoMenuRunStub(useBottom, state->choiceMsg, state->choiceWidth, 0);
	if(answer != 0) {
		fdActorSetAnimationById(actorId, ANIM_SHAKE_HEAD);
		mnPortraitCloseWindowStub(sprite);
		mnMessageCloseFinishedWindowsStub();
		fdActorWaitForAnimationEnd(actorId, ANIM_SHAKE_HEAD);
	}
	else {
		fdActorSetAnimationById(actorId, ANIM_NOD);
		mnPortraitCloseWindowStub(sprite);
		mnMessageCloseFinishedWindowsStub();
		fdActorWaitForAnimationEnd(actorId, ANIM_NOD);
	}

	return answer;
}

/* Asks yes or no at the top of the screen, or at the bottom with atBottom set.
 */
/* Returns 0 for yes, 1 for no. */
s32 fdYesNoAsk(s32 atBottom) { return mnYesNoMenuRunStub(atBottom, 0, 0, 0); }

/* Teaches unit the Psynergy ability and says so with its jingle, waiting for */
/* the player to read it. */
void fdPartyLearnAbility(s32 unit, s32 ability) {
	/* gmUnitLearnPsynergyStub is declared without parameters: it learns this */
	/* function's own unit and ability, passed on unchanged. */
	gmUnitLearnPsynergyStub();
	sySndPlayStub(MUS_TREASURE_FOUND);
	mnMessageArgumentPushStub(unit, MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
	mnFieldMessageShowStub(MsgAbilityLearned,
						   FIELD_MESSAGE_ZOOM |
							   FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/* Starts the event's screen transition, as FIELD_STATE.transition and */
/* transitionFrm give it. */
void fdEventStartScrTransition(void) {
	FIELD_STATE *fieldState = FieldStateWork;

	fdScrTransitionStart(fieldState->transition, fieldState->transitionFrm);
	fieldState->transitionActive = 1;
}

/* Finishes the event's screen transition. */
void fdEventFinishScrTransition(void) {
	FIELD_STATE *fieldState = FieldStateWork;

	fdScrTransitionFinish(fieldState->transition, fieldState->transitionFrm);
	fieldState->transitionActive = 0;
}

/* Waits as long as the event's screen transition runs. */
void fdEventWaitTransitionFrames(void) {
	sySynch(FieldStateWork->transitionFrm);
}

/* Makes the event leave for the given entrance of the given scene: raises the
 */
/* exit request EXIT_KEEP_DESTINATION and writes MainRoutineStateWork.next and
 */
/* entrance. */
void fdEventSetDestination(s32 sceneId, s32 entrance) {
	FieldStateWork->exit = EXIT_KEEP_DESTINATION;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
}

/* Makes the event leave and records scene and entrance as the restart */
/* point. */
void fdEventSetRestartPoint(u16 sceneId, u16 entrance) {
	FIELD_STATE *fieldState = FieldStateWork;
	fieldState->exit = EXIT_KEEP_DESTINATION;
	MainRoutineStateWork.restartScene = sceneId;
	MainRoutineStateWork.restartEntrance = entrance;
}

/* Sets the field's exit request, which ends the scene: an exit kind, or */
/* EXIT_KEEP_DESTINATION for MainRoutineStateWork's next and entrance. */
void fdEventSetExitRequest(u16 request) {
	FIELD_STATE *fieldState = FieldStateWork;
	fieldState->exit = request;
}

#undef GIFT_RECEIVED_MESSAGE
#undef GIFT_RECEIVED_MESSAGE_MODE
#undef YES_NO_BELOW_LAST_ROW
