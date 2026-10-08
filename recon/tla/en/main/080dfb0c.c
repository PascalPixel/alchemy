#include "types.h"
#include "motobj.h"
#include "field_scene.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "system.h"

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
