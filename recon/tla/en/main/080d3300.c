/*
 * Draft: ObjectMotion_SetVariantCallback does not yet match; 4 bytes differ from +0x18.
 * Links as recon/tla/raw/080d3300.s.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)(Object_VariantMotionScripts + ((3 - variant) << 7)));
    }
}
