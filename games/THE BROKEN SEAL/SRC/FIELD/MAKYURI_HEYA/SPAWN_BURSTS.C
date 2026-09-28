#include "PROBE.H"

void SceneEffect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Actor_Get(0);
    s32 m = gFrameCount & 3;

    if (m == 0) {
        buf[1] = 7;
        if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + ((((u32)Engine_RandomNext() << 2) >> 16) << 16);
            s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

            Effect_Spawn(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = gFrameCount & 3;

    if (m == 0) {
        buf[1] = 7;
        if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

            Effect_Spawn(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}

void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + ((((u32)Engine_RandomNext() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + ((((u32)Engine_RandomNext() << 3) >> 16) << 16) + 0xfffc0000;

        Effect_Spawn(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}

void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Audio_PlayCue(0xd8);
    i = 0;
    do {
        if ((i & 1) != 0) {
            desc[1] = 7;
            if ((i & 2) != 0)
                desc[1] = 5;
            desc[2] = 0x9999;
            desc[3] = 0x9999;
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            Vector_AddPolarOffset((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);
                s32 b = z + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);

                Effect_Spawn(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        Task_Wait(2);
        i++;
    } while (i <= 11);
}
