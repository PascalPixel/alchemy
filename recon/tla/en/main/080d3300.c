/*
 * Draft: ObjectMotion_SetVariantCallback does not yet match; 4 bytes differ from +0x18.
 * Links as recon/tla/raw/080d3300.s.
 */
#include "OBJECT_RUNTIME.H"

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
    struct ObjectRuntime *object;

    object = ObjectTable_Get(object_id);
    if (object != NULL && variant > 0) {
        if (variant > 3) {
            variant = 3;
        }
        Object_SetCallback(object,
            Object_VariantMotionScripts + ((3 - variant) << 7));
    }
}
