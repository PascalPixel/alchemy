/* Using an item on the field. Part of field_mode.c. */

#include "item_ids.h"

/* What fdScriptEndLastingPsynergy leaves running, as bits of its skip. */
typedef enum PSYNERGY_END_SKIP {
	END_SKIP_REVEAL = 1 << 0,
	END_SKIP_RING = 1 << 1, /* the ring of light (ABILITY_MAGNET) */
	END_SKIP_SAND = 1 << 2,
} PSYNERGY_END_SKIP;

/* Runs message as a script of its own, shown at actor. */
void fdScriptRunMessage(Sint32 actor, Sint32 message)
{
	fdScriptStart();
	fdScriptSetMessageId(message);
	fdScriptShowMessageAndWait(actor, 0);
	fdScriptEnd();
}

/* Ends every lasting Psynergy (fdScriptEndLastingPsynergy with none
   skipped). */
void fdScriptEndAllLastingPsynergy(void)
{
	fdScriptEndLastingPsynergy(0);
}

/* Ends the lasting Psynergy and runs their end scripts: Reveal, the ring of
   light and Sand (while the party is sand, MainRoutineStateWork.sandActive),
   each unless skip holds its bit (PSYNERGY_END_SKIP). */
void fdScriptEndLastingPsynergy(Sint32 skip)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->busy = 0;
	if(state->revealActive != 0 && !(skip & END_SKIP_REVEAL))
		fdRunPsynergyEndScripts(ABILITY_REVEAL | ACT_PACKED);
	if(state->ringActive != 0 && !(skip & END_SKIP_RING))
		fdRunPsynergyEndScripts(ABILITY_MAGNET | ACT_PACKED);
	if(MainRoutineStateWork.sandActive != 0 && !(skip & END_SKIP_SAND))
		fdRunPsynergyEndScripts(ABILITY_SAND | ACT_PACKED);
}

/* Returns the leader's actor id, or ACTOR_FIRST_PLACED while
   MainRoutineStateWork.ownerOverride is set. */
Sint32 fdPartyGetLeaderActor(void)
{
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	Sint32				actor;

	/* While the party sails, the first placed actor, the ship, stands for
	   it. */
	actor = ACTOR_FIRST_PLACED;
	if(state->ownerOverride == 0)
		actor = state->leaderActorId;
	return actor;
}

/* Returns 1 when itemId answers filter: the item itself, ITEM_FILTER_ANY any
   item, or ITEM_FILTER_HEAVY any weapon, armor, shield or helm; else 0. */
Bool fdItemMatchesFilter(Sint32 filter, Sint32 itemId)
{
	if(filter == itemId)
		return TRUE;
	if(filter == ITEM_FILTER_ANY)
		return TRUE;
	if(filter == ITEM_FILTER_HEAVY &&
		(Uint32)(gmItemGetDirectStub(itemId)->type - ITEM_TYPE_WEAPON) <=
			ITEM_TYPE_HELM - ITEM_TYPE_WEAPON)
		return TRUE;
	return FALSE;
}

/* Returns the scene's item cue (CUE_ITEM) for item that applies to what the
   leader faces, or NULL (item 0 matches any): its item or item filter takes
   item (fdItemMatchesFilter), and its target is the actor the leader can talk
   to (CUE_ITEM_ON_ACTOR) or the metatile ahead. */
const CUE *fdFindItemCue(Sint32 item)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	const CUE	*cue = state->cues;
	Sint32		 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;
	Sint32		 actorAhead = fdFindNearbyTalkTarget(fdPartyGetLeaderActor());
	Sint32		 tileAhead;

	item &= INVENTORY_ITEM_ID_MASK;
	tileAhead = fdGetCollisionAheadOfLeader();

	for(; cue->ctrl != -1; cue++) {
		Sint32 eventFacing = (Sint16)(Uint16)cue->subject & CUE_SUBJECT_FACING;
		Sint16 requiresFacing = (Uint16)cue->subject & CUE_SUBJECT_NEEDS_FACING;
		Sint32 eventTarget = (Uint16)cue->subject & CUE_SUBJECT_TARGET;

		if((cue->ctrl & CUE_KIND_MASK) == CUE_ITEM &&
			fdGameFlagIsConditionActive(cue->condition) != 0 &&
			(requiresFacing == 0 ||
				(Uint16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
					TRIG_TURN * 3 / 16 - 2)) {
			/* The item, as USE_ITEM packs it above the cue's kind. */
			Sint32 eventItem = (Uint16)((Sint32)cue->ctrl / 256);

			if(item == 0 || fdItemMatchesFilter(eventItem, item)) {
				if((cue->ctrl & CUE_ITEM_ON_ACTOR) != 0) {
					if(eventTarget == actorAhead)
						return cue;
				} else if(eventTarget == tileAhead) {
					return cue;
				}
			}
		}
	}
	return NULL;
}

/* Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
   character using it (bits 10 to 13; UNIT_NONE for whoever holds it in the most
   inventory slots); slot is its inventory slot. The scene's item cue for what
   the leader faces runs if there is one; otherwise an item with a field action
   casts it, Retreat asking first. Returns 0 when the scene's script ran or
   Retreat was declined, else -1. */
Sint32 fdUseItem(Sint32 packedItem, Sint32 slot)
{
	Sint32		 result;
	Sint32		 itemId;
	Sint32		 actorId;
	UNIT		*unit;
	const CUE	*cue;
	Sint32		 inventorySlot;
	Sint32		 partyIndex;
	Sint32		 mostMatchingSlots;
	Sint32		 matchingSlots;
	Sint32		 partyCount;
	FIELD_STATE *state;

	result = -1;
	itemId = ACT_ITEM_ID(packedItem);
	actorId = ACT_UNIT(packedItem);
	/* Count the user's slots holding the item; with no user given, the user is
	   whoever holds it in the most slots, the earlier on a tie. */
	{
		mostMatchingSlots = 0;
		partyCount = gmPartyCountMembersStub();

		if(actorId == UNIT_NONE) {
			actorId = 0;
			for(partyIndex = 0; partyIndex < partyCount; partyIndex++) {
				unit = syUnitGet(MainRoutineStateWork.party[partyIndex]);
				matchingSlots = 0;
				for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
					inventorySlot++) {
					if((unit->inventory[inventorySlot] &
						   INVENTORY_ITEM_ID_MASK) == itemId)
						matchingSlots++;
				}

				if(mostMatchingSlots < matchingSlots) {
					mostMatchingSlots = matchingSlots;
					actorId = MainRoutineStateWork.party[partyIndex];
				}
			}
		} else {
			unit = syUnitGet(actorId);
			for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
				inventorySlot++) {
				if((unit->inventory[inventorySlot] & INVENTORY_ITEM_ID_MASK) ==
					itemId)
					mostMatchingSlots++;
			}
		}

		if(mostMatchingSlots == 0) {
			/* "But nothing happens." */
			mnFieldMessageShowStub(
				(Sint32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
			return -1;
		}
	}

	cue = fdFindItemCue(itemId);
	if(cue != NULL && cue->value != 0) {
		/* The scene's script: a message, or a function. */
		syGameFlagClear(FLAG_ITEM_USED_UP);
		syGameFlagClear(FLAG_NOTHING_HAPPENS);
		if(!(cue->subject & CUE_SUBJECT_OWN_EFFECT)) {
			mnMessageArgumentPushStub(actorId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(
				(Sint32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
		}
		if((Sint32)cue->value < CUE_RESULT_MESSAGE_END) {
			Sint32 nearbyActorId =
				fdFindNearbyTalkTarget(fdPartyGetLeaderActor());

			fdScriptStart();
			fdScriptSetMessageId(cue->value);
			fdScriptShowMessageAndWait(nearbyActorId, 0);
			fdScriptEnd();
		} else {
			((ITEM_CUE_CALLBACK)cue->value)(itemId, actorId, slot);
		}
		result = 0;
	} else {
		/* The item's own field action, if it has one: the ability abilityId
		   names plays as if cast. */
		Sint32 abilityId;

		syGameFlagClear(FLAG_ITEM_USED_UP);
		syGameFlagSet(FLAG_NOTHING_HAPPENS);
		abilityId = gmItemGetDirectStub(itemId)->abilityId;
		state = Ram_HeapSlots->fieldState;

		if(abilityId != 0) {
			syGameFlagSet(FLAG_PSYNERGY_FROM_ITEM);
			syGameFlagClear(FLAG_NOTHING_HAPPENS);

			if(abilityId == ABILITY_RETREAT &&
				!syGameFlagGet(FLAG_RETREAT_BLOCKED)) {
				mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
				mnFieldMessageShowStub((Sint32)&MsgUseItemQuestion,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
						FIELD_MESSAGE_LOWER);
				/* partyIndex now holds the answer: 0 is yes. */
				partyIndex = fdYesNoAsk(TRUE);
				mnMessageCloseFinishedWindowsStub();
				if(partyIndex != 0)
					return 0;

				MainRoutineStateWork.next = MainRoutineStateWork.retreatScene;
				MainRoutineStateWork.entrance =
					MainRoutineStateWork.retreatEntrance;
				state->exit = EXIT_KEEP_DESTINATION;
			}

			mnMessageArgumentPushStub(actorId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			mnFieldMessageShowStub(
				(Sint32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
			fdPsynergyLoadResources(abilityId, 0);
			state->psynergyActing = 1;
			fdPsynergyRun();
			state->psynergyActing = 0;
			fdPsynergyEnd();

			/* ITEM_USE_TEACHES has ITEM_USE_CONSUMED's bit too: both are used
			   up. */
			if(gmItemGetDirectStub(itemId)->useType & ITEM_USE_CONSUMED)
				syGameFlagSet(FLAG_ITEM_USED_UP);
		}
	}

	if(syGameFlagGet(FLAG_NOTHING_HAPPENS))
		/* "But nothing happens." */
		mnFieldMessageShowStub((Sint32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
	if(syGameFlagGet(FLAG_ITEM_USED_UP))
		gmInventoryRemoveStub(actorId, slot);
	return result;
}

/* Chooses what a Psynergy of the given kind acts on, for fdFindPsynergyCue:
   among the scene's Psynergy cues for that kind whose condition holds (and
   whose direction, if any, the leader faces), the metatile the leader faces, or
   for Teleport the metatile under it; else the nearest target in reach (with
   PSYNERGY_TARGET_ACTOR); else PSYNERGY_TARGET_ANYWHERE for a cue that applies
   anywhere, or -1. */
Sint32 fdSelectPsynergyTarget(Sint32 kind)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	const CUE	*event = (const CUE *)state->cues;
	Uint32		 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;
	Sint32		 tileAhead = fdGetCollisionAheadOfLeader();
	Sint32		 tileUnder = fdGetCollisionUnderLeader();
	Sint32		 best = BEYOND_REACH;
	/* Default: -1, though every path below sets it. */
	Sint32 result = -1;
	Sint32 selected = -1;
	Sint32 nearby = -1;
	Sint32 fallback = -1;

	for(; event->ctrl != CUE_TABLE_END; event++) {
		Sint32 eventFacing = event->subject & CUE_SUBJECT_FACING;
		Sint16 requiresFacing =
			(Uint16)event->subject & CUE_SUBJECT_NEEDS_FACING;
		Sint32 eventTarget = (Uint16)event->subject & CUE_SUBJECT_TARGET;
		Sint32 eventKind =
			gmAbilityGetDirectStub((Uint16)event->ctrl >> 8)->fieldPsynergy;

		if((event->ctrl & CUE_KIND_MASK) == CUE_PSYNERGY && eventKind == kind &&
			fdGameFlagIsConditionActive(event->condition)) {
			if(requiresFacing) {
				Sint32 distance = (Sint16)(eventFacing - facing);

				if(distance < 0)
					distance = -distance;
				if(distance > TRIG_TURN * 3 / 32 - 1)
					continue;
			}
			if(event->ctrl & CUE_PSYNERGY_ANYWHERE) {
				fallback = PSYNERGY_TARGET_ANYWHERE;
			} else if(event->ctrl & CUE_PSYNERGY_ON_ACTOR) {
				Sint32 distance = fdMeasurePsynergyReach(
					kind, fdPartyGetLeaderActor(), eventTarget);

				if(distance != -1 && best > distance) {
					nearby = eventTarget | PSYNERGY_TARGET_ACTOR;
					best = distance;
				}
			} else if(kind == FIELD_PSYNERGY_TELEPORT) {
				if(eventTarget == tileUnder) {
					selected = eventTarget;
					break;
				}
			} else if(eventTarget == tileAhead) {
				selected = eventTarget;
				break;
			}
		}
	}
	if(selected != -1)
		result = selected;
	else if(nearby != -1)
		result = nearby;
	else
		result = fallback;
	return result;
}

/* Returns the scene's first Psynergy cue (CUE_PSYNERGY) for a field */
/* Psynergy kind whose condition holds, whose facing (when it has one) the
   leader faces, whose stage matches filter (PSYNERGY_CUE_ANY takes any) and
   whose target is what selector names (fdSelectPsynergyTarget: a metatile, an
   actor, or anything); NULL when there is none. */
CUE *fdFindPsynergyCue(Sint32 filter, Sint32 kind, Sint32 selector)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	CUE			*event = (CUE *)state->cues;
	Uint32		 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;

	if(selector == -1)
		return NULL;
	for(; event->ctrl != CUE_TABLE_END; event++) {
		Sint32 eventFacing = event->subject & CUE_SUBJECT_FACING;
		Sint16 requiresFacing =
			(Uint16)event->subject & CUE_SUBJECT_NEEDS_FACING;
		Sint32 eventTarget = (Uint16)event->subject & CUE_SUBJECT_TARGET;
		Sint32 eventKind =
			gmAbilityGetDirectStub((Uint16)event->ctrl >> 8)->fieldPsynergy;

		if((event->ctrl & CUE_KIND_MASK) == CUE_PSYNERGY && eventKind == kind &&
			fdGameFlagIsConditionActive(event->condition)) {
			if(requiresFacing) {
				Sint32 distance = (Sint16)(eventFacing - facing);

				if(distance < 0)
					distance = -distance;
				if(distance > TRIG_TURN * 3 / 32 - 1)
					continue;
			}
			if(filter != PSYNERGY_CUE_ANY &&
				(event->ctrl & (PSYNERGY_PHASE_MASK | CUE_KIND_MASK)) != filter)
				continue;
			if(event->ctrl & CUE_PSYNERGY_ANYWHERE) {
				if((selector & PSYNERGY_TARGET_KIND) ==
					PSYNERGY_TARGET_ANYWHERE)
					return event;
			} else if(event->ctrl & CUE_PSYNERGY_ON_ACTOR) {
				if((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ACTOR &&
					(selector & PSYNERGY_TARGET_ID) == eventTarget)
					return event;
			} else if((selector & PSYNERGY_TARGET_KIND) == 0 &&
				(selector & PSYNERGY_TARGET_ID) == eventTarget) {
				return event;
			}
		}
	}
	return NULL;
}

/* Runs the scene's scripts for Psynergy without paying its PP or showing its
   usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
   the kind's ending), as for a Psynergy that ends by itself: packed holds the
   Psynergy (bits 0 to 9) and its user (bits 10 to 13). */
Sint32 fdRunPsynergyEndScripts(Uint32 packed)
{
	Sint32		 action, user, kind, selector, target;
	PENDING_CUE *beforeEnd, *afterEnd;

	action = ABILITY_ID(packed);
	user = ACT_UNIT(packed);
	kind = gmAbilityGetDirectStub(action)->fieldPsynergy;
	selector = fdSelectPsynergyTarget(kind);
	beforeEnd = (PENDING_CUE *)fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_BEFORE_END), kind, selector);
	afterEnd = (PENDING_CUE *)fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_AFTER_END), kind, selector);
	target = -1;
	if(beforeEnd != NULL || afterEnd != NULL) {
		if(selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
			target = selector & PSYNERGY_TARGET_ID;
	}
	fdPsynergyLoadResources(action, 0);
	fdPsynergySetupActorPair(fdPartyGetLeaderActor(), target);
	fdRunPsynergyScript(beforeEnd, user, target);
	fdPsynergyRunExtraHandler();
	fdBattleEffectStopMainActor();
	fdRunPsynergyScript(afterEnd, user, target);
	fdPsynergyEnd();
	return 0;
}
