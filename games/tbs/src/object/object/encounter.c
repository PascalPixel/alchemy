/// @file
/// Random encounters, and each scene's music, place name and backdrop.
// Part of object.c.

#include "../object_internal.h"

/// A field scene's encounter zones (gBattle_LocationRules, location_rules.s),
/// ended by scene -1. Encounter_SelectSceneZones takes the first rule that
/// matches the scene and entrance and is not turned off.
struct BattleLocationRule {
  s16 scene;                 ///< -1 ends the table
  s16 entrance;              ///< -1 for any entrance
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
typedef struct TerrainRule {
  s16 kind; ///< the terrain kind of the map block; -1 ends the table
  /// The terrain's behavior, which also picks the backdrop (1 to 7,
  /// Battle_SelectTerrainBackdrop), or -1 for any.
  s16 behavior;
  s16 flag; ///< -1, or a flag that turns the rule off once set
  s16 zone; ///< a row of encounter.tsv
} TerrainRule;

/// The number of enemy groups an encounter zone can hold.
enum { ENCOUNTER_GROUP_COUNT = 8 };

/// An encounter zone, a row of encounter.tsv (gEncounter_EnemyGroupTable): its
/// rate, the level it is made for, and the enemy groups that can appear with
/// their chances. 28 bytes, the 14 halfwords Encounter_GetEnemyGroupAtSlot
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

/// Chooses the current scene's encounter zones, one for each kind of ground
/// the party can stand on (gEventWork->encounterGroups, read by
/// EffectRuntime_LookupByTableEntry), from the first rule matching the scene
/// and entrance. Ground 0 has none, and grounds 1 to 7 take consecutive zones
/// from the rule's first one. With useRules 0, or no matching rule, no ground
/// has encounters. Also resets the random bias and the battle threshold (the
/// count itself, gGameState.encounterSteps, is reset only when a random battle
/// starts or a new scene group is entered) and chooses the battle backdrop.
/// Called by Field_RunScene on scene load, with useRules 0 on the world map and
/// 1 in a field scene; writes work->encounterGroups, encounterBias and
/// encounterThreshold, and gGameState.special.
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
  // 16 in 16.16: Encounter_SelectEnemyGroup starts a battle once
  // gGameState.encounterSteps reaches it.
  work->encounterThreshold = 16 * Q16_ONE;
  Battle_SelectSceneBackdrop();
}

/// Returns the world map encounter zone for the terrain at pos (0 for none),
/// and chooses the battle backdrop for that terrain (gGameState.special).
/// Called by Encounter_RollWorldMap on each step, and by
/// Field_RequestBattleFromEncounterTable and Field_RequestDjinnBattle on the
/// world map, which use only the backdrop.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *pos) {
  s32 kind;
  TerrainRule *rule = (TerrainRule *)gWorldMap_EncounterRules;
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
/// fight (a row of formatn.tsv), or 0 for no battle. steps (16.16) scales how
/// far the counter moves; a battle starts once it reaches encounterThreshold.
/// FLAG_NO_ENCOUNTERS, FLAG_NO_ENCOUNTERS_IN_PLACE and gGameState.cloaked
/// (Cloak) prevent battles. FLAG_NO_ENCOUNTERS_IN_PLACE is set by Venus
/// Lighthouse while its Reveal path is shown (English and later editions), by
/// Mercury Lighthouse while the leader carries the pillar's lights and by the
/// world map's debug entrance 99, and cleared by every entry that does not keep
/// the party's position. Flag 351 (0x15f), which no ☀️ C code sets, skips the
/// checks and the counting and draws a group at once. Called by
/// Encounter_RollTerrain and Encounter_RollWorldMap; adds to
/// gGameState.encounterSteps, sets or clears work->encounterBias, and on a
/// battle sets gGameState.sceneCue.
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
  // UB: flag 351 reaches the draw without setting entry. Hoisting the zone
  // lookup changes native instructions at the same 440-byte size in tbs-en.
#ifdef UBFIX
  entry = (struct EncounterZone *)gEncounter_EnemyGroupTable + zone;
#endif
  if (GameFlag_TestFar(351) == 0) {
    if (GameFlag_TestFar(FLAG_NO_ENCOUNTERS) != 0 ||
        GameFlag_TestFar(FLAG_NO_ENCOUNTERS_IN_PLACE) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gGameState.cloaked != 0)
      return 0;
    entry = (struct EncounterZone *)gEncounter_EnemyGroupTable + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;
    // Flag 5 is FLAG_IN_PARTY(JENNA): with Jenna in the party, there are no
    // encounters while she has more than 130 experience points. XXX: why is
    // unknown. Vale adds her with Garet (Event_RoofMended, scene/vale.c) and
    // the debug battle's presets can add her (battle/battle/debug.c); her
    // experience there, and where she leaves the party, would settle it.
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
    // The step grows with the rate and the bias. The counter is not cleared
    // here: Field_RunScene clears it when it starts the battle.
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
  BattleFx_SelectBattleCue(zone, i);
  return sum;
}

