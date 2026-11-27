/* Battles and destinations a script requests, and the queued sound. Part of
   field_mode.c. */

/* Sets up a battle with the formation at row and column of the encounter
   table. */
void fdRequestBattleFromEncounterTable(Sint32 row, Sint32 column)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	state->encounter = fdEncounterGetFormationAtSlot(row, column);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		fdEncounterResolveWorldMapZoneAndBackdrop(
			&fdActorLookup(fdPartyGetLeaderActor())->x.fixed);
	fdEncounterSelectBattleMusic(row, column);
}

/* The low bits of fdRequestDjinnBattle's flags: the Djinni's actor. */
#define DJINN_ACTOR_MASK 0xff

/* Sets up the battle with Djinni djinn, DJINN_PER_ELEMENT to an element; an
   element's last Djinni stands for the one of The Broken Seal's chosen for it
   (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). The low 8 bits of flags
   are the Djinni's actor, which hops first unless DJINN_NO_HOP is set. */
void fdRequestDjinnBattle(Uint32 flg, Uint32 djinn)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint32		 skipHop = flg & DJINN_NO_HOP;
	Uint32		 element;
	Sint32		 idx;

	flg &= DJINN_ACTOR_MASK;
	element = djinn / DJINN_PER_ELEMENT;
	idx = djinn % DJINN_PER_ELEMENT;

	if(idx == DJINN_PER_ELEMENT - 1) {
		idx = SceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];
		if(idx == 0)
			return;
		idx--;
	}
	djinn = element * DJINN_PER_ELEMENT + idx;
	if(skipHop == 0)
		fdDjinnRunBattleStartHop(flg);
	/* DJINN_NO_HOP rides along: it tells fdDjinnRestoreActorAfterBattle to put
	   the Djinni back 32 pixels north of the leader, not at its placement's
	   position. */
	MainRoutineStateWork.pendingDjinni =
		(djinn + DJINNI_BATTLE_SCRIPT_FIRST) | skipHop;
	state->encounter = fdEncounterGetDjinnFormation(djinn);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		fdEncounterResolveWorldMapZoneAndBackdrop(
			&fdActorLookup(fdPartyGetLeaderActor())->x.fixed);
	fdEncounterSelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

#include "field/script_battle_destinations.c"

/* A script that does nothing. */
void fdScriptNoOperation(void)
{
}

/* Plays the sound fdQueueMusic remembered, if any. */
void fdPlayQueuedMusic(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	if(state != NULL) {
		Sint16 snd = state->queuedSnd;

		if(snd != -1)
			sySndPlayStub(snd);
	}
}

/* Shows message in a script and clears FLAG_NOTHING_HAPPENS; when someone in
   the party has Scoop, the field menu then opens. */
void fdScriptShowMessageAndOfferScoop(Sint32 msg)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	mnFieldMessageShowStub(msg, FIELD_MESSAGE_ZOOM);
	syGameFlagClear(FLAG_NOTHING_HAPPENS);
	fdScriptEnd();
	if(gmPartyFindOwnerWithActionStub(ABILITY_SCOOP) != -1)
		FieldStateWork->fieldMenu = 1;
}

/* Plays sound when the actor is in view: within 152 pixels across and 112 up or
   down the screen of the actor the camera follows. */
void fdActorPlaySndInView(ACTOR *actor, Sint32 snd)
{
	ACTOR *focus = FieldStateWork->camera;

	if(actor == 0 || focus == 0)
		return;
	if(ABS(focus->x.fixed - actor->x.fixed) > toFIXED(152))
		return;
	if(ABS((focus->z.fixed - focus->y.fixed) -
		   (actor->z.fixed - actor->y.fixed)) > toFIXED(112))
		return;
	sySndPlayStub(snd);
}

/* Counts one more weird answer Felix gives
   (MainRoutineStateWork.weirdAnswers). */
void fdScriptCountAnswer(void)
{
	MainRoutineStateWork.weirdAnswers++;
}
