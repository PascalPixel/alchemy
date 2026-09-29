#include "TOPIC.H"
extern u8 MsgTorebiLuckyWheelsPrizesPrizesDetermined[];
extern u8 MsgTorebiLuckyWheelsRulesPullLever[];

void SceneDialogue_RunMessage0e34(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiLuckyWheelsRulesPullLever);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void SceneDialogue_RunMessage0e35(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgTorebiLuckyWheelsPrizesPrizesDetermined);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void FieldScene_RunIndexedStep0(void)
{
    TorebiIzumi_OfferLuckyWheels(0);
}
