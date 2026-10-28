/// @file
/// Random encounters, and each scene's music, area name and battle background.
// Part of overworld.c.

#include "../overworld_internal.h"

/// A scene's encounter zones (gScene_EncounterRules, encounter_rules.s),
/// ended by scene -1. Encounter_SelectMapZones takes the first rule that
/// matches the scene and entrance and is not turned off.
struct SceneEncounterRule {
  s16 sceneId;               ///< -1 ends the table
  s16 entrance;              ///< -1 for any entrance
  s16 flag : 15;             ///< -1, or a flag that turns the rule off once set
  s16 firstZoneOnGround : 1; ///< the first zone is for ground 0, not 1
  /// The first of up to seven consecutive zones, rows of encounter_zones.c; 0
  /// for none on any ground.
  s16 firstZone;
};

extern const struct SceneEncounterRule gScene_EncounterRules[];

/// The World Map's encounter zone for each encounter region
/// (gWorldMap_EncounterRules, zonedata.c), ended by region -1. Read by
/// Encounter_ResolveWorldMapZoneAndBackdrop, first match first.
typedef struct WorldMapEncounterRule {
  s16 kind; ///< the World Map cell picture's encounter region; -1 ends the
            ///< table
  /// The battle environment, which also picks the battle background (1 to 7,
  /// BattleBackground_SelectForEnvironment), or -1 for any.
  s16 behavior;
  s16 flag; ///< -1, or a flag that turns the rule off once set
  s16 zone; ///< a row of encounter_zones.c
} WorldMapEncounterRule;

/// encounter_zones.c defines the zones; the code reads them through struct
/// EncounterZone or as halfwords (gEncounterZones in battle.h).

