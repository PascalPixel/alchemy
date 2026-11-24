/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Sets up a battle with the formation in slot of encounter table zone.
void Field_RequestBattleFromEncounterTable(s32 zone, s32 slot) {
  struct FieldState *state = gFieldState;

  state->encounter = Encounter_GetFormationAtSlot(zone, slot);
  if (zone == ENCOUNTER_ZONE_SCRIPTED && slot == 0)
    gMainRoutineState.battleBackground =
        RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP;
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct Actor *leader = Actor_Lookup(gMainRoutineState.leaderActorId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
  }
  Encounter_SelectBattleMusic(zone, slot);
}

/// The low bits of Field_RequestDjinnBattle's actorAndHop: the Djinni's actor.
#define DJINN_ACTOR_MASK 0xff

/// Sets up the battle with Djinni number `djinni`. The low 8 bits of
/// actorAndHop are the Djinni's actor, which hops first unless DJINN_NO_HOP is
/// set.
void Field_RequestDjinnBattle(s32 actorAndHop, s32 djinni) {
  struct FieldState *fieldState;
  struct MainRoutineState *state;
  s32 skipHop;

  fieldState = gFieldState;
  skipHop = actorAndHop & DJINN_NO_HOP;
  actorAndHop &= DJINN_ACTOR_MASK;

  if (skipHop == 0)
    Djinn_RunBattleStartHop(actorAndHop);

  state = &gMainRoutineState;
  // DJINN_NO_HOP rides along: it tells Djinn_RestoreActorAfterBattle to put
  // the Djinni back 32 pixels north of the leader, not at its placement's
  // position.
  state->pendingDjinni = (djinni + DJINNI_BATTLE_EVENT_FIRST) | skipHop;
  fieldState->encounter = Encounter_GetDjinnFormation(djinni);
  if (fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct Actor *leader = Actor_Lookup(state->leaderActorId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
  }
  // Key 0 is no listed encounter, so every Djinni battle takes MUS_BATTLE, the
  // last of gEncounter_BattleMusic.
  Encounter_SelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

/// Sets the after-battle destination (gMainRoutineState.afterBattleScene and
/// afterBattleEntrance), where the party goes once the coming battle is won.
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance) {
  gMainRoutineState.afterBattleScene = sceneId;
  gMainRoutineState.afterBattleEntrance = entrance;
}

/// Sets where the party wakes up after losing a battle
/// (gMainRoutineState.defeatScene and defeatEntrance).
void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  gMainRoutineState.defeatScene = sceneId;
  gMainRoutineState.defeatEntrance = entrance;
}

/// Sends the party to an entrance of another scene: the field sequence leaves
/// on its next pass (EXIT_KEEP_DESTINATION).
void Event_LeaveFor(u16 sceneId, u16 entrance) {
  struct FieldState *state = gFieldState;
  gMainRoutineState.next = sceneId;
  gMainRoutineState.entrance = entrance;
  state->exit = EXIT_KEEP_DESTINATION;
}

/// Changes the scene's music to soundId and remembers it for
/// Scene_PlayQueuedMusic; -1 fades the music out and remembers none.
void Scene_QueueMusic(s32 soundId) {
  gFieldState->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SOUND_FADE_OUT_ACTION_SE;
  }
  Sound_PlayFar(SE_STOP_EXIT_SE);
  Sound_PlayFar(soundId);
}

/// Plays the sound Scene_QueueMusic remembered, if any.
void Scene_PlayQueuedMusic(void) {
  s16 soundId = gFieldState->queuedSound;

  if (soundId != -1)
    Sound_PlayFar(soundId);
}
