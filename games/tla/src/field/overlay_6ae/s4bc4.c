#include "ov6ae.h"

void Func_02004bc4(void)
{
    Func_0200560c();
    Func_02005784(0);
    Func_02005704(19, 0x5000);
    Func_020056dc(MsgDoNotLet);
    Func_020056f4(0x8013, 0);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 480, 348);
    Ov6ae_ObjTurn(19, 0xb000);
    Func_02005614();
}

void Func_02004c2c(void)
{
    Func_0200560c();
    Func_02005784(0);
    Func_02005704(19, 0x3000);
    Func_020056dc(MsgDoNotLet);
    Func_020056f4(0x8013, 0);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 544, 348);
    Ov6ae_ObjTurn(19, 0xb000);
    Func_02005614();
}
