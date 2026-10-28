/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Sets up a battle with the formation in slot of encounter table zone. On
/// the World Map the encounter region and battle environment under the leader
/// pick the battle background. Sets the field work's battle request, which
/// the field sequence then handles, and the battle music;
/// ENCOUNTER_ZONE_SCRIPTED's first slot also sets gMainRoutineWork.special
/// to the Aerie's storm battle background. About 35 scenes reach it through
/// Field_RequestBattleFromEncounterTableFar, and the Battle Arena calls it
/// directly. The battle starts only once the event that asked for it has
/// returned.
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct FieldWork *work = gFieldWork;

  work->encounter = Encounter_GetFormationAtSlot(zone, slot);
  if (zone == ENCOUNTER_ZONE_SCRIPTED && slot == 0)
    gMainRoutineWork.special =
        (s32)&ResourceId_VenusLighthouseAerieStormBackdrop;
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x.fixed);
  }
  Encounter_SelectBattleMusic(zone, slot);
}

/// Sets up the battle with Djinni number `no`. The low 8 bits of flag are the
/// Djinni's actor, which hops first unless DJINN_NO_HOP is set. Djinn
/// are numbered 20 to an element (Djinn_ResolvePending). Sets
/// gMainRoutineWork.pendingDjinni and the field work's encounter request;
/// when the scene is entered again after the battle, Djinn_ResolvePending reads
/// the pending event to make the Djinni join or hide it. About 20 scenes reach
/// it through Field_RequestDjinnBattleFar.
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
  // DJINN_NO_HOP rides along: it tells Actor_PlaceMapActors to put
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
/// About 30 scenes and the Battle Arena reach it through
/// Event_SetAfterBattleDestinationFar.
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.afterBattleScene = sceneId;
  gMainRoutineWork.afterBattleEntrance = entrance;
}

/// Sets where the party wakes up after losing a battle
/// (gMainRoutineWork.defeatScene and defeatEntrance). The scenes before
/// event battles (Colosso, the Vault, Vale, the Aerie) and the Battle Arena
/// reach it through Event_SetDefeatDestinationFar.
void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.defeatScene = sceneId;
  gMainRoutineWork.defeatEntrance = entrance;
}

/// Makes the event leave for entrance entrance of scene sceneId: sets
/// gMainRoutineWork.next and entrance and raises the exit request
/// EXIT_KEEP_DESTINATION (999), so the field sequence leaves on its next pass.
/// Only Tolbi's spring reaches it, through Event_LeaveForFar.
void Event_LeaveFor(u16 sceneId, u16 entrance) {
  struct FieldWork *work = gFieldWork;
  gMainRoutineWork.next = sceneId;
  gMainRoutineWork.entrance = entrance;
  work->exit = EXIT_KEEP_DESTINATION;
}

/// Plays SE_STOP_MUSIC_PLAYER_5, then soundId (SOUND_FADE_OUT_ACTION_SE for
/// -1), and remembers soundId for Scene_PlayQueuedMusic in the field work's
/// queuedSound; -1 is remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE
/// now and nothing later. About 30 scenes reach it through the far alias, and
/// the ground particles (weather.c) queue SE_AMBIENT_WATER with it.
void Scene_QueueMusic(s32 soundId) {
  gFieldWork->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SOUND_FADE_OUT_ACTION_SE;
  }
  Sound_PlayFar(SE_STOP_MUSIC_PLAYER_5);
  Sound_PlayFar(soundId);
}

/// Plays the sound Scene_QueueMusic remembered, if any. Called
/// by scenes, by the event runner (field_sequence/triggers.c), by
/// field_psynergy_work.c and by an actor's landing (actor_commands.c).
void Scene_PlayQueuedMusic(void) {
  s16 soundId = gFieldWork->queuedSound;

  if (soundId != -1)
    Sound_PlayFar(soundId);
}
