#include "TYPES.H"
#include "SYSTEM.H"
/* Battle: order the turn queue by priority, highest first. Commands of
   kind 5 whose action has effect 46, 47 or 53 are raised by 10000 first;
   the sort is a bubble sort of 16-byte entries through the IWRAM word
   copier. */
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "IWRAM_CALL.H"

extern u8 gBattleWork[];
s32 BattleParty_ListLivingUnits(s32 side, u16 *out_units);
void BattleCommand_SelectAutomatic(void *entry, s32 arg1);

struct BattlePresentationOpponentEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

struct BattleQueueEntry {
    s16 owner_id;
    u16 unknown_02;
    s16 priority;
    s16 command_kind;
    u16 encoded_action;
    u8 unknown_0a[6];
};

s32 Func_080771e8(s32 group, s32 index);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: a direct call changes BattleQueue_SortByPriority from mov r7, r1 to sub sp, sp, #20 (123/126 assembly lines). */
    Iwram_CopyWords(destination, source, size);
}

s32 BattlePres_BuildOpponentEntries(
    struct BattlePresentationOpponentEntry *entries)
{
    u16 unit_ids[14];
    s32 entry_count = 0;
    u8 *battle = *(u8 **)gBattleWork;
    s32 unit_count;
    s32 i;

    if (battle[0x45] == 1) {
        return 0;
    }

    unit_count = BattleParty_ListLivingUnits(2, unit_ids);
    if (unit_count == 0) {
        return 0;
    }

    for (i = 31; i >= 0; i--) {
        u32 first = (u32)(unit_count *Random16()) >> 16;
        u32 second = (u32)(unit_count *Random16()) >> 16;
        s32 swap = unit_ids[first];
        unit_ids[first] = unit_ids[second];
        unit_ids[second] = swap;
    }

    if (battle[0x45] == 2) {
        s32 limit = ((u32)(Random16() * 5) >> 16) + 1;

        if (limit <= 1) {
            limit = 2;
        }
        if (limit < unit_count) {
            unit_count = limit;
        }
    }

    for (i = 0; i < unit_count; i++) {
        s32 unit_id = unit_ids[i];
        u8 *unit = Owner_GetStateFar(unit_id);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit[0x43]; copy_index++) {
            struct BattlePresentationOpponentEntry *entry =
                &entries[entry_count];
            s32 value;

            entry->unit_id = unit_id;
            value = *(u16 *)(unit + 0x40);
            entry->value = value;
            if (copy_index != 0) {
                entry->value = (s16)value / 2;
            }

            if (unit[0x13c] != 0 || unit[0x13b] != 0) {
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x100;
            } else {
                BattleCommand_SelectAutomatic(entry, 0);
            }

            entry_count++;

            if (battle[0x45] == 2) {
                break;
            }
        }
    }

    return entry_count;
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

            Owner_GetStateFar(entry->owner_id);
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
