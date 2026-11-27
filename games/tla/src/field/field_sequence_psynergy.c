/* @file */
/* Using Psynergy on the field. */
/* Part of field_sequence.c. */

#include "field/field_psynergy_state.h"

/* Shows message with the user's and the Psynergy's names, and waits. */
#define PSYNERGY_MESSAGE(user, ability, message)                               \
	(mnMessageArgumentPushStub((user), MESSAGE_ARGUMENT_ACTOR),                \
	 mnMessageArgumentPushStub((ability), MESSAGE_ARGUMENT_ABILITY),           \
	 fdMessageShowStub((s32)&(message), FIELD_MESSAGE_ZOOM))

/* Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its user
 */
/* (bits 10 to 13; UNIT_NONE for the first party member) and ACT_PACKED to use
 */
/* it without paying PP. */
s32 fdUsePsynergy(u32 code) {
	FIELD_STATE *fieldState = Ram_HeapSlots->fieldState;
	s32 abilityId = ABILITY_ID(code);
	PENDING_EVENT *before;
	s32 group = btAbilityGet(abilityId)->fieldPsynergy;
	s32 userId = ACT_UNIT(code);
	s32 retreat = 0;
	PENDING_EVENT *after;
	PENDING_EVENT *during;
	PENDING_EVENT *event;
	ACTOR *actor;
	s32 selector;
	s32 target;
	s32 declined;
	s32 hit;
	FIELD_EFFECT_STATE *effect;

	fdLeaderStopMoving();
	fieldState->psynergyHitObj = -1;
	fieldState->standing = retreat;
	syGameFlagClear(FLAG_PSYNERGY_FROM_ITEM);
	if(userId == UNIT_NONE)
		userId = 0;

	/* Where Psynergy has no effect. */
	if(syGameFlagGet(FLAG_IN_BATTLE_ARENA)) {
		/* "<Psynergy>!!! ...Nothing happened." */
		PSYNERGY_MESSAGE(userId, abilityId, MsgPsynergyNothingHappened);
		return 0;
	}
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(abilityId == ABILITY_REVEAL || abilityId == ABILITY_MAGNET) {
			/* "<Psynergy>!!! ...Nothing happened." */
			PSYNERGY_MESSAGE(userId, abilityId, MsgPsynergyNothingHappened);
			return 0;
		}
		if(abilityId == ABILITY_HOVER) {
			if(MainRoutineStateWork.ownerOverride != 0 &&
			   syGameFlagGet(FLAG_SHIP_FLIES)) {
				syGameFlagSet(FLAG_SHIP_HOVERING);
				fieldState->raisedTrigger = WORLD_MAP_RAISED_TAKE_OFF;
				return 0;
			}
			/* "<Psynergy>!!! ...Nothing happened." */
			PSYNERGY_MESSAGE(userId, abilityId, MsgPsynergyNothingHappened);
			return 0;
		}
	}
	if(MainRoutineStateWork.ownerOverride != 0 && abilityId == ABILITY_SAND) {
		/* "Sand!!! ...Nothing happened." */
		PSYNERGY_MESSAGE(userId, ABILITY_SAND, MsgPsynergyNothingHappened);
		return 0;
	}

	/* Retreat: ask, then go back to where the party entered this area. */
	if(abilityId == ABILITY_RETREAT) {
		if(syGameFlagGet(FLAG_RETREAT_BLOCKED)) {
			/* "Retreat... It doesn't work here." */
			PSYNERGY_MESSAGE(userId, ABILITY_RETREAT,
							 MsgPsynergyDoesntWorkHere);
			return 0;
		}
		mnMessageArgumentPushStub(ABILITY_RETREAT, MESSAGE_ARGUMENT_ABILITY);
		/* "Use Retreat?" Yes: retreat; No: nothing more. */
		fdMessageShowStub((s32)&MsgPsynergyUseQuestion,
						  FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN |
							  FIELD_MESSAGE_LOWER);
		declined = fdYesNoAsk(TRUE); /* 0 is yes */
		mnMessageCloseFinishedWindowsStub();
		if(declined != 0)
			return 0;
		{
			MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
			s32 scene, entrance;

			scene = (u16)state->retreatScene;
			state->next = scene;
			entrance = (u16)state->retreatEntrance;
			state->entrance = entrance;
		}
		fieldState->exit = EXIT_KEEP_DESTINATION;
		retreat = 1;
	}

	/* The scene's first event; a use that pays no PP only runs its ending
	 * events. */
	selector = scSelectPsynergyTarget(group);
	event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_FIRST), group,
								  selector);
	if(event != NULL) {
		target = -1;
		if(selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
			target = selector & PSYNERGY_TARGET_ID;
		fdRunPsynergyEvent(event, userId, target);
	}
	if(code & ACT_PACKED)
		return fdRunPsynergyEndEvents(code);

	/* Find the scene's events; Teleport off the World Map needs one. */
	selector = scSelectPsynergyTarget(group);
	before = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_BEFORE_CAST),
								   group, selector);
	after = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_CAST),
								  group, selector);
	during = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), group,
								   selector);
	if(abilityId == ABILITY_TELEPORT &&
	   MainRoutineStateWork.next != SCENE_WORLD_MAP && before == NULL &&
	   after == NULL && during == NULL) {
		/* "Teleport... It doesn't work here." */
		PSYNERGY_MESSAGE(userId, ABILITY_TELEPORT, MsgPsynergyDoesntWorkHere);
		return 0;
	}

	/* A party member pays the PP. */
	if(userId <= ALLY_MAX) {
		ABILITY_DEFINITION *ability = btAbilityGet(abilityId);

		FIELD_UNIT *unit = syUnitGet(userId);
		s32 ppCost = ability->ppCost;

		if(unit->pp < ppCost) {
			/* Retreat was already accepted: take its exit back. */
			if(retreat)
				fieldState->exit = 0;
			/* "<Psynergy>... Not enough PP." */
			PSYNERGY_MESSAGE(userId, abilityId, MsgPsynergyNotEnoughPp);
			return 0;
		}
		gmUnitAddPpStub(userId, -ppCost);
	}

	/* Sand used off sandy ground finds no events. */
	if(group == FIELD_PSYNERGY_SAND) {
		actor = fdActorLookup(fdPartyGetLeaderActor());
		if(fdMapGridIsSandCell(actor->x.fixed, actor->z.fixed, actor->layer) ==
		   0) {
			before = NULL;
			after = NULL;
			during = NULL;
		}
	}

	/* Choose how the Psynergy plays. */
	syGameFlagSet(FLAG_PLAY_PSYNERGY_CAST);
	syGameFlagClear(FLAG_PSYNERGY_TARGET);
	target = -1;
	if(before != NULL || after != NULL || during != NULL) {
		if(selector != -1 && (selector & PSYNERGY_TARGET_ACTOR))
			target = selector & PSYNERGY_TARGET_ID;
		syGameFlagSet(FLAG_PSYNERGY_TARGET);
		if(after != NULL && (after->flg & TRIGGER_SUBJECT_OWN_EFFECT)) {
			syGameFlagClear(FLAG_PLAY_PSYNERGY_CAST);
			syGameFlagClear(FLAG_PSYNERGY_TARGET);
		}
	}

	/* Play the Psynergy; an event marked quiet hides its effect. */
	fdPsynergyLoadResources(abilityId, 0);
	fieldState->psynergyActing = 1;
	if(before != NULL && (before->flg & TRIGGER_SUBJECT_QUIET)) {
		effect = Ram_HeapSlots->fieldEffState;
		effect->flashCaster = 0;
		effect->announceName = 0;
	}
	if(after != NULL && (after->flg & TRIGGER_SUBJECT_QUIET)) {
		effect = Ram_HeapSlots->fieldEffState;
		effect->flashCaster = 0;
		effect->announceName = 0;
	}
	if(during != NULL && (during->flg & TRIGGER_SUBJECT_QUIET)) {
		effect = Ram_HeapSlots->fieldEffState;
		effect->flashCaster = 0;
		effect->announceName = 0;
	}
	fdPsynergySetupActorPair(fdPartyGetLeaderActor(), target);
	fdPsynergyStartCast();
	fdRunPsynergyEvent(before, userId, target);
	if(syGameFlagGet(FLAG_PLAY_PSYNERGY_CAST)) {
		if(syGameFlagGet(FLAG_PSYNERGY_TARGET))
			fdPsynergyDispatch();
		else
			fdPsynergyRun();
	}
	fdBattleEffectStopMainActor();
	/* What the Psynergy hit has an event of its own. */
	hit = fieldState->psynergyHitObj;
	if(hit != -1) {
		selector = hit | PSYNERGY_TARGET_ACTOR;
		event = scFindPsynergyTrigger(
			PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END), group, selector);
		if(event != NULL)
			fdRunPsynergyEvent(event, userId, fieldState->psynergyHitObj);
	}
	fdRunPsynergyEvent(after, userId, target);
	if(syGameFlagGet(FLAG_PLAY_PSYNERGY_CAST))
		fdPsynergyFinishMindRead();
	fieldState->psynergyActing = 0;
	fdPsynergyEnd();
	syGameFlagClear(FLAG_PLAY_PSYNERGY_CAST);
	syGameFlagClear(FLAG_PSYNERGY_TARGET);
	return 0;
}

