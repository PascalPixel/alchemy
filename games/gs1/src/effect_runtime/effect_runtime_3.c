#include "scene.h"
#include "effect_runtime.h"
#include "global_cells.h"

/* effect_runtime/find_slot_by_object.c */
s32 EffectRuntime_FindSlotByObject(s32 object)
{
    s32 index;
    s32 found;
    struct EffectRuntimeSlot *slot;

    slot = EFFECT_RUNTIME->slots;
    found = -1;
    index = 0;
    if (slot->active == object) {
        found = 0;
    } else {
loop_2:
        index += 1;
        slot++;
        if (index <= 9) {
            if (slot->active == object) {
                found = index;
            } else {
                goto loop_2;
            }
        }
    }
    return found;
}

/* effect_runtime/set_mode5_and_play_cue.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode5AndPlayCue(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 5);
        }
        Audio_PlayCue(0x7D);
        WaitFrames(0xC);
    }
}

/* effect_runtime/set_mode7_and_launch.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(void *, s32);

s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode7AndLaunch(void)
{
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        WaitFrames(0x12);
        if (object != NULL) {
            Object_SetMode(object, 7);
        }
        Audio_PlayCue(0x92);
        if (object != NULL) {
            FIELD_AT_OFFSET(object, s32 *, 0x28) = 0x80000;
            Sys_Apply(object, 1);
        }
    }
}

/* effect_runtime/set_mode4_and_play_cue.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode4AndPlayCue(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 4);
        }
        Audio_PlayCue(0x7C);
        WaitFrames(0xC);
    }
}

/* effect_runtime/set_mode2.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetMode2(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 2);
        }
    }
}

/* effect_runtime/get_current_object.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

s32 EffectRuntime_GetCurrentObject(void)
{
    s32 slot;

    slot = EffectRuntime_FindSlotByObject();
    if (slot == -1) {
        return 0;
    }
    { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; return *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
}

/* effect_runtime/clear_current_flags.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_ClearCurrentFlags(void)
{
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != NULL) {
            FIELD_AT_OFFSET(object, s8 *, 0x54) = 0;
        }
    }
}

/* effect_runtime/set_current_position.c */
s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetCurrentPosition(s32 unused, s32 x, s32 y)
{
    s32 slot;
    s32 angle;
    s32 objectX;
    s32 objectY;
    void *slotData;
    void *slotBase;
    void *object;

    objectY = y;
    objectX = x;
    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        slotBase = *(s32 *)ADDR_03001EBC + (slot * 8);
        slotData = slotBase + 0x11C;
        object = FIELD_AT_OFFSET(slotBase, void **, 0x11C);
        if (object != NULL) {
            if (objectX == -1) {
                objectX = (FIELD_AT_OFFSET(slotData, u8 *, 6) << 0x14) + 0x80000;
            }
            if (objectY == -1) {
                objectY = (FIELD_AT_OFFSET(slotData, u8 *, 7) << 0x14) + 0x80000;
            }
            FIELD_AT_OFFSET(object, s32 *, 8) = objectX;
            FIELD_AT_OFFSET(object, s32 *, 0x10) = objectY;
            angle = Sys_Place(0, objectX, objectY);
            FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
            FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
        }
    }
}
