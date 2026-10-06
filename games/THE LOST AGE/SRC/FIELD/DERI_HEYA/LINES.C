#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"
#include "SCENE_CALL.H"
#include "text/MSG_IDS.H"

void Func_02000d94(s32 actor, s32 jump, s32 frames);

void Func_02000150(s32 unused, s32 actor)
{
    ObjectMotion_SetAngleToward(actor, 4, 0);
    Battle_WaitMode0(10);
    Func_02000d94(actor, 0x103, 50);
    ObjectMotion_Launch(actor, 6, 15);
    ObjectMotion_Launch(actor, 6, 15);
    Battle_WaitMode0(10);
    Engine_EventSetMessage((s32)MsgDeriHeyaWhatAreYouDoing);
    Engine_EventShowMessage(actor, 0);
    Battle_WaitMode0(10);
}

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
