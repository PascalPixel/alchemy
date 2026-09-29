#include "SANCTUM.H"

s32 CheckAllStatueLights(void)
{
    s32 all_set = 1;

    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) == 0)
        all_set = 0;
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) == 0)
        all_set = 0;

    return all_set;
}

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Event_ShowMessage(actor_step, 0);
    Event_Wait(wait_frames);
}
