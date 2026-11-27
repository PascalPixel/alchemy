/* @file */
/* Using an item on the field. */
/* Part of field_sequence.c. */

#include "item_ids.h"

/* What fdEventEndLastingPsynergy leaves running, as bits of its skip. */
typedef enum PSYNERGY_END_SKIP {
	END_SKIP_REVEAL = 1 << 0,
	END_SKIP_RING = 1 << 1, /* the ring of light (ABILITY_MAGNET) */
	END_SKIP_SAND = 1 << 2,
} PSYNERGY_END_SKIP;

/* Runs message as an event of its own, shown at actor. */
void fdEventRunMessage(s32 actor, s32 message) {
	fdEventStart();
	fdEventSetMessageId(message);
	fdEventShowMessageAndWait(actor, 0);
	fdEventEnd();
}

/* Ends every lasting Psynergy (fdEventEndLastingPsynergy with none skipped). */
void fdEventEndAllLastingPsynergy(void) { fdEventEndLastingPsynergy(0); }

/* Ends the lasting Psynergy and runs their end events: Reveal, the ring of */
/* light and Sand (while the party is sand, MainRoutineStateWork.sandActive), */
/* each unless skip holds its bit (PSYNERGY_END_SKIP). */
void fdEventEndLastingPsynergy(s32 skip) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->busy = 0;
	if(state->revealActive != 0 && !(skip & END_SKIP_REVEAL))
		fdRunPsynergyEndEvents(ABILITY_REVEAL | ACT_PACKED);
	if(state->ringActive != 0 && !(skip & END_SKIP_RING))
		fdRunPsynergyEndEvents(ABILITY_MAGNET | ACT_PACKED);
	if(MainRoutineStateWork.sandActive != 0 && !(skip & END_SKIP_SAND))
		fdRunPsynergyEndEvents(ABILITY_SAND | ACT_PACKED);
}

/* Returns the leader's actor id, or ACTOR_FIRST_PLACED while */
/* MainRoutineStateWork.ownerOverride is set. */
s32 fdPartyGetLeaderActor(void) {
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	s32 actor;

	/* While the party sails, the first placed actor, the ship, stands for it.
	 */
	actor = ACTOR_FIRST_PLACED;
	if(state->ownerOverride == 0)
		actor = state->leaderActorId;
	return actor;
}

/* Returns 1 when itemId answers filter: the item itself, ITEM_FILTER_ANY any */
/* item, or ITEM_FILTER_HEAVY any weapon, armor, shield or helm; else 0. */
s32 fdItemMatchesFilter(s32 filter, s32 itemId) {
	if(filter == itemId)
		return 1;
	if(filter == ITEM_FILTER_ANY)
		return 1;
	if(filter == ITEM_FILTER_HEAVY &&
	   (u32)(gmItemGetDirectStub(itemId)->type - ITEM_TYPE_WEAPON) <=
		   ITEM_TYPE_HELM - ITEM_TYPE_WEAPON)
		return 1;
	return 0;
}

/* Returns the scene's item trigger (TRIGGER_ITEM) for item that applies to */
/* what the leader faces, or NULL (item 0 matches any): its item or item filter
 */
/* takes item (fdItemMatchesFilter), and its target is the actor the leader can
 */
/* talk to (TRIGGER_ITEM_ON_ACTOR) or the cell ahead. */
const TRIGGER *fdFindItemTrigger(s32 item) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	const TRIGGER *trigger = state->triggers;
	s32 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;
	s32 actorAhead = fdFindNearbyTalkTarget(fdPartyGetLeaderActor());
	s32 tileAhead;

	item &= INVENTORY_ITEM_ID_MASK;
	tileAhead = fdGetCollisionAheadOfLeader();

	for(; trigger->ctrl != -1; trigger++) {
		s32 eventFacing = (s16)(u16)trigger->subject & TRIGGER_SUBJECT_FACING;
		s16 requiresFacing =
			(u16)trigger->subject & TRIGGER_SUBJECT_NEEDS_FACING;
		s32 eventTarget = (u16)trigger->subject & TRIGGER_SUBJECT_TARGET;

		if((trigger->ctrl & TRIGGER_KIND_MASK) == TRIGGER_ITEM &&
		   fdGameFlagIsConditionActive(trigger->condition) != 0 &&
		   (requiresFacing == 0 ||
			(u16)(eventFacing - facing + (TRIG_TURN * 3 / 32 - 1)) <=
				TRIG_TURN * 3 / 16 - 2)) {
			/* The item, as USE_ITEM packs it above the trigger's kind. */
			s32 eventItem = (u16)((s32)trigger->ctrl / 256);

			if(item == 0 || fdItemMatchesFilter(eventItem, item)) {
				if((trigger->ctrl & TRIGGER_ITEM_ON_ACTOR) != 0) {
					if(eventTarget == actorAhead)
						return trigger;
				}
				else if(eventTarget == tileAhead) {
					return trigger;
				}
			}
		}
	}
	return NULL;
}

