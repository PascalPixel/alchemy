#include "layout_guard.h"
#include "types.h"

enum { OBJECT_SCAN_COUNT_0808DF1C = 67 };

typedef struct FieldObject_0808df1c {
    u8 padding00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[0x45];
    u8 flags;
} FieldObject_0808df1c;

LAYOUT_OFFSET_GUARD(
    FieldObject0808df1c_Facing,
    FieldObject_0808df1c,
    facing,
    6);
LAYOUT_OFFSET_GUARD(
    FieldObject0808df1c_X,
    FieldObject_0808df1c,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    FieldObject0808df1c_Flags,
    FieldObject_0808df1c,
    flags,
    0x59);

typedef s32 (*LengthGateway_0808df1c)(s32 sum_of_squares);

s32 Func_080022ec(s32, s32);
s32 Func_080044d0(s32, s32);
FieldObject_0808df1c *Func_0808ba1c(u32);
s32 Func_0808ddb8(s32);

static s32 VerticalRange_0808df1c(s32 requested_type)
{
    switch (requested_type) {
    case 0x0d:
        return 0x300000;
    case 5:
        return 0x400000;
    case 2:
        return 0x100000;
    default:
        return 0x080000;
    }
}

static s32 FixedToInteger_0808df1c(s32 value)
{
    if (value < 0)
        value += 0xffff;
    return value >> 16;
}

/*
 * Find the nearest eligible field object within the requested vertical
 * range and the target's forward cone. The fixed 0x030001d8 gateway takes a
 * sum of squares and returns its length; its relocated body has no ROM name.
 */
u32 Func_0808df1c(u32 target_id, s32 requested_type)
{
    static LengthGateway_0808df1c const length_gateway =
        (LengthGateway_0808df1c)0x030001d8;
    FieldObject_0808df1c *target = Func_0808ba1c(target_id);
    u32 nearest_id = (u32)-1;
    s32 nearest_distance = Func_0808ddb8(requested_type);
    s32 vertical_range = VerticalRange_0808df1c(requested_type);
    u16 forward;
    u32 candidate_id;

    if (target == 0)
        return nearest_id;

    forward = (target->facing + 0x2000) & 0xc000;
    for (candidate_id = 0;
         candidate_id < OBJECT_SCAN_COUNT_0808DF1C;
         candidate_id++) {
        FieldObject_0808df1c *candidate;
        s32 vertical_delta;
        s32 delta_x;
        s32 delta_z;
        s32 distance;
        u16 bearing;
        s32 angular_limit;
        s32 angular_delta;

        if (candidate_id == target_id)
            continue;
        candidate = Func_0808ba1c(candidate_id);
        if (candidate == 0 || (candidate->flags & 8) != 0)
            continue;

        vertical_delta = candidate->y - target->y;
        if (vertical_delta < 0)
            vertical_delta = -vertical_delta;
        if (vertical_delta > vertical_range)
            continue;

        delta_x = FixedToInteger_0808df1c(candidate->x - target->x);
        delta_z = FixedToInteger_0808df1c(candidate->z - target->z);
        distance = length_gateway(
            delta_x * delta_x + delta_z * delta_z);
        if ((candidate->flags & 0x10) != 0)
            distance = Func_080022ec(distance * 2, 3);
        if (distance >= nearest_distance)
            continue;

        bearing = (u16)Func_080044d0(
            candidate->z - target->z,
            candidate->x - target->x);
        angular_limit = distance > 0x13 ? 0x1000 : 0x1800;
        if (requested_type == 2)
            angular_limit = 0x2000;

        angular_delta = (s16)(bearing - forward);
        if (angular_delta < 0)
            angular_delta = -angular_delta;
        if (distance > 0x0b && angular_delta >= angular_limit)
            continue;

        nearest_id = candidate_id;
        nearest_distance = distance;
    }

    return nearest_id;
}
