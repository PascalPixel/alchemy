/*
 * Draft: ObjectDispatch_Initialize does not yet match; 3 halfwords differ from ☀️'s C, first at +0x6 (ldr r3, [pc, #24]).
 * Links as recon/tla/raw/080233a8.s.
 */
#include "OBJECT_DISPATCH.H"

struct State_0800b7c0;

void ObjectDispatch_Initialize(struct DispatchObject *object, u32 value)
{
    if (object != 0) {
        object->value_04 = 0;
        object->value_00 = value;
        object->value_5b = 0;
        object->value_5d = 0;
        object->value_57 = 0;
    }
}
