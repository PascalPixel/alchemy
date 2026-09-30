#include "TYPES.H"
#include "SCENE.H"
void ResourceObject_ReleaseFar(void *);
s32 Object_ApplyProjectedPlacementFar(u32 object, u32 *request, u32 *motion, u32 limit);
extern struct FourObjectMotionState *Data_03001f2c;

extern u8 RomBytes_080ad40d[];

/* object/motion/four_object/FourObjectMotion_InitializeTopRow.c */
extern s32 RomBytes_080af304[];

void *ResourceObject_CreateFar(s32);
void AnimationObjects_SelectAnimationFar(void *, s32);

s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void FourObjectMotion_UpdateAllPositions(void);
void FourObjectMotion_UpdateBottomRow(void);

void FourObjectMotion_ClearSlotsAndSchedule(void)
{
    struct FourObjectMotionState *state = Data_03001f2c;
    s32 index = 0;

    do {
        void *object = state->objects[index];

        if (object != 0) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    Scheduler_RemoveCallback((s32)&FourObjectMotion_UpdateAllPositions);
}
