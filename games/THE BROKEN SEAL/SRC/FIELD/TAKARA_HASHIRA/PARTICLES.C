#include "HASHIRA.H"

void SceneEffect_SpawnRandomizedParticle(void)
{
    u8 descriptor[40];
    u8 *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((Data_03001e40 & 2) != 0)
        return;
    if ((Data_03001e40 & 7) == 0)
        Audio_PlayCue(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Random_Next();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = 0x020093b1;

    draw = (u32)Random_Next();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Random_Next();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Effect_Spawn(0x01440000, 0x00300000, 0x00e40000, spread,
                  secondary, 0, 0x014d0000, d);
}

s32 SceneEffect_SpawnRandomEffectEveryEightFrames(u8 *actor)
{
    u8 desc[40];
    u8 *p;
    u32 phase = (u32)Data_03001e40 & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = desc;
    *(s32 *)(p + 4) = 7;
    *(s32 *)(p + 8) = 0xb333;
    *(s32 *)(p + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Random_Next() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Random_Next() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Random_Next() * 17) >> 16) - 8) << 16);
    scale = Math_Divide((s32)(((u32)Random_Next() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Effect_Spawn(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}
