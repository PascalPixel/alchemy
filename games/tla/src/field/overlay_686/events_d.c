#include "ov686.h"

void Func_020011c0(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);

    Func_02002e44();
    Func_02002edc(0);
    Object_SetModeById(gPartyState.current_owner, 0);
    Func_02002e9c(gPartyState.current_owner, 258);
    object->motion_flags = 0;
    object->action_flags = 0;
    {
        /* FAKEMATCH: the zero pinned to r1 and set just before the call comes before the owner's load and the 8, as the native has; as a plain argument the 8 comes before the owner (one reordered row), set at its declaration it rises above the stores, behind a one-pass block or empty asm the stores' addresses swap (differ 2 and 4). */
        register s32 zero asm("r1");

        zero = 0;
        ObjectMotion_OffsetPositionAndResetMotion(gPartyState.current_owner, zero, 8);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
    /* FAKEMATCH: called as value-returning, the call sets its constants before the owner as the native does (differ 2 as the void call). */
    ((s32 (*)(s32, s32, s32))Func_02002e94)(gPartyState.current_owner, 256, 0);
    Battle_WaitMode0(10);
    Ov686_CamBounds(-1, -1, -1, 0);
    Func_02002eec(204);
    Func_02000b30(gPartyState.current_owner, 188);
    object->action_flags = 1;
    Func_02002e4c();
}

void Func_02000850(s32 a, s32 id);
void Func_02000c24(s32 a, s32 id);

void Func_0200124c(s32 a, s32 id)
{
    if ((u32)(id - 9) <= 1)
        Func_02000c24(a, id);
    else
        Func_02000850(a, id);
}
