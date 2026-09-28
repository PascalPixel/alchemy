#include "TOPIC.H"

void SceneDialogue_RunMessage0e34(void)
{
    Event_Begin();
    Event_SetMessage(MSG_LUCKY_WHEELS_RULES_PULL_LEVER);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void SceneDialogue_RunMessage0e35(void)
{
    Event_Begin();
    Event_SetMessage(MSG_LUCKY_WHEELS_PRIZES_PRIZES_DETERMINED);
    Event_OpenMessage(-1, 0);
    Event_End();
}

void FieldScene_RunIndexedStep0(void)
{
    TorebiIzumi_OfferLuckyWheels(0);
}
