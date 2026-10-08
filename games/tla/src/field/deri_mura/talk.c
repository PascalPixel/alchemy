#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"


void DeriMura_TalkBeenBefore(void)
{
    s32 msg;

    if (Engine_GameFlagIsSet(0x843) == 0) {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        msg = (s32)MsgDeriBeenBefore;
        Engine_EventSetMessage(msg);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(Engine_PartyGetLeaderActor(), 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(msg + 1);
        } else {
            Engine_EventWait(20);
            Engine_EventSetMessage(msg + 2);
        }
        Engine_EventShowMessage(8, 0);
        Engine_GameFlagSet(0x843);
        Engine_EventEnd();
    } else {
        Engine_EventSetMessage((s32)MsgDeriWaveNoInjuries);
        Engine_EventShowMessage(8, 0);
    }
}

void DeriMura_TalkIndraMoved(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    msg = (s32)MsgDeriIndraMoved;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(9, 0);
    if (Engine_EventChooseYesNo(Engine_PartyGetLeaderActor(), 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventWait(20);
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(9, 0);
    Engine_EventEnd();
}

void DeriMura_TalkBoat(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    msg = (s32)MsgDeriWantBoat;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(9, 0);
    if (Engine_EventChooseYesNo(Engine_PartyGetLeaderActor(), 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventWait(20);
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(9, 0);
    Engine_EventEnd();
}
