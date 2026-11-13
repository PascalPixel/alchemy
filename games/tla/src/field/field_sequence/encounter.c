/// @file
/// Random encounters, and each scene's music, area name and battle background.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "encounter_zone.h"
#include "gamepak/far_call/game.h"
#include "module/field/field_sequence.h"
#include "gamepak/far_call/field.h"
#include "module/tla.h"
#include "resource_ids.h"

struct EventWorkView {
  u8 unk000[0x15a];
  s16 unk15a;
  s16 unk15c;
  s16 unk15e;
  u8 unk160[4];
  s16 unk164;
  s16 unk166;
  u8 unk168[0x1c];
  s16 blockedSteps;
  u16 pushKind;
  u8 unk188[4];
  u8 settings[8];
  s32 unk194;
  s32 unk198;
};

void Func_080c9de0(s32 id, s32 variant) {
  const struct FlagEntry *entry = FlagEntries_GetTable();

  for (; entry->flag != 0; entry++) {
    if (entry->id != id)
      continue;
    if (entry->variant != -1 && variant != entry->variant)
      continue;
    GameFlag_SetBit(entry->flag + 0x180);
    break;
  }
}

s32 Func_080c9e1c(s32 flag) {
  const struct FlagEntry *entry = FlagEntries_GetTable();

  while (entry->flag != 0) {
    if (entry->flag == flag && entry->value != 0)
      return entry->value;
    entry++;
  }
  return 1;
}

void Encounter_SelectMapZones(s32 search) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  u8 *out = work->settings;
  u32 first = 0;
  u32 second = 0;
  const struct SceneMusicEntry *entry = gScene_EncounterRules;
  u32 i;

  if (search != 0) {
    for (; entry->scene != -1; entry++) {
      if (entry->scene != scene)
        continue;
      if (entry->entrance != -1 && entry->entrance != entrance)
        continue;
      if (entry->flag != -1 && GameFlag_Test(entry->flag))
        continue;
      first = entry->first;
      second = entry->second;
      break;
    }
  }
  *out++ = first;
  *out++ = second;
  for (i = 0; i < 6; i++)
    *out++ = 0;
  work->unk194 = 0;
  work->unk198 = 0x100000;
  BattleBackground_SelectForMap();
}

/// Returns the world map encounter zone for the terrain at position (0 for
/// none) and chooses the battle backdrop for that terrain. There are no
/// encounters in render mode 8, render mode 7 reads the second rule table,
/// and a rule's flag must be set for it to apply.
// As ☀️'s function of the same name.
s32 Encounter_ResolveWorldMapZoneAndBackdrop(void *position) {
  struct TerrainRule *rule = gData_080eedbc;
  s32 behavior;
  u32 kind;
  s32 zone;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 8)
    return 0;
  if (party->renderMode == 7)
    rule = gData_080eef34;
  behavior = WorldMap_GetBattleEnvironmentFar(position, (s32 *)&kind);
  kind %= 20;
  zone = 0;
  for (; rule->kind != -1; rule++) {
    if (rule->kind != kind)
      continue;
    if (rule->behavior != -1 && rule->behavior != behavior)
      continue;
    if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
      continue;
    zone = rule->zone;
    break;
  }
  BattleBackground_SelectForEnvironment(behavior);
  return zone;
}

#define ENCOUNTER_STEPS (gMainRoutineWork.encounterSteps)

s32 Encounter_CountStepsAndSelectFormation(s32 zone, s32 steps) {
  struct EncounterWork *work;
  const struct EncounterZone *entry;
  s32 rate;
  s32 level;
  s32 bias;
  s32 step;
  s32 total;
  s32 sum;
  s32 pick;
  s32 i;
  /* FAKEMATCH(barrier): an empty asm reading pick before it is set, which
   * makes it live from the entry and keeps a register for it across the whole
   * function; without it, 42 of 180 instructions differ in tla-en. */
  asm("" : : "l"(pick));

  work = Ram_HeapSlots->eventWork;
  if (GameFlag_Test(351) == 0) {
    if (GameFlag_Test(352) != 0 || GameFlag_Test(353) != 0)
      return 0;
    if (zone == 0)
      return 0;
    if (gMainRoutineWork.cloaked != 0)
      return 0;
    entry = gEncounter_EnemyGroupTable + zone;
    rate = entry->rate;
    if (rate == 0)
      return 0;

    level = Party_GetAverageLevelFar() - entry->level;
    if (level < 0)
      level = 0;
    if (level > 5)
      level = 5;
    if (level > 0 && gMainRoutineWork.avoidDistance != 0)
      return 0;
    rate += level * 5;

    bias = work->bias;
    if (bias == 0) {
      s32 a = Random16();
      s32 b = Random16();
      s32 c = Random16();

      bias = (a - b + c - (s32)Random16()) / 2;
      work->bias = bias;
    }
    step =
        Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = ENCOUNTER_STEPS += Iwram_MulQ16(step, steps);
    if (total < work->threshold)
      return 0;
  }

  work->bias = 0;
  sum = 0;
  for (i = 0; i < 8; i++)
    sum += entry->weights[i];
  if (sum == 0)
    return 0;
  pick = (Random16() * sum) >> 16;
  for (i = 0; i < 8; i++) {
    pick -= entry->weights[i];
    if (pick < 0)
      break;
  }
  sum = (u16)entry->enemies[i];
  BattleEffect_SelectBattleCue(zone, i);
  return sum;
}

