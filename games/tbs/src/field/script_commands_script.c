/* Script commands: waits, received items, questions and destinations. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* When a gift goes to another party member, `<leader> gave <item> to <member>.`
   follows `<leader> got <item>.`; it is MsgGaveItemToMember, the next message,
   in every edition. */
#define GIFT_RECEIVED_MESSAGE GERMAN_OR(MsgGaveItemToMember, (MsgGotItem + 1))

/* Japanese and English show it zooming open and waiting for the jingle; the
   German, Spanish, French and Italian editions only zoom it open. */
#if GIFT_TO_MEMBER_MESSAGE_WAITS_FOR_BUTTON
#define GIFT_RECEIVED_MESSAGE_MODE FIELD_MESSAGE_ZOOM
#else
#define GIFT_RECEIVED_MESSAGE_MODE \
	(FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND)
#endif

/* Shows the message for an item the party has just received. */
static inline void fdItemGiftShowGotItem(Sint32 item, Sint32 flags)
{
#if LANGUAGE == LANGUAGE_GERMAN
	if(item == ITEM_BLACK_ORB) {
		mnFieldMessageShowStub(MsgGotBlackOrb, flags);
		return;
	}
#endif
	mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
	mnFieldMessageShowStub(MsgGotItem, flags);
}

/* Debug only: while debug mode is on, R makes every script wait (fdScriptWait)
   end at once and L restores normal waits. */
void fdScriptUpdateDebugSkip(void)
{
	FIELD_STATE *fieldState = FieldStateWork;

	if(DebugMode != 0) {
		if(PerPush & KEY_L) {
			fieldState->fastForward = 0;
		}
		if(PerPush & KEY_R) {
			fieldState->fastForward = -1;
		}
	}
}

/* Waits the requested ticks through sySynch(), unless debug fast-forward
   (fdScriptUpdateDebugSkip) is on: the wait scripts use. */
void fdScriptWait(Sint32 ticks)
{
	if(FieldStateWork->fastForward == 0 && ticks != 0) {
		sySynch(ticks);
	}
}

/* Stops the leader where it stands: no horizontal velocity, the normal walking
   speed (1.0) and acceleration (0.5), no destination, and the standing pose, or
   the clinging pose while the leader is on a climbable wall. */
void fdLeaderStopMoving(void)
{
	ACTOR *actor;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->targetX = ACTOR_NO_TARGET;
	actor->targetZ = ACTOR_NO_TARGET;
	actor->velocityX = 0;
	actor->velocityZ = 0;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_CLIMB) {
		fdActorSelectAnimationStub(actor, ANIM_LEADER_CLING);
		return;
	}
	fdActorSelectAnimationStub(actor, ANIM_STAND);
}

/* Begins a script: closes any area name window, stops the leader, resets the
   message and timing state, and starts the debug fast-forward event with the
   leader as the message speaker. */
void fdScriptStart(void)
{
	FIELD_STATE *fieldState = FieldStateWork;

	mnAreaNameWindowHideStub();
	fdLeaderStopMoving();
	/* A raised or step cue still marked running is ended first. */
	if((Sint16)fieldState->busy != 0) {
		fdEndBusyAndReveal();
	}
	/* Every script starts with no name window and normal waits. */
	fieldState->choiceMsg = 0;
	fieldState->choiceWidth = 0;
	fieldState->transitionFrame = MAP_TRANSITION_TICKS;
	fieldState->fastForward = 0;
	fieldState->unusedAfterMsg = -1;
	fieldState->unusedBeforeCamera[0] = -1;
	fieldState->unusedBeforeCamera[1] = -1;
	sySetEvent(fdScriptUpdateDebugSkip, EVENT_PASS_TICK);
	/* XXX (no evidence): what flag 0x132 means; every script start in both
	   games clears it, nothing in either game's source names it or reads it by
	   its number, and no other code of tbs-en's build loads that number for a
	   flag. */
	gmFlagClearStub(0x132);
	fieldState->speaker = MainRoutineStateWork.leaderActorId;
	fieldState->msgWindow = 0;
}

/* Ends a script: stops the debug fast-forward event, gives the camera back to
   the leader and refreshes FLAG_LURE_CAP (gmFlagRefreshLureCap). */
void fdScriptEnd(void)
{
	syCloseEvent(fdScriptUpdateDebugSkip);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	gmFlagRefreshLureCapStub();
}

UNUSED void fdScriptNoOperationUpdateA(void)
{
}

