#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"

/* Battle timers: per-unit countdown bytes in the owner record, and the
   placement entries whose timers expire into Djinn activation. */

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 owner);
void Owner_RecalculateStatsFar(s32 unit_id);
s32 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
s32 Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);

s32 BattleUnit_TickCounter13f(s32 id)
{
    u8 *value = (u8 *)&Owner_GetStateFar(id)->reflect;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

s32 BattleUnit_TickCounter146(s32 id)
{
    struct BattleUnit *unit = Owner_GetStateFar(id);
    u8 *value = &unit->agility_modifier_turns;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            unit->agility_modifier = 0;
            return 1;
        }
    }
    return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void)
{
    struct DjinnRecoveryList *list;
    struct DjinnRecoveryEntry *timed_entry;
    struct DjinnRecoveryEntry *expired_entry;
    s32 index;
    s32 removed;
    s32 initial_count;

    list = &Trade_GetOfferStateFar(0)->list;
    initial_count = list->count;
    index = 0;
    removed = 0;
    if (index < initial_count) {
        timed_entry = list->entries;
        do {
            if (timed_entry->turns > 0 &&
                Owner_GetStateFar(timed_entry->unit_id)->hp != 0) {
                timed_entry->turns--;
            }
            index++;
            timed_entry++;
        } while (index < list->count);
    }
    index = 0;
    if (index < list->count) {
        expired_entry = list->entries;
        do {
            if (expired_entry->turns == 0) {
                u8 id = expired_entry->unit_id;

                Djinn_ActivateFar(id, expired_entry->element, expired_entry->index);
                Trade_RemoveOfferFar(id, expired_entry->element, expired_entry->index);
                Owner_RecalculateStatsFar(id);
                removed = 1;
            } else {
                expired_entry++;
                index++;
            }
        } while (index < list->count);
    }
    return removed;
}

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
    s32 turns;

    turns = 19;
    do {
        turns--;
        BattlePlacement_UpdateTimedEntries();
    } while (turns >= 0);
    return 0;
}
