#include "FOUR_OBJECT_MOTION.H"

extern struct FourObjectMotionState *gMenuWork;
extern s32 Data_080af304[];

void ResourceObject_ReleaseFar(void *);
void *ResourceObject_CreateFar(s32);
void Object_InitializeMode(void *, s32);
void FourObjectMotion_UpdateBottomRow(void);
s32 ScheduleCallbackAfterFrames(void (*)(void), s32);
void Scheduler_RemoveCallback(void (*)(void));

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
        void *object = ResourceObject_CreateFar(Data_080af304[index]);

        if (object != NULL)
            Object_InitializeMode(object, 2);
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    ScheduleCallbackAfterFrames(FourObjectMotion_UpdateBottomRow, 0xc80);
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
        void *object = ResourceObject_CreateFar(Data_080af304[kind]);

        if (object != NULL)
            Object_InitializeMode(object, value);
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
    Scheduler_RemoveCallback(FourObjectMotion_UpdateBottomRow);
}