/* An empty hook; it does nothing in the released game. Only
   fdScriptRunActorUpdateAndWait calls it. */
void fdScriptNoOperationUpdate(void)
{
}

/* Respawns the scene's actors from an actor placement table and waits one tick
   for them to appear. */
void fdScriptRunActorUpdateAndWait(ACTOR_PLACE *table)
{
	fdScriptNoOperationUpdate();
	fdActorResetForLeader(table);
	sySynch(1);
	fdActorLookup(MainRoutineStateWork.leaderActorId);
}

/* Spawns the actors of an actor placement table after the last actor in use. */
void fdScriptAppendActorPlacements(ACTOR_PLACE *table)
{
	fdScriptSpawnActorPlacements(table, fdActorFindLastActiveId());
}

/* Sets the scene's cue table, which the field checks as the leader moves: Altin
   Peak's script calls it. */
void fdScriptSetCues(const CUE *cues)
{
	FIELD_STATE *state = FieldStateWork;

	state->cues = cues;
}

/* Adds unit to the party. With announce nonzero it also shows `<name> joined
   your party.` (mnPartyShowJoinedMessage, which receives announce too). */
void fdScriptAddPartyMember(Sint32 unit, Sint32 announce)
{
	gmUnitGetStub(unit);
	gmPartyAddMemberStub(unit);
	if(announce != 0) {
		mnPartyShowJoinedMessageStub(unit, announce);
	}
}

/* Adds two units to the party and announces them together. Vale's script calls
   it twice. */
void fdScriptAddPartyMemberPair(Sint32 first, Sint32 second)
{
	gmPartyAddMemberStub(first);
	gmPartyAddMemberStub(second);
	mnPartyShowPairJoinedMessageStub(first, second);
}

/* Checks an L or R button shortcut, packed as a field Psynergy use is: the
   party member (ACT_UNIT) and the Psynergy (ABILITY_ID). Returns 0 when it can
   be cast, -1 for no such character, -2 when they are out of the party and -3
   when they do not know the Psynergy. */
Sint32 fdPartyCheckShortcut(Uint32 shortcut)
{
	Sint32 member = ACT_UNIT(shortcut);
	Sint32 ability = ABILITY_ID(shortcut);

	if(member > ALLY_MAX)
		return -1;
	if(gmFlagGetStub(FLAG_IN_PARTY(member)) == 0)
		return -2;
	if(gmUnitKnowsPsynergyStub(member, ability) == 0)
		return -3;
	return 0;
}

/* Clears each button shortcut whose Psynergy can no longer be cast: zeroes
   MainRoutineStateWork.firstShortcut or secondShortcut. */
void fdPartyClearUnusableShortcuts(void)
{
	if(fdPartyCheckShortcut(MainRoutineStateWork.firstShortcut) != 0)
		MainRoutineStateWork.firstShortcut = 0;
	if(fdPartyCheckShortcut(MainRoutineStateWork.secondShortcut) != 0)
		MainRoutineStateWork.secondShortcut = 0;
}

/* Takes unit out of the party, fully restoring its HP and PP and curing its
   poison. */
void fdPartyRemoveMemberRestored(Sint32 unit)
{
	UNIT  *vitals;
	Sint32 partyIndex;
	Sint32 partyCount;
	Sint32 survivingMembers;

	gmPartyRemoveMemberStub(unit);
	fdPartyClearUnusableShortcuts();
	vitals = gmUnitGetStub(unit);
	vitals->hp = vitals->hpMax;
	vitals->pp = vitals->ppMax;
	UNIT_UPDATE_GAUGES(vitals);
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
		UNIT_UPDATE_GAUGES(vitals);
	}
}

/* Checks whether the party can carry one more of item: each party member
   carries at most INVENTORY_STACK_MAX of one item. */
