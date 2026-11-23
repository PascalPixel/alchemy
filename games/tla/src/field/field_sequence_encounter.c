/// @file
/// Random encounters, and each scene's music, area name and battle background.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "sounds.h"
#include "game/character_ids.h"
#include "field/encounter_zone.h"
#include "gamepak/far_call/game.h"
#include "field/field_sequence.h"
#include "gamepak/far_call/field.h"
#include "resource_ids.h"

/// Marks the world map place the party has just entered as visited: sets
/// FLAG_PLACE_VISITED of the first place whose visitScene is `id` and whose
/// visitEntrance is `variant` (or any, for -1). The main routine calls it at
/// every scene entry; MapScreen_Show offers the marked places.
void WorldMap_MarkPlaceVisited(s32 id, s32 variant) {
  const struct FlagEntry *entry = FlagEntries_GetTable();

  for (; entry->flag != 0; entry++) {
    if (entry->visitScene != id)
      continue;
    if (entry->visitEntrance != -1 && variant != entry->visitEntrance)
      continue;
    GameFlag_Set(FLAG_PLACE_VISITED(entry->flag));
    break;
  }
}

/// Returns the value of the flag entry whose flag is flag, or 1 when there is
/// none or its value is 0.
s32 FlagEntries_GetValue(s32 flag) {
  const struct FlagEntry *entry = FlagEntries_GetTable();

  for (; entry->flag != 0; entry++) {
    if (entry->flag == flag && entry->value != 0)
      return entry->value;
  }
  return 1;
}

/// Chooses the current scene's encounter zones, one for each kind of ground the
/// party can stand on (gFieldWork->encounterZones, read by
/// Encounter_RollMapStep), from the first rule matching the scene and entrance.
void Encounter_SelectMapZones(s32 useRules) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 sceneId = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  u8 *groundZones = work->encounterZones;
  u32 first = 0;
  u32 second = 0;
  const struct SceneMusicEntry *rule = gScene_EncounterRules;
  u32 i;

  if (useRules != 0) {
    for (; rule->scene != -1; rule++) {
      if (rule->scene != sceneId)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_Get(rule->flag))
        continue;
      first = rule->first;
      second = rule->second;
      break;
    }
  }
  *groundZones++ = first;
  *groundZones++ = second;
  for (i = 0; i < ARRAY_COUNT(work->encounterZones) - 2; i++)
    *groundZones++ = 0;
  work->encounterBias = 0;
  // Encounter_CountStepsAndSelectFormation starts a battle once
  // gMainRoutineWork.encounterSteps reaches it.
  work->encounterThreshold = toFIXED(16);
  BattleBackground_SelectForMap();
}

/// The World Map's encounter regions: a block's region number past them takes
/// the encounters of its number modulo WORLD_MAP_ENCOUNTER_REGIONS (Prox's
/// blizzard, 36, those of 16), the higher numbers choosing only the ground's
/// battle environment (WorldMap_GetBattleEnvironment).
#define WORLD_MAP_ENCOUNTER_REGIONS 20

/// Returns the World Map encounter zone for the ground at position (0 for
/// none), and chooses the battle background for its battle environment
/// (gMainRoutineWork.battleBackground). Flying, there are none; aboard the
/// ship, the zones are gWorldMap_ShipEncounterRules'; a rule's flag must be set
/// for it to apply.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *position) {
  struct TerrainRule *rule = gWorldMap_EncounterRules;
  s32 environment;
  u32 region;
  s32 zone;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode == LEADER_MOVEMENT_FLYING_SHIP)
    return 0;
  if (party->movementMode == LEADER_MOVEMENT_SHIP)
    rule = gWorldMap_ShipEncounterRules;
  environment = WorldMap_GetBattleEnvironmentFar(position, (s32 *)&region);
  region %= WORLD_MAP_ENCOUNTER_REGIONS;
  zone = 0;
  for (; rule->kind != -1; rule++) {
    if (rule->kind != region)
      continue;
    if (rule->behavior != -1 && rule->behavior != environment)
      continue;
    if (rule->flag != -1 && GameFlag_Get(rule->flag) == 0)
      continue;
    zone = rule->zone;
    break;
  }
  BattleBackground_SelectForEnvironment(environment);
  return zone;
}

