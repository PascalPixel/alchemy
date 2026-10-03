#include "FOUR_OBJECT_MOTION.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "INVENTORY_MENU.H"
#include "ANIMSPR.H"

extern s32 FourObjectMotion_ResourceIds[];
void ResourceObject_ReleaseFar(void *);
void *ResourceObject_CreateFar(s32);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
void FourObjectMotion_UpdateBottomRow(void);

void FourObjectMotion_InitializeBottomRow(void)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;
    s32 index;

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
    }
    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

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
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;

    if (state->objects[index] != NULL) {
        state->positions_x[index] = x;
        state->positions_y[index] = negative != 0 ? y | (s32)0xffff8000 : y;
        return 0x23c + index * 2;
    }
    return index;
}

void FourObjectMotion_SetSlotPhase(s32 index, s32 phase)
{
    ((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;

    if (state->objects[index] != NULL) {
        ResourceObject_ReleaseFar(state->objects[index]);
        state->objects[index] = NULL;
    }
    {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[kind]);

        if (object != NULL)
            AnimationObjects_SelectAnimationFar(object, value);
        state->objects[index] = object;
    }
    return 1;
}

void FourObjectMotion_ClearSlotsAndScheduleAlt(void)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;
    s32 index = 0;

    do {
        struct AnimationObject *object = state->objects[index];

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
    struct InventoryMenuState *state = gMenuWork;
    s32 index;

    for (index = 0; index < state->party_count; ++index) {
        AnimationObjects_SelectAnimationFar(state->owner_objects[index], 1);
    }
}
