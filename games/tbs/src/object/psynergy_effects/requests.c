/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// The zone of the scripted battles (Suhalla Desert, Lunpa Fortress, the
/// Aerie, the world map); its slot 0 is the Aerie's second battle.
#define AERIE 98
/// Sets up a battle with enemy group slot of encounter table zone. On the
/// world map (mode 3) the terrain under the leader picks the battle backdrop.
/// Sets the field work's battle request, which the scene loop then handles,
/// and the battle cue; zone 98's first slot also sets gGameState.special to
/// the Aerie's storm backdrop. About 35 scenes reach it through
/// Field_RequestBattleFromEncounterTableFar, and the lobby calls it directly.
/// The battle starts only once the event that asked for it has returned.
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct FieldWork *field = (struct FieldWork *)gEventWork;

  field->battle = Encounter_GetEnemyGroupAtSlot(zone, slot);
  if (zone == AERIE && slot == 0)
    gGameState.special = (s32)&ResourceId_VenusLighthouseAerieStormBackdrop;
  if (field->mode == MODE_WORLD_MAP) {
    struct ObjectRuntime *leader = ObjectTable_Get(gGameState.selectedActor);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  BattleFx_SelectBattleCue(zone, slot);
}

/// Sets up the battle with Djinni number `no`. The low 8 bits of flag are the
/// Djinni's actor, which hops first unless DJINN_NO_HOP is set. Djinn are
/// numbered 20 to an element (Djinn_ResolvePendingEvent). Sets
/// gGameState.pendingDjinnEvent and the field work's battle request; when
/// the scene is entered again after the battle, Djinn_ResolvePendingEvent
/// reads the pending event to make the Djinni join or hide it. About 20
/// scenes reach it through Field_RequestDjinnBattleFar.
void Field_RequestDjinnBattle(s32 flag, s32 no) {
  struct FieldWork *field;
  struct GameState *gameState;
  s32 skipHop;

  field = (struct FieldWork *)gEventWork;
  skipHop = flag & DJINN_NO_HOP;
  flag &= 0xff;

  if (skipHop == 0)
    Djinn_RunBattleStartHop(flag);

  gameState = &gGameState;
  // DJINN_NO_HOP rides along: it tells FieldObject_PlaceSceneActors to put
  // the Djinni back 32 pixels north of the leader, not at its placement.
  gameState->pendingDjinnEvent = (no + DJINNI_0) | skipHop;
  field->battle = Encounter_GetDjinnEnemyGroup(no);
  if (field->mode == MODE_WORLD_MAP) {
    struct ObjectRuntime *leader = ObjectTable_Get(gameState->selectedActor);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  // Key 0 is no listed encounter, so every Djinni battle takes the music
  // list's last song (battle_cue.c).
  BattleFx_SelectBattleCue(0, 0);
}

/// Sets the scene and entrance that the field goes to next
/// (gGameState.nextScene and nextEntrance). About 30 scenes and the lobby
/// reach it through Event_SetNextDestinationFar.
void Event_SetNextDestination(s32 scene, s32 entrance) {
  gGameState.nextScene = scene;
  gGameState.nextEntrance = entrance;
}

/// Sets where the party wakes up after losing a battle
/// (gGameState.defeatScene and defeatEntrance). The scenes before scripted
/// battles (Colosso, the Vault, Vale, the Aerie) and the lobby reach it
/// through Event_SetDefeatDestinationFar.
void Event_SetDefeatDestination(s32 scene, s32 entrance) {
  gGameState.defeatScene = scene;
  gGameState.defeatEntrance = entrance;
}

/// Makes the event leave for entrance entrance of scene scene: sets
/// gGameState.scene and entrance and raises the exit request EXIT_NONE (999),
/// so the scene loop leaves on its next pass. Only Tolbi's spring reaches it,
/// through Event_RequestDestinationFar.
void Event_RequestDestination(u16 scene, u16 entrance) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  gGameState.scene = scene;
  gGameState.entrance = entrance;
  work->exit = 999;
}

/// Plays SE_298, then soundId (SE_289 for -1), and remembers
/// soundId for BattleFx_PlayQueuedSound in the field work's queuedSound;
/// -1 is remembered as none, so it plays SE_289 now and nothing later.
/// About 30 scenes reach it through the far alias, and the ground particles
/// (weather.c) queue SE_170 with it.
void BattleFx_SetQueuedSoundAndPlay(s32 soundId) {
  ((struct FieldWork *)gEventWork)->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SE_289;
  }
  AudioCommand_PlayFar(SE_298);
  AudioCommand_PlayFar(soundId);
}

/// Plays the sound BattleFx_SetQueuedSoundAndPlay remembered, if any. Called
/// by scenes, by the event runner (object/events.c), by setup.c and by an
/// actor's landing (actor_commands.c).
void BattleFx_PlayQueuedSound(void) {
  s16 soundId = ((struct FieldWork *)gEventWork)->queuedSound;

  if (soundId != -1)
    AudioCommand_PlayFar(soundId);
}

#undef AERIE
