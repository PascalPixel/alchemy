/// @file
/// Random encounters, and each scene's music, place name and backdrop.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// A field scene's encounter zones (gBattle_LocationRules, location_rules.s),
/// ended by scene -1.
struct BattleLocationRule {
  s16 scene;
  s16 entrance;              ///< -1 for any entrance
  s16 flag : 15;             ///< -1, or a flag that turns the rule off once set
  s16 firstZoneOnGround : 1; ///< the first zone is for ground 0, not 1
  s16 firstZone;             ///< the first of up to seven consecutive zones
};

extern const struct BattleLocationRule gBattle_LocationRules[];

/// The world map's encounter zone for each kind of terrain
/// (gWorldMap_EncounterRules, zonedata.c), ended by kind -1.
typedef struct TerrainRule {
  s16 kind;     ///< the terrain kind of the map block
  s16 behavior; ///< the terrain's behavior, or -1 for any
  s16 flag;     ///< -1, or a flag that turns the rule off once set
  s16 zone;
} TerrainRule;

/// The number of enemy groups an encounter zone can hold.
enum { ENCOUNTER_GROUP_COUNT = 8 };

/// An encounter zone, a row of encounter.tsv: its rate, the level it is made
/// for, and the enemy groups that can appear with their chances.
struct EncounterZone {
  u16 rate;                           ///< the base encounter rate
  u16 level;                          ///< the party level the zone is made for
  u16 enemies[ENCOUNTER_GROUP_COUNT]; ///< the enemy groups that can appear
  u8 weights[ENCOUNTER_GROUP_COUNT];  ///< each group's relative chance
};

