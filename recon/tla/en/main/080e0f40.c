/* Near miss, differ 4 (two reordered rows): the twin of The Broken Seal's
 * BattleFx_UpdateDescendingParticlePositiveArc. The listing loads particle->floor
 * before it builds 0x1fa, and loads the 0xfffe0000 literal before source->x; this
 * source schedules both loads one step later. A party pointer followed by a
 * one-pass block fixes the first row (differ 2); no statement order or barrier
 * in the tail fixed the second. Both rows are the order the scheduler gives
 * when a load's result is one cycle further from its use. 080e0fb0 is the
 * same function with the signs turned. */
#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

struct ArcParticle {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 floor;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x48];
    struct ArcParticle *source;
};

void Object_Destroy(struct ArcParticle *particle);

void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle)
{
    struct ArcParticle *source;
    s32 y;
    s32 limit;
    s32 scale;

    limit = particle->floor + 0xA0000;
    source = particle->source;
    if (gPartyState.map == (s32)&SceneId_Ov64a)
        limit = particle->floor + 0x40000;

    y = particle->y;
    if (y <= limit) {
        Object_Destroy(particle);
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
