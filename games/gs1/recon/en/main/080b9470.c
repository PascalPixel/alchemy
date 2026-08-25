#include "battle_runtime.h"
#include "battle_types.h"
#include "types.h"

struct BattleQueueEntry {
    s16 owner_id;
    u16 unknown_02;
    s16 priority;
    s16 command_kind;
    u16 encoded_action;
    u8 unknown_0a[6];
};

s32 Func_080771e8(s32 group, s32 index);
void Func_08007304(void *destination, const void *source, s32 size);

void Func_080b9470(struct BattleQueueEntry *entries, s32 count)
{
    s32 index;
    s32 swapped;

    for (index = 0; index < count; index++) {
        struct BattleQueueEntry *entry = &entries[index];

        if (entry->command_kind == 5) {
            struct BattleAction *action;
            u16 encoded = entry->encoded_action;
            u8 effect;

            Func_08077008(entry->owner_id);
            action = BattleAction_Get(
                Func_080771e8(((s8)(encoded >> 8)) & 0x0f, encoded & 0xff));
            effect = action->effect;
            if (effect == 46 || effect == 47 || effect == 53) {
                entry->priority += 10000;
            }
        }
    }

    do {
        swapped = 0;
        for (index = count - 1; index > 0; index--) {
            if (entries[index].priority > entries[index - 1].priority) {
                struct BattleQueueEntry temporary;

                Func_08007304(&temporary, &entries[index], sizeof(temporary));
                Func_08007304(&entries[index], &entries[index - 1], sizeof(temporary));
                Func_08007304(&entries[index - 1], &temporary, sizeof(temporary));
                swapped++;
            }
        }
    } while (swapped != 0);
}
