#include "scene.h"
#include "colosso_log_rolling_stage.h"

/* map/locations/colosso/log_rolling_stage/event/run_if_ready.c */
void Colosso_RunSceneEventIfReady(void)
{
    if (Map_Check() == 0) {
        Map_Run();
    }
}

/* map/locations/colosso/log_rolling_stage/event/finish_or_cont.c */
void Colosso_FinishOrContinueSceneEvent(void)
{
    if (Map_Check() == 0) {
        Map_Run();
    } else {
        Map_unk2_4();
    }
}

/* map/locations/colosso/log_rolling_stage/event/get_state.c */
s32 Colosso_GetSceneEventState(void)
{
    return 0x0200D488;
}

/* map/locations/colosso/log_rolling_stage/fx/adv_particle.c */
typedef struct SceneParticle {
    u8 filler00[8];
    s32 x;
    s32 y;
    u8 filler10[8];
    s32 scale_x;
    s32 scale_y;
    u8 filler20[0x44];
    s16 velocity_x;
    s16 velocity_y;
} SceneParticle;

s32 Colosso_AdvanceParticleMotion(SceneParticle *particle)
{
    particle->x += particle->velocity_x << 8;
    particle->y += particle->velocity_y << 8;
    particle->scale_x += 0x666;
    particle->scale_y += 0x666;
    particle->velocity_x += 5;
    particle->velocity_y -= 1;
    return 0;
}

/* map/locations/colosso/log_rolling_stage/fx/spawn_particle.c */
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

extern SceneParticle *Map_unk2_4(s32);
extern SceneParticle *Map_unk3_4(s32);

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
    particle = Map_unk2_4(kind);
    if (particle == 0) {
        return;
    }
    source = Map_unk3_4(0);
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
