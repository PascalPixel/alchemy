#include "TYPES.H"

extern u8 *gEventWork;

extern s32 Encounter_SelectEnemyGroup();

void Scene_ResolveInteractionResult(void)
{
    s16 result = 18;
    s16 progress = gGameState[224];
    s16 sub = gGameState[225];
    s16 alt = gGameState[230];
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
        } else {
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
    }

    gGameState[248] = result;
}