/// Advances the encounter counter for zone and returns the formation to fight,
/// or 0 for no battle. steps (16.16) scales how far the counter moves; a
/// battle starts once it reaches encounterThreshold.
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
  s32 formation;
  s32 i;

  work = Ram_HeapSlots->fieldWork;
  // UB: FLAG_IGNORE_ENCOUNTER_CHECKS reaches the draw without setting entry.
#ifdef UBFIX
  entry = gEncounterZones + zone;
#endif
  if (GameFlag_Get(FLAG_IGNORE_ENCOUNTER_CHECKS) == 0) {
    if (GameFlag_Get(FLAG_NO_ENCOUNTERS) != 0 ||
        GameFlag_Get(FLAG_NO_ENCOUNTERS_IN_AREA) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gMainRoutineWork.cloaked != 0)
      return 0;
    entry = gEncounterZones + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;

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
  for (i = 0; i < 8; i++)
    sum += entry->weights[i];
  if (sum == 0)
    return 0;
  pick = Random_Below(sum);
  for (i = 0; i < 8; i++) {
    pick -= entry->weights[i];
    if (pick < 0)
      break;
  }
  formation = (u16)entry->formations[i];
  Encounter_SelectBattleMusic(zone, i);
  return formation;
}

/// Rolls a random battle on the encounter zone of the ground underfoot.
/// terrain: the ground, an index into work->encounterZones: 0 for ordinary
/// ground, 1 for a MAP_CELL_SECOND_ZONE cell. steps: the step weight, passed on
/// to Encounter_CountStepsAndSelectFormation.
s32 Encounter_RollMapStep(u32 terrain, FIXED steps) {
  u8 *zone = gFieldWork->encounterZones;

  return Encounter_CountStepsAndSelectFormation(zone[terrain], steps);
}

/// Rolls a random battle on the World Map, taking the zone from the cell at
/// pos. Returns the formation to fight, or 0 for no battle.
s32 Encounter_RollWorldMap(s32 pos, FIXED steps) {
  return Encounter_CountStepsAndSelectFormation(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)pos), steps);
}

/// Returns the formation in slot (0 to 7) of zone.
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot) {
  return gEncounterZones[zone].formations[slot];
}