/// Chooses the current scene's encounter zones, one for each kind of ground
/// the party can stand on (gFieldWork->encounterZones, read by
/// Encounter_RollMapStep), from the first rule matching the scene
/// and entrance. Ground 0 has none, and grounds 1 to 7 take consecutive
/// zones from the rule's first one. With useRules 0, or no matching rule, no
/// ground has encounters. Also resets the random bias and the battle threshold
/// (the count itself, gSequenceParentWork.encounterSteps, is reset only when a
/// random battle starts or a new area is entered) and chooses the battle
/// background. Called by SceneSequence_Run on scene load, with useRules 0 on
/// the World Map and 1 in a scene; writes work->encounterZones, encounterBias
/// and encounterThreshold, and gSequenceParentWork.special.
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
  sceneId = gSequenceParentWork.sceneId;
  entrance = gSequenceParentWork.entrance;
  groundZones = work->encounterZones;
  rule = gScene_EncounterRules;
  zone = 0;
  firstZoneOnGround = 0;
  if (useRules != 0) {
    while (rule->sceneId != -1) {
      if (rule->sceneId == sceneId &&
          (rule->entrance == -1 || rule->entrance == entrance)) {
        if (rule->flag == -1 || GameFlag_GetFar(rule->flag) == 0) {
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
  // gSequenceParentWork.encounterSteps reaches it.
  work->encounterThreshold = 16 * Q16_ONE;
  BattleBackground_SelectForMap();
}

/// Returns the World Map encounter zone for the ground at pos (0 for none), and
/// chooses the battle background for its battle environment
/// (gSequenceParentWork.special). Called by Encounter_RollWorldMap on each
/// step, and by Field_RequestBattleFromEncounterTable and
/// Field_RequestDjinnBattle on the World Map, which use only the battle
/// background.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos) {
  s32 kind;
  WorldMapEncounterRule *rule =
      (WorldMapEncounterRule *)gWorldMap_EncounterRules;
  s32 behavior = WorldMap_GetBattleEnvironmentFar(pos, &kind);
  s32 zone = 0;

  while (rule->kind != -1) {
    if (rule->kind == kind &&
        (rule->behavior == -1 || rule->behavior == behavior) &&
        (rule->flag == -1 || GameFlag_GetFar(rule->flag) == 0)) {
      zone = rule->zone;
      break;
    }
    rule++;
  }
  BattleBackground_SelectForEnvironment(behavior);
  return zone;
}

/// Advances the encounter counter for zone and returns the formation to fight
/// (a row of formations.c), or 0 for no battle. steps (16.16) scales how far
/// the counter moves; a battle starts once it reaches encounterThreshold.
/// FLAG_NO_ENCOUNTERS, FLAG_NO_ENCOUNTERS_IN_AREA and
/// gSequenceParentWork.cloaked (Cloak) prevent battles.
/// FLAG_NO_ENCOUNTERS_IN_AREA is set by Venus Lighthouse while its Reveal path
/// is shown (the international editions), by Mercury Lighthouse while the
/// leader carries the pillar's lights and by the World Map's debug entrance 99,
/// and cleared by every entry that does not keep the party's position. Flag 351
/// (0x15f), which no Broken Seal C code sets, skips the checks and the counting
/// and draws a formation at once. Called by Encounter_RollMapStep and
/// Encounter_RollWorldMap; adds to gSequenceParentWork.encounterSteps, sets or
/// clears work->encounterBias, and on a battle sets
/// gSequenceParentWork.battleMusic.
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

  work = gFieldWork;
  // UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting entry.
  // Hoisting the zone lookup changes native instructions at the same 440-byte
  // size in tbs-en.
#ifdef UBFIX
  entry = (struct EncounterZone *)gEncounterZones + zone;
#endif
  if (GameFlag_GetFar(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
    if (GameFlag_GetFar(FLAG_NO_ENCOUNTERS) != 0 ||
        GameFlag_GetFar(FLAG_NO_ENCOUNTERS_IN_AREA) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gSequenceParentWork.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounterZones + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    // With Jenna in the party, there are no encounters while she has more
    // than 130 experience points. She is in the party only from Vale
    // (Vale_RoofMended, scene/vale.c) to the Sol Sanctum's eruption,
    // starting at level 1 with none, so the rule acts only in the Sol
    // Sanctum, where it ends random battles once she nears level 4 (158):
    // the party cannot train there before the journey begins.
    if (GameFlag_GetFar(FLAG_IN_PARTY(JENNA)) != 0) {
      level = Unit_GetFar(JENNA)->experience;
      if (level > 130)
        return 0;
    }

    // Each level (up to five) the party's average is above the zone's raises
    // the rate by 5; with Avoid active such a zone has no encounters at all.
    level = Party_GetAverageLevelFar() - entry->level;
    CLAMP(level, 0, 5);
    if (level > 0 && gSequenceParentWork.avoidDistance != 0)
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
    // not cleared here: SceneSequence_Run clears it when it starts the battle.
    step =
        Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = gSequenceParentWork.encounterSteps += Iwram_MulQ16(step, steps);
    if (total < work->encounterThreshold)
      return 0;
  }

  // Draw one of the eight formations by weight.
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
/// ground, 1 and 2 for CELL_BEHAVIOR_ENCOUNTER_GROUND_1 and _2 cells.
/// steps: the step weight, passed on to Encounter_CountStepsAndSelectFormation.
/// Returns the formation to fight, or 0 for no battle.
/// Called by Field_UpdateWalking in a scene, which stores the result in
/// work->encounter; steps is the leader's speed limit, doubled with
/// FLAG_LURE_CAP.
s32 Encounter_RollMapStep(u32 terrain, s32 steps) {
  u8 *zone;

  zone = gFieldWork->encounterZones;
  return Encounter_CountStepsAndSelectFormation(zone[terrain], steps);
}

/// Rolls a random battle on the World Map, taking the zone from the cell at
/// pos. Returns the formation to fight, or 0 for no battle. Called by
/// Field_UpdateWalking on the World Map.
s32 Encounter_RollWorldMap(s32 pos, s32 steps) {
  return Encounter_CountStepsAndSelectFormation(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// Returns the formation in slot (0 to 7) of zone; 14 halfwords make a zone.
/// Called by Field_RequestBattleFromEncounterTable,
/// Encounter_GetDjinnFormation and the trigger runner (triggers.c,
/// ENCOUNTER_ZONE_MIMIC).
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot) {
  // A zone's halfwords are rate, level, then the eight formations.
  return gEncounterZones[zone * 14 + slot + 2];
}

/// Returns the formation of Djinni battle djinni, which the table gives as
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

/// A rule choosing a scene's field music (gScene_MusicRules,
/// scene_rules.s), ended by id -1.
struct MapMusicRule {
  s16 id;            ///< a scene, or an area
  s16 entrance : 15; ///< -1 for any entrance
  u16 idIsMap : 1;   ///< 1 when id is a scene, 0 when it is an area
  s16 flag;          ///< -1, or a flag that must be set
  s16 music;         ///< the song (sounds.h's MUS_ numbers)
};

extern const struct MapMusicRule gScene_MusicRules[];

/// Chooses the current scene's field music (gSequenceParentWork.cue) from the
/// first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM
/// when none does. Called by SequenceParent_Run on entering each scene, before
/// Scene_PlayMusic plays it.
void Scene_ChooseMusic(void) {
  s16 music = SOUND_KEEP_BGM;
  s16 sceneId = gSequenceParentWork.sceneId;
  s16 entrance = gSequenceParentWork.entrance;
  s16 group = gSequenceParentWork.area;
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
    if (rule->id != group)
      continue;
    if (rule->entrance != -1 && rule->entrance != entrance)
      continue;
    if (rule->flag != -1 && GameFlag_GetFar(rule->flag) == 0)
      continue;
    music = rule->music;
    break;
  }

  gSequenceParentWork.sceneMusic = music;
}

/// A rule naming the area a scene is in (gArea_NameRules, scene_rules.s),
/// ended by id -1.
struct AreaNameRule {
  s16 id;            ///< a scene, or an area
  s16 entrance : 15; ///< -1 for any entrance
  u16 useMapId : 1;  ///< 1 when id is a scene, 0 when it is an area
  /// The area name, as an offset that most readers add to MsgDebugEntryName.
  /// 0 is the Title's name (area 0's rule), which a scene no rule
  /// matches also gets.
  s32 areaNameIndex;
};

extern const struct AreaNameRule gArea_NameRules[];

/// Returns the area name of sceneId and entrance, from the first rule matching
/// the scene or its area; 0, the Title's name, when none does. No
/// reader treats 0 apart. Called by the save, debug and
/// area name windows through Area_GetNameIndexFar, and by
/// WorldPicture_UpdateMarkers; most add it to MsgDebugEntryName for the
/// message. The save stores it as its area (save.h).
s32 Area_GetNameIndex(s32 sceneId, s32 entrance) {
  const struct AreaNameRule *rule = gArea_NameRules;
  s32 name = 0;
  s32 area = Scene_GetArea(sceneId);

  while (rule->id != -1) {
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
    rule++;
  }
  return name;
}

/// Sets the scene the party returns to after a defeat
/// (gSequenceParentWork.restartScene and restartEntrance), from the first rule
/// of gScene_RestartRules that matches the current scene and entrance: {scene,
/// entrance (-1 for any), return scene, return entrance}, ended by scene -1.
/// Without a match the restart point stays as it was. Called by
/// SceneSequence_Run on scene load.
void Scene_SetRestartPoint(void) {
  s16 sceneId = gSequenceParentWork.sceneId;
  s16 entrance = gSequenceParentWork.entrance;
  const s16 *rule = gScene_RestartRules;

  while (rule[0] != -1) {
    if (rule[0] == sceneId && (rule[1] == -1 || rule[1] == entrance)) {
      gSequenceParentWork.restartScene = rule[2];
      gSequenceParentWork.restartEntrance = rule[3];
      return;
    }
    rule += 4;
  }
}

/// Returns the battle background chosen for the current scene. The battle setup
/// (battle/encounter.c) reads it through Event_GetSpecialValueFar as the
/// battle's background.
s16 Event_GetSpecialValue(void) { return gSequenceParentWork.special; }

/// Chooses the battle background for the current scene.
/// gScene_BattleBackgroundRules lists each battle background (a word with bit
/// 31 set) followed by the scenes that use it, ended by 0; a scene takes the
/// one listed last before it, and a scene not listed takes the last one. Called
/// by Encounter_SelectMapZones; sets gSequenceParentWork.special.
void BattleBackground_SelectForMap(void) {
  s32 target = gSequenceParentWork.sceneId;
  s32 *table = gScene_BattleBackgroundRules;
  s32 entry = *table++;
  // The first scene_rules.s word names a battle background. It cannot match the
  // halfword scene id, so the loop sets this before testing any scene.
  s32 backdrop;

  if (entry != 0 && entry != target) {
    do {
      // A battle background's word: bit 31 set, its resource id below.
      if (entry & 0x80000000) {
        backdrop = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  gSequenceParentWork.special = backdrop;
}

/// Chooses the battle background for a World Map battle environment (enum
/// BattleEnvironment; any other gives Sol Sanctum's) and sets
/// gSequenceParentWork.special. Called by
/// Encounter_ResolveWorldMapZoneAndBackdrop.
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
  gSequenceParentWork.special = backdrop;
}

/// Chooses the battle music for slot of zone (gSequenceParentWork.battleMusic).
/// gEncounter_BattleMusic lists each song (with bit 15 set) followed by the
/// (zone << 4) + slot keys that use it, ended by 0; a key takes the song listed
/// last before it. Link battles always use SOUND_KEEP_BGM. Called by
/// Encounter_CountStepsAndSelectFormation for a random battle, by the battle
/// requests (Field_RequestBattleFromEncounterTable, Field_RequestDjinnBattle
/// with zone 0) and by the trigger runner (triggers.c).
void Encounter_SelectBattleMusic(s32 zone, s32 slot) {
  u16 key = (zone << 4) + slot;
  const u16 *table = gEncounter_BattleMusic;
  // Encounter keys have bit 15 clear. battle_cue.c starts with a nonzero
  // song marker, so the loop assigns music before either stopping test holds.
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
      // A song marker: bit 15 set, the song in the low 12 bits.
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  gSequenceParentWork.battleMusic = music;
}
