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

typedef void (*WordCopy)(void *destination, const void *source, s32 size);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    ((WordCopy)0x03001388)(destination, source, size);
}

extern u8 Value_0000000f;

void Func_080b9470(struct BattleQueueEntry *entries, s32 count)
{
    s32 i;
    s32 j;
    s32 swapped;

    for (i = 0; i < count; i++) {
        struct BattleQueueEntry *entry = &entries[i];

        if (entry->command_kind == 5) {
            struct BattleAction *action;
            s8 group;
            u8 effect;

            Func_08077008(entry->owner_id);
            group = (s8)(entry->encoded_action >> 8);
            action = BattleAction_Get(Func_080771e8(
                group & (s32)&Value_0000000f, entry->encoded_action & 0xff));
            effect = action->effect;
            if (effect == 46 || effect == 47 || effect == 53) {
                entry->priority += 10000;
            }
        }
    }

    do {
        swapped = 0;
        for (j = count - 1; j > 0; j--) {
            if (entries[j].priority > entries[j - 1].priority) {
                struct BattleQueueEntry temporary;

                CopyWords(&temporary, &entries[j], 16);
                CopyWords(&entries[j], &entries[j - 1], 16);
                CopyWords(&entries[j - 1], &temporary, 16);
                swapped++;
            }
        }
    } while (swapped != 0);
}
