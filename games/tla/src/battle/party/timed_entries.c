#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "btlpty.h"
#include "b5_context.h"

struct PlacementList {
    struct DjinnRecoveryEntry entries[72];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};


s32 BattleUnit_TickReflect(s32 id)
{
    u8 *value = (u8 *)&((struct BtlUnit *)UnitGet(id))->reflect;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id)
{
    struct BtlUnit *unit = UnitGet(id);
    u8 *value = &unit->agi_cnt;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            unit->agi_lv = 0;
            return 1;
        }
    }
    return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void)
{
    struct PlacementList *list;
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
                ((struct BtlUnit *)UnitGet(timed_entry->unit_id))->hp != 0) {
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
                UnitCalcFar(id);
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

void Func_08124cc0(void)
{
}
