/* Tolbi town: the event table by story progress and two actor lines. */
#include "MACHI.H"

u8 *TorebiMachi_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x950) != 0) {
        return gTorebiMachiEvents3;
    }
    if (Engine_GameFlagIsSet(0x962) != 0) {
        return gTorebiMachiEvents2;
    }
    return gTorebiMachiEvents;
}

void SceneDialogue_RunActor15Message1f92(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(0x1F92);
    Engine_EventAskYesNo(15, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor24Message1f9d(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage(0x1F9D);
    Engine_EventAskYesNo(24, 0);
    Engine_EventEnd();
}
