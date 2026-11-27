/* Battles and destinations a script requests, and the queued sound. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* Sets up a battle with the formation in slot of encounter table zone. */
void fdRequestBattleFromEncounterTable(Sint32 zone, Sint32 slot)
{
	FIELD_STATE *state = FieldStateWork;

	state->encounter = fdEncounterGetFormationAtSlot(zone, slot);
	if(zone == ENCOUNTER_ZONE_SCRIPTED && slot == 0)
		MainRoutineStateWork.battleBg =
			RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);

		fdEncounterResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
	}
	fdEncounterSelectBattleMusic(zone, slot);
}

/* The low bits of fdRequestDjinnBattle's actorAndHop: the Djinni's actor. */
#define DJINN_ACTOR_MASK 0xff

/* Sets up the battle with Djinni number `djinni`. The low 8 bits of actorAndHop
   are the Djinni's actor, which hops first unless DJINN_NO_HOP is set. */
void fdRequestDjinnBattle(Sint32 actorAndHop, Sint32 djinni)
{
	FIELD_STATE		   *fieldState;
	MAIN_ROUTINE_STATE *state;
	Sint32				skipHop;

	fieldState = FieldStateWork;
	skipHop = actorAndHop & DJINN_NO_HOP;
	actorAndHop &= DJINN_ACTOR_MASK;

	if(skipHop == 0)
		fdDjinnRunBattleStartHop(actorAndHop);

	state = &MainRoutineStateWork;
	/* DJINN_NO_HOP rides along: it tells fdDjinnRestoreActorAfterBattle to put
	   the Djinni back 32 pixels north of the leader, not at its placement's
	   position. */
	state->pendingDjinni = (djinni + DJINNI_BATTLE_SCRIPT_FIRST) | skipHop;
	fieldState->encounter = fdEncounterGetDjinnFormation(djinni);
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		ACTOR *leader = fdActorLookup(state->leaderActorId);

		fdEncounterResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
	}
	/* Key 0 is no listed encounter, so every Djinni battle takes MUS_BATTLE,
	   the last of EncounterBattleMusic. */
	fdEncounterSelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

#include "field/script_battle_destinations.c"

/* Plays the sound fdQueueMusic remembered, if any. */
void fdPlayQueuedMusic(void)
{
	Sint16 sndId = FieldStateWork->queuedSnd;

	if(sndId != -1)
		sySndPlayStub(sndId);
}
