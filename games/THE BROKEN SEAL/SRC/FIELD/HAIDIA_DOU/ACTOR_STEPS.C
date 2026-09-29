#include "HAIDIA.H"

void SceneState_SetValues8_3_4(void)
{
    BattleFx_RunPageEffectForSlot(8, 3, 4);
}

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */
void SceneActor_WaitActorDescent(u8 *obj)
{
    s32 cnt = 60;

    while (cnt != 0) {
        WaitFrames(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20))
            break;
    }
    *(s32 *)(obj + 12) = *(s32 *)(obj + 20);
}

/* Point an object toward actor zero using their fixed-point X/Z delta. */
s32 SceneActor_FaceActorZero(u8 *obj)
{
    u8 *target = Actor_Get(ACTOR_PARTY_LEADER);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)ArcTan2(dz, dx);
    return 0;
}

