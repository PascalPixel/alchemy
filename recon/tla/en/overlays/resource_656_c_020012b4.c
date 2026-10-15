// near miss: differ 22 (listing 68 instructions, draft 68); registers: the native keeps the owner's address in r5 and the leader in r6; the draft swaps them
#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"

void Func_020012b4(void)
{
    struct LemurianShipObject *leader = Object_GetById(gPartyState.currentOwner);
    struct LemurianShipObject *object;

    Func_02002c68();
    Func_02002da8(0);
    Func_02002d68(0x80000, 0x100000, 0xf80000, 0x1000000);
    Func_02002cd0(gPartyState.currentOwner, leader->x, leader->z + 0xfed00000);
    object = Object_GetById(64);
    Func_02002cd0(64, object->x, object->z + 0xfed00000);
    Object_SetPartAttribute(Object_GetById(64), 0);
    WaitFrames(10);
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 0);
    WaitFrames(1);
    LemurianShip_BlendStart(6, 0, 1, 2, 6, 4);
    Func_02002c10();
    WaitFrames(1);
    GameFlag_SetBit(0x9e4);
    Func_02002d88(0x10000, 0);
    Func_02002c70();
}
