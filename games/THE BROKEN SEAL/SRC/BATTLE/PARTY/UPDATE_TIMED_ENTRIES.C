#include "TYPES.H"

struct PlacementEntry {
    u8 x;
    u8 y;
    u8 id;
    s8 timer;
};

struct PlacementList {
    struct PlacementEntry entries[64];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};

struct BattleObject {
    u8 padding[0x38];
    s16 active;
};

struct PlacementTable *Trade_GetOfferStateFar(s32 owner);
struct BattleObject *Runtime_GetObject(u8 id);
void BattleUnit_Recalculate(u8 id);
void Djinn_ActivateFar(u8 id, u8 x, u8 y);
void Trade_RemoveOfferFar(u8 id, u8 x, u8 y);

s32 BattlePlacement_UpdateTimedEntries(void)
{
    struct PlacementList *list;
    struct PlacementEntry *timed_entry;
    struct PlacementEntry *expired_entry;
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
            if (timed_entry->timer > 0 &&
                Runtime_GetObject(timed_entry->id)->active != 0) {
                timed_entry->timer--;
            }
            index++;
            timed_entry++;
        } while (index < list->count);
    }
    index = 0;
    if (index < list->count) {
        expired_entry = list->entries;
        do {
            if (expired_entry->timer == 0) {
                u8 id = expired_entry->id;

                Djinn_ActivateFar(id, expired_entry->x, expired_entry->y);
                Trade_RemoveOfferFar(id, expired_entry->x, expired_entry->y);
                BattleUnit_Recalculate(id);
                removed = 1;
            } else {
                expired_entry++;
                index++;
            }
        } while (index < list->count);
    }
    return removed;
}
