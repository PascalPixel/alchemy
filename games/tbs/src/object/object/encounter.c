/// @file
/// Random encounters, and each map's music, area name and backdrop.
// Part of object.c.

#include "../object_internal.h"

/// A map's encounter zones (gBattle_LocationRules, location_rules.s),
/// ended by map -1. Encounter_SelectMapZones takes the first rule that
/// matches the map and warp destination and is not turned off.
struct BattleLocationRule {
  s16 mapId;                 ///< -1 ends the table
  s16 warpDest;              ///< -1 for any warp destination
  s16 flag : 15;             ///< -1, or a flag that turns the rule off once set
  s16 firstZoneOnGround : 1; ///< the first zone is for ground 0, not 1
  /// The first of up to seven consecutive zones, rows of encounter.tsv; 0 for
  /// none on any ground.
  s16 firstZone;
};

extern const struct BattleLocationRule gBattle_LocationRules[];

/// The world map's encounter zone for each kind of terrain
/// (gWorldMap_EncounterRules, zonedata.c), ended by kind -1. Read by
/// Encounter_ResolveWorldMapZoneAndBackdrop, first match first.
typedef struct WorldMapEncounterRule {
  s16 kind; ///< the terrain kind of the world map metatile; -1 ends the table
  /// The terrain's behavior, which also picks the backdrop (1 to 7,
  /// BattleBackground_SelectForEnvironment), or -1 for any.
  s16 behavior;
  s16 flag; ///< -1, or a flag that turns the rule off once set
  s16 zone; ///< a row of encounter.tsv
} WorldMapEncounterRule;

/// The number of enemy groups an encounter zone can hold.
enum { ENCOUNTER_GROUP_COUNT = 8 };

/// An encounter zone, a row of encounter.tsv (gEncounterZones): its
/// rate, the level it is made for, and the enemy groups that can appear with
/// their chances. 28 bytes, the 14 halfwords Encounter_GetFormationAtSlot
/// counts a zone as.
struct EncounterZone {
  /// The base encounter rate; 0 for zone 0 (no encounters) and for a zone only
  /// scripted battles, mimics and Djinn use.
  u16 rate;
  u16 level; ///< the party level the zone is made for
  /// The enemy groups that can appear, rows of battle/formatn.tsv.
  u16 enemies[ENCOUNTER_GROUP_COUNT];
  u8 weights[ENCOUNTER_GROUP_COUNT]; ///< each group's relative chance
};

/// Chooses the current map's encounter zones, one for each kind of ground
/// the party can stand on (gFieldWork->encounterZones, read by
/// Encounter_RollLayoutStep), from the first rule matching the map
/// and warp destination. Ground 0 has none, and grounds 1 to 7 take consecutive
/// zones from the rule's first one. With useRules 0, or no matching rule, no
/// ground has encounters. Also resets the random bias and the battle threshold
/// (the count itself, gGameState.encounterSteps, is reset only when a random
/// battle starts or a new map section is entered) and chooses the battle
/// backdrop. Called by Map_Run on map load, with useRules 0 on the world
/// map and 1 in a map; writes work->encounterZones, encounterBias and
/// encounterThreshold, and gGameState.special.
void Encounter_SelectMapZones(s32 useRules) {
  struct FieldWork *work;
  s32 mapId;
  s32 warpDest;
  u8 *groundZones;
  const struct BattleLocationRule *rule;
  s32 zone;
  s16 firstZoneOnGround;
  u32 i;

  work = (struct FieldWork *)gFieldWork;
  mapId = gGameState.mapId;
  warpDest = gGameState.warpDest;
  groundZones = work->encounterZones;
  rule = gBattle_LocationRules;
  zone = 0;
  firstZoneOnGround = 0;
  if (useRules != 0) {
    while (rule->mapId != -1) {
      if (rule->mapId == mapId &&
          (rule->warpDest == -1 || rule->warpDest == warpDest)) {
        if (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0) {
          zone = rule->firstZone;
          firstZoneOnGround = rule->firstZoneOnGround;
          break;
        }
      }
      rule++;
    }
  }

  // Ground zero is normally safe; the rule can move the first zone onto it.
  *groundZones++ = 0;
  for (i = 0; i <= 6; i++) {
    *groundZones++ = zone;
    if (zone != 0)
      zone++;
  }
  if (firstZoneOnGround != 0) {
    work->encounterZones[0] = work->encounterZones[1];
    work->encounterZones[1] = 0;
  }
  work->encounterBias = 0;
  // 16 in 16.16: Encounter_CountStepsAndSelectFormation starts a battle once
  // gGameState.encounterSteps reaches it.
  work->encounterThreshold = 16 * Q16_ONE;
  BattleBackground_SelectForMap();
}

