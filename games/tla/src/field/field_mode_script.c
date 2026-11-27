/* Script commands: waits, received items, questions and destinations. Part of
   field_mode.c. */

/* After a drop, "Felix gave <item> to <member>." shows zooming open alone in
   German, waiting for the jingle as well in Japanese and English, and the
   French, Spanish and Italian editions drop the message. */
#if GIFT_TO_MEMBER_AFTER_DROP_WAITS_FOR_BUTTON
#define GIFT_RECEIVED_MESSAGE_MODE FIELD_MESSAGE_ZOOM
#else
#define GIFT_RECEIVED_MESSAGE_MODE \
	(FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND)
#endif

/* Debug only: while debug mode is on, R makes every script wait (fdScriptWait)
   end at once and L restores normal waits. */
void fdScriptUpdateDebugSkip(void)
{
	FIELD_STATE *field = FieldStateWork;

	if(DebugMode != 0) {
		if(PerWork.push & KEY_L) {
			field->fastForward = 0;
		}
		if(PerWork.push & KEY_R) {
			field->fastForward = -1;
		}
	}
}

#include "field/script_wait.c"

/* Stops the leader where it stands: no horizontal velocity, the normal walking
   speed (1.0) and acceleration (0.5), no destination, and the standing pose, or
   ANIM_LEADER_CLING while the leader is on a climbable wall. */
void fdLeaderStopMoving(void)
{
	ACTOR *actor = fdActorLookup(fdPartyGetLeaderActor());
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->velX = 0;
	actor->velZ = 0;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS)
		fdActorSelectAnimationStub(actor, ANIM_LEADER_CLING);
	else
		fdActorSelectAnimationStub(actor, ANIM_STAND);
}

/* Begins a script: closes any area name window, stops the leader, ends the
   lasting Psynergy if a cue is running, resets the message and timing state,
   and starts the debug fast-forward event with the leader as the message
   speaker. */
void fdScriptStart(void)
{
	FIELD_STATE *field = RamHeapSlots->fieldState;
	mnAreaNameWindowHideStub();
	fdLeaderStopMoving();
	if(field->busy != 0)
		fdScriptEndAllLastingPsynergy();
	field->choiceMsg = 0;
	field->choiceWidth = 0;
	field->choiceSelection = 0;
	field->transitionFrm = MAP_TRANSITION_TICKS;
	field->fastForward = 0;
	field->unusedAfterMsg = -1;
	field->unusedBeforeCamera[0] = -1;
	field->unusedBeforeCamera[1] = -1;
	sySetEvent((Sint32)fdScriptUpdateDebugSkip, EVENT_PASS_TICK);
	/* XXX (no evidence): what flag 0x132 means; every script start in both
	   games clears it, and nothing in either game's code, tables or cleared
	   ranges of flags sets or reads it. */
	syGameFlagClear(0x132);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
		field->speaker = party->leaderActorId;
		field->msgWin = 0;
	}
}

/* Ends a script: stops the debug fast-forward event, gives the camera back to
   the leader (or to the first placed actor, the ship, while
   MainRoutineStateWork.ownerOverride is set) and refreshes the Lure Cap's flag
   (gmFlagRefreshLureCap). */
void fdScriptEnd(void)
{
	syCloseEvent((Uint32)fdScriptUpdateDebugSkip);
	{
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
		if(party->ownerOverride == 0)
			fdCameraFollowActor(party->leaderActorId, 1);
		else
			fdCameraFollowActor(ACTOR_FIRST_PLACED, 1);
		gmFlagRefreshLureCapStub();
	}
}

/* An empty function nothing calls. */
UNUSED void fdScriptNoOperationCallback(void)
{
}

/* An empty hook; it does nothing in the released game. Only
   fdScriptResetLeaderAndWait calls it. */
void fdScriptNoOperationHook(void)
{
}

/* Rebuilds the leader's actor from its placement row (fdActorResetForLeader),
   waits a tick and returns the leader's actor. */
