#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

s32 FixedSqrt(s32 value);

/*
 * Aims an object at a point. A point closer than one unit is taken at once;
 * otherwise, unless the object moves without easing, the target is pulled
 * in to where the object must start braking at its speed and acceleration.
 * The dominant axis of the move is kept at +0x56 (16 x, 17 y, 18 z).
 */
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    s32 dx;
    s32 dist;
    s32 dz;
    s32 total;
    u8 *axis;

    dx = (x - object->x) / 0x10000;
    dist = (y - object->y) / 0x10000;
    dz = (z - object->z) / 0x10000;
    total = dz * dz;
    dist = Iwram_Sqrt(dx * dx + dist * dist + total) << 16;
    if (dist < 0x100000) {
        dx = x - object->x;
        dist = y - object->y;
        dz = z - object->z;
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dist, dist) + Iwram_MulQ16(dz, dz));
    }
    if (dist < 0x10000) {
        object->x = x;
        object->y = y;
        object->z = z;
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        return;
    }
    if (object->unknown_56[2] == 0) {
        s32 brake;

        brake = Iwram_RatioMulQ14(object->acceleration,
                             Iwram_MulQ16(object->speed_limit, object->speed_limit));
        if (dist > brake)
            brake = dist - brake / 2;
        else
            brake = dist / 2;
        dist = Iwram_RatioMulQ14(dist, brake);
        x = object->x + Iwram_MulQ16(x - object->x, dist);
        y = object->y + Iwram_MulQ16(y - object->y, dist);
        z = object->z + Iwram_MulQ16(z - object->z, dist);
    }
    object->target_x = x;
    object->target_y = y;
    object->target_z = z;
    dx = x - object->x;
    dist = y - object->y;
    dz = z - object->z;
    axis = &object->unknown_56[0];
    *axis = 16;
    if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
        *axis = 18;
        dx = dz;
    }
    if (object->flags == 0) {
        if (dx < 0)
            dx = -dx;
        if (dx < (dist < 0 ? -dist : dist))
            *axis = 17;
    }
}
