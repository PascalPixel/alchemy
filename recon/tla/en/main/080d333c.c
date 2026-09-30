/*
 * Draft: Object_DriftAndDamp does not yet match; the reference loads the
 * second drift before adding the first and builds 0x400 later (5 units); with the tagged pin below, 3 units remain: 0x400 is started right after the z load.
 * Links as recon/tla/raw/080d333c.s.
 */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

s32 Math_Div(s32 numerator, s32 denominator);

/* Moves an object by its two drift values, lifts it by 1/64 of a unit and
   damps the drifts by 1/18 and 1/16. */
void Object_DriftAndDamp(struct ObjectRuntime *object)
{
    s32 drift_x;
    s32 drift_z;

    s32 pos;
    s32 lift;

    drift_x = object->speed_limit;
    pos = object->x;
    drift_z = object->acceleration;
    asm volatile("" : "+l"(drift_z)); /* FAKEMATCH: ⚓️ loads both drifts first */
    object->x = pos + drift_x;
    object->z += drift_z;
    object->y += 0x400;
    object->speed_limit = drift_x - Math_Div(drift_x, 18);
    object->acceleration = drift_z - drift_z / 16;
}