#define Scene_GetArea ((s32 (*)(s32))Scene_GetArea)

s32 Encounter_RollTerrain(u32 index, s32 steps) {
  u8 *table = (u8 *)Ram_HeapSlots->eventWork + 0x18c;

  return Encounter_CountStepsAndSelectFormation(table[index], steps);
}

/// Chooses a world-map encounter from the terrain zone and step weight.
s32 Encounter_RollWorldMap(s32 position, s32 steps) {
  return Encounter_CountStepsAndSelectFormation(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)position), steps);
}

/// Returns the selected group from a fourteen-halfword encounter row.
u16 Encounter_GetFormationAtSlot(s32 zone, s32 slot) {
  u16 *table;
  s32 byteOffset;

  byteOffset = zone * 14;
  table = (u16 *)gEncounter_EnemyGroupTable;
  byteOffset += slot;
  byteOffset <<= 1;
  byteOffset += 4;
  return *(u16 *)((u8 *)table + byteOffset);
}

/// Resolves the Djinni table's zone and slot pair to its enemy group.
u16 Encounter_GetDjinnFormation(s32 djinni) {
  u16 *zoneAndSlot = (u16 *)(gEncounter_AreaEntryTable + djinni * 4);

#ifdef UBFIX
  return Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the native leaves the called getter's result in
  // r0.
  Encounter_GetFormationAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

void Scene_ChooseMusic(void) {
  s16 cue = 79;
  s16 scene = gMainRoutineWork.scene;
  s16 entrance = gMainRoutineWork.entrance;
  s16 group = gMainRoutineWork.sceneGroup;
  const struct SceneCueRule *rule = gScene_CueRuleTable;

  for (; rule->id != -1; rule++) {
    if (rule->byScene) {
      if (rule->id != scene)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
        continue;
      cue = rule->cue;
      break;
    } else {
      if (rule->id != group)
        continue;
      if (rule->entrance != -1 && rule->entrance != entrance)
        continue;
      if (rule->flag != -1 && GameFlag_Test(rule->flag) == 0)
        continue;
      cue = rule->cue;
      break;
    }
  }

  gMainRoutineWork.cue = cue;
}

s32 Area_GetNameIndex(s32 scene, s32 entrance) {
  const struct SceneAreaNameRule *entry = gScene_AreaNameRules;
  s32 areaNameIndex = 0;
  s32 group = Scene_GetArea(scene);

  while (entry->id != -1) {
    if (entry->useSceneId) {
      if (entry->id == scene) {
        if (entry->entrance == -1 || entry->entrance == entrance) {
          areaNameIndex = entry->areaNameIndex;
          break;
        }
      }
    } else {
      if (entry->id == group) {
        if (entry->entrance == -1 || entry->entrance == entrance) {
          areaNameIndex = entry->areaNameIndex;
          break;
        }
      }
    }
    entry++;
  }
  return areaNameIndex;
}

/// Sets the scene the party returns to after a defeat
/// (gMainRoutineWork.savedScene and savedEntrance), from the first rule of
/// gParty_PairResolveRules that matches the current scene and entrance: rows
/// of eight halfwords that open {scene, entrance (-1 for any), return scene,
/// return entrance}, ended by scene -1. Without a match the restart point stays
/// as it was.
void Scene_SetRestartPoint(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 first;
  s16 second;
  const s16 *entry;

  first = party->scene;
  second = party->entrance;
  entry = gParty_PairResolveRules;

  while (entry[0] != -1) {
    if (entry[0] == first && (entry[1] == -1 || entry[1] == second)) {
      party->savedScene = entry[2];
      party->savedEntrance = entry[3];
      return;
    }
    entry += 8;
  }
}

/// With flag 0x8de set, finds the first rule of gParty_PairResolveRules for
/// scene and entrance (-1 on either side matches any entrance) and copies its
/// position, x and z in 16.16 and the third value as is, to Data_020004b8.
void Func_080ca368(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (GameFlag_Test(0x8de) == 0)
    return;
  while (rule[0] != -1) {
    if (rule[0] == scene &&
        (rule[1] == -1 || entrance == -1 || rule[1] == entrance)) {
      *(s32 *)(Data_020004b8 + 0) = rule[4] << 16;
      *(s32 *)(Data_020004b8 + 4) = rule[5] << 16;
      *(s32 *)(Data_020004b8 + 8) = rule[6];
      return;
    }
    rule += 8;
  }
}

/// Func_080ca368's twin for the rule's return scene and entrance (its third
/// and fourth values): with flag 0x8de set, clears ownerOverride and copies
/// the rule's position to the party's saved position.
void Func_080ca3f4(s32 scene, s32 entrance) {
  const s16 *rule = gParty_PairResolveRules;

  if (GameFlag_Test(0x8de) == 0)
    return;
  gMainRoutineWork.ownerOverride = 0;
  while (rule[0] != -1) {
    if (rule[2] == scene &&
        (rule[3] == -1 || entrance == -1 || rule[3] == entrance)) {
      gMainRoutineWork.savedX = rule[4] << 16;
      gMainRoutineWork.savedZ = rule[5] << 16;
      gMainRoutineWork.unk280 = rule[6];
      return;
    }
    rule += 8;
  }
}

s16 BattleBackground_GetSelected(void) {
  return gMainRoutineWork.battleBackground;
}

/// As The Broken Seal's, with this game's dragon hall check after the loop:
/// looks the scene up in gScene_BattleBackgroundRules and keeps the last marked
/// backdrop before it; with flag 0xa30 set, the second dragon hall backdrop
/// becomes the first.
void BattleBackground_SelectForMap(void) {
  s32 target = gMainRoutineWork.scene;
  s32 *table = gScene_BattleBackgroundRules;
  s32 entry = *table++;
  s32 result; // not initialised: with no marked row before the scene it keeps
              // whatever it held

  if (entry != 0 && entry != target) {
    do {
      if (entry & 0x80000000) {
        result = entry & 0xFFFF;
      }
      entry = *table++;
    } while (entry != 0 && entry != target);
  }
  if (result == (s32)&ResourceId_DragonHallBBackdrop && GameFlag_Test(0xa30))
    result = (s32)&ResourceId_DragonHallABackdrop;
  gMainRoutineWork.battleBackground = result;
}

/// Chooses the battle backdrop for a terrain; the ship deck's is the default.
void BattleBackground_SelectForEnvironment(s32 behavior) {
  u16 value;

  switch (behavior - 1) {
  case 0:
    value = (u32)&ResourceId_GrasslandBBackdrop;
    break;
  case 1:
    value = (u32)&ResourceId_DenseForestBackdrop;
    break;
  case 2:
    value = (u32)&ResourceId_DesertBackdrop;
    break;
  case 3:
  case 6:
    value = (u32)&ResourceId_RockyBeachBackdrop;
    break;
  case 4:
  case 5:
    value = (u32)&ResourceId_SnowfieldBBackdrop;
    break;
  case 10:
  case 11:
    value = (u32)&ResourceId_BlizzardBackdrop;
    break;
  default:
    value = (u32)&ResourceId_ShipDeckBackdrop;
    break;
  }
  gMainRoutineWork.battleBackground = value;
}

s32 Func_080ca5a0(s32 owner) {
  s32 count = Party_CountMembersFar();
  s32 i;

  if (count > 4)
    count = 4;
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.activeOwners[i] == owner)
      return 1;
  }
  return 0;
}

