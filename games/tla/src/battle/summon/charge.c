#include "types.h"
#include "tla_edition.h"
#include "owner_state.h"
#include "ram_buffer.h"

#define LIST_MARKER_CHAR SUMMON_CHANNEL_MARKER

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];
    u32 used_masks[6];
    s8 channels[6];
    u8 unknown_3a[6];
    u8 count;
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[0x11b];
    u8 unavailable;
    u8 unknown_12a[0x20];
    u16 class_id;
};

s32 EnmCntRel(s32 actor_id)
{
    struct SummonChargeState *state = (struct SummonChargeState *)Ram_HeapSlots->battle_work;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = UnitGet(actor_id);
    if (actor->unavailable != 0)
        return;

    class_id = actor->class_id;
    for (index = 0; index < count; index++) {
        if (state->class_ids[index] == class_id)
            break;
    }
    if (index == count || state->used_masks[index] == 0)
        return;

    for (name_length = 0; name_length <= 13; name_length++) {
        if (actor->name[name_length] == 0)
            break;
    }

    bit = 32;
    if (name_length > 0)
        bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
    state->used_masks[index] &= ~(1 << bit);
}

s32 EnmCntClr(s32 enemy_id)
{
    struct BtlUnit *summon;
    s32 object_index;
    s32 slot;
    s32 next_slot;
    u8 occupied;
    u8 marker;

    object_index = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = UnitGet(object_index + 0x80);
    occupied = summon->side;
    if (occupied != 1) goto next_object;
    if (summon->enemy_id != enemy_id) goto next_object;
    slot = 0;
    if (summon->name[0] != 0) goto scan_slots;
    summon->name[0] = marker;
    summon->name[occupied] = slot;
    return;
scan_slots:
    slot++;
    if (slot > 13) return;
    occupied = summon->name[slot];
    if (occupied != 0) goto scan_slots;
    next_slot = slot + 1;
    summon->name[slot] = marker;
    summon->name[next_slot] = occupied;
    return;
next_object:
    object_index++;
    if (object_index <= 5) goto scan_objects;
}
