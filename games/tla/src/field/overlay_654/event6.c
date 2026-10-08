#include "kabe.h"
#include "text/msg_ids.h"

void Func_020019dc(void)
{
    Kabe_CopyRect(42, 1, 15, 14, 1, 2);
    Kabe_BlendStart(0, 0, 1, 1, 15, 14);
}

void Func_02001a0c(void)
{
    Kabe_CopyRect(15, 9, 15, 14, 1, 2);
    Kabe_BlendStart(14, 14, 1, 1, 15, 14);
}

void Func_02001a3c(void)
{
    struct KabeObject *leader;

    Func_020039d8();
    Func_02003b08(0);
    Func_02003a70((s32)MsgFieldRopeThrownAgain);
    Kabe_MesPut(0x200c, 0);
    Kabe_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Kabe_ObjSpeed(14, 0xcccc, 0x6666);
    Kabe_ObjWalk(gPartyState.current_owner, 272, 336);
    Kabe_ObjTurn(gPartyState.current_owner, 0xc000, 0);
    Kabe_ObjPlace(14, 272 << 16, 336 << 16);
    WaitFrames(1);
    Kabe_ObjWalk(14, 256, 336);
    Kabe_ObjTurn(14, 0, 0);
    Kabe_ObjTurn(gPartyState.current_owner, 0x8000, 40);
    Kabe_ObjTurn(14, 0xd000, 0);
    Kabe_ObjTurn(gPartyState.current_owner, 0xc000, 0);
    Kabe_CamMove(-1, -1, -1, 0);
    Kabe_CamSpeed(0x4cccc, 0x9999);
    Kabe_CamMove(288 << 16, -1, 126 << 16, 1);
    Func_02003ac8();
    Kabe_ObjLaunch(13, 4, 20);
    Kabe_MesPut(0x800d, 0);
    Kabe_ObjModeWait(12, 3);
    Kabe_Wait(20);
    Kabe_ObjVar(12, 2);
    Kabe_ObjLaunch(12, 6, 80);
    Kabe_MesPut(0x800d, 0);
    Kabe_ObjMode(12, 4);
    Kabe_MesPut(0x400c, 0);
    Kabe_ObjTurn(13, 0x3000, 0);
    Kabe_Jump(13, 265, 20);
    Kabe_MesPut(0x800d, 0);
    Kabe_MesPut(0x400c, 0);
    Kabe_ObjTurn(13, 0, 0);
    Kabe_ObjEmote(13, 258);
    Kabe_Wait(20);
    Kabe_MesPut(0x800d, 0);
    Kabe_ObjModeWait(12, 3);
    Kabe_MesPut(0x400c, 0);
    Kabe_CamSpeed(0x33333, 0x6666);
    Kabe_CamMove(420 << 16, -1, 200 << 16, 1);
    Kabe_ObjSpeed(12, 0x19999, 0xcccc);
    Kabe_ObjWalk(12, 346, 186);
    Kabe_ObjWalk(12, 374, 192);
    Kabe_ObjWalk(12, 392, 224);
    Kabe_ObjWalk(12, 394, 246);
    Kabe_ObjWalk(12, 420, 268);
    Kabe_ObjWalk(12, 456, 268);
    Kabe_ObjWalk(12, 488, 240);
    Kabe_ObjWalk(12, 488, 226);
    Kabe_ObjPlace(12, 0, 0);
    Kabe_CamSpeed(0x4cccc, 0x9999);
    Kabe_CamMove(264 << 16, -1, 320 << 16, 1);
    Func_02003ac8();
    Kabe_ObjTurn(gPartyState.current_owner, 0x8000, 0);
    Kabe_ObjTurn(14, 0, 20);
    Kabe_ObjMode(gPartyState.current_owner, 3);
    Kabe_ObjModeWait(14, 3);
    Kabe_ObjMode(14, 2);
    leader = Object_GetById(gPartyState.current_owner);
    if (leader != NULL)
        ObjectMotion_ResetAndSetPosition(14, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
    ObjectMotion_CommitCurrentPositionAndActivate(14);
    Kabe_ObjPlace(14, 0, 0);
    Kabe_ObjPlace(13, 0, 0);
    GameFlag_SetBit(0x872);
    Func_020039e0();
}
