#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "text/MSG_IDS.H"


void DeriMura_TalkHip(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    msg = (s32)MsgDeriHurtHip;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(12, 0);
    if (Engine_EventChooseYesNo(Engine_PartyGetLeaderActor(), 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventWait(20);
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(12, 0);
    Engine_EventEnd();
}
