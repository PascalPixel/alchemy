#include "TYPES.H"
#include "FIELD_EFFECT.H"

union OrbitObject {
    union FieldObject object;
    struct {
        u8 unknown_00[0x64];
        s16 angle;
        s16 paused;
    } orbit;
};

LAYOUT_OFFSET_GUARD(OrbitObject_Angle, union OrbitObject, orbit.angle, 0x64);
LAYOUT_OFFSET_GUARD(OrbitObject_Paused, union OrbitObject, orbit.paused, 0x66);

extern u32 gWorldMapMotionFlags;
void Func_0200c25c(union OrbitObject *object, s32 animation);
s32 Func_0200c1a4(s32 angle);
void Func_0200c1ac(s32 radius, s32 angle, void *position);

void WorldMap_UpdateOrbitActor(union OrbitObject *object)
{
    s32 wave;

    if (gWorldMapMotionFlags & 2)
        Func_0200c25c(object, 10);
    else
        Func_0200c25c(object, 7);
    if (object->orbit.paused == 0) {
        object->object.actor.x.fixed = 0x15d00000;
        wave = Func_0200c1a4(object->orbit.angle * 8);
        /* The reference evaluates this Q16 product in IWRAM via an ip return.
         * This draft expresses its arithmetic, not that assembly call ABI. */
        object->object.actor.y.fixed =
            (s32)(((s64)wave * 0x40000) >> 16) + 0x100000;
        object->object.actor.z.fixed = 0x05300000;
        Func_0200c1ac(0x100000, object->orbit.angle, &object->object.actor.x);
        object->object.actor.facing = object->orbit.angle + 0x4000;
        object->orbit.angle += 0x400;
    }
}
