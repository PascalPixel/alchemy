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

struct BattleObjectSlot;

s32 Func_080b6a60(u16 *owners);
s32 Func_08077210(s32 id, s32 x, s32 y);
struct PlacementTable *Func_08077000(s32 owner);
void Func_080771c8(s32 id, s32 x, s32 y);
s32 Func_080770c0(s32 message);
struct BattleObjectSlot *Func_080b7dd0(s32 object_id);
void Func_080771b0(u8 id, u8 x, u8 y);
void Func_080771c0(u8 id, u8 x, u8 y);

void Func_080b5c08(void)
{
    u16 owners[10];
    s32 count;
    s32 i;
    s32 owner;
    s32 x;
    s32 y;

    count = Func_080b6a60(owners);

    for (i = 0; i < count; i++) {
        owner = owners[i];
        for (x = 0; x <= 3; x++) {
            for (y = 0; y <= 19; y++) {
                if (Func_08077210(owner, x, y) != 0) {
                    struct PlacementList *list = &Func_08077000((u32)owner > 7 ? 1 : 0)->list;
                    s32 j;

                    for (j = 0; j < list->count; j++) {
                        if (x == list->entries[j].x && y == list->entries[j].y)
                            break;
                    }
                    if (j == list->count)
                        Func_080771c8(owner, x, y);
                }
            }
        }
    }

    if (Func_080770c0(364) != 0)
        return;

    {
        struct PlacementList *list = &Func_08077000(0)->list;
        struct PlacementEntry *entry;

        i = 0;
        if (i < list->count) {
            entry = list->entries;
            do {
                if (entry->timer == -1 && Func_080b7dd0(entry->id) == 0) {
                    u8 id = entry->id;
                    u8 ex = entry->x;
                    u8 ey = entry->y;

                    Func_080771b0(id, ex, ey);
                    Func_080771c0(id, ex, ey);
                }
                i++;
                entry++;
            } while (i < list->count);
        }
    }
}
