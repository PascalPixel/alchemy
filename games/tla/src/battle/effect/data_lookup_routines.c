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
#include "party_state.h"

#define BattleFx_GetResourceGroup ((s32 (*)(s32))BattleFx_GetResourceGroup)

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps);

s32 Encounter_RollTerrain(u32 index, s32 steps) {
  u8 *table = (u8 *)Ram_HeapSlots->eventWork + 0x18c;

  return Encounter_SelectEnemyGroup(table[index], steps);
}

s32 Encounter_RollWorldMap(s32 arg0, s32 arg1) {
  return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)arg0), arg1);
}

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1) {
  u16 *table;
  s32 row;

  row = arg0 * 14;
  table = gEncounter_EnemyGroupTable;
  /* FAKEMATCH: ⚓️ loads the table between the row and the column */
  asm volatile("" : "+l"(table), "+l"(row));
  row += arg1;
  row <<= 1;
  row += 4;
  return *(u16 *)((u8 *)table + row);
}

u16 BattleFx_GetPhaseResult(s32 phase) {
  u16 *entry = (u16 *)(gEncounter_AreaEntryTable + phase * 4);

  BattleFx_GetWeightedResult(entry[0], entry[1]);
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
  s16 scene = gPartyState.scene;
  s16 entrance = gPartyState.entrance;
  s16 group = gPartyState.sceneGroup;
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

  gPartyState.cue = cue;
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
  s32 group = BattleFx_GetResourceGroup(scene);

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
