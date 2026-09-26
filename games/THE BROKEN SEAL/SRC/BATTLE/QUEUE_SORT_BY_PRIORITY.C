/* Battle: order the turn queue by priority, highest first. Commands of
   kind 5 whose action has effect 46, 47 or 53 are raised by 10000 first;
   the sort is a bubble sort of 16-byte entries through the IWRAM word
   copier. */
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "TYPES.H"

struct BattleQueueEntry {
    s16 owner_id;
    u16 unknown_02;
    s16 priority;
    s16 command_kind;
    u16 encoded_action;
    u8 unknown_0a[6];
};

s32 Func_080771e8(s32 group, s32 index);

typedef s32 (*WordCopy)(void *destination, const void *source, s32 size);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    ((WordCopy)0x03001388)(destination, source, size);
}

void BattleQueue_SortByPriority(struct BattleQueueEntry *entries, s32 count)
{
    s32 i;
    s32 j;
    s32 swapped;

    for (i = 0; i < count; i++) {
        struct BattleQueueEntry *entry = &entries[i];

        if (entry->command_kind == 5) {
            struct BattleAction *action;

            Func_08077008(entry->owner_id);
            action = BattleAction_Get(Func_080771e8(
                (s16)entry->encoded_action >> 8 & 15, entry->encoded_action & 0xff));
            if (action->effect == 46 || action->effect == 47 || action->effect == 53) {
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
