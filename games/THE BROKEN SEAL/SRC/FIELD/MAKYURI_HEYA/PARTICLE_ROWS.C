#include "PROBE.H"

void SceneEffect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Map_CopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Map_CopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Map_CopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Audio_PlayCue(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Effect_Spawn((0x319 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Effect_Spawn((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Engine_RandomNext)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Effect_Spawn(0x3380000 - (i << 17) - (j << 19), 0,
                                  ((((u32)Engine_RandomNext() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Event_Wait(1);
            }
        }
        if (mode == 0)
            Map_CopyCellsTo(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Map_CopyCellsTo(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Map_CopyCellsTo(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}