/// Returns the world map encounter zone for the terrain at pos (0 for none),
/// and chooses the battle backdrop for that terrain (gGameState.special).
/// Called by Encounter_RollWorldMap on each step, and by
/// Field_RequestBattleFromEncounterTable and Field_RequestDjinnBattle on the
/// world map, which use only the backdrop.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos) {
  s32 kind;
  WorldMapEncounterRule *rule =
      (WorldMapEncounterRule *)gWorldMap_EncounterRules;
  s32 behavior = WorldMap_GetBattleEnvironmentFar(pos, &kind);
  s32 zone = 0;

  while (rule->kind != -1) {
    if (rule->kind == kind &&
        (rule->behavior == -1 || rule->behavior == behavior) &&
        (rule->flag == -1 || GameFlag_TestFar(rule->flag) == 0)) {
      zone = rule->zone;
      break;
    }
    rule++;
  }
  BattleBackground_SelectForEnvironment(behavior);
  return zone;
}

/// Advances the encounter counter for zone and returns the enemy group to
/// fight (a row of formatn.tsv), or 0 for no battle. steps (16.16) scales how
/// far the counter moves; a battle starts once it reaches encounterThreshold.
/// FLAG_NO_ENCOUNTERS, FLAG_NO_ENCOUNTERS_IN_MAPSEC and gGameState.cloaked
/// (Cloak) prevent battles. FLAG_NO_ENCOUNTERS_IN_MAPSEC is set by Venus
/// Lighthouse while its Reveal path is shown (the international editions), by
/// Mercury Lighthouse while the leader carries the pillar's lights and by the
/// world map's debug warp destination 99, and cleared by every entry that does
/// not keep the party's position. Flag 351 (0x15f), which no Broken Seal C code
/// sets, skips the checks and the counting and draws a group at once. Called by
/// Encounter_RollLayoutStep and Encounter_RollWorldMap; adds to
/// gGameState.encounterSteps, sets or clears work->encounterBias, and on a
/// battle sets gGameState.battleMusic.
s32 Encounter_CountStepsAndSelectFormation(s32 zone, s32 steps) {
  struct FieldWork *work;
  struct EncounterZone *entry;
  s32 rate;
  s32 level;
  s32 bias;
  s32 step;
  s32 total;
  s32 sum;
  s32 pick;
  s32 i;

  work = (struct FieldWork *)gFieldWork;
  // UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting entry.
  // Hoisting the zone lookup changes native instructions at the same 440-byte
  // size in tbs-en.
#ifdef UBFIX
  entry = (struct EncounterZone *)gEncounterZones + zone;
#endif
  if (GameFlag_TestFar(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
    if (GameFlag_TestFar(FLAG_NO_ENCOUNTERS) != 0 ||
        GameFlag_TestFar(FLAG_NO_ENCOUNTERS_IN_MAPSEC) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gGameState.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounterZones + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    // Flag 5 is FLAG_IN_PARTY(JENNA): with Jenna in the party, there are no
    // encounters while she has more than 130 experience points. She is in
    // the party only from Vale (EventScript_RoofMended, map/vale.c) to the Sol
    // Sanctum's eruption, starting at level 1 with none, so this ends random
    // battles in the Sol Sanctum once she nears level 4 (158). XXX: likely a
    // cap on training before the journey, because it can act nowhere else.
    if (GameFlag_TestFar(5) != 0) {
      level = Unit_GetFar(5)->experience;
      if (level > 130)
        return 0;
    }

    // Each level (up to five) the party's average is above the zone's raises
    // the rate by 5; with Avoid active such a zone has no encounters at all.
    level = Party_GetAverageLevelFar() - entry->level;
    CLAMP(level, 0, 5);
    if (level > 0 && gGameState.avoidDistance != 0)
      return 0;
    rate += level * 5;

    // A random bias varies the distance between battles: two differences of
    // random numbers, halved, so from -65535 to 65535 and most often near 0.
    // It is drawn again after each battle.
    bias = work->encounterBias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->encounterBias = bias;
    }
    // The rate and the bias divide the step, so a higher rate (a party above
    // the zone's level) means smaller steps and fewer battles. The counter is
    // not cleared here: Map_Run clears it when it starts the battle.
    step =
        Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = gGameState.encounterSteps += Iwram_MulQ16(step, steps);
    if (total < work->encounterThreshold)
      return 0;
  }

  // Draw one of the eight groups by weight.
  work->encounterBias = 0;
  sum = 0;
  for (i = 0; i < ENCOUNTER_GROUP_COUNT; i++)
    sum += entry->weights[i];
  // Zone zero stays encounter-free even when flag 351 bypasses the rate checks.
  if (sum == 0)
    return 0;
  // Random16 is below 65536, so this draw is below sum and must stop in 0..7.
  pick = (Random16() * sum) >> 16;
  pick -= entry->weights[0];
  i = 0;
  if (pick >= 0) {
    do {
      i++;
      if (i >= ENCOUNTER_GROUP_COUNT)
        break;
      pick -= entry->weights[i];
    } while (pick >= 0);
  }
  sum = entry->enemies[i];
  Encounter_SelectBattleMusic(zone, i);
  return sum;
}

/// Rolls a random battle on the encounter zone of the ground underfoot.
/// terrain: the ground, an index into work->encounterZones: 0 for ordinary
/// ground, 1 and 2 for MB_ENCOUNTER_GROUND_1 and _2 cells.
/// steps: the step weight, passed on to Encounter_CountStepsAndSelectFormation.
/// Returns the enemy group to fight, or 0 for no battle.
/// Called by Field_ProcessStep in a map, which stores the result in
/// work->battle; steps is the leader's speed limit, doubled with FLAG_LURE_CAP.
s32 Encounter_RollLayoutStep(u32 terrain, s32 steps) {
  u8 *zone;

  zone = ((struct FieldWork *)gFieldWork)->encounterZones;
  return Encounter_CountStepsAndSelectFormation(zone[terrain], steps);
}

/// Rolls a random battle on the world map, taking the zone from the tile at
/// pos. Returns the enemy group to fight, or 0 for no battle. Called by
/// Field_ProcessStep on the world map.
s32 Encounter_RollWorldMap(s32 pos, s32 steps) {
  return Encounter_CountStepsAndSelectFormation(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// Returns the enemy group in slot (0 to 7) of zone; 14 halfwords make a zone.
/// Called by Field_RequestBattleFromEncounterTable,
/// Encounter_GetDjinnFormation and the map event runner (events.c,
/// ENCOUNTER_ZONE_MIMIC).
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot) {
  // A zone's halfwords are rate, level, then the eight groups.
  return gEncounterZones[zone * 14 + slot + 2];
}

/// Returns the enemy group of Djinni battle djinni, which the table gives as
/// a zone and slot. Called by Field_RequestDjinnBattle.
s16 Encounter_GetDjinnFormation(s32 djinni) {
  u16 *zoneAndSlot;

  zoneAndSlot = (u16 *)&gEncounter_AreaEntryTable[djinni * 2];
#ifdef UBFIX
  return Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the result of the call is left in r0.
  Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

/// A rule choosing a map's field music (gMap_MusicRules,
/// map_rules.s), ended by id -1.
struct MapMusicRule {
  s16 id;            ///< a map, or a map section
  s16 warpDest : 15; ///< -1 for any warp destination
  u16 idIsMap : 1;   ///< 1 when id is a map, 0 when it is a map section
  s16 flag;          ///< -1, or a flag that must be set
  s16 music;         ///< the song (songs.h's MUS_ numbers)
};

extern const struct MapMusicRule gMap_MusicRules[];

/// Chooses the current map's field music (gGameState.cue) from the first rule
/// matching the map or its map section and the warp destination;
/// SONG_KEEP_BGM when none does. Called by Game_Run on entering each map,
/// before Map_PlayMusic plays it.
void Map_ChooseMusic(void) {
  s16 music = SONG_KEEP_BGM;
  s16 mapId = gGameState.mapId;
  s16 warpDest = gGameState.warpDest;
  s16 group = gGameState.mapSec;
  const struct MapMusicRule *rule = gMap_MusicRules;

  for (; rule->id != -1; rule++) {
    if (rule->idIsMap) {
      if (rule->id != mapId)
        continue;
      if (rule->warpDest != -1 && rule->warpDest != warpDest)
        continue;
      if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    }
    if (rule->id != group)
      continue;
    if (rule->warpDest != -1 && rule->warpDest != warpDest)
      continue;
    if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
      continue;
    music = rule->music;
    break;
  }

  gGameState.mapMusic = music;
}

/// A rule naming the map section a map is in (gMapSec_NameRules, map_rules.s),
/// ended by id -1.
struct MapSecNameRule {
  s16 id;            ///< a map, or a map section
  s16 warpDest : 15; ///< -1 for any warp destination
  u16 useMapId : 1;  ///< 1 when id is a map, 0 when it is a map section
  /// The area name, as an offset that most readers add to MsgDebugEntryName.
  /// 0 is the title screen's name (map section 0's rule), which a map no rule
  /// matches also gets.
  s32 areaNameIndex;
};

extern const struct MapSecNameRule gMapSec_NameRules[];

/// Returns the area name of mapId and warpDest, from the first rule matching
/// the map or its map section; 0, the title screen's name, when none does. No
/// reader treats 0 apart. Called by the save, debug and
/// location windows through MapSec_GetNameIndexFar, and by
/// RegionMap_UpdateMarkers; most add it to MsgDebugEntryName for the
/// message. The save stores it as its area (save_state.h).
s32 MapSec_GetNameIndex(s32 mapId, s32 warpDest) {
  const struct MapSecNameRule *rule = gMapSec_NameRules;
  s32 name = 0;
  s32 group = Map_GetMapSec(mapId);

  while (rule->id != -1) {
    if (rule->useMapId) {
      if (rule->id == mapId) {
        if (rule->warpDest == -1 || rule->warpDest == warpDest) {
          name = rule->areaNameIndex;
          break;
        }
      }
    } else {
      if (rule->id == group) {
        if (rule->warpDest == -1 || rule->warpDest == warpDest) {
          name = rule->areaNameIndex;
          break;
        }
      }
    }
    rule++;
  }
  return name;
}

/// Sets the map the party returns to after a defeat (gGameState.savedMap
/// and savedWarpDest), from the first rule of gParty_PairResolveRules that
/// matches the current map and warp destination: {map, warp destination (-1
/// for any), return map, return warp destination}, ended by map -1. Without a
/// match the return point stays as it was. Called by Map_Run on map load.
void Map_SetDefeatReturnPoint(void) {
  s16 mapId = gGameState.mapId;
  s16 warpDest = gGameState.warpDest;
  const s16 *rule = gParty_PairResolveRules;

  while (rule[0] != -1) {
    if (rule[0] == mapId && (rule[1] == -1 || rule[1] == warpDest)) {
      gGameState.savedMap = rule[2];
      gGameState.savedWarpDest = rule[3];
      return;
    }
    rule += 4;
  }
}

/// Returns the battle backdrop chosen for the current map. The battle setup
/// (battle/encounter.c) reads it through FieldScript_GetSpecialValueFar as the
/// battle's background.
s16 FieldScript_GetSpecialValue(void) { return gGameState.special; }

/// Chooses the battle backdrop for the current map. gBattleBackground_Maps
/// lists each backdrop (a word with bit 31 set) followed by the maps that
/// use it, ended by 0; a map takes the backdrop listed last before it, and a
/// map not listed takes the last backdrop. Called by
/// Encounter_SelectMapZones; sets gGameState.special.
void BattleBackground_SelectForMap(void) {
  s32 target = gGameState.mapId;
  s32 *table = gBattleBackground_Maps;
  s32 entry = *table++;
  // The first map_rules.s word is a backdrop marker. It cannot match the
  // halfword map id, so the loop sets this before testing any map.
  s32 backdrop;

  if (entry != 0 && entry != target) {
    do {
      // A backdrop marker: bit 31 set, the backdrop's resource id below.
      if (entry & 0x80000000) {
        backdrop = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  gGameState.special = backdrop;
}

/// Chooses the battle backdrop for a world map terrain behavior (enum
/// BattleEnvironment; any other gives Sol Sanctum's) and sets
/// gGameState.special. Called by Encounter_ResolveWorldMapZoneAndBackdrop.
void BattleBackground_SelectForEnvironment(s32 behavior) {
  u16 backdrop;

  switch (behavior - 1) {
  case BATTLE_ENVIRONMENT_GRASSLAND - 1:
    backdrop = (u32)&ResourceId_GrasslandBackdrop;
    break;
  case BATTLE_ENVIRONMENT_FOREST - 1:
    backdrop = (u32)&ResourceId_ForestBackdrop;
    break;
  case BATTLE_ENVIRONMENT_DESERT - 1:
    backdrop = (u32)&ResourceId_DesertBackdrop;
    break;
  case BATTLE_ENVIRONMENT_BEACH - 1:
  case BATTLE_ENVIRONMENT_SHALLOWS - 1:
    backdrop = (u32)&ResourceId_BeachBackdrop;
    break;
  case BATTLE_ENVIRONMENT_SNOWFIELD - 1:
  case BATTLE_ENVIRONMENT_SNOW_FOREST - 1:
    backdrop = (u32)&ResourceId_SnowfieldBackdrop;
    break;
  default:
    backdrop = (u32)&ResourceId_SolSanctumBackdrop;
    break;
  }
  gGameState.special = backdrop;
}

/// Chooses the battle music for slot of zone (gGameState.battleMusic).
/// gEncounter_BattleMusic lists each song (with bit 15 set) followed by the
/// (zone << 4) + slot keys that use it, ended by 0; a key takes the song listed
/// last before it. Link battles always use SONG_KEEP_BGM. Called by
/// Encounter_CountStepsAndSelectFormation for a random battle, by the battle
/// requests (Field_RequestBattleFromEncounterTable, Field_RequestDjinnBattle
/// with zone 0) and by the map event runner (events.c).
void Encounter_SelectBattleMusic(s32 zone, s32 slot) {
  u16 key = (zone << 4) + slot;
  const u16 *table = gEncounter_BattleMusic;
  // Encounter keys have bit 15 clear. battle_cue.c starts with a nonzero
  // song marker, so the loop assigns music before either stopping test holds.
  u16 music;
  u16 value;

  if (GameFlag_TestFar(FLAG_LINK_BATTLE)) {
    music = SONG_KEEP_BGM;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == key)
        break;
      // A song marker: bit 15 set, the song in the low 12 bits.
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  gGameState.battleMusic = music;
}