ACTOR *fdScriptResetLeaderAndWait(Sint32 tbl)
{
	fdScriptNoOperationHook();
	fdActorResetForLeader(tbl);
	sySynch(1);
	return fdActorLookup(fdPartyGetLeaderActor());
}

/* Spawns the actors of an actor placement table after the last actor in use. */
void fdScriptSpawnActorsAfterLast(Sint32 tbl)
{
	fdScriptSpawnActorTable((ACTOR_PLACE *)tbl, fdActorFindLastActiveId());
}

/* Replaces the scene's cue table with scripts. */
void fdScriptSetSceneCues(Sint32 events)
{
	FieldStateWork->cues = events;
}

/* Adds unit to the party. With announce nonzero it also shows `<name> joined
   your party.` (mnPartyShowJoinedMessage, which receives announce too). */
void fdScriptAddPartyMember(Sint32 unit, Sint32 announce)
{
	/* DEAD: the unit looked up and never used; the ROM keeps the call. */
	syUnitGet(unit);
	gmPartyAddMemberStub(unit);
	if(announce != 0) {
		mnPartyShowJoinedMessageStub(unit, announce);
	}
}

/* Adds two units to the party and announces them together ("<a> and <b> joined
   your party.", mnPartyShowPairJoinedMessage). */
void fdScriptAddTwoPartyMembers(Sint32 first, Sint32 second)
{
	gmPartyAddMemberStub(first);
	gmPartyAddMemberStub(second);
	mnPartyShowPairJoinedMessageStub(first, second);
}

/* What fdPartyCheckShortcut finds wrong with a shortcut. */
typedef enum SHORTCUT_PROBLEM {
	SHORTCUT_USABLE = 0,
	SHORTCUT_NO_SUCH_MEMBER = -1,
	SHORTCUT_MEMBER_AWAY = -2, /* not in the party */
	SHORTCUT_NOT_LEARNED = -3  /* the member does not know the Psynergy */
} SHORTCUT_PROBLEM;

/* Checks an L or R button shortcut, packed as the party member in bits 10 to 13
   and the Psynergy in bits 0 to 9. */
Sint32 fdPartyCheckShortcut(Uint32 shortcut)
{
	Sint32 member = ACT_UNIT(shortcut);
	Sint32 ability = ABILITY_ID(shortcut);

	if(member > ALLY_MAX)
		return SHORTCUT_NO_SUCH_MEMBER;
	if(syGameFlagGet(FLAG_IN_PARTY(member)) == 0)
		return SHORTCUT_MEMBER_AWAY;
	if(gmUnitKnowsPsynergyStub(member, ability) == 0)
		return SHORTCUT_NOT_LEARNED;
	return SHORTCUT_USABLE;
}

/* Clears each button shortcut whose Psynergy can no longer be cast: zeroes
   MainRoutineStateWork.firstShortcut or secondShortcut. */
void fdPartyClearUnusableShortcuts(void)
{
	if(fdPartyCheckShortcut(MainRoutineStateWork.firstShortcut) !=
		SHORTCUT_USABLE)
		MainRoutineStateWork.firstShortcut = 0;
	if(fdPartyCheckShortcut(MainRoutineStateWork.secondShortcut) !=
		SHORTCUT_USABLE)
		MainRoutineStateWork.secondShortcut = 0;
}

/* Takes unit out of the party, fully restoring its HP and PP and curing its
   poison. */
