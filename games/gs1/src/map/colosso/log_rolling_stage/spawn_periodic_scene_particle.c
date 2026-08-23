#include "colosso_log_rolling_stage.h"

typedef struct SceneParticle {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler20[0x1c];
    s32 anchor_y;
    u8 filler40[0x15];
    u8 state;
    u8 filler56[0xe];
    s16 velocity_x;
    s16 velocity_y;
} SceneParticle;

extern SceneParticle *Func_02005de2(s32);
extern s32 Func_02005c1a(s32, s32);
extern SceneParticle *Func_02005e1a(s32);
extern SceneParticle *Func_02005e26(s32);
extern void Func_02005e84(s32, s32, s32);
extern s32 Func_02005e3a(s32);
extern void Func_02005d80(s32, s32);
extern void Func_02005e8a(s32, s32);

void ColossoLogRollingStage_SpawnPeriodicParticle(void)
{
    SceneParticle *particle;
    SceneParticle *source;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    particle = Func_02005de2(0);
    count = *(s32 *)0x0200DB80 + 1;
    kind = 41;
    x = particle->x;
    y = particle->y;
    *(s32 *)0x0200DB80 = count;
    switch (Func_02005c1a(count, 180)) {
    case 10:
        break;
    case 20:
        kind = 42;
        break;
    case 30:
        kind = 43;
        break;
    default:
        return;
    }
    particle = Func_02005e1a(kind);
    if (particle == 0) {
        return;
    }
    source = Func_02005e26(0);
    if (source != 0) {
        Func_02005e84(kind, source->x, source->z);
    }
    Func_02005d80(Func_02005e3a(kind), 0);
    particle->state = 0;
    particle->scale_x = 0x6666;
    particle->scale_y = 0x6666;
    {
        s32 t = 0x40000;
        particle->x = x + t;
        t += y;
        particle->y = t;
        particle->anchor_y = t;
    }
    particle->velocity_x = 25;
    particle->velocity_y = 128;
    Func_02005e8a(kind, 0x0200D96C);
}
