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
#include "SYSTEM.H"
#include "BTLSYS.H"
#include "BTLTYPE.H"
#include "BATTLE_COMMAND.H"
#include "BTLPTY.H"
#include "IWRAM_CALL.H"



static __inline__ void CopyWords(
    void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: needed to match: a direct call changes BattleQueue_SortByPriority from mov r7, r1 to sub sp, sp, #20 */
    Iwram_CopyWords(destination, source, size);
}

s32 BattlePres_BuildOpponentEntries(
    struct BattleActionRecord *entries)
{
    u16 unit_ids[14];
    s32 entry_count = 0;
    struct BtlWork *battle = BtlWk;
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
        u32 first = (unit_count *Random16()) >> 16;
        u32 second = (unit_count *Random16()) >> 16;
        s32 swap = unit_ids[first];
        unit_ids[first] = unit_ids[second];
        unit_ids[second] = swap;
    }

    if (battle->encounter_mode == 2) {
        s32 limit = ((Random16() * 5) >> 16) + 1;

        if (limit <= 1) {
            limit = 2;
        }
        if (limit < unit_count) {
            unit_count = limit;
        }
    }

    for (i = 0; i < unit_count; i++) {
        s32 unit_id = unit_ids[i];
        struct BtlUnit *unit = UnitGetFar(unit_id);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++) {
            struct BattleActionRecord *entry =
                &entries[entry_count];
            s32 value;

            entry->unit_id = unit_id;
            value = unit->agi;
            entry->priority = value;
            if (copy_index != 0) {
                entry->priority = (s16)value / 2;
            }

            if (unit->nemuri != 0 || unit->shibire != 0) {
                entry->command = 8;
                entry->parameter = 0;
                entry->target = 0x100;
            } else {
                BattleCommand_SelectAutomatic(entry, 0);
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

        if (entry->command == 5) {
            struct BtlAct *action;

            UnitGetFar(entry->unit_id);
            action = BtlActGet(Djinn_GetDefinitionHeaderFar(
                (entry->parameter) >> 8 & 15, entry->parameter & 0xff));
            if (action->efx == 46 || action->efx == 47 || action->efx == 53) {
                entry->priority += 10000;
            }
        }
    }

    do {
        swapped = 0;
        for (j = count - 1; j > 0; j--) {
            if ((s16)entries[j].priority > (s16)entries[j - 1].priority) {
                struct BattleActionRecord temporary;

                CopyWords(&temporary, &entries[j], sizeof(struct BattleActionRecord));
                CopyWords(&entries[j], &entries[j - 1], sizeof(struct BattleActionRecord));
                CopyWords(&entries[j - 1], &temporary, sizeof(struct BattleActionRecord));
                swapped++;
            }
        }
    } while (swapped != 0);
}
