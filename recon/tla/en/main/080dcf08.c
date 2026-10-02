/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "TYPES.H"
#include "OBJDISP.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern const u8 BattleFx_UntargetedObjectScript[];

struct Target_08097a54 {
    u8 unknown_00[0x38];
    s32 x;
    s32 y;
    s32 z;
};

void BattleFx_SetCallbackWhenTargetUnset(struct Target_08097a54 *target)
{
    s32 ty;
    s32 tx;

    tx = target->x;
    if (tx == 0x80000000) {
        ty = target->y;
        if ((ty == tx) && (target->z == ty)) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)BattleFx_UntargetedObjectScript);
        }
    }
}
