#include "TYPES.H"
extern u32 gFrameCount;

void Engine_AudioPlayCue();
u32 Engine_RandomNext();
void Effect_Spawn();

struct DustParams {
    s32 count;
    s32 kind;
    s32 spreadX;
    s32 spreadY;
    s32 growX;
    s32 growY;
};

/* Every fourth frame, blow a puff of dust across the cave mouth. */
void ImiruFuchin_BlowCaveMouthDust(void)
{
    struct DustParams params;
    struct DustParams *p;
    s32 phase;
    s32 dx;
    s32 dy;

    phase = *(volatile s32 *)&gFrameCount & 3;
    if (phase != 0)
        return;
    p = &params;
    p->kind = 10;
    p->spreadX = 0x8000;
    p->spreadY = 0x8000;
    p->growX = 0x1cccc;
    p->growY = 0x1cccc;
    if ((*(volatile s32 *)&gFrameCount & 7) == 0)
        Engine_AudioPlayCue(136);
    dx = -0x10000 - (((Engine_RandomNext() << 1) >> 16) << 16);
    dy = -(s32)(((Engine_RandomNext() * 3) >> 16) * 0x3333);
    Effect_Spawn(0x1340000, 0x400000, 0xde0000, dx, dy, phase, 0xd0001, p);
}
