#include "CAVE.H"
extern u8 MsgRunpaBunzaAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksAboutUnfinishedBusiness[];
extern u8 MsgRunpaGeraldAsksIfNotRiding[];
extern u8 MsgRunpaGeraldChecksNothingLeft[];
extern u8 MsgRunpaLetsTakeWagon[];
extern u8 MsgRunpaMiaAsksIfRidingAfter[];
extern u8 MsgRunpaTotallyConfusedChanged[];

/*
 * Bunza leads the way to his wagon and Gerald asks whether the party rides
 * too. The party questions an answer that seems to change its mind until
 * the choice is settled either way.
 */
void WagonChoice_Run(void)
{
    s32 wagon;
    s32 insisted;
    s32 confusion;

    wagon = (s32)MsgRunpaLetsTakeWagon;
    Event_SetMessage(wagon + WAGON_BUNZA_LEADS_THE_WAY);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage(wagon + WAGON_GERALD_ASKS_TO_RIDE);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
    if (Leader_AnswersYes()) {
ask_about_business:
        if (!Bunza_AsksAboutUnfinishedBusiness()) {
            goto ride;
        }
        insisted = FALSE;
        if (!Gerald_AsksAboutThingsToDo()) {
insist_nothing_left:
            insisted = TRUE;
check_nothing_left:
            Gerald_ChecksNothingLeftToDo();
            if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
                goto ride;
            }
        }
        if (Bunza_CannotWait()) {
            goto stay;
        }
        if (!insisted) {
            goto stay;
        }
        goto check_nothing_left;
    } else {
        if (Gerald_AsksIfNotRiding()) {
            if (Gerald_AsksAboutUnfinishedBusiness()) {
                goto stay;
            }
            goto insist_nothing_left;
        } else {
            if (Mia_AsksIfRidingAfterAll()) {
                goto ask_about_business;
            }
            confusion = (s32)MsgRunpaTotallyConfusedChanged;
            Event_SetMessage(confusion + CONFUSION_IVAN_IS_CONFUSED);
            Event_ShowMessage(ACTOR_IVAN, 0);
            Event_SetMessage(confusion + CONFUSION_GERALD_ASKS_AGAIN);
            Event_OpenMessage(ACTOR_GERALD, 0);
            goto ask_to_ride;
ride:
            Party_RidesWagon();
            goto done;
        }
    }
stay:
    Party_ConfirmsStaying();
    Party_StaysBehind();
done:;
}

u8 Leader_AnswersYes(void)
{
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksIfNotRiding(void)
{
    Event_SetMessage((s32)MsgRunpaGeraldAsksIfNotRiding);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_AsksAboutUnfinishedBusiness(void)
{
    Event_SetMessage((s32)MsgRunpaGeraldAsksAboutUnfinishedBusiness);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Party_ConfirmsStaying(void)
{
    return TRUE;
}

u8 Bunza_AsksAboutUnfinishedBusiness(void)
{
    Event_SetMessage((s32)MsgRunpaBunzaAsksAboutUnfinishedBusiness);
    Event_OpenMessage(ACTOR_BUNZA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

u8 Gerald_ChecksNothingLeftToDo(void)
{
    Event_SetMessage((s32)MsgRunpaGeraldChecksNothingLeft);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return TRUE;
}

u8 Mia_AsksIfRidingAfterAll(void)
{
    Event_SetMessage((s32)MsgRunpaMiaAsksIfRidingAfter);
    Event_OpenMessage(ACTOR_MIA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}
