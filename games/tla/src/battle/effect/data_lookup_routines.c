#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"

#define BattleEffect_GetResourceGroup ((s32 (*)(s32))BattleEffect_GetResourceGroup)

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps);

s32 Encounter_RollTerrain(u32 index, s32 steps) {
  u8 *table = (u8 *)Ram_HeapSlots->eventWork + 0x18c;

  return Encounter_SelectEnemyGroup(table[index], steps);
}

/// Chooses a world-map encounter from the terrain zone and step weight.
s32 Encounter_RollWorldMap(s32 position, s32 steps) {
  return Encounter_SelectEnemyGroup(
      Encounter_ResolveWorldMapZoneAndBackdrop((void *)position), steps);
}

/// Returns the selected group from a fourteen-halfword encounter row.
u16 Encounter_GetEnemyGroupAtSlot(s32 zone, s32 slot) {
  u16 *table;
  s32 byteOffset;

  byteOffset = zone * 14;
  table = gEncounter_EnemyGroupTable;
  /* FAKEMATCH: ⚓️ loads the table between scaling the zone and adding the slot. */
  asm volatile("" : "+l"(table), "+l"(byteOffset));
  byteOffset += slot;
  byteOffset <<= 1;
  byteOffset += 4;
  return *(u16 *)((u8 *)table + byteOffset);
}

/// Resolves the Djinni table's zone and slot pair to its enemy group.
u16 Encounter_GetDjinnEnemyGroup(s32 djinni) {
  u16 *zoneAndSlot = (u16 *)(gEncounter_AreaEntryTable + djinni * 4);

#ifdef UBFIX
  return Encounter_GetEnemyGroupAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#else
  // UB: no return statement; the native leaves the called getter's result in
  // r0.
  Encounter_GetEnemyGroupAtSlot(zoneAndSlot[0], zoneAndSlot[1]);
#endif
}

s32 GameFlag_Test(s32 flag);

struct SceneCueRule {
  s16 id;
  s16 entrance : 15;
  u16 byScene : 1;
  s16 flag;
  s16 cue;
};

void Scene_ResolveInteractionResult(void) {
  s16 cue = 79;
  s16 scene = gSequenceParentWork.scene;
  s16 entrance = gSequenceParentWork.entrance;
  s16 group = gSequenceParentWork.sceneGroup;
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

  gSequenceParentWork.cue = cue;
}

struct SceneAreaNameRule {
  s16 id;
  s16 entrance : 15;
  u16 useSceneId : 1;
  s32 areaNameIndex;
};

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance) {
  const struct SceneAreaNameRule *entry = gScene_AreaNameRules;
  s32 areaNameIndex = 0;
  s32 group = BattleEffect_GetResourceGroup(scene);

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
