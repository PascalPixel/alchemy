#include "ov670.h"

void Party_RemoveOwnerRestored(s32 owner);

void Func_02001218(void)
{
    if (!Ov670_FlagCheck(0x92e)) {
        Func_02005bc4(5);
        return;
    }
    Func_02005a9c();
    Func_02005c3c(0);
    Func_02005b1c(7, gPartyState.current_owner);
    ObjectMotion_SetActionVariant(7, Object_GetById(gPartyState.current_owner)->sprite->priority);
    WaitFrames(1);
    Ov670_ObjArm(gPartyState.current_owner, 0xa000, 0);
    Ov670_ObjSpeed(7, 0x19999, 0xcccc);
    Ov670_ObjWalk(7, 62, 444);
    Ov670_ObjTurn(7, 0x2000);
    Func_02005b4c((s32)MsgNowThatIKnow);
    Ov670_MessagePut(7, 0);
    Object_GetById(7)->priority_flags |= 1;
    Ov670_ObjWalk(7, 56, 420);
    Ov670_ObjWalk(7, 40, 344);
    Func_02005b0c(7, 0, 0);
    Ov670_ObjPlace(gPartyState.current_owner, 80, 510);
    Ov670_Wait(10);
    Func_02005bc4(5);
    GameFlag_ClearBit(0x92e);
    GameFlag_SetBit(0x92f);
    Party_RemoveOwnerRestored(7);
    Func_02005aa4();
}
