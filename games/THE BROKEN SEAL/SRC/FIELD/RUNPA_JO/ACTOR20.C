/* The Lunpa fortress: actor 20's sequence and its end. */
#include "FORTRESS.H"
extern u8 MsgRunpaDontLookNearly[];
extern u8 MsgRunpaMaybeMerchantReason[];
extern u8 MsgRunpaWhWhWho[];
extern u8 MsgRunpaWontTellAnyone[];

void RunActor20SceneSequence(void)
{
    u32 i;
    s32 record;
    s32 msg;
    s32 msg2;

    if (GameFlag_IsSet(0x226) != 0) {
        Event_SetMessage((s32)MsgRunpaWontTellAnyone);
        Event_ShowMessage(20, 0);
    } else {
        Event_Begin();
        Actor_FaceActor(20, ACTOR_PARTY_LEADER, 0);
        if (GameFlag_IsSet(0x227) == 0) {
            Actor_Jump(20, 4, 0);
            Actor_Stop(20);
            Object_RefreshSelectorById(20);
            Event_Wait(20);
            msg = (s32)MsgRunpaWhWhWho;
            Event_SetMessage(msg);
            Event_ShowMessage(20, 0);
            Actor_ShowEmote(20, 0x102, 30);
            Event_SetMessage(msg + 1);
            Event_ShowMessage(20, 0);
            Event_Wait(30);
            Actor_SetAnimation(20, 4);
            Event_Wait(30);
        }
        msg2 = (s32)MsgRunpaDontLookNearly;
        Event_SetMessage(msg2);
        Event_ShowMessage(20, 0);
        Actor_ShowEmote(20, 0x101, 40);
        Event_SetMessage(msg2 + 1);
        Event_OpenMessage(20, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage(msg2 + 2);
            Event_OpenMessage(20, 0);
            GameFlag_Set(0x226);
        } else {
            Event_SetMessage(msg2 + 3);
            Event_OpenMessage(20, 0);
        }
        GameFlag_Set(0x227);
        Event_End();
    }
}

void FinishActor20SceneSequence(void)
{

    if (GameFlag_IsSet(0x226)) {
        Event_SetMessage((s32)MsgRunpaMaybeMerchantReason);
        Event_ShowMessage(20, 0);
    } else {
        s16 *q = (s16 *)(((u8*)gEventWork) + 382);

        *q = 0;
        Psynergy_Cancel();
        RunActor20SceneSequence();
    }
}

void NoOpActorCallback(void)
{
}
