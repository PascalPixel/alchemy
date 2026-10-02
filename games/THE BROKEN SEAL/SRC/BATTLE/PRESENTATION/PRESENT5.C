#include "TYPES.H"
#include "SYSTEM.H"
/* Battle: order the turn queue by priority, highest first. Commands of
   kind 5 whose action has effect 46, 47 or 53 are raised by 10000 first;
   the sort is a bubble sort of 16-byte entries through the IWRAM word
   copier. */
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_PARTY.H"
#include "IWRAM_CALL.H"

s32 BattleParty_ListLivingUnits(s32 side, u16 *out_units);
void BattleCommand_SelectAutomatic(struct BattleCommandRequest *entry, s32 mode);

s32 Func_080771e8(s32 group, s32 index);

static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: a direct call changes BattleQueue_SortByPriority from mov r7, r1 to sub sp, sp, #20 (123/126 assembly lines). */
    Iwram_CopyWords(destination, source, size);
}

s32 BattlePres_BuildOpponentEntries(
    struct BattleActionRecord *entries)
{
    u16 unit_ids[14];
    s32 entry_count = 0;
    struct BattleSession *battle = gBattleWork;
    s32 unit_count;
    s32 i;

    if (battle->encounter_mode == 1) {
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

    if (battle->encounter_mode == 2) {
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
        struct BattleUnit *unit = Owner_GetStateFar(unit_id);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++) {
            struct BattleActionRecord *entry =
                &entries[entry_count];
            s32 value;

            entry->unit_id = unit_id;
            value = unit->agility;
            entry->value = value;
            if (copy_index != 0) {
                entry->value = (s16)value / 2;
            }

            if (unit->sleep != 0 || unit->stun != 0) {
                entry->kind = 8;
                entry->parameter = 0;
                entry->target = 0x100;
            } else {
                BattleCommand_SelectAutomatic((struct BattleCommandRequest *)entry, 0);
            }

            entry_count++;

            if (battle->encounter_mode == 2) {
                break;
            }
        }
    }

    return entry_count;
}

void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count)
{
    s32 i;
    s32 j;
    s32 swapped;

    for (i = 0; i < count; i++) {
        struct BattleActionRecord *entry = &entries[i];

        if (entry->kind == 5) {
            struct BattleAction *action;

            Owner_GetStateFar(entry->unit_id);
            action = BattleAction_Get(Func_080771e8(
                (s16)((u16)entry->parameter) >> 8 & 15, ((u16)entry->parameter) & 0xff));
            if (action->effect == 46 || action->effect == 47 || action->effect == 53) {
                entry->value += 10000;
            }
        }
    }

    do {
        swapped = 0;
        for (j = count - 1; j > 0; j--) {
            if ((s16)entries[j].value > (s16)entries[j - 1].value) {
                struct BattleActionRecord temporary;

                CopyWords(&temporary, &entries[j], 16);
                CopyWords(&entries[j], &entries[j - 1], 16);
                CopyWords(&entries[j - 1], &temporary, 16);
                swapped++;
            }
        }
    } while (swapped != 0);
}
