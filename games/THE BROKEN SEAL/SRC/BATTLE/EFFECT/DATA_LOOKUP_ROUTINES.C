#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_RUNTIME.H"
#include "GAME_STATE.H"
#include "EVENTWRK.H"

#define BattleFx_GetResourceGroup ((s32 (*)(s32))BattleFx_GetResourceGroup)

extern struct EventWork *gEventWork;

extern s32 Encounter_SelectEnemyGroup();

s32 EffectRuntime_LookupByTableEntry(u32 index)
{
    u8 *table = gEventWork->encounter_groups;

    return Encounter_SelectEnemyGroup(table[index]);
}

s32 Encounter_SelectEnemyGroup(s32, s32);

s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1)
{
    return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)arg0), arg1);
}

extern u16 Encounter_EnemyGroupTable[];

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1)
{
    return Encounter_EnemyGroupTable[(arg0 * 14) + arg1 + 2];
}

unsigned char BattleFx_GetPhaseResult(s32 phase_index)
{
  s32 entry_offset;
  int entry_address;
  int weighted_index_address;
  u16 weighted_row;
  u8 *entry;
  u8 *weighted_row_address;
  entry_offset = phase_index * 4;
  entry = ((u8 *)entry_offset) + (s32)Encounter_AreaEntryTable;
  weighted_index_address = entry_offset + (s32)Encounter_AreaEntryTable;
  weighted_row_address = entry;
  weighted_row = *((u16 *)weighted_row_address);
  entry_address = weighted_index_address;
  BattleFx_GetWeightedResult(
      weighted_row, *((u16 *)(entry_address + 2)));
}

struct SceneInteractionEntry {
    s16 id;
    s16 value : 15;
    u16 alt_source : 1;
    s16 condition;
    s16 result;
};

void Scene_ResolveInteractionResult(void)
{
    s16 result = 18;
    s16 progress = gGameState.scene;
    s16 sub = gGameState.entrance;
    s16 alt = gGameState.scene_group;
    const struct SceneInteractionEntry *entry = Scene_InteractionRuleTable;

    for (; entry->id != -1; entry++) {
        if (entry->alt_source) {
            if (entry->id != progress) {
                continue;
            }
            if (entry->value != -1 && entry->value != sub) {
                continue;
            }
            if (entry->condition != -1 && GameFlag_TestFar(entry->condition) == 0) {
                continue;
            }
            result = entry->result;
            break;
        }
        if (entry->id != alt) {
            continue;
        }
        if (entry->value != -1 && entry->value != sub) {
            continue;
        }
        if (entry->condition != -1 && GameFlag_TestFar(entry->condition) == 0) {
            continue;
        }
        result = entry->result;
        break;
    }

    gGameState.cue = result;
}

struct SceneAreaNameRule {
    s16 id;
    s16 entrance : 15;
    u16 use_scene_id : 1;
    s32 area_name_index;
};

s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance)
{
    const struct SceneAreaNameRule *entry = Scene_AreaNameRules;
    s32 area_name_index = 0;
    s32 group = BattleFx_GetResourceGroup(scene);

    while (entry->id != -1) {
        if (entry->use_scene_id) {
            if (entry->id == scene) {
                if (entry->entrance == -1 || entry->entrance == entrance) {
                    area_name_index = entry->area_name_index;
                    break;
                }
            }
        } else {
            if (entry->id == group) {
                if (entry->entrance == -1 || entry->entrance == entrance) {
                    area_name_index = entry->area_name_index;
                    break;
                }
            }
        }
        entry++;
    }
    return area_name_index;
}
