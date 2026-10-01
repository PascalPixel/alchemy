/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords plain-source attempt.
 * Removing this one source device changes BattleQueue_SortByPriority.
 * Remaining difference: a direct call changes BattleQueue_SortByPriority from mov r7, r1 to sub sp, sp, #20 (123/126 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
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


s32 BattlePres_BuildOpponentEntries(
    struct BattlePresentationOpponentEntry *entries)
;

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

                Iwram_CopyWords(&temporary, &entries[j], 16);
                Iwram_CopyWords(&entries[j], &entries[j - 1], 16);
                Iwram_CopyWords(&entries[j - 1], &temporary, 16);
                swapped++;
            }
        }
    } while (swapped != 0);
}