Sint32 fdPartyCheckItemLimit(Sint32 item)
{
	Sint32 partyCount;
	Sint32 heldQuantity = 0;
	Sint32 partyIndex;

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

/* Gives item to the first party member with room for it and says so; when every
   bag is full the player picks something to drop. Returns who got it, or -1. */
Sint32 fdPartyInventoryGiveItem(Sint32 item, Sint32 unused)
{
	FIELD_STATE *state;
	Sint32		 savedMsg;
	Sint32		 owner;
	Sint32		 result;
	Sint32		 discardQuantity;
	Sint32		 discardIndex;
	Sint32		 member;
	Sint32		 slot;

	state = FieldStateWork;
	/* The messages below move the script's message on; every return puts it
	   back, so the script continues where it was. */
	savedMsg = state->msg;
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
			   ("That <item>'s valuable! You must get it!"). */
			if(gmItemGetDirectStub(item)->flags & ITEM_INDISPENSABLE) {
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub(
					MsgWhatWillYouDrop + 4, FIELD_MESSAGE_ZOOM);
				goto retry;
			}
			/* "Drop the <item>?" */
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(MsgWhatWillYouDrop + 1,
				FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
			result = fdYesNoAsk(1);
			mnMessageCloseFinishedWindowsStub();
			if(result != 0)
				goto retry;
			/* "Isaac drops the <item>." */
			gmItemAdjustRareItemStockStub(item, 1);
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(MsgWhatWillYouDrop + 2, FIELD_MESSAGE_ZOOM);
			state->msg = savedMsg;
			return owner;
		} else {
			/* A carried item was picked: drop it (the whole stack) and take the
			   new one. */
			{
				UNIT *unit = gmUnitGetStub(member);
				/* FAKEMATCH(dead): unit and the picked item read through it. */
				UNUSED Sint32 pickedItem = unit->inventory[slot];

				discardQuantity = gmInventoryGetQuantityStub(member, slot);
			}
			/* A party member who already carries INVENTORY_STACK_MAX of the new
			   item cannot take it: say so and ask again. */
#if !GIFT_DROP_SKIPS_FULL_STACK_CHECK
			if(gmInventoryCountItemStub(member, item) >= INVENTORY_STACK_MAX) {
				mnMessageArgumentPushStub(member, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub(
					MsgWhatWillYouDrop + 7, FIELD_MESSAGE_ZOOM);
				goto retry;
			}
#endif
			for(discardIndex = 0; discardIndex < discardQuantity;
				discardIndex++)
				gmInventoryDiscardStub(member, slot);
			owner = gmPartyInventoryAddStub(item);
			sySoundPlayStub(MUS_TREASURE_FOUND);
			if(owner == MainRoutineStateWork.leaderActorId) {
				fdItemGiftShowGotItem(
					item, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
			} else {
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
				mnFieldMessageShowStub(MsgGaveItemToMember,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
			}
			state->msg = savedMsg;
			return owner;
		}
	} else {
		/* There was room: "Isaac got <item>.", and "Isaac gave <item> to
		   <member>." when someone other than the leader carries it. */
		sySoundPlayStub(MUS_TREASURE_FOUND);
		fdItemGiftShowGotItem(
			item, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
		if(owner != MainRoutineStateWork.leaderActorId) {
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
			mnFieldMessageShowStub(
				GIFT_RECEIVED_MESSAGE, GIFT_RECEIVED_MESSAGE_MODE);
		}
		state->msg = savedMsg;
	}
	return owner;
}

/* Gives item to one party member. Returns unit, or -1 if its bag is full. */
Sint32 fdInventoryTryAddAndReturnMember(Sint32 item, Sint32 unused, Sint32 unit)
{
	if(gmInventoryAddItemStub(unit, item) >= 0)
		return unit;
	return -1;
}

/* An empty function; nothing in The Broken Seal calls it. */
void fdInventoryNoOperationCallback(void)
{
}

/* An empty function; nothing in The Broken Seal calls it. */
void fdInventoryReservedNoOperation(void)
{
}

/* Waits, at most 90 ticks, for an actor's animation to stop being animation:
   the actor has finished a one-off animation such as a nod. */
void fdActorWaitForAnimationEnd(Sint32 actorId, Sint32 animation)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != 0 && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
		Sint32 ticks = 0;
		Uint8 *animationNumber = &actor->sprite->lastNo;

		for(; ticks < 90; ticks++) {
			sySynch(1);
			if(animation != *animationNumber) {
				break;
			}
		}
	}
}

/* The last tile row the message windows may reach with a yes-or-no question
   still asked below them. */
#define YES_NO_BELOW_LAST_ROW 15

/* Once the message on screen has been read, asks yes or no below it, or above
   it when the message windows reach past YES_NO_BELOW_LAST_ROW, unless
   forceBottom is set; the actor then nods for yes or shakes its head for no.
   Returns 0 for yes. */
Sint32 fdScriptAskYesNoWithActorAnimation(Sint32 actorId, Sint32 forceBottom)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 sprite = fdFindActorPlacement(state->speaker)->sprite;
	WINDOW		*window = (WINDOW *)state->msgWindow;
	WINDOW		*side = (WINDOW *)state->sideWindow;
	Bool		 useBottom = TRUE;
	Sint32		 answer;

	/* Wait for every button to be released and the message to finish. */
	while(PerPush != 0)
		sySynch(1);

	while(mnMessageAllTypedStub() == 0)
		sySynch(1);

	sySynch(3);

	/* The question goes above the message windows when they reach too low, so
	   it does not cover them. */
	if(forceBottom == 0) {
		Sint32 bottom = window->y + window->height;

		if(side != 0) {
			Sint32 sideBottom = side->y + side->height;
			if(bottom < sideBottom)
				bottom = sideBottom;
		}

		if(bottom > YES_NO_BELOW_LAST_ROW)
			useBottom = FALSE;
	}

	answer =
		mnYesNoMenuRunStub(useBottom, state->choiceMsg, state->choiceWidth, 0);
	if(answer != 0) {
		fdActorSetAnimationById(actorId, ANIM_SHAKE_HEAD);
		mnPortraitCloseWindowStub(sprite);
		mnMessageCloseFinishedWindowsStub();
		fdActorWaitForAnimationEnd(actorId, ANIM_SHAKE_HEAD);
	} else {
		fdActorSetAnimationById(actorId, ANIM_NOD);
		mnPortraitCloseWindowStub(sprite);
		mnMessageCloseFinishedWindowsStub();
		fdActorWaitForAnimationEnd(actorId, ANIM_NOD);
	}

	return answer;
}

/* Asks yes or no at the top of the screen, or at the bottom with atBottom set.
   Returns 0 for yes, 1 for no. */
Sint32 fdYesNoAsk(Sint32 atBottom)
{
	return mnYesNoMenuRunStub(atBottom, 0, 0, 0);
}

/* Teaches unit the Psynergy ability and says so with its jingle, waiting for
   the player to read it. */
void fdPartyLearnAbility(Sint32 unit, Sint32 ability)
{
	/* gmUnitLearnPsynergyStub is declared without parameters: it learns this
	   function's own unit and ability, passed on unchanged. */
	gmUnitLearnPsynergyStub();
	sySoundPlayStub(MUS_TREASURE_FOUND);
	mnMessageArgumentPushStub(unit, MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
	mnFieldMessageShowStub(MsgAbilityLearned,
		FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/* Starts the script's screen transition, as FIELD_STATE.transition and
   transitionFrame give it. */
void fdScriptStartScreenTransition(void)
{
	FIELD_STATE *fieldState = FieldStateWork;

	fdScreenTransitionStart(
		fieldState->transition, fieldState->transitionFrame);
	fieldState->transitionActive = 1;
}

/* Finishes the script's screen transition. */
void fdScriptFinishScreenTransition(void)
{
	FIELD_STATE *fieldState = FieldStateWork;

	fdScreenTransitionFinish(
		fieldState->transition, fieldState->transitionFrame);
	fieldState->transitionActive = 0;
}

/* Waits as long as the script's screen transition runs. */
void fdScriptWaitTransitionTicks(void)
{
	sySynch(FieldStateWork->transitionFrame);
}

/* Makes the script leave for the given entrance of the given scene: raises the
   exit request EXIT_KEEP_DESTINATION and writes MainRoutineStateWork.next and
   entrance. */
void fdScriptSetDestination(Sint32 sceneId, Sint32 entrance)
{
	FieldStateWork->exit = EXIT_KEEP_DESTINATION;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
}

/* Makes the script leave and records scene and entrance as the restart
   point. */
void fdScriptSetRestartPoint(Uint16 sceneId, Uint16 entrance)
{
	FIELD_STATE *fieldState = FieldStateWork;
	fieldState->exit = EXIT_KEEP_DESTINATION;
	MainRoutineStateWork.restartScene = sceneId;
	MainRoutineStateWork.restartEntrance = entrance;
}

/* Sets the field's exit request, which ends the scene: an exit kind, or
   EXIT_KEEP_DESTINATION for MainRoutineStateWork's next and entrance. */
void fdScriptSetExitRequest(Uint16 request)
{
	FIELD_STATE *fieldState = FieldStateWork;
	fieldState->exit = request;
}

#undef GIFT_RECEIVED_MESSAGE
#undef GIFT_RECEIVED_MESSAGE_MODE
#undef YES_NO_BELOW_LAST_ROW
