#include "types.h"
#include "tla_edition.h"
#include "owner_state.h"
#include "ram_buffer.h"

#define LIST_MARKER_CHAR SUMMON_CHANNEL_MARKER

struct SummonChargeState {
    u8 unk00[0x10];
    u16 classIds[6];
    u32 usedMasks[6];
    s8 channels[6];
    u8 unk3a[6];
    u8 count;
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unk0e[0x11b];
    u8 unavailable;
    u8 unk12a[0x20];
    u16 classId;
};

s32 EnemyCountRelease(s32 actorId)
{
    struct SummonChargeState *state = (struct SummonChargeState *)Ram_HeapSlots->battleWork;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 nameLength;
    s32 bit;
    s32 classId;

    actor = UnitGet(actorId);
    if (actor->unavailable != 0)
        return;

    classId = actor->classId;
    for (index = 0; index < count; index++) {
        if (state->classIds[index] == classId)
            break;
    }
    if (index == count || state->usedMasks[index] == 0)
        return;

    for (nameLength = 0; nameLength <= 13; nameLength++) {
        if (actor->name[nameLength] == 0)
            break;
    }

    bit = 32;
    if (nameLength > 0)
        bit = actor->name[nameLength - 1] - LIST_MARKER_CHAR;
    state->usedMasks[index] &= ~(1 << bit);
}

s32 EnemyCountClear(s32 enemyId)
{
    struct BattleUnit *summon;
    s32 objectIndex;
    s32 slot;
    s32 nextSlot;
    u8 occupied;
    u8 marker;

    objectIndex = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = UnitGet(objectIndex + 0x80);
    occupied = summon->side;
    if (occupied != 1) goto next_object;
    if (summon->enemyId != enemyId) goto next_object;
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
    nextSlot = slot + 1;
    summon->name[slot] = marker;
    summon->name[nextSlot] = occupied;
    return;
next_object:
    objectIndex++;
    if (objectIndex <= 5) goto scan_objects;
}
