#include "local.h"

extern char SceneId_SuhallaGate1;
extern char SceneId_VenusEntrance;
extern u8 MsgWaahhhItS[];

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Func_02000a40(s32 a, s32 b);

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as local.h's layer does (differ 2 called directly). */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

// After the "Waahhh" message, places the leader and the camera, runs actor
// 8's motions and sends the party to Suhalla Gate 1, entrance 99.
void Func_020003ac(void)
{
    Func_02000988();
    Func_02000a70(0);
    Func_02000a00((s32)MsgWaahhhItS);
    ObjectMotion_ResetAndSetPositionInMode2(gPartyState.currentOwner, 472, 320);
    Motion_CamBounds(0x1c80000, -1, 0x1180000, 1);
    Func_02000a30();
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0xa000, 0);
    Battle_WaitMode0(10);
    ObjectMotion_Launch(8, 6, 20);
    Battle_WaitMode0(20);
    Func_02000a08(8, 0, 10);
    {
        s32 x = 256;
        s32 z = 50;

        /* FAKEMATCH: the native builds both arguments before the actor id, as Func_0200071c's barrier does (differ 2 without). */
        __asm__ volatile("" : "+r"(x), "+r"(z));
        Func_02000a18(8, x, z);
    }
    Func_02000a08(8, 0, 10);
    Motion_SetModeAndWaitAnimation(8, 3);
    Battle_WaitMode0(30);
    Func_02000a08(8, 0, 10);
    ObjectMotion_SetPositionAndReset_(8, 456, 312);
    Party_SetFields1eeAnd1f0((s32)&SceneId_SuhallaGate1, 99);
    Party_SetFields1f2And1f4((s32)&SceneId_VenusEntrance, 98);
    gPartyState.battleRule24b = 2;
    Func_02000a40(9, 1);
    Func_02000990();
}
