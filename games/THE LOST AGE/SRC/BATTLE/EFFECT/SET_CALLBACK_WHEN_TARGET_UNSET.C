#include "GLOBAL.H"
#include "TYPES.H"
#include "OBJDISP.H"
#include "OBJECT_RUNTIME.H"


void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *target)
{
    s32 tx;
    s32 ty;

    tx = target->target_x;
    /* FAKEMATCH: ⚓️ loads x before it builds the unset marker. */
    __asm__("");
    if (tx == 0x80000000) {
        ty = target->target_y;
        if ((ty == tx) && (target->target_z == ty)) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)BattleFx_UntargetedObjectScript);
        }
    }
}
