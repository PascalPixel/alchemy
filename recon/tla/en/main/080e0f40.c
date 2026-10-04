/* Ported from ☀️ games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/SPAWN_DESCENDING_ARC_PARTICLES.C. */
#include "RESOURCE.H"
#include "VRAM_BLOCK.H"
#include "TYPES.H"
#include "SCENE_IDS.H"

/* Drops the particle two pixels a frame beside its source, on the positive
   z side, while its sprite grows to full size; it vanishes ten pixels above
   its floor (four in the Clear scene). */
void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle)
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
    particle->scale_y = scale;
    particle->x = source->x;
    particle->y = y - 0x20000;
    particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}
