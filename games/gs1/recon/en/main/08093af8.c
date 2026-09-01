#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

struct FacingRecord {
    u8 unknown_00[0x28];
    s16 *id;
};

struct FacingEntry {
    void *data;
    u8 unknown_04[2];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x3c];
    struct FacingRecord *record;
    u8 kind;
    u8 unknown_55[0x1b];
};

s32 ArcTan2(s32, s32);

struct FacingEntry *Func_08093af8(struct FacingEntry *self, s32 id)
{
    struct FacingEntry *entry;
    struct FacingEntry *found;
    struct FacingEntry *result;
    s32 cnt;
    s32 best;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 turn;

    found = NULL;
    best = 40;
    entry = *(struct FacingEntry **)ADDR_03001E64;
    for (cnt = 0; cnt < 64; cnt++, entry++) {
        if (entry->data == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->kind != 1)
            continue;
        dy = entry->y - self->y;
        if (dy >= 0) {
            if (dy > 0x2fffff)
                continue;
        } else {
            if (self->y - entry->y > 0x2fffff)
                continue;
        }
        dx = (entry->x - self->x) / 0x10000;
        dz = (entry->z - self->z) / 0x10000;
        dist = ((s32 (*)(s32))0x030001d8)(dx * dx + dz * dz);
        if (dist >= best)
            continue;
        angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
        if (dist > 23) {
            turn = (s16)(angle - self->facing);
            if (turn < -0x2fff)
                continue;
            if (turn > 0x2fff)
                continue;
        }
        found = entry;
        best = dist;
    }
    result = NULL;
    if (found != NULL && *found->record->id == id)
        result = found;
    return result;
}
