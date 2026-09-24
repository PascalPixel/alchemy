#include "TYPES.H"

/* The Mercury Djinni capture: the Djinni bounces three times, splits into
   eight linked copies of itself, rises out of view, and a ring of slot
   particles closes the scene. */

struct CaptureSprite {
    u8 unknown_00[40];
    s16 *resource;
};

struct CaptureObject {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 priority_flags;
    u8 unknown_24[44];
    struct CaptureSprite *sprite;
    u8 unknown_54;
    u8 mode;
    u8 unknown_56[4];
    u8 collision_flags;
    u8 visible;
    u8 unknown_5c[12];
    struct CaptureObject *linked;
    void *callback;
};

struct CaptureResource {
    u8 unknown_00[28];
    u8 id;
};

extern s32 gGameState[];

struct CaptureObject *Object_GetById(s32 id);
void BattleFx_InitializeSlots(void);
void Unnamed_080b0840Far(s32 value);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
struct CaptureObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
void Object_SetMode(struct CaptureObject *object, s32 mode);
void ObjectDispatch_SetSingleChildField26Far(struct CaptureObject *object, s32 value);
struct CaptureResource *Object_ReplaceResourceEntry(struct CaptureSprite *sprite, struct CaptureResource *resource);
void Func_080090d0(struct CaptureObject *object);
void Resource_ResetEntry(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void Camera_WorldToScreen(s32 *position);
void EffectSlot_Initialize(void *slot, s32 kind, s32 x, s32 y);
void EffectSlot_SetCallback(void *slot, void *callback);
void EffectSlot_SetObjectMode(void *slot, s32 mode);
void ObjectGroup_SetChildValueUnlessFifteenFar(s32 object, s32 value);
void Shop_InitEffectFar(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

void BattleFx_RunMercuryDjinnCapture(s32 arg)
{
    struct CaptureObject *djinni;
    struct CaptureObject *copy;
    struct CaptureObject *previous;
    struct CaptureResource *resource;
    void *effect_slots;
    struct CaptureObject **cursor;
    struct CaptureObject **list;
    u8 *slot;
    s8 *slot_state;
    s32 kind;
    s32 remaining;
    s32 resource_id;
    struct CaptureObject *copies[8];
    struct {
        s32 x;
        s32 y;
        s32 z;
    } position;

    djinni = Object_GetById(arg);
    if (djinni == NULL)
        return;

    BattleFx_InitializeSlots();
    effect_slots = *(void **)0x03001F30;
    Unnamed_080b0840Far(0x204084);
    WaitFrames(30);
    djinni->visible = 0;
    Audio_PlayCue(173);
    Motion_SetVarCbAndRefresh(arg, 1);
    Audio_PlayCue(175);
    Motion_SetVarCbAndRefresh(arg, 1);
    WaitFrames(20);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 3, 14);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 5, 16);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 7, 18);
    WaitFrames(20);

    kind = *djinni->sprite->resource;
    resource = NULL;
    previous = djinni;
    cursor = copies;
    for (remaining = 7; remaining >= 0; remaining--) {
        copy = Object_CreateFar(kind, djinni->x, djinni->y, djinni->z);
        *cursor++ = copy;
        if (copy != NULL) {
            copy->scale_y = 0xf000;
            copy->scale_x = 0xf000;
            copy->mode = 0;
            copy->priority_flags = 2;
            copy->collision_flags |= 1;
            copy->callback = (void *)0x08096575;
            copy->facing = djinni->facing;
            Animation_ApplyChildValuesFar(copy, 9);
            Object_SetMode(copy, 0);
            ObjectDispatch_SetSingleChildField26Far(copy, 0);
            resource = Object_ReplaceResourceEntry(copy->sprite, resource);
            copy->linked = previous;
            previous = copy;
        }
    }

    resource_id = resource->id;
    Audio_PlayCue(153);
    djinni->mode = 0;
    for (remaining = 14; remaining >= 0; remaining--) {
        djinni->y += 0x80000;
        WaitFrames(1);
    }
    Func_080090d0(djinni);
    list = copies;
    for (remaining = 7; remaining >= 0; remaining--)
        Func_080090d0(*list++);
    if (resource_id != 96)
        Resource_ResetEntry(resource_id);
    WaitFrames(10);
    ObjectMotion_ArmCallback(gGameState[125], 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(gGameState[125]), 28);
    WaitFrames(20);

    position.x = djinni->x;
    position.y = djinni->y;
    position.z = djinni->z;
    Camera_WorldToScreen(&position.x);

    slot = (u8 *)effect_slots + 88;
    remaining = 23;
    do {
        EffectSlot_Initialize(slot, 240, position.x, position.z);
        EffectSlot_SetCallback(slot, (void *)0x0809641d);
        EffectSlot_SetObjectMode(slot, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(*(s32 *)slot, 9);
        remaining--;
        WaitFrames(1);
        slot += 72;
    } while (remaining >= 0);

    WaitFrames(120);
    {
        s32 next_state;

        next_state = 2;
        slot_state = effect_slots;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(50);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