void fdPartyRemoveMemberRestored(Sint32 unit)
{
	UNIT  *vitals;
	Sint32 partyIdx;
	Sint32 partyCnt;
	Sint32 survivingMembers;

	gmPartyRemoveMemberStub(unit);
	fdPartyClearUnusableShortcuts();
	vitals = syUnitGet(unit);
	vitals->hp = vitals->hpMax;
	vitals->pp = vitals->ppMax;
	UNIT_UPDATE_GAUGES(vitals);
	vitals->poison = 0;
	/* Count the party members who remain and still have HP. */
	survivingMembers = 0;
	partyCnt = gmPartyCountMembersStub();
	for(partyIdx = 0; partyIdx < partyCnt; partyIdx++) {
		vitals = syUnitGet(MainRoutineStateWork.party[partyIdx]);
		if(vitals->hp != 0)
			survivingMembers++;
	}
	if(survivingMembers == 0) {
		vitals = syUnitGet(MainRoutineStateWork.leaderActorId);
		vitals->hp = 1;
		UNIT_UPDATE_GAUGES(vitals);
	}
}

/* Checks whether the party can carry one more of item; when it cannot, says so
   and returns -1, else 0. */
Sint32 fdPartyCheckItemLimit(Sint32 item)
{
	/* DEAD: the count never used; the ROM keeps the call. */
	gmPartyCountMembersStub();
	if(gmPartyInventoryFindRoomStub(item) < 0) {
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Felix's party cannot carry any more <item>s..." */
		mnFieldMessageShowStub((Sint32)MsgPartyCannotCarry, FIELD_MESSAGE_ZOOM);
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Felix threw the <item> as far away as possible." */
		mnFieldMessageShowStub(
			(Sint32)MsgPartyCannotCarry + 1, FIELD_MESSAGE_ZOOM);
		return -1;
	}
	return 0;
}

/* The script command "give item": gives item to the first party member with
   room for it and says so; with no room, the player chooses what to drop. */