/// Chooses the battle music for slot of zone (gMainRoutineWork.sceneCue): a key
/// takes the song listed last before it in Data_080efd1c, and flag 0x16c's
/// battles always use music 79. With no song found, it is 56 while
/// ownerOverride is set and flag 0x8df clear, else 50 when owner 4 is active,
/// 750 when owner 0 is, 49 when owner 5 is, or 50.
// As ☀️'s function of the same name.
void BattleEffect_SelectBattleCue(s32 zone, s32 slot) {
  s16 key = (zone << 4) + slot;
  const s16 *table = Data_080efd1c;
  u16 music;
  u16 value;

  if (GameFlag_Test(0x16c)) {
    music = 79;
  } else {
    for (;;) {
      value = *table++;
      if (value == 0)
        break;
      if (value == (u16)key)
        break;
      if (value & 0x8000)
        music = value & 0xfff;
    }
  }
  if (music == 0) {
    if (gMainRoutineWork.ownerOverride != 0 && !GameFlag_Test(0x8df))
      music = 56;
    else if (Func_080ca5a0(4))
      music = 50;
    else if (Func_080ca5a0(0))
      music = 750;
    else if (Func_080ca5a0(5))
      music = 49;
    else
      music = 50;
  }
  gMainRoutineWork.sceneCue = music;
}
