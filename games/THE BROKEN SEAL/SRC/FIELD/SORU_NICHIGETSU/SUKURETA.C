#include "SANCTUM.H"

void Sukureta_Talk(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_ROBIN_SEARCHING_FOR_SUKURETA) != 0) {
        Event_SetMessage(MSG_SUKURETA_LET_ME_KNOW_WHAT_YOU_FIND);
    } else {
        Event_SetMessage(MSG_SUKURETA_JUST_WAIT_OVER_THERE);
    }
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Event_End();
}
