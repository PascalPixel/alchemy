#include "TASK.H"

extern const struct SceneEvent gKorosseoKabeEvents[];

const struct SceneEvent *Scene_GetEvents(void)
{
    return gKorosseoKabeEvents;
}

void FieldScene_RunPairedEntranceWalk(s32 a0)
{

    u32 i;
    s32 record;

    Actor_Destroy(40);
    Actor_Destroy(41);
    Owner_RefreshActiveRatios(1);
    Event_Begin();
    Actor_SetPosition(8, 0x580000, 0x1000000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1000000);
    Actor_FaceActor(8, 0x4000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
    if (a0 < 0) {
        Actor_SetAnimation(8, 10);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 35);
    } else {
        Actor_SetAnimation(8, 8);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Task_Wait(1);
    Camera_MoveTo(0x680000, 0, 0xc00000, 0);
    FieldScene_RunLateSequence(a0);
    Event_End();
}