/// Chooses the current scene's encounter zones, one for each kind of ground
/// the party can stand on (gEventWork->encounterGroups, read by
/// EffectRuntime_LookupByTableEntry), from the first rule matching the scene
/// and entrance. Ground 0 has none, and grounds 1 to 7 take consecutive zones
/// from the rule's first one. With useRules 0, or no matching rule, no ground
/// has encounters. Also resets the encounter counters and chooses the battle
/// backdrop.
void Encounter_SelectSceneZones(s32 useRules) {
  struct FieldWork *work;
  s32 scene;
  s32 entrance;
  u8 *groundZones;
  const struct BattleLocationRule *rule;
  s32 zone;
  s16 firstZoneOnGround;
  u32 i;

  work = (struct FieldWork *)gEventWork;
  scene = gGameState.scene;
  entrance = gGameState.entrance;
  groundZones = work->encounterGroups;
  rule = gBattle_LocationRules;
  zone = 0;
  firstZoneOnGround = 0;
  if (useRules != 0) {
    while (rule->scene != -1) {
      if (rule->scene == scene &&
          (rule->entrance == -1 || rule->entrance == entrance)) {
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
    work->encounterGroups[0] = work->encounterGroups[1];
    work->encounterGroups[1] = 0;
  }
  work->encounterBias = 0;
  work->encounterThreshold = 0x100000;
  Battle_SelectSceneBackdrop();
}

/// Returns the world map encounter zone for the terrain at pos (0 for none),
/// and chooses the battle backdrop for that terrain.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos) {
  s32 kind;
  TerrainRule *rule = gWorldMap_EncounterRules;
  s32 behavior = GetWorldMapTerrainBehaviorFar(pos, &kind);
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
  Battle_SelectTerrainBackdrop(behavior);
  return zone;
}

/// Advances the encounter counter for zone and returns the enemy group to
/// fight, or 0 for no battle. steps (16.16) scales how far the counter moves;
/// a battle starts once it reaches encounterThreshold. Flags 352 and 353 and
/// gGameState.cloaked prevent battles. Flag 351 skips the checks and the
/// counting and draws a group at once, from an entry this function never sets.
s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps) {
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

  work = (struct FieldWork *)gEventWork;
  if (GameFlag_TestFar(351) == 0) {
    if (GameFlag_TestFar(352) != 0 || GameFlag_TestFar(353) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gGameState.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounter_EnemyGroupTable + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    // With flag 5 set, there are no encounters while unit 5 has more than 130
    // experience points.
    if (GameFlag_TestFar(5) != 0) {
      level = UnitGetFar(5)->experience;
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

    // A random bias, the sum of four random numbers, varies the distance
    // between battles; it is drawn again after each battle.
    bias = work->encounterBias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->encounterBias = bias;
    }
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
  if (sum == 0)
    return 0;
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
  BattleFx_SelectBattleCue(zone, i);
  return sum;
}

/// Rolls a random battle on the encounter zone of the ground underfoot.
/// terrain: the terrain number, 0 for ordinary ground.
/// steps: the step weight, passed on to Encounter_SelectEnemyGroup.
/// Returns the enemy group to fight, or 0 for no battle.
s32 Encounter_RollTerrain(u32 terrain, s32 steps) {
  u8 *zone;

  zone = ((struct FieldWork *)gEventWork)->encounterGroups;
  return Encounter_SelectEnemyGroup(zone[terrain], steps);
}

/// Rolls a random battle on the world map, taking the zone from the tile at
/// pos. Returns the enemy group to fight, or 0 for no battle.
s32 Encounter_RollWorldMap(s32 pos, s32 steps) {
  return Encounter_SelectEnemyGroup(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// Returns the enemy group in slot (0 to 7) of zone; 14 halfwords make a zone.
u16 Encounter_GetEnemyGroupAtSlot(s32 zone, s32 slot) {
  return gEncounter_EnemyGroupTable[zone * 14 + slot + 2];
}

/// Returns the enemy group of Djinni battle djinni, which the table gives as
/// a zone and slot.
s16 Encounter_GetDjinnEnemyGroup(s32 djinni) {
  u16 *zoneAndSlot;

  zoneAndSlot = (u16 *)&gEncounter_AreaEntryTable[djinni * 2];
#ifdef UBFIX
  return Encounter_GetEnemyGroupAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the result of the call is left in r0.
  Encounter_GetEnemyGroupAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

/// A rule choosing a scene's field music (gScene_InteractionRuleTable,
/// scene_rules.s), ended by id -1.
struct SceneInteractionEntry {
  s16 id;            ///< a scene, or a scene group
  s16 entrance : 15; ///< -1 for any entrance
  u16 idIsScene : 1; ///< 1 when id is a scene, 0 when it is a group
  s16 flag;          ///< -1, or a flag that must be set
  s16 music;
};

extern const struct SceneInteractionEntry gScene_InteractionRuleTable[];

/// Chooses the current scene's field music (gGameState.cue) from the first rule
/// matching the scene or its group and the entrance; MUS_018 when none does.
void Scene_ResolveInteractionResult(void) {
  s16 music = MUS_018;
  s16 scene = gGameState.scene;
  s16 entrance = gGameState.entrance;
  s16 group = gGameState.sceneGroup;
  const struct SceneInteractionEntry *rule = gScene_InteractionRuleTable;

  for (; rule->id != -1; rule++) {
    if (rule->idIsScene) {
      if (rule->id != scene)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    }
    if (rule->id != group)
      continue;
    if (rule->entrance != -1 && rule->entrance != entrance)
      continue;
    if (rule->flag != -1 && GameFlag_TestFar(rule->flag) == 0)
      continue;
    music = rule->music;
    break;
  }

  gGameState.cue = music;
}

/// A rule naming the place a scene is in (gScene_AreaNameRules, scene_rules.s),
/// ended by id -1.
struct SceneAreaNameRule {
  s16 id;             ///< a scene, or a scene group
  s16 entrance : 15;  ///< -1 for any entrance
  u16 useSceneId : 1; ///< 1 when id is a scene, 0 when it is a group
  s32 areaNameIndex;  ///< the place name
};

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

/// Returns the place name of scene and entrance, from the first rule matching
/// the scene or its group; 0 when none does.
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance) {
  const struct SceneAreaNameRule *rule = gScene_AreaNameRules;
  s32 name = 0;
  s32 group = BattleFx_GetResourceGroup(scene);

  while (rule->id != -1) {
    if (rule->useSceneId) {
      if (rule->id == scene) {
        if (rule->entrance == -1 || rule->entrance == entrance) {
          name = rule->areaNameIndex;
          break;
        }
      }
    } else {
      if (rule->id == group) {
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

/// Sets the scene the party returns to after a defeat (gGameState.savedScene
/// and savedEntrance), from the first rule of gParty_PairResolveRules that
/// matches the current scene and entrance: {scene, entrance (-1 for any),
/// return scene, return entrance}, ended by scene -1. Without a match the
/// return point stays as it was.
void Scene_SetDefeatReturnPoint(void) {
  s16 scene = gGameState.scene;
  s16 entrance = gGameState.entrance;
  const s16 *rule = gParty_PairResolveRules;

  while (rule[0] != -1) {
    if (rule[0] == scene && (rule[1] == -1 || rule[1] == entrance)) {
      gGameState.savedScene = rule[2];
      gGameState.savedEntrance = rule[3];
      return;
    }
    rule += 4;
  }
}

/// Returns the battle backdrop chosen for the current scene.
s16 Event_GetSpecialValue(void) { return gGameState.special; }

/// Chooses the battle backdrop for the current scene. gBattle_BackdropScenes
/// lists each backdrop (a word with bit 31 set) followed by the scenes that
/// use it, ended by 0; a scene takes the backdrop listed last before it, and a
/// scene not listed takes the last backdrop.
void Battle_SelectSceneBackdrop(void) {
  s32 target = gGameState.scene;
  s32 *table = gBattle_BackdropScenes;
  s32 entry = *table++;
  s32 backdrop; // unset only for a table that starts with the scene

  if (entry != 0 && entry != target) {
    do {
      if (entry & 0x80000000) {
        backdrop = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  gGameState.special = backdrop;
}

/// Chooses the battle backdrop for a world map terrain behavior.
void Battle_SelectTerrainBackdrop(s32 behavior) {
  u16 backdrop;

  switch (behavior - 1) {
  case 0:
    backdrop = (u32)&ResourceId_GrasslandBackdrop;
    break;
  case 1:
    backdrop = (u32)&ResourceId_ForestBackdrop;
    break;
  case 2:
    backdrop = (u32)&ResourceId_DesertBackdrop;
    break;
  case 3:
  case 6:
    backdrop = (u32)&ResourceId_BeachBackdrop;
    break;
  case 4:
  case 5:
    backdrop = (u32)&ResourceId_SnowfieldBackdrop;
    break;
  default:
    backdrop = (u32)&ResourceId_SolSanctumBackdrop;
    break;
  }
  gGameState.special = backdrop;
}

/// Chooses the battle music for slot of zone (gGameState.sceneCue).
/// gEncounter_BattleMusic lists each song (with bit 15 set) followed by the
/// (zone << 4) + slot keys that use it, ended by 0; a key takes the song listed
/// last before it. Link battles always use MUS_018.
void BattleFx_SelectBattleCue(s32 zone, s32 slot) {
  u16 key = (zone << 4) + slot;
  const u16 *table = gEncounter_BattleMusic;
  u16 music;
  u16 value;

  if (GameFlag_TestFar(FLAG_LINK_BATTLE)) {
    music = MUS_018;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == key)
        break;
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  gGameState.sceneCue = music;
}
