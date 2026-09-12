#include "types.h"
#include "gs1_edition.h"

#define Summon_ResetCharge Func_080c1f50

u8 *Runtime_GetObject(s32);

s32 Summon_ResetCharge(s32 class_id)
{
    u8 *summon;
    s32 object_index;
    s32 slot;
    s32 next_slot;
    u8 occupied;
    u8 marker;

    object_index = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = Runtime_GetObject(object_index + 0x80);
    occupied = summon[298];
    if (occupied != 1) goto next_object;
    if (summon[296] != class_id) goto next_object;
    slot = 0;
    if (summon[0] != 0) goto scan_slots;
    summon[0] = marker;
    summon[occupied] = slot;
    return;
scan_slots:
    slot++;
    if (slot > 13) return;
    occupied = summon[slot];
    if (occupied != 0) goto scan_slots;
    next_slot = slot + 1;
    summon[slot] = marker;
    summon[next_slot] = occupied;
    return;
next_object:
    object_index++;
    if (object_index <= 5) goto scan_objects;
}
