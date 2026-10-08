#include "resource.h"
#include "vram_block.h"
#include "types.h"
#include "scene_ids.h"

void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle)
{
    struct ArcParticle *source;
    s32 y;
    s32 limit;
    s32 scale;

    limit = particle->floor + 0xA0000;
    source = particle->source;
    if (gGameState.scene == (s32)&SceneId_Clear)
        limit = particle->floor + 0x40000;

    y = particle->y;
    if (y <= limit) {
        Object_Destroy();
        return;
    }

    scale = particle->scale_x + 0xC00;
    if (scale > 0x10000)
        scale = 0x10000;
    particle->scale_x = scale;
    particle->scale_y = -scale;
    particle->x = source->x;
    particle->y = y - 0x20000;
    particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}
