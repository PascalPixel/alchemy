#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/fx/spawn_particle.h"
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

extern SceneParticle *Map_Run(s32);

extern SceneParticle *Map_Run2(s32);
extern SceneParticle *Map_Run3(s32);

void Colosso_SpawnPeriodicParticle(void)
{
    SceneParticle *particle;
    SceneParticle *source;
    s32 x;
    s32 y;
    s32 kind;
    s32 count;

    particle = Map_Run(0);
    count = *(s32 *)0x0200DB80 + 1;
    kind = 41;
    x = particle->x;
    y = particle->y;
    *(s32 *)0x0200DB80 = count;
    switch (Map_Apply(count, 180)) {
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
    particle = Map_Run2(kind);
    if (particle == 0) {
        return;
    }
    source = Map_Run3(0);
    if (source != 0) {
        Map_Place(kind, source->x, source->z);
    }
    Map_Apply2(Map_Check(kind), 0);
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
    Map_Apply3(kind, 0x0200D96C);
}