/* Runs the event tied to a field Psynergy: an action below */
/* TRIGGER_RESULT_MESSAGE_END is a message, shown at target; any other is a */
/* function, called with argument (the user, when a cast runs it) and target. */
/* With FLAG_NOTHING_HAPPENS set it then says "But nothing happens." */
s32 fdRunPsynergyEvent(PENDING_EVENT *event, s32 argument, s32 target) {
	if(event != NULL) {
		if(event->action != 0) {
			if(event->action < TRIGGER_RESULT_MESSAGE_END) {
				fdEventStart();
				fdEventSetMessageId(event->action);
				fdEventShowMessageAndWait(target, 0);
				fdEventEnd();
			}
			else {
				((void (*)(s32, s32))event->action)(argument, target);
			}
		}
		if(syGameFlagGet(FLAG_NOTHING_HAPPENS)) {
			fdEventStart();
			fdMessageShowStub((s32)MsgNothingHappens, FIELD_MESSAGE_ZOOM);
			fdEventEnd();
		}
	}
	return 0;
}

/* Returns whether the scene has a trigger for the field Psynergy kind on what
 */
/* it would act on (scSelectPsynergyTarget), at any stage. */
s32 scHasPsynergyTrigger(s32 fieldPsynergy) {
	u16 kind = fieldPsynergy;

	if(scFindPsynergyTrigger(PSYNERGY_TRIGGER_ANY, kind,
							 scSelectPsynergyTarget(kind)))
		return 1;
	return 0;
}

/* Returns whether the scene has an event for item and what the leader faces. */
u32 scHasItemTrigger(u16 item) {
	if(fdFindItemTrigger(item))
		return 1;
	return 0;
}
