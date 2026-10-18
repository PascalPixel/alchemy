// Part of psynergy_effects.c, compiled only through it.
// Battles and destinations an event requests, and the queued sound.

#include "../psynergy_effects_internal.h"

#define AERIE 98           // encounter table fought before the Aerie's storm
#define DJINN_NO_HOP 0x800 // do not make the Djinni hop first
// Sets up a battle with enemy group slot of encounter table zone. On the
// world map (mode 3) the terrain under the leader picks the battle backdrop.
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct FieldWork *field = (struct FieldWork *)gEventWork;

  field->battle = Encounter_GetEnemyGroupAtSlot(zone, slot); // enemy group
  if (zone == AERIE && slot == 0)
    gGameState.special = (s32)&ResourceId_VenusLighthouseAerieStormBackdrop;
  if (field->mode == MODE_WORLD_MAP) {
    struct ObjectRuntime *leader = ObjectTable_Get(gGameState.selectedActor);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  BattleFx_SelectBattleCue(zone, slot);
}

// Sets up the battle with Djinni no. The low 8 bits of flag are the Djinni's
// actor, which hops first unless DJINN_NO_HOP is set. Djinn are numbered 20
// to an element (Djinn_ResolvePendingEvent).
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
  gameState->pendingDjinnEvent = (no + DJINNI_0) | skipHop;
  field->battle = Encounter_GetDjinnEnemyGroup(no); // the Djinni's enemy group
  if (field->mode == MODE_WORLD_MAP) {
    struct ObjectRuntime *leader = ObjectTable_Get(gameState->selectedActor);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  BattleFx_SelectBattleCue(0, 0);
}

// Sets the scene and entrance that the field goes to next.
void Event_SetNextDestination(s32 scene, s32 entrance) {
  gGameState.nextScene = scene;
  gGameState.nextEntrance = entrance;
}

// Sets where the party wakes up after losing a battle.
void Event_SetDefeatDestination(s32 scene, s32 entrance) {
  gGameState.defeatScene = scene;
  gGameState.defeatEntrance = entrance;
}

// Makes the event leave for entrance entrance of scene scene.
void Event_RequestDestination(u16 scene, u16 entrance) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  gGameState.scene = scene;
  gGameState.entrance = entrance;
  work->exit = 999;
}

// Plays SE_298, then soundId (SE_289 for -1), and remembers
// soundId for BattleFx_PlayQueuedSound.
void BattleFx_SetQueuedSoundAndPlay(s32 soundId) {
  ((struct FieldWork *)gEventWork)->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SE_289;
  }
  AudioCommand_PlayFar(SE_298);
  AudioCommand_PlayFar(soundId);
}

// Plays the sound BattleFx_SetQueuedSoundAndPlay remembered, if any.
void BattleFx_PlayQueuedSound(void) {
  s16 soundId = ((struct FieldWork *)gEventWork)->queuedSound;

  if (soundId != -1)
    AudioCommand_PlayFar(soundId);
}
