#include "TYPES.H"

void Engine_MapCopyCellsTo(s32 sx, s32 sy, s32 dx, s32 dy, s32 w, s32 h);
u32 Engine_RandomNext(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_EventWait(s32 frames);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[24];
};

/* Open the three-step stair: each step throws dust along both of its edges. */
void MakyuriHeya_Func02003948(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    u32 i;
    u32 j;
    s32 z;

    Engine_MapCopyCellsTo(76, 61, 74, 38, 1, 1);
    p = &params;
    p->kind = 5;
    p->spread = 0x8000;
    p->rise = 0x8000;
    for (i = 0; i <= 2; i++) {
        z = -0x20000;
        for (j = 1; j <= 7; j++) {
            if (j & 1) {
                if (j & 2) {
                    Effect_Spawn((105 - ((Engine_RandomNext() * 5) >> 16)) << 16, 0, z - (i << 19) + 0x22e0000, 0, 0, -0x4000, 0x90000, p);
                } else {
                    Effect_Spawn((((i << 2) + j) << 17) + 0xb70000, 0, (0x26c - ((Engine_RandomNext() * 5) >> 16)) << 16, 0x4000, 0, 0, 0x90000, p);
                }
                Engine_EventWait(1);
            }
            z += -0x20000;
        }
        Engine_MapCopyCellsTo(71, 59, 70, 34 - i, 1, 1);
        Engine_MapCopyCellsTo(71, 59, i + 75, 38, 1, 1);
    }
}
