#include "types.h"

/* 配置表から有効な札を数える。counts が非0なら種別ごとの内訳も返す。 */

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

s32 Func_080b6c08(s32, u16 *);
struct PlacementTable *Func_08077000(s32 owner);

s32 Func_080be0b4(u32 arg0, u8 *counts)
{
    u16 values[8];
    struct PlacementList *list;
    s32 total;
    s32 found;
    s32 i;
    s32 j;
    s32 kind;
    s32 owner;

    found = 0;
    kind = 1;
    if (arg0 > 7) {
        kind = 2;
    }
    total = Func_080b6c08(kind, values);

    owner = 0;
    if (arg0 > 7) {
        owner = 1;
    }
    list = &Func_08077000(owner)->list;

    if (counts != 0) {
        for (j = 3; j >= 0; j--) {
            counts[j] = 0;
        }
    }

    i = 0;
    if (list->count != 0) {
        do {
            if (list->entries[i].timer == -1) {
                for (j = 0; j < total; j++) {
                    if (values[j] == list->entries[i].id) {
                        break;
                    }
                }
                if (j != total) {
                    if (counts != 0) {
                        counts[list->entries[i].x]++;
                    }
                    found++;
                }
            }
            i++;
        } while (i != list->count);
    }
    return found;
}
