/// @file
/// Random encounters, and each scene's music, area name and battle background.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// Chooses the current scene's encounter zones, one for each kind of ground the
/// party can stand on (gFieldWork->encounterZones, read by
/// Encounter_RollMapStep), from the first rule matching the scene and entrance.
void Encounter_SelectMapZones(s32 useRules) {
  struct FieldWork *work;
  s32 sceneId;
  s32 entrance;
  u8 *groundZones;
  const struct SceneEncounterRule *rule;
  s32 zone;
  s16 firstZoneOnGround;
  u32 i;

  work = gFieldWork;
  sceneId = gMainRoutineWork.next;
  entrance = gMainRoutineWork.entrance;
  groundZones = work->encounterZones;
  rule = gScene_EncounterRules;
  zone = 0;
  firstZoneOnGround = 0;
  if (useRules != 0) {
    for (; rule->sceneId != -1; rule++) {
      if (rule->sceneId == sceneId &&
          (rule->entrance == -1 || rule->entrance == entrance)) {
        if (rule->flag == -1 || GameFlag_GetFar(rule->flag) == 0) {
          zone = rule->firstZone;
          firstZoneOnGround = rule->firstZoneOnGround;
          break;
        }
      }
    }
  }

  // Ground zero is normally safe; the rule can move the first zone onto it.
  *groundZones++ = 0;
  for (i = 0; i < ARRAY_COUNT(work->encounterZones) - 1; i++) {
    *groundZones++ = zone;
    if (zone != 0)
      zone++;
  }
  if (firstZoneOnGround != 0) {
    work->encounterZones[0] = work->encounterZones[1];
    work->encounterZones[1] = 0;
  }
  work->encounterBias = 0;
  // Encounter_CountStepsAndSelectFormation starts a battle once
  // gMainRoutineWork.encounterSteps reaches it.
  work->encounterThreshold = toFIXED(16);
  BattleBackground_SelectForMap();
}

/// Returns the World Map encounter zone for the ground at pos (0 for none), and
/// chooses the battle background for its battle environment
/// (gMainRoutineWork.battleBackground).
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos) {
  s32 region;
  const struct WorldMapEncounterRule *rule = gWorldMap_EncounterRules;
  s32 environment = WorldMap_GetBattleEnvironmentFar(pos, &region);
  s32 zone = 0;

  for (; rule->region != -1; rule++) {
    if (rule->region == region &&
        (rule->environment == -1 || rule->environment == environment) &&
        (rule->flag == -1 || GameFlag_GetFar(rule->flag) == 0)) {
      zone = rule->zone;
      break;
    }
  }
  BattleBackground_SelectForEnvironment(environment);
  return zone;
}

/// With Jenna in the party, random battles stop once her experience is past
/// this.
#define JENNA_ENCOUNTERS_EXPERIENCE_MAX 130
/// A battle background rule's word (battle_background_rules.c's
/// BATTLE_BACKGROUND): bit 31 marks a battle background, its resource id below;
/// any other word is a scene.
#define BATTLE_BACKGROUND_RULE 0x80000000
#define BATTLE_BACKGROUND_RULE_RESOURCE 0xffff

