#include "IMIRU.H"

void FieldScene_RunScene399_02000a3c(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)((s32)Object_GetById(0));
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Inn_Open(4, 16);
    } else {
        Event_Begin();
        Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(MSG_ITS_ALMOST_TIME_FOR_LEAVE);
            Event_AskYesNo(16, 0);
        } else {
            Event_SetMessage(MSG_WHY_HAVE_TWO_GROUPS_TRAVELERS);
            Event_ShowMessage(16, 0);
        }
        Actor_FaceDirection(16, 0x3000, 10);
        Event_End();
    }
}

void FieldScene_RunScene399_02000abc(void)
{
    struct FieldActor *leader;

    leader = (struct FieldActor *)((s32)Object_GetById(0));
    if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
        Event_Begin();
        if (GameFlag_IsSet(0x82d) == 0) {
            Event_SetMessage(MSG_MAY_ONLY_STUDENT_BUT_CAN);
            Event_ShowMessage(19, 0);
            GameFlag_Set(0x82d);
        }
        Event_End();
        Sanctum_Open(19);
    } else {
        Event_Begin();
        if (GameFlag_IsSet(0x881) != 0) {
            Event_SetMessage(MSG_MIA_GOING_ON_JOURNEY_WITH);
            Event_ShowMessage(19, 0);
        } else if (GameFlag_IsSet(3) != 0) {
            Event_SetMessage(MSG_AM_HEALER_WHILE_MIA_OUT);
            Event_ShowMessage(19, 0);
        } else {
            Event_SetMessage(MSG_LOOKING_FOR_MIA);
            (void)Event_AskYesNo(19, 0);
            Actor_FaceDirection(19, 0x3000, 10);
        }
        Event_End();
    }
}
