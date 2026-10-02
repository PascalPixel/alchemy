#include "EDITION.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "FIXED_MATH.H"
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

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetMode(struct ItemBreakFragmentObject *, s32);

void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source)
{
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
#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
            s32 angle = ArcTan2(dz, dx);
            s32 current = source->angle;
            s32 max_turn = 128;

            /* FAKEMATCH: the zero-instruction dependency keeps the clamp byte seed before the signed angle difference. */
            asm("" : "+r"(max_turn), "+r"(angle) : "r"(current));
            steering_delta = (s16)(angle - current);
            max_turn <<= 5;
            if (steering_delta > max_turn)
                steering_delta = max_turn;
#else
            steering_delta = (s16)(ArcTan2(dz, dx) - source->angle);
            if (steering_delta > 0x1000)
                steering_delta = 0x1000;
#endif
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

#if defined(TLA_EDITION_JA) || defined(TLA_EDITION_EN) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
    object = Object_Spawn(0x2a1, position.x, position.y, position.z);
#else
    object = Object_Spawn(0x11d, position.x, position.y, position.z);
#endif
    if (object != 0) {
        object->mode_55 = 2;
        object->field_48 = 0x1999;
        Object_SetMode(object, 0);
        object->field_5e = 12;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
}
