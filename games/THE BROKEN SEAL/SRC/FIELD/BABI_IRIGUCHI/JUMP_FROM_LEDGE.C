#include "TYPES.H"
#include "CALL.H"

s32 Object_GetById();
void Engine_EventBegin();
void Engine_ActorShowEmote();
void ObjectMotion_SetSpeedParameters();
void Engine_ActorJump();
void ObjectMotion_OffsetPositionAndResetMotion();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Engine_ActorFaceDirection();
void SceneState_ApplyRectsAtActors8And9();
void Engine_EventEnd();

void BabiIriguchi_JumpFromLedge(void)
{
    s32 rec7;

    rec7 = Object_GetById(0);
    Engine_EventBegin();
    if ((*(s32 *)(rec7 + 8) >> 20) != 6) {
        if ((*(s32 *)(rec7 + 8) >> 20) != 18) {
            goto done;
        }
    }
    if ((*(s32 *)(rec7 + 16) >> 20) == 20) {
        *(s32 *)(rec7 + 56) = -0x80000000;
        *(s32 *)(rec7 + 64) = -0x80000000;
        Call3(Engine_ActorShowEmote, 0, 0x100, 20);
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x20000, 0x10000);
        Engine_ActorJump(0, 4, 0);
{ u16 dir = *(u16 *)(rec7 + 6); if ((u16)(dir + 0x4fff) > 0x1fff && (u16)(dir - 0x3001) > 0x1fff) goto step_down; }
        ObjectMotion_OffsetPositionAndResetMotion(0, 16, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Call3(Engine_ActorFaceDirection, 0, 0x8000, 20);
        goto done;
        step_down:;
        Call3(ObjectMotion_OffsetPositionAndResetMotion, 0, 0, -16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    }
    done:;
    SceneState_ApplyRectsAtActors8And9();
    Engine_EventEnd();
}
