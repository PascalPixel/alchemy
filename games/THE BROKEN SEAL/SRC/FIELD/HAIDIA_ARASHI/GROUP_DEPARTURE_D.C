#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaHuh[];

void FieldScene_DrawFiveTileBlocks(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Map_CopyCellAttributes(29, 20, 1, 1, a, h);
    b = 0x46;
    Map_CopyCellAttributes(29, 20, 1, 1, a, b);
    a2 = 27;
    Map_CopyCellAttributes(29, 20, 1, 1, a2, b);
    Map_CopyCellAttributes(28, 21, 1, 1, 28, h);
    Map_CopyCellAttributes(28, 22, 1, 1, a2, 0x48);
}

void FieldScene_RunScene372SequenceA(void)
{
    s32 base;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x106, 0x32a);
    Actor_SetPosition(20, 0x1060000, 0x3250000);
    Actor_WalkTo(20, 0x106, 0x339);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x11a, 0x357);
    Actor_SetAnimation(20, 1);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 20, 0);
    BattleFx_PlayQueuedSound();
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_ShowEmote(20, 0x100, 20);
    base = (s32)MsgHaidiaHuh;
    Event_SetMessage(base);
    Event_ShowMessage(20, 0);
    Event_Wait(20);
    Event_AskYesNo(20, 0);
    Actor_RunRepeatedMotion(20, 2);
    Event_SetMessage((base + 4));
    Event_ShowMessageAndWait(20, 0, 20);
    Object_SetActionCallbackAndRefreshById(20, (s32)HaidiaArashi_ActorTwentyScript);
    GameFlag_Set(0x835);
    Event_End();
}