Sint32 fdPartyInventoryGiveItem(Sint32 item, Sint32 unused)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint16		 saved = state->msg;
	Sint32		 owner;
	Sint32		 result;
	Sint32		 cnt;
	Sint32		 i;
	Sint32		 member;
	Sint32		 slot;

	owner = gmPartyInventoryAddStub(item);
	if(owner == -1) {
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Felix got <item>." */
		mnFieldMessageShowStub((Sint32)&MsgItemReceived, FIELD_MESSAGE_ZOOM);
		/* "But you can't carry any more items." */
		mnFieldMessageShowStub((Sint32)&MsgItemCannotCarry, FIELD_MESSAGE_ZOOM);
	retry: {
		/* "What will you drop?" */
		mnFieldMessageShowStub((Sint32)&MsgItemDropPrompt, FIELD_MESSAGE_ZOOM);
		result = mnItemPickerRunToDropStub(&member, &slot);
		if(result == -1) {
			if(gmItemGetDirectStub(item)->flg & ITEM_INDISPENSABLE) {
				mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
				/* "That <item>'s valuable! You must get it!" */
				mnFieldMessageShowStub(
					(Sint32)&MsgItemDropPrompt + 4, FIELD_MESSAGE_ZOOM);
				goto retry;
			}
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			/* "Drop the <item>?" */
			mnFieldMessageShowStub((Sint32)&MsgItemDropPrompt + 1,
				FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
			result = fdYesNoAsk(1);
			mnMessageCloseFinishedWindowsStub();
			if(result != 0)
				goto retry;
			gmItemAdjustRareItemStockStub(item, 1);
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			/* "Felix drops the <item>." */
			mnFieldMessageShowStub(
				(Sint32)&MsgItemDropPrompt + 2, FIELD_MESSAGE_ZOOM);
			goto done;
		}
		/* A carried item was picked: drop it (the whole stack) and take the new
		   one. A party member who already carries INVENTORY_STACK_MAX of the
		   new item cannot take it: say so and ask again. */
		syUnitGet(member);
		cnt = gmInventoryGetQuantityStub(member, slot);
		if(gmInventoryCountItemStub(member, item) >= INVENTORY_STACK_MAX) {
			mnMessageArgumentPushStub(member, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			/* "Empty": the line every edition's text has here, the word for an
			   empty inventory slot, though the member and item pushed suit a
			   sentence such as MsgPartyCannotCarry's. */
			mnFieldMessageShowStub(
				(Sint32)&MsgItemDropPrompt + 7, FIELD_MESSAGE_ZOOM);
			goto retry;
		}
		for(i = 0; i < cnt; i++)
			gmInventoryDiscardStub(member, slot);
		gmUnitRefreshPsynergyStub(member);
		gmUnitCalcStub(member);
		owner = gmPartyInventoryAddStub(item);
		sySndPlayStub(MUS_TREASURE_FOUND);
		if(owner == MainRoutineStateWork.leaderActorId) {
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub((Sint32)&MsgItemReceived,
				FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
		} else {
			mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
			mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
#if !GIFT_TO_MEMBER_AFTER_DROP_NOT_SHOWN
			mnFieldMessageShowStub(
				(Sint32)&MsgItemGiven, GIFT_RECEIVED_MESSAGE_MODE);
#endif
		}
	}
	done:
		state->msg = saved;
		return owner;
	}
	/* There was room: "Felix got <item>.", and "Felix gave <item> to <member>."
	   when someone other than the leader carries it. */
	sySndPlayStub(MUS_TREASURE_FOUND);
	mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
	mnFieldMessageShowStub((Sint32)&MsgItemReceived,
		FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
	if(owner != MainRoutineStateWork.leaderActorId) {
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPushStub(owner, MESSAGE_ARGUMENT_ACTOR);
		mnFieldMessageShowStub(
			(Sint32)&MsgItemReceived + 1, FIELD_MESSAGE_ZOOM);
	}
	state->msg = saved;
	return owner;
}

#include "field/inventory_try_add_and_return_member.c"

/* An empty function nothing calls. */
UNUSED void fdInventoryNoOperationCallback(void)
{
}

/* An empty function nothing calls. */
UNUSED void fdInventoryReservedNoOperation(void)
{
}

/* Waits, at most 90 ticks, for an actor's animation to stop being animation:
   the actor has finished a one-off animation such as a nod. */
void fdActorWaitForAnimationEnd(Sint32 actorId, Sint32 anm)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != 0 && actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		Sint32 ticks;
		SPR	  *spr = actor->spr;

		for(ticks = 0; ticks <= 89; ticks++) {
			sySynch(1);
			if(anm != spr->lastNo) {
				break;
			}
		}
	}
}

/* The last tile row the message windows may reach with a yes-or-no question
   still asked below them. */
#define YES_NO_BELOW_LAST_ROW 15

/* The script command "choose yes or no": once the message on screen has been
   read, asks yes or no below it, or above it when the message windows reach
   past YES_NO_BELOW_LAST_ROW (unless forceBottom is set). The speaker nods
   (ANIM_NOD) at yes and shakes its head (ANIM_SHAKE_HEAD) at no; at the fastest
   message speed neither is waited for. */
Sint32 fdScriptAskYesNoWithActorAnimation(Sint32 actor, Sint32 forceBottom)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 spr = *(Sint16 *)fdFindActorPlacement(state->speaker);
	WINDOW		*win = state->msgWin;
	WINDOW		*side = state->sideWin;
	Bool		 useBottom = TRUE;
	Sint32		 answer;

	while(mnMessageAllTypedStub() == 0)
		sySynch(1);

	sySynch((MESSAGE_SPEED_FAST - MainRoutineStateWork.msgSpeed) * 3);

	if(forceBottom == 0) {
		Sint32 bottom = win->y + win->height;

		if(side != NULL) {
			Sint32 sideBottom = side->y + side->height;
			if(bottom < sideBottom)
				bottom = sideBottom;
		}

		if(bottom > YES_NO_BELOW_LAST_ROW)
			useBottom = FALSE;
	}

	answer = mnYesNoMenuRunStub(useBottom, state->choiceMsg, state->choiceWidth,
		state->choiceSelection);
	if(answer != 0) {
		fdActorSetAnimationById(actor, ANIM_SHAKE_HEAD);
		mnPortraitCloseWindowStub(spr);
		mnMessageCloseFinishedWindowsStub();
		if(MainRoutineStateWork.msgSpeed != MESSAGE_SPEED_FAST)
			fdActorWaitForAnimationEnd(actor, ANIM_SHAKE_HEAD);
	} else {
		fdActorSetAnimationById(actor, ANIM_NOD);
		mnPortraitCloseWindowStub(spr);
		mnMessageCloseFinishedWindowsStub();
		if(MainRoutineStateWork.msgSpeed != MESSAGE_SPEED_FAST)
			fdActorWaitForAnimationEnd(actor, ANIM_NOD);
	}

	return answer;
}

