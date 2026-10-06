#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

extern s32 gColossoSceneTaskStatus;
extern u32 gColossoSceneTaskState;
extern s8 Data_0200cc20[][6];

void KorosseoMaruta_CycleLogPoses(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor *actor = Object_GetById(gGameState.selected_actor);
    s32 z = actor->z.fixed >> 20;
    s32 i;
    s32 x;
    s32 pose;

    if (gColossoSceneTaskState == 0) {
        s32 row;

        gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
        row = 11;
        for (i = 18, x = 33; i <= 22; i++, x += 2) {
            pose = Data_0200cc20[gColossoSceneTaskStatus][i - 18];
            Engine_ActorSetAnimation(i, pose);
            Engine_ActorSetAnimation(i + 5, pose + 8);
            Call6((void (*)())Engine_MapCopyCellAttributes, 32, 11, 1, 2, x, row);
            if (pose != 7) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 74, 12, 1, 1, x, row);
            }
        }
        Engine_ActorSetAnimation(28, Data_0200cc20[gColossoSceneTaskStatus][5]);
    } else {
        for (i = 18; i <= 22; i++) {
            pose = Data_0200cc20[gColossoSceneTaskStatus][i - 18];
            if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
                if (z == 11 && pose == 4) {
                    work->raised_trigger = pose;
                }
                if (z == 12 && pose == 5) {
                    work->raised_trigger = pose;
                }
            }
        }
    }
    if (++gColossoSceneTaskState > 17) {
        gColossoSceneTaskState = 0;
    }
}
