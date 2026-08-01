/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY: the routine at 0x030001d8 is not established.  It RETURNS a
 * value, unlike the other relocated helpers in this audit, and both callers
 * feed it sums of squares and use the result as a distance -- which reads
 * like a magnitude or square root.  That reading is an inference from two
 * call sites and is NOT asserted; the type records the arity and the return,
 * nothing else.
 */
#include "types.h"

typedef s32 (*Resident_030001D8)(s32 arg0, s32 arg1, s32 arg2);

struct ObjectVisual_08093af8 {
    u8 unknown_00[0x28];
    s16 *identifier;
};

struct WorldObject_08093af8 {
    u32 active;
    u8 unknown_04[2];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x3c];
    struct ObjectVisual_08093af8 *visual;
    u8 kind;
    u8 unknown_55[0x1b];
};

s32 Func_080044d0(s32 z, s32 x);

/*
 * Find the nearest active object in front of source, within forty map units,
 * whose visual identifier matches the requested value.
 */
struct WorldObject_08093af8 *Func_08093af8(
    struct WorldObject_08093af8 *source,
    s32 requested_identifier)
{
    struct WorldObject_08093af8 *objects =
        *(struct WorldObject_08093af8 **)0x03001e64;
    struct WorldObject_08093af8 *nearest = 0;
    s32 nearest_distance = 40;
    s32 index;

    for (index = 0; index < 64; index++) {
        struct WorldObject_08093af8 *candidate = &objects[index];

        if (candidate->active != 0 &&
            candidate != source &&
            candidate->kind == 1) {
            s32 vertical = candidate->y - source->y;

            if (vertical >= -0x002fffff && vertical <= 0x002fffff) {
                s32 delta_x = candidate->x - source->x;
                s32 delta_z = candidate->z - source->z;
                s32 z_squared;
                s32 distance;

                if (delta_x < 0) {
                    delta_x += 0x0000ffff;
                }
                if (delta_z < 0) {
                    delta_z += 0x0000ffff;
                }
                delta_x >>= 16;
                delta_z >>= 16;
                z_squared = delta_z * delta_z;
                distance = ((Resident_030001D8)0x030001d8)(
                    delta_x * delta_x + z_squared,
                    candidate->y,
                    z_squared);

                if (distance < nearest_distance) {
                    u16 direction = (u16)Func_080044d0(
                        candidate->z - source->z,
                        candidate->x - source->x);
                    s16 facing_difference =
                        (s16)(direction - source->facing);

                    if (distance <= 23 ||
                        (facing_difference >= -0x2fff &&
                         facing_difference <= 0x2fff)) {
                        nearest = candidate;
                        nearest_distance = distance;
                    }
                }
            }
        }
    }

    if (nearest != 0 &&
        *nearest->visual->identifier == requested_identifier) {
        return nearest;
    }
    return 0;
}