/// Advances the encounter counter for zone and returns the formation to fight
/// (a row of formations.c), or 0 for no battle. steps (16.16) scales how far
/// the counter moves; a battle starts once it reaches encounterThreshold.
s32 Encounter_CountStepsAndSelectFormation(s32 zone, FIXED steps) {
  struct FieldWork *work;
  struct EncounterZone *entry;
  s32 rate;
  s32 level;
  s32 bias;
  FIXED step;
  s32 total;
  s32 sum;
  s32 pick;
  s32 i;

  work = gFieldWork;
  // UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting entry.
#ifdef UBFIX
  entry = (struct EncounterZone *)gEncounterZones + zone;
#endif
  if (GameFlag_GetFar(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
    if (GameFlag_GetFar(FLAG_NO_ENCOUNTERS) != 0 ||
        GameFlag_GetFar(FLAG_NO_ENCOUNTERS_IN_AREA) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gMainRoutineWork.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounterZones + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    if (GameFlag_GetFar(FLAG_IN_PARTY(JENNA)) != 0) {
      level = Unit_GetFar(JENNA)->experience;
      if (level > JENNA_ENCOUNTERS_EXPERIENCE_MAX)
        return 0;
    }

    // Each level (up to five) the party's average is above the zone's raises
    // the rate by 5; with Avoid active such a zone has no encounters at all.
    level = Party_GetAverageLevelFar() - entry->level;
    CLAMP(level, 0, 5);
    if (level > 0 && gMainRoutineWork.avoidDistance != 0)
      return 0;
    rate += level * 5;

    // A random bias varies the distance between battles: two differences of
    // random numbers, halved, so from -65535 to 65535 and most often near 0.
    bias = work->encounterBias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->encounterBias = bias;
    }
    // The rate and the bias divide the step, so a higher rate (a party above
    // the zone's level) means smaller steps and fewer battles: a step is
    // 16 / (16 * rate + 16 * (rate - 1) * bias), that is
    // 1 / (rate + (rate - 1) * bias) with bias a 16.16 fraction from -1 to 1.
    step = Iwram_RatioMulQ14(rate * toFIXED(16) + (rate - 1) * 16 * bias,
                             toFIXED(16));
    total = gMainRoutineWork.encounterSteps += Iwram_MulFX(step, steps);
    if (total < work->encounterThreshold)
      return 0;
  }

  // Draw one of the zone's formations by weight.
  work->encounterBias = 0;
  sum = 0;
  for (i = 0; i < ENCOUNTER_GROUP_COUNT; i++)
    sum += entry->weights[i];
  // Zone zero stays encounter-free even when FLAG_IGNORE_ENCOUNTER_CHECKS
  // bypasses the rate checks.
  if (sum == 0)
    return 0;
  // Random16 is below 65536, so this draw is below sum and must stop at one of
  // the ENCOUNTER_GROUP_COUNT formations.
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
  sum = entry->formations[i];
  Encounter_SelectBattleMusic(zone, i);
  return sum;
}

/// Rolls a random battle on the encounter zone of the ground underfoot.
/// terrain: the ground, an index into work->encounterZones: 0 for ordinary
/// ground, 1 and 2 for CELL_BEHAVIOR_ENCOUNTER_GROUND_A and _B cells. steps:
/// the step weight, passed on to Encounter_CountStepsAndSelectFormation.
s32 Encounter_RollMapStep(u32 terrain, FIXED steps) {
  u8 *zone;

  zone = gFieldWork->encounterZones;
  return Encounter_CountStepsAndSelectFormation(zone[terrain], steps);
}

/// Rolls a random battle on the World Map, taking the zone from the cell at
/// pos. Returns the formation to fight, or 0 for no battle.
s32 Encounter_RollWorldMap(s32 pos, FIXED steps) {
  return Encounter_CountStepsAndSelectFormation(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// A struct EncounterZone in halfwords, and where its formations start.
#define ENCOUNTER_ZONE_HALFWORDS                                               \
  ((s32)(sizeof(struct EncounterZone) / sizeof(u16)))
#define ENCOUNTER_ZONE_FORMATIONS                                              \
  ((s32)(offsetof(struct EncounterZone, formations) / sizeof(u16)))

/// Returns the formation in slot (0 to 7) of zone, read from gEncounterZones as
/// halfwords.
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot) {
  return gEncounterZones[zone * ENCOUNTER_ZONE_HALFWORDS + slot +
                         ENCOUNTER_ZONE_FORMATIONS];
}

/// Returns the formation of Djinni battle djinni, which the table gives as a
/// zone and slot.
s16 Encounter_GetDjinnFormation(s32 djinni) {
  const struct DjinniBattle *battle = &gEncounter_DjinnBattles[djinni];

#ifdef UBFIX
  return Encounter_GetFormationAtSlot(battle->zone, battle->slot);
#else
  // UB: no return statement; callers get the formation only because it is the
  // last call's result.
  Encounter_GetFormationAtSlot(battle->zone, battle->slot);
#endif
}

/// Chooses the current scene's field music (gMainRoutineWork.cue) from the
/// first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM
/// when none does.
void Scene_ChooseMusic(void) {
  s16 music = SOUND_KEEP_BGM;
  s16 sceneId = gMainRoutineWork.next;
  s16 entrance = gMainRoutineWork.entrance;
  s16 area = gMainRoutineWork.area;
  const struct MapMusicRule *rule = gScene_MusicRules;

  for (; rule->id != -1; rule++) {
    if (rule->idIsMap) {
      if (rule->id != sceneId)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_GetFar(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    }
    if (rule->id != area)
      continue;
    if (rule->entrance != -1 && rule->entrance != entrance)
      continue;
    if (rule->flag != -1 && GameFlag_GetFar(rule->flag) == 0)
      continue;
    music = rule->music;
    break;
  }

  gMainRoutineWork.sceneMusic = music;
}

/// Returns the area name of sceneId and entrance, from the first rule matching
/// the scene or its area; 0, the Title's name, when none does.
s32 Area_GetNameIndex(s32 sceneId, s32 entrance) {
  const struct AreaNameRule *rule = gArea_NameRules;
  s32 name = 0;
  s32 area = Scene_GetArea(sceneId);

  for (; rule->id != -1; rule++) {
    if (rule->useMapId) {
      if (rule->id == sceneId) {
        if (rule->entrance == -1 || rule->entrance == entrance) {
          name = rule->areaNameIndex;
          break;
        }
      }
    } else {
      if (rule->id == area) {
        if (rule->entrance == -1 || rule->entrance == entrance) {
          name = rule->areaNameIndex;
          break;
        }
      }
    }
  }
  return name;
}

/// Sets the scene the party returns to after a defeat
/// (gMainRoutineWork.restartScene and restartEntrance), from the first rule of
/// gScene_RestartRules that matches the current scene and entrance: {scene,
/// entrance (-1 for any), return scene, return entrance}, ended by scene -1.
void Scene_SetRestartPoint(void) {
  s16 sceneId = gMainRoutineWork.next;
  s16 entrance = gMainRoutineWork.entrance;
  const s16 *rule;

  for (rule = gScene_RestartRules; rule[0] != -1; rule += 4) {
    if (rule[0] == sceneId && (rule[1] == -1 || rule[1] == entrance)) {
      gMainRoutineWork.restartScene = rule[2];
      gMainRoutineWork.restartEntrance = rule[3];
      return;
    }
  }
}

/// Returns the battle background chosen for the current scene.
s16 BattleBackground_GetSelected(void) {
  return gMainRoutineWork.battleBackground;
}

/// Chooses the battle background for the current scene.
/// gScene_BattleBackgroundRules lists each battle background (a word with bit
/// 31 set) followed by the scenes that use it, ended by 0; a scene takes the
/// one listed last before it, and a scene not listed takes the last one.
void BattleBackground_SelectForMap(void) {
  s32 target = gMainRoutineWork.next;
  s32 *table = gScene_BattleBackgroundRules;
  s32 entry = *table++;
  // The first battle_background_rules.c word names a battle background.
  s32 backdrop;

  while (entry != 0 && entry != target) {
    if (entry & BATTLE_BACKGROUND_RULE) {
      backdrop = entry & BATTLE_BACKGROUND_RULE_RESOURCE;
    }
    entry = *table++;
  }
  gMainRoutineWork.battleBackground = backdrop;
}

/// Chooses the battle background for a World Map battle environment (enum
/// BattleEnvironment; any other gives Sol Sanctum's) and sets
/// gMainRoutineWork.battleBackground.
void BattleBackground_SelectForEnvironment(s32 environment) {
  u16 backdrop;

  switch (environment) {
  case BATTLE_ENVIRONMENT_GRASSLAND:
    backdrop = RESOURCE_GRASSLAND_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_FOREST:
    backdrop = RESOURCE_FOREST_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_DESERT:
    backdrop = RESOURCE_DESERT_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_BEACH:
  case BATTLE_ENVIRONMENT_SHALLOWS:
    backdrop = RESOURCE_BEACH_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_SNOWFIELD:
  case BATTLE_ENVIRONMENT_SNOW_FOREST:
    backdrop = RESOURCE_SNOWFIELD_BACKDROP;
    break;
  default:
    backdrop = RESOURCE_SOL_SANCTUM_BACKDROP;
    break;
  }
  gMainRoutineWork.battleBackground = backdrop;
}

/// Chooses the battle music for slot of zone (gMainRoutineWork.battleMusic).
void Encounter_SelectBattleMusic(s32 zone, s32 slot) {
  u16 key = ENCOUNTER_KEY(zone, slot);
  const u16 *table = gEncounter_BattleMusic;
  // gEncounter_BattleMusic opens with a music marker, so the loop assigns music
  // before either stopping test holds.
  u16 music;
  u16 value;

  if (GameFlag_GetFar(FLAG_LINK_BATTLE)) {
    music = SOUND_KEEP_BGM;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == key)
        break;
      if (value & ENCOUNTER_BATTLE_MUSIC_MARKER)
        music = value & ENCOUNTER_BATTLE_MUSIC_SONG_MASK;
    }
  }
  gMainRoutineWork.battleMusic = music;
}

#undef JENNA_ENCOUNTERS_EXPERIENCE_MAX
#undef BATTLE_BACKGROUND_RULE
#undef BATTLE_BACKGROUND_RULE_RESOURCE
#undef ENCOUNTER_ZONE_HALFWORDS
#undef ENCOUNTER_ZONE_FORMATIONS
