#include "effect_runtime.h"
#include "object_runtime.h"
#include "system.h"
#include "field.h"
extern struct EffectRuntime *gEventWork;

void ObjectDispatch_ApplyArgumentToChildrenFar(struct ObjectRuntime *, s32);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *, s32);
void AudioCommand_PlayFar(s32);

s32 EffectRuntime_FindSlotByObject(s32 id)
{
    s32 index;
    s32 found;
    struct EffectRuntimeSlot *slot;

    slot = gEventWork->slots;
    found = -1;
    index = 0;
    if (slot->active == id) {
        found = 0;
    } else {
    next_slot:
        index++;
        slot++;
        if (index <= 9) {
            if (slot->active == id)
                found = index;
            else
                goto next_slot;
        }
    }
    return found;
}

void EffectRuntime_ReservedNoOp(void)
{
}

void EffectRuntime_SetMode5AndPlayCue(s32 id)
{
    s32 slot;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        if (object != NULL)
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
        AudioCommand_PlayFar(0x7d);
        WaitFrames(0xc);
    }
}

void EffectRuntime_SetMode7AndLaunch(s32 id)
{
    s32 slot;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        WaitFrames(0x12);
        if (object != NULL)
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
        AudioCommand_PlayFar(0x92);
        if (object != NULL) {
            object->velocity_y = 0x80000;
            ObjectDispatch_SetSingleChildFlagsFar(object, 1);
        }
    }
}

void EffectRuntime_SetMode4AndPlayCue(s32 id)
{
    s32 slot;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        if (object != NULL)
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 4);
        AudioCommand_PlayFar(0x7c);
        WaitFrames(0xc);
    }
}

void EffectRuntime_SetMode2(s32 id)
{
    s32 slot;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        if (object != NULL)
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
    }
}

struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id)
{
    s32 slot = EffectRuntime_FindSlotByObject(id);
    struct EffectRuntimeSlot *slot_data;

    if (slot == -1)
        return NULL;
    slot_data = &gEventWork->slots[slot];
    return slot_data->object;
}

void EffectRuntime_ClearCurrentFlags(s32 id)
{
    s32 slot;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        if (object != NULL)
            object->animation_kind = 0;
    }
}

void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 y)
{
    s32 slot;
    s32 height;
    s32 object_x;
    s32 object_y;
    struct EffectRuntimeSlot *slot_data;
    struct ObjectRuntime *object;

    object_y = y;
    object_x = x;
    slot = EffectRuntime_FindSlotByObject(id);
    if (slot != -1) {
        slot_data = &gEventWork->slots[slot];
        object = slot_data->object;
        if (object != NULL) {
            if (object_x == -1)
                object_x = (slot_data->tileX << 20) + 0x80000;
            if (object_y == -1)
                object_y = (slot_data->tileY << 20) + 0x80000;
            object->x = object_x;
            object->z = object_y;
            height = Map_GetTerrainHeightFar(0, object_x, object_y);
            object->terrain_height = height;
            object->y = height;
        }
    }
}
