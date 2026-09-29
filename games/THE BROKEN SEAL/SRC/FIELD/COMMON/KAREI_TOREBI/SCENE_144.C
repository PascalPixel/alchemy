#include "KAREI.H"

void FieldScene_RunScene3ae_02000144(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    GameFlag_Set(0x8aa);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x128);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 0x198, 0x128);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(20);
    Event_End();
}
