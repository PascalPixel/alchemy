#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Two of Daila's people, each asking the party a question. */

extern u8 MsgDeriIndraMoved[];
extern u8 MsgDeriWantBoat[];

void DeriMura_TalkIndraMoved(void)
{
    s32 msg;

    Engine_EventBegin();
    Engine_EventPrepareSpeakers(0);
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
    Engine_EventPrepareSpeakers(0);
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
