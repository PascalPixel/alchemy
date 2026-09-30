#include "TYPES.H"
extern u8 gObjectSlots[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ObjectDispatch_SetSingleChildField26Far(void *, s32);

struct FacingEntry *Object_FindNearestFacingTarget(struct FacingEntry *self, s32 id)
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
    entry = *(struct FacingEntry **)((u32)&gObjectSlots);
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
        dist = Iwram_Sqrt(dx *dx + dz *dz);
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
    if (found == NULL)
        return NULL;
    if (*found->record->id != id)
        return NULL;
    return found;
}