/// Rolls a random battle on the encounter zone of the ground underfoot.
/// terrain: the ground, an index into work->encounterGroups: 0 for ordinary
/// ground, 1 and 2 for MAP_CELL_ENCOUNTER_GROUND_1 and _2 cells.
/// steps: the step weight, passed on to Encounter_SelectEnemyGroup.
/// Returns the enemy group to fight, or 0 for no battle.
/// Called by Field_ProcessStep in a field scene, which stores the result in
/// work->battle; steps is the leader's speed limit, doubled with FLAG_LURE_CAP.
s32 Encounter_RollTerrain(u32 terrain, s32 steps) {
  u8 *zone;

  zone = ((struct FieldWork *)gEventWork)->encounterGroups;
  return Encounter_SelectEnemyGroup(zone[terrain], steps);
}

/// Rolls a random battle on the world map, taking the zone from the tile at
/// pos. Returns the enemy group to fight, or 0 for no battle. Called by
/// Field_ProcessStep on the world map.
s32 Encounter_RollWorldMap(s32 pos, s32 steps) {
  return Encounter_SelectEnemyGroup(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// Returns the enemy group in slot (0 to 7) of zone; 14 halfwords make a zone.
/// Called by Field_RequestBattleFromEncounterTable,
/// Encounter_GetDjinnEnemyGroup and the scene event runner (events.c, zone 99).
u16 Encounter_GetEnemyGroupAtSlot(s32 zone, s32 slot) {
  // A zone's halfwords are rate, level, then the eight groups.
  return gEncounter_EnemyGroupTable[zone * 14 + slot + 2];
}

/// Returns the enemy group of Djinni battle djinni, which the table gives as
/// a zone and slot. Called by Field_RequestDjinnBattle.
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
  s16 music;         ///< the song (songs.h's MUS_ numbers)
};

extern const struct SceneInteractionEntry gScene_InteractionRuleTable[];

/// Chooses the current scene's field music (gGameState.cue) from the first rule
/// matching the scene or its group and the entrance; MUS_018 when none does.
/// Called by Game_Run on entering each field scene, before
/// Audio_PlayCueFromEventWork plays it.
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
  /// The place name, as an offset that most readers add to MsgDebugEntryName.
  /// 0 is the title screen's name (group 0's rule), which a scene no rule
  /// matches also gets.
  s32 areaNameIndex;
};

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

/// Returns the place name of scene and entrance, from the first rule matching
/// the scene or its group; 0, the title screen's name, when none does. No
/// reader treats 0 apart. Called by the save, debug and
/// location windows through Scene_GetAreaNameIndexFar, and by
/// Map_UpdateWorldMapMarkers; most add it to MsgDebugEntryName for the
/// message. The save stores it as its area (save_state.h).
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
/// return point stays as it was. Called by Field_RunScene on scene load.
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

/// Returns the battle backdrop chosen for the current scene. The battle setup
/// (battle/encounter.c) reads it through Event_GetSpecialValueFar as the
/// battle's background.
s16 Event_GetSpecialValue(void) { return gGameState.special; }

/// Chooses the battle backdrop for the current scene. gBattle_BackdropScenes
/// lists each backdrop (a word with bit 31 set) followed by the scenes that
/// use it, ended by 0; a scene takes the backdrop listed last before it, and a
/// scene not listed takes the last backdrop. Called by
/// Encounter_SelectSceneZones; sets gGameState.special.
void Battle_SelectSceneBackdrop(void) {
  s32 target = gGameState.scene;
  s32 *table = gBattle_BackdropScenes;
  s32 entry = *table++;
  // The first scene_rules.s word is a backdrop marker. It cannot match the
  // halfword scene id, so the loop sets this before testing any scene.
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

/// Chooses the battle backdrop for a world map terrain behavior (1 to 7; any
/// other gives Sol Sanctum's) and sets gGameState.special. Called by
/// Encounter_ResolveWorldMapZoneAndBackdrop.
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
/// last before it. Link battles always use MUS_018. Called by
/// Encounter_SelectEnemyGroup for a random battle, by the battle requests
/// (Field_RequestBattleFromEncounterTable, Field_RequestDjinnBattle with
/// zone 0) and by the scene event runner (events.c).
void BattleFx_SelectBattleCue(s32 zone, s32 slot) {
  u16 key = (zone << 4) + slot;
  const u16 *table = gEncounter_BattleMusic;
  // Encounter keys have bit 15 clear. battle_cue.c starts with a nonzero
  // song marker, so the loop assigns music before either stopping test holds.
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
      // A song marker: bit 15 set, the song in the low 12 bits.
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  gGameState.sceneCue = music;
}
