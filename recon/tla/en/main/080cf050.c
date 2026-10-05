/* Near miss: differ 2 (55/55 instructions), one scheduling swap. After
   storing the leader's x the listing sets up ArcTan2's second argument
   (adds r1, r6, #0) before loading the other object's z (ldr r3, [r4, #16]),
   which stalls on the store that follows; this draft loads first. The
   range test must be two nested ifs: one && chain folds only the first
   coordinate into an unsigned range check (31). Goes after EVENT_POSE.C. */
#include "TYPES.H"
#include "OBJECT_RUNTIME.H"

s32 EventRuntime_GetControlledOwner(void);
s32 ArcTan2(s32 y, s32 x);
void Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position);

/* Snaps the controlled character onto the first of objects 64-79 within
   sixteen pixels of it and steps it 20 pixels out along the line between
   them, clearing its target. */
void Func_080cf050(void)
{
    struct ObjectRuntime *leader;
    struct ObjectRuntime *other;
    s32 dx;
    s32 dz;
    s32 i;

    leader = ObjectTable_Get(EventRuntime_GetControlledOwner());
    for (i = 0; i <= 15; i++) {
        other = ObjectTable_Get(i + 64);
        dx = leader->x - other->x;
        dz = leader->z - other->z;
        if (dx > -0x100000 && dx < 0x100000)
            if (dz > -0x100000 && dz < 0x100000) {
                leader->x = other->x;
                leader->z = other->z;
                Vector_AddPolarOffset(0x140000, ArcTan2(dz, dx), &leader->x);
                leader->target_x = 0x80000000;
                leader->target_y = 0x80000000;
                leader->target_z = 0x80000000;
                break;
            }
    }
}
