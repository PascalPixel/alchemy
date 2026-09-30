/*
 * Draft: Object_SetTargetAndCallback does not yet match; 3 halfwords differ from ☀️'s C, first at +0x30 (ldr r1, [pc, #24]).
 * Links as recon/tla/raw/080d3600.s.
 */
#include "OBJECT_RUNTIME.H"
#include "FIELD_EVENT.H"

void ObjectDispatch_InitializeFar(struct ObjectRuntime *, const void *);

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_56[3] = 0;
        }
        ObjectDispatch_InitializeFar(first, callback);
    }
}