#include "field/yes_no_ask.c"

/* Gives the party item from actor: with no room it shows the item, says it was
   found and left behind and returns -1; otherwise it adds the item, sets flag
   (unless -1) and returns 0. */
Sint32 fdScriptPickUpItem(Sint32 actor, Sint32 item, Sint32 flg)
{
	if(gmPartyInventoryFindRoomStub(item) < 0) {
		fdActorGetById(actor)->scaleX = 0;
		fdItemDispShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		mnMessageArgumentPushStub(item, MESSAGE_ARGUMENT_ITEM);
		/* "Felix found <item>." */
		mnFieldMessageShowStub((Sint32)&MsgFoundItem, FIELD_MESSAGE_ZOOM);
		/* "But Felix's party can't carry any more, so they left it behind." */
		mnFieldMessageShowStub((Sint32)&MsgFoundItem + 4, FIELD_MESSAGE_ZOOM);
		fdActorGetById(actor)->scaleX = toFIXED(1);
		fdActorFaceActorById(MainRoutineStateWork.leaderActorId, actor, 0);
		fdScriptWait(20);
		return -1;
	}
	fdActorSetHorizontalPosWithTerrain(actor, 0, 0);
	fdItemDispShowItem(item, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	fdPartyInventoryGiveItem(item, 0);
	if(flg != -1)
		syGameFlagSet(flg);
	return 0;
}

/* Teaches unit the Psynergy ability and says so with its jingle, waiting for
   the player to read it. */
void fdPartyLearnAbility(Sint32 unit, Sint32 ability)
{
	gmUnitLearnPsynergyStub(unit, ability);
	sySndPlayStub(MUS_TREASURE_FOUND);
	mnMessageArgumentPushStub(unit, MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPushStub(ability, MESSAGE_ARGUMENT_ABILITY);
	/* "<member> learned <ability>." */
	mnFieldMessageShowStub(MsgAbilityLearned,
		FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
}

/* The script command "open screen": starts the script's screen transition and
   sets transitionActive. */
void fdScriptStartScreenTransition(void)
{
	FIELD_STATE *field = FieldStateWork;

	fdScreenTransitionStart(field->transition, field->transitionFrm);
	field->transitionActive = 1;
}

/* The script command "close screen": finishes the script's screen transition
   and clears transitionActive. */
void fdScriptFinishScreenTransition(void)
{
	FIELD_STATE *field = FieldStateWork;

	fdScreenTransitionFinish(field->transition, field->transitionFrm);
	field->transitionActive = 0;
}

#include "field/script_wait_and_destination.c"

/* Makes the script leave and records sceneId and entrance as the restart
   point. */
void fdScriptSetRestartPoint(Sint32 sceneId, Sint32 entrance)
{
	FIELD_STATE		   *field = RamHeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party;

	field->exit = EXIT_KEEP_DESTINATION;
	party = &MainRoutineStateWork;
	party->restartScene = sceneId;
	party->restartEntrance = entrance;
}

/* The script command "request exit": sets the field's exit request
   (EXIT_KEEP_DESTINATION leaves for MainRoutineStateWork's next and
   entrance). */
void fdScriptSetExitRequest(Uint16 exit)
{
	FIELD_STATE *field = FieldStateWork;
	field->exit = exit;
}

#undef GIFT_RECEIVED_MESSAGE_MODE
#undef YES_NO_BELOW_LAST_ROW
