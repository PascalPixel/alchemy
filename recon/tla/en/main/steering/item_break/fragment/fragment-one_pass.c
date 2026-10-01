/* TLA-EN BattleFx_UpdateItemBreakFragment: 200 emitted bytes versus the complete
   200-byte current native template; 7 differing byte positions after excluding
   the template's external relocation words. Calls and address pools are not
   linked in this comparison. This trial is uncredited and is not an adoption. */

#include "FIXED_MATH.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

struct ItemBreakFragmentPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ItemBreakFragmentSource {
    u8 reserved_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[70];
    u8 flag_5a;
    u8 reserved_5b[13];
    struct ItemBreakFragmentSource *target;
};

struct ItemBreakFragmentObject {
    u8 reserved_00[72];
    s32 field_48;
    u8 reserved_4c[9];
    u8 mode_55;
    u8 reserved_56[8];
    u16 field_5e;
};

extern s32 ArcTan2(s32, s32);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetMode(struct ItemBreakFragmentObject *, s32);
extern void Object_SetCallback(struct ItemBreakFragmentObject *, const void *);

void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    struct ItemBreakFragmentSource *target;
    struct ItemBreakFragmentPosition position;
    struct ItemBreakFragmentObject *object;
    s32 steering_delta;
    s32 drift_magnitude;

    target = source->target;
    if (target != 0) {
        s32 dx = target->x - source->x;
        s32 dz = target->z - source->z;

        if (dx != 0 || dz != 0) {
            /* FAKEMATCH: a measured one-pass boundary preserves the clamp-threshold setup before the signed difference. */
            do {
                steering_delta = (s16)(ArcTan2(dz, dx) - source->angle);
            } while (0);
            if (steering_delta > 0x1000)
                steering_delta = 0x1000;
            if (steering_delta < -0x1000)
                steering_delta = -0x1000;
            source->angle += steering_delta;
        }
        source->flag_5a = 0;
    }

    position.x = source->x;
    position.y = source->y - (Random16() << 4) - 0x80000;
    position.z = source->z;
    drift_magnitude = Random16() * 3;
    drift_magnitude <<= 4;
    Vector_AddPolarOffset(drift_magnitude, Random16(), &position);

    object = Object_Spawn(0x2a1, position.x, position.y, position.z);
    if (object != 0) {
        object->mode_55 = 2;
        object->field_48 = 0x1999;
        Object_SetMode(object, 0);
        object->field_5e = 12;
        Object_SetCallback(object, BattleFx_CommonParticleScript);
    }
}
