/* Near miss: ☀️'s BattleFx_SpawnRadialParticleRing, differ 2 under the
   ARM9TDMI schedule. The native stores the sine into the velocity before
   copying the origin pointer out of sl; this draft copies it first.
   Assignment chains, reading the velocity back for the call, local
   coordinates and declaration order did not move it. ADVANCE_MOTION.C
   before it is being repaired elsewhere; adopt into a module of its own
   after the padding listing at 080dfb0a. */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"

void Effect_AdvanceMotion(struct FieldEffect *effect);

void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin)
{
    struct EffectOptions options;
    s32 velocity[3];
    s32 sine;
    s32 angle;
    u32 i;

    options.palette = 0;
    options.update = Effect_AdvanceMotion;
    options.start_scale_x = 0xCCCC;
    options.start_scale_y = 0xCCCC;
    i = 0;
    do {
        angle = i << 12;
        velocity[0] = Trig_Cos(angle) * 3 / 2;
        velocity[1] = 0;
        sine = Trig_Sin(angle);
        velocity[2] = sine;
        Effect_SpawnResident(origin->x, origin->y, origin->z, velocity[0], velocity[1], sine, 0x01090001, &options);
        i++;
    } while (i <= 16);
}
