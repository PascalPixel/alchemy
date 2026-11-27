/* @file */
/* Using an item on the field. */
/* Part of field_sequence.c. */

#include "field_sequence_internal.h"

/* The function a scene's item trigger may name instead of a message, called */
/* with the item, its user and its inventory slot; its result is not read. */
typedef s32 (*ITEM_TRIGGER_CALLBACK)(s32 item, s32 actorId, s32 slot);

/* Returns the scene's item trigger (TRIGGER_ITEM) for item that applies to */
/* what the leader faces, or 0 (item 0 matches any item). */
const TRIGGER *fdFindItemTrigger(s32 item) {
	FIELD_STATE *state = FieldStateWork;
	const TRIGGER *trigger = state->triggers;
	s32 facing =
		((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;
	s32 actorAhead = fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
	s32 tileAhead;

	item &= INVENTORY_ITEM_ID_MASK;
	/* Query both target forms once; matching events choose actor or cell */
	/* below. */
	tileAhead = fdGetCollisionAheadOfLeader();
	/* The kind's bits: 15..8 the item, 4 on an actor, 3..0 the kind. */
	for(; trigger->kind != -1; trigger++) {
		s32 eventFacing = trigger->subject & TRIGGER_SUBJECT_FACING;
		s16 requiresFacing = trigger->subject & TRIGGER_SUBJECT_NEEDS_FACING;
		s32 eventTarget = trigger->subject & TRIGGER_SUBJECT_TARGET;

		if((trigger->kind & TRIGGER_KIND_MASK) == TRIGGER_ITEM &&
		   fdGameFlagIsConditionActive(trigger->condition) != 0 &&
		   (requiresFacing == 0 ||
			(u16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
				TRIG_TURN * 3 / 16 - 2)) {
			u32 flags = trigger->kind;
			s32 eventItem = ((const u8 *)&trigger->kind)[1];

			if(item == 0 || eventItem == item) {
				if((flags & TRIGGER_ITEM_ON_ACTOR) != 0) {
					if(eventTarget == actorAhead)
						return trigger;
				}
				else if(eventTarget == tileAhead) {
					return trigger;
				}
			}
		}
	}
	return 0;
}

/* Uses an item on the field. packedItem holds the item (bits 0 to 9) and the */
/* character using it (bits 10 to 13; UNIT_NONE for whoever holds it in the */
/* most inventory slots); slot is its inventory slot. */
s32 fdUseItem(s32 packedItem, s32 slot) {
	s32 result;
	s32 itemId;
	s32 actorId;
	UNIT *unit;
	const TRIGGER *trigger;
	s32 inventorySlot;
	s32 partyIndex;
	s32 mostMatchingSlots;
	s32 matchingSlots;
	s32 partyCount;
	FIELD_STATE *state;

	result = -1;
	itemId = ACT_ITEM_ID(packedItem);
	actorId = ACT_UNIT(packedItem);
	/* Count the user's slots holding the item; with no user given, the user is
	 */
	/* whoever holds it in the most slots, the earlier on a tie. */
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
		}
		else {
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

	trigger = fdFindItemTrigger(itemId);
	if(trigger != 0 && trigger->value != 0) {
		/* The scene's event: a message, or a function. */
		gmFlagClearStub(FLAG_ITEM_USED_UP);
		gmFlagClearStub(FLAG_NOTHING_HAPPENS);
		if(!(trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT)) {
			mnMessageArgumentPushStub(actorId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			/* "[member] uses [item]." */
			mnFieldMessageShowStub(MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
		}
		if((s32)trigger->value < TRIGGER_RESULT_MESSAGE_END) {
			s32 nearbyActorId =
				fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
			fdEventStart();
			fdEventSetMessageId(trigger->value);
			fdEventShowMessageAndWait(nearbyActorId, 0);
			fdEventEnd();
		}
		else {
			((ITEM_TRIGGER_CALLBACK)trigger->value)(itemId, actorId, slot);
		}
		result = 0;
	}
	else {
		/* The item's own field action, if it has one: the ability abilityId
		 * names */
		/* plays as if cast, and one whose useType has bit 0 set
		 * (ITEM_USE_CONSUMED, */
		/* and ITEM_USE_TEACHES too) is used up. */
		s32 abilityId;

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
									   FIELD_MESSAGE_ZOOM |
										   FIELD_MESSAGE_KEEP_OPEN |
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
			 * prototype */
			/* is, though it is defined void. */
#ifdef UBFIX
			fdPsynergyLoadResources(abilityId, 0);
#else
			((s32 (*)(s32, s32))fdPsynergyLoadResources)(abilityId, 0);
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

/* Returns the scene's Psynergy trigger (TRIGGER_PSYNERGY) for Psynergy of the
 */
/* given group that applies to what the leader faces, or NULL; stores the actor
 */
/* the Psynergy would target in *target. eventFlags gives the event's phase and
 */
/* kind (PSYNERGY_TRIGGER_ANY for any phase). */
void *scFindPsynergyTrigger(s32 eventFlags, s32 group, s32 *target) {
	FIELD_STATE *state = FieldStateWork;
	const TRIGGER *trigger = state->triggers;
	s32 facing =
		((ACTOR *)fdActorLookup(MainRoutineStateWork.leaderActorId))->facing;
	s32 actorAhead =
		fdFindNearbyPsynergyTarget(MainRoutineStateWork.leaderActorId, group);
	s32 tileAhead;
	s32 anyPhase = 0;

	*target = actorAhead;
	/* The target output is filled even when no event ultimately matches. */
	tileAhead = fdGetCollisionAheadOfLeader();
	if(eventFlags == PSYNERGY_TRIGGER_ANY)
		anyPhase = 1;

	for(; trigger->kind != -1; trigger++) {
		s32 eventFacing = trigger->subject & TRIGGER_SUBJECT_FACING;
		s16 requiresFacing = trigger->subject & TRIGGER_SUBJECT_NEEDS_FACING;
		s32 eventTarget = trigger->subject & TRIGGER_SUBJECT_TARGET;

		if((trigger->kind & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
		   fdGameFlagIsConditionActive(trigger->condition) != 0 &&
		   (requiresFacing == 0 ||
			(u16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
				TRIG_TURN * 3 / 16 - 2) &&
		   mnAbilityGet(((const u8 *)&trigger->kind)[1])->fieldPsynergy ==
			   group &&
		   (anyPhase || (trigger->kind & (PSYNERGY_PHASE_MASK |
										  TRIGGER_KIND_MASK)) == eventFlags)) {
			/* Any Psynergy of the same field kind (fieldPsynergy) as the
			 * event's */
			/* matches. */
			if((trigger->kind & TRIGGER_PSYNERGY_ANYWHERE) != 0)
				return (void *)trigger;
			if((trigger->kind & TRIGGER_PSYNERGY_ON_ACTOR) != 0) {
				if(eventTarget == actorAhead)
					return (void *)trigger;
			}
			else if(eventTarget == tileAhead) {
				return (void *)trigger;
			}
		}
	}
	return NULL;
}

/* Runs the scene's events for Psynergy without paying its PP or showing its */
/* usual effect (PSYNERGY_PHASE_BEFORE_END and PSYNERGY_PHASE_AFTER_END, around
 */
/* the kind's ending), as for a Psynergy that ends by itself: packed holds the
 */
/* Psynergy (bits 0 to 9) and its user (bits 10 to 13). */
s32 fdRunPsynergyEndEvents(s32 packed) {
	s32 target;
	s32 abilityId;
	s32 userId;
	u8 group;
	void *beforeEvent;
	void *afterEvent;

	abilityId = ABILITY_ID(packed);
	userId = ACT_UNIT((u32)packed);
	group = mnAbilityGet(abilityId)->fieldPsynergy;
	/* The leader is looked up and the result is not used. */
	fdActorLookup(MainRoutineStateWork.leaderActorId);
	beforeEvent = scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_END), group, &target);
	afterEvent = scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), group, &target);
	fdPsynergyLoadResources(abilityId, 0);
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, target);
	fdRunPsynergyEvent(beforeEvent, userId, target);
	fdPsynergyRunExtraHandler();
	fdPsynergyStopCasterUpdate();
	fdRunPsynergyEvent(afterEvent, userId, target);
	fdPsynergyEnd();
	return 0;
}
