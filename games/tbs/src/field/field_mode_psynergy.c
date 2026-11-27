/* Using Psynergy on the field. Part of field_mode.c. */

#include "field_mode_internal.h"

/* Uses Psynergy on the field. code holds the Psynergy (bits 0 to 9), its user
   (bits 10 to 13; UNIT_NONE for ISAAC) and ACT_PACKED to use it without paying
   PP. */
Sint32 fdUsePsynergy(Uint32 code)
{
	Sint32		 abilityId = ABILITY_ID(code);
	FIELD_STATE *fieldState = FieldStateWork;
	const CUE	*before;
	const CUE	*after;
	const CUE	*during;
	Sint32		 userId;
	Sint32		 target;
	Bool		 retreat;
	Sint32		 abilityGroup;
	Sint32		 ppCost;
	Sint32		 declined;

	abilityGroup = gmAbilityGetDirectStub(abilityId)->fieldPsynergy;
	userId = ACT_UNIT(code);
	/* The leader is looked up and the result is not used. */
	fdActorLookup(MainRoutineStateWork.leaderActorId);
	retreat = FALSE;
	fdLeaderStopMoving();
	gmFlagClearStub(FLAG_PSYNERGY_FROM_ITEM);
	if(userId == UNIT_NONE)
		userId = 0;

	/* Where Psynergy has no effect. */
	if(gmFlagGetStub(FLAG_IN_BATTLE_ARENA)) {
		/* "[Psynergy]!!! ...Nothing happened." */
		PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
		return 0;
	}
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP &&
		abilityId == ABILITY_REVEAL) {
		/* "Reveal!!! ...Nothing happened." */
		PSYNERGY_MESSAGE(userId, ABILITY_REVEAL, MsgNothingHappened);
		return 0;
	}

	/* Retreat: ask, then go back to where the party entered this area. */
	if(abilityId == ABILITY_RETREAT) {
		if(gmFlagGetStub(FLAG_RETREAT_BLOCKED)) {
			/* "Retreat... It doesn't work here." */
			PSYNERGY_MESSAGE(userId, ABILITY_RETREAT, MsgDoesNotWorkHere);
			return 0;
		}
		mnMessageArgumentPushStub(ABILITY_RETREAT, MESSAGE_ARGUMENT_ABILITY);
		/* "Use Retreat?" Yes: the party retreats; No: nothing more. */
		mnFieldMessageShowStub(MsgUseAbilityConfirm,
			FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN | FIELD_MESSAGE_LOWER);
		declined = fdYesNoAsk(1); /* 0 is yes */
		mnMessageCloseFinishedWindowsStub();
		if(declined != 0)
			return 0;
		{
			MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
			Sint32				sceneId, returnEntrance;
			sceneId = (Uint16)state->retreatScene;
			state->next = sceneId;
			returnEntrance = (Uint16)state->retreatEntrance;
			state->entrance = returnEntrance;
		}
		fieldState->exit = EXIT_KEEP_DESTINATION;
		retreat = TRUE;
	}
	/* A use that pays no PP only runs the scene's ending scripts. */
	if(code & ACT_PACKED)
		return fdRunPsynergyEndScripts(code);

	/* A party member pays the PP. */
	if(userId <= ALLY_MAX) {
		ppCost = gmAbilityGetDirectStub(abilityId)->ppCost;
		if(gmUnitGetStub(userId)->pp < ppCost) {
			/* "[Psynergy]... Not enough PP." */
			PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
			/* Retreat was already accepted: take its exit back (the scene and
			   entrance it chose stay in the main routine's state). */
			if(retreat)
				fieldState->exit = 0;
			return 0;
		}
		gmUnitAddPpStub(userId, -ppCost);
	}

	/* Find the scene's scripts, and choose how the Psynergy plays. */
	before = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_BEFORE_CAST), abilityGroup, &target);
	after = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_AFTER_CAST), abilityGroup, &target);
	during = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), abilityGroup, &target);
	target = -1;
	gmFlagSetStub(FLAG_PLAY_PSYNERGY_CAST);
	gmFlagSetStub(FLAG_PSYNERGY_TARGET);
	if(before || after || during) {
		target = fdFindNearbyPsynergyTarget(
			MainRoutineStateWork.leaderActorId, abilityGroup);
		/* The after script plays the Psynergy itself. */
		if(after && (after->subject & CUE_SUBJECT_OWN_EFFECT)) {
			gmFlagClearStub(FLAG_PLAY_PSYNERGY_CAST);
			gmFlagClearStub(FLAG_PSYNERGY_TARGET);
		}
	} else
		gmFlagClearStub(FLAG_PSYNERGY_TARGET);

	/* Play the Psynergy. */
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP)
		fdActorClearOutOfBounds();
	fdPsynergyLoadResources(abilityId, 0);
	fieldState->psynergyActing = 1;
	fdPsynergySetupActorPair(MainRoutineStateWork.leaderActorId, target);
	fdPsynergyStartCast();

	fdRunPsynergyScript(before, userId, target);
	if(gmFlagGetStub(FLAG_PLAY_PSYNERGY_CAST)) {
		if(gmFlagGetStub(FLAG_PSYNERGY_TARGET))
			fdPsynergyDispatch();
		else
			fdPsynergyRun();
	}
	fdPsynergyStopCasterUpdate();
	fdRunPsynergyScript(after, userId, target);
	if(gmFlagGetStub(FLAG_PLAY_PSYNERGY_CAST))
		fdPsynergyFinishMindRead();
	gmFlagClearStub(FLAG_PLAY_PSYNERGY_CAST);
	gmFlagClearStub(FLAG_PSYNERGY_TARGET);
	fieldState->psynergyActing = 0;
	fdPsynergyEnd();
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP)
		fdActorRespawn();
	return 0;
}

/* Returns whether the scene has a Psynergy cue, in any phase, for Psynergy of
   group and what the leader faces. */
Bool fdHasPsynergyCue(Sint32 group)
{
	Sint32 target;

	if(fdFindPsynergyCue(PSYNERGY_CUE_ANY, (Uint16)group, &target))
		return TRUE;
	return FALSE;
}

/* Returns whether the scene has a script for item and what the leader faces. */
Uint32 fdHasItemCue(Uint16 item)
{
	if(fdFindItemCue(item))
		return 1;
	return 0;
}
