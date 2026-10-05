#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"
#include "text/MSG_IDS.H"


s32 DeriHeya_TalkScamps(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDeriHeyaScamps);
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
    return 0;
}

s32 DeriHeya_TalkMazeTreasure(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_EventSetMessage((s32)MsgDeriHeyaMazeTreasure);
    Engine_EventShowMessage(19, 0);
    Engine_EventEnd();
    return 0;
}

void DeriHeya_BeginLeaderEvent(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetSpritePriority(gPartyState.current_owner, 1);
}
