#include "TYPES.H"
#include "TLA_EDITION.H"
#include "OWNER_STATE.H"
#include "RAM_BUFFER.H"

/* Summon charge channels: each class holds a mask of the channels in use,
   and a unit's name ends in the marker digit of its channel. */
#define LIST_MARKER_CHAR SUMMON_CHANNEL_MARKER

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];   /* 0x10 */
    u32 used_masks[6];  /* 0x1c */
    s8 channels[6];     /* 0x34 */
    u8 unknown_3a[6];
    u8 count;           /* 0x40 */
};

/* ⚓️ keeps the class as a halfword at 0x14a; ☀️ as a byte at 0x128. */
struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[0x11b];
    u8 unavailable;     /* 0x129 */
    u8 unknown_12a[0x20];
    u16 class_id;       /* 0x14a */
};

s32 Summon_ReleaseCharge(s32 actor_id)
{
    struct SummonChargeState *state = (struct SummonChargeState *)Ram_HeapSlots->battle_work;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = Owner_GetState(actor_id);
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
    summon = (u8 *)Owner_GetState(object_index + 0x80);
    occupied = summon[298];
    if (occupied != 1) goto next_object;
    if (*(u16 *)(summon + 330) != class_id) goto next_object;
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
