#include "SANCTUM.H"
extern u8 MsgSoruSukuretaJustWaitOverThere[];
extern u8 MsgSoruSukuretaLetMeKnowWhat[];

void Sukureta_Talk(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(FLAG_ROBIN_SEARCHING_FOR_SUKURETA) != 0) {
        Event_SetMessage((s32)MsgSoruSukuretaLetMeKnowWhat);
    } else {
        Event_SetMessage((s32)MsgSoruSukuretaJustWaitOverThere);
    }
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xc000, 10);
    Event_End();
}
