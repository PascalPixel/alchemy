/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Sets up a battle with the formation in slot of encounter table zone.
void Field_RequestBattleFromEncounterTable(s32 zone, s32 slot) {
  struct FieldWork *work = gFieldWork;

  work->encounter = Encounter_GetFormationAtSlot(zone, slot);
  if (zone == ENCOUNTER_ZONE_SCRIPTED && slot == 0)
    gMainRoutineWork.battleBackground =
        RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP;
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
  }
  Encounter_SelectBattleMusic(zone, slot);
}

/// Sets up the battle with Djinni number `no`. The low 8 bits of flag are the
/// Djinni's actor, which hops first unless DJINN_NO_HOP is set.
void Field_RequestDjinnBattle(s32 flag, s32 no) {
  struct FieldWork *work;
  struct MainRoutineWork *state;
  s32 skipHop;

  work = gFieldWork;
  skipHop = flag & DJINN_NO_HOP;
  flag &= 0xff;

  if (skipHop == 0)
    Djinn_RunBattleStartHop(flag);

  state = &gMainRoutineWork;
  // DJINN_NO_HOP rides along: it tells Djinn_RestoreActorAfterBattle to put
  // the Djinni back 32 pixels north of the leader, not at its placement's
  // position.
  state->pendingDjinni = (no + DJINNI_BATTLE_EVENT_FIRST) | skipHop;
  work->encounter = Encounter_GetDjinnFormation(no);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct Actor *leader = Actor_Lookup(state->leaderActorId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
  }
  // Key 0 is no listed encounter, so every Djinni battle takes MUS_BATTLE, the
  // last song in gEncounter_BattleMusic.
  Encounter_SelectBattleMusic(0, 0);
}

/// Sets the after-battle destination (gMainRoutineWork.afterBattleScene and
/// afterBattleEntrance), where the party goes once the coming battle is won.
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.afterBattleScene = sceneId;
  gMainRoutineWork.afterBattleEntrance = entrance;
}

/// Sets where the party wakes up after losing a battle
/// (gMainRoutineWork.defeatScene and defeatEntrance).
void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.defeatScene = sceneId;
  gMainRoutineWork.defeatEntrance = entrance;
}

/// Makes the event leave for entrance entrance of scene sceneId: sets
/// gMainRoutineWork.next and entrance and raises the exit request
/// EXIT_KEEP_DESTINATION (999), so the field sequence leaves on its next pass.
void Event_LeaveFor(u16 sceneId, u16 entrance) {
  struct FieldWork *work = gFieldWork;
  gMainRoutineWork.next = sceneId;
  gMainRoutineWork.entrance = entrance;
  work->exit = EXIT_KEEP_DESTINATION;
}

/// Plays SE_STOP_MUSIC_PLAYER_5, then soundId (SOUND_FADE_OUT_ACTION_SE for
/// -1), and remembers soundId for Scene_PlayQueuedMusic in the field work's
/// queuedSound; -1 is remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE
/// now and nothing later.
void Scene_QueueMusic(s32 soundId) {
  gFieldWork->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SOUND_FADE_OUT_ACTION_SE;
  }
  Sound_PlayFar(SE_STOP_MUSIC_PLAYER_5);
  Sound_PlayFar(soundId);
}

/// Plays the sound Scene_QueueMusic remembered, if any.
void Scene_PlayQueuedMusic(void) {
  s16 soundId = gFieldWork->queuedSound;

  if (soundId != -1)
    Sound_PlayFar(soundId);
}
