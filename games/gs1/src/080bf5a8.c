#include "types.h"

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

struct Object_080bf5a8 {
    u8 padding[0x38];
    s16 active;
};

struct PlacementTable *Func_08077000(s32 owner);
struct Object_080bf5a8 *Func_08077008(u8 id);
void Func_08077010(u8 id);
void Func_080771b0(u8 id, u8 x, u8 y);
void Func_080771c0(u8 id, u8 x, u8 y);

s32 Func_080bf5a8(void)
{
    struct PlacementList *list;
    struct PlacementEntry *timed_entry;
    struct PlacementEntry *expired_entry;
    s32 index;
    s32 removed;
    s32 initial_count;

    list = &Func_08077000(0)->list;
    initial_count = list->count;
    index = 0;
    removed = 0;
    if (index < initial_count) {
        timed_entry = list->entries;
        do {
            if (timed_entry->timer > 0 &&
                Func_08077008(timed_entry->id)->active != 0) {
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

                Func_080771b0(id, expired_entry->x, expired_entry->y);
                Func_080771c0(id, expired_entry->x, expired_entry->y);
                Func_08077010(id);
                removed = 1;
            } else {
                expired_entry++;
                index++;
            }
        } while (index < list->count);
    }
    return removed;
}