/// Returns the formation of Djinni battle djinni, which the table gives as a
/// zone and slot.
u16 Encounter_GetDjinnFormation(s32 djinni) {
  u16 *zoneAndSlot = (u16 *)(gEncounter_AreaEntryTable + djinni * 4);

#ifdef UBFIX
  return Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the original leaves the called getter's result in
  // r0.
  Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

/// Chooses the current scene's field music (gMainRoutineWork.cue) from the
/// first rule matching the scene or its area and the entrance; SOUND_KEEP_BGM
/// when none does.
void Scene_ChooseMusic(void) {
  s16 music = SOUND_KEEP_BGM;
  s16 sceneId = gMainRoutineWork.next;
  s16 entrance = gMainRoutineWork.entrance;
  s16 group = gMainRoutineWork.area;
  const struct MapMusicRule *rule = gScene_MusicRules;

  for (; rule->id != -1; rule++) {
    if (rule->byScene) {
      if (rule->id != sceneId)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_Get(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    } else {
      if (rule->id != group)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_Get(rule->flag) == 0)
        continue;
      music = rule->music;
      break;
    }
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
    if (rule->useSceneId) {
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
/// gParty_PairResolveRules that matches the current scene and entrance: rows
/// of eight halfwords that open {scene, entrance (-1 for any), return scene,
/// return entrance}, ended by scene -1. Without a match the restart point stays
/// as it was.
void Scene_SetRestartPoint(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 sceneId;
  s16 entrance;
  const s16 *rule;

  sceneId = party->next;
  entrance = party->entrance;

  for (rule = gParty_PairResolveRules; rule[0] != -1; rule += 8) {
    if (rule[0] == sceneId && (rule[1] == -1 || rule[1] == entrance)) {
      party->restartScene = rule[2];
      party->restartEntrance = rule[3];
      return;
    }
  }
}

/// Where the party comes out on the world map after reaching scene and
/// entrance (Teleport's choice calls it). With FLAG_SHIP_CAN_SAIL set (the
/// party has the ship, whose mooring the map screen marks), finds the
/// first rule of gParty_PairResolveRules for scene and entrance (-1 on either
/// side matches any entrance) and copies its position, x and z in 16.16 and the
/// third value as is, to gMainRoutineWork.shipX, shipZ and shipFacing.
void WorldMap_SetReturnForScene(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (GameFlag_Get(FLAG_SHIP_CAN_SAIL) == 0)
    return;
  for (; rule[0] != -1; rule += 8) {
    if (rule[0] == scene &&
        (rule[1] == -1 || entrance == -1 || rule[1] == entrance)) {
      gMainRoutineWork.shipX = toFIXED(rule[4]);
      gMainRoutineWork.shipZ = toFIXED(rule[5]);
      gMainRoutineWork.shipFacing = rule[6];
      return;
    }
  }
}

/// WorldMap_SetReturnForScene's twin for the rule's return scene and entrance
/// (its third and fourth values): with FLAG_SHIP_CAN_SAIL set, clears
/// ownerOverride and copies the rule's position to the party's saved position.
void WorldMap_SetReturnLeavingScene(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (GameFlag_Get(FLAG_SHIP_CAN_SAIL) == 0)
    return;
  gMainRoutineWork.ownerOverride = 0;
  for (; rule[0] != -1; rule += 8) {
    if (rule[2] == scene &&
        (rule[3] == -1 || entrance == -1 || rule[3] == entrance)) {
      gMainRoutineWork.shipX = toFIXED(rule[4]);
      gMainRoutineWork.shipZ = toFIXED(rule[5]);
      gMainRoutineWork.shipFacing = rule[6];
      return;
    }
  }
}

/// Returns the battle background chosen for the current scene.
s16 BattleBackground_GetSelected(void) {
  return gMainRoutineWork.battleBackground;
}

/// A battle background rule's word (scene_rules.c's BATTLE_BACKGROUND): bit 31
/// marks a battle background, its resource id below; any other word is a
/// scene.
#define BATTLE_BACKGROUND_RULE 0x80000000
#define BATTLE_BACKGROUND_RULE_RESOURCE 0xffff

/// Chooses the battle background for the current scene.
/// gScene_BattleBackgroundRules lists each battle background (a word with bit
/// 31 set) followed by the scenes that use it, ended by 0; a scene takes the
/// one listed last before it, and a scene not listed takes the last one. Once
/// the Mars Lighthouse has thawed, the second dragon hall backdrop becomes the
/// first.
void BattleBackground_SelectForMap(void) {
  s32 target = gMainRoutineWork.next;
  const s32 *table = gScene_BattleBackgroundRules;
  s32 entry = *table++;
  // The first scene_rules.c word names a battle background.
  s32 backdrop;

  if (entry != 0 && entry != target) {
    do {
      if (entry & BATTLE_BACKGROUND_RULE) {
        backdrop = entry & BATTLE_BACKGROUND_RULE_RESOURCE;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  if (backdrop == RESOURCE_DRAGON_HALL_B_BACKDROP &&
      GameFlag_Get(FLAG_MARS_LIGHTHOUSE_THAWED))
    backdrop = RESOURCE_DRAGON_HALL_A_BACKDROP;
  gMainRoutineWork.battleBackground = backdrop;
}

/// Chooses the battle background for a World Map battle environment (enum
/// BattleEnvironment; any other gives the ship deck's) and sets
/// gMainRoutineWork.battleBackground.
void BattleBackground_SelectForEnvironment(s32 environment) {
  u16 backdrop;

  switch (environment - 1) {
  case BATTLE_ENVIRONMENT_GRASSLAND - 1:
    backdrop = RESOURCE_GRASSLAND_B_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_FOREST - 1:
    backdrop = RESOURCE_DENSE_FOREST_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_DESERT - 1:
    backdrop = RESOURCE_DESERT_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_BEACH - 1:
  case BATTLE_ENVIRONMENT_SHALLOWS - 1:
    backdrop = RESOURCE_ROCKY_BEACH_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_SNOWFIELD - 1:
  case BATTLE_ENVIRONMENT_SNOW_FOREST - 1:
    backdrop = RESOURCE_SNOWFIELD_B_BACKDROP;
    break;
  case BATTLE_ENVIRONMENT_BLIZZARD - 1:
  case BATTLE_ENVIRONMENT_BLIZZARD_FOREST - 1:
    backdrop = RESOURCE_BLIZZARD_BACKDROP;
    break;
  default:
    backdrop = RESOURCE_SHIP_DECK_BACKDROP;
    break;
  }
  gMainRoutineWork.battleBackground = backdrop;
}

/// Whether the character is one of the first PARTY_BATTLE_SIZE party members,
/// the ones in battle.
s32 Party_IsInFrontRow(s32 owner) {
  s32 count = Party_CountMembersFar();
  s32 i;

  if (count > PARTY_BATTLE_SIZE)
    count = PARTY_BATTLE_SIZE;
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.party[i] == owner)
      return 1;
  }
  return 0;
}

/// Chooses the battle music for slot of zone (gMainRoutineWork.battleMusic): a
/// key takes the song listed last before it in gEncounter_BattleMusic, and a
/// link battle (FLAG_LINK_BATTLE) keeps the music playing. With no song found,
/// it is MUS_BATTLE_ON_THE_SEA on the ship before it flies, else by the front
/// row: MUS_BATTLE_THE_LOST_AGE with Felix, MUS_BATTLE_THE_BROKEN_SEAL with
/// Isaac, MUS_BATTLE_JENNA with Jenna, or MUS_BATTLE_THE_LOST_AGE.
void Encounter_SelectBattleMusic(s32 zone, s32 slot) {
  s16 key = (zone << 4) + slot;
  const s16 *table = gEncounter_BattleMusic;
  u16 music;
  u16 value;

  if (GameFlag_Get(FLAG_LINK_BATTLE)) {
    music = SOUND_KEEP_BGM;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == (u16)key)
        break;
      if (value & ENCOUNTER_BATTLE_MUSIC_MARKER)
        music = value & 0xfff;
    }
  }
  if (music == 0) {
    if (gMainRoutineWork.ownerOverride != 0 && !GameFlag_Get(FLAG_SHIP_FLIES))
      music = MUS_BATTLE_ON_THE_SEA;
    else if (Party_IsInFrontRow(FELIX))
      music = MUS_BATTLE_THE_LOST_AGE;
    else if (Party_IsInFrontRow(ISAAC))
      music = MUS_BATTLE_THE_BROKEN_SEAL;
    else if (Party_IsInFrontRow(JENNA))
      music = MUS_BATTLE_JENNA;
    else
      music = MUS_BATTLE_THE_LOST_AGE;
  }
  gMainRoutineWork.battleMusic = music;
}

#undef WORLD_MAP_ENCOUNTER_REGIONS
#undef BATTLE_BACKGROUND_RULE
#undef BATTLE_BACKGROUND_RULE_RESOURCE
