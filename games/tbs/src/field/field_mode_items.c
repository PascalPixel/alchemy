/* Using an item on the field. Part of field_mode.c. */

#include "field_mode_internal.h"

/* The function a scene's item cue may name instead of a message, called with
   the item, its user and its inventory slot; its result is not read. */
typedef Sint32 (*ITEM_CUE_CALLBACK)(Sint32 item, Sint32 actorId, Sint32 slot);

/* Returns the scene's item cue (CUE_ITEM) for item that applies to what the
   leader faces, or 0 (item 0 matches any item). */
const CUE *fdFindItemCue(Sint32 item)
{
	FIELD_STATE *state = FieldStateWork;
	const CUE	*cue = state->cues;
	Sint32		 facing =
		((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;
	Sint32 actorAhead =
		fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
	Sint32 tileAhead;

	item &= INVENTORY_ITEM_ID_MASK;
	/* Query both target forms once; matching scripts choose actor or metatile
	   below. */
	tileAhead = fdGetCollisionAheadOfLeader();
	/* The kind's bits: 15..8 the item, 4 on an actor, 3..0 the kind. */
	for(; cue->kind != -1; cue++) {
		Sint32 eventFacing = cue->subject & CUE_SUBJECT_FACING;
		Sint16 requiresFacing = cue->subject & CUE_SUBJECT_NEEDS_FACING;
		Sint32 eventTarget = cue->subject & CUE_SUBJECT_TARGET;

		if((cue->kind & CUE_KIND_MASK) == CUE_ITEM &&
			fdGameFlagIsConditionActive(cue->condition) != 0 &&
			(requiresFacing == 0 ||
				(Uint16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
					TRIG_TURN * 3 / 16 - 2)) {
			Uint32 flags = cue->kind;
			Sint32 eventItem = ((const Uint8 *)&cue->kind)[1];

			if(item == 0 || eventItem == item) {
				if((flags & CUE_ITEM_ON_ACTOR) != 0) {
					if(eventTarget == actorAhead)
						return cue;
				} else if(eventTarget == tileAhead) {
					return cue;
				}
			}
		}
	}
	return 0;
}

/* Uses an item on the field. packedItem holds the item (bits 0 to 9) and the
   character using it (bits 10 to 13; UNIT_NONE for whoever holds it in the most
   inventory slots); slot is its inventory slot. */
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
				unit = gmUnitGetStub(MainRoutineStateWork.party[partyIndex]);
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
			unit = gmUnitGetStub(actorId);
			for(inventorySlot = 0; inventorySlot < INVENTORY_SLOTS;
				inventorySlot++) {
				if((unit->inventory[inventorySlot] & INVENTORY_ITEM_ID_MASK) ==
					itemId)
					mostMatchingSlots++;
			}
		}

		if(mostMatchingSlots == 0) {
			/* "But nothing happens." */
			mnFieldMessageShowStub(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
			return -1;
		}
	}

	cue = fdFindItemCue(itemId);
	if(cue != 0 && cue->value != 0) {
		/* The scene's script: a message, or a function. */
		gmFlagClearStub(FLAG_ITEM_USED_UP);
		gmFlagClearStub(FLAG_NOTHING_HAPPENS);
		if(!(cue->subject & CUE_SUBJECT_OWN_EFFECT)) {
			mnMessageArgumentPushStub(actorId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			/* "[member] uses [item]." */
			mnFieldMessageShowStub(MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
		}
		if((Sint32)cue->value < CUE_RESULT_MESSAGE_END) {
			Sint32 nearbyActorId =
				fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
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
		   names plays as if cast, and one whose useType has bit 0 set
		   (ITEM_USE_CONSUMED, and ITEM_USE_TEACHES too) is used up. */
		Sint32 abilityId;

		gmFlagClearStub(FLAG_ITEM_USED_UP);
		gmFlagSetStub(FLAG_NOTHING_HAPPENS);
		abilityId = gmItemGetDirectStub(itemId)->abilityId;
		state = FieldStateWork;

		if(abilityId != 0) {
			gmFlagSetStub(FLAG_PSYNERGY_FROM_ITEM);
			gmFlagClearStub(FLAG_NOTHING_HAPPENS);

			if(abilityId == ABILITY_RETREAT &&
				!gmFlagGetStub(FLAG_RETREAT_BLOCKED)) {
				mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
				/* "Use [item]?" Yes: the party retreats; No: nothing more. */
				mnFieldMessageShowStub(MsgUseItemQuestion,
					FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
						FIELD_MESSAGE_LOWER);
				/* partyIndex now holds the answer: 0 is yes. */
				partyIndex = fdYesNoAsk(1);
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
			/* "[member] uses [item]." */
			mnFieldMessageShowStub(MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
			/* UB: called as a function returning int, as a call with no
			   prototype is, though it is defined void. */
#if FIX_FUNCTION_SIGNATURES
			fdPsynergyLoadResources(abilityId, 0);
#else
			((Sint32 (*)(Sint32, Sint32))fdPsynergyLoadResources)(abilityId, 0);
#endif
			state->psynergyActing = 1;
			fdPsynergyRun();
			state->psynergyActing = 0;
			fdPsynergyEnd();

			if(gmItemGetDirectStub(itemId)->useType & ITEM_USE_CONSUMED)
				gmFlagSetStub(FLAG_ITEM_USED_UP);
		}
	}

	if(gmFlagGetStub(FLAG_NOTHING_HAPPENS))
		/* "But nothing happens." */
		mnFieldMessageShowStub(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
	if(gmFlagGetStub(FLAG_ITEM_USED_UP))
		gmInventoryRemoveStub(actorId, slot);
	return result;
}

/* Returns the scene's Psynergy cue (CUE_PSYNERGY) for Psynergy of the given
   group that applies to what the leader faces, or NULL; stores the actor the
   Psynergy would target in *target. eventFlags gives the script's phase and
   kind (PSYNERGY_CUE_ANY for any phase). */
void *fdFindPsynergyCue(Sint32 eventFlags, Sint32 group, Sint32 *target)
{
	FIELD_STATE *state = FieldStateWork;
	const CUE	*cue = state->cues;
	Sint32		 facing =
		((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;
	Sint32 actorAhead =
		fdFindNearbyPsynergyTarget(MainRoutineStateWork.leaderActorId, group);
	Sint32 tileAhead;
	Sint32 anyPhase = 0;

	*target = actorAhead;
	/* The target output is filled even when no script ultimately matches. */
	tileAhead = fdGetCollisionAheadOfLeader();
	if(eventFlags == PSYNERGY_CUE_ANY)
		anyPhase = 1;

	for(; cue->kind != -1; cue++) {
		Sint32 eventFacing = cue->subject & CUE_SUBJECT_FACING;
		Sint16 requiresFacing = cue->subject & CUE_SUBJECT_NEEDS_FACING;
		Sint32 eventTarget = cue->subject & CUE_SUBJECT_TARGET;

		if((cue->kind & CUE_KIND_MASK) == CUE_PSYNERGY &&
			fdGameFlagIsConditionActive(cue->condition) != 0 &&
			(requiresFacing == 0 ||
				(Uint16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
					TRIG_TURN * 3 / 16 - 2) &&
			gmAbilityGetDirectStub(((const Uint8 *)&cue->kind)[1])
					->fieldPsynergy == group &&
			(anyPhase ||
				(cue->kind & (PSYNERGY_PHASE_MASK | CUE_KIND_MASK)) ==
					eventFlags)) {
			/* Any Psynergy of the same field kind (fieldPsynergy) as the
			   script's matches. */
			if((cue->kind & CUE_PSYNERGY_ANYWHERE) != 0)
				return (void *)cue;
			if((cue->kind & CUE_PSYNERGY_ON_ACTOR) != 0) {
				if(eventTarget == actorAhead)
					return (void *)cue;
			} else if(eventTarget == tileAhead) {
				return (void *)cue;
			}
		}
	}
	return NULL;
}

/* Runs the scene's scripts for Psynergy without paying its PP or showing its
   usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
   the kind's ending), as for a Psynergy that ends by itself: packed holds the
   Psynergy (bits 0 to 9) and its user (bits 10 to 13). */
Sint32 fdRunPsynergyEndScripts(Sint32 packed)
{
	Sint32 target;
	Sint32 abilityId;
	Sint32 userId;
	Uint8  group;
	void  *beforeEvent;
	void  *afterEvent;

	abilityId = ABILITY_ID(packed);
	userId = ACT_UNIT((Uint32)packed);
	group = gmAbilityGetDirectStub(abilityId)->fieldPsynergy;
	/* The leader is looked up and the result is not used. */
	fdActorLookup(MainRoutineStateWork.leaderActorId);
	beforeEvent = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_BEFORE_END), group, &target);
	afterEvent = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_AFTER_END), group, &target);
	fdPsynergyLoadResources(abilityId, 0);
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, target);
	fdRunPsynergyScript(beforeEvent, userId, target);
	fdPsynergyRunExtraHandler();
	fdPsynergyStopCasterUpdate();
	fdRunPsynergyScript(afterEvent, userId, target);
	fdPsynergyEnd();
	return 0;
}
