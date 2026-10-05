#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "PARTY_STATE.H"

/* ☀️'s encounter lookups, reading ⚓️'s event work through its heap slot. */

extern s32 Encounter_SelectEnemyGroup();
s32 BattleFx_LookupResult(void *);
extern u16 Encounter_EnemyGroupTable[];
extern u8 Encounter_AreaEntryTable[];

s32 EffectRuntime_LookupByTableEntry(u32 index)
{
    u8 *table = (u8 *)Ram_HeapSlots->event_work + 0x18c;

    /* 第2引数は呼出元のr1を引き継ぐ特殊な呼出規約。 */
    return Encounter_SelectEnemyGroup(table[index]);
}

s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1)
{
    return Encounter_SelectEnemyGroup(BattleFx_LookupResult((void *)arg0), arg1);
}

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1)
{
    u16 *table;
    s32 row;

    row = arg0 * 14;
    table = Encounter_EnemyGroupTable;
    /* FAKEMATCH: ⚓️ loads the table between the row and the column */
    asm volatile("" : "+l"(table), "+l"(row));
    row += arg1;
    row <<= 1;
    row += 4;
    return *(u16 *)((u8 *)table + row);
}

u16 BattleFx_GetPhaseResult(s32 phase)
{
    u16 *entry = (u16 *)(Encounter_AreaEntryTable + phase * 4);

    /* Exact GCC 2.96 output carries this call's result through r0. */
    BattleFx_GetWeightedResult(entry[0], entry[1]);
}

s32 GameFlag_Test(s32 flag);

/* A scene's cue: the scene or its area, an entrance (-1 for any) and a
   flag (-1 for none) that must be set. */
struct SceneCueRule {
    s16 id;
    s16 entrance : 15;
    u16 by_scene : 1;
    s16 flag;
    s16 cue;
};

extern const struct SceneCueRule Scene_CueRuleTable[];

/* Picks the cue of the first rule that matches the scene, or its area, and
   the entrance; 79 when none does. */
void Scene_ResolveInteractionResult(void)
{
    s16 cue = 79;
    s16 scene = gPartyState.scene;
    s16 entrance = gPartyState.entrance;
    s16 group = gPartyState.scene_group;
    const struct SceneCueRule *rule = Scene_CueRuleTable;

    for (; rule->id != -1; rule++) {
        if (rule->by_scene) {
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
    u16 use_scene_id : 1;
    s32 area_name_index;
};

extern const struct SceneAreaNameRule Scene_AreaNameRules[];

s32 BattleFx_GetResourceGroup(s32 effect_id);

/* Selects an area-name index by scene or scene group and entrance. A rule
   with entrance -1 matches every entrance. The group routine returns s8 in
   its own file, but this caller takes the result unextended, as from an
   s32 prototype. */
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
