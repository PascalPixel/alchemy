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

/* Sets the after-battle destination (MainRoutineStateWork.afterBattleScene and
   afterBattleEntrance), where the party goes once the coming battle is won. */
void fdScriptSetAfterBattleDestination(Sint32 sceneId, Sint32 entrance)
{
	MainRoutineStateWork.afterBattleScene = sceneId;
	MainRoutineStateWork.afterBattleEntrance = entrance;
}

/* Sets where the party wakes up after losing a battle
   (MainRoutineStateWork.defeatScene and defeatEntrance). */
void fdScriptSetDefeatDestination(Sint32 sceneId, Sint32 entrance)
{
	MainRoutineStateWork.defeatScene = sceneId;
	MainRoutineStateWork.defeatEntrance = entrance;
}

/* Sends the party to an entrance of another scene: the field mode leaves on its
   next pass (EXIT_KEEP_DESTINATION). */
void fdScriptLeaveFor(Uint16 sceneId, Uint16 entrance)
{
	FIELD_STATE *state = FieldStateWork;
	MainRoutineStateWork.next = sceneId;
	MainRoutineStateWork.entrance = entrance;
	state->exit = EXIT_KEEP_DESTINATION;
}

/* Changes the scene's music to soundId and remembers it for fdPlayQueuedMusic;
   -1 fades the music out and remembers none. */
void fdQueueMusic(Sint32 soundId)
{
	FieldStateWork->queuedSound = soundId;
	if((Sint16)soundId == -1) {
		soundId = SOUND_FADE_OUT_ACTION_SE;
	}
	sySoundPlayStub(SE_STOP_EXIT_SE);
	sySoundPlayStub(soundId);
}

/* Plays the sound fdQueueMusic remembered, if any. */
void fdPlayQueuedMusic(void)
{
	Sint16 soundId = FieldStateWork->queuedSound;

	if(soundId != -1)
		sySoundPlayStub(soundId);
}
