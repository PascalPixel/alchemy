// near miss: differ 15 (listing 140 instructions, draft 139); as Func_02000978
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_02000b2c(void)
{
    Func_02005b4c((s32)MsgHeSeemsTo);
    Func_02005b64(6, 0);
    Ov670_ObjArm(5, 0x4000, 10);
    Object_SetModeById(5, 4);
    Func_02005b64(5, 0);
    Func_02005b94(7, 0x101, 20);
    Func_02005b64(7, 0);
    Ov670_ObjArm(gPartyState.current_owner, 0x2000, 0);
    Ov670_ObjArm(26, 0xb000, 0);
    Ov670_ObjArm(5, 0x6000, 0);
    Ov670_ObjArm(6, 0xe000, 40);
    Ov670_ObjArm(gPartyState.current_owner, 0xe000, 0);
    Ov670_ObjArm(26, 0xd000, 0);
    Ov670_ObjArm(5, 0, 0);
    Ov670_ObjArm(6, 0xe000, 0);
    Func_02005b9c(7, 0x102);
    Battle_WaitMode0(20);
    Func_02005b64(7, 0);
    Func_02005b5c(26, 0, 20);
    Func_02005b64(7, 0);
    Func_02005b94(6, 0x109, 0);
    Func_02005b64(6, 0);
    Ov670_ObjArm(5, 0x6000, 40);
    Ov670_ObjArm(5, 0, 0);
    Func_02005b64(5, 0);
    Func_02005b64(7, 0);
    Func_02005b94(6, 0x103, 0);
    ObjectMotion_Launch(6, 2, 10);
    ObjectMotion_Launch(6, 4, 10);
    Func_02005b64(6, 0);
    Func_02005b94(5, 0x103, 10);
    Func_02005b64(5, 0);
    Func_02005b94(7, 0x103, 10);
    Func_02005b64(7, 0);
}
