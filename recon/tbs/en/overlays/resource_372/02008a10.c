/* NONMATCHING: resource_372 at 0x02008a10, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE.C, stays listing.
 *
 * Remaining difference: its messages have catalogue names now; 3 halfwords
 * still differ from the ROM, and it names symbols no link defines
 * (Func_02005264_a, Func_02005270, Func_02005284, Func_020052ea, ...).
 */

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaBigBoyWhy[];
extern u8 MsgHaidiaKnowWayGo[];
extern u8 MsgHaidiaKyleAbleStop[];

void Scene_DoraSendsRobinToThePlaza(void)
{
    u32 i;
    s32 record;
    s32 base5_e5c;

    Event_Begin();
    Func_02005264_a();
    Func_02005270();
    Func_02005284();
    Task_Wait(60);
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(0x13c0000, 0xa00000, 0x3700000, 1);
    Actor_SetPosition(ACTOR_KYLE, 0x1260000, 0x3640000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 16;
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_020052ea();
    Audio_PlayCue(158);
    Map_AnimateCells(0x200d78a, 50, 44);
    Actor_SetAttachedEffect(22, 0x101);
    Actor_SetSpeed(ACTOR_DORA, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_KYLE, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_DORA, 0x1560000, 0x37a0000);
    Actor_WalkToAndWait(ACTOR_DORA, 0x156, 0x389);
    Func_0200537c();
    Actor_WalkTo(ACTOR_DORA, 0x128, 0x389);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1560000, 0x37a0000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x156, 0x37a);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x156, 0x389);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x13e, 0x389);
    Actor_SetAnimation(ACTOR_DORA, 1);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 60);
    base5_e5c = (s32)MsgHaidiaKyleAbleStop;
    Event_SetMessage(base5_e5c);
    Event_ShowMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_KYLE, 0x126, 0x346);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_KYLE, 0x4000, 0);
    Event_ShowMessageAndWait(ACTOR_KYLE, 0, 20);
    Actor_ShowEmote(ACTOR_DORA, 0x101, 20);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_KYLE, 4);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_DORA, 0, 50);
    Actor_FaceDirection(ACTOR_DORA, 0xc000, 10);
    Actor_SetSpeed(ACTOR_DORA, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_DORA, 0x121, 0x373);
    Actor_FaceDirection(ACTOR_DORA, 0xe000, 0);
    Event_ShowMessage(ACTOR_DORA, 0);
    Actor_RunRepeatedMotion(ACTOR_KYLE, 2);
    Event_ShowMessage(ACTOR_KYLE, 0);
    Actor_SetAnimationAndWait(ACTOR_DORA, 4);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_FaceDirection(ACTOR_DORA, 0x2000, 10);
    Event_SetMessage((base5_e5c + 8));
    Event_OpenMessage(ACTOR_DORA, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x12e, 0x389);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    while (Event_ChooseYesNo(0, 0) == 1) {
        Actor_RunRepeatedMotion(ACTOR_DORA, 1);
        Event_SetMessage((s32)MsgHaidiaBigBoyWhy);
        Event_OpenMessage(ACTOR_DORA, 0);
    }
    Actor_SetAnimationAndWait(ACTOR_DORA, 3);
    Event_SetMessage((s32)MsgHaidiaKnowWayGo);
    Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetSpeed(ACTOR_KYLE, 0x18000, 0xc000);
    Actor_WalkTo(ACTOR_KYLE, 0x129, 0x2ee);
    Event_Wait(10);
    Actor_WalkToAndWait(ACTOR_DORA, 0x129, 0x2ee);
    Actor_SetPosition(ACTOR_DORA, 0, 0);
    Actor_SetPosition(ACTOR_KYLE, 0, 0);
    Actor_SetAnimation(ACTOR_KYLE, 1);
    Actor_SetAnimation(21, 2);
    Actor_SetAnimation(22, 5);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x87b);
    GameFlag_Set(0x205);
    Event_End();
}
