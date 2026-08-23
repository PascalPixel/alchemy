#include "colosso_log_rolling_stage.h"

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

s32 ColossoLogRollingStage_AdvanceParticleMotion(SceneParticle *particle)
{
    particle->x += particle->velocity_x << 8;
    particle->y += particle->velocity_y << 8;
    particle->scale_x += 0x666;
    particle->scale_y += 0x666;
    particle->velocity_x += 5;
    particle->velocity_y -= 1;
    return 0;
}
