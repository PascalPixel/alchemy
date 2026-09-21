#include "TYPES.H"

struct RingOrigin {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
};

s32 Func_0800231c(s32);
#define Engine_MathCos Func_0800231c
s32 Func_08002322(s32);
#define Engine_MathSin Func_08002322
void Func_0809a484(s32, s32, s32, s32, s32, s32, s32, s32 *);
#define Effect_Spawn Func_0809a484
extern u8 Data_0809a65d;

void BattleFx_SpawnRadialParticleRing(struct RingOrigin *origin)
{
    s32 particle_config[10];
    s32 velocity[3];
    s32 sine;
    s32 angle;
    u32 i;

    particle_config[1] = 0;
    particle_config[9] = (s32)&Data_0809a65d;
    particle_config[2] = 0xCCCC;
    particle_config[3] = 0xCCCC;
    i = 0;
    do {
        angle = i << 12;
        velocity[0] = Engine_MathCos(angle) * 3 / 2;
        velocity[1] = 0;
        sine = Engine_MathSin(angle);
        velocity[2] = sine;
        Effect_Spawn(origin->x, origin->y, origin->z, velocity[0], velocity[1], sine, 0x01090001, particle_config);
        i++;
    } while (i <= 16);
}
