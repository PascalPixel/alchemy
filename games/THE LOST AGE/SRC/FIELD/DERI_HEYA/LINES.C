#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"

/* Lines spoken in Daila's houses, and an event opening that raises the
   leader's sprite. */

extern u8 MsgDeriHeyaScamps[];
extern u8 MsgDeriHeyaMazeTreasure[];

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

/* Begins an event that the caller ends, with the leader drawn in front. */
void DeriHeya_BeginLeaderEvent(void)
{
    Engine_EventBegin();
    Engine_EventResolvePendingActions(0);
    Engine_ActorSetSpritePriority(gPartyState.current_owner, 1);
}
