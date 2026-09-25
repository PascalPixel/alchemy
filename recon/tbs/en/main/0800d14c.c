/* Draft, not exact: 20 differing halfwords, 440-byte candidate for the
   440-byte owner (2026-09-24). Residual: the reference keeps the distance
   in r5 (sharing the coarse and fine y delta's register) and the stopping
   distance in r1, where this candidate uses r6 and r5, and the coarse
   sum squares dz into r2 after dx; it also reloads 0x0300013c from the pool
   for the first divide where this candidate derives it as 0x03000118 + 36.
   Tried: sum orders, local order, a separate shift, an inline stopping
   distance helper, a function-pointer local. */

#include "OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

#define Iwram_Sqrt ((s32 (*)(s32))0x030001d8)
#define Iwram_DivQ16 ((s32 (*)(s32, s32))0x0300013c)

s32 FixedSqrt(s32 value);

void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 brake;
    u8 *facing;

    dx = (x - object->x) / 0x10000;
    dy = (y - object->y) / 0x10000;
    dz = (z - object->z) / 0x10000;
    dist = Iwram_Sqrt(dx * dx + dy * dy + dz * dz) << 16;
    if (dist < 0x100000) {
        dx = x - object->x;
        dy = y - object->y;
        dz = z - object->z;
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz));
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
        brake = Iwram_DivQ16(object->acceleration, Iwram_MulQ16(object->speed_limit, object->speed_limit));
        brake = Iwram_DivQ16(dist, dist > brake ? dist - brake / 2 : dist / 2);
        x = object->x + Iwram_MulQ16(x - object->x, brake);
        y = object->y + Iwram_MulQ16(y - object->y, brake);
        z = object->z + Iwram_MulQ16(z - object->z, brake);
    }
    object->target_x = x;
    object->target_y = y;
    object->target_z = z;
    dx = x - object->x;
    dy = y - object->y;
    dz = z - object->z;
    facing = &object->unknown_56[0];
    *facing = 16;
    if ((dx < 0 ? -dx : dx) < (dz < 0 ? -dz : dz)) {
        *facing = 18;
        dx = dz;
    }
    if (object->flags == 0) {
        if ((dx < 0 ? -dx : dx) < (dy < 0 ? -dy : dy))
            *facing = 17;
    }
}
