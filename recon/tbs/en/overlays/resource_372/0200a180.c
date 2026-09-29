/* NONMATCHING: resource_372 at 0x0200a180, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_E.C, stays listing.
 *
 * Remaining difference: its messages have catalogue names now; 679 halfwords
 * still differ from the ROM, and it names symbols no link defines
 * (Func_020067f6, Func_02006850, Func_02006890, Func_02006c84, ...); it also
 * lacks declarations it needs to compile.
 */

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaNoBrother[];

void FieldScene_RunFlagGatedActorSequence(void)
{
    s32 kc0_1 = 0xC00000;
    s32 k4be_1 = 0x4BE0000;
    s32 k2000_1 = 0x2000;
    s32 kc0_2 = 0xC00000;
    s32 k4a5_1 = 0x4A50000;
    s32 k2000_2 = 0x2000;
    s32 ke3_1 = 0xE30000;
    s32 k4be_2 = 0x4BE0000;
    s32 k4000_1 = 0x4000;
    s32 kfa_1 = 0xFA0000;
    s32 k4be_3 = 0x4BE0000;
    s32 k4000_2 = 0x4000;
    s32 ke3_2 = 0xE30000;
    s32 k4a5_2 = 0x4A50000;
    s32 k2000_3 = 0x3000;
    s32 kf3_1 = 0xF30000;
    s32 k4fd_1 = 0x4FD0000;
    s32 kc000_1 = 0xC000;
    s32 k100_1 = 0x100;
    s32 k446_1 = 0x446;
    s32 k446_2 = 0x446;
    s32 k4000_3 = 0x4000;
    s32 k4000_4 = 0x4000;
    s32 k40000_1 = 0x40000;
    s32 k8000_1 = 0x8000;
    s32 kd8_1 = 0xD80000;
    s32 ng1 = -1;
    s32 k4d0_1 = 0x4D00000;
    s32 k3000_1 = 0x3000;
    s32 k30000_1 = 0x30000;
    s32 k6000_1 = 0x6000;
    s32 ke8_1 = 0xE80000;
    s32 ng2 = -1;
    s32 k4e5_1 = 0x4E50000;
    s32 k9999_1 = 0x9999;
    s32 k1333_1 = 0x1333;
    s32 kd8_2 = 0xD80000;
    s32 ng3 = -1;
    s32 k4d0_2 = 0x4D00000;
    s32 k102_1 = 0x102;
    s32 k102_2 = 0x102;
    s32 k4b5_1 = 0x4B5;
    s32 k4b1_1 = 0x4B1;
    s32 ke8_2 = 0xE80000;
    s32 ng4 = -1;
    s32 k4e5_2 = 0x4E50000;
    s32 kf3_2 = 0xF30000;
    s32 k4fd_2 = 0x4FD0000;
    s32 k20000_1 = 0x20000;
    s32 k19999_1 = 0x19999;
    s32 k3333_1 = 0x3333;
    s32 kd8_3 = 0xD80000;
    s32 ng5 = -1;
    s32 k4d0_3 = 0x4D00000;
    s32 k105_1 = 0x105;
    s32 k800a_1 = 0x800A;
    s32 k9999_2 = 0x9999;
    s32 k4ccc_1 = 0x4CCC;
    s32 k9999_3 = 0x9999;
    s32 k4ccc_2 = 0x4CCC;
    s32 k4ba_1 = 0x4BA;
    s32 k4a5_3 = 0x4A5;
    s32 k6000_2 = 0x6000;
    s32 k8000_2 = 0x8000;
    s32 k8018_1 = 0x8018;
    s32 kc000_2 = 0xC000;
    s32 k800a_2 = 0x800A;
    s32 k105_2 = 0x105;
    s32 k105_3 = 0x105;
    s32 k106_1 = 0x106;
    s32 k8000_3 = 0x8000;
    s32 kc000_3 = 0xC000;
    s32 k4000_5 = 0x4000;
    s32 kc000_4 = 0xC000;
    s32 k9000_1 = 0x9000;
    s32 ka000_1 = 0xA000;
    s32 k8000_4 = 0x8000;
    s32 k800a_3 = 0x800A;
    s32 k105_4 = 0x105;
    s32 k105_5 = 0x105;
    s32 k105_6 = 0x105;
    s32 k105_7 = 0x105;
    s32 k8000_5 = 0x8000;
    s32 k8000_6 = 0x8000;
    s32 k800a_4 = 0x800A;
    s32 kd000_1 = 0xD000;
    s32 k2000_4 = 0x2000;
    s32 ka000_2 = 0xA000;
    s32 k8000_7 = 0x8000;
    s32 v83a_2 = 0x83A;
    u8 *tbl;
    s32 w16;
    s32 m;
    s32 one;

    if (GameFlag_IsSet(0x83a) != 0) {
        return;
    }
    Event_Begin();
    Actor_SetPosition(10, kc0_1, k4be_1);
    Actor_FaceDirection(10, k2000_1, 0);
    Actor_SetAnimation(10, 5);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(10);
        v = Func_020067f6(Random_Next(), 0x5A) + 60;
        tbl = Data_0200cec8;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(10, tbl);
    }
    Actor_SetPosition(9, kc0_2, k4a5_1);
    Actor_FaceDirection(9, k2000_2, 0);
    Actor_SetPosition(24, ke3_1, k4be_2);
    Actor_FaceDirection(24, k4000_1, 0);
    Actor_SetAnimation(24, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(24);
        v = Func_02006850(Random_Next(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(24, tbl);
    }
    Actor_SetPosition(25, kfa_1, k4be_3);
    Actor_FaceDirection(25, k4000_2, 0);
    Actor_SetAnimation(25, 6);
    {
        u8 *o;
        s32 v;
        o = Actor_Get(25);
        v = Func_02006890(Random_Next(), 0x5A) + 60;
        *(u16 *)(o + 0x64) = v;
        Actor_EnableActionCallback(25, tbl);
    }
    Actor_SetPosition(26, ke3_2, k4a5_2);
    Actor_FaceDirection(26, k2000_3, 0);
    Actor_SetPosition(23, kf3_1, k4fd_1);
    Actor_FaceDirection(23, kc000_1, 0);
    Actor_SetSpriteFlags(Actor_Get(23), 0);
    Task_Wait(3);
    Event_SetMessage((s32)MsgHaidiaNoBrother);
    Event_ShowMessage(0x201a, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, k100_1, 20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 150, k446_1);
    {
        u8 *p;
        p = Actor_Get(ACTOR_PARTY_LEADER);
        if (p != 0) {
            Actor_SetPosition(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
        }
    }
    Actor_WalkToAndWait(22, 132, k446_2);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, k4000_3, 0);
    Actor_FaceDirection(22, k4000_4, 20);
    Camera_SetSpeed(k40000_1, k8000_1);
    Camera_MoveTo(kd8_1, ng1, k4d0_1, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_RunRepeatedMotion(23, 3);
    Actor_FaceDirection(9, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(9, k3000_1, 10);
    Camera_SetSpeed(k30000_1, k6000_1);
    Camera_MoveTo(ke8_1, ng2, k4e5_1, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Audio_PlayCue(134);
    Actor_Jump(23, 4, 0);
    Actor_SetAnimation(23, 6);
    Event_Wait(10);
    Actor_SetPosition(23, 0, 0);
    Event_Wait(60);
    Func_02006c84();
    Actor_SetAnimation(10, 1);
    {
        u8 *o;
        o = Actor_Get(10);
        w16 = 0x10000;
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_SetAnimation(24, 1);
    {
        u8 *o;
        o = Actor_Get(24);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_SetAnimation(25, 1);
    {
        u8 *o;
        o = Actor_Get(25);
        *(s32 *)(o + 0x18) = w16;
        *(s32 *)(o + 0x1C) = w16;
    }
    Actor_StartRepeatedMotion(10, 2);
    Actor_StartRepeatedMotion(9, 2);
    Actor_StartRepeatedMotion(24, 2);
    Actor_StartRepeatedMotion(25, 2);
    Actor_RunRepeatedMotion(26, 2);
    Camera_SetSpeed(k9999_1, k1333_1);
    Camera_MoveTo(kd8_2, ng3, k4d0_2, 1);
    Camera_WaitForMove();
    Actor_SetAttachedEffect(26, k102_1);
    Actor_SetAttachedEffect(9, k102_2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(26, 2);
    Actor_StartRepeatedMotion(26, 3);
    Event_ShowMessage(26, 0);
    Actor_Jump(25, 2, 0);
    Actor_SetDestination(25, 234, k4b5_1);
    Actor_Jump(26, 2, 0);
    Actor_SetDestination(26, 227, k4b1_1);
    Event_Wait(90);
    Camera_MoveTo(ke8_2, ng4, k4e5_2, 1);
    Camera_WaitForMove();
    Actor_SetPosition(23, kf3_2, k4fd_2);
    Task_Wait(1);
    Audio_PlayCue(106);
    {
        u8 *o;
        o = Actor_Get(23);
        *(s32 *)(o + 0x28) = k20000_1;
    }
    Event_Wait(6);
    Actor_SetAnimation(23, 7);
    Event_Wait(20);
    Func_02006db4();
    Event_Wait(20);
    Camera_SetSpeed(k19999_1, k3333_1);
    Camera_MoveTo(kd8_3, ng5, k4d0_3, 1);
    Camera_WaitForMove();
    Actor_RunRepeatedMotion(24, 2);
    Event_Wait(20);
    Actor_ShowEmote(24, k105_1, 40);
    Actor_FaceEachOther(24, 10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_ShowMessage(k800a_1, 0);
    {
        u8 *o;
        o = Actor_Get(25);
        o += 0x5A;
        m = 0xFE;
        {
            u8 lv = m & *o;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Get(26);
        o += 0x5A;
        *o = *o & m;
    }
    Actor_SetSpeed(25, k9999_2, k4ccc_1);
    Actor_SetSpeed(26, k9999_3, k4ccc_2);
    Actor_SetDestination(25, 247, k4ba_1);
    Actor_MoveToAndWait(26, 227, k4a5_3);
    {
        u8 *o;
        o = Actor_Get(25);
        o += 0x5A;
        one = 1;
        {
            u8 lv = *o | one;
            *o = lv;
        }
    }
    {
        u8 *o;
        o = Actor_Get(26);
        o += 0x5A;
        {
            u8 lv = one | *o;
            *o = lv;
        }
    }
    Actor_FaceDirection(26, k6000_2, 0);
    Actor_FaceDirection(25, k8000_2, 10);
    Actor_SetAnimation(24, 4);
    Event_ShowMessageAndWait(k8018_1, 0, 10);
    Actor_FaceDirection(10, kc000_2, 20);
    Actor_FaceDirection(10, 0, 10);
    Actor_SetAnimation(10, 4);
    Event_ShowMessageAndWait(k800a_2, 0, 10);
    Actor_ShowEmote(24, k105_2, 0);
    Actor_ShowEmote(10, k105_3, 60);
    Actor_ShowEmote(9, k106_1, 20);
    Actor_FaceDirection(9, k8000_3, 40);
    Actor_FaceDirection(9, kc000_3, 20);
    Actor_FaceDirection(9, 0, 30);
    Actor_FaceDirection(9, k4000_5, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(10, kc000_4, 0);
    Actor_FaceDirection(25, k9000_1, 0);
    Actor_FaceDirection(24, ka000_1, 0);
    Actor_FaceDirection(26, k8000_4, 10);
    Actor_RunRepeatedMotion(10, 1);
    Event_ShowMessageAndWait(k800a_3, 0, 10);
    Actor_SetAnimation(9, 4);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(10, k105_4, 0);
    Actor_ShowEmote(24, k105_5, 0);
    Actor_ShowEmote(25, k105_6, 0);
    Actor_ShowEmote(26, k105_7, 40);
    Actor_FaceDirection(9, 0, 10);
    Actor_FaceEachOther(24, 25, 0);
    Event_Wait(20);
    Actor_FaceDirection(9, 0, 0);
    Actor_FaceDirection(10, 0, 10);
    Actor_FaceDirection(24, k8000_5, 0);
    Actor_FaceDirection(25, k8000_6, 10);
    Actor_SetAnimation(24, 3);
    Actor_SetAnimationAndWait(25, 3);
    Actor_FaceEachOther(10, 9, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(10, 1);
    Event_ShowMessageAndWait(k800a_4, 0, 10);
    Actor_SetAnimationAndWait(9, 3);
    Actor_FaceDirection(24, kd000_1, 10);
    Actor_StartRepeatedMotion(24, 1);
    Event_ShowMessageAndWait(24, 0, 10);
    Actor_FaceDirection(10, 0, 0);
    Actor_FaceDirection(9, 0, 0);
    Actor_RunRepeatedMotion(26, 1);
    Actor_FaceDirection(26, k2000_4, 20);
    Actor_FaceDirection(25, ka000_2, 20);
    Actor_SetAnimationAndWait(25, 3);
    Event_ShowMessageAndWait(25, 0, 10);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_FaceDirection(26, k8000_7, 10);
    Actor_SetAnimationAndWait(26, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Event_ShowMessageAndWait(9, 0, 10);
    Func_02005116();
    GameFlag_Set(v83a_2);
    Event_End();
}
