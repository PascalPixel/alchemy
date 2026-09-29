/* The Lunpa fortress: the end of actor 20's sequence. */
#include "FORTRESS.H"
extern u8 MsgRunpaMaybeMerchantReason[];

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
