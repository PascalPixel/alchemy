#include "kibombo.h"

void Func_020032b0(void);

void Func_02003dec(void)
{
    struct KibomboObject *leader;

    Kibombo_CamMove(-1, -1, -1, 0);
    Func_02005bbc()->motion_flags = 0;
    Func_02005b4c((s32)MsgAtLastThe);
    Kibombo_MessagePut(17, 0);
    Kibombo_SePlay(78);
    Kibombo_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Kibombo_ObjWalk(gPartyState.current_owner, 86, 455);
    Func_02005b0c(26, 172 << 15, 0x1c70000);
    WaitFrames(1);
    Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
    Kibombo_ObjWalk(26, 72, 455);
    Kibombo_ObjArm(26, 0, 0);
    Kibombo_ObjArm(gPartyState.current_owner, 0x8000, 40);
    Kibombo_ObjArm(26, 0xd000, 0);
    Kibombo_ObjArm(gPartyState.current_owner, 0xe000, 0);
    Kibombo_CamSpeed(0x26666, 0x4ccc);
    Kibombo_CamMove(132 << 17, 0x100000, 210 << 17, 1);
    Func_02005bb4();
    Kibombo_Wait(40);
    Kibombo_CamSpeed(0x6666, 0xccc);
    Kibombo_CamMove(132 << 17, 0x200000, 160 << 17, 1);
    Func_02005bb4();
    Kibombo_Wait(80);
    ObjectMotion_Launch(17, 4, 40);
    Kibombo_MessagePut(17, 0);
    Kibombo_CamSpeed(0x4cccc, 0x9999);
    Kibombo_CamMove(132 << 17, 0x200000, 200 << 17, 1);
    Func_02005bb4();
    Kibombo_Wait(20);
    Kibombo_ObjArm(17, 0x3000, 20);
    Kibombo_MessagePut(0x4000 + 17, 0);
    Kibombo_SePlay(11);
    GameFlag_SetBit(0x8f2);
    Kibombo_SePlay(148);
    Object_GetById(18)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(19)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(20)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(21)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(22)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(23)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(24)->velocity_y = 0x40000;
    WaitFrames(5);
    Object_GetById(25)->velocity_y = 0x40000;
    WaitFrames(20);
    Kibombo_ObjArm(17, 0xd000, 20);
    Func_020032b0();
    Kibombo_Wait(40);
    Kibombo_CamSpeed(0x3333, 0x666);
    Kibombo_CamMove(132 << 17, 0, 200 << 17, 1);
    Func_02005bb4();
    Kibombo_Wait(200);
    Kibombo_CamSpeed(0x4cccc, 0x9999);
    Kibombo_CamMove(222 << 15, 0x200000, 228 << 17, 1);
    Func_02005bb4();
    Kibombo_ObjArm(26, 0, 0);
    Kibombo_MessagePut(26, 0);
    Kibombo_ObjArm(gPartyState.current_owner, 0x8000, 20);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Object_SetModeById(26, 4);
    Kibombo_MessagePut(26, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Motion_SetModeAndWaitAnimation(26, 3);
    Object_SetModeById(26, 2);
    leader = Object_GetById(gPartyState.current_owner);
    if (leader != 0)
        ObjectMotion_ResetAndSetPosition(26, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(26);
    Func_02005b0c(26, 0, 0);
    GameFlag_SetBit(0x8f1);
}
