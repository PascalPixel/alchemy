/// @file
/// Battles and destinations a script requests, and the queued sound.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Sets up a battle with enemy group slot of encounter table zone. On the
/// world map the terrain under the leader picks the battle backdrop.
/// Sets the field work's battle request, which the map loop then handles,
/// and the battle cue; ENCOUNTER_ZONE_SCRIPTED's first slot also sets
/// gGameState.special to the Aerie's storm backdrop. About 35 maps reach it
/// through Field_RequestBattleFromEncounterTableFar, and the lobby calls it
/// directly. The battle starts only once the script that asked for it has
/// returned.
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo) {
  s32 zone = zoneNo;
  s32 slot = slotNo;
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  work->battle = Encounter_GetFormationAtSlot(zone, slot);
  if (zone == ENCOUNTER_ZONE_SCRIPTED && slot == 0)
    gGameState.special = (s32)&ResourceId_VenusLighthouseAerieStormBackdrop;
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    struct Object *leader = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  Encounter_SelectBattleMusic(zone, slot);
}

/// Sets up the battle with Djinni number `no`. The low 8 bits of flag are the
/// Djinni's object event, which hops first unless DJINN_NO_HOP is set. Djinn
/// are numbered 20 to an element (Djinn_ResolvePendingEvent). Sets
/// gGameState.pendingDjinnEvent and the field work's battle request; when
/// the map is entered again after the battle, Djinn_ResolvePendingEvent
/// reads the pending event to make the Djinni join or hide it. About 20
/// maps reach it through Field_RequestDjinnBattleFar.
void Field_RequestDjinnBattle(s32 flag, s32 no) {
  struct FieldWork *work;
  struct GameState *state;
  s32 skipHop;

  work = (struct FieldWork *)gFieldWork;
  skipHop = flag & DJINN_NO_HOP;
  flag &= 0xff;

  if (skipHop == 0)
    Djinn_RunBattleStartHop(flag);

  state = &gGameState;
  // DJINN_NO_HOP rides along: it tells ObjectEvent_PlaceMapObjectEvents to put
  // the Djinni back 32 pixels north of the leader, not at its template's
  // position.
  state->pendingDjinnEvent = (no + DJINNI_EVENT_0) | skipHop;
  work->battle = Encounter_GetDjinnFormation(no);
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    struct Object *leader = ObjectEvent_GetByLocalId(state->leaderLocalId);

    Encounter_ResolveWorldMapZoneAndBackdrop(&leader->x);
  }
  // Key 0 is no listed encounter, so every Djinni battle takes the music
  // list's last song (battle_cue.c).
  Encounter_SelectBattleMusic(0, 0);
}

/// Sets the map and warp destination that the field goes to next
/// (gGameState.nextMap and nextWarpDest). About 30 maps and the lobby
/// reach it through FieldScript_SetNextDestinationFar.
void FieldScript_SetNextDestination(s32 mapId, s32 warpDest) {
  gGameState.nextMap = mapId;
  gGameState.nextWarpDest = warpDest;
}

/// Sets where the party wakes up after losing a battle
/// (gGameState.defeatMap and defeatWarpDest). The maps before scripted
/// battles (Colosso, the Vault, Vale, the Aerie) and the lobby reach it
/// through FieldScript_SetDefeatDestinationFar.
void FieldScript_SetDefeatDestination(s32 mapId, s32 warpDest) {
  gGameState.defeatMap = mapId;
  gGameState.defeatWarpDest = warpDest;
}

/// Makes the script leave for warp destination warpDest of map mapId: sets
/// gGameState.mapId and warpDest and raises the warp request WARP_NONE (999),
/// so the map loop leaves on its next pass. Only Tolbi's spring reaches it,
/// through FieldScript_RequestDestinationFar.
void FieldScript_RequestDestination(u16 mapId, u16 warpDest) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;
  gGameState.mapId = mapId;
  gGameState.warpDest = warpDest;
  work->warp = 999;
}

/// Plays SE_STOP_SLOT5, then soundId (AUDIO_CUE_FADE_SLOT3 for -1), and
/// remembers soundId for Map_PlayQueuedMusic in the field work's
/// queuedSound; -1 is remembered as none, so it plays AUDIO_CUE_FADE_SLOT3 now
/// and nothing later. About 30 maps reach it through the far alias, and the
/// ground particles (weather.c) queue SE_170 with it.
void Map_QueueMusic(s32 soundId) {
  ((struct FieldWork *)gFieldWork)->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = AUDIO_CUE_FADE_SLOT3;
  }
  Sound_PlaySongFar(SE_STOP_SLOT5);
  Sound_PlaySongFar(soundId);
}

/// Plays the sound Map_QueueMusic remembered, if any. Called
/// by maps, by the event runner (object/events.c), by setup.c and by an
/// object event's landing (object_event_commands.c).
void Map_PlayQueuedMusic(void) {
  s16 soundId = ((struct FieldWork *)gFieldWork)->queuedSound;

  if (soundId != -1)
    Sound_PlaySongFar(soundId);
}