/* Uses an item on the field. packedItem holds the item (bits 0 to 9) and the */
/* character using it (bits 10 to 13; UNIT_NONE for whoever holds it in the */
/* most inventory slots); slot is its inventory slot. The scene's item trigger
 */
/* for what the leader faces runs if there is one; otherwise an item with a */
/* field action casts it, Retreat asking first. Returns 0 when the scene's */
/* event ran or Retreat was declined, else -1. */
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
		}
		else {
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
			fdMessageShowStub((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
			return -1;
		}
	}

	trigger = fdFindItemTrigger(itemId);
	if(trigger != NULL && trigger->value != 0) {
		/* The scene's event: a message, or a function. */
		syGameFlagClear(FLAG_ITEM_USED_UP);
		syGameFlagClear(FLAG_NOTHING_HAPPENS);
		if(!(trigger->subject & TRIGGER_SUBJECT_OWN_EFFECT)) {
			mnMessageArgumentPushStub(actorId, MESSAGE_ARGUMENT_ACTOR);
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			fdMessageShowStub((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
		}
		if((s32)trigger->value < TRIGGER_RESULT_MESSAGE_END) {
			s32 nearbyActorId = fdFindNearbyTalkTarget(fdPartyGetLeaderActor());

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
		/* plays as if cast. */
		s32 abilityId;

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
				fdMessageShowStub((s32)&MsgUseItemQuestion,
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
			fdMessageShowStub((s32)&MsgActorUsesItem, FIELD_MESSAGE_ZOOM);
			fdPsynergyLoadResources(abilityId, 0);
			state->psynergyActing = 1;
			fdPsynergyRun();
			state->psynergyActing = 0;
			fdPsynergyEnd();

			/* ITEM_USE_TEACHES has ITEM_USE_CONSUMED's bit too: both are used
			 * up. */
			if(gmItemGetDirectStub(itemId)->useType & ITEM_USE_CONSUMED)
				syGameFlagSet(FLAG_ITEM_USED_UP);
		}
	}

	if(syGameFlagGet(FLAG_NOTHING_HAPPENS))
		/* "But nothing happens." */
		fdMessageShowStub((s32)&MsgNothingHappens, FIELD_MESSAGE_ZOOM);
	if(syGameFlagGet(FLAG_ITEM_USED_UP))
		gmInventoryRemoveStub(actorId, slot);
	return result;
}

/* Chooses what a Psynergy of the given kind acts on, for */
/* scFindPsynergyTrigger: among the scene's Psynergy triggers for that */
/* kind whose condition holds (and whose direction, if any, the leader faces),
 */
/* the cell the leader faces, or for Teleport the cell under it; else the */
/* nearest target in reach (with PSYNERGY_TARGET_ACTOR); else */
/* PSYNERGY_TARGET_ANYWHERE for a trigger that applies anywhere, or -1. */
s32 scSelectPsynergyTarget(s32 kind) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	const TRIGGER *event = (const TRIGGER *)state->triggers;
	u32 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;
	s32 tileAhead = fdGetCollisionAheadOfLeader();
	s32 tileUnder = fdGetCollisionUnderLeader();
	s32 best = BEYOND_REACH;
	/* Default: -1, though every path below sets it. */
	s32 result = -1;
	s32 selected = -1;
	s32 nearby = -1;
	s32 fallback = -1;

	for(; event->ctrl != TRIGGER_TABLE_END; event++) {
		s32 eventFacing = event->subject & TRIGGER_SUBJECT_FACING;
		s16 requiresFacing = (u16)event->subject & TRIGGER_SUBJECT_NEEDS_FACING;
		s32 eventTarget = (u16)event->subject & TRIGGER_SUBJECT_TARGET;
		s32 eventKind = btAbilityGet((u16)event->ctrl >> 8)->fieldPsynergy;

		if((event->ctrl & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
		   eventKind == kind && fdGameFlagIsConditionActive(event->condition)) {
			if(requiresFacing) {
				s32 distance = (s16)(eventFacing - facing);

				if(distance < 0)
					distance = -distance;
				if(distance > TRIG_TURN * 3 / 32 - 1)
					continue;
			}
			if(event->ctrl & TRIGGER_PSYNERGY_ANYWHERE) {
				fallback = PSYNERGY_TARGET_ANYWHERE;
			}
			else if(event->ctrl & TRIGGER_PSYNERGY_ON_ACTOR) {
				s32 distance = fdMeasurePsynergyReach(
					kind, fdPartyGetLeaderActor(), eventTarget);

				if(distance != -1 && best > distance) {
					nearby = eventTarget | PSYNERGY_TARGET_ACTOR;
					best = distance;
				}
			}
			else if(kind == FIELD_PSYNERGY_TELEPORT) {
				if(eventTarget == tileUnder) {
					selected = eventTarget;
					break;
				}
			}
			else if(eventTarget == tileAhead) {
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

/* Returns the scene's first Psynergy trigger (TRIGGER_PSYNERGY) for a field */
/* Psynergy kind whose condition holds, whose facing (when it has one) the */
/* leader faces, whose stage matches filter (PSYNERGY_TRIGGER_ANY takes any) */
/* and whose target is what selector names (scSelectPsynergyTarget: a cell, */
/* an actor, or anything); NULL when there is none. */
TRIGGER *scFindPsynergyTrigger(s32 filter, s32 kind, s32 selector) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	TRIGGER *event = (TRIGGER *)state->triggers;
	u32 facing = fdActorLookup(fdPartyGetLeaderActor())->facing;

	if(selector == -1)
		return NULL;
	for(; event->ctrl != TRIGGER_TABLE_END; event++) {
		s32 eventFacing = event->subject & TRIGGER_SUBJECT_FACING;
		s16 requiresFacing = (u16)event->subject & TRIGGER_SUBJECT_NEEDS_FACING;
		s32 eventTarget = (u16)event->subject & TRIGGER_SUBJECT_TARGET;
		s32 eventKind = btAbilityGet((u16)event->ctrl >> 8)->fieldPsynergy;

		if((event->ctrl & TRIGGER_KIND_MASK) == TRIGGER_PSYNERGY &&
		   eventKind == kind && fdGameFlagIsConditionActive(event->condition)) {
			if(requiresFacing) {
				s32 distance = (s16)(eventFacing - facing);

				if(distance < 0)
					distance = -distance;
				if(distance > TRIG_TURN * 3 / 32 - 1)
					continue;
			}
			if(filter != PSYNERGY_TRIGGER_ANY &&
			   (event->ctrl & (PSYNERGY_PHASE_MASK | TRIGGER_KIND_MASK)) !=
				   filter)
				continue;
			if(event->ctrl & TRIGGER_PSYNERGY_ANYWHERE) {
				if((selector & PSYNERGY_TARGET_KIND) ==
				   PSYNERGY_TARGET_ANYWHERE)
					return event;
			}
			else if(event->ctrl & TRIGGER_PSYNERGY_ON_ACTOR) {
				if((selector & PSYNERGY_TARGET_KIND) == PSYNERGY_TARGET_ACTOR &&
				   (selector & PSYNERGY_TARGET_ID) == eventTarget)
					return event;
			}
			else if((selector & PSYNERGY_TARGET_KIND) == 0 &&
					(selector & PSYNERGY_TARGET_ID) == eventTarget) {
				return event;
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
s32 fdRunPsynergyEndEvents(u32 packed) {
	s32 action, user, kind, selector, target;
	PENDING_EVENT *beforeEnd, *afterEnd;

	action = ABILITY_ID(packed);
	user = ACT_UNIT(packed);
	kind = btAbilityGet(action)->fieldPsynergy;
	selector = scSelectPsynergyTarget(kind);
	beforeEnd = (PENDING_EVENT *)scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_END), kind, selector);
	afterEnd = (PENDING_EVENT *)scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), kind, selector);
	target = -1;
	if(beforeEnd != NULL || afterEnd != NULL) {
		if(selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
			target = selector & PSYNERGY_TARGET_ID;
	}
	fdPsynergyLoadResources(action, 0);
	fdPsynergySetupActorPair(fdPartyGetLeaderActor(), target);
	fdRunPsynergyEvent(beforeEnd, user, target);
	fdPsynergyRunExtraHandler();
	fdBattleEffectStopMainActor();
	fdRunPsynergyEvent(afterEnd, user, target);
	fdPsynergyEnd();
	return 0;
}
