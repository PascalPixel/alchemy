#include "FOUR_OBJECT_MOTION.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"

extern struct FourObjectMotionState *gMenuWork;
extern s32 FourObjectMotion_ResourceIds[];
void ResourceObject_ReleaseFar(void *);
void *ResourceObject_CreateFar(s32);
void AnimationObjects_SelectAnimationFar(void *, s32);
void FourObjectMotion_UpdateBottomRow(void);

/* The menu's item objects and the number currently in use. */
struct MenuItemObjects {
    u8 unknown_000[276];
    void *items[64];
    u8 unknown_214[5];
    u8 count;
};

void FourObjectMotion_InitializeBottomRow(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index;

    for (index = 0; index < 4; index++) {
        void *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
    }
    for (index = 0; index < 4; index++) {
        void *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

        if (object != NULL)
            AnimationObjects_SelectAnimationFar(object, 2);
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    Scheduler_AddOrUpdateCallback((s32)(FourObjectMotion_UpdateBottomRow), 0xc80);
}

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative)
{
    struct FourObjectMotionState *state = gMenuWork;

    if (state->objects[index] != NULL) {
        state->positions_x[index] = x;
        state->positions_y[index] = negative != 0 ? y | (s32)0xffff8000 : y;
        return 0x23c + index * 2;
    }
    return index;
}

void FourObjectMotion_SetSlotPhase(s32 index, s32 phase)
{
    gMenuWork->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
    struct FourObjectMotionState *state = gMenuWork;

    if (state->objects[index] != NULL) {
        ResourceObject_ReleaseFar(state->objects[index]);
        state->objects[index] = NULL;
    }
    {
        void *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[kind]);

        if (object != NULL)
            AnimationObjects_SelectAnimationFar(object, value);
        state->objects[index] = object;
    }
    return 1;
}

void FourObjectMotion_ClearSlotsAndScheduleAlt(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index = 0;

    do {
        void *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
        index++;
    } while (index < 4);
    Scheduler_RemoveCallback((u32)(FourObjectMotion_UpdateBottomRow));
}

void Menu_EnableAllItemObjects(void)
{
    struct MenuItemObjects *state = gMenuWork;
    s32 index;

    for (index = 0; index < state->count; ++index) {
        AnimationObjects_SelectAnimationFar(state->items[index], 1);
    }
}
