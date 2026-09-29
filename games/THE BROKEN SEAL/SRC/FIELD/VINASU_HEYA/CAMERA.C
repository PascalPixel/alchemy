#include "ENTRY_SETUP.H"

void FieldScene_SetupActorTenCamera(void)
{
    u8 *state = *(u8 **)&gEventWork;
    {
        u16 *target = (u16 *)(state + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        u16 *target = (u16 *)(state + 0xcb6);
        s32 shown = 1;

        *target = shown;
    }
    Event_Begin();
    Event_SetMessage(MSG_THOUGHT_ID_EXPLORE_AFTER_DOOR);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(10, 0, 20);
    SetActorDirection(10, 57344, 0);
    Camera_SetSpeed(65536, 8192);
    Camera_MoveTo(29360128, -1, 28311552, 1);
    Camera_WaitForMove();
    Event_ShowMessage(10, 0);
    Event_End();
}
