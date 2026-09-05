#include "staged_actor.h"

#define UpdateStagedActorVerticalPosition Func_02000058
#define GetDescendingMotionStep Func_0200649a
#define GetAscendingMotionStep Func_020064b4

s32 GetDescendingMotionStep(void);
s32 GetAscendingMotionStep(void);

s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor) {
    s16 vertical_motion_direction;
    s32 descending_y;
    s32 ascending_y;

    if (actor->vertical_motion_direction != 0) {
        descending_y = (actor->y
            - ((u32) (GetDescendingMotionStep() << 0xF) >> 0x10)) + 0xFFFF8000;
        actor->y = descending_y;
        if (descending_y < 0) {
            vertical_motion_direction = 0;
            goto block_5;
        }
    } else {
        ascending_y = actor->y
            + ((u32) (GetAscendingMotionStep() << 0xF) >> 0x10) + 0x8000;
        actor->y = ascending_y;
        if (ascending_y > 0x80000) {
            vertical_motion_direction = 1;
block_5:
            actor->vertical_motion_direction = vertical_motion_direction;
        }
    }
    return 1;
}
