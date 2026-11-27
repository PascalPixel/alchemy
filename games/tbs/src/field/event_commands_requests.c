/* @file */
/* Battles and destinations an event requests, and the queued sound. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Sets up a battle with the formation in slot of encounter table zone. */
void fdRequestBattleFromEncounterTable(s32 zone, s32 slot) {
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

/* Sets up the battle with Djinni number `djinni`. The low 8 bits of */
/* actorAndHop are the Djinni's actor, which hops first unless DJINN_NO_HOP is
 */
/* set. */
void fdRequestDjinnBattle(s32 actorAndHop, s32 djinni) {
	FIELD_STATE *fieldState;
	MAIN_ROUTINE_STATE *state;
	s32 skipHop;

	fieldState = FieldStateWork;
	skipHop = actorAndHop & DJINN_NO_HOP;
	actorAndHop &= DJINN_ACTOR_MASK;

	if(skipHop == 0)
		fdDjinnRunBattleStartHop(actorAndHop);

	state = &MainRoutineStateWork;
	/* DJINN_NO_HOP rides along: it tells fdDjinnRestoreActorAfterBattle to put
	 */
	/* the Djinni back 32 pixels north of the leader, not at its placement's */
	/* position. */
	state->pendingDjinni = (djinni + DJINNI_BATTLE_EVENT_FIRST) | skipHop;
	fieldState->encounter = fdEncounterGetDjinnFormation(djinni);
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		ACTOR *leader = fdActorLookup(state->leaderActorId);

		fdEncounterResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
	}
	/* Key 0 is no listed encounter, so every Djinni battle takes MUS_BATTLE,
	 * the */
	/* last of Encounter_BattleMusic. */
	fdEncounterSelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

/* Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
 */
/* afterBattleEntrance), where the party goes once the coming battle is won. */
void fdEventSetAfterBattleDestination(s32 sceneId, s32 entrance) {
	MainRoutineStateWork.afterBattleScene = sceneId;
	MainRoutineStateWork.afterBattleEntrance = entrance;
}

/* Sets where the party wakes up after losing a battle */
/* (MainRoutineStateWork.defeatScene and defeatEntrance). */
void fdEventSetDefeatDestination(s32 sceneId, s32 entrance) {
	MainRoutineStateWork.defeatScene = sceneId;
	MainRoutineStateWork.defeatEntrance = entrance;
}

/* Sends the party to an entrance of another scene: the field sequence leaves */
/* on its next pass (EXIT_KEEP_DESTINATION). */
void fdEventLeaveFor(u16 sceneId, u16 entrance) {
	FIELD_STATE *state = FieldStateWork;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
	state->exit = EXIT_KEEP_DESTINATION;
}

/* Changes the scene's music to soundId and remembers it for */
/* scPlayQueuedMusic; -1 fades the music out and remembers none. */
void scQueueMusic(s32 soundId) {
	FieldStateWork->queuedSnd = soundId;
	if((s16)soundId == -1) {
		soundId = SOUND_FADE_OUT_ACTION_SE;
	}
	sySndPlayStub(SE_STOP_EXIT_SE);
	sySndPlayStub(soundId);
}

/* Plays the sound scQueueMusic remembered, if any. */
void scPlayQueuedMusic(void) {
	s16 soundId = FieldStateWork->queuedSnd;

	if(soundId != -1)
		sySndPlayStub(soundId);
}
